//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMI2C.CPP
//	
//	WINDOWS Data IO Stream I2C class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamI2CConfig.h"

#include "DIOWINDOWSStreamI2C.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	


/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMI2C::DIOWINDOWSSTREAMI2C
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:51:23
//	
//	@return 			

 
 
*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMI2C::DIOWINDOWSSTREAMI2C() : DIOSTREAMI2C()
{
	threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, __L("DIOWINDOWSSTREAMI2C::DIOWINDOWSSTREAMI2C"), ThreadConnexion, (void*)this);	
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMI2C::~DIOWINDOWSSTREAMI2C
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:19:33
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOWINDOWSSTREAMI2C::~DIOWINDOWSSTREAMI2C()
{ 
	if(threadconnexion) 
		{
			threadconnexion->End();
			DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, threadconnexion);			
			threadconnexion = NULL;
		}
}





/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMI2C::Open
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/10/2012 10:03:42
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMI2C::Open()
{
	if(!config) return false;
  
	if(!threadconnexion)	return false;

	ResetXBuffers();

	ResetConnexionStatistics();
	
	return threadconnexion->Ini();	
}




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMI2C::Close
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/10/2012 10:03:47
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMI2C::Close()
{	
	if(!threadconnexion) return false;	

	threadconnexion->End();
  
	return true;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMI2C::ThreadConnexion
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/09/2012 16:59:27
//	
//	@return 			void : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMI2C::ThreadConnexion(void* data)
{  	
	DIOWINDOWSSTREAMI2C* diostream = (DIOWINDOWSSTREAMI2C*)data;
	if(!diostream) return;	

	if(diostream->GetEvent()==DIOSTREAMI2C_FSMEVENT_NONE) // No hay nuevos Eventos
		{			
			switch(diostream->GetCurrentState())
				{
					case DIOSTREAMI2C_FSMSTATE_NONE											: break;

					case DIOSTREAMI2C_FSMSTATE_CONNECTED								: break;

					case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD						: /*
																																		{ if(diostream->outbuffer->IsBlocked()) break;
						
																																		while(1)
																																			{
																																				XDWORD size  = diostream->outbuffer->GetSize();																												

																																				if(!size) break;
																																				if(size>(XDWORD)DIOWINDOWSSTREAMI2C_MAXSIZEBUFFER) size = DIOWINDOWSSTREAMI2C_MAXSIZEBUFFER;		
																													
																																				memset(diostream->bufferread  , 0 , DIOWINDOWSSTREAMI2C_MAXSIZEBUFFER);
																																				memset(diostream->bufferwrite , 0 , DIOWINDOWSSTREAMI2C_MAXSIZEBUFFER);
																													
																																				diostream->outbuffer->Extract(diostream->bufferwrite,0,size);
																																				diostream->TransferBuffer(diostream->bufferread,diostream->bufferwrite,size);																																																									 																																														
																																
																																				if(!(diostream->config->IsOnlyWrite())) diostream->inbuffer->Add(diostream->bufferread,size);																												
																																			}		
																																	}
																																	*/
																																	break;


					case DIOSTREAMI2C_FSMSTATE_SENDINGDATA							: break;

					case DIOSTREAMI2C_FSMSTATE_DISCONNECTING						: break;
																			
				}
		}
	 else
		{
			if(diostream->GetEvent()<DIOSTREAMI2C_LASTEVENT)
				{
					diostream->CheckTransition();

					switch(diostream->GetCurrentState())
						{
							case DIOSTREAMI2C_FSMSTATE_NONE									: break;	
						
							case DIOSTREAMI2C_FSMSTATE_CONNECTED						: { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);								
																																	xevent.SetDIOStream(diostream);		
																																	diostream->PostEvent(&xevent);
																																	
																																	diostream->SetEvent(DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD);
																																}
																																break;

							case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD				: break;

							case DIOSTREAMI2C_FSMSTATE_SENDINGDATA					: break;

							case DIOSTREAMI2C_FSMSTATE_DISCONNECTING				: { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);								
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

