//------------------------------------------------------------------------------------------
//	DIOANDROIDSTREAMICMP.CPP
//	
//	ANDROID Data IO Stream ICMP class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#if defined(DIO_ACTIVE) && defined(DIOICMP_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <malloc.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h> 
#include <netdb.h>
#include <linux/if_ether.h> 

#include "XFactory.h"
#include "XThreadCollected.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamICMPConfig.h"
#include "DIOStreamICMP.h"


#include "DIOANDROIDStreamICMP.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP::DIOANDROIDSTREAMICMP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 23:10:30
//	
//	@return 			

 
 
*/
/*-----------------------------------------------------------------*/
DIOANDROIDSTREAMICMP::DIOANDROIDSTREAMICMP() : DIOSTREAMICMP() , XFSMACHINE(0)
{
	Clean();

	AddState(	DIOANDROIDICMPFSMSTATE_NONE									, 								
				 		DIOANDROIDICMPFSMEVENT_GETTINGCONNEXION			,	DIOANDROIDICMPFSMSTATE_GETTINGCONNEXION	 ,	
						DIOANDROIDICMPFSMEVENT_CONNECTED						,	DIOANDROIDICMPFSMSTATE_CONNECTED			 	 ,						
						DIOANDROIDICMPFSMEVENT_DISCONNECTING				,	DIOANDROIDICMPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);

	AddState(	DIOANDROIDICMPFSMSTATE_GETTINGCONNEXION			,
						DIOANDROIDICMPFSMEVENT_CONNECTED						,	DIOANDROIDICMPFSMSTATE_CONNECTED			 	 ,	
						DIOANDROIDICMPFSMEVENT_WAITINGTOREAD				,	DIOANDROIDICMPFSMSTATE_WAITINGTOREAD		 ,
						DIOANDROIDICMPFSMEVENT_SENDINGDATA					,	DIOANDROIDICMPFSMSTATE_SENDINGDATA			 ,						
						DIOANDROIDICMPFSMEVENT_DISCONNECTING				,	DIOANDROIDICMPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOANDROIDICMPFSMSTATE_CONNECTED						, 												 		
						DIOANDROIDICMPFSMEVENT_GETTINGCONNEXION			,	DIOANDROIDICMPFSMSTATE_GETTINGCONNEXION,	
						DIOANDROIDICMPFSMEVENT_WAITINGTOREAD				,	DIOANDROIDICMPFSMSTATE_WAITINGTOREAD		 ,
						DIOANDROIDICMPFSMEVENT_SENDINGDATA					,	DIOANDROIDICMPFSMSTATE_SENDINGDATA			 ,					
						DIOANDROIDICMPFSMEVENT_DISCONNECTING				,	DIOANDROIDICMPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOANDROIDICMPFSMSTATE_WAITINGTOREAD				, 												 		
						DIOANDROIDICMPFSMEVENT_GETTINGCONNEXION			,	DIOANDROIDICMPFSMSTATE_GETTINGCONNEXION	 ,	
						DIOANDROIDICMPFSMEVENT_CONNECTED						,	DIOANDROIDICMPFSMSTATE_CONNECTED			 	 ,												
						DIOANDROIDICMPFSMEVENT_SENDINGDATA					,	DIOANDROIDICMPFSMSTATE_SENDINGDATA			 ,		
						DIOANDROIDICMPFSMEVENT_DISCONNECTING				,	DIOANDROIDICMPFSMSTATE_DISCONNECTING		 ,									
						EVENTDEFEND);	

	AddState(	DIOANDROIDICMPFSMSTATE_DISCONNECTING				, 												 		
						DIOANDROIDICMPFSMEVENT_GETTINGCONNEXION		  ,	DIOANDROIDICMPFSMSTATE_GETTINGCONNEXION  ,	
						DIOANDROIDICMPFSMEVENT_CONNECTED						,	DIOANDROIDICMPFSMSTATE_CONNECTED			 	 ,						
						DIOANDROIDICMPFSMEVENT_WAITINGTOREAD				,	DIOANDROIDICMPFSMSTATE_WAITINGTOREAD		 ,
						DIOANDROIDICMPFSMEVENT_SENDINGDATA					,	DIOANDROIDICMPFSMSTATE_SENDINGDATA			 ,											
						EVENTDEFEND);	
						
	threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMICMP, __L("DIOANDROIDSTREAMICMP::DIOANDROIDSTREAMICMP"), ThreadRunFunction, (void*)this);
}


//-------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP::~DIOANDROIDSTREAMICMP
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
DIOANDROIDSTREAMICMP::~DIOANDROIDSTREAMICMP()
{	
	if(threadconnexion) 
		{			
			DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMICMP, threadconnexion);
		}
		
	Clean();
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP::Open
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMICMP::Open()
{	
	if(!threadconnexion)	return false;

	if(!inbuffer)					return false;
	if(!outbuffer)				return false;
					 	
	SetEvent(DIOANDROIDICMPFSMEVENT_GETTINGCONNEXION);
		
	status = DIOSTREAMSTATUS_GETTINGCONNEXION;	

	ResetXBuffers();

	ResetConnexionStatistics();
	
	return threadconnexion->Ini();	
}


/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP::Disconnect
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/12/2010 23:10:56
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMICMP::Disconnect()
{	
  if((GetConnectStatus()==DIOSTREAMSTATUS_GETTINGCONNEXION)||
		 (GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED))
		 {
			 SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);

			 while(GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
				{
					usleep(10000);
				}
		 }

	return true;
}


//-------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP::Close
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMICMP::Close()
{
	if(!threadconnexion) return false;
	
	threadconnexion->End();

	if(handle>=0)  
		{					
			shutdown(handle,SHUT_RDWR);
			close(handle);				
			handle	= -1;			
		}

	return true;
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP::IsReadyConnect
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2006 15:36:59
//	
//	@return				int :
//	@param				sock :
*/
//-------------------------------------------------------------------
int DIOANDROIDSTREAMICMP::IsReadyConnect(int socket)
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

	FD_SET(socket,&fdr);
  FD_SET(socket,&fdw);
	FD_SET(socket,&fds);

	tv.tv_sec  = 0;
	tv.tv_usec = 100;
								
	rc = select(socket+1, &fdr, &fdw, &fds, &tv);
	if(rc==-1) return -1;
		
  int status1 = FD_ISSET(socket,&fdr) ? 1 : 0;
	int status2 = FD_ISSET(socket,&fdw) ? 1 : 0;
	int status3 = FD_ISSET(socket,&fds) ? 1 : 0;

	if(config->IsServer())
		{
			if(status1 || status2 ) return  1;		
			if(status3)							return -1;	
		}
	 else
		{	
			if((!status1) && status2) return  1;	
			if(status3)			 				  return -1;	
		}

	return 0;
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP::Clean
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
void DIOANDROIDSTREAMICMP::Clean()
{
	threadconnexion   = NULL;	
	status 			 			= DIOSTREAMSTATUS_DISCONNECTED;		
	handle 						= -1;		
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP::ThreadRunFunction
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2006 15:44:00
//	
//	@return				void :
//	@param				data :
*/
//-------------------------------------------------------------------
void DIOANDROIDSTREAMICMP::ThreadRunFunction(void* thread)
{	
	DIOANDROIDSTREAMICMP* diostream = (DIOANDROIDSTREAMICMP*)thread;
	if(!diostream) return;
		  
	if(diostream->GetEvent()==DIOANDROIDICMPFSMEVENT_NONE) 
		{			
			switch(diostream->GetCurrentState())
				{
					case DIOANDROIDICMPFSMSTATE_NONE								: break;	

					case DIOANDROIDICMPFSMSTATE_GETTINGCONNEXION 	: switch(diostream->IsReadyConnect(diostream->handle))
																													{
																														case -1: 	diostream->SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);																																																																																								
																																			break;										

																														case  1:	diostream->SetEvent(DIOANDROIDICMPFSMEVENT_CONNECTED);																	
																																			diostream->status = DIOSTREAMSTATUS_CONNECTED;																																			
																																			break;

																														default:	diostream->status = DIOSTREAMSTATUS_GETTINGCONNEXION;
																																			break;
																													}																												
																												break;
																								
					case DIOANDROIDICMPFSMSTATE_CONNECTED				  : break;

					case DIOANDROIDICMPFSMSTATE_WAITINGTOREAD			: { struct timeval	waitd;
																														fd_set					read_flags;
																														fd_set					write_flags;
																															
																														if(diostream->handle == -1)
																															{
																																diostream->SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);
																																break;
																															}			

																														waitd.tv_sec  = 0;				
																														waitd.tv_usec = 100;			
																													
																														FD_ZERO(&read_flags);			
																														if(diostream->GetFirstDatagram(true) != DIOSTREAMICMP_NOTFOUND)  FD_ZERO(&write_flags);
																										
																														FD_SET(diostream->handle, &read_flags);		
																														FD_SET(diostream->handle, &write_flags);
																														
																														int error = select(diostream->handle+1, &read_flags, &write_flags, NULL, &waitd);
																														if(error==-1)
																															{
																																diostream->SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);
																																break;
																															}
																						 																							
																														if(FD_ISSET(diostream->handle, &read_flags))  //Socket ready for reading
																															{	
																																FD_CLR(diostream->handle, &read_flags);																															      																											 																															

																																if(!diostream->IsBlockRead()) 
																																	{
																																	  XBYTE buffer[DIOSTREAM_MAXBUFFER];

																																		struct sockaddr_in origin_addr;
																																		socklen_t					 size_addr   = sizeof(sockaddr_in);

																																		memset(&origin_addr, 0, size_addr);

																																		int size = recvfrom(diostream->handle, buffer, DIOSTREAM_MAXBUFFER, 0,(struct sockaddr*)&origin_addr, &size_addr);
																																		if(size < 0)
																																			{																																	
													      																				diostream->SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);
																																				break;
																																			}
																													 		 
																																			if(size != 0) 
																																				{
																																					XSTRING address;
																																																																				
																																					int _address[4] = { 0, 0, 0, 0 };

																																					sscanf(inet_ntoa(origin_addr.sin_addr),"%d.%d.%d.%d",&_address[0]
																														 																																	,&_address[1]
																																																															,&_address[2]
																																																															,&_address[3]);

																																					address.Format(__L("%d.%d.%d.%d") ,	_address[0]
																																																						,	_address[1]
																																																						, _address[2]
																																																						,	_address[3]);
																																			
																																					//XDEBUG_PRINTCOLOR(1, __L("Read UDP from [%s] (%d)"), address.Get(), size);

																																					diostream->AddDatagram(false, address.Get(), (XBYTE*)buffer,size);		
																																					diostream->inbuffer->Add(buffer, size);
																																				}
																																			 else
																																				{																																			
																																					diostream->SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);
																																					break;
																																				}  																							      																																																				
																																		}
																															}

																														if(FD_ISSET(diostream->handle, &write_flags))  //Socket ready for writing
																															{					
																																FD_CLR(diostream->handle, &write_flags);	

																																if(!diostream->IsBlockWrite()) 
																																	{
																																		int indexdatagram  = diostream->GetFirstDatagram(true);																																																									
																																		if(indexdatagram != DIOSTREAMICMP_NOTFOUND)
																																			{
																																				DIOSTREAMICMPDATAGRAM* datagram = (DIOSTREAMICMPDATAGRAM*)diostream->GetDatagram(indexdatagram);
																																				if(datagram) 
																																					{																																																																																																																																																						
																																						struct sockaddr_in  target_addr;
																																						int									size_addr = sizeof(struct sockaddr_in);
																																						int									size			= 0;
																																						XSTRING							tmpremoteaddress;

																																						memset(&target_addr, 0, size_addr);

																																						target_addr.sin_family = AF_INET;
																																																																																																																	
																																						if(datagram->GetAddress()->IsEmpty())																																						
																																										tmpremoteaddress = diostream->remoteaddress.Get();
																																							else	tmpremoteaddress = datagram->GetAddress()->Get();																																								

																																						XSTRING_CREATEOEM(tmpremoteaddress, charstr)
																																						target_addr.sin_addr.s_addr		= inet_addr(charstr);
																																						XSTRING_DELETEOEM(charstr)				
																																																																											
																																						target_addr.sin_port	= 0;

																																						size = sendto(diostream->handle,(char*)datagram->GetData()->Get(), datagram->GetData()->GetSize(), 0, (sockaddr*)&target_addr, size_addr);
																																																																							
																																						if(size < 0) 
																																							{
																																								//XDEBUG_PRINTCOLOR(4, __L("Write ICMP to [%s] (%d) ERROR!"), tmpremoteaddress.Get(), size);

																																								diostream->SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);	
																																								break;
																																							}

																																						if(size) 
																																							{
																																								//XDEBUG_PRINTCOLOR(1, __L("Write ICMP to [%s] (%d)"), tmpremoteaddress.Get(), size);

																																								diostream->outbuffer->Extract(NULL, 0 , datagram->GetData()->GetSize());
																																								diostream->DeleteDatagram(indexdatagram);																																							
																																							}																																					
																																					}
																																			}																							
																																	}
																															}
																													}
																													break;

					case DIOANDROIDICMPFSMSTATE_SENDINGDATA				: break;

					case DIOANDROIDICMPFSMSTATE_DISCONNECTING			: break;
				}
		}
	 else
		{
			if(diostream->GetEvent()<DIOANDROIDICMP_LASTEVENT)
				{
					diostream->CheckTransition();

					switch(diostream->GetCurrentState())
						{
							case DIOANDROIDICMPFSMSTATE_NONE								: break;	

							case DIOANDROIDICMPFSMSTATE_GETTINGCONNEXION 	: { struct sockaddr_in loc_addr = { 0 };		
																																																												
																																diostream->handle = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
																																if(diostream->handle < 0)	
																																	{
																																		diostream->SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);
																																		break;
																																	}		
																																																																																											
																																if(diostream->config->IsServer() || (!diostream->config->GetLocalIP()->IsEmpty()))
																																	{	
																																		XSTRING IPstring;
																																	
																																		diostream->config->GetLocalIP()->GetXString(IPstring);				
																															
																																		loc_addr.sin_family	= AF_INET;

																																		if(!diostream->config->GetLocalIP()->IsEmpty())																				
																																			{																																																																																																																																							
																																				XSTRING_CREATEOEM(IPstring, charOEM)	
																																				loc_addr.sin_addr.s_addr	= inet_addr(charOEM);																																																																																																											
																																				XSTRING_DELETEOEM(charOEM)	

																																			} else loc_addr.sin_addr.s_addr	= htonl(INADDR_ANY);																																																												
																																		
																																		loc_addr.sin_port	= 0;
																																																																						
																																		if(bind(diostream->handle, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) == -1)
																																			{
																																				diostream->SetEvent(DIOANDROIDICMPFSMEVENT_DISCONNECTING);
																																				break;																																	
																																			}
																																	}

																																if(!diostream->config->IsServer())
																																	{
																																		diostream->config->GetRemoteURL()->ResolveURL(diostream->remoteaddress);																																																																																																				
																																	}		

																																fcntl(diostream->handle, F_SETFL, fcntl(diostream->handle, F_GETFL,0) | O_NONBLOCK);	
																																																																																																								
																															}																					
																															break;


							case DIOANDROIDICMPFSMSTATE_CONNECTED				  : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);								
																																xevent.SetDIOStream(diostream);		
																																diostream->PostEvent(&xevent);

																																diostream->SetEvent(DIOANDROIDICMPFSMEVENT_WAITINGTOREAD);
																															}
																															break;

							case DIOANDROIDICMPFSMSTATE_WAITINGTOREAD			: break;

							case DIOANDROIDICMPFSMSTATE_SENDINGDATA				: break;

							case DIOANDROIDICMPFSMSTATE_DISCONNECTING			: { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);								
																															xevent.SetDIOStream(diostream);		
																															diostream->PostEvent(&xevent);
																															
																															diostream->threadconnexion->Run(false);
																															diostream->status 			 = DIOSTREAMSTATUS_DISCONNECTED;
																														}																														
																														break;

						}
				}
		}
}

#endif