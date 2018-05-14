//------------------------------------------------------------------------------------------
//  XSTM32FXXXTHREAD.CPP
//
//  STM32Fxxx thread class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 06/03/2006 11:46:26
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------


#include "XSleep.h"
#include "XDebug.h"
#include "XThreadListNonPreemptive.h"
#include "XSTM32Fxxx_HAL.h"

#include "XSTM32FxxxThread.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------





/*-------------------------------------------------------------------
//  XSTM32FXXXTHREAD::XSTM32FXXXTHREAD
*/
/**
//
//  Class Constructor XSTM32FXXXTHREAD
//
//  @author       Abraham J. Velez
//  @version      21/04/2016 12:20:05
//
//  @param        groupID :
//  @param        ID :
//  @param        function :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
XSTM32FXXXTHREAD::XSTM32FXXXTHREAD(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* data) : XTHREAD(groupID, ID, function, data)
{
  Clean();


}


//-------------------------------------------------------------------
//  XSTM32FXXXTHREAD::~XSTM32FXXXTHREAD
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:03:47
//
//  @return
//  */
//-------------------------------------------------------------------
XSTM32FXXXTHREAD::~XSTM32FXXXTHREAD()
{
  End();

  Clean();
}


//-------------------------------------------------------------------
//  XSTM32FXXXTHREAD::Ini
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:03:47
//
//  @return
//  */
//-------------------------------------------------------------------
bool XSTM32FXXXTHREAD::Ini(bool run)
{
  if(run)
    {
      if(!Run(true)) return false;
    }
   else statusfunc = XTHREADSTATUS_STOP;

  return XTHREADLISTNONPREEMPTIVE::GetInstance().AddThread(this);
}




/*-------------------------------------------------------------------
//  XSTM32FXXXTHREAD::Wait
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
bool XSTM32FXXXTHREAD::Wait(int miliseconds)
{
  xsleep->MilliSeconds(miliseconds);

  return true;
}



//-------------------------------------------------------------------
//  XSTM32FXXXTHREAD::End
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 12:03:47
//
//  @return
//  */
//-------------------------------------------------------------------
bool XSTM32FXXXTHREAD::End()
{
  if(statusfunc == XTHREADSTATUS_END) return false;

  statusfunc = XTHREADSTATUS_EXIT;

  //WaitToEnd();

  return XTHREADLISTNONPREEMPTIVE::GetInstance().DeleteThread(this);
}





//-------------------------------------------------------------------
//  void* XSTM32FXXXTHREAD::Callback
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
/*
void* XSTM32FXXXTHREAD::Callback(void* thread)
{
  XSTM32FXXXTHREAD* th   = NULL;
  XTHREADFUNCTION  func = NULL;
  void*            param;

  th = (XSTM32FXXXTHREAD*)thread;
  if(!th) return NULL;

  func = (XTHREADFUNCTION)th->GetFunction();
  if(!func) return NULL;

  param = th->GetParam();

  if((th->priority!=XTHREADPRIORITY_UNKNOWN) && (th->priority!=XTHREADPRIORITY_NORMAL))
    {
       struct sched_param param;  // scheduling priority
       int policy = SCHED_FIFO;     // scheduling policy

      switch(th->priority)
        {
          case XTHREADPRIORITY_LOW      : policy = SCHED_IDLE;
                                          param.sched_priority = sched_get_priority_min(policy);
                                          break;

          case XTHREADPRIORITY_HIGH     : policy = SCHED_FIFO;
                                          param.sched_priority = sched_get_priority_max(policy)/2;
                                          break;

          case XTHREADPRIORITY_REALTIME : policy = SCHED_FIFO;
                                          param.sched_priority = sched_get_priority_max(policy);
                                          break;
        }

      pthread_setschedparam(th->thhandle, policy, &param);
    }

  while(1)
    {
      if(!th) break;

      if(th->status == XTHREADSTATUS_EXIT)     break;
      if(th->status == XTHREADSTATUS_UNKNOWN)  break;

      if(th->status == XTHREADSTATUS_RUN)
        {
          if(func)
            {
              th->isinfunction = true;
              func(param);
              th->isinfunction = false;

            } else break;
        }

      if(!th) break;

      usleep(XTHREAD_DEFAULTWAITYIELD*1000);
    }


  th->status = XTHREADSTATUS_UNKNOWN;

  pthread_exit(0);

  return NULL;
}

*/
