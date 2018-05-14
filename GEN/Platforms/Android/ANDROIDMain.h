//------------------------------------------------------------------------------------------
//  ANDROIDMAIN.H
//
/**
// \class ANDROIDMAIN
//
//  Android Main function for C++
//
//  @author  Abraham J. Velez
//  @version 10/01/2010
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _ANDROIDMAIN_H_
#define _ANDROIDMAIN_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XString.h"
#include "Main.h"

#include "XANDROIDDebug.h"

#include "GRPANDROIDScreen.h"

#include "ANDROIDNativeActivity.h"

#include "GRPApplication.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

class XFACTORY;
class XPATHS;
class XPATH;
class XSTRING;
class XAPPLICATION;
class XANDROIDFACTORY;
class XANDROIDSYSTEM;
class GRPANDROIDFACTORY;
class INPMANAGER;
class INPANDROIDDEVICEKEYBOARD;
class INPANDROIDDEVICEMOUSE;
class DIOANDROIDFACTORY;
class INPDEVICE;



class ANDROIDMAIN : public ANDROIDNATIVEACTIVITY_HANDLER, public ANDROIDNATIVEINPUT_HANDLER, public MAIN
{
  public:
                                    ANDROIDMAIN                         ();
    virtual                        ~ANDROIDMAIN                         ();

    bool                            Ini                                 (XSTRING* apkpath, XSTRING* xpath);
    bool                            Update                              ();
    bool                            End                                 ();

    GRPAPPLICATION*                 GetXApplication                     ();

    #ifdef INP_ACTIVE
    INPDEVICE*                      GetKeyboard                         ();
    INPDEVICE*                      GetTouchscreen                      ();
    #endif

    void                            SetAndroidApplication               (android_app* application)
                                    {
                                      this->application = application;
                                    }

    android_app*                    GetAndroidApplication()
                                    {
                                      return this->application;
                                    }

  protected:

    bool                            OnTouchEvent                        (AInputEvent* event);
    bool                            OnKeyboardEvent                     (AInputEvent* event);
    bool                            OnTrackballEvent                    (AInputEvent* event);

    STATUS                          OnActivate                          ();
    void                            OnDeactivate                        ();
    STATUS                          OnStep                              ();

    void                            OnStart                             ();
    void                            OnResume                            ();
    void                            OnPause                             ();
    void                            OnStop                              ();
    void                            OnDestroy                           ();

    void                            OnSaveState                         (void** data, size_t* size);
    void                            OnConfigurationChanged              ();
    void                            OnLowMemory                         ();

    void                            OnCreateWindow                      ();
    void                            OnDestroyWindow                     ();
    void                            OnGainFocus                         ();
    void                            OnLostFocus                         ();

    bool                            GetPackageResourcePath              (struct android_app* app, XSTRING& path);
    bool                            GetAbsolutePath                     (struct android_app* app, XSTRING& path);
    bool                            GetDPI                              (struct android_app* app);

  private:

    void                            Clean                               ();

    bool                            OverturnAssetsToExternalLocation    (XPATH& origin ,XPATH& target);
    bool                            OverturnAssetsToExternalLocation    (XPATH& origin ,XCHAR* target);
    bool                            OverturnAssetsToExternalLocation    (XCHAR* origin ,XPATH& target);
    bool                            OverturnAssetsToExternalLocation    (XCHAR* origin ,XCHAR* target);

    #ifdef INP_ACTIVE
    bool                            InputDevices                        (INPMANAGER* inpmanager, GRPANDROIDSCREEN* screen, bool create);
    #endif

    XANDROIDSYSTEM*                 xsystem;

    #ifdef INP_ACTIVE
    //INPANDROIDDEVICEKEYBOARD*       keyboard;
    INPDEVICE* keyboard;
    //INPANDROIDDEVICEMOUSE*          mouse;
    INPDEVICE* mouse;
    #endif


    android_app*                    application;

    bool                            initialized;

    XAPPLICATION*                   xapplication;
    XTIMER*                         timer;


};


//---- DEFINITIONS -------------------------------------------------------------------------------

#ifdef XDEBUG
extern XANDROIDDEBUGCTRL  debug;
#endif

extern ANDROIDMAIN        androidmain;

#endif


