//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMBLUETOOTH.H
//
/**
// \class
//
//  WINDOWS Data IO Stream Bluetooth class
//
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMBLUETOOTH_H_
#define _DIOWINDOWSSTREAMBLUETOOTH_H_


#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"

#include "DIOStreamBluetooth.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOWINDOWSBTFSMEVENTS
{
	DIOWINDOWSBTFSMEVENT_NONE								= 0 ,
	DIOWINDOWSBTFSMEVENT_GETTINGCONNEXION				,
	DIOWINDOWSBTFSMEVENT_CONNECTED							,
	DIOWINDOWSBTFSMEVENT_WAITINGTOREAD					,
	DIOWINDOWSBTFSMEVENT_SENDINGDATA						,
	DIOWINDOWSBTFSMEVENT_DISCONNECTING					,	

	DIOWINDOWSBT_LASTEVENT
};


enum DIOWINDOWSBTFSMSTATES
{
	DIOWINDOWSBTFSMSTATE_NONE								= 0 ,	
	DIOWINDOWSBTFSMSTATE_GETTINGCONNEXION				,
	DIOWINDOWSBTFSMSTATE_CONNECTED							,
	DIOWINDOWSBTFSMSTATE_WAITINGTOREAD					,
	DIOWINDOWSBTFSMSTATE_SENDINGDATA						,
	DIOWINDOWSBTFSMSTATE_DISCONNECTING					,	

	DIOWINDOWSBT_LASTSTATE
};


#define BTHNS_RESULT_DEVICE_CONNECTED			 0x00010000
#define BTHNS_RESULT_DEVICE_REMEMBERED 		 0x00020000
#define BTHNS_RESULT_DEVICE_AUTHENTICATED  0x00040000

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTHREADCOLLECTED;
class DIODEVICEBLUETOOTH;


class DIOWINDOWSSTREAMBLUETOOTH : public DIOSTREAMBLUETOOTH , public XFSMACHINE
{
	public:
															DIOWINDOWSSTREAMBLUETOOTH						();
		virtual									 ~DIOWINDOWSSTREAMBLUETOOTH						();

		bool											Open																();
		bool											Disconnect													();
		bool											Close																();
		
		int												IsReadyConnect											(SOCKET socket);
				
	protected:

		bool											SDP_RegisterService									(bool reg,char* service_name,char* service_dsc,char* service_prov,void* addr);
	    
	private:
		
		void											Clean																();
		
		static void 							ThreadConnexion											(void* param);
		
		XTHREADCOLLECTED*					threadconnexion;	

		SOCKET										handlesocket;
		void*											addr;		
		SOCKET										handleserver;		
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif



