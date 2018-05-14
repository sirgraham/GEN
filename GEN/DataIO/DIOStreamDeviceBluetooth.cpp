//------------------------------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH.CPP
//
//  Data IO Stream Device Bluetooth class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XString.h"
#include "XConsole.h"
#include "DIOStreamDeviceBluetooth.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH:: DIOSTREAMDEVICEBLUETOOTH
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:31
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOSTREAMDEVICEBLUETOOTH:: DIOSTREAMDEVICEBLUETOOTH()
{
  Clean();
}


//-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::~ DIOSTREAMDEVICEBLUETOOTH
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:20
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOSTREAMDEVICEBLUETOOTH::~ DIOSTREAMDEVICEBLUETOOTH()
{
  Service_DeleteAll();

  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::GetMAC
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/05/2013 0:32:24
//
//  @return       DIOMAC* :
//  */
/*-----------------------------------------------------------------*/
DIOMAC* DIOSTREAMDEVICEBLUETOOTH::GetMAC()
{
  return &MAC;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::GetServiceClass
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/05/2013 19:16:15
//
//  @return       XDWORD :
//  */
/*-----------------------------------------------------------------*/
DIOGUID* DIOSTREAMDEVICEBLUETOOTH::GetServiceClass()
{
  return &serviceclass;
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::IsVisible
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 0:08:24
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICEBLUETOOTH::IsVisible()
{
  return isvisible;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::SetIsVisible
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 0:08:54
//
//  @return       bool :
//  @param        isvisible :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICEBLUETOOTH::SetIsVisible(bool isvisible)
{
  this->isvisible = isvisible;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::Service_Add
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2013 22:11:36
//
//  @return       bool :
//  @param        service :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICEBLUETOOTH::Service_Add(DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service)
{
  if(!service) return false;

  services.Add(service);

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::Service_GetAll
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2013 22:11:49
//
//  @return       XVECTOR* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*>* DIOSTREAMDEVICEBLUETOOTH::Service_GetAll()
{
  return &services;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::Service_DeleteAll
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2013 22:11:59
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICEBLUETOOTH::Service_DeleteAll()
{
  if(services.IsEmpty()) return false;

  services.DeleteContents();

  services.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::GetChannel
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/06/2013 20:45:42
//
//  @return       int :
//  @param        servicetype :
*/
/*-----------------------------------------------------------------*/
int DIOSTREAMDEVICEBLUETOOTH::GetChannel(XWORD servicetype)
{
  for(int c=0; c<(int)services.GetSize(); c++)
    {
       DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service =  (DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*)services.Get(c);
       if(service)
         {
           if(servicetype == service->GetID())
             {
               for(int d=0; d<(int)service->GetNProtocols(); d++)
                 {
                   DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL* protocol = (DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL*)service->GetProtocol(d);
                   if(protocol)
                     {
                       int channel = protocol->GetChannel();
                       if(channel) return channel;
                     }
                 }
            }
         }
    }

  return DIOSTREAMDEVICEBLUETOOTH_NOCHANNEL;
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::GetChannelSerialPort
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/06/2013 0:27:29
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOSTREAMDEVICEBLUETOOTH::GetChannelSerialPort()
{
  return GetChannel(DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_SERIALPORT);
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::GetChannelObexPush
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/06/2013 0:27:34
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOSTREAMDEVICEBLUETOOTH::GetChannelObexPush()
{
  return GetChannel(DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_OBEXOBJECTPUSH);
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::GetChannelDialUpNetworking
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/06/2013 0:27:41
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOSTREAMDEVICEBLUETOOTH::GetChannelDialUpNetworking()
{
  return GetChannel(DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_DIALUPNETWORKING);
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::GetChannelHID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/02/2015 21:19:29
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
int DIOSTREAMDEVICEBLUETOOTH::GetChannelHID()
{
  return GetChannel(DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HUMANINTERFACEDEVICESERVICE);
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::DebugPrintInfo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/05/2013 11:28:09
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICEBLUETOOTH::DebugPrintInfo(XCONSOLE* xconsole)
{
  DIOSTREAMDEVICE::DebugPrintInfo(xconsole);

  XSTRING line;
  XSTRING data;

  GetMAC()->GetXString(data);
  line.Format(__L("MAC           : %s") , data.Get());                          DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());

  line.Format(__L("Service Class : %08X"), GetServiceClass()->GetData1());      DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());

  XSTRING ID;
  XSTRING channel;
  int     nservices = services.GetSize();

  for(int c=0;c<nservices;c++)
    {
      DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service = (DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*)services.Get(c);
      if(!service) continue;

      ID.Format(__L("%08X"),service->GetID());

      line  = ID;
      line += __L(" - ");

      line += __L(" \"");
      line += service->GetName()->Get();
      line += __L("\" ");

      line += __L(" \"");
      line += service->GetDescription()->Get();
      line += __L("\" ");

      line += __L(" \"");
      line += service->GetProvider()->Get();
      line += __L("\" ");

      for(int d=0;d<service->GetNProtocols();d++)
        {
          DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL* protocol = service->GetProtocol(d);
          if(protocol)
            {
              ID.Format(__L("%08X"),protocol->GetType());

              line += ID;
              line += " ";

              channel.Format(__L("[%d]"),protocol->GetChannel());

              line += channel;
              line += " ";
            }
        }

      DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());
    }

  return true;
}



//-------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:12
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOSTREAMDEVICEBLUETOOTH::Clean()
{
  type      = DIOSTREAMDEVICE_TYPE_BLUETOOTH;

  isvisible = false;
}



