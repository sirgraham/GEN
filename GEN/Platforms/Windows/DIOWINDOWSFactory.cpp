//------------------------------------------------------------------------------------------
//  DIOWINDOWSFACTORY.CPP
//
//  WINDOWS Utils Platform Factory class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 08/08/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifdef DIO_ACTIVE

//---- INCLUDES ----------------------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>

#include "DIOWINDOWSURL.h"

#ifdef DIOUART_ACTIVE
#include "DIOStreamUARTConfig.h"
#include "DIOWINDOWSStreamUARTLocalEnumDevices.h"
#include "DIOWINDOWSStreamUART.h"
#endif

#ifdef DIOUSB_ACTIVE
#include "DIOStreamUSBConfig.h"
#include "DIOWINDOWSStreamUSBLocalEnumDevices.h"
#include "DIOWINDOWSStreamUSB.h"
#endif

#if defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE)
#include "DIOWINDOWSStreamIPLocalEnumDevices.h"
#endif

#ifdef DIOICMP_ACTIVE
#include "DIOStreamICMPConfig.h"
#include "DIOWINDOWSStreamICMP.h"
#endif

#ifdef DIOUDP_ACTIVE
#include "DIOStreamUDPConfig.h"
#include "DIOWINDOWSStreamUDP.h"
#endif

#ifdef DIOTCPIP_ACTIVE
#include "DIOStreamTCPIPConfig.h"
#include "DIOWINDOWSStreamTCPIP.h"
#endif

#ifdef DIOBLUETOOTH_ACTIVE
#include "DIOStreamBluetoothConfig.h"
#include "DIOWINDOWSStreamBluetoothLocalEnumDevices.h"
#include "DIOWINDOWSStreamBluetoothRemoteEnumDevices.h"
#include "DIOWINDOWSStreamBluetooth.h"
#endif

#ifdef DIOWIFI_ACTIVE
#include "DIOWINDOWSStreamWifiRemoteEnumDevices.h"
#endif

#ifdef DIOSPI_ACTIVE
#include "DIOStreamSPIConfig.h"
#include "DIOWINDOWSStreamSPI.h"
#endif

#ifdef DIOI2C_ACTIVE
#include "DIOStreamI2CConfig.h"
#include "DIOWINDOWSStreamI2C.h"
#endif

#ifdef DIOPING_ACTIVE
#include "DIOWINDOWSPing.h"
#endif

#ifdef DIOPCAP_ACTIVE
#include "DIOWINDOWSPCap.h"
#endif

#ifdef DIOGPIO_ACTIVE
#include "DIOWINDOWSGPIO.h"
#endif

#include "DIOWINDOWSFactory.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::CreateURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/02/2013 22:51:06
//
//  @return       DIOURL* :
//  */
/*-----------------------------------------------------------------*/
DIOURL* DIOWINDOWSFACTORY::CreateURL()
{
  DIOWINDOWSURL* _class = new DIOWINDOWSURL();

  return (DIOURL*)_class;
};



/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::DeleteURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/02/2013 22:51:15
//
//  @return       bool :
//  @param        url :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSFACTORY::DeleteURL(DIOURL* url)
{
  if(!url) return false;

  DIOWINDOWSURL* _url = (DIOWINDOWSURL*)url;
  delete _url;

  return true;
};





#ifdef ANYTYPEOFDIOSTREAMIO

/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::CreateStreamEnumDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2013 1:20:06
//
//  @return       DIOSTREAMENUMDEVICES* :


//  @param        type :
*/
/*-----------------------------------------------------------------*/
DIOSTREAMENUMDEVICES* DIOWINDOWSFACTORY::CreateStreamEnumDevices(DIOSTREAMENUMTYPE type)
{
  DIOSTREAMENUMDEVICES* _class = NULL;

  switch(type)
    {
      case DIOSTREAMENUMTYPE_UNKNOWN            : return NULL;

      #ifdef DIOUART_ACTIVE
      case DIOSTREAMENUMTYPE_UART_LOCAL         : _class = new DIOWINDOWSSTREAMUARTLOCALENUMDEVICES();                break;
      #endif

      #ifdef DIOUSB_ACTIVE
      case DIOSTREAMENUMTYPE_USB_LOCAL          : _class = new DIOWINDOWSSTREAMUSBLOCALENUMDEVICES();                 break;
      #endif

      #if defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE)
      case DIOSTREAMENUMTYPE_IP_LOCAL           : _class = new DIOWINDOWSSTREAMIPLOCALENUMDEVICES();                  break;
      #endif

      #ifdef DIOBLUETOOTH_ACTIVE
      case DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL    : _class = new DIOWINDOWSSTREAMBLUETOOTHLOCALENUMDEVICES();           break;
      case DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE   : _class = new DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES();          break;
      #endif

      #ifdef DIOWIFI_ACTIVE
      case DIOSTREAMENUMTYPE_WIFI_REMOTE        : _class = new DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES();               break;
      #endif
    }

  if(_class)_class->SetType(type);

  return _class;
};



/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::DeleteStreamEnumDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2013 1:20:13
//
//  @return       bool :
//  @param        enumdevices :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSFACTORY::DeleteStreamEnumDevices(DIOSTREAMENUMDEVICES* enumdevices)
{
  if(!enumdevices)  return false;

  delete enumdevices;

  return true;
};




/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::CreateStreamIO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/06/2011 16:52:26
//
//  @return       DIOSTREAM* :


//  @param        config :
*/
/*-----------------------------------------------------------------*/
DIOSTREAM* DIOWINDOWSFACTORY::CreateStreamIO(DIOSTREAMCONFIG* config)
{
  if(!config) return NULL;

  DIOSTREAM* _class = NULL;

  switch(config->GetType())
    {
      case DIOSTREAMTYPE_UNKNOWN    : return NULL;

      #ifdef DIOUART_ACTIVE
      case DIOSTREAMTYPE_UART       : _class = new DIOWINDOWSSTREAMUART();        break;
      #endif

      #ifdef DIOUSB_ACTIVE
      case DIOSTREAMTYPE_USB        : _class = new DIOWINDOWSSTREAMUSB();         break;
      #endif

      #ifdef DIOICMP_ACTIVE
      case DIOSTREAMTYPE_ICMP       : _class = new DIOWINDOWSSTREAMICMP();        break;
      #endif

      #ifdef DIOUDP_ACTIVE
      case DIOSTREAMTYPE_UDP        : _class = new DIOWINDOWSSTREAMUDP();         break;
      #endif

      #ifdef DIOTCPIP_ACTIVE
      case DIOSTREAMTYPE_TCPIP      : _class = new DIOWINDOWSSTREAMTCPIP();       break;
      #endif

      #ifdef DIOBLUETOOTH_ACTIVE
      case DIOSTREAMTYPE_BLUETOOTH  : _class = new DIOWINDOWSSTREAMBLUETOOTH();   break;
      #endif

      #ifdef DIOSPI_ACTIVE
      case DIOSTREAMTYPE_SPI        : _class = new DIOWINDOWSSTREAMSPI();         break;
      #endif

      #ifdef DIOI2C_ACTIVE
      case DIOSTREAMTYPE_I2C        : _class = new DIOWINDOWSSTREAMI2C();         break;
      #endif
    }

  if(_class)
    {
      _class->SetType(config->GetType());
      _class->SetConfig(config);
    }

  return _class;
}


//-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::DeleteStreamIO
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        diostream :
*/
//-------------------------------------------------------------------
bool DIOWINDOWSFACTORY::DeleteStreamIO(DIOSTREAM* diostream)
{
  if(!diostream)  return false;

  delete diostream;

  return true;
}

#endif




#ifdef DIOPING_ACTIVE
/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::CreatePing
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/03/2016 12:50:04
//
//  @return       DIOPING* :
//
*/
/*-----------------------------------------------------------------*/
DIOPING* DIOWINDOWSFACTORY::CreatePing()
{
  DIOWINDOWSPING* _class = new DIOWINDOWSPING();

  return (DIOPING*)_class;
}


/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::DeletePing
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/03/2016 12:50:23
//
//  @return       bool :
//
//  @param        ping :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSFACTORY::DeletePing(DIOPING* ping)
{
  if(!ping) return false;

  DIOWINDOWSPING* _ping = (DIOWINDOWSPING*)ping;
  delete _ping;

  return true;

}
#endif



#ifdef DIOPCAP_ACTIVE
/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::CreatePCap
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/10/2012 13:11:14
//
//  @return       DIOPCAP* :
//  @param        xfactory :
*/
/*-----------------------------------------------------------------*/
DIOPCAP* DIOWINDOWSFACTORY::CreatePCap()
{
  DIOWINDOWSPCAP* _class = new DIOWINDOWSPCAP();

  return (DIOPCAP*)_class;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::DeleteGPIO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2009 07:27:55 p.m.
//
//  @return       bool :
//  @param        port :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSFACTORY::DeletePCap(DIOPCAP* pcap)
{
  if(!pcap) return false;

  DIOWINDOWSPCAP* _pcap = (DIOWINDOWSPCAP*)pcap;
  delete _pcap;

  return true;
}
#endif




#ifdef DIOGPIO_ACTIVE
/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::CreateGPIO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2009 07:27:51 p.m.
//
//  @return       DIOGPIO* :

*/
/*-----------------------------------------------------------------*/
DIOGPIO* DIOWINDOWSFACTORY::CreateGPIO()
{
  DIOWINDOWSGPIO* _class = new DIOWINDOWSGPIO();

  return (DIOGPIO*)_class;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSFACTORY::DeleteGPIO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2009 07:27:55 p.m.
//
//  @return       bool :
//  @param        port :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSFACTORY::DeleteGPIO(DIOGPIO* port)
{
  if(!port) return false;

  DIOWINDOWSGPIO* _port = (DIOWINDOWSGPIO*)port;
  delete _port;

  return true;
}

#endif

#endif
