//------------------------------------------------------------------------------------------
//  DIOANDROIDFACTORY.CPP
//
//  ANDROID DIO factory class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 08/08/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifdef DIO_ACTIVE

//---- INCLUDES ----------------------------------------------------------------------------


#include "DIOANDROIDURL.h"

#ifdef DIOUART_ACTIVE
#include "DIOStreamUARTConfig.h"
#include "DIOANDROIDStreamUARTLocalEnumDevices.h"
#include "DIOANDROIDStreamUART.h"
#endif

#ifdef DIOUSB_ACTIVE
#include "DIOStreamUSBConfig.h"
#include "DIOANDROIDStreamUSBLocalEnumDevices.h"
#include "DIOANDROIDStreamUSB.h"
#endif

#if defined(DIOICMP_ACTIVE) || defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE)
#include "DIOANDROIDStreamIPLocalEnumDevices.h"
#endif

#ifdef DIOICMP_ACTIVE
#include "DIOStreamICMPConfig.h"
#include "DIOANDROIDStreamICMP.h"
#endif

#ifdef DIOUDP_ACTIVE
#include "DIOStreamUDPConfig.h"
#include "DIOANDROIDStreamUDP.h"
#endif

#ifdef DIOTCPIP_ACTIVE
#include "DIOStreamTCPIPConfig.h"
#include "DIOANDROIDStreamTCPIP.h"
#endif

#ifdef DIOBLUETOOTH_ACTIVE
#include "DIOStreamBluetoothConfig.h"
#include "DIOANDROIDStreamBluetoothLocalEnumDevices.h"
#include "DIOANDROIDStreamBluetoothRemoteEnumDevices.h"
#include "DIOANDROIDStreamBluetooth.h"
#endif

#ifdef DIOWIFI_ACTIVE
#include "DIOANDROIDStreamWifiRemoteEnumDevices.h"
#endif

#ifdef DIOSPI_ACTIVE
#include "DIOStreamSPIConfig.h"
#include "DIOANDROIDStreamSPI.h"
#endif

#ifdef DIOI2C_ACTIVE
#include "DIOStreamI2CConfig.h"
#include "DIOANDROIDStreamI2C.h"
#endif

#ifdef DIOPING_ACTIVE
#include "DIOANDROIDPing.h"
#endif
#ifdef DIOPCAP_ACTIVE
#include "DIOANDROIDPCap.h"
#endif
#ifdef DIOGPIO_ACTIVE
#include "DIOANDROIDGPIO.h"
#endif

#include "DIOANDROIDFactory.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::CreateURL
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
DIOURL* DIOANDROIDFACTORY::CreateURL()
{
  DIOANDROIDURL* _class = new DIOANDROIDURL();

  return (DIOURL*)_class;
};



/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::DeleteURL
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
bool DIOANDROIDFACTORY::DeleteURL(DIOURL* url)
{
  if(!url) return false;

  DIOANDROIDURL* _url = (DIOANDROIDURL*)url;
  delete _url;

  return true;
};





#ifdef ANYTYPEOFDIOSTREAMIO

/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::CreateStreamEnumDevices
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
DIOSTREAMENUMDEVICES* DIOANDROIDFACTORY::CreateStreamEnumDevices(DIOSTREAMENUMTYPE type)
{
  DIOSTREAMENUMDEVICES* _class = NULL;

  switch(type)
    {
      #ifdef DIOUART_ACTIVE
      case DIOSTREAMENUMTYPE_UART_LOCAL         : _class = new DIOANDROIDSTREAMUARTLOCALENUMDEVICES();                break;
      #endif

      #ifdef DIOUSB_ACTIVE
      case DIOSTREAMENUMTYPE_USB_LOCAL          : _class = new DIOANDROIDSTREAMUSBLOCALENUMDEVICES();                 break;
      #endif

      #if defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE)
      case DIOSTREAMENUMTYPE_IP_LOCAL           : _class = new DIOANDROIDSTREAMIPLOCALENUMDEVICES();                  break;
      #endif

      #ifdef DIOBLUETOOTH_ACTIVE
      case DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL    : _class = new DIOANDROIDSTREAMBLUETOOTHLOCALENUMDEVICES();           break;
      case DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE   : _class = new DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES();          break;
      #endif

      #ifdef DIOWIFI_ACTIVE
      case DIOSTREAMENUMTYPE_WIFI_REMOTE        : _class = new DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES();               break;
      #endif
    }

  if(_class)_class->SetType(type);

  return _class;
};



/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::DeleteStreamEnumDevices
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
bool DIOANDROIDFACTORY::DeleteStreamEnumDevices(DIOSTREAMENUMDEVICES* enumdevices)
{
  if(!enumdevices)  return false;

  delete enumdevices;

  return true;
};




/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::CreateStreamIO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2013 23:19:55
//
//  @return       DIOSTREAM* :



//  @param        config :
*/
/*-----------------------------------------------------------------*/
DIOSTREAM* DIOANDROIDFACTORY::CreateStreamIO(DIOSTREAMCONFIG* config)
{
  if(!config) return NULL;

  DIOSTREAM*_class = NULL;

  switch(config->GetType())
    {
      case DIOSTREAMTYPE_UNKNOWN    : return NULL;

      #ifdef DIOUART_ACTIVE
      case DIOSTREAMTYPE_UART       : _class = new DIOANDROIDSTREAMUART();        break;
      #endif

      #ifdef DIOUSB_ACTIVE
      case DIOSTREAMTYPE_USB        : _class = new DIOANDROIDSTREAMUSB();         break;
      #endif

      #ifdef DIOICMP_ACTIVE
      case DIOSTREAMTYPE_ICMP       : _class = new DIOANDROIDSTREAMICMP();        break;
      #endif

      #ifdef DIOUDP_ACTIVE
      case DIOSTREAMTYPE_UDP        : _class = new DIOANDROIDSTREAMUDP();         break;
      #endif

      #ifdef DIOTCPIP_ACTIVE
      case DIOSTREAMTYPE_TCPIP      : _class = new DIOANDROIDSTREAMTCPIP();       break;
      #endif

      #ifdef DIOBLUETOOTH_ACTIVE
      case DIOSTREAMTYPE_BLUETOOTH  : _class = new DIOANDROIDSTREAMBLUETOOTH();   break;
      #endif

      #ifdef DIOSPI_ACTIVE
      case DIOSTREAMTYPE_SPI        : _class = new DIOANDROIDSTREAMSPI();         break;
      #endif

      #ifdef DIOI2C_ACTIVE
      case DIOSTREAMTYPE_I2C        : _class = new DIOANDROIDSTREAMI2C();         break;
      #endif
    }

  if(_class)
    {
      _class->SetType(config->GetType());
      _class->SetConfig(config);
    }

  return  _class;
}



//-------------------------------------------------------------------
//  DIOANDROIDFACTORY::DeleteStreamIO
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        streamio :
*/
//-------------------------------------------------------------------
bool DIOANDROIDFACTORY::DeleteStreamIO(DIOSTREAM* diostream)
{
  if(!diostream) return false;
  delete diostream;
  return true;
}

#endif




#ifdef DIOPING_ACTIVE
/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::CreatePing
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
DIOPING* DIOANDROIDFACTORY::CreatePing()
{
  DIOANDROIDPING* _class = new DIOANDROIDPING();

  return (DIOPING*)_class;
}


/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::DeletePing
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
bool DIOANDROIDFACTORY::DeletePing(DIOPING* ping)
{
  if(!ping) return false;

  DIOANDROIDPING* _ping = (DIOANDROIDPING*)ping;
  delete _ping;

  return true;

}
#endif



#ifdef DIOPCAP_ACTIVE
/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::CreatePCap
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2009 07:27:51 p.m.
//
//  @return       DIOPCAP* :

*/
/*-----------------------------------------------------------------*/
DIOPCAP* DIOANDROIDFACTORY::CreatePCap()
{
  DIOANDROIDPCAP* _class = new DIOANDROIDPCAP();

  return (DIOPCAP*)_class;
}



/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::DeleteGPIO
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
bool DIOANDROIDFACTORY::DeletePCap(DIOPCAP* pcap)
{
  if(!pcap) return false;

  DIOANDROIDPCAP* _pcap = (DIOANDROIDPCAP*)pcap;
  delete _pcap;

  return true;
}

#endif




#ifdef DIOGPIO_ACTIVE
/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::CreateGPIO
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
DIOGPIO* DIOANDROIDFACTORY::CreateGPIO()
{
  DIOANDROIDGPIO* _class = new DIOANDROIDGPIO();

  return (DIOGPIO*)_class;
}



/*-------------------------------------------------------------------
//  DIOANDROIDFACTORY::DeleteGPIO
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
bool DIOANDROIDFACTORY::DeleteGPIO(DIOGPIO* port)
{
  if(!port) return false;

  DIOANDROIDGPIO* _port = (DIOANDROIDGPIO*)port;
  delete _port;

  return true;
}

#endif



#endif