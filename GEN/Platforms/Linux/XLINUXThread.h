//------------------------------------------------------------------------------------------
//  XLINUXTHREAD.H
//
/**
// \class
//
//  LINUX thread class
//
//  @author  Abraham J. Velez
//  @version 06/03/2006 11:41:39
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XLINUXTHREAD_H_
#define _XLINUXTHREAD_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <pthread.h>

#include "XThread.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XLINUXMUTEX : public XMUTEX
{
  public:
                          XLINUXMUTEX           ();
    virtual              ~XLINUXMUTEX           ();

    bool                  Lock                  ();
    bool                  UnLock                ();

  private:

    void                  Clean                 ();

    bool                  init;
    pthread_mutex_t       mutex;
    pthread_mutexattr_t   attr;
};



class XLINUXTHREAD  : public XTHREAD
{
  public:
                          XLINUXTHREAD          (XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function = NULL,void* data = NULL);
     virtual             ~XLINUXTHREAD          ();

     bool                 Ini                   (bool run = true);
     bool                 Wait                  (int miliseconds = XTHREAD_DEFAULTWAITYIELD);
     bool                 End                   ();


  private:

     void                 Clean                 ();

     static void*         Callback              (void* thread);
     pthread_t            thhandle;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

