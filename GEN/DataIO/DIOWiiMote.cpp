
/*------------------------------------------------------------------------------------------
//  DIOWIIMOTE.CPP
//
//  DIO Wii Mote  (Wii controler)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 15/02/2015 16:52:55
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XSleep.h"
#include "XPublisher.h"

#include "DIOFactory.h"
#include "DIOStreamDeviceBluetooth.h"
#include "DIOStreamBluetoothRemoteEnumDevices.h"
#include "DIOStreamBluetoothConfig.h"
#include "DIOStreamBluetooth.h"

#include "DIOWiiMote.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  DIOWIIMOTE::DIOWIIMOTE
*/
/**
//
//  Class Constructor DIOWIIMOTE
//
//  @author       Abraham J. Velez
//  @version      15/02/2015 16:54:52
//


//  @param        publisher :
*/
/*-----------------------------------------------------------------*/
DIOWIIMOTE::DIOWIIMOTE()
{
  Clean();


  ;

  xtimerout = xfactory->CreateTimer();

  enumremotedevices =  (DIOSTREAMBLUETOOTHREMOTEENUMDEVICES*)diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE);
  if(enumremotedevices)
    {
      diostreamcfg = new DIOSTREAMBLUETOOTHCONFIG();
      if(diostreamcfg)
        {
          diostream = (DIOSTREAMBLUETOOTH*)diofactory->CreateStreamIO(diostreamcfg);
        }
    }
}




/*-------------------------------------------------------------------
//  DIOWIIMOTE::~DIOWIIMOTE
*/
/**
//
//   Class Destructor DIOWIIMOTE
//
//  @author       Abraham J. Velez
//  @version      15/02/2015 16:54:56
//
*/
/*-----------------------------------------------------------------*/
DIOWIIMOTE::~DIOWIIMOTE()
{
  if(diostream) diofactory->DeleteStreamIO(diostream);
  if(diostreamcfg) delete diostreamcfg;

  if(enumremotedevices) diofactory->DeleteStreamEnumDevices(enumremotedevices);

  if(xtimerout) xfactory->DeleteTimer(xtimerout);

  Clean();
}




/*-------------------------------------------------------------------
//  DIOWIIMOTE::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2015 17:38:53
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOWIIMOTE::Ini(int timeout)
{
  device = NULL;

  if(!enumremotedevices) return false;

  bool status = false;

  enumremotedevices->Search();

  xtimerout->Reset();

  while(enumremotedevices->IsSearching())
    {
      xsleep->MilliSeconds(10);
    }


  if(enumremotedevices->GetDevices()->GetSize())
    {
      DIOSTREAMDEVICEBLUETOOTH* deviceremote = (DIOSTREAMDEVICEBLUETOOTH*)enumremotedevices->GetDevices()->Get(0);
      if(deviceremote)
        {
          deviceremote->DebugPrintInfo(NULL);
        }
    }


  for(int c=0;c<(int)enumremotedevices->GetDevices()->GetSize();c++)
    {
      device = (DIOSTREAMDEVICEBLUETOOTH*)enumremotedevices->GetDevices()->Get(c);
      if(device)
        {
          if(device->GetName()->Find(__L("Nintendo"), true) != XSTRING_NOTFOUND)
            {
              if(device->GetChannelHID() != DIOSTREAMDEVICEBLUETOOTH_NOCHANNEL)
                {
                  XSTRING pin;

                  pin.AdjustSize(6);

                  diostreamcfg->GetRemoteMAC()->Set(device->GetMAC()->Get());
                  diostreamcfg->SetRemoteChannel(device->GetChannelHID());

                  pin.Get()[0] = device->GetMAC()->Get()[5];
                  pin.Get()[1] = device->GetMAC()->Get()[4];
                  pin.Get()[2] = device->GetMAC()->Get()[3];
                  pin.Get()[3] = device->GetMAC()->Get()[2];
                  pin.Get()[4] = device->GetMAC()->Get()[1];
                  pin.Get()[5] = device->GetMAC()->Get()[0];

                  diostreamcfg->GetPIN()->Set(pin);

                  status = diostream->Open();
                  if(status)  status = diostream->WaitToConnected(timeout);
                }

              return status;
            }
        }
    }



  return false;
}




/*-------------------------------------------------------------------
//  DIOWIIMOTE::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2015 17:38:59
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOWIIMOTE::End()
{
  diostream->Close();

  return true;
}
