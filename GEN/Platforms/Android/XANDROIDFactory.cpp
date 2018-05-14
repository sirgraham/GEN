//------------------------------------------------------------------------------------------
//  XANDROIDFACTORY.CPP
//
//  Android Utils Platform Factory
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 08/11/2010
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XANDROIDDateTime.h"
#include "XANDROIDTimer.h"
#include "XANDROIDRand.h"
#include "XANDROIDFile.h"
#include "XANDROIDDir.h"
#include "XANDROIDSystem.h"
#include "XANDROIDConsole.h"
#include "XANDROIDThread.h"

#include "XFactoryMacros.h"
#include "XANDROIDFactory.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------

CREATEFUNC(XANDROIDFACTORY, XTIMER     , XANDROIDTIMER      , CreateTimer)
DELETEFUNC(XANDROIDFACTORY, XTIMER     , XANDROIDTIMER      , DeleteTimer)

CREATEFUNC(XANDROIDFACTORY, XDATETIME  , XANDROIDDATETIME   , CreateDateTime)
DELETEFUNC(XANDROIDFACTORY, XDATETIME  , XANDROIDDATETIME   , DeleteDateTime)

CREATEFUNC(XANDROIDFACTORY, XRAND      , XANDROIDRAND       , CreateRand)
DELETEFUNC(XANDROIDFACTORY, XRAND      , XANDROIDRAND       , DeleteRand)

CREATEFUNC(XANDROIDFACTORY, XFILE      , XANDROIDFILE       , Create_File)
DELETEFUNC(XANDROIDFACTORY, XFILE      , XANDROIDFILE       , Delete_File)


XDIR* XANDROIDFACTORY::Create_Dir()
{
  XANDROIDDIR* _class =  new XANDROIDDIR();
  return (XDIR*)_class;
}
DELETEFUNC(XANDROIDFACTORY, XDIR       , XANDROIDDIR        , Delete_Dir)


XSYSTEM* XANDROIDFACTORY::CreateSystem()
{
  XANDROIDSYSTEM* _class =  new XANDROIDSYSTEM();
  return (XSYSTEM*)_class;
}
DELETEFUNC(XANDROIDFACTORY, XSYSTEM    , XANDROIDSYSTEM     , DeleteSystem)


XCONSOLE* XANDROIDFACTORY::CreateConsole()
{
  XANDROIDCONSOLE* _class =  new XANDROIDCONSOLE();
  return (XCONSOLE*)_class;
}
DELETEFUNC(XANDROIDFACTORY, XCONSOLE    , XANDROIDCONSOLE   , DeleteConsole)


CREATEFUNC(XANDROIDFACTORY, XMUTEX    , XANDROIDMUTEX     , Create_Mutex)
DELETEFUNC(XANDROIDFACTORY, XMUTEX    , XANDROIDMUTEX     , Delete_Mutex)


XTHREAD* XANDROIDFACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function,void* data)
{
  XANDROIDTHREAD* _class =  new XANDROIDTHREAD(groupID, ID,function,data);
  return (XTHREAD*)_class;
}


bool XANDROIDFACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
{
  if(!xthread) return false;

  XANDROIDTHREAD* _xthread = (XANDROIDTHREAD*)xthread;
  delete _xthread;

  return true;
};

