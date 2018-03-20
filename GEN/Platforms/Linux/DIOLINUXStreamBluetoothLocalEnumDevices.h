//------------------------------------------------------------------------------------------
//	DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES.H
//
/**
// \class
//
//  LINUX Data IO Stream Bluetooth Local Enum Devices class
//
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES_H_
#define _DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES_H_


#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamBluetoothLocalEnumDevices.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;


class DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES :  public DIOSTREAMBLUETOOTHLOCALENUMDEVICES
{
	public:
															DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES			();
		virtual									 ~DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES			();

		bool											Search																			();		
	
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif