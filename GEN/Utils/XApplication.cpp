//------------------------------------------------------------------------------------------
//  XAPPLICATION.CPP
//
//  Generic Application
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 09/08/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XPath.h"
#include "XDir.h"
#include "XPublisher.h"
#include "XScheduler.h"
#include "XSystem.h"
#include "XDebug.h"

#include "Main.h"

#include "XApplication.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




//-------------------------------------------------------------------
//  XAPPLICATION::XAPPLICATION
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/08/2002 17:17:17
//
//  @return
//  */
//-------------------------------------------------------------------
XAPPLICATION::XAPPLICATION()
{
  Clean();
}



//-------------------------------------------------------------------
//  XAPPLICATION::~XAPPLICATION
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/08/2002 17:17:45
//
//  @return       void :
//  */
//-------------------------------------------------------------------*
XAPPLICATION::~XAPPLICATION()
{
  Clean();
}





/*-------------------------------------------------------------------
//  XAPPLICATION::IniApplication
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/06/2012 7:45:43
//
//  @return       bool :

//  @param        xsystem :
//  @param        execparams :

*/
/*-----------------------------------------------------------------*/
bool XAPPLICATION::IniApplication(XSYSTEM* xsystem, XVECTOR<XSTRING*>* execparams)
{
  if(!xsystem)    return false;

  #ifdef MICROCONTROLLER
  XDEBUG_ADDTARGET(XDEBUGCTRLTYPE_SPECIAL, XDEBUG_DEFAULT_SPECIALAIM);
  #endif

  this->xsystem     = xsystem;
  this->execparams  = execparams;

  xtimerglobal = xfactory->CreateTimer();
  if(!xtimerglobal) return false;

  xtimerglobal->Reset();

  if(!Ini()) return false;

  #if defined(WINDOWS) || defined(LINUX) || defined(MICROCONTROLLER)
  return FirstUpdate();
  #else
  return true;
  #endif
}




//-------------------------------------------------------------------
//  XAPPLICATION::UpdateApplication
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/03/2004 15:25:05
//
//  @return       bool :
//  @param        exit :
*/
//-------------------------------------------------------------------
bool XAPPLICATION::UpdateApplication()
{
    if (!Update())
    {
        this->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER);
        return false;
    }

  return true;
}



//-------------------------------------------------------------------
//  XAPPLICATION::EndApplication
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/08/2002 17:26:18
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XAPPLICATION::EndApplication()
{
  bool status;

  status = LastUpdate();

  if(status) status = End();

  XPUBLISHER::GetInstance().DeRegisterAllEvents();

  if(xtimerglobal)
    {
      xfactory->DeleteTimer(xtimerglobal);
      xtimerglobal = NULL;
    }

  return status;
}





