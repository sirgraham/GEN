//------------------------------------------------------------------------------------------
//  XWINDOWSFACTORY.CPP
//
//  WINDOWS utils factory class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 08/08/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactoryMacros.h"

#include "XWINDOWSTimer.h"
#include "XWINDOWSDateTime.h"
#include "XWINDOWSRand.h"
#include "XWINDOWSFile.h"
#include "XWINDOWSDir.h"
#include "XWINDOWSSystem.h"
#include "XWINDOWSConsole.h"
#ifdef XPHONE_ACTIVE
#include "XWINDOWSPhone.h"
#endif
#include "XWINDOWSThread.h"

#include "XWINDOWSFactory.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


CREATEFUNC(XWINDOWSFACTORY, XTIMER    , XWINDOWSTIMER     , CreateTimer)
DELETEFUNC(XWINDOWSFACTORY, XTIMER    , XWINDOWSTIMER     , DeleteTimer)

CREATEFUNC(XWINDOWSFACTORY, XDATETIME , XWINDOWSDATETIME  , CreateDateTime)
DELETEFUNC(XWINDOWSFACTORY, XDATETIME , XWINDOWSDATETIME  , DeleteDateTime)

CREATEFUNC(XWINDOWSFACTORY, XRAND     , XWINDOWSRAND      , CreateRand)
DELETEFUNC(XWINDOWSFACTORY, XRAND     , XWINDOWSRAND      , DeleteRand)

CREATEFUNC(XWINDOWSFACTORY, XFILE     , XWINDOWSFILE      , Create_File)
DELETEFUNC(XWINDOWSFACTORY, XFILE     , XWINDOWSFILE      , Delete_File)

CREATEFUNC(XWINDOWSFACTORY, XDIR      , XWINDOWSDIR       , Create_Dir)
DELETEFUNC(XWINDOWSFACTORY, XDIR      , XWINDOWSDIR       , Delete_Dir)

CREATEFUNC(XWINDOWSFACTORY, XSYSTEM   , XWINDOWSSYSTEM    , CreateSystem)
DELETEFUNC(XWINDOWSFACTORY, XSYSTEM   , XWINDOWSSYSTEM    , DeleteSystem)

CREATEFUNC(XWINDOWSFACTORY, XCONSOLE  , XWINDOWSCONSOLE   , CreateConsole)
DELETEFUNC(XWINDOWSFACTORY, XCONSOLE  , XWINDOWSCONSOLE   , DeleteConsole)

#ifdef XPHONE_ACTIVE
CREATEFUNC(XWINDOWSFACTORY, XPHONE    , XWINDOWSPHONE     , CreatePhone)
DELETEFUNC(XWINDOWSFACTORY, XPHONE    , XWINDOWSPHONE     , DeletePhone)
#endif

CREATEFUNC(XWINDOWSFACTORY, XMUTEX    , XWINDOWSMUTEX     , Create_Mutex)
DELETEFUNC(XWINDOWSFACTORY, XMUTEX    , XWINDOWSMUTEX     , Delete_Mutex)




/*-------------------------------------------------------------------
//   XWINDOWSFACTORY::CreateThread
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   XTHREAD* :
//
//  @param    XTHREADGROUPID :
//  @param    XCHAR* :
//  @param    XTHREADFUNCTION :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
XTHREAD* XWINDOWSFACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function,void* data)
{
  XWINDOWSTHREAD* _class =  new XWINDOWSTHREAD(groupID, ID,function,data);
  return (XTHREAD*)_class;
}


/*-------------------------------------------------------------------
//   XWINDOWSFACTORY::DeleteThread
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   bool :
//
//  @param    XTHREADGROUPID :
//  @param    XTHREAD* :
//
*//*-----------------------------------------------------------------*/
bool XWINDOWSFACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
{
  if(xthread)
    {
      delete xthread;
      xthread = NULL;
    }

  return true;
};

