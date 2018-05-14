//------------------------------------------------------------------------------------------
//  XLINUXDEBUG.H
//
/**
// \class
//
//  LINUX debug class
//
//  @author  Abraham J. Velez
//  @version 05/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XLINUXDEBUG_H_
#define _XLINUXDEBUG_H_

#ifdef XDEBUG

//---- INCLUDES ----------------------------------------------------------------------------

#include <pthread.h>

#include "XBase.h"
#include "XString.h"
#include "DIOURL.h"

#include "XLINUXFactory.h"
#include "XLINUXDateTime.h"

#include "XDebug.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XLINUXDEBUGCTRL : public XDEBUGCTRL
{
  public:
                          XLINUXDEBUGCTRL       ();
    virtual              ~XLINUXDEBUGCTRL       ();

    void                  PrintSpecial          (XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
    void                  PrintFile             (XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
    void                  PrintNet              (XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);

    bool                  GetHandleNet          (XDEBUGCTRLTARGET* target);
    bool                  CloseHandleNet        (XDEBUGCTRLTARGET* target);

  private:

    void                  Clean                 ()
                          {
                            mutexhandle  = 0;
                          }


    bool                  Lock                  ();
    bool                  UnLock                ();

    XLINUXDATETIME        xdatetime;
    pthread_mutexattr_t   attr;
    pthread_mutex_t       mutex;
    XDWORD                mutexhandle;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif
