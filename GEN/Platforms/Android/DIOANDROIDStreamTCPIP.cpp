//------------------------------------------------------------------------------------------
//	DIOANDROIDSTREAMTCPIP.CPP
//	
//	ANDROID Data IO Stream TCP/IP class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <malloc.h>
#include <syslog.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h> 
#include <netdb.h>
#include <linux/if_ether.h> 

#include "XFactory.h"
#include "XBuffer.h"
#include "XThread.h"
#include "XDebug.h"
#include "XString.h"
#include "XDebug.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamEnumServers.h"
#include "DIOStreamTCPIPConfig.h"

#include "DIOANDROIDStreamTCPIP.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP::DIOANDROIDSTREAMTCPIP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:48:15
//	
//	@return 			
*/
/*-----------------------------------------------------------------*/
DIOANDROIDSTREAMTCPIP::DIOANDROIDSTREAMTCPIP() : DIOSTREAMTCPIP() , XFSMACHINE(0)
{
	Clean();

	AddState(	DIOANDROIDTCPIPFSMSTATE_NONE							, 								
				 		DIOANDROIDTCPIPFSMEVENT_GETTINGCONNEXION	,	DIOANDROIDTCPIPFSMSTATE_GETTINGCONNEXION ,	
						DIOANDROIDTCPIPFSMEVENT_CONNECTED					,	DIOANDROIDTCPIPFSMSTATE_CONNECTED			 	 ,						
						DIOANDROIDTCPIPFSMEVENT_DISCONNECTING			,	DIOANDROIDTCPIPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);

	AddState(	DIOANDROIDTCPIPFSMSTATE_GETTINGCONNEXION	,
						DIOANDROIDTCPIPFSMEVENT_CONNECTED					,	DIOANDROIDTCPIPFSMSTATE_CONNECTED			 	 ,							
						DIOANDROIDTCPIPFSMEVENT_WAITINGTOREAD			,	DIOANDROIDTCPIPFSMSTATE_WAITINGTOREAD		 ,
						DIOANDROIDTCPIPFSMEVENT_SENDINGDATA				,	DIOANDROIDTCPIPFSMSTATE_SENDINGDATA			 ,						
						DIOANDROIDTCPIPFSMEVENT_DISCONNECTING			,	DIOANDROIDTCPIPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOANDROIDTCPIPFSMSTATE_CONNECTED					, 												 		
						DIOANDROIDTCPIPFSMEVENT_GETTINGCONNEXION	,	DIOANDROIDTCPIPFSMSTATE_GETTINGCONNEXION ,	
						DIOANDROIDTCPIPFSMEVENT_WAITINGTOREAD			,	DIOANDROIDTCPIPFSMSTATE_WAITINGTOREAD		 ,
						DIOANDROIDTCPIPFSMEVENT_SENDINGDATA				,	DIOANDROIDTCPIPFSMSTATE_SENDINGDATA			 ,					
						DIOANDROIDTCPIPFSMEVENT_DISCONNECTING			,	DIOANDROIDTCPIPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOANDROIDTCPIPFSMSTATE_WAITINGTOREAD			,
						DIOANDROIDTCPIPFSMEVENT_GETTINGCONNEXION	,	DIOANDROIDTCPIPFSMSTATE_GETTINGCONNEXION ,	
						DIOANDROIDTCPIPFSMEVENT_CONNECTED					,	DIOANDROIDTCPIPFSMSTATE_CONNECTED			 	 ,												
						DIOANDROIDTCPIPFSMEVENT_SENDINGDATA				,	DIOANDROIDTCPIPFSMSTATE_SENDINGDATA			 ,		
						DIOANDROIDTCPIPFSMEVENT_DISCONNECTING			,	DIOANDROIDTCPIPFSMSTATE_DISCONNECTING		 ,									
						EVENTDEFEND);	

	AddState(	DIOANDROIDTCPIPFSMSTATE_DISCONNECTING			, 												 		
						DIOANDROIDTCPIPFSMEVENT_GETTINGCONNEXION	,	DIOANDROIDTCPIPFSMSTATE_GETTINGCONNEXION ,	
						DIOANDROIDTCPIPFSMEVENT_CONNECTED					,	DIOANDROIDTCPIPFSMSTATE_CONNECTED			 	 ,						
						DIOANDROIDTCPIPFSMEVENT_WAITINGTOREAD			,	DIOANDROIDTCPIPFSMSTATE_WAITINGTOREAD		 ,
						DIOANDROIDTCPIPFSMEVENT_SENDINGDATA				,	DIOANDROIDTCPIPFSMSTATE_SENDINGDATA			 ,											
						EVENTDEFEND);	
			
	threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, __L("DIOANDROIDSTREAMTCPIP::DIOANDROIDSTREAMTCPIP"), ThreadConnexion, (void*)this);	
}
 



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP::~DIOANDROIDSTREAMTCPIP
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
DIOANDROIDSTREAMTCPIP::~DIOANDROIDSTREAMTCPIP()
{	
	if(threadconnexion) 
		{
			DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, threadconnexion);	
		}

	Clean();
}


//-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP::Open
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMTCPIP::Open()
{	
	if(!threadconnexion)	return false;

	if(!inbuffer)					return false;
	if(!outbuffer)				return false;

	SetEvent(DIOANDROIDTCPIPFSMEVENT_GETTINGCONNEXION);

	status = DIOSTREAMSTATUS_GETTINGCONNEXION;

  ResetXBuffers();

	ResetConnexionStatistics();

	return threadconnexion->Ini();		
}


/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP::Disconnect
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
bool DIOANDROIDSTREAMTCPIP::Disconnect()
{	
  if(GetConnectStatus() == DIOSTREAMSTATUS_CONNECTED)
		 {			 
			 while(GetConnectStatus() != DIOSTREAMSTATUS_DISCONNECTED)
				{
					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					usleep(10000);	
					
					if(threadconnexion)
						{
							if(!threadconnexion->IsRunning())
								{
									return true;
								}
						}
				}
		 }

	return true;
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP::Close
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 					
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMTCPIP::Close()
{		
	if(!threadconnexion)	return false;	
	
	Disconnect();

	threadconnexion->End();
	
	if(config->IsServer())
		{			
			int handleserver = (int)config->GetHandleMultiServer();

			if(handleserver != -1)
		  	{					
					config->SetCounterMultiServer(config->GetCounterMultiServer()-1);
					
					if(config->GetCounterMultiServer() <= 0)
						{    									    					
		    			close(handleserver);

							if(config->GetXMutexMultiServer()) 
								{	
									config->GetXMutexMultiServer()->Lock();			    	
									config->SetHandleMultiServer(-1);	
									config->GetXMutexMultiServer()->UnLock();			    	

									//XDEBUG_PRINTCOLOR(2, __L("TCPIP: Delete Handle BASE %d Port %d"), handleserver, config->GetRemotePort());
								}
    				}
        }			
		}

	if(handlesocket != -1)
		{			
			close(handlesocket);
			handlesocket 	= -1;
		}	

	return true;
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP::Accept
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/04/2007 11:31:55
//	
//	@return				int : 
//	@param				socket : 
//  @param				addr : 
//  @param				addrlen : 
//  @param				sec : 
*/
//-------------------------------------------------------------------
int DIOANDROIDSTREAMTCPIP::Accept(int socket, void* addr, void* addrlen, XDWORD usec)
{
	fd_set				 fds;	
	struct timeval time_out;
	int 					 status;

	if(socket==-1) return -1;

	FD_ZERO(&fds);
	FD_SET((unsigned int)socket,&fds);
	
	time_out.tv_sec  = 0;
	time_out.tv_usec = usec;

	status = (int)select((int)(socket)+1, &fds, NULL, NULL, &time_out);
	if(status>0)	
			   status = accept(socket, (struct sockaddr*)addr, (socklen_t*)addrlen);
	  else status = -1;
		
	return status;
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP::IsReadyConnect
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
int DIOANDROIDSTREAMTCPIP::IsReadyConnect(int socket)
{
	if(socket==-1) return -1;
	
	struct timeval	tv;
	int							rc;
	fd_set					fdr;
	fd_set					fdw;
	fd_set					fds;
								
	FD_ZERO(&fdr);
	FD_ZERO(&fdw);
	FD_ZERO(&fds);

	FD_SET((unsigned int)socket, &fdr);
  FD_SET((unsigned int)socket, &fdw);
	FD_SET((unsigned int)socket, &fds);

	tv.tv_sec  = 0;
	tv.tv_usec = 100;
								
	rc = select((int)(socket)+1, &fdr, &fdw, &fds, &tv);
	if(rc == -1) return -1;		
	
  int status1 = FD_ISSET(socket, &fdr) ? 1 : 0;
	int status2 = FD_ISSET(socket, &fdw) ? 1 : 0;
	int status3 = FD_ISSET(socket, &fds) ? 1 : 0;
		
	if(config->IsServer())
		{
			if(status1 || status2 )		return  1;											
		}			
	 else
		{	
			if(status1 || status2)
				{
					int				optval;
					socklen_t optlen = sizeof(optval);				

					if(getsockopt(socket,SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0 ) return -1;					
					if(optval) return -1;

					return  1;	
				}						
		}
	
	if(status3)	return -1;				

	return 0;	
}



/*-------------------------------------------------------------------
//	DIOANDROIDSTREAMTCPIP::SetPropertysHandle
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			18/05/2016 16:35:02
//	
//	@return 			bool : 
//
//  @param				socket : 
//  @param				isserver : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMTCPIP::SetPropertysHandle(int socket)
{
	int opt;
	
	opt = 1;
  if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)) < 0) return false;
	
	opt = 1;
	if(setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE , &opt, sizeof(opt)) < 0) return false;
	
	//opt = 1;           
	//if(setsockopt(socket, SOL_SOCKET, TCP_NODELAY,  &opt, sizeof(opt)) < 0) return false;

	return true;
}





/*-------------------------------------------------------------------
//	DIOANDROIDSTREAMTCPIP::GetHandleServer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/04/2016 13:36:53
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMTCPIP::GetHandleServer()
{
	int									handleserver = (int)config->GetHandleMultiServer();
	struct sockaddr_in  loc_addr		 = { 0 };
	struct sockaddr_in  rem_addr		 = { 0 };
	XSTRING							IPstring;

	if(!config->GetXMutexMultiServer())  return false;

	config->GetXMutexMultiServer()->Lock();

	if(handleserver < 0)
		{																																					
			handleserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  		if(handleserver < 0)
				{
					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					config->GetXMutexMultiServer()->UnLock();
					return false;
				}			
			
			if(!SetPropertysHandle(handleserver))
				{
					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					config->GetXMutexMultiServer()->UnLock();
					return false;
				}

			config->GetLocalIP()->GetXString(IPstring);

			loc_addr.sin_family	= AF_INET;
			if(!config->GetLocalIP()->IsEmpty())
				{
					XSTRING_CREATEOEM(IPstring, charstr)
					loc_addr.sin_addr.s_addr	= inet_addr(charstr);
					XSTRING_DELETEOEM(charstr)

				} else loc_addr.sin_addr.s_addr	= htonl(INADDR_ANY);

			loc_addr.sin_port	= htons(config->GetRemotePort());
																																												
			if(bind(handleserver, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0)
				{	
					if(errno == EADDRINUSE)
						{							
							SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);	

						} else SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					
					config->GetXMutexMultiServer()->UnLock();
					return false;
				}					

  		if(listen(handleserver, DIOSTREAMTCPIP_MAXLISTEN) < 0)
				{
					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					config->GetXMutexMultiServer()->UnLock();
					return false;
				}				
			
			fcntl(handleserver, F_SETFL, fcntl(handleserver, F_GETFL,0) | O_NONBLOCK);		

			config->SetHandleMultiServer(handleserver);				
		} 

	config->GetXMutexMultiServer()->UnLock();

	if(enumservers)
		{
			if(enumservers->GetXTimer())
				{
					if(enumservers->GetXTimer()->GetMeasureSeconds() >= DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL)	
						{
							enumservers->SendSignal();				
							enumservers->GetXTimer()->Reset();																																									
						}																																																																										
				}
		}			

	socklen_t sizeaddr = sizeof(struct sockaddr_in);
	handlesocket = Accept(handleserver, (struct sockaddr*)&rem_addr, &sizeaddr, 100/*50*1000*/);																																				
	if(handlesocket >= 0)
		{						
			if(!SetPropertysHandle(handlesocket))
				{
					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					return false;
				}			
			
		 int flags = fcntl(handlesocket, F_GETFL, 0);
 		 fcntl(handlesocket, F_SETFL, flags | O_NONBLOCK);			
			
			clientIP.Set(inet_ntoa(rem_addr.sin_addr));																																														
		}
	 else
		{
			if(errno == EADDRINUSE)
				{
					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);
					return false;
				}
		}

	
	return true;
}



/*-------------------------------------------------------------------
//	DIOANDROIDSTREAMTCPIP::GetHandleClient
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/04/2016 13:35:33
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMTCPIP::GetHandleClient()
{
	if(config->GetRemoteURL()->IsEmpty())
		{
			SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
			return false;
		} 

	handlesocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(handlesocket == -1) 
		{
			SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
			SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
			return false;	
		}
	                                                   
	if(!config->GetLocalIP()->IsEmpty())
		{ 
			struct sockaddr_in loc_addr;
			XSTRING						 IPstring;
																																					
			config->GetLocalIP()->GetXString(IPstring);

			memset(&loc_addr, 0, sizeof(struct sockaddr_in));

			loc_addr.sin_family			 = AF_INET;

			XSTRING_CREATEOEM(IPstring, charstr)
			loc_addr.sin_addr.s_addr = inet_addr(charstr);
			XSTRING_DELETEOEM(charstr)

			loc_addr.sin_port				 = 0; //htons(diostream->config->GetRemotePort());
																																					
			if(bind(handlesocket, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0 )
				{
					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					return false;
				}																																					
		}
																																			
	struct sockaddr_in  rem_addr;																																																																																																				
	DIOIP								remoteIP;
	XSTRING							IPstring;
																																	
	memset(&rem_addr, 0, sizeof(struct sockaddr_in));

	if(!config->GetRemoteURL()->ResolveURL(remoteIP))
		{
			SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
			SetLastDIOError(DIOSTREAMERROR_URLNOTVALID);
			return false;
		}
	
	remoteIP.GetXString(IPstring);
																																																																												
	rem_addr.sin_family				= AF_INET;	
	XSTRING_CREATEOEM(IPstring, charstr)
	rem_addr.sin_addr.s_addr  = inet_addr(charstr);	
	XSTRING_DELETEOEM(charstr)
	rem_addr.sin_port					= htons(config->GetRemotePort());			

		if(!SetPropertysHandle(handlesocket))
		{
			SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
			SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
			return false;
		}	

  int flags = fcntl(handlesocket, F_GETFL, 0);
	fcntl(handlesocket, F_SETFL, flags | O_NONBLOCK);																																			

	if(connect(handlesocket, (struct sockaddr *)&rem_addr, sizeof(rem_addr)) < 0)
		{
			if(errno == EADDRINUSE)
				{
					SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);
					return false;
				}
		}	
																													
	DIOSTREAMXEVENT xevent(this, DIOSTREAMXEVENTTYPE_GETTINGCONNEXION);								
	xevent.SetDIOStream(this);		
	PostEvent(&xevent);

	return true;
}





//-------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP::ThreadRunFunction
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
void DIOANDROIDSTREAMTCPIP::ThreadConnexion(void* data)
{  	
	DIOANDROIDSTREAMTCPIP* diostream = (DIOANDROIDSTREAMTCPIP*)data;
	if(!diostream) return;	

	if(diostream->GetEvent()==DIOANDROIDTCPIPFSMEVENT_NONE) // No hay nuevos Eventos
		{			
			switch(diostream->GetCurrentState())
				{
					case DIOANDROIDTCPIPFSMSTATE_NONE								: break;	

					case DIOANDROIDTCPIPFSMSTATE_GETTINGCONNEXION		: if(diostream->handlesocket == -1)
																															{
																																if(diostream->config->IsServer())
																																	{
																																		diostream->GetHandleServer();																																					
																																	}
                                                        				 else
																																	{
																																		diostream->GetHandleClient();																																					
																																	}																																																
																															}											
																														 else
																														  {						
																																switch(diostream->IsReadyConnect(diostream->handlesocket))
																																	{
																																		case -1: 	diostream->SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);																																																																																																											
																																							break;

																																		case  1:	diostream->SetEvent(DIOANDROIDTCPIPFSMEVENT_CONNECTED);
																																							diostream->status = DIOSTREAMSTATUS_CONNECTED;																																						
																																							break;

																																		default:	diostream->status = DIOSTREAMSTATUS_GETTINGCONNEXION;
																																							break;
																																	}
																															}
																														break;

					case DIOANDROIDTCPIPFSMSTATE_CONNECTED				  : break;

					case DIOANDROIDTCPIPFSMSTATE_WAITINGTOREAD			: {	struct timeval	waitd;
																														fd_set					read_flags;
																														fd_set					write_flags;
																																																																																					
																														if(diostream->handlesocket==-1) 
																															{
																																diostream->SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
																																break;	
																															}		

																														waitd.tv_sec  = 0; 				
																														waitd.tv_usec = 100;	

																														FD_ZERO(&read_flags);			
																														FD_ZERO(&write_flags);
																																																										 
																														if(!diostream->IsBlockRead()) FD_SET(diostream->handlesocket, &read_flags);
																														if(!diostream->IsBlockWrite()) 
																															{
																																if(diostream->outbuffer->GetSize()) FD_SET(diostream->handlesocket, &write_flags);
																															}
																													
																														int error = select((int)(diostream->handlesocket)+1, &read_flags, &write_flags, (fd_set*)NULL, &waitd);																															
																														if((error==-1))   //Socket ready for reading
																															{																																	
																																diostream->SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
																																break;
																															}
																													
																														if(FD_ISSET(diostream->handlesocket, &read_flags))  //Socket ready for reading
																															{
																																XBYTE buffer[DIOSTREAM_MAXBUFFER];

																																FD_CLR(diostream->handlesocket, &read_flags);																															
																																																															
										      																			int size = recv(diostream->handlesocket,(char*)buffer, DIOSTREAM_MAXBUFFER, 0);
																																if(size==-1)
																																	{
																																		diostream->SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
																																		break;
																																	}
																													 		 
																																if(size) 
																																	{
																																		diostream->inbuffer->Add(buffer, size);
																																	}
																																 else
																																	{																																			
																																		diostream->SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);
																																		break;
																																	}																																																																																																																																																	      																																					      																																																                                                            
																															}
																													
																														if(FD_ISSET(diostream->handlesocket, &write_flags))  //Socket ready for writing
  																														{	
																																FD_CLR(diostream->handlesocket, &write_flags);																																																																																																																											
																														
																																int esize = diostream->outbuffer->GetSize();
																																if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;																																			
																																if(esize)
																																	{																																					
																																		diostream->outbuffer->SetBlocked(true);
																																		int size = send(diostream->handlesocket,(char*)diostream->outbuffer->Get(), esize, 0);
																																		diostream->outbuffer->SetBlocked(false);

																																		if(size == -1) 
																																			{					
																																				diostream->SetEvent(DIOANDROIDTCPIPFSMEVENT_DISCONNECTING);	
																																				break;
																																			} 

																																		if(size) diostream->outbuffer->Extract(NULL, 0, size);	
																																	}																																																															  																															    																																																									
																															}																																																																																																																																													 																													
																													}																													
																													break;

					case DIOANDROIDTCPIPFSMSTATE_SENDINGDATA				: break;

					case DIOANDROIDTCPIPFSMSTATE_DISCONNECTING			: break;
																			
				}
		}
	 else
		{
			if(diostream->GetEvent()<DIOANDROIDTCPIP_LASTEVENT)
				{
					diostream->CheckTransition();

					switch(diostream->GetCurrentState())
						{
							case DIOANDROIDTCPIPFSMSTATE_NONE								: break;	

							case DIOANDROIDTCPIPFSMSTATE_GETTINGCONNEXION		: if(diostream->enumservers) 
																																{
																																	if(diostream->enumservers->GetXTimer()) diostream->enumservers->GetXTimer()->AddSeconds(DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL);
																																}

																															if(diostream->config->IsServer())
																																{
																																	diostream->config->SetCounterMultiServer(diostream->config->GetCounterMultiServer()+1);
																																}
																															break;

							case DIOANDROIDTCPIPFSMSTATE_CONNECTED				  : { DIOSTREAMXEVENT xevent(diostream, DIOSTREAMXEVENTTYPE_CONNECTED);								
																																xevent.SetDIOStream(diostream);		
																																diostream->PostEvent(&xevent);

																																diostream->SetEvent(DIOANDROIDTCPIPFSMEVENT_WAITINGTOREAD);
																															}
																															break;

							case DIOANDROIDTCPIPFSMSTATE_WAITINGTOREAD			: break;

							case DIOANDROIDTCPIPFSMSTATE_SENDINGDATA				: break;

							case DIOANDROIDTCPIPFSMSTATE_DISCONNECTING			: { DIOSTREAMXEVENT xevent(diostream, DIOSTREAMXEVENTTYPE_DISCONNECTED);								
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
