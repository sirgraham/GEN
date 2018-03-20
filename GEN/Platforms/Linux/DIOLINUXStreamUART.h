//------------------------------------------------------------------------------------------
//	DIOLINUXSTREAMUART.H
//	
/**	
// \class 
//   
//  LINUX Data IO Stream UART class
//   
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOLINUXSTREAMUART_H_
#define _DIOLINUXSTREAMUART_H_
	
#if defined(DIO_ACTIVE) && defined(DIOUART_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XFSMachine.h"

#include "DIOStreamUART.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOLINUXUARTFSMEVENTS
{
	DIOLINUXUARTFSMEVENT_NONE							 = 0  ,
	DIOLINUXUARTFSMEVENT_CONNECTED							,
	DIOLINUXUARTFSMEVENT_WAITINGTOREAD					,
	DIOLINUXUARTFSMEVENT_DISCONNECTING					,		

	DIOLINUXUART_LASTEVENT

};


enum DIOLINUXUARTFSMSTATES
{
	DIOLINUXUARTFSMSTATE_NONE							 = 0  ,	

	DIOLINUXUARTFSMSTATE_CONNECTED							,
	DIOLINUXUARTFSMSTATE_WAITINGTOREAD					,
	DIOLINUXUARTFSMSTATE_DISCONNECTING					,		

	DIOLINUXUART_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTHREADCOLLECTED;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;


class DIOLINUXSTREAMUART : public DIOSTREAMUART , public XFSMACHINE 
{
	public:
													DIOLINUXSTREAMUART						();
		virtual							 ~DIOLINUXSTREAMUART						();
		
		DIOSTREAMSTATUS				GetConnectStatus							();		
		
		bool									Open													();	
		
		bool									Config												(XWORD mask = DIOSTREAMUARTMASK_ALL);	

		XDWORD								ReadDirect										(XBYTE* buffer, XDWORD size);
		XDWORD								WriteDirect										(XBYTE* buffer, XDWORD size);

		bool									Disconnect										()																{	return false; };
		bool									Close													();	
	
		bool									GetCTS												();
		bool									GetDSR												();
		bool									GetRing												();
		bool									GetRLSD												();

		bool									SetRTS												(bool on = true);
		bool									SetDTR												(bool on = true); 								

		bool									CleanBuffers									(); 											
				
	protected:

    void									Clean													();

		static void 					ThreadConnexion								(void* param);
	
		XTHREADCOLLECTED*			threadconnexion;
    
    int 									fd;

		int										readtimeout;
		int										writetimeout;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif





