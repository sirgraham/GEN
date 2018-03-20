//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMTCPIP.CPP
//	
//	WINDOWS Data IO Stream TCP/IP class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

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

#include "DIOWINDOWSStreamTCPIP.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIP::DIOWINDOWSSTREAMTCPIP
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
DIOWINDOWSSTREAMTCPIP::DIOWINDOWSSTREAMTCPIP() : DIOSTREAMTCPIP() , XFSMACHINE(0)
{
	Clean();

	AddState(	DIOWINDOWSTCPIPFSMSTATE_NONE							, 								
				 		DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNEXION	,	DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNEXION ,	
						DIOWINDOWSTCPIPFSMEVENT_CONNECTED					,	DIOWINDOWSTCPIPFSMSTATE_CONNECTED			 	 ,						
						DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING			,	DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);

	AddState(	DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNEXION	,
						DIOWINDOWSTCPIPFSMEVENT_CONNECTED					,	DIOWINDOWSTCPIPFSMSTATE_CONNECTED			 	 ,							
						DIOWINDOWSTCPIPFSMEVENT_WAITINGTOREAD			,	DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD		 ,
						DIOWINDOWSTCPIPFSMEVENT_SENDINGDATA				,	DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA			 ,						
						DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING			,	DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOWINDOWSTCPIPFSMSTATE_CONNECTED					, 												 		
						DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNEXION	,	DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNEXION ,	
						DIOWINDOWSTCPIPFSMEVENT_WAITINGTOREAD			,	DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD		 ,
						DIOWINDOWSTCPIPFSMEVENT_SENDINGDATA				,	DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA			 ,					
						DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING			,	DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD			,
						DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNEXION	,	DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNEXION ,	
						DIOWINDOWSTCPIPFSMEVENT_CONNECTED					,	DIOWINDOWSTCPIPFSMSTATE_CONNECTED			 	 ,												
						DIOWINDOWSTCPIPFSMEVENT_SENDINGDATA				,	DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA			 ,		
						DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING			,	DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING		 ,									
						EVENTDEFEND);	

	AddState(	DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING			, 												 		
						DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNEXION	,	DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNEXION ,	
						DIOWINDOWSTCPIPFSMEVENT_CONNECTED					,	DIOWINDOWSTCPIPFSMSTATE_CONNECTED			 	 ,						
						DIOWINDOWSTCPIPFSMEVENT_WAITINGTOREAD			,	DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD		 ,
						DIOWINDOWSTCPIPFSMEVENT_SENDINGDATA				,	DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA			 ,											
						EVENTDEFEND);	
			
	threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, __L("DIOWINDOWSSTREAMTCPIP::DIOWINDOWSSTREAMTCPIP"), ThreadConnexion, (void*)this);	
}
 



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIP::~DIOWINDOWSSTREAMTCPIP
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
DIOWINDOWSSTREAMTCPIP::~DIOWINDOWSSTREAMTCPIP()
{	
	if(threadconnexion) 
		{
			DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, threadconnexion);	
		}

	Clean();
}


//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIP::Open
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMTCPIP::Open()
{	
	if(!threadconnexion)	return false;
	if(!inbuffer)					return false;		
	if(!outbuffer)				return false;
		
	SetEvent(DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNEXION);

	status = DIOSTREAMSTATUS_GETTINGCONNEXION;

  ResetXBuffers();

	ResetConnexionStatistics();

	return threadconnexion->Ini();		
}


/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIP::Disconnect
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
bool DIOWINDOWSSTREAMTCPIP::Disconnect()
{		
  if((GetConnectStatus() == DIOSTREAMSTATUS_CONNECTED))
		 {			 
			 while(GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
				{
					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
					Sleep(10);		
					
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
//  DIOWINDOWSSTREAMTCPIP::Close
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMTCPIP::Close()
{			
	if(!threadconnexion)	return false;	
	
	Disconnect();

	threadconnexion->End();
	
	if(config->IsServer())
		{			
			SOCKET handleserver = (SOCKET)config->GetHandleMultiServer();

			if(handleserver != INVALID_SOCKET)
		  	{					
					config->SetCounterMultiServer(config->GetCounterMultiServer()-1);
					
					if(config->GetCounterMultiServer() <=0)
						{    									    					
		    			closesocket(handleserver);

							if(config->GetXMutexMultiServer()) 
								{	
									config->GetXMutexMultiServer()->Lock();			    	
									config->SetHandleMultiServer((int)INVALID_SOCKET);	
									config->GetXMutexMultiServer()->UnLock();		

									//XDEBUG_PRINTCOLOR(1, __L("TCPIP: Delete Handle BASE %d Port %d"), handleserver, config->GetRemotePort());
								}
    				}
        }			
		}

	if(handlesocket != INVALID_SOCKET)	
		{
			closesocket(handlesocket);
			handlesocket 	= INVALID_SOCKET;
		}	
		
	return true;
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIP::Accept
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
SOCKET DIOWINDOWSSTREAMTCPIP::Accept(SOCKET socket, void* addr, void* addrlen, XDWORD usec)
{
	fd_set				 fds;	
	struct timeval time_out;
	SOCKET 				 status;

	if(socket==INVALID_SOCKET) return INVALID_SOCKET;

	FD_ZERO(&fds);
	FD_SET((unsigned int)socket,&fds);
	
	time_out.tv_sec  = 0;
	time_out.tv_usec = usec;

	status = (SOCKET)select((int)(socket)+1, &fds, NULL, NULL, &time_out);
	if(status>0)
			   status = accept(socket,(SOCKADDR*)addr, (int*)addrlen);
	  else status = INVALID_SOCKET;
		
	return status;
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIP::IsReadyConnect
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
int DIOWINDOWSSTREAMTCPIP::IsReadyConnect(SOCKET socket)
{
	if(socket==INVALID_SOCKET) return -1;

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
	if(rc == SOCKET_ERROR) return -1;

  int status1 = FD_ISSET(socket,&fdr) ? 1 : 0;
	int status2 = FD_ISSET(socket,&fdw) ? 1 : 0;
	int status3 = FD_ISSET(socket,&fds) ? 1 : 0;
	
	if(config->IsServer())
		{
			if(status1 || status2 )		return  1;					
		}
	 else
		{	
			if(status1 || status2) 
				{
					int optval;
					int optlen = sizeof(int);				

					if(getsockopt(socket,SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen) < 0) return -1;
					
					return  1;	
				}			
		}
		
	if(status3)	return -1;				

	return 0;
}



/*-------------------------------------------------------------------
//	DIOWINDOWSSTREAMTCPIP::SetPropertysHandle
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			18/05/2016 15:59:09
//	
//	@return 			bool : 
//
//  @param				socket : 
//  @param				isserver : 
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIP::SetPropertysHandle(SOCKET socket, bool isserver)
{
	int opt;
	
	opt = 1;
	if(setsockopt(socket, SOL_SOCKET, isserver?SO_EXCLUSIVEADDRUSE:SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)  return false;

	opt = 1;
	if(setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE			  , (char*)&opt, sizeof(opt)) == SOCKET_ERROR)  return false;
	
	//opt = 1;           
	//if(setsockopt(socket, SOL_SOCKET, TCP_NODELAY, (char*)&opt, sizeof(opt))	== SOCKET_ERROR)	return false;

	return true;
}





/*-------------------------------------------------------------------
//	DIOWINDOWSSTREAMTCPIP::GetHandleServer
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
bool DIOWINDOWSSTREAMTCPIP::GetHandleServer()
{
	SOCKET			 handleserver = (SOCKET)config->GetHandleMultiServer();
	SOCKADDR_IN  loc_addr;
	SOCKADDR_IN  rem_addr;
	XSTRING			 IPstring;

	memset(&loc_addr,0,sizeof(SOCKADDR_IN));
	memset(&rem_addr,0,sizeof(SOCKADDR_IN));
	
	if(!config->GetXMutexMultiServer())  return false;

	config->GetXMutexMultiServer()->Lock();

	if(handleserver == INVALID_SOCKET)
		{					
			handleserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  		if(handleserver== INVALID_SOCKET)
				{					
					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					config->GetXMutexMultiServer()->UnLock();
					return false;
				}
			
			if(!SetPropertysHandle(handleserver, true))
				{
					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					config->GetXMutexMultiServer()->UnLock();
					return false;
				}									

			config->GetLocalIP()->GetXString(IPstring);

			loc_addr.sin_family	= AF_INET;
			if(!config->GetLocalIP()->IsEmpty())
				{
					XSTRING_CREATEOEM(IPstring, charstr)
          #ifndef BUILDER
					inet_pton(loc_addr.sin_family, charstr, &loc_addr.sin_addr.s_addr);
          #else
					loc_addr.sin_addr.s_addr	= inet_addr(charstr);
          #endif
					XSTRING_DELETEOEM(charstr)

				} else loc_addr.sin_addr.s_addr	= htonl(INADDR_ANY);

			loc_addr.sin_port	= htons(config->GetRemotePort());
																																																																																																												
			if(bind(handleserver, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
				{	
					int errortype = WSAGetLastError();			
					if(errortype == WSAEADDRINUSE)
						{
							SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);

					  } else SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);					
					config->GetXMutexMultiServer()->UnLock();
					return false;
				}
																																																																										
  		if(listen(handleserver, DIOSTREAMTCPIP_MAXLISTEN) == SOCKET_ERROR)
				{ 
					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					config->GetXMutexMultiServer()->UnLock();
					return false;
				}
					
			DWORD nonblock = 1;
			ioctlsocket(handleserver, FIONBIO, &nonblock);

			config->SetHandleMultiServer((int)handleserver);			

			//XDEBUG_PRINTCOLOR(1, __L("TCPIP: Create Handle BASE %d Port %d"), handleserver, config->GetRemotePort());
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
	
	int sizeaddr   = sizeof(SOCKADDR_IN);																																																																							
	handlesocket = Accept(handleserver, (SOCKADDR*)&rem_addr, &sizeaddr, 100/*50*1000*/);																																					
	if(handlesocket != INVALID_SOCKET)
		{				
			if(!SetPropertysHandle(handlesocket, false))
				{
					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					return false;
				}			

			DWORD nonblock = 1;
			ioctlsocket(handlesocket, FIONBIO, &nonblock);		

      #ifndef BUILDER
			char IPcharstring[_MAXSTR];
			inet_ntop(AF_INET, &(rem_addr.sin_addr), IPcharstring, _MAXSTR);
			clientIP.Set(IPcharstring);
      #else
			clientIP.Set(inet_ntoa(rem_addr.sin_addr));
      #endif																																				
		}
	 else
		{
			int errortype = WSAGetLastError();
			// consider what's better, check for individual error codes or just anything now WSAEWOULDBLOCK
			if(errortype == WSAEADDRINUSE)
				{
					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);
					return false;
				}
		}
	
	return true;
}



/*-------------------------------------------------------------------
//	DIOWINDOWSSTREAMTCPIP::GetHandleClient
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
bool DIOWINDOWSSTREAMTCPIP::GetHandleClient()
{
	if(config->GetRemoteURL()->IsEmpty())
		{
			SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
			return false;
		} 

	handlesocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(handlesocket == INVALID_SOCKET) 
		{
			SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
			SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
			return false;	
		}
                                                   
	if(!config->GetLocalIP()->IsEmpty())
		{ 
			SOCKADDR_IN  loc_addr;
			XSTRING			 IPstring;
																																					
			config->GetLocalIP()->GetXString(IPstring);

			memset(&loc_addr,0,sizeof(SOCKADDR_IN));

			loc_addr.sin_family			 = AF_INET;

			XSTRING_CREATEOEM(IPstring, charstr)
      #ifndef BUILDER
			inet_pton(loc_addr.sin_family, charstr, &loc_addr.sin_addr.s_addr);
      #else
			loc_addr.sin_addr.s_addr = inet_addr(charstr);
      #endif
			XSTRING_DELETEOEM(charstr)

			loc_addr.sin_port				 = 0; //htons(diostream->config->GetRemotePort());
																																					
			if(bind(handlesocket, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
				{
					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
					SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
					return false;
				}																																					
		}
																																			
	SOCKADDR_IN rem_addr;																																																																																																				
	DIOIP				remoteIP;
	XSTRING			IPstring;
																																	
	memset(&rem_addr,0,sizeof(SOCKADDR_IN));

	if(!config->GetRemoteURL()->ResolveURL(remoteIP))
		{
			SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
			SetLastDIOError(DIOSTREAMERROR_URLNOTVALID);

			return false;
		}	
																																			
	remoteIP.GetXString(IPstring);
																																																																												
	rem_addr.sin_family				= AF_INET;	
	XSTRING_CREATEOEM(IPstring, charstr)
  #ifndef BUILDER
	inet_pton(rem_addr.sin_family, charstr, &rem_addr.sin_addr.s_addr);
  #else
	rem_addr.sin_addr.s_addr  = inet_addr(charstr);
  #endif	
	XSTRING_DELETEOEM(charstr)
	rem_addr.sin_port					= htons(config->GetRemotePort());																																		

	if(!SetPropertysHandle(handlesocket, false))
		{
			SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
			SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

			return false;
		}			

	DWORD nonblock = 1;
	ioctlsocket(handlesocket, FIONBIO, &nonblock); 
																																				
	if(connect(handlesocket, (LPSOCKADDR)&rem_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		{
			int errortype = WSAGetLastError();
			// consider what's better, check for individual error codes or just anything now WSAEWOULDBLOCK
			if(errortype == WSAEADDRINUSE)
				{
					SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
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
//  DIOWINDOWSSTREAMTCPIP::ThreadRunFunction
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
void DIOWINDOWSSTREAMTCPIP::ThreadConnexion(void* data)
{  	
	DIOWINDOWSSTREAMTCPIP* diostream = (DIOWINDOWSSTREAMTCPIP*)data;
	if(!diostream) return;	

	if(diostream->GetEvent()==DIOWINDOWSTCPIPFSMEVENT_NONE) // No hay nuevos Eventos
		{			
			switch(diostream->GetCurrentState())
				{
					case DIOWINDOWSTCPIPFSMSTATE_NONE								: break;	

					case DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNEXION		: if(diostream->handlesocket == INVALID_SOCKET)
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
																																		case -1: 	diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);																																																																																																											
																																							break;

																																		case  1:	diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_CONNECTED);
																																							diostream->status = DIOSTREAMSTATUS_CONNECTED;																																						
																																							break;

																																		default:	diostream->status = DIOSTREAMSTATUS_GETTINGCONNEXION;
																																							break;
																																	}
																															}
																														break;

					case DIOWINDOWSTCPIPFSMSTATE_CONNECTED				  : break;

					case DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD			: {	struct timeval	waitd;
																															fd_set					read_flags;
																															fd_set					write_flags;
																																																																																					
																															if(diostream->handlesocket==INVALID_SOCKET) 
																																{
																																	diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
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
																															if((error==SOCKET_ERROR))   //Socket ready for reading
																																{																																	
																																	diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
																																	break;
																																}
																													
																															if(FD_ISSET(diostream->handlesocket, &read_flags))  //Socket ready for reading
																																{	
																																	XBYTE buffer[DIOSTREAM_MAXBUFFER];

																																	FD_CLR(diostream->handlesocket, &read_flags);																															
																															
																																	memset(buffer, 0, DIOSTREAM_MAXBUFFER);

										      																				int size = recv(diostream->handlesocket,(char*)buffer, DIOSTREAM_MAXBUFFER, 0);
																																	if(size==SOCKET_ERROR)
																																		{
																																			diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
																																			break;
																																		}
																													 		 
																																	if(size) 
																																		{
																																			diostream->inbuffer->Add(buffer,size);
																																		}
																																	 else
																																		{																																			
																																			diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
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
																																			int size = send(diostream->handlesocket,(char*)diostream->outbuffer->Get(), esize,0);
																																			diostream->outbuffer->SetBlocked(false);
																																			if(size==SOCKET_ERROR) 
																																				{					
																																					diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);	
																																					break;
																																				} 

																																			if(size) 
																																				{
																																					diostream->outbuffer->Extract(NULL, 0 ,size);	
																																				}
																																		}																																																															  																															    																																																									
																																}																																																																																																																																													 																													
																														}																													
																														break;

					case DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA				: break                                                                                   ;

					case DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING			: break;
																			
				}
		}
	 else
		{
			if(diostream->GetEvent()<DIOWINDOWSTCPIP_LASTEVENT)
				{
					diostream->CheckTransition();

					switch(diostream->GetCurrentState())
						{
							case DIOWINDOWSTCPIPFSMSTATE_NONE								: break;	

							case DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNEXION		: if(diostream->enumservers) 
																																	{
																																		if(diostream->enumservers->GetXTimer()) diostream->enumservers->GetXTimer()->AddSeconds(DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL);
																																	}

																																if(diostream->config->IsServer())
																																	{
																																		diostream->config->SetCounterMultiServer(diostream->config->GetCounterMultiServer()+1);
																																	}
																																break;

							case DIOWINDOWSTCPIPFSMSTATE_CONNECTED				  : { DIOSTREAMXEVENT xevent(diostream, DIOSTREAMXEVENTTYPE_CONNECTED);								
																																	xevent.SetDIOStream(diostream);		
																																	diostream->PostEvent(&xevent);

																																	diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_WAITINGTOREAD);
																																}
																																break;

							case DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD			: break;

							case DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA				: break;

							case DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING			: { DIOSTREAMXEVENT xevent(diostream, DIOSTREAMXEVENTTYPE_DISCONNECTED);								
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
