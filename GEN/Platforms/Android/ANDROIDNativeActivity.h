/*------------------------------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY.H
*/
/**
// \class
//
//  ANDROID Native Activity class
//
//  @author  Abraham J. Velez
//  @version 28/09/2013 16:57:29
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _ANDROIDNATIVEACTIVITY_H_
#define _ANDROIDNATIVEACTIVITY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <jni.h>
#include <android/window.h>
#include <android/input.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <android/native_activity.h>

#include "XBase.h"
#include "XSubject.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

typedef int32_t       STATUS;

const STATUS          STATUS_OK   = 0;
const STATUS          STATUS_KO   = -1;
const STATUS          STATUS_EXIT = -2;


/*---- CLASS -----------------------------------------------------------------------------*/



class ANDROIDNATIVEINPUT_HANDLER
{
  public:

                                     ANDROIDNATIVEINPUT_HANDLER()                                               {           };
    virtual                         ~ANDROIDNATIVEINPUT_HANDLER()                                               {           };

    virtual bool                     OnTouchEvent                             (AInputEvent* event)  = 0;
    virtual bool                     OnKeyboardEvent                          (AInputEvent* event)  = 0;
    virtual bool                     OnTrackballEvent                         (AInputEvent* event)  = 0;
  //virtual bool                     OnAccelerometerEvent                     (ASensorEvent* event) = 0;
};




class ANDROIDNATIVEACTIVITY_HANDLER
{
  public:
                                      ANDROIDNATIVEACTIVITY_HANDLER           ()                                {               };
    virtual                          ~ANDROIDNATIVEACTIVITY_HANDLER           ()                                {               };

    virtual STATUS                    OnActivate                              () = 0;
    virtual void                      OnDeactivate                            () = 0;
    virtual STATUS                    OnStep                                  () = 0;

    virtual void                      OnStart                                 ()                                {               };
    virtual void                      OnResume                                ()                                {               };
    virtual void                      OnPause                                 ()                                {               };
    virtual void                      OnStop                                  ()                                {               };
    virtual void                      OnDestroy                               ()                                {               };

    virtual void                      OnSaveState                             (void** data, size_t* size)       {               };
    virtual void                      OnConfigurationChanged                  ()                                {               };
    virtual void                      OnLowMemory                             ()                                {               };

    virtual void                      OnCreateWindow                          ()                                {               };
    virtual void                      OnDestroyWindow                         ()                                {               };
    virtual void                      OnGainFocus                             ()                                {               };
    virtual void                      OnLostFocus                             ()                                {               };
};



class ANDROIDNATIVEACTIVITY_EVENTLOOP
{
  public:
                                      ANDROIDNATIVEACTIVITY_EVENTLOOP         (android_app* application);
    virtual                          ~ANDROIDNATIVEACTIVITY_EVENTLOOP         ();

    void                              Run                                     (ANDROIDNATIVEACTIVITY_HANDLER* activityhandler, ANDROIDNATIVEINPUT_HANDLER* inputhandler);

  protected:

    void                              Activate                                ();
    void                              Deactivate                              ();

    int32_t                           ProcessInputEvent                       (AInputEvent* event, android_app* application);
    void                              ProcessApplicationEvent                 (int32_t command, android_app* application);

  private:

    void                              Clean                                   ();

    static int32_t                    CallbackInput                           (android_app* application, AInputEvent* event);
    static void                       CallbackEvent                           (android_app* app, int32_t command);


    bool                              enabled;                                // Saves application state when application is active/paused.
    bool                              quit;                                   // Indicates if the event handler wants to exit.

    android_app*                      application;                            // Application details provided by Android.
    ANDROIDNATIVEACTIVITY_HANDLER*    activityhandler;                        // Activity event observer.
    ANDROIDNATIVEINPUT_HANDLER*       inputhandler;                           // Input event observer.
};







/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

