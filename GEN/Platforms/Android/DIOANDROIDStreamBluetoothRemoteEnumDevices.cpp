//------------------------------------------------------------------------------------------
//	DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES.CPP
//	
//	ANDROID Data IO Stream Bluetooth Remote Enum Devices class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <syslog.h>
#include <signal.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <netinet/in.h>

#include "XFactory.h"
#include "XVector.h"
#include "XString.h"
#include "XThread.h"
#include "DIOStream.h"
#include "DIOStreamDeviceBluetooth.h"

#include "DIOANDROIDStreamBluetoothRemoteEnumDevices.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*  SDP_lastservice = NULL;
static char													 UUID_str[MAX_LEN_UUID_STR];
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


extern "C"
{
	static inline int		sdp_is_local_nonblock			(const bdaddr_t* device);
	sdp_session_t*			sdp_connect_nonblock			(const bdaddr_t* src, const bdaddr_t* dst, uint32_t flags);
	
	void								print_service_class				(void* value, void* userData);
	void								print_service_desc				(void* value, void* user);
	void								print_access_protos				(void* value, void* userData);
	void								print_service_attr				(sdp_record_t* rec);
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIPREMOTEENUMDEVICES::DIOANDROIDSTREAMTCPIPREMOTEENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 19:00:01
//	
//	@return 			

//  @param				publisher : 
*/
/*-----------------------------------------------------------------*/
DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES() : DIOSTREAMBLUETOOTHREMOTEENUMDEVICES() , XFSMACHINE(0)
{
	Clean();

	AddState(	DIOANDROIDBTENUMFSMSTATE_NONE								, 								
				 		DIOANDROIDBTENUMFSMEVENT_SEARCHMAC					, DIOANDROIDBTENUMFSMSTATE_SEARCHMAC				, 
						DIOANDROIDBTENUMFSMEVENT_SEARCHEND					, DIOANDROIDBTENUMFSMSTATE_SEARCHEND				,								
						EVENTDEFEND);

	AddState(	DIOANDROIDBTENUMFSMSTATE_SEARCHMAC					, 											
		 				DIOANDROIDBTENUMFSMEVENT_SEARCHSERVICES			, DIOANDROIDBTENUMFSMSTATE_SEARCHSERVICES		, 	
						DIOANDROIDBTENUMFSMEVENT_SEARCHEND					, DIOANDROIDBTENUMFSMSTATE_SEARCHEND				,															
						EVENTDEFEND);

	AddState(	DIOANDROIDBTENUMFSMSTATE_SEARCHSERVICES			, 								
						DIOANDROIDBTENUMFSMEVENT_SEARCHEND					, DIOANDROIDBTENUMFSMSTATE_SEARCHEND				,																
						EVENTDEFEND);

	AddState(	DIOANDROIDBTENUMFSMSTATE_SEARCHEND					,								
						DIOANDROIDBTENUMFSMEVENT_SEARCHMAC					, DIOANDROIDBTENUMFSMSTATE_SEARCHMAC				, 																							
						EVENTDEFEND);

	threadenumdevices = CREATEXTHREAD(__L("DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
	if(threadenumdevices) threadenumdevices->Ini();
}
	


/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIPREMOTEENUMDEVICES::~DIOANDROIDSTREAMTCPIPREMOTEENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 19:00:12
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::~DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES()
{
	StopSearch(true);

	if(threadenumdevices) 
		{
			threadenumdevices->End();
			DELETEXTHREAD(threadenumdevices);
		}
  
	Clean();
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIPREMOTEENUMDEVICES::Search
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 19:00:27
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::Search()
{		
	if(!threadenumdevices)			 return false;
	
	DelAllDevices();
	
	SetEvent(DIOANDROIDBTENUMFSMEVENT_SEARCHMAC);	
	
	threadenumdevices->Run(true);

	if(xtimerout)
		{
			xtimerout->Reset();
			while(GetCurrentState() == DIOANDROIDBTENUMFSMSTATE_NONE)
				{
					if(xtimerout) xtimerout->SleepMilliSeconds(10);

					if(xtimerout->GetMeasureSeconds() >= 5) 
						{
							return false;
						}
				}
		}


 	return true;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::StopSearch
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2013 23:41:35
//	
//	@return 			bool : 
//	@param				waitend : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::StopSearch(bool waitend)
{
	if(!IsSearching()) return false;

	SetEvent(DIOANDROIDBTENUMFSMEVENT_SEARCHEND);

	if(waitend)
		{
			while(GetCurrentState()!=DIOANDROIDBTENUMFSMEVENT_SEARCHEND)
				{
					if(xtimerout) xtimerout->SleepMilliSeconds(10);
				}
		}

	if(threadenumdevices) threadenumdevices->Run(false);
	
	sleep(1000);

	return true;
};




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::IsSearching
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2013 23:41:40
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::IsSearching()
{ 
	if(!threadenumdevices) return false;

	if(!threadenumdevices->IsRunning()) return false;

	if((GetCurrentState() == DIOANDROIDBTENUMFSMSTATE_SEARCHMAC) || (GetCurrentState() == DIOANDROIDBTENUMFSMSTATE_SEARCHSERVICES)) return true;

	return false;	  
};




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2013 23:25:03
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean()
{
	threadenumdevices	= NULL;
	
}


/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::GetAddrFromLocalDevice
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2013 20:54:25
//	
//	@return 			bool : 
//	@param				localdeviceindex : 
//  @param				btaddr : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::GetAddrFromLocalDevice(int localdeviceindex, char* btaddr)
{
	struct hci_dev_info* di;

	di = new hci_dev_info();
	if(!di) return false;
		
	int error = hci_devinfo(localdeviceindex, di);
	if(error>=0) ba2str(&di->bdaddr, btaddr);
						
	delete di;
		
	if(error<0) return false;
	
	return true;
}




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::IsReadyConnect
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2013 2:41:57
//	
//	@return 			int : 
//	@param				socket : 
*/
/*-----------------------------------------------------------------*/
int DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::IsReadyConnect(int socket)
{
	struct timeval	tv;
	int							rc;
	fd_set					fdr;
	fd_set					fdw;
	fd_set					fds;

	if(socket==-1) return -1;
								
	FD_ZERO(&fdr);
	FD_ZERO(&fdw);
	FD_ZERO(&fds);

	FD_SET((unsigned int)socket,&fdr);
  FD_SET((unsigned int)socket,&fdw);
	FD_SET((unsigned int)socket,&fds);

	tv.tv_sec  = 0;
	tv.tv_usec = 100;
								
	rc = select(socket+1, &fdr, &fdw, &fds, &tv);
	if(rc==-1) return -1;	
	if(rc== 2) return -1;

  int status1 = FD_ISSET(socket,&fdr) ? 1 : 0;
	int status2 = FD_ISSET(socket,&fdw) ? 1 : 0;
	int status3 = FD_ISSET(socket,&fds) ? 1 : 0;

	if(status3)	return -1;	
	if(((status1) || (status2)) && (rc==1))  return  1;
																																													
	return 0;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevices
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/05/2013 17:36:53
//	
//	@return 			bool : 
//	@param				devices : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevices(XVECTOR<DIOSTREAMDEVICE*>* devices)
{
	inquiry_info* info = NULL; 	
	int						maxanswers     = 255;	 // Max answers																											 																													 																											     
	int						timeout				 = 6;    // 8;    // El tiempo de inquiry por dispositivo es de 1.28x8=10.24 secs/dispositivo 
	int						nanswers;		
	bool					status				 = false;
											
	info = (inquiry_info*)new XBYTE[maxanswers * sizeof(inquiry_info)];
	if(!info) return false;	
	
  int flags = IREQ_CACHE_FLUSH;	
	int retry = 30;
	
	nanswers = hci_inquiry(0, timeout, maxanswers, NULL, &info, flags); 

	for(int c=0;c<nanswers;c++)
		{	
			inquiry_info*  infodev = (info+c);																																	
			char					 addr[32];
											          																							
			DIOSTREAMDEVICEBLUETOOTH* device = new DIOSTREAMDEVICEBLUETOOTH();
			if(device) 
				{		
					uint32_t devclass;	

					ba2str(&infodev->bdaddr, addr); 
					device->GetMAC()->Set(addr);
					
					devclass = (uint32_t)(infodev->dev_class[2]) << 16 |
 										 (uint32_t)(infodev->dev_class[1]) <<  8 |
										 (uint32_t)(infodev->dev_class[0]);
					
					device->GetServiceClass()->SetData1((XDWORD)devclass);

					ScanDevicesName(device);
					
					devices->Add(device);									
				}
		
			status = true;
		}
	
	delete [] info;
	
	return status;
}




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevicesName
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2013 0:52:20
//	
//	@return 			bool : 
//	@param				device : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevicesName(DIOSTREAMDEVICEBLUETOOTH* device)
{
	if(!device)	return false;	

	bdaddr_t bdaddr;
	XSTRING	 MACstring;
	bool		 status = false;
	
	char* name = new char[_MAXSTR];							
	if(!name)  return false;

	memset(name,0,_MAXSTR);															
	memset(&bdaddr,0,sizeof(bdaddr_t));

	device->GetMAC()->GetXString(MACstring);
	
	XSTRING_CREATEOEM(MACstring, charOEM)	
	str2ba(charOEM, &bdaddr);
	XSTRING_DELETEOEM(charOEM)	

	int handlesocket = hci_open_dev(0);
	if(handlesocket>=0) 
		{											
			if(hci_read_remote_name(handlesocket, &bdaddr, _MAXSTR, name, 0)>=0)
				{								
					device->GetName()->ConvertFromUTF8((XBYTE*)name, strlen(name));									
					status = true;
				}	

		  close(handlesocket);			
		}

	delete [] name;
						
	return status;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevicesServices
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2013 1:09:55
//	
//	@return 			bool : 
//	@param				devices : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDevicesServices(XVECTOR<DIOSTREAMDEVICE*>* devices)
{
	DIOSTREAMDEVICEBLUETOOTH* device;
	int												index = 0;

	do{	device = (DIOSTREAMDEVICEBLUETOOTH*)(devices->Get(index));						
			if(!device)	break; 			
																  																	   											
			struct search_context context;
						
			uint8_t			protocol[16] = { 0x00, 0x00, 0x10 ,0x02, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };  // Public	
	 	//uint8_t			protocol[16] = { 0x00, 0x00, 0x01 ,0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };  // L2CAP_PROTOCOL_UUID

			
			memset(&context, '\0', sizeof(struct search_context));		
				
			//		
		
			//sdp_uuid16_create(&protocol, PUBLIC_BROWSE_GROUP);
																		
			sdp_uuid128_create(&(context.group), &protocol);	
			
							
		  if(!ScanDeviceServices(device, &context))
				{
											
				}		
										
			index++;

		} while(device);																									  			
	
	return true;																												
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDeviceServices
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2013 1:10:54
//	
//	@return 			bool : 
//	@param				device : 
//  @param				index : 
//  @param				context : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ScanDeviceServices(DIOSTREAMDEVICEBLUETOOTH* device, struct search_context* context)
{
	if(!device)	return false;
	
	bdaddr_t 				origin;	
	bdaddr_t 				target;	
	sdp_list_t*			attrid;
	sdp_list_t*			search;
	sdp_list_t*			seq;
	sdp_list_t*			next;
	uint32_t				range = 0x0000ffff;
	sdp_session_t*	session;
	char						addrlocaldevice[DIOMAC_MAXSIZESTR];	
  int							timeout		= 5*100; //seconds 


	if(!GetAddrFromLocalDevice(0, addrlocaldevice)) return false;

	XSTRING  MACstring;
	
	device->GetMAC()->GetXString(MACstring);

	str2ba(addrlocaldevice		, &origin);	
	
	XSTRING_CREATEOEM(MACstring, charOEM)	
	str2ba(charOEM , &target);
	XSTRING_DELETEOEM(charOEM)	

	// ----------------------------------------------------------------------
  // Special version Blocking	
	session = sdp_connect(&origin, &target, SDP_RETRY_IF_BUSY);
	if(!session) return false;
			
	attrid = sdp_list_append(0, &range);
	search = sdp_list_append(0, &context->group);

	if(sdp_service_search_attr_req(session, search, SDP_ATTR_REQ_RANGE, attrid, &seq)) 
		{			
			sdp_close(session);
			return false;
		}

	sdp_list_free(attrid, 0);
	sdp_list_free(search, 0);

	for(; seq; seq = next) 
		{	
			struct search_context		sub_context;
			size_t									_rec = (size_t)seq->data;
			sdp_record_t*						rec	 = (sdp_record_t *)_rec;

			if(_rec && ((int)_rec!=-1))
				{					
					DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service = new DIOSTREAMDEVICEBLUETOOTHSDPSERVICE();
					if(service) 
						{							
							sdp_data_t* data		 = 0;
							sdp_list_t* list		 = 0;
							sdp_list_t* proto		 = 0;
							int					sizedata = 0;             
							char				str[128];

							service->SetRecHandle(rec->handle);
														
							memset(str, 0, 128);		sdp_get_service_name (rec, str , 128-1);			service->GetName()->Set(str);
							memset(str, 0, 128);		sdp_get_service_desc (rec, str , 128-1);			service->GetDescription()->Set(str);
							memset(str, 0, 128);		sdp_get_provider_name(rec, str , 128-1);			service->GetProvider()->Set(str);							
															
							SDP_lastservice = service;
							
							if(sdp_get_service_classes(rec, &list) == 0) 
								{
									sdp_list_foreach(list, print_service_class, 0);
									sdp_list_free(list, free);
								}
										
							if(sdp_get_access_protos(rec, &proto) == 0) 
								{
									sdp_list_foreach(proto, print_access_protos, 0);
									sdp_list_foreach(proto, (sdp_list_func_t)sdp_list_free, 0);
									sdp_list_free(proto, 0);
								}
							
							SDP_lastservice = NULL;
							if(service->GetID())
										device->Service_Add(service);					
								else delete service;
						}
					
					if(sdp_get_group_id(rec, &sub_context.group) != -1) 
						{					
							memcpy(&sub_context, context, sizeof(struct search_context));					
							if(sub_context.group.value.uuid16!= context->group.value.uuid16) ScanDeviceServices(device, &sub_context);
						}
				}

			next = seq->next;
			free(seq);

			sdp_record_free(rec);			
		}

	sdp_close(session);

	return true;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ThreadEnumDevices
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/05/2013 17:02:14
//	
//	@return 			void : 
//	@param				param : 
*/
/*-----------------------------------------------------------------*/
void DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
{	
	DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES* enumeration = (DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES*)param;
	if(!enumeration) return;
	
	if(enumeration->GetEvent()==DIOANDROIDBTENUMFSMEVENT_NONE) // No hay nuevos Eventos
		{			
			switch(enumeration->GetCurrentState())
				{
					case DIOANDROIDBTENUMFSMSTATE_NONE						:	break;

					case DIOANDROIDBTENUMFSMSTATE_SEARCHMAC			  : enumeration->SetEvent(DIOANDROIDBTENUMFSMEVENT_SEARCHSERVICES);																																																																																			
																												break;
 
					case DIOANDROIDBTENUMFSMSTATE_SEARCHSERVICES	: enumeration->SetEvent(DIOANDROIDBTENUMFSMEVENT_SEARCHEND);																					
																												break;
			
					case DIOANDROIDBTENUMFSMSTATE_SEARCHEND				: break;																																																																		
							
																								 
				}
		}
	 else
		{
			if(enumeration->GetEvent()<DIOANDROIDBTENUM_LASTEVENT)
				{
					enumeration->CheckTransition();

					switch(enumeration->GetCurrentState())
						{
							case DIOANDROIDBTENUMFSMSTATE_NONE						: break;

							case DIOANDROIDBTENUMFSMSTATE_SEARCHMAC			  : enumeration->ScanDevices(&enumeration->devices);																														
																														break;
																											
							case DIOANDROIDBTENUMFSMSTATE_SEARCHSERVICES	: enumeration->ScanDevicesServices(&enumeration->devices);																												
																														break;
																																							
							case DIOANDROIDBTENUMFSMSTATE_SEARCHEND				: break;
					 }
				}
		}

}



extern "C"
{

	/*-------------------------------------------------------------------
	//  print_service_class
	*/ 
	/**
	//	
	//	
	//	@author				Abraham J. Velez
	//	@version			19/05/2013 2:29:21
	//	
	//	@return 			void : 
	//	@param				value : 
	//  @param				userData : 
	*/
	/*-----------------------------------------------------------------*/
	void print_service_class(void* value, void* userData)
	{	
		if(!SDP_lastservice) return;

		//XDEBUG_PRINTCOLOR(4, __L("print_service_class"));

		uuid_t* uuid = (uuid_t*)value;
		sdp_uuid2strn(uuid, UUID_str, MAX_LEN_UUID_STR);

		XDWORD ID = 0;

		sscanf(UUID_str,"%x",&ID);
		SDP_lastservice->SetID(ID);
	}



	/*-------------------------------------------------------------------
	//  print_service_desc
	*/ 
	/**
	//	
	//	
	//	@author				Abraham J. Velez
	//	@version			19/05/2013 2:29:13
	//	
	//	@return 			void : 
	//	@param				value : 
	//  @param				user : 
	*/
	/*-----------------------------------------------------------------*/
	void print_service_desc(void* value, void* user)
	{
		if(!SDP_lastservice) return;

		char				str[MAX_LEN_PROTOCOL_UUID_STR];
		sdp_data_t* p = (sdp_data_t*)value;	
		int proto			= 0;
		int i					= 0;

		DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL* protocol = SDP_lastservice->GetProtocol(SDP_lastservice->GetNProtocols());
		if(!protocol) return;
					
		for(;p; p = p->next, i++) 
			{				
				switch(p->dtd) 
					{
						case SDP_UUID16		:
						case SDP_UUID32		:
						case SDP_UUID128	:	proto = sdp_uuid_to_proto(&p->val.uuid);																
																protocol->SetType(proto);
																break;

						case SDP_UINT8		:	if(proto == RFCOMM_UUID) protocol->SetChannel(p->val.uint8);
											 					break;

											default :	XDEBUG_PRINTCOLOR(1, __L("[type unknown]"));
																break;
					
					}
			 }
		
		XDEBUG_PRINTCOLOR(4, __L("Protocol: %08X %08X %d"), SDP_lastservice->GetID(), protocol->GetType(), protocol->GetChannel());

		SDP_lastservice->SetNProtocols(SDP_lastservice->GetNProtocols()+1);	
	}




	/*-------------------------------------------------------------------
	//  print_access_protos
	*/ 
	/**
	//	
	//	
	//	@author				Abraham J. Velez
	//	@version			19/05/2013 2:29:07
	//	
	//	@return 			void : 
	//	@param				value : 
	//  @param				userData : 
	*/
	/*-----------------------------------------------------------------*/
	void print_access_protos(void* value, void* userData)
	{
		//XDEBUG_PRINTCOLOR(4, __L("print_access_protos"));

		sdp_list_t* protDescSeq = (sdp_list_t*)value;
		sdp_list_foreach(protDescSeq, print_service_desc, 0);
	}


}


#endif