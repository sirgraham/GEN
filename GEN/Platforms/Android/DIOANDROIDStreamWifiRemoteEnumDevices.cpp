//------------------------------------------------------------------------------------------
//	DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES.CPP
//	
//	ANDROID Data IO Stream Wifi Remote Enum Devices class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#if defined(DIO_ACTIVE) && defined(DIOWIFI_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <syslog.h>
#include <signal.h>
#include <time.h>
//#include <iwlib.h>


#include "XVector.h"
#include "XString.h"
#include "XDebug.h"
#include "XANDROIDThread.h"

#include "DIOStream.h"
#include "DIOStreamDeviceWifi.h"

#include "DIOANDROIDStreamWifiRemoteEnumDevices.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------





/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2014 19:22:27
//	
//	@return 			

 
*/
/*-----------------------------------------------------------------*/
DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES() : DIOSTREAMWIFIREMOTEENUMDEVICES() , XFSMACHINE(0)
{
	Clean();

	AddState(	DIOANDROIDWIFIENUMFSMSTATE_NONE								, 								
				 		DIOANDROIDWIFIENUMFSMEVENT_SEARCH							, DIOANDROIDWIFIENUMFSMSTATE_SEARCH						, 
						DIOANDROIDWIFIENUMFSMEVENT_SEARCHEND					, DIOANDROIDWIFIENUMFSMSTATE_SEARCHEND				,								
						EVENTDEFEND);

	AddState(	DIOANDROIDWIFIENUMFSMSTATE_SEARCH							, 													 				
						DIOANDROIDWIFIENUMFSMEVENT_SEARCHEND					, DIOANDROIDWIFIENUMFSMSTATE_SEARCHEND				,															
						EVENTDEFEND);
	
	AddState(	DIOANDROIDWIFIENUMFSMSTATE_SEARCHEND					,								
						DIOANDROIDWIFIENUMFSMEVENT_SEARCH							, DIOANDROIDWIFIENUMFSMSTATE_SEARCH						, 																							
						EVENTDEFEND);

	threadenumdevices = CREATEXTHREAD(__L("DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
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
DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::~DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES()
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
//	@author				Abraham J. Velezifconfig
//	@version			28/04/2013 19:00:27
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::Search()
{		
	if(!threadenumdevices)			 return false;
	
	DelAllDevices();
	
	SetEvent(DIOANDROIDWIFIENUMFSMEVENT_SEARCH);	
	
	threadenumdevices->Run(true);

	sleep(1);

 	return true;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::StopSearch
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
bool DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::StopSearch(bool waitend)
{
	if(IsSearching()) 
		{
			SetEvent(DIOANDROIDWIFIENUMFSMEVENT_SEARCHEND);

			if(waitend)
				{
					while(GetCurrentState()!=DIOANDROIDWIFIENUMFSMEVENT_SEARCHEND)
						{
							usleep(1000);
						}
				}
		}

	if(threadenumdevices) threadenumdevices->Run(false);
	
	usleep(1000);

	return true;
};




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::IsSearching
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
bool DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::IsSearching()
{ 	
	if(!threadenumdevices)							return false;
	if(!threadenumdevices->IsRunning()) return false;

	if(GetCurrentState()!=DIOANDROIDWIFIENUMFSMSTATE_SEARCH) return false;

	return true;	  
};




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::Clean
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
void DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::Clean()
{
	threadenumdevices	= NULL;
	
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::ThreadEnumDevices
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
void DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
{	
	DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES* enumeration = (DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES*)param;
	if(!enumeration) return;
	
	if(enumeration->GetEvent()==DIOANDROIDWIFIENUMFSMEVENT_NONE) // No hay nuevos Eventos
		{			
			switch(enumeration->GetCurrentState())
				{
					case DIOANDROIDWIFIENUMFSMSTATE_NONE						:	break;

					case DIOANDROIDWIFIENUMFSMSTATE_SEARCH				  : enumeration->SetEvent(DIOANDROIDWIFIENUMFSMEVENT_SEARCHEND);																																																																																			
																													break;
 			
					case DIOANDROIDWIFIENUMFSMSTATE_SEARCHEND				: break;																																																																		
							
																								 
				}
		}
	 else
		{
			if(enumeration->GetEvent()<DIOANDROIDWIFIENUM_LASTEVENT)
				{
					enumeration->CheckTransition();

					switch(enumeration->GetCurrentState())
						{
							case DIOANDROIDWIFIENUMFSMSTATE_NONE						: break;

							case DIOANDROIDWIFIENUMFSMSTATE_SEARCH				  : { wireless_scan_head	head;
																																wireless_scan*			result = NULL;
																																iwrange							range;
																																int									sock;
																																int									index = 0;		

																																memset(&head, 0, sizeof(wireless_scan_head));
																																memset(&range, 0, sizeof(iwrange));
																																
																															  /* Open socket to kernel */
																															  sock = iw_sockets_open();
																																if(sock==-1) break;
																																
																																/* Get some metadata to use for scanning */
																																if (iw_get_range_info(sock, "wlan0", &range)<0) break;
																															
																																/* Perform the scan */
																																if(iw_scan(sock, (char*)"wlan0", range.we_version_compiled, &head)<0) break;
																																
																															  /* Traverse the results */
																																result = head.result;
																																while (NULL != result) 
																																	{
																																		DIOSTREAMDEVICEWIFI* device = new DIOSTREAMDEVICEWIFI();
																																		if(device)
																																			{																																																																						
																																				device->SetIndex(index);
																																				(*device->GetName()) = result->b.essid;
																																					
																																				enumeration->devices.Add(device);

																																				index++;																																																																							
																																			}	
																																																																				
																																		result = result->next;
																																	}																																
																															}
																															break;
																																							
							case DIOANDROIDWIFIENUMFSMSTATE_SEARCHEND				: break;
					 }
				}
		}

}


#endif