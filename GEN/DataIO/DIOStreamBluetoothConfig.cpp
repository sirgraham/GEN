//------------------------------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG.CPP
//
//  Data IO Stream Bluetooth Config class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------


#include "DIOStreamBluetoothConfig.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::DIOSTREAMBLUETOOTHCONFIG
*/
/**
//
//  Class Constructor DIOSTREAMBLUETOOTHCONFIG
//
//  @author       Abraham J. Velez
//  @version      29/05/2014 17:40:30
//

*/
/*-----------------------------------------------------------------*/
DIOSTREAMBLUETOOTHCONFIG::DIOSTREAMBLUETOOTHCONFIG() : DIOSTREAMCONFIG()
{
  Clean();

  type = DIOSTREAMTYPE_BLUETOOTH;
}


//-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::~DIOSTREAMBLUETOOTHCONFIG
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:15:01
//
//  @return
//  */
//-------------------------------------------------------------------
DIOSTREAMBLUETOOTHCONFIG::~DIOSTREAMBLUETOOTHCONFIG()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::GetDeviceSelect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2013 18:33:18
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOSTREAMBLUETOOTHCONFIG::GetDeviceSelect()
{
  return deviceselect;
}




/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::SetDeviceSelect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2013 18:33:05
//
//  @return       bool :
//  @param        deviceselect :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMBLUETOOTHCONFIG::SetDeviceSelect(int deviceselect)
{
  this->deviceselect = deviceselect;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::GetLocalMAC
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2013 18:17:58
//
//  @return       DIOMAC* :
//  */
/*-----------------------------------------------------------------*/
DIOMAC* DIOSTREAMBLUETOOTHCONFIG::GetLocalMAC()
{
  return &localMAC;
}





/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::GetRemoteMAC
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2013 18:17:58
//
//  @return       DIOMAC* :
//  */
/*-----------------------------------------------------------------*/
DIOMAC* DIOSTREAMBLUETOOTHCONFIG::GetRemoteMAC()
{
  return &remoteMAC;
}



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::GetRemoteChannel
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2013 18:17:50
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOSTREAMBLUETOOTHCONFIG::GetRemoteChannel()
{
  return channel;
}



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::SetChannel
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2013 18:17:44
//
//  @return       bool :
//  @param        channel :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMBLUETOOTHCONFIG::SetRemoteChannel(int channel)
{
  this->channel = channel;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::GetPIN
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/06/2013 19:36:39
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOSTREAMBLUETOOTHCONFIG::GetPIN()
{
  return &pin;
}




/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::GetServerPropertys
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/05/2013 16:19:22
//
//  @return       void* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
void* DIOSTREAMBLUETOOTHCONFIG::GetServerPropertys(int index)
{
  if(index <  0) return NULL;
  if(index >= 3) return NULL;

  return serverproperties[index];
}




//-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHCONFIG::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:15:22
//
//  @return
//  */
//-------------------------------------------------------------------
void DIOSTREAMBLUETOOTHCONFIG::Clean()
{
  type                = DIOSTREAMTYPE_BLUETOOTH;
  deviceselect        = 0;

  channel             = 0;

  serverproperties[0] = NULL;
  serverproperties[1] = NULL;
  serverproperties[2] = NULL;
}


