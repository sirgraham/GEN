//------------------------------------------------------------------------------------------
//  DIOSTM32FXXXFACTORY.CPP
//
//  STM32Fxxx utils platform factory class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 08/08/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifdef DIO_ACTIVE

//---- INCLUDES ----------------------------------------------------------------------------


#ifdef DIOUART_ACTIVE
#include "DIOStreamUARTConfig.h"
#include "DIOSTM32FxxxStreamUART.h"
#endif

#ifdef DIOUSB_ACTIVE
#include "DIOStreamUSBConfig.h"
#include "DIOSTM32FxxxStreamUSB.h"
#endif

#ifdef DIOSPI_ACTIVE
#include "DIOStreamSPIConfig.h"
#include "DIOSTM32FxxxStreamSPI.h"
#endif

#ifdef DIOI2C_ACTIVE
#include "DIOStreamI2CConfig.h"
#include "DIOSTM32FxxxStreamI2C.h"
#endif

#ifdef DIOGPIO_ACTIVE
#include "DIOSTM32FxxxGPIO.h"
#endif

#include "DIOSTM32FxxxFactory.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



#ifdef ANYTYPEOFDIOSTREAMIO

/*-------------------------------------------------------------------
//  DIOSTM32FXXXFACTORY::CreateStreamIO
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
DIOSTREAM* DIOSTM32FXXXFACTORY::CreateStreamIO(DIOSTREAMCONFIG* config)
{
  if(!config) return NULL;

  DIOSTREAM* _class = NULL;

  switch(config->GetType())
    {
      case DIOSTREAMTYPE_UNKNOWN    : return NULL;

      #ifdef DIOUART_ACTIVE
      case DIOSTREAMTYPE_UART       : _class = new DIOSTM32FXXXSTREAMUART();        break;
      #endif

      #ifdef DIOUSB_ACTIVE
      case DIOSTREAMTYPE_USB        : _class = new DIOSTM32FXXXSTREAMUSB();         break;
      #endif

      #ifdef DIOSPI_ACTIVE
      case DIOSTREAMTYPE_SPI        : _class = new DIOSTM32FXXXSTREAMSPI();         break;
      #endif

      #ifdef DIOI2C_ACTIVE
      case DIOSTREAMTYPE_I2C        : _class = new DIOSTM32FXXXSTREAMI2C();         break;
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
//  DIOSTM32FXXXFACTORY::DeleteStreamIO
/*
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        streamio :
//
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXFACTORY::DeleteStreamIO(DIOSTREAM* diostream)
{
  if(!diostream) return false;
  delete(diostream);

  return true;
}



#endif




#ifdef DIOGPIO_ACTIVE
/*-------------------------------------------------------------------
//  DIOSTM32FXXXFACTORY::CreateGPIO
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
DIOGPIO* DIOSTM32FXXXFACTORY::CreateGPIO()
{
  DIOSTM32FXXXGPIO* _class = new DIOSTM32FXXXGPIO();

  return (DIOGPIO*)_class;
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXFACTORY::DeleteGPIO
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
bool DIOSTM32FXXXFACTORY::DeleteGPIO(DIOGPIO* port)
{
  if(!port) return false;

  DIOSTM32FXXXGPIO* _port = (DIOSTM32FXXXGPIO*)port;
  delete _port;

  return true;
}

#endif



#endif