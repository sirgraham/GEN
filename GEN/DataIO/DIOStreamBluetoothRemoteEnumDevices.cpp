//------------------------------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHREMOTEENUMDEVICES.CPP
//
//  Data IO Stream Bluetooth REMOTE Enum Devices class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamBluetoothRemoteEnumDevices.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOSTREAMBLUETOOTHREMOTEENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 18:37:55
//
//  @return       void :


*/
/*-----------------------------------------------------------------*/
DIOSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOSTREAMBLUETOOTHREMOTEENUMDEVICES() : DIOSTREAMENUMDEVICES()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHREMOTEENUMDEVICES::~DIOSTREAMBLUETOOTHREMOTEENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 18:38:48
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMBLUETOOTHREMOTEENUMDEVICES::~DIOSTREAMBLUETOOTHREMOTEENUMDEVICES()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 18:42:16
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean()
{
  type = DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE;
}


