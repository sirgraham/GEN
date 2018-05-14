//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIP.CPP
//
//  WINDOWS Data IO Stream IP Local Enum Devices class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>
#include <iphlpapi.h>

#include "DIOStreamDeviceIP.h"

#include "DIOWINDOWSStreamIPLocalEnumDevices.h"

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

//  @param        publisher :
*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMIPLOCALENUMDEVICES::DIOWINDOWSSTREAMIPLOCALENUMDEVICES() : DIOSTREAMIPLOCALENUMDEVICES()
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
DIOWINDOWSSTREAMIPLOCALENUMDEVICES::~DIOWINDOWSSTREAMIPLOCALENUMDEVICES()
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
bool DIOWINDOWSSTREAMIPLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  IP_ADAPTER_INFO adapterinfo[16];
  XWORD           ninterfaces = 0;

  DWORD           size        = sizeof(adapterinfo);
  ULONG           status      = GetAdaptersInfo(adapterinfo,&size);
  int             index       = 0;

  if(status != ERROR_SUCCESS) return false;


  PIP_ADAPTER_INFO padapterinfo = adapterinfo;
  while(adapterinfo)
    {
      ninterfaces++;
      padapterinfo = padapterinfo->Next;
      if(!padapterinfo) break;
    }

  padapterinfo = adapterinfo;

  do {  // Eliminate the virtual Wifi Adapter
        if(strcmp(padapterinfo->Description,"Microsoft Virtual WiFi Miniport Adapter"))
          {
            DIOSTREAMDEVICEIP* device = new DIOSTREAMDEVICEIP();
            if(device)
              {
                //device->SetIndex(padapterinfo->Index);
                device->SetIndex(index++);

                device->SetType(DIOSTREAMDEVICE_TYPE_IP);

                switch(padapterinfo->Type)
                  {
                    case MIB_IF_TYPE_LOOPBACK : device->SetIPType(DIOSTREAMIPDEVICE_TYPE_LOOPBACK);   break;
                    case IF_TYPE_IEEE80211    : device->SetIPType(DIOSTREAMIPDEVICE_TYPE_WIFI);       break;
                    case MIB_IF_TYPE_ETHERNET : device->SetIPType(DIOSTREAMIPDEVICE_TYPE_ETHERNET);   break;
                                  default     : device->SetIPType(DIOSTREAMIPDEVICE_TYPE_UNKNOWN);    break;
                  }

                device->GetName()->Set(padapterinfo->AdapterName);
                device->GetDescription()->Set(padapterinfo->Description);
                device->GetMAC()->Set((XBYTE*)padapterinfo->Address);

                XSTRING IP;
                XSTRING gateway;

                gateway.Set(padapterinfo->GatewayList.IpAddress.String);

                IP.Set(padapterinfo->IpAddressList.IpAddress.String);
                device->GetIP()->Set(IP);

                if(gateway.Compare(__L("0.0.0.0")) || (ninterfaces == 1))
                  {
                    devices.Add(device);

                  } else delete device;
              }
          }

       padapterinfo = padapterinfo->Next;

     } while(padapterinfo);

  return true;
}



