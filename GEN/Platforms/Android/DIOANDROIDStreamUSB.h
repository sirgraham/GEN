//------------------------------------------------------------------------------------------
//	DIOANDROIDSTREAMUSB.H
//	
/**	
// \class 
//   
//  ANDROID Data IO Stream USB class
//   
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOANDROIDSTREAMUSB_H_
#define _DIOANDROIDSTREAMUSB_H_
	

#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XFSMachine.h"

#include "DIOStreamUSB.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOANDROIDUSBFSMEVENTS
{
	DIOANDROIDUSBFSMEVENT_NONE							 = 0  ,
	DIOANDROIDUSBFSMEVENT_CONNECTED							,
	DIOANDROIDUSBFSMEVENT_WAITINGTOREAD					,
	DIOANDROIDUSBFSMEVENT_SENDINGDATA						,
	DIOANDROIDUSBFSMEVENT_DISCONNECTING					,		

	DIOANDROIDUSB_LASTEVENT

};


enum DIOANDROIDUSBFSMSTATES
{
	DIOANDROIDUSBFSMSTATE_NONE							 = 0  ,	

	DIOANDROIDUSBFSMSTATE_CONNECTED							,
	DIOANDROIDUSBFSMSTATE_WAITINGTOREAD					,
	DIOANDROIDUSBFSMSTATE_SENDINGDATA						,
	DIOANDROIDUSBFSMSTATE_DISCONNECTING					,		

	DIOANDROIDUSB_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREAD;
class DIOFACTORY;
class DIOSTREAMCONFIG;


class DIOANDROIDSTREAMUSB : public DIOSTREAMUSB , public XFSMACHINE 
{
	public:
													DIOANDROIDSTREAMUSB							( );
		virtual							 ~DIOANDROIDSTREAMUSB							();

		DIOSTREAMSTATUS				GetConnectStatus							();		
		
		bool									Open													();		

		bool									Disconnect										()																{	return false; };
		bool									Close													();	

		bool									CleanBuffers									(); 											
				
	protected:

    void									Clean													();

		static void 					ThreadConnexion								(void* data);

		XDWORD								ReadBuffer										(XBYTE* buffer,XDWORD size);
		XDWORD								WriteBuffer										(XBYTE* buffer,XDWORD size);

		XTHREAD*							threadconnexion;
    
    int 									fd;

		int										readtimeout;
		int										writetimeout;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif





