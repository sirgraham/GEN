//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES.H
//
/**
// \class
//
//  WINDOWS Data IO Stream Wifi Remote Enum Devices class
//
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES_H_
#define _DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOWIFI_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <Windows.h>


#include "XFSMachine.h"
#include "DIOStream.h"

#include "DIOStreamWifiRemoteEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOWINDOWSWIFIENUMFSMFSMEVENTS
{
	DIOWINDOWSWIFIENUMFSMEVENT_NONE							= 0 ,
	DIOWINDOWSWIFIENUMFSMEVENT_SEARCH								,	
	DIOWINDOWSWIFIENUMFSMEVENT_SEARCHEND						,
		
	DIOWINDOWSWIFIENUM_LASTEVENT
};


enum DIOWINDOWSWIFIENUMFSMSTATES
{
	DIOWINDOWSWIFIENUMFSMSTATE_NONE							= 0 ,	
	DIOWINDOWSWIFIENUMFSMSTATE_SEARCH								,
	DIOWINDOWSWIFIENUMFSMSTATE_SEARCHEND						,

	DIOWINDOWSWIFIENUM_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;


class DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES :  public DIOSTREAMWIFIREMOTEENUMDEVICES, public XFSMACHINE
{
	public:
															DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES						();
		virtual									 ~DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES						();

		bool											Search																					();	
		bool											StopSearch																			(bool waitend);
		bool											IsSearching																			();

	private:

		void											Clean																						();	

		static void								ThreadEnumDevices																(void* thread);	
		void											SearchServices																	();
	
		XTHREADCOLLECTED*					threadenumdevices;	
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif