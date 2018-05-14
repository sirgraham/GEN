//------------------------------------------------------------------------------------------
//  XTHREAD.CPP
//
//  Thread for Windows
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 06/03/2006 11:46:26
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XTimer.h"
#include "XThread.h"

#include "XMemory.h"



//---- GENERAL VARIABLE --------------------------------------------------------------------



//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XTHREAD::XTHREAD
*/
/**
//
//  Class Constructor XTHREAD
//
//  @author       Abraham J. Velez
//  @version      21/04/2016 10:02:26
//
//  @param        groupID :
//  @param        ID :
//  @param        function :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
XTHREAD::XTHREAD(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
{
  Clean();

  statusfunc = XTHREADSTATUS_NONE;

  this->groupID     = groupID;
  this->ID          = ID;
  this->function    = function;
  this->param       = param;

}



//-------------------------------------------------------------------
//  XTHREAD::~XTHREAD
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:03:47
//
//  @return
//  */
//-------------------------------------------------------------------
XTHREAD::~XTHREAD()
{
  Clean();
}



/*-------------------------------------------------------------------
//  XTHREAD::WaitToEnd
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      02/11/2016 10:43:30
//
//  @return       bool :
//
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool XTHREAD::WaitToEnd(XDWORD timeout)
{

  if(!gotofunction)
    {
      statusfunc = XTHREADSTATUS_END;
      return true;
    }


  bool statustimeout = false;

  XTIMER* xtimerout = xfactory->CreateTimer();
  if(!xtimerout) return false;

  while(statusfunc != XTHREADSTATUS_END)
    {
      Wait();

      if(xtimerout->GetMeasureSeconds() >= timeout)
        {
          statustimeout = true;
          break;
        }
    }

  xfactory->DeleteTimer(xtimerout);

  return (!statustimeout);
}
