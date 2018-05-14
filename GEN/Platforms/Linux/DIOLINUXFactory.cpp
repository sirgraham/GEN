

//------------------------------------------------------------------------------------------
//  DIOLINUXFACTORY.CPP
//
//  LINUX DIO factory class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 08/08/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------


#include "DIOLINUXURL.h"

#ifdef DIOUART_ACTIVE
#include "DIOStreamUARTConfig.h"
#include "DIOLINUXStreamUARTLocalEnumDevices.h"
#include "DIOLINUXStreamUART.h"
#endif

#ifdef DIOUSB_ACTIVE
#include "DIOStreamUSBConfig.h"
#include "DIOLINUXStreamUSBLocalEnumDevices.h"
#include "DIOLINUXStreamUSB.h"
#endif

#if defined(DIOICMP_ACTIVE) || defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE)
#include "DIOLINUXStreamIPLocalEnumDevices.h"
#endif

#ifdef DIOICMP_ACTIVE
#include "DIOStreamICMPConfig.h"
#include "DIOLINUXStreamICMP.h"
#endif

#ifdef DIOUDP_ACTIVE
#include "DIOStreamUDPConfig.h"
#include "DIOLINUXStreamUDP.h"
#endif

#ifdef DIOTCPIP_ACTIVE
#include "DIOStreamTCPIPConfig.h"
#include "DIOLINUXStreamTCPIP.h"
#endif

#ifdef DIOBLUETOOTH_ACTIVE
#include "DIOStreamBluetoothConfig.h"
#include "DIOLINUXStreamBluetoothLocalEnumDevices.h"
#include "DIOLINUXStreamBluetoothRemoteEnumDevices.h"
#include "DIOLINUXStreamBluetooth.h"
#endif

#ifdef DIOWIFI_ACTIVE
#include "DIOLINUXStreamWifiRemoteEnumDevices.h"
#endif

#ifdef DIOSPI_ACTIVE
#include "DIOStreamSPIConfig.h"
#include "DIOLINUXStreamSPI.h"
#endif

#ifdef DIOI2C_ACTIVE
#include "DIOStreamI2CConfig.h"
#include "DIOLINUXStreamI2C.h"
#endif

#ifdef DIOPING_ACTIVE
#include "DIOLINUXPing.h"
#endif

#ifdef DIOPCAP_ACTIVE
#include "DIOLINUXPCap.h"
#endif

#ifdef DIOGPIO_ACTIVE

#ifdef HW_PC
#include "DIOLINUXGPIOPC.h"
#endif

#ifdef HW_GUMSTIXCONNEX
#include "DIOLINUXGPIOGumstixConnex.h"
#endif

#ifdef HW_LEX3V700A
#include "DIOLINUXGPIOLex3V700A.h"
#endif

#ifdef HW_ARTILA500
#include "DIOLINUXGPIOArtila500.h"
#endif

#ifdef HW_RASPBERRYPI
#include "DIOLINUXGPIORaspberryPi.h"
#endif

#endif


#include "DIOLINUXFactory.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::CreateURL
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
DIOURL* DIOLINUXFACTORY::CreateURL()
{
  DIOLINUXURL* _class = new DIOLINUXURL();

  return (DIOURL*)_class;
};



/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::DeleteURL
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
bool DIOLINUXFACTORY::DeleteURL(DIOURL* url)
{
  if(!url) return false;

  DIOLINUXURL* _url = (DIOLINUXURL*)url;
  delete _url;

  return true;
};





#ifdef ANYTYPEOFDIOSTREAMIO

/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::CreateStreamEnumDevices
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
DIOSTREAMENUMDEVICES* DIOLINUXFACTORY::CreateStreamEnumDevices(DIOSTREAMENUMTYPE type)
{
  DIOSTREAMENUMDEVICES* _class = NULL;

  switch(type)
    {
      #ifdef DIOUART_ACTIVE
      case DIOSTREAMENUMTYPE_UART_LOCAL         : _class = new DIOLINUXSTREAMUARTLOCALENUMDEVICES();                      break;
      #endif

      #ifdef DIOUSB_ACTIVE
      case DIOSTREAMENUMTYPE_USB_LOCAL          : _class = new DIOLINUXSTREAMUSBLOCALENUMDEVICES();                       break;
      #endif

      #if defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE)
      case DIOSTREAMENUMTYPE_IP_LOCAL           :  _class = new DIOLINUXSTREAMIPLOCALENUMDEVICES();                       break;
      #endif

      #ifdef DIOBLUETOOTH_ACTIVE
      case DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL    :  _class = new DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES();                break;
      case DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE   :  _class = new DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES();               break;
      #endif

      #ifdef DIOWIFI_ACTIVE
      case DIOSTREAMENUMTYPE_WIFI_REMOTE        :  _class = new DIOLINUXSTREAMWIFIREMOTEENUMDEVICES();                    break;
      #endif
    }

  if(_class)_class->SetType(type);

  return _class;
};



/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::DeleteStreamEnumDevices
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
bool DIOLINUXFACTORY::DeleteStreamEnumDevices(DIOSTREAMENUMDEVICES* enumdevices)
{
  if(!enumdevices)  return false;

  delete enumdevices;

  return true;
}




/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::CreateStreamIO
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
DIOSTREAM* DIOLINUXFACTORY::CreateStreamIO(DIOSTREAMCONFIG* config)
{
  if(!config) return NULL;

  DIOSTREAM* _class=NULL;

  switch(config->GetType())
    {
      case DIOSTREAMTYPE_UNKNOWN    : return NULL;

      #ifdef DIOUART_ACTIVE
      case DIOSTREAMTYPE_UART       : _class = new DIOLINUXSTREAMUART();        break;
      #endif

      #ifdef DIOUSB_ACTIVE
      case DIOSTREAMTYPE_USB        : _class = new DIOLINUXSTREAMUSB();         break;
      #endif

      #ifdef DIOICMP_ACTIVE
      case DIOSTREAMTYPE_ICMP       : _class = new DIOLINUXSTREAMICMP();        break;
      #endif

      #ifdef DIOUDP_ACTIVE
      case DIOSTREAMTYPE_UDP        : _class = new DIOLINUXSTREAMUDP();         break;
      #endif

      #ifdef DIOTCPIP_ACTIVE
      case DIOSTREAMTYPE_TCPIP      : _class = new DIOLINUXSTREAMTCPIP();       break;
      #endif

      #ifdef DIOBLUETOOTH_ACTIVE
      case DIOSTREAMTYPE_BLUETOOTH  : _class = new DIOLINUXSTREAMBLUETOOTH();   break;
      #endif

      #ifdef DIOSPI_ACTIVE
      case DIOSTREAMTYPE_SPI        : _class = new DIOLINUXSTREAMSPI();         break;
      #endif

      #ifdef DIOI2C_ACTIVE
      case DIOSTREAMTYPE_I2C        : _class = new DIOLINUXSTREAMI2C();         break;
      #endif
    }

  if(_class)
    {
      _class->SetType   (config->GetType());
      _class->SetConfig (config);
    }

  return  _class;
}


//-------------------------------------------------------------------
//  DIOLINUXFACTORY::DeleteStreamIO
/*
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        streamio :
//*/
//-------------------------------------------------------------------
bool DIOLINUXFACTORY::DeleteStreamIO(DIOSTREAM* diostream)
{
  if(!diostream) return false;

  delete(diostream);

  return true;
}


#endif




#ifdef DIOPING_ACTIVE
/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::CreatePing
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
DIOPING* DIOLINUXFACTORY::CreatePing()
{
  DIOLINUXPING* _class = new DIOLINUXPING();

  return (DIOPING*)_class;
}


/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::DeletePing
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
bool DIOLINUXFACTORY::DeletePing(DIOPING* ping)
{
  if(!ping) return false;

  DIOLINUXPING* _ping = (DIOLINUXPING*)ping;
  delete _ping;

  return true;

}
#endif



#ifdef DIOPCAP_ACTIVE
/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::CreatePCap
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
DIOPCAP* DIOLINUXFACTORY::CreatePCap()
{
  DIOLINUXPCAP* _class = new DIOLINUXPCAP();

  return (DIOPCAP*)_class;
}



/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::DeleteGPIO
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
bool DIOLINUXFACTORY::DeletePCap(DIOPCAP* pcap)
{
  if(!pcap) return false;

  DIOLINUXPCAP* _pcap = (DIOLINUXPCAP*)pcap;
  delete _pcap;

  return true;
}

#endif




#ifdef DIOGPIO_ACTIVE
/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::CreateGPIO
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
DIOGPIO* DIOLINUXFACTORY::CreateGPIO()
{
  DIOGPIO* _class;

  #ifdef HW_PC
  _class = (DIOGPIO*)new DIOLINUXGPIOPC();
  #endif

  #ifdef HW_GUMSTIXCONNEX
  _class = (DIOGPIO*)new DIOLINUXGPIOGUMSTIXCONNEX();
  #endif

  #ifdef HW_LEX3V700A
  _class = (DIOGPIO*)new DIOLINUXGPIOLEX3V700A();
  #endif

  #ifdef HW_ARTILA500
  _class = (DIOGPIO*)new DIOLINUXGPIOARTILA500();
  #endif

  #ifdef HW_RASPBERRYPI
  _class = (DIOGPIO*)new DIOLINUXGPIORASPBERRYPI();
  #endif

  return (DIOGPIO*)_class;
}



/*-------------------------------------------------------------------
//  DIOLINUXFACTORY::DeleteGPIO
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
bool DIOLINUXFACTORY::DeleteGPIO(DIOGPIO* port)
{
  if(!port) return false;

  #ifdef HW_PC
  DIOLINUXGPIOPC* _port = (DIOLINUXGPIOPC*)port;
  #endif

  #ifdef HW_GUMSTIXCONNEX
  DIOLINUXGPIOGUMSTIXCONNEX* _port = (DIOLINUXGPIOGUMSTIXCONNEX*)port;
  #endif

  #ifdef HW_LEX3V700A
  DIOLINUXGPIOLEX3V700A* _port = (DIOLINUXGPIOLEX3V700A*)port;
  #endif

  #ifdef HW_ARTILA500
  DIOLINUXGPIOARTILA500* _port = (DIOLINUXGPIOARTILA500*)port;
  #endif

  #ifdef HW_RASPBERRYPI
  DIOLINUXGPIORASPBERRYPI* _port = (DIOLINUXGPIORASPBERRYPI*)port;
  #endif

  delete _port;

  return true;
}

#endif





