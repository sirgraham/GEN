//------------------------------------------------------------------------------------------
//  XANDROIDTHREAD.CPP
//
//  Android Thread class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 06/03/2006 11:46:26
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <errno.h>

#include "XDebug.h"
#include "XANDROIDThread.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  XANDROIDMUTEX::XANDROIDMUTEX
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 11:43:12
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XANDROIDMUTEX::XANDROIDMUTEX()
{
  Clean();

  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  if(!pthread_mutex_init(&mutex, &attr)) init = true;

}


/*-------------------------------------------------------------------
//  XANDROIDMUTEX::~XANDROIDMUTEX
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 11:43:31
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
XANDROIDMUTEX::~XANDROIDMUTEX()
{
  if(init)
    {
      pthread_mutex_destroy(&mutex);
      init = false;
    }

  Clean();
}



/*-------------------------------------------------------------------
//  XANDROIDMUTEX::Lock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 11:44:27
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XANDROIDMUTEX::Lock()
{
  if(!init)                      return false;
  if(pthread_mutex_lock(&mutex)) return false;

  return true;
}


/*-------------------------------------------------------------------
//  XANDROIDMUTEX::UnLock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 11:44:30
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XANDROIDMUTEX::UnLock()
{
  if(!init)                        return false;
  if(pthread_mutex_unlock(&mutex)) return false;

  return true;
}


/*-------------------------------------------------------------------
//  XANDROIDMUTEX::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 11:44:34
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XANDROIDMUTEX::Clean()
{
  init = false;
}





/*-------------------------------------------------------------------
//  XANDROIDTHREAD::XANDROIDTHREAD
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/11/2008 08:47:29 p.m.
//
//  @return
//  @param        ID :
//  @param        function :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
XANDROIDTHREAD::XANDROIDTHREAD(XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function,void* data) : XTHREAD(groupID, ID,function,data)
{
  Clean();
}


//-------------------------------------------------------------------
//  XANDROIDTHREAD::~XANDROIDTHREAD
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:03:47
//
//  @return
//  */
//-------------------------------------------------------------------
XANDROIDTHREAD::~XANDROIDTHREAD()
{
  End();

  Clean();
}


//-------------------------------------------------------------------
//  XANDROIDTHREAD::Ini
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:03:47
//
//  @return
//  */
//-------------------------------------------------------------------
bool XANDROIDTHREAD::Ini(bool run)
{
  if(run)
    {
      if(!Run(true)) return false;
    }
   else statusfunc = XTHREADSTATUS_STOP;


  pthread_attr_t attr;

  if(!stacksize) SetStackSize(1024*1024);

  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr,stacksize);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  int status = pthread_create(&thhandle,&attr,Callback,this);

  pthread_attr_destroy(&attr);


  //int status = pthread_create(&thhandle,NULL,Callback,this);


  return (status)?false:true;
}


/*-------------------------------------------------------------------
//  XANDROIDTHREAD::Wait
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/03/2011 11:16:03
//
//  @return       bool :
//  @param        miliseconds :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDTHREAD::Wait(int miliseconds)
{
  usleep(1000*miliseconds);

  return true;
}



//-------------------------------------------------------------------
//  XANDROIDTHREAD::End
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:03:47
//
//  @return
//  */
//-------------------------------------------------------------------
bool XANDROIDTHREAD::End()
{
  if(statusfunc == XTHREADSTATUS_END) return false;

  statusfunc = XTHREADSTATUS_EXIT;

  WaitToEnd();

  thhandle = -1;

  return true;
}



/*-------------------------------------------------------------------
//  XANDROIDTHREAD::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2008 09:33:02 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
void XANDROIDTHREAD::Clean()
{
  thhandle = -1;
}



//-------------------------------------------------------------------
//  XANDROIDTHREAD::WaitToEnd
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:10:41
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XANDROIDTHREAD::WaitToEnd()
{
  if(thhandle==-1)  return false;

  unsigned long rc=0;

  rc = pthread_join(thhandle, NULL);


  do{ usleep(1000);

    } while(GetStatusFunc() != XTHREADSTATUS_END);


  return true;
}



//-------------------------------------------------------------------
//  void* XANDROIDTHREAD::Callback
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:14:02
//
//  @return       static :
//  @param        thread :
*/
//-------------------------------------------------------------------
void* XANDROIDTHREAD::Callback(void* thread)
{
  XANDROIDTHREAD*  th;
  XTHREADFUNCTION  func;
  void*            param;

  th = (XANDROIDTHREAD*)thread;
  if(!th) return NULL;

  func = (XTHREADFUNCTION)th->GetFunction();
  if(!func) return NULL;

  param = th->GetParam();

  th->gotofunction = true;

  while(1)
    {
      if(!th) break;

      if(th->GetStatusFunc() == XTHREADSTATUS_EXIT)    break;
      if(th->GetStatusFunc() == XTHREADSTATUS_END)  break;

      if(th->GetStatusFunc() == XTHREADSTATUS_RUN)
        {
          if(func)
                 func(param);
            else break;
        }

      if(!th) break;

      usleep(1000);
    }

  th->statusfunc = XTHREADSTATUS_END;

  pthread_exit(0);

  return NULL;
}


