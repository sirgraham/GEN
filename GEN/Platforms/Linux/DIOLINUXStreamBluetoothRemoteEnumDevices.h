//------------------------------------------------------------------------------------------
//	DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES.H
//
/**
// \class
//
//  LINUX Data IO Stream Bluetooth Remote Enum Devices class
//
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES_H_
#define _DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

#include "XFSMachine.h"
#include "DIOMAC.h"
#include "DIOStreamBluetoothRemoteEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOLINUXBTENUMFSMFSMEVENTS
{
	DIOLINUXBTENUMFSMEVENT_NONE							= 0 ,
	DIOLINUXBTENUMFSMEVENT_SEARCHMAC						,	
	DIOLINUXBTENUMFSMEVENT_SEARCHSERVICES				,
	DIOLINUXBTENUMFSMEVENT_SEARCHNAME						,
	DIOLINUXBTENUMFSMEVENT_SEARCHEND						,
		
	DIOLINUXBTENUM_LASTEVENT
};


enum DIOLINUXBTENUMFSMSTATES
{
	DIOLINUXBTENUMFSMSTATE_NONE							= 0 ,	
	DIOLINUXBTENUMFSMSTATE_SEARCHMAC						,
	DIOLINUXBTENUMFSMSTATE_SEARCHSERVICES				,
	DIOLINUXBTENUMFSMSTATE_SEARCHNAME						,
	DIOLINUXBTENUMFSMSTATE_SEARCHEND						,

	DIOLINUXBTENUM_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;
class DIOSTREAMDEVICEBLUETOOTH;


struct search_context 
{
	char*				svc;			/* Service */
	uuid_t			group;		/* Browse group */
	int					tree;			/* Display full attribute tree */
	uint32_t		handle;		/* Service record handle */
};


class DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES :  public DIOSTREAMBLUETOOTHREMOTEENUMDEVICES, public XFSMACHINE
{
	public:
															DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES				();
		virtual									 ~DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES				();

		bool											Search																					();	
		bool											StopSearch																			(bool waitend);
		bool											IsSearching																			();

	private:

		void											Clean																						();	
		
		bool											GetAddrFromLocalDevice													(int localdeviceindex, char* btaddr);

		int												IsReadyConnect																	(int socket);

		void											SearchServices																	();
		bool											ScanDevices																			(XVECTOR<DIOSTREAMDEVICE*>* devices);
		bool											ScanDevicesName																	(DIOSTREAMDEVICEBLUETOOTH* device);
		bool											ScanDevicesServices															(XVECTOR<DIOSTREAMDEVICE*>* devices);
		bool											ScanDeviceServices															(DIOSTREAMDEVICEBLUETOOTH* device, struct search_context* context);

		static void								ThreadEnumDevices																(void* thread);	

		XTHREADCOLLECTED*					threadenumdevices;	
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif