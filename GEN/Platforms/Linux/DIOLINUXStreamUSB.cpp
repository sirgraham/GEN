//------------------------------------------------------------------------------------------
//	DIOLINUXSTREAMUSB.CPP
//	
//	LINUX Data IO Stream USB class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <errno.h>
#include <termios.h>

#include "XFactory.h"
#include "XBase.h"
#include "XString.h"
#include "XDebug.h"
#include "XThreadCollected.h"

#include "DIOStreamXEvent.h"
#include "DIOStreamUSBConfig.h"

#include "DIOLINUXStreamUSB.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::DIOLINUXSTREAMUSB
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 23:11:51
//	
//	@return 			

 
 
*/
/*-----------------------------------------------------------------*/
DIOLINUXSTREAMUSB::DIOLINUXSTREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
{
	Clean();	

	AddState(	DIOLINUXUSBFSMSTATE_NONE							, 												 
						DIOLINUXUSBFSMEVENT_CONNECTED				,	DIOLINUXUSBFSMSTATE_CONNECTED			 	 ,						
						DIOLINUXUSBFSMEVENT_DISCONNECTING		,	DIOLINUXUSBFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);

	AddState(	DIOLINUXUSBFSMSTATE_CONNECTED				, 												 							
						DIOLINUXUSBFSMEVENT_WAITINGTOREAD		,	DIOLINUXUSBFSMSTATE_WAITINGTOREAD		 ,
						DIOLINUXUSBFSMEVENT_SENDINGDATA			,	DIOLINUXUSBFSMSTATE_SENDINGDATA			 ,					
						DIOLINUXUSBFSMEVENT_DISCONNECTING		,	DIOLINUXUSBFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOLINUXUSBFSMSTATE_WAITINGTOREAD		, 												 								
						DIOLINUXUSBFSMEVENT_CONNECTED				, DIOLINUXUSBFSMSTATE_CONNECTED			 	 ,												
						DIOLINUXUSBFSMEVENT_SENDINGDATA			,	DIOLINUXUSBFSMSTATE_SENDINGDATA			 ,		
						DIOLINUXUSBFSMEVENT_DISCONNECTING		,	DIOLINUXUSBFSMSTATE_DISCONNECTING		 ,									
						EVENTDEFEND);	

	AddState(	DIOLINUXUSBFSMSTATE_DISCONNECTING		, 												 								
						DIOLINUXUSBFSMEVENT_CONNECTED				,	DIOLINUXUSBFSMSTATE_CONNECTED			 	 ,						
						DIOLINUXUSBFSMEVENT_WAITINGTOREAD		,	DIOLINUXUSBFSMSTATE_WAITINGTOREAD		 ,
						DIOLINUXUSBFSMEVENT_SENDINGDATA			,	DIOLINUXUSBFSMSTATE_SENDINGDATA			 ,											
						EVENTDEFEND);	
						
	threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, __L("DIOLINUXSTREAMUSB::DIOLINUXSTREAMUSB"), ThreadConnexion, (void*)this);
}


//-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::~DIOLINUXSTREAMUSB
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
DIOLINUXSTREAMUSB::~DIOLINUXSTREAMUSB()
{
	Close();

	if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, threadconnexion);

	Clean();
}




//-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::GetConnectStatus
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			DIOSTREAMSTATUS :
*/
//-------------------------------------------------------------------
DIOSTREAMSTATUS DIOLINUXSTREAMUSB::GetConnectStatus()
{
	if(fd<0)		return DIOSTREAMSTATUS_DISCONNECTED;	
	if(!config)	return DIOSTREAMSTATUS_DISCONNECTED;

	return status;
}


//-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::Open
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
*/
//-------------------------------------------------------------------
bool DIOLINUXSTREAMUSB::Open()
{
	if(!config)	return false;
	
	XSTRING_CREATEOEM((*config->GetResource()), charOEM)	
	fd= open(charOEM, O_RDWR | O_NOCTTY | O_NONBLOCK);
	XSTRING_DELETEOEM(charOEM)	

  if(fd<0) return false;

	//fcntl(fd, F_SETFL, FNDELAY);
	//fcntl(fd, F_SETFL, 0);
  
	SetEvent(DIOLINUXUSBFSMEVENT_CONNECTED);

	status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();		

	ResetConnexionStatistics();

	return threadconnexion->Ini();
}




//-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::Close
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
bool DIOLINUXSTREAMUSB::Close()
{
	if(!threadconnexion) return false;
	
	threadconnexion->End();

	if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;
	
	if(fd>=0) 
		{
			close(fd);
			fd = -1;
		}
 	
	return true;
}





//-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::CleanBuffers
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
bool DIOLINUXSTREAMUSB::CleanBuffers()
{
	if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

	return true;
}



//-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::Clean
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
void DIOLINUXSTREAMUSB::Clean()
{
	fd						= -1;
	readtimeout		= 3000;
	writetimeout	= 3000;	
}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::ThreadConnexion
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2012 13:56:19
//	
//	@return 			void : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
void DIOLINUXSTREAMUSB::ThreadConnexion(void* data)
{  	
	DIOLINUXSTREAMUSB* diostream = (DIOLINUXSTREAMUSB*)data;
	if(!diostream) return;	

	if(diostream->GetEvent()==DIOLINUXUSBFSMEVENT_NONE) // No hay nuevos Eventos
		{			
			switch(diostream->GetCurrentState())
				{
					case DIOLINUXUSBFSMSTATE_NONE										: break;

					case DIOLINUXUSBFSMSTATE_CONNECTED								: break;

					case DIOLINUXUSBFSMSTATE_WAITINGTOREAD						: { 
																																if(!diostream->IsBlockRead()) 
																																	{
																																		XBYTE buffer[DIOSTREAM_MAXBUFFER];

																																		XDWORD br = diostream->ReadBuffer(buffer, DIOSTREAM_MAXBUFFER);
																																		if(br) 
																																			{
																																				//XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(2), (XBYTE*)diostream->buffer, (int)br);		
																																				diostream->inbuffer->Add(buffer, br);
																																			}
																																	}

																																if(!diostream->IsBlockWrite()) 
																																	{
																																		int esize = diostream->outbuffer->GetSize();																																																													
																																		if(esize)
																																			{	
																																				if(esize > DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;							
																																																																								
																																				diostream->outbuffer->SetBlocked(true);
																																				XDWORD bw = diostream->WriteBuffer(diostream->outbuffer->Get(), esize);
																																				diostream->outbuffer->SetBlocked(false);

																																				if(bw) 
																																					{
																																						//XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(4), (XBYTE*)diostream->buffer, (int)bw);
																																						diostream->outbuffer->Extract(NULL , 0, bw);																																				
																																					} 
																																				 else 
																																					{
																																						//XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(3), (XBYTE*)diostream->buffer, (int)bw);
																																					}
																																			}	
																																	}
																															}
																															break;


					case DIOLINUXUSBFSMSTATE_SENDINGDATA							: break;

					case DIOLINUXUSBFSMSTATE_DISCONNECTING						: break;
																			
				}
		}
	 else
		{
			if(diostream->GetEvent()<DIOLINUXUSB_LASTEVENT)
				{
					diostream->CheckTransition();

					switch(diostream->GetCurrentState())
						{
							case DIOLINUXUSBFSMSTATE_NONE								: break;	
						
							case DIOLINUXUSBFSMSTATE_CONNECTED					  : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);								
																																xevent.SetDIOStream(diostream);		
																																diostream->PostEvent(&xevent);
																																	
																																diostream->SetEvent(DIOLINUXUSBFSMEVENT_WAITINGTOREAD);
																															}
																															break;

							case DIOLINUXUSBFSMSTATE_WAITINGTOREAD				: break;

							case DIOLINUXUSBFSMSTATE_SENDINGDATA					: break;

							case DIOLINUXUSBFSMSTATE_DISCONNECTING				: { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);								
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





//-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::ReadBuffer
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			XDWORD :
//  @param				buffer :
//	@param				size :
*/
//-------------------------------------------------------------------
XDWORD DIOLINUXSTREAMUSB::ReadBuffer(XBYTE* buffer,XDWORD size)
{
	if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  fd_set				 fds;		
  struct timeval tv;		

	tv.tv_sec  = 0;	 
	tv.tv_usec = readtimeout;
	
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	int br = 0;
	
	if(select(fd+1, &fds, NULL, NULL, &tv) > 0)
	  {
			br = read(fd, buffer, size);

			if(br< 0)
				{
					return 0;					
				}                    
		}
	
	return (XDWORD)br;
}




//-------------------------------------------------------------------
//  DIOLINUXSTREAMUSB::Write
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			XDWORD :
//  @param				buffer :
//	@param				size :
*/
//-------------------------------------------------------------------
XDWORD DIOLINUXSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
{
	if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

	fd_set				 fds;		
  struct timeval tv;		

	tv.tv_sec  = 0;	 
	tv.tv_usec = writetimeout;
	
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	int bw = 0;
	
	if(select(fd+1, NULL , &fds, NULL, &tv) > 0)
	  {
			bw = write(fd, buffer, size);
			if(bw<0) return 0;
		}
		                    	 
	return (XDWORD)bw;
}



#endif