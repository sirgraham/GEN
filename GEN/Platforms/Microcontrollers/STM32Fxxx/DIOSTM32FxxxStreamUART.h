//------------------------------------------------------------------------------------------
//	DIOSTM32FXXXSTREAMUART.H
//	
/**	
// \class 
//   
//  STM32Fxxx Data IO Stream UART class
//   
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTM32FXXXSTREAMUART_H_
#define _DIOSTM32FXXXSTREAMUART_H_
	
#if defined(DIO_ACTIVE) && defined(DIOUART_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XFSMachine.h"

#include "XSTM32Fxxx_HAL.h"

#include "DIOStreamUART.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOSTM32FXXXUARTFSMEVENTS
{
	DIOSTM32FXXXUARTFSMEVENT_NONE							 = 0  ,
	DIOSTM32FXXXUARTFSMEVENT_CONNECTED							,
	DIOSTM32FXXXUARTFSMEVENT_WAITINGTOREAD					,
	DIOSTM32FXXXUARTFSMEVENT_DISCONNECTING					,		

	DIOSTM32FXXXUART_LASTEVENT

};


enum DIOSTM32FXXXUARTFSMSTATES
{
	DIOSTM32FXXXUARTFSMSTATE_NONE							 = 0  ,	

	DIOSTM32FXXXUARTFSMSTATE_CONNECTED							,
	DIOSTM32FXXXUARTFSMSTATE_WAITINGTOREAD					,
	DIOSTM32FXXXUARTFSMSTATE_DISCONNECTING					,		

	DIOSTM32FXXXUART_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class XTHREADCOLLECTED;


class DIOSTM32FXXXSTREAMUART : public DIOSTREAMUART, public XFSMACHINE 
{
	public:
                                DIOSTM32FXXXSTREAMUART				();
		virtual							       ~DIOSTM32FXXXSTREAMUART				();
		
		DIOSTREAMSTATUS				      GetConnectStatus							();		
		
		bool									      Open													();	
		
		bool									      Config												(XWORD mask = DIOSTREAMUARTMASK_ALL);	

		XDWORD								      ReadDirect										(XBYTE* buffer, XDWORD size);
		XDWORD								      WriteDirect										(XBYTE* buffer, XDWORD size);
    
    XDWORD                      Write                         (XBYTE* buffer, XDWORD size);

		bool									      Disconnect										()																{	return false;													}
		bool									      Close													();	
	
		bool									      GetCTS												()																{	return false;													}
		bool									      GetDSR												()																{	return false;													}
		bool									      GetRing												()																{	return false;													}
		bool									      GetRLSD												()																{	return false;													}

		bool									      SetRTS												(bool on = true)									{	return false;													}	
		bool									      SetDTR												(bool on = true)									{	return false;													} 								

		bool									      CleanBuffers									(); 											
   
    UART_HandleTypeDef		      huart;	   
    XBYTE                       receiveddata;	   
    XBUFFER                     readcache[3]; 	
   
	protected:

    void									      Clean													()
                                {                           
																	threadconnexion   = NULL;				    
                                  indexport         = -1;
                                } 
		
		static void									ThreadConnexion								(void* param); 
    	
		XTHREADCOLLECTED*						threadconnexion;				    
    int                         indexport;    
    XBYTE                       writebuffer[DIOSTREAM_MAXBUFFER];
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------

extern DIOSTM32FXXXSTREAMUART*  diostreamuartptrhandle[3]; 


#endif

#endif





