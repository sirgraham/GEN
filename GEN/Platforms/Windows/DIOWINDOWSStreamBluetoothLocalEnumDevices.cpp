//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIP.CPP
//
//  WINDOWS Data IO Stream Bluetooth Local Enum Devices class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)


//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>
#include <iphlpapi.h>
#include <bluetoothapis.h>

#include "DIOStreamDeviceBluetooth.h"
#include "DIOWINDOWSStreamBluetoothLocalEnumDevices.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPLOCALENUMDEVICES::DIOWINDOWSSTREAMTCPIPLOCALENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:01
//
//  @return


*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES::DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES() : DIOSTREAMBLUETOOTHLOCALENUMDEVICES()
{

}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPLOCALENUMDEVICES::~DIOWINDOWSSTREAMTCPIPLOCALENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:12
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES::~DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES()
{

}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPLOCALENUMDEVICES::Search
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:27
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  BLUETOOTH_FIND_RADIO_PARAMS radioparams;
  HBLUETOOTH_RADIO_FIND       hfind;
  HANDLE                      hradio   = NULL;

  memset(&radioparams, 0, sizeof(radioparams));
  radioparams.dwSize = sizeof(radioparams);

  hfind = BluetoothFindFirstRadio((BLUETOOTH_FIND_RADIO_PARAMS*)&radioparams,(HANDLE*)&hradio);
  if(hfind)
    {
      BLUETOOTH_RADIO_INFO  radioinfo;
      BOOL                  foundnext;
      int                   index = 0;

      do{ memset(&radioinfo, 0, sizeof(radioinfo));
          radioinfo.dwSize =  sizeof(radioinfo);

          int wt     = 0;
          int status;

          do{ status = BluetoothGetRadioInfo(hradio, &radioinfo);
              if(status == ERROR_SUCCESS) break;

              wt++;
              if(wt>8) break;

            } while(1);

          if(status == ERROR_SUCCESS)
            {
              DIOSTREAMDEVICEBLUETOOTH* device = new DIOSTREAMDEVICEBLUETOOTH();
              if(device)
                {
                  device->SetIndex(index);

                  device->GetName()->Set((XCHAR*)radioinfo.szName);

                  XSTRING MAC;
                  MAC.Format(__L("%02X:%02X:%02X:%02X:%02X:%02X") , (XBYTE)radioinfo.address.rgBytes[5]
                                                                  , (XBYTE)radioinfo.address.rgBytes[4]
                                                                  , (XBYTE)radioinfo.address.rgBytes[3]
                                                                  , (XBYTE)radioinfo.address.rgBytes[2]
                                                                  , (XBYTE)radioinfo.address.rgBytes[1]
                                                                  , (XBYTE)radioinfo.address.rgBytes[0]);
                  device->GetMAC()->Set(MAC);

                  device->SetIsVisible(BluetoothIsDiscoverable(hradio)?true:false);

                  devices.Add(device);
                }
            }

          foundnext = BluetoothFindNextRadio(hfind,(HANDLE*)&hradio);

          if(foundnext) index++;


        } while(foundnext);

      BluetoothFindRadioClose(hfind);
    }

  return true;
}


#endif
