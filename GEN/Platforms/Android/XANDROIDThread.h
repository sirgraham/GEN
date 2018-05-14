//------------------------------------------------------------------------------------------
//  XANDROIDTHREAD.H
//
/**
// \class
//
//  Android Thread class
//
//  @author  Abraham J. Velez
//  @version 06/03/2006 11:41:39
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XANDROIDTHREAD_H_
#define _XANDROIDTHREAD_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <pthread.h>

#include "XThread.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XANDROIDMUTEX : public XMUTEX
{
  public:
                          XANDROIDMUTEX         ();
    virtual              ~XANDROIDMUTEX         ();

    bool                  Lock                  ();
    bool                  UnLock                ();

  private:

    void                  Clean                 ();

    bool                  init;
    pthread_mutex_t       mutex;
    pthread_mutexattr_t   attr;
};



class XANDROIDTHREAD  : public XTHREAD
{
  public:
                          XANDROIDTHREAD        (XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function = NULL,void* data = NULL);
     virtual             ~XANDROIDTHREAD        ();

     bool                 Ini                   (bool run = true);
     bool                 Wait                  (int miliseconds = 100);
     bool                 End                   ();


  private:

     void                 Clean                 ();
     bool                 WaitToEnd             ();

     static void*         Callback              (void* thread);
     pthread_t            thhandle;

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

