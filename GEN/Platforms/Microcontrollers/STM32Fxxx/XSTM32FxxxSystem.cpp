//------------------------------------------------------------------------------------------
//  XSTM32FXXXSYSTEM.CPP
//
//  STM32Fxxx system class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/03/2004 12:28:40
//  Lsdddast Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XString.h"

#include "XSTM32Fxxx_HAL.h"
#include "XSTM32FxxxSystem.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::XSTM32FXXXSYSTEM
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:28:59
//
//  @return
//  */
//-------------------------------------------------------------------
XSTM32FXXXSYSTEM::XSTM32FXXXSYSTEM() : XSYSTEM()
{

}



//-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::~XSTM32FXXXSYSTEM
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:20
//
//  @return
//  */
//-------------------------------------------------------------------
XSTM32FXXXSYSTEM::~XSTM32FXXXSYSTEM()
{

}



/*-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::GetTypeHardware
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/08/2009 05:01:13 p.m.
//
//  @return       XSYSTEM_HARDWARE :
//  */
/*-----------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XSTM32FXXXSYSTEM::GetTypeHardware(int* revision)
{
  return XSYSTEM_HARDWARETYPE_MICRO_STM32FXXX;
}




//-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::GetSO
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:40
//
//  @return       XSTM32FXXXSYSTEM_SO :
//  */
//-------------------------------------------------------------------
XSYSTEM_SO XSTM32FXXXSYSTEM::GetSO()
{
  return XSYSTEM_SO_UNKNOWN;
}


//-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::GetLanguage
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:55
//
//  @return       XLANGUAGE_CODE :
//  */
//-------------------------------------------------------------------
XLANGUAGE_CODE XSTM32FXXXSYSTEM::GetLanguage()
{
  return XLANGUAGE_UNKNOWN;
}



/*-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::GetMemoryInfo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2012 9:22:44
//
//  @return       bool :
//  @param        total :
//  @param        free :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
{
  total = 0;
  free  = 0;

  return true;
}



/*-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::MakeCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/11/2014 20:48:59
//
//  @return       bool :
//
//  @param        command :
//  @param        returncode :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSYSTEM::MakeCommand(XCHAR* command, int* returncode)
{
  return false;
}


/*-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::ExecuteApplication
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/08/2015 16:06:58
//
//  @return       bool :
//
//  @param        command :
//  @param        params :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSYSTEM::ExecuteApplication(XCHAR* command, XCHAR* params, bool special)
{
  return false;
}




/*-------------------------------------------------------------------
//  XSTM32FXXXSYSTEM::ShutDown
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/11/2014 20:48:43
//
//  @return       bool :
//
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
{
  switch(type)
    {
      case XSYSTEM_SHUTDOWNTYPE_REBOOT   :
                                            #if defined(STM32F407xx) || defined(STM32F411xE) || defined(STM32F446x)
                                            { __DSB();                                                                                                                     // Ensure all outstanding memory accesses included buffered write are completed before reset
                                              SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) | (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) | SCB_AIRCR_SYSRESETREQ_Msk);        // Keep priority group unchanged
                                              __DSB();                                                                                                                     // Ensure completion of memory access
                                              while(1);                                                                                                                    // Wait until reset
                                            }
                                            #endif
                                            break;

      case XSYSTEM_SHUTDOWNTYPE_LOGOFF   :  break;

      case XSYSTEM_SHUTDOWNTYPE_POWEROFF :  break;
    }

  return true;
}





