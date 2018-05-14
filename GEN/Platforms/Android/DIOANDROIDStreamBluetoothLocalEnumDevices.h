//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHLOCALENUMDEVICES.H
//
/**
// \class
//
//  ANDROID Data IO Stream Bluetooth Local Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMBLUETOOTHLOCALENUMDEVICES_H_
#define _DIOANDROIDSTREAMBLUETOOTHLOCALENUMDEVICES_H_


#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamBluetoothLocalEnumDevices.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;


class DIOANDROIDSTREAMBLUETOOTHLOCALENUMDEVICES :  public DIOSTREAMBLUETOOTHLOCALENUMDEVICES
{
  public:
                              DIOANDROIDSTREAMBLUETOOTHLOCALENUMDEVICES     ( );
    virtual                  ~DIOANDROIDSTREAMBLUETOOTHLOCALENUMDEVICES     ();

    bool                      Search                                      ();

};




//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif