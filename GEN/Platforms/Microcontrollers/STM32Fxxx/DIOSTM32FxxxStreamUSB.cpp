//------------------------------------------------------------------------------------------
//	DIOSTM32FXXXSTREAMUSB.CPP
//	
//	STM32Fxxx Data IO Stream USB class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSTM32Fxxx_HAL.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"

#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStreamXEvent.h"
#include "DIOStreamUSBConfig.h"

#include "DIOSTM32FxxxStreamUSB.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------

DIOSTM32FXXXSTREAMUSB*			diostreamusbptrhandle =  NULL; 
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUSB::DIOSTM32FXXXSTREAMUSB
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
DIOSTM32FXXXSTREAMUSB::DIOSTM32FXXXSTREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
{
	Clean();	
		  
	AddState(	DIOSTM32FXXXUSBFSMSTATE_NONE							, 												 
						DIOSTM32FXXXUSBFSMEVENT_CONNECTED				,	DIOSTM32FXXXUSBFSMSTATE_CONNECTED			 	 ,						
						DIOSTM32FXXXUSBFSMEVENT_DISCONNECTING		,	DIOSTM32FXXXUSBFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);

	AddState(	DIOSTM32FXXXUSBFSMSTATE_CONNECTED				, 												 							
						DIOSTM32FXXXUSBFSMEVENT_WAITINGTOREAD		,	DIOSTM32FXXXUSBFSMSTATE_WAITINGTOREAD		 ,						
						DIOSTM32FXXXUSBFSMEVENT_DISCONNECTING		,	DIOSTM32FXXXUSBFSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOSTM32FXXXUSBFSMSTATE_WAITINGTOREAD		, 												 								
						DIOSTM32FXXXUSBFSMEVENT_CONNECTED				, DIOSTM32FXXXUSBFSMSTATE_CONNECTED			 	 ,												
						DIOSTM32FXXXUSBFSMEVENT_DISCONNECTING		,	DIOSTM32FXXXUSBFSMSTATE_DISCONNECTING		 ,									
						EVENTDEFEND);	

	AddState(	DIOSTM32FXXXUSBFSMSTATE_DISCONNECTING		, 												 								
						DIOSTM32FXXXUSBFSMEVENT_CONNECTED				,	DIOSTM32FXXXUSBFSMSTATE_CONNECTED			 	 ,						
						DIOSTM32FXXXUSBFSMEVENT_WAITINGTOREAD		,	DIOSTM32FXXXUSBFSMSTATE_WAITINGTOREAD		 ,						
						EVENTDEFEND);	

	threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, __L("DIOSTM32FXXXSTREAMUSB::DIOSTM32FXXXSTREAMUSB"), ThreadConnexion, (void*)this);								
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUSB::~DIOSTM32FXXXSTREAMUSB
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
DIOSTM32FXXXSTREAMUSB::~DIOSTM32FXXXSTREAMUSB()
{
	Close();

	if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, threadconnexion);	
	
	Clean();
}





//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUSB::GetConnectStatus
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			DIOSTREAMSTATUS :
*/
//-------------------------------------------------------------------
DIOSTREAMSTATUS DIOSTM32FXXXSTREAMUSB::GetConnectStatus()
{	
	if(!config)	return DIOSTREAMSTATUS_DISCONNECTED;
	
	return status;
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUSB::Open
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMUSB::Open()
{	    
	if(!threadconnexion)	return false;
  
  MX_USB_DEVICE_Init();

  diostreamusbptrhandle = this; 
	  
	SetEvent(DIOSTM32FXXXUSBFSMEVENT_CONNECTED);

	status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();		
	ResetConnexionStatistics();
    
	return threadconnexion->Ini();
}




/*-------------------------------------------------------------------
//	DIOSTM32FXXXSTREAMUSB::WriteDirect
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2015 13:25:02
//	
//	@return 			XDWORD : 
//
//  @param				buffer : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTM32FXXXSTREAMUSB::WriteDirect(XBYTE* buffer, XDWORD size)
{	
	if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;
  
  if(CDC_Transmit_FS((uint8_t*)buffer, (XWORD)size) == USBD_OK) return size;
  	
	return (XDWORD)0;
}




/*-------------------------------------------------------------------
//	DIOSTM32FXXXSTREAMUSB::Write
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/02/2017 11:37:04
//	
//	@return 			XDWORD : 
//
//  @param				buffer : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTM32FXXXSTREAMUSB::Write(XBYTE* buffer, XDWORD size)
{      
	if(!outbuffer) return 0;
  
  while(outbuffer->IsBlocked())
    {
    
    }
  
	if(!outbuffer->Add(buffer,size))	return 0;
    
	nbyteswrite+=size;
	if(xtimernotactivity) xtimernotactivity->Reset();
  
  outbuffer->SetBlocked(false);
  	
	return size;
}



//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUSB::Close
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMUSB::Close()
{
	if(!threadconnexion) return false;
	
	threadconnexion->End();

	if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;
    
  diostreamusbptrhandle = NULL;
	
	return true;
}





//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUSB::CleanBuffers
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMUSB::CleanBuffers()
{

	return false;
}



/*-------------------------------------------------------------------
//	DIOSTM32FXXXSTREAMUSB::HAL_USB_RxCpltCallback
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			05/08/2016 14:27:24
//	
*/
/*-----------------------------------------------------------------*/
void DIOSTM32FXXXSTREAMUSB::HAL_USB_RxCpltCallback(uint8_t* buffer, uint32_t *len)
{    
  if(GetConnectStatus() == DIOSTREAMSTATUS_DISCONNECTED) return;  
  if(IsBlockRead())                                      return; 
   
  if(!inbuffer->IsBlocked())
    { 
      if(readcache.GetSize())
        {
          inbuffer->Add(readcache);	
          readcache.Delete();
        } 
      
      inbuffer->Add((XBYTE*)buffer, (XDWORD)(*len));	
    }
   else
    {
      readcache.Add((XBYTE*)buffer, (XDWORD)(*len));
    } 
}

 


/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUSB::ThreadConnexion
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
void DIOSTM32FXXXSTREAMUSB::ThreadConnexion(void* param)
{			
	DIOSTM32FXXXSTREAMUSB* diostream = (DIOSTM32FXXXSTREAMUSB*)param;
	if(!diostream) return;	
    	
	if(diostream->GetEvent()==DIOSTM32FXXXUSBFSMEVENT_NONE) // No hay nuevos Eventos
		{			
			switch(diostream->GetCurrentState())
				{
					case DIOSTM32FXXXUSBFSMSTATE_NONE											: break;

					case DIOSTM32FXXXUSBFSMSTATE_CONNECTED								: break;

					case DIOSTM32FXXXUSBFSMSTATE_WAITINGTOREAD						: { if(!diostream->IsBlockRead())
                                                                      {   
                                                                        if(!diostream->inbuffer->IsBlocked())
                                                                          {                                                                                                            
                                                                            if(diostream->readcache.GetSize())
                                                                              {
                                                                                diostream->inbuffer->Add(diostream->readcache);	
                                                                                diostream->readcache.Delete();
                                                                              }                                                                                                                                                                      
                                                                          }
                                                                      }
                                                                      
																																		if(!diostream->IsBlockWrite())
																																			{
																																				if(!diostream->outbuffer->IsBlocked())
                                                                          {                                                                                                                                                            
                                                                            int esize = diostream->outbuffer->GetSize();
                                                                            if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;
                                                                            
                                                                            if(esize) 
                                                                              {
                                                                                diostream->outbuffer->Get(diostream->writebuffer, esize, 0);
                                                                                if(diostream->WriteDirect(diostream->writebuffer, esize))
                                                                                  {
                                                                                    diostream->outbuffer->Extract(NULL, 0, esize);
                                                                                  }
                                                                              }                                                                                                                                                                                                                                  
																																					}																																				
																																			}
																																	}
																																	break;
					
					case DIOSTM32FXXXUSBFSMSTATE_DISCONNECTING						: break;
																			
				}
		}
	 else
		{
			if(diostream->GetEvent()<DIOSTM32FXXXUSB_LASTEVENT)
				{
					diostream->CheckTransition();

					switch(diostream->GetCurrentState())
						{
							case DIOSTM32FXXXUSBFSMSTATE_NONE								: break;	
						
							case DIOSTM32FXXXUSBFSMSTATE_CONNECTED					  : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);								
																																		xevent.SetDIOStream(diostream);		
																																		diostream->PostEvent(&xevent);
																																	
																																		diostream->SetEvent(DIOSTM32FXXXUSBFSMEVENT_WAITINGTOREAD);
																																	}
																																	break;

							case DIOSTM32FXXXUSBFSMSTATE_WAITINGTOREAD				: break;
							
							case DIOSTM32FXXXUSBFSMSTATE_DISCONNECTING				: { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);								
																																		xevent.SetDIOStream(diostream);		
																																		diostream->PostEvent(&xevent);
																																										
																																		diostream->status = DIOSTREAMSTATUS_DISCONNECTED;																															
																																	}
																																	break;
						}
				}
		}	
}



#endif



