//------------------------------------------------------------------------------------------
//  XWINDOWSTHREAD.CPP
//
//  WINDOWS thread class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 06/03/2006 11:46:26
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>
#include <process.h>

#include "XTimer.h"
#include "XDebugTrace.h"

#ifndef BUILDER
#include "WINDOWSMain.h"
#endif

#include "XWINDOWSThread.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  XWINDOWSMUTEX::XWINDOWSMUTEX
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
XWINDOWSMUTEX::XWINDOWSMUTEX()
{
  Clean();

  mxhandle = CreateMutex( NULL, FALSE, NULL );
}


/*-------------------------------------------------------------------
//  XWINDOWSMUTEX::~XWINDOWSMUTEX
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
XWINDOWSMUTEX::~XWINDOWSMUTEX()
{
  if(mxhandle) CloseHandle(mxhandle);

  Clean();
}



/*-------------------------------------------------------------------
//  XWINDOWSMUTEX::Lock
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
bool XWINDOWSMUTEX::Lock()
{
  if(!mxhandle) return false;

  DWORD status = WaitForSingleObject(mxhandle, INFINITE);
  if(status != WAIT_OBJECT_0) return false;

  islock = true;

  //HANDLE handles[1]= { mxhandle };
  //if(MsgWaitForMultipleObjects(1,handles,FALSE, INFINITE, QS_ALLINPUT) != WAIT_OBJECT_0) return false;

  return true;
}


/*-------------------------------------------------------------------
//  XWINDOWSMUTEX::UnLock
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
bool XWINDOWSMUTEX::UnLock()
{
  if(!mxhandle)               return false;
  if(!ReleaseMutex(mxhandle)) return false;

  islock = false;

  return true;
}


/*-------------------------------------------------------------------
//  XWINDOWSMUTEX::Clean
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
void XWINDOWSMUTEX::Clean()
{
  mxhandle = 0;
}




/*-------------------------------------------------------------------
//  XWINDOWSTHREAD::XWINDOWSTHREAD
*/
/**
//
//  Class Constructor XWINDOWSTHREAD
//
//  @author       Abraham J. Velez
//  @version      21/04/2016 10:06:43
//
//  @param        groupID :
//  @param        ID :
//  @param        function :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
XWINDOWSTHREAD::XWINDOWSTHREAD(XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function,void* data) : XTHREAD(groupID, ID,function,data)
{
  Clean();
}



//-------------------------------------------------------------------
//  XWINDOWSTHREAD::~XWINDOWSTHREAD
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:03:47
//
//  @return
//  */
//-------------------------------------------------------------------
XWINDOWSTHREAD::~XWINDOWSTHREAD()
{
  End();

  Clean();
}




/*-------------------------------------------------------------------
//  XWINDOWSTHREAD::Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/02/2011 11:50:31
//
//  @return       bool :
//  @param        run :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSTHREAD::Ini(bool run)
{
  if(run)
    {
      if(!Run(true))
        {
          return false;
        }
    }
   else statusfunc = XTHREADSTATUS_STOP;

  thhandle = CreateThread( NULL, stacksize, Callback, this, 0, NULL);
  if(thhandle == NULL) return false;

  if((priority != XTHREADPRIORITY_UNKNOWN) && (priority != XTHREADPRIORITY_NORMAL))
    {
      switch(priority)
        {
          case XTHREADPRIORITY_LOW       : SetThreadPriority(thhandle, THREAD_PRIORITY_LOWEST);         break;
          case XTHREADPRIORITY_HIGH      : SetThreadPriority(thhandle, THREAD_PRIORITY_HIGHEST);        break;
          case XTHREADPRIORITY_REALTIME  : SetThreadPriority(thhandle, THREAD_PRIORITY_TIME_CRITICAL);  break;
        }
    }

  return true;
}


/*-------------------------------------------------------------------
//  XWINDOWSTHREAD::Wait
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
bool XWINDOWSTHREAD::Wait(int miliseconds)
{
  //WaitForMultipleObjects(1,&thhandle, FALSE, miliseconds?miliseconds:1);

  Sleep(miliseconds?miliseconds:1);

  return true;
}


/*-------------------------------------------------------------------
//  XWINDOWSTHREAD::End
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2008 08:35:17 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XWINDOWSTHREAD::End()
{
  if(statusfunc == XTHREADSTATUS_END) return false;

  statusfunc = XTHREADSTATUS_EXIT;

  WaitToEnd();

  if(thhandle)
    {
      CloseHandle(thhandle);
      thhandle = NULL;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XWINDOWSTHREAD::Clean
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
void XWINDOWSTHREAD::Clean()
{
  thhandle = NULL;
}




//-------------------------------------------------------------------
//  void* XWINDOWSTHREAD::Callback
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:14:02
//
//  @return
//  @param        thread :
*/
//-------------------------------------------------------------------
DWORD WINAPI XWINDOWSTHREAD::Callback(LPVOID thread)
{
  XWINDOWSTHREAD*  th;
  XTHREADFUNCTION  func;
  void*            param;

  th = (XWINDOWSTHREAD*)thread;
  if(!th) return 0;

  #ifndef BUILDER
  //__try
  #endif
    {
      th = (XWINDOWSTHREAD*)thread;
      if(!th) return 0;

      func = (XTHREADFUNCTION)th->GetFunction();
      if(!func) return 0;

      param = th->GetParam();

      th->gotofunction = true;

      while(1)
        {
          if(!th) break;

          if(th->statusfunc == XTHREADSTATUS_EXIT)     break;
          if(th->statusfunc == XTHREADSTATUS_END)      break;

          if(th->statusfunc == XTHREADSTATUS_RUN)
            {
              if(func)
                {
                  th->isinfunction = true;
                  func(param);
                  th->isinfunction = false;

                } else break;
            }

          if(!th) break;

          if(th->waityield) Sleep(th->waityield);
        }

      if(th) th->statusfunc = XTHREADSTATUS_END;
    }

  #ifndef BUILDER
  //__except(Exception_Filter(GetExceptionCode(), GetExceptionInformation()))
  //  {
  //    if(th)
  //      {
  //        if(th->thhandle)
  //          {
  //            CloseHandle(th->thhandle);
  //            th->thhandle = NULL;
  //          }
  //      }
  //  }
  #endif

  return 0;
}


