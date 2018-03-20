//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES.H
//
/**
// \class
//
//  WINDOWS Data IO Stream Bluetooth Local Enum Devices class
//
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES_H_
#define _DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamBluetoothLocalEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;

class DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES :  public DIOSTREAMBLUETOOTHLOCALENUMDEVICES
{
	public:
															DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES			();
		virtual									 ~DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES			();

		bool											Search																				();		
	
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif