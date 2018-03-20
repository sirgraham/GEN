//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMUDP.CPP
//	
//	WINDOWS Data IO Stream UDP class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "XThreadCollected.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamUDPConfig.h"

#include "DIOWINDOWSStreamUDP.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP::DIOWINDOWSSTREAMUDP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:42:19
//	
//	@return 			void : 
*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMUDP::DIOWINDOWSSTREAMUDP() : DIOSTREAMUDP() , XFSMACHINE(0)
{
	Clean();

	AddState(	DIOWINDOWSUDPFSMSTATE_NONE								, 								
				 		DIOWINDOWSUDPFSMEVENT_GETTINGCONNEXION		,	DIOWINDOWSUDPFSMSTATE_GETTINGCONNEXION ,	
						DIOWINDOWSUDPFSMEVENT_CONNECTED						,	DIOWINDOWSUDPFSMSTATE_CONNECTED			 	 ,						
						DIOWINDOWSUDPFSMEVENT_DISCONNECTING				,	DIOWINDOWSUDPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);

	AddState(	DIOWINDOWSUDPFSMSTATE_GETTINGCONNEXION		,
						DIOWINDOWSUDPFSMEVENT_CONNECTED						,	DIOWINDOWSUDPFSMSTATE_CONNECTED			 	 ,							
						DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD				,	DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD		 ,
						DIOWINDOWSUDPFSMEVENT_SENDINGDATA					,	DIOWINDOWSUDPFSMSTATE_SENDINGDATA			 ,						
						DIOWINDOWSUDPFSMEVENT_DISCONNECTING				,	DIOWINDOWSUDPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOWINDOWSUDPFSMSTATE_CONNECTED						, 												 		
						DIOWINDOWSUDPFSMEVENT_GETTINGCONNEXION		,	DIOWINDOWSUDPFSMSTATE_GETTINGCONNEXION ,	
						DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD				,	DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD		 ,
						DIOWINDOWSUDPFSMEVENT_SENDINGDATA					,	DIOWINDOWSUDPFSMSTATE_SENDINGDATA			 ,					
						DIOWINDOWSUDPFSMEVENT_DISCONNECTING				,	DIOWINDOWSUDPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD				,
						DIOWINDOWSUDPFSMEVENT_GETTINGCONNEXION		,	DIOWINDOWSUDPFSMSTATE_GETTINGCONNEXION ,	
						DIOWINDOWSUDPFSMEVENT_CONNECTED						,	DIOWINDOWSUDPFSMSTATE_CONNECTED			 	 ,												
						DIOWINDOWSUDPFSMEVENT_SENDINGDATA					,	DIOWINDOWSUDPFSMSTATE_SENDINGDATA			 ,		
						DIOWINDOWSUDPFSMEVENT_DISCONNECTING				,	DIOWINDOWSUDPFSMSTATE_DISCONNECTING		 ,									
						EVENTDEFEND);	

	AddState(	DIOWINDOWSUDPFSMSTATE_DISCONNECTING				, 												 		
						DIOWINDOWSUDPFSMEVENT_GETTINGCONNEXION		,	DIOWINDOWSUDPFSMSTATE_GETTINGCONNEXION ,	
						DIOWINDOWSUDPFSMEVENT_CONNECTED						,	DIOWINDOWSUDPFSMSTATE_CONNECTED			 	 ,						
						DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD				,	DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD		 ,
						DIOWINDOWSUDPFSMEVENT_SENDINGDATA					,	DIOWINDOWSUDPFSMSTATE_SENDINGDATA			 ,											
						EVENTDEFEND);	
		
	threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, __L("DIOWINDOWSSTREAMUDP::DIOWINDOWSSTREAMUDP"), ThreadConnexion, (void*)this);	
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP::~DIOWINDOWSSTREAMUDP
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
DIOWINDOWSSTREAMUDP::~DIOWINDOWSSTREAMUDP()
{	
	if(threadconnexion) 
		{
			DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, threadconnexion);
		}

	Clean();
}




//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP::Open
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMUDP::Open()
{	
	if(!threadconnexion)	return false;

	if(!inbuffer)					return false;
	if(!outbuffer)				return false;
	if(!config)						return true;

	SetEvent(DIOWINDOWSUDPFSMEVENT_GETTINGCONNEXION);

	status = DIOSTREAMSTATUS_GETTINGCONNEXION;

  ResetXBuffers();

	ResetConnexionStatistics();

	threadconnexion->SetWaitYield(config->GetThreadWaitYield());
	threadconnexion->SetPriority(config->GetThreadPriority());

	return threadconnexion->Ini();
}




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP::Disconnect
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
bool DIOWINDOWSSTREAMUDP::Disconnect()
{	
  if((GetConnectStatus()==DIOSTREAMSTATUS_GETTINGCONNEXION)||
		 (GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED))
		 {
			 SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);

			 while(GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
				{
					Sleep(10);
				}
		 }

	return true;
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP::Close
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMUDP::Close()
{		
	if(!threadconnexion) return false;
	
	threadconnexion->End();
	
	if(handle!=INVALID_SOCKET)
	 	{					
  		shutdown(handle,SD_BOTH);
			closesocket(handle);
	   	handle	= INVALID_SOCKET;  
		}

	return true;
}




//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP::IsReadyConnect
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
int DIOWINDOWSSTREAMUDP::IsReadyConnect(SOCKET socket)
/*
{
	struct timeval	tv;
	int							rc;
	fd_set					fdr;
	fd_set					fdw;
	fd_set					fds;

	if(socket==INVALID_SOCKET) return -1;
								
	FD_ZERO(&fdr);
	FD_ZERO(&fdw);
	FD_ZERO(&fds);

	FD_SET((unsigned int)socket,&fdr);
  FD_SET((unsigned int)socket,&fdw);
	FD_SET((unsigned int)socket,&fds);

	tv.tv_sec  = 0;
	tv.tv_usec = 100;
								
	rc = select((int)(socket)+1, &fdr, &fdw, &fds, &tv);
	if(rc==SOCKET_ERROR) return -1;
	//if(rc>1)						 return -1;
	
  int status1 = FD_ISSET(socket,&fdr) ? 1 : 0;
	int status2 = FD_ISSET(socket,&fdw) ? 1 : 0;
	int status3 = FD_ISSET(socket,&fds) ? 1 : 0;

	if(status1 || status2) return  1;		
	if(status3)						 return -1;

	return 0;
}
*/
{
	struct timeval	tv;
	int							rc;
	fd_set					fdr;
	fd_set					fdw;
	fd_set					fds;

	if(socket == INVALID_SOCKET) return -1;
								
	FD_ZERO(&fdr); 
	FD_ZERO(&fdw);
	FD_ZERO(&fds);

	FD_SET((unsigned int)socket, &fdr);
  FD_SET((unsigned int)socket, &fdw);
	FD_SET((unsigned int)socket, &fds);

	tv.tv_sec  = 0;
	tv.tv_usec = 100;
								
	rc = select((int)(socket)+1,&fdr, &fdw, &fds, &tv);
	if(rc == SOCKET_ERROR) return -1;
	
  int status1 = FD_ISSET(socket, &fdr) ? 1 : 0;
	int status2 = FD_ISSET(socket, &fdw) ? 1 : 0;
	int status3 = FD_ISSET(socket, &fds) ? 1 : 0;

	//XDEBUG_PRINTCOLOR(1, __L("UDP stream: r: %d, w: %d, s: %d"), status1, status2, status3);

	if(config->IsServer())
		{
			if(status1 || status2 )		return  1;		
			if(status3)								return -1;	
		}
	 else
		{	
			if((!status1) && status2) return  1;	
			if(status3)			 				  return -1;	
		}
		
	return 0;
}





//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP::Clean
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
void DIOWINDOWSSTREAMUDP::Clean()
{
	threadconnexion		=	NULL;
	status 			 			= DIOSTREAMSTATUS_DISCONNECTED;	
	handle						= INVALID_SOCKET;	
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP::ThreadRunFunction
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
void DIOWINDOWSSTREAMUDP::ThreadConnexion(void* data)
{  	
	DIOWINDOWSSTREAMUDP* diostream = (DIOWINDOWSSTREAMUDP*)data;
	if(!diostream) return;	

	if(diostream->GetEvent()==DIOWINDOWSUDPFSMEVENT_NONE) // No hay nuevos Eventos
		{			
			switch(diostream->GetCurrentState())
				{
					case DIOWINDOWSUDPFSMSTATE_NONE								: break;	

					case DIOWINDOWSUDPFSMSTATE_GETTINGCONNEXION		: switch(diostream->IsReadyConnect(diostream->handle))
																														{
																															case -1: 	diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);																																																																																																											
																																				break;

																															case  1:	diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_CONNECTED);
																																				diostream->status = DIOSTREAMSTATUS_CONNECTED;																																						
																																				break;

																															default:	diostream->status = DIOSTREAMSTATUS_GETTINGCONNEXION;
																																				break;
																														}
																													break;

					case DIOWINDOWSUDPFSMSTATE_CONNECTED				  : break;

					case DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD			: {	struct timeval	waitd;
																														fd_set					read_flags;
																														fd_set					write_flags;
																														XBYTE           buffer[DIOSTREAM_MAXBUFFER];
																																																																																					
																														if(diostream->handle == INVALID_SOCKET) 
																															{
																																diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
																																break;	
																															}

																														waitd.tv_sec  = 0; 				
																														waitd.tv_usec = 100;

																														FD_ZERO(&read_flags);			
																														FD_ZERO(&write_flags);

																														if(!diostream->IsBlockRead()) FD_SET(diostream->handle, &read_flags);																																																																																																																																																			 																													 																																																												
																														if(!diostream->IsBlockWrite()) 
																															{																																																														
																																if(diostream->config->IsUsedDatagrams())
																																	{
																																		if(diostream->GetFirstDatagram(true) != DIOSTREAMUDP_NOTFOUND) FD_SET(diostream->handle, &write_flags);
																																	}
																																 else
																																	{
																																		if(diostream->outbuffer->GetSize()) FD_SET(diostream->handle, &write_flags);																																
																																	}
																															}
																																																																																								
																														int error = select((int)(diostream->handle)+1, &read_flags, &write_flags, (fd_set*)0, &waitd);
																														if(error == SOCKET_ERROR)
																															{
																																diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
																																break;
																															}
																													
																														if(FD_ISSET(diostream->handle, &read_flags))  //Socket ready for reading
																															{	
																																FD_CLR(diostream->handle, &read_flags);	

																																memset(buffer,0,DIOSTREAM_MAXBUFFER);

																																SOCKADDR_IN  origin_addr;
																																int					 size_addr = sizeof(SOCKADDR_IN);

																																memset(&origin_addr, 0, size_addr);

																																int size = recvfrom(diostream->handle, (char*)buffer, DIOSTREAM_MAXBUFFER, 0,(sockaddr*)&origin_addr, &size_addr);
																																																																				
																																if(size == SOCKET_ERROR)
																																	{																																				
													      																		diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
																																		break;
																																	}
																													 		 
																																if(size) 
																																	{
																																		XSTRING address;
																																		XWORD   port;

																																		address.Format(__L("%d.%d.%d.%d") ,	origin_addr.sin_addr.S_un.S_un_b.s_b1
																																																			,	origin_addr.sin_addr.S_un.S_un_b.s_b2
																																																			, origin_addr.sin_addr.S_un.S_un_b.s_b3
																																																			,	origin_addr.sin_addr.S_un.S_un_b.s_b4);

																																		//XDEBUG_PRINTCOLOR(1, __L("Read UDP from [%s] (%d)"), address.Get(), size);

																																		port =  ntohs(origin_addr.sin_port);

																																		if(diostream->config->IsUsedDatagrams())
																																						diostream->AddDatagram(false, address.Get(), port, (XBYTE*)buffer,size);
																																		 	 else diostream->inbuffer->Add(buffer, size);
																																	}
																																 else
																																	{																																			
																																		diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
																																		break;																															
																																	}
																															}
																													

																														if(FD_ISSET(diostream->handle, &write_flags))  //Socket ready for writing
  																														{	
																																FD_CLR(diostream->handle, &write_flags);		
																														
																																if(diostream->config->IsUsedDatagrams())
																																	{
																																		int indexdatagram  = diostream->GetFirstDatagram(true);
																																		if(indexdatagram != DIOSTREAMUDP_NOTFOUND)
																																			{
																																				DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)diostream->GetDatagram(indexdatagram);
																																				if(datagram) 
																																					{																																																																											  
																																						SOCKADDR_IN  target_addr;
																																						int					 size_addr = sizeof(SOCKADDR_IN);	
																																						int					 size;																																						
																																						XSTRING			 tmpremoteaddress;

																																						memset(&target_addr, 0, size_addr);

																																						target_addr.sin_family = AF_INET;
																																						
																																						if(diostream->config->IsBroadcastModeActive())																																													
																																							{
																																								target_addr.sin_addr.s_addr	= htonl(INADDR_BROADCAST);
																																							}
																																						 else
																																							{																																								
																																								if(datagram->GetAddress()->IsEmpty())																																						
																																												tmpremoteaddress = diostream->remoteaddress.Get();
																																									else	tmpremoteaddress = datagram->GetAddress()->Get();																																								

																																								XSTRING_CREATEOEM(tmpremoteaddress, charstr)
                                                                                #ifndef BUILDER
																																								inet_pton(target_addr.sin_family, charstr, &target_addr.sin_addr.s_addr);
                                                                                #else
                                                                                target_addr.sin_addr.s_addr		= inet_addr(charstr);
                                                                                #endif
																																								
																																								XSTRING_DELETEOEM(charstr)				
																																							}
																																						
																																						target_addr.sin_port	= htons(datagram->GetPort()?datagram->GetPort():diostream->config->GetRemotePort());
																																						
																																						size = sendto(diostream->handle,(char*)datagram->GetData()->Get(), datagram->GetData()->GetSize(), 0, (sockaddr*)&target_addr, size_addr);
																																							
																																						//XDEBUG_PRINTCOLOR(1, __L("Write UDP to [%s] (%d)"), tmpremoteaddress.Get(), size);

																																						if(size == SOCKET_ERROR) 
																																							{
																																								diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);	
																																								break;
																																							}

																																						if(size) 
																																							{
																																								if(diostream->config->IsUsedDatagrams())
																																										 diostream->DeleteDatagram(indexdatagram);																																								
																																								else diostream->outbuffer->Extract(NULL, 0 , datagram->GetData()->GetSize());																																								
																																							}
																																					}
																																			}
																																	}																																																																			
																															 	 else
																																	{																																				
																																		int esize = diostream->outbuffer->GetSize();
																																		if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;
																																			
																																		if(esize)
																																			{																																				
																																				SOCKADDR_IN  target_addr;
																																				int					 size_addr = sizeof(SOCKADDR_IN);	
																																				int					 size;																																						
																																				XSTRING			 tmpremoteaddress;

																																				memset(&target_addr, 0, size_addr);

																																				target_addr.sin_family = AF_INET;
																																								
																																				if(diostream->config->IsBroadcastModeActive())																																													
																																					{
																																						target_addr.sin_addr.s_addr	= htonl(INADDR_BROADCAST);
																																					}
																																				 else 
																																					{	
																																						tmpremoteaddress = diostream->remoteaddress.Get();
																																						XSTRING_CREATEOEM(tmpremoteaddress, charstr)
                                                                            #ifndef BUILDER
																																						inet_pton(target_addr.sin_family, charstr, &target_addr.sin_addr.s_addr);
                                                                            #else
																																						target_addr.sin_addr.s_addr		= inet_addr(charstr);
                                                                            #endif
																																						XSTRING_DELETEOEM(charstr)				
																																					}	
																																						
																																				target_addr.sin_port	= htons(diostream->config->GetRemotePort());

																																				diostream->outbuffer->SetBlocked(true);
																																				size = sendto(diostream->handle,(char*)diostream->outbuffer->Get(), esize, 0, (sockaddr*)&target_addr, size_addr);
																																				diostream->outbuffer->SetBlocked(false);
																																						
																																				if(size == SOCKET_ERROR) 
																																					{
																																						diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);	
																																						break;
																																					}

																																				if(size) 
																																					{
																																						diostream->outbuffer->Extract(NULL, 0 , esize);																																								
																																					}																																							
																																	
																																			}
																																	}
																														  }																																																																																																																																													 																													
																													}																													
																													break;

					case DIOWINDOWSUDPFSMSTATE_SENDINGDATA				: break;

					case DIOWINDOWSUDPFSMSTATE_DISCONNECTING			: break;
																			
				}
		}
	 else
		{
			if(diostream->GetEvent()<DIOWINDOWSUDP_LASTEVENT)
				{
					diostream->CheckTransition();

					switch(diostream->GetCurrentState())
						{
							case DIOWINDOWSUDPFSMSTATE_NONE								: break;	

							case DIOWINDOWSUDPFSMSTATE_GETTINGCONNEXION		: {	SOCKADDR_IN loc_addr;
								
																																memset(&loc_addr, 0, sizeof(SOCKADDR_IN));	
																														 																																																																
																																diostream->handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  																															if(diostream->handle == INVALID_SOCKET)
																																	{
																																		diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
																																		break;
																																	}																																
																																				
																																if(diostream->config->IsServer())
																																	{
																																		XSTRING IPstring;																																				
																																				
																																		diostream->config->GetLocalIP()->GetXString(IPstring);	
																																		
																																		loc_addr.sin_family	= AF_INET;								
																																		
																																		if(!diostream->config->GetLocalIP()->IsEmpty())		
																																			{
																																				XSTRING_CREATEOEM(IPstring, charstr)
                                                                        #ifndef BUILDER
																																				inet_pton(loc_addr.sin_family, charstr, &loc_addr.sin_addr.s_addr);
                                                                        #else
																																				loc_addr.sin_addr.s_addr	= inet_addr(charstr);
                                                                        #endif
																																				XSTRING_DELETEOEM(charstr)	 
																																																																					
																																			} 
																																		 else 
																																			{
																																				loc_addr.sin_addr.s_addr	= htonl(INADDR_ANY);																																				
																																			}
																																			
																																		loc_addr.sin_port	= htons(diostream->config->GetRemotePort());	
																																		
																																		if(bind(diostream->handle, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
																																			{
																																				diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
																																				break;
																																			}						
																																	}

																																if(!diostream->config->IsServer())
																																	{																																																																																																											
																																		if(diostream->config->IsBroadcastModeActive())
																																			{
																																				int yes = 1;
																																				setsockopt(diostream->handle, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));																																				
																																			}	
																																		 else
																																			{
																																				if(diostream->config->GetRemoteURL()->GetSize()) diostream->config->GetRemoteURL()->ResolveURL(diostream->remoteaddress);
																																			}
																																	}		

																																if(diostream->config->GetSizeBufferSO())
																																	{
																																		XDWORD sizebuffer = diostream->config->GetSizeBufferSO();
																																		if(setsockopt(diostream->handle, SOL_SOCKET, SO_RCVBUF, (const char*)&sizebuffer, sizeof(XDWORD)) == -1) 
																																			{
																																				//fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
																																			}
																																	}
																																																																	
																																DWORD nonblock = 1;
																																if (ioctlsocket(diostream->handle, FIONBIO, &nonblock) == SOCKET_ERROR) 
																																	{
																																		diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
																																		break;
																																	}																																																																		
																															}																																		
																															break;

							case DIOWINDOWSUDPFSMSTATE_CONNECTED				  : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);								
																																xevent.SetDIOStream(diostream);		

																																diostream->PostEvent(&xevent);

																																diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD);
																															}
																															break;

							case DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD			: break;

							case DIOWINDOWSUDPFSMSTATE_SENDINGDATA				: break;

							case DIOWINDOWSUDPFSMSTATE_DISCONNECTING			: { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);								
																																xevent.SetDIOStream(diostream);		

																																diostream->PostEvent(&xevent);
								
																																diostream->threadconnexion->Run(false); 
																																diostream->status       = DIOSTREAMSTATUS_DISCONNECTED;																															
																															}
																															break;

						}
				}
		}							
}
