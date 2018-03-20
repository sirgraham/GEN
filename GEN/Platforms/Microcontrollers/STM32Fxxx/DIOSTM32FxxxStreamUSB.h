//------------------------------------------------------------------------------------------
//	DIOSTM32FXXXSTREAMUSB.H
//	
/**	
// \class 
//   
//  STM32Fxxx Data IO Stream USB class
//   
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTM32FXXXSTREAMUSB_H_
#define _DIOSTM32FXXXSTREAMUSB_H_
	
#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "XBuffer.h"
#include "XFSMachine.h"

#include "XSTM32Fxxx_HAL.h"
#include "usb_device.h"

#include "DIOStreamUSB.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOSTM32FXXXUSBFSMEVENTS
{
	DIOSTM32FXXXUSBFSMEVENT_NONE							 = 0  ,
	DIOSTM32FXXXUSBFSMEVENT_CONNECTED							,
	DIOSTM32FXXXUSBFSMEVENT_WAITINGTOREAD					,
	DIOSTM32FXXXUSBFSMEVENT_DISCONNECTING					,		

	DIOSTM32FXXXUSB_LASTEVENT

};


enum DIOSTM32FXXXUSBFSMSTATES
{
	DIOSTM32FXXXUSBFSMSTATE_NONE							 = 0  ,	

	DIOSTM32FXXXUSBFSMSTATE_CONNECTED							,
	DIOSTM32FXXXUSBFSMSTATE_WAITINGTOREAD					,
	DIOSTM32FXXXUSBFSMSTATE_DISCONNECTING					,		

	DIOSTM32FXXXUSB_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class XTHREADCOLLECTED;


class DIOSTM32FXXXSTREAMUSB : public DIOSTREAMUSB, public XFSMACHINE 
{
	public:
                                DIOSTM32FXXXSTREAMUSB					();
		virtual							       ~DIOSTM32FXXXSTREAMUSB					();
		
		DIOSTREAMSTATUS				      GetConnectStatus							();		
		
		bool									      Open													();	
		
		XDWORD								      WriteDirect										(XBYTE* buffer, XDWORD size);
        
    XDWORD                      Write                         (XBYTE* buffer, XDWORD size);

		bool									      Disconnect										()																{	return false;													}
		bool									      Close													();	
		
		bool									      CleanBuffers									(); 	
    
    void                        HAL_USB_RxCpltCallback        (uint8_t* Buf, uint32_t *Len);
    
	protected:

    void									      Clean													()
                                {                           
																	threadconnexion   = NULL;				                                      
                                } 
		
		static void									ThreadConnexion								(void* param); 
    
    XBUFFER                     readcache;
    XBYTE                       writebuffer[DIOSTREAM_MAXBUFFER];
   
		XTHREADCOLLECTED*						threadconnexion;				        
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------

extern DIOSTM32FXXXSTREAMUSB*  diostreamusbptrhandle; 


#endif

#endif






