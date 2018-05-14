//------------------------------------------------------------------------------------------
//  STM32FXXXMAIN.CPP
//
//  STM32Fxxx main
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/10/2005 16:00:46
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>

#include "XSTM32Fxxx_HAL.h"
#include "XSTM32FxxxFactory.h"
#include "XSTM32FxxxSystem.h"
#include "XSTM32FxxxSleep.h"
#include "XSTM32FxxxDebug.h"

#include "STM32FxxxIni.h"

#ifdef DIO_ACTIVE
#include "DIOSTM32FxxxFactory.h"
#endif

#include "XThreadListNonPreemptive.h"
#include "XApplication.h"

#include "STM32FxxxMain.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

#ifdef XDEBUG
XSTM32FXXXDEBUGCTRL debug;
#endif

STM32FXXXMAIN      stm32fxxxmain;

//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  STM32FXXXMAIN::STM32FXXXMAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
STM32FXXXMAIN::STM32FXXXMAIN()
{
  Clean();
}


//-------------------------------------------------------------------
//  STM32FXXXMAIN::~STM32FXXXMAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
STM32FXXXMAIN::~STM32FXXXMAIN()
{
  Clean();
}



/*-------------------------------------------------------------------
//  STM32FXXXMAIN::Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/03/2011 14:04:56
//
//  @return       bool :
//  @param        xpath :
//  @param        mainscreenhandle :
*/
/*-----------------------------------------------------------------*/
bool STM32FXXXMAIN::Ini()
{
  typesystem = MAIN_SYSTEM_MICRO_STM32FXXX;

  HAL_Init();                          // Reset of all peripherals, Initializes the Flash interface and the Systick.
  SystemClock_Config();                // Configure the system clock

  MX_GPIO_Init();                      // Initialize all configured peripherals
  MX_TIMER3_Init();                    // Initialize TIMER3 for ThreadListNonPreemptive
  MX_NVIC_Init();

  HAL_TIM_Base_Start_IT(&htim3);

  #ifdef XDEBUG
  debug.IniUARTSpecial();
  #endif

  xfactory = (XFACTORY*)new XSTM32FXXXFACTORY();
  if(!xfactory) return false;

  xsleep = (XSLEEP*) new XSTM32FXXXSLEEP();
  if(!xsleep) return false;

  xsystem = new XSTM32FXXXSYSTEM();
  if(!xsystem) return false;

  #ifdef DIO_ACTIVE
  diofactory = (DIOFACTORY*)new DIOSTM32FXXXFACTORY();
  if(!diofactory) return false;
  #endif

  xapplication  = XAPPLICATION::Create();
  if(!xapplication) return false;

  xapplication->SetMain(&stm32fxxxmain);

  if(!xapplication->IniApplication(xsystem)) return false;

  return true;
}



//-------------------------------------------------------------------
//  STM32FXXXMAIN::Update
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:17:1
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool STM32FXXXMAIN::Update()
{
  if(!xapplication) return false;

  xapplication->UpdateApplication();

  return true;
}



//-------------------------------------------------------------------
//  STM32FXXXMAIN::End
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:10:57
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool STM32FXXXMAIN::End()
{
  #ifdef DIO_ACTIVE
  if(diofactory)
    {
      delete diofactory;
      diofactory = NULL;
    }
  #endif

  if(xapplication)
    {
      delete xapplication;
      xapplication = NULL;
    }

  if(xsystem)
    {
      delete xsystem;
      xsystem = NULL;
    }

  if(xsleep)
    {
      delete xsleep;
      xsleep = NULL;
    }

  if(xfactory)
    {
      delete xfactory;
      xfactory = NULL;
    }

  return true;
}


/*-------------------------------------------------------------------
//  STM32FXXXMAIN::GetApplication
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2012 16:33:59
//
//  @return       XAPPLICATION* :
//  */
/*-----------------------------------------------------------------*/
XAPPLICATION* STM32FXXXMAIN::GetXApplication()
{
  return xapplication;
}


//-------------------------------------------------------------------
//  STM32FXXXMAIN::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2003 12:09:01
//
//  @return
//  */
//-------------------------------------------------------------------
void STM32FXXXMAIN::Clean()
{
  xsystem      = NULL;
  xapplication = NULL;
}


//-------------------------------------------------------------------
//  main
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/10/2005 15:58:09
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int main()
{
  int status = 0;

  if(stm32fxxxmain.Ini())
    {
      while( stm32fxxxmain.Update())
        {

        }

    } else status = 1;

  if(!stm32fxxxmain.End()) status = 1;

  return status;
}



/*-------------------------------------------------------------------
//  new
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/09/2016 13:08:41
//
//  @return       void* :
//
//  @param        size :
*/
/*-----------------------------------------------------------------*/
void* operator new(size_t size)
{
  __disable_irq();

  void* ptr = malloc(size);

  __enable_irq();

  return ptr;
}




/*-------------------------------------------------------------------
//  delete
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/09/2016 13:08:47
//
*/
/*-----------------------------------------------------------------*/
void operator delete(void *ptr)
{
  __disable_irq();

  free(ptr);

   __enable_irq();
}
