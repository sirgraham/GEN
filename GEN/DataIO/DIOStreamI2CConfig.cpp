//------------------------------------------------------------------------------------------
//  DIOSTREAMI2C.CPP
//
//  Data IO Stream I2C Config class
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

#include "DIOStreamI2CConfig.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOSTREAMI2CCONFIG::DIOSTREAMI2CCONFIG
*/
/**
//
//  Class Constructor DIOSTREAMI2CCONFIG
//
//  @author       Abraham J. Velez
//  @version      29/05/2014 17:28:02
//

*/
/*-----------------------------------------------------------------*/
DIOSTREAMI2CCONFIG::DIOSTREAMI2CCONFIG() : DIOSTREAMCONFIG()
{
  Clean();

  type  = DIOSTREAMTYPE_I2C;
}



//-------------------------------------------------------------------
//  DIOSTREAMI2CCONFIG::~DIOSTREAMI2CCONFIG
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:18:59
//
//  @return
//  */
//-------------------------------------------------------------------
DIOSTREAMI2CCONFIG::~DIOSTREAMI2CCONFIG()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMI2CCONFIG::SetLocalDeviceNameByPort
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 12:55:42
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMI2CCONFIG::SetLocalDeviceNameByPort()
{  
  int revision = 0;

  switch(XSYSTEM::GetInstance().GetTypeHardware(&revision))
    {
      case XSYSTEM_HARDWARETYPE_PC          : localdevicename.Format(__L("/dev/i2c-%d"), port);
                                              break;

      case XSYSTEM_HARDWARETYPE_RASPBERRYPI : if(revision == 1)
                                                    localdevicename = __L("/dev/i2c-0");
                                               else localdevicename = __L("/dev/i2c-1");
                                              break;

                                   default  : break;
    }

  return true;
}