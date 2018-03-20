//------------------------------------------------------------------------------------------
//	DIOLINUXSTREAMWIFIREMOTEENUMDEVICES.H
//
/**
// \class
//
//  LINUX Data IO Stream Wifi Remote Enum Devices class
//
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMWIFIREMOTEENUMDEVICES_H_
#define _DIOLINUXSTREAMWIFIREMOTEENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOWIFI_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"
#include "DIOMAC.h"
#include "DIOStreamWifiRemoteEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOLINUXWIFIENUMFSMFSMEVENTS
{
	DIOLINUXWIFIENUMFSMEVENT_NONE							= 0 ,
	DIOLINUXWIFIENUMFSMEVENT_SEARCH								,	
	DIOLINUXWIFIENUMFSMEVENT_SEARCHEND						,
		
	DIOLINUXWIFIENUM_LASTEVENT
};


enum DIOLINUXWIFIENUMFSMSTATES
{
	DIOLINUXWIFIENUMFSMSTATE_NONE							= 0 ,	
	DIOLINUXWIFIENUMFSMSTATE_SEARCH   						,
	DIOLINUXWIFIENUMFSMSTATE_SEARCHEND						,

	DIOLINUXWIFIENUM_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;
class DIOSTREAMDEVICEWIFI;



class DIOLINUXSTREAMWIFIREMOTEENUMDEVICES :  public DIOSTREAMWIFIREMOTEENUMDEVICES, public XFSMACHINE
{
	public:
															DIOLINUXSTREAMWIFIREMOTEENUMDEVICES							();
		virtual									 ~DIOLINUXSTREAMWIFIREMOTEENUMDEVICES							();

		bool											Search																					();	
		bool											StopSearch																			(bool waitend);
		bool											IsSearching																			();

	private:

		void											Clean																						();	
				
		static void								ThreadEnumDevices																(void* thread);	

		XTHREADCOLLECTED*					threadenumdevices;	
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif