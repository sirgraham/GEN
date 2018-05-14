//------------------------------------------------------------------------------------------
//  XANDROIDDEBUG.H
//
/**
// \class
//
//  ANDROID debug class
//
//  @author  Abraham J. Velez
//  @version 05/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XANDROIDDEBUG_H_
#define _XANDROIDDEBUG_H_

#ifdef XDEBUG

//---- INCLUDES ----------------------------------------------------------------------------

#include <pthread.h>

#include "XBase.h"
#include "XString.h"
#include "DIOURL.h"

#include "XANDROIDFactory.h"

#include "XDebug.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define XANDROIDDEBUG_LOG_INFO(...)       __android_log_print(ANDROID_LOG_INFO, FRAMEWORKNAME, "%s", __VA_ARGS__)
#define XANDROIDDEBUG_LOG_ERROR(...)      __android_log_print(ANDROID_LOG_INFO, FRAMEWORKNAME, "%s", __VA_ARGS__)
#define XANDROIDDEBUG_LOG_WARNING(...)    __android_log_print(ANDROID_LOG_INFO, FRAMEWORKNAME, "%s", __VA_ARGS__)
#define XANDROIDDEBUG_LOG_DEBUG(...)      __android_log_print(ANDROID_LOG_INFO, FRAMEWORKNAME, "%s", __VA_ARGS__)

//---- CLASS -------------------------------------------------------------------------------

class XANDROIDDEBUGCTRL : public XDEBUGCTRL
{
  public:
                          XANDROIDDEBUGCTRL       ();
    virtual              ~XANDROIDDEBUGCTRL       ();


    void                  PrintSpecial            (XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
    void                  PrintFile               (XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
    void                  PrintNet                (XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);

  private:

    void                  Clean                   ();

    bool                  Lock                    ();
    bool                  UnLock                  ();

    XDWORD                mutexhandle;
    pthread_mutex_t       mutex;
    pthread_mutexattr_t   attr;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif
