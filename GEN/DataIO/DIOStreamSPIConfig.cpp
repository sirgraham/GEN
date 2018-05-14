//------------------------------------------------------------------------------------------
//  DIOSTREAMSPICONFIG.CPP
//
//  Data IO Stream SPI Config class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XSystem.h"

#include "DIOStreamSPIConfig.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMSPICONFIG::DIOSTREAMSPICONFIG
*/
/**
//
//  Class Constructor DIOSTREAMSPICONFIG
//
//  @author       Abraham J. Velez
//  @version      29/05/2014 17:28:34
//

*/
/*-----------------------------------------------------------------*/
DIOSTREAMSPICONFIG::DIOSTREAMSPICONFIG() : DIOSTREAMCONFIG()
{
  Clean();
  type  = DIOSTREAMTYPE_SPI;
}



//-------------------------------------------------------------------
//  DIOSTREAMSPICONFIG::~DIOSTREAMSPICONFIG
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:18:59
//
//  @return
//  */
//-------------------------------------------------------------------
DIOSTREAMSPICONFIG::~DIOSTREAMSPICONFIG()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMSPICONFIG::SetLocalDeviceNameByPort
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/10/2016 8:43:49
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMSPICONFIG::SetLocalDeviceNameByPort()
{
  XSYSTEM* xsystem = xfactory->CreateSystem();
  if(!xsystem) return false;

  int revision = 0;

  switch(xsystem->GetTypeHardware(&revision))
    {
      case XSYSTEM_HARDWARETYPE_PC          : localdevicename.Format(__L("/dev/spi%d"), port);
                                              break;

      case XSYSTEM_HARDWARETYPE_RASPBERRYPI : localdevicename.Format(__L("/dev/spidev%d.%d"), port, chipselect);
                                              break;

                                   default  : break;
    }

  xfactory->DeleteSystem(xsystem);

  return true;
}


