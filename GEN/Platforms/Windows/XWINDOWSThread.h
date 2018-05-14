//------------------------------------------------------------------------------------------
//  XWINDOWSTHREAD.H
//
/**
// \class
//
//  WINDOWS thread class
//
//  @author  Abraham J. Velez
//  @version 06/03/2006 11:41:39
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XWINDOWSTHREAD_H_
#define _XWINDOWSTHREAD_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <windows.h>

#include "XThread.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XWINDOWSMUTEX : public XMUTEX
{
  public:
                            XWINDOWSMUTEX         ();
    virtual                ~XWINDOWSMUTEX         ();

    bool                    Lock                  ();
    bool                    UnLock                ();

  private:

    void                    Clean                 ();

    HANDLE                  mxhandle;
};



class XWINDOWSTHREAD : public XTHREAD
{
  public:
                            XWINDOWSTHREAD        (XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function = NULL,void* data = NULL);
     virtual               ~XWINDOWSTHREAD        ();

     bool                   Ini                   (bool run = true);
     bool                   Wait                  (int miliseconds =  XTHREAD_DEFAULTWAITYIELD);
     bool                   End                   ();

  private:

     void                   Clean                 ();

     static DWORD WINAPI    Callback              (LPVOID thread);
     HANDLE                 thhandle;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

