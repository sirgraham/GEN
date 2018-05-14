/*------------------------------------------------------------------------------------------
//  GRPAPPLICATION.H
*/
/**
// \class
//
//  basic graphical application
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 01/09/2016 12:07:26
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPAPPLICATION_H_
#define _GRPAPPLICATION_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XApplication.h"
#include "XFSMachine.h"
#include "XPath.h"
#include "GRPScene.h"
#include "GRPObject.h"
#include "GRPFile3D.h"

#include "GRPObserver.h"
#include "GRPRenderer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPSCREEN;
class GRPCONTEXT;
class GRPPROPERTIES;
class GRPSCENE;
class GRPRENDERER;
class GRPLOADERINTERFACE;

class INPMANAGER;
class INPDEVICE;

class SNDFACTORY;
class GRPAPPLICATIONCONFIG;

/*---- CLASS -----------------------------------------------------------------------------*/

enum GRPAPPLICATION_OPTIONS
{
    GRPAPPLICATION_OPTIONS_NONE         = 0,
    GRPAPPLICATION_OPTIONS_DEBUG        = 1 << 1,
    GRPAPPLICATION_OPTIONS_INPUT        = 1 << 2,
    GRPAPPLICATION_OPTIONS_LOG          = 1 << 3,
    GRPAPPLICATION_OPTIONS_SCREEN       = 1 << 4,
    GRPAPPLICATION_OPTIONS_RENDERER     = 1 << 5,
    GRPAPPLICATION_OPTIONS_LOADCONFIG   = 1 << 6,

    GRPAPPLICATION_OPTIONS_WINDOW   = GRPAPPLICATION_OPTIONS_SCREEN | GRPAPPLICATION_OPTIONS_RENDERER,
    GRPAPPLICATION_OPTIONS_DEFAULT  = GRPAPPLICATION_OPTIONS_DEBUG  | GRPAPPLICATION_OPTIONS_INPUT | GRPAPPLICATION_OPTIONS_LOG | GRPAPPLICATION_OPTIONS_WINDOW | GRPAPPLICATION_OPTIONS_LOADCONFIG
};

#ifndef GRP_ACTIVE

#define GRPAPPLICATION XAPPLICATION

#else
class GRPAPPLICATION : public XAPPLICATION , public XFSMACHINE, public GRPOBSERVER
{
  public:

      enum GRPAPPLICATION_STATE
      {
          GRPAPPLICATION_STATE_UNKNOWN,
          GRPAPPLICATION_STATE_WAITING_FOR_WINDOW,
          GRPAPPLICATION_STATE_STARTED,
          GRPAPPLICATION_STATE_INIT,
          GRPAPPLICATION_STATE_RENDERING,
          GRPAPPLICATION_STATE_END
      };

                                  GRPAPPLICATION                ()  : XAPPLICATION(),XFSMACHINE(0)        {     Clean();                                };

    bool                          IniApplication                (XSYSTEM* xsystem, XVECTOR<XSTRING*>* execparams = NULL);
    bool                          IsFullScreen                  ()                                        {     return isfullscreen;                    }
    void                          SetIsFullScreen               (bool isfullscreen)                       {     this->isfullscreen    = isfullscreen;   }
    void                          SetMultiSampling              (XDWORD sampling)                         {             multisampling = sampling;       }
    XDWORD                        GetMultiSampling              ()                                        {     return  multisampling;                  }
    GRPSCREEN*                    GetMainScreen                 ()                                        {     return  mainscreen;                     }
    GRPSCENE*                     GetMainScene                  ()                                        {     return  mainscene;                      }
    GRPRENDERER*                  GetMainRenderer               ()                                        {     return  mainrenderer;                   }

    virtual bool                  Notify                        (GRPMSGTYPE type, GRPSUBJECT* subject);

    bool                          PathManagement                (XCHAR* addpath);
    virtual bool                  Update                        ();

    virtual bool                  IniInputManager               ();

    virtual bool                  Ini                           ();
    virtual bool                  End                           ();
    virtual bool                  FirstUpdate                   ();

    virtual bool                  LoadResources                 ();

    virtual bool                  CreateRenderer                ();
    virtual bool                  DeleteRenderer                ();

    virtual bool                  WindowCreated                 ();

    virtual bool                  IniMainScreen                 (GRPPROPERTIES* mainscreenproperties = NULL,  void*     mainscreenhandle = NULL);
    virtual bool                  IniRenderer                   (GRPSCREEN*     screen,                       GRPSCENE* scene);

    virtual bool                  EndRenderer                   ();
    virtual bool                  EndMainScreen                 ();

    virtual bool                  InitLog                       (XCHAR*);
    virtual bool                  InitDebug                     (const int VERSION, const int SUBVERSION, const int SUBVERSIONERR);
    virtual bool                  InitPaths                     ();
    virtual bool                  InitLoader                    (XCHAR* xml, XCHAR* material, XCHAR* shader);
    virtual bool                  LoadConfigurationFromFile     ();

    virtual bool                  SetVersion                    (XDWORD, XDWORD, XDWORD);

    virtual GRPOBJECT*            LoadObject                    (GRPOBJECT* obj, XCHAR* path, XDWORD loadflags=GRPFILE3D_INCLUDE_ALL, XDWORD verbose=GRPFILE3D_VERBOSE_LEVEL_ERRORS, XDWORD NextStackID=0);

    //---------- EVENTS
    virtual bool OnCreate                       ()                  { return true; }
    virtual bool OnCreateRenderer               ()                  { return true; }
    virtual bool OnCreateWindow                 ()                  { return true; }
    virtual bool OnConfig                       ()                  { return true; }
    virtual bool OnInit                         ()                  { return true; }
    virtual bool OnEnd                          ()                  { return true; }
    virtual bool OnEnded                        ()                  { return true; }
    virtual bool OnStartRendering               ()                  { return true; }
    virtual bool OnUpdate                       ()                  { return true; }
    virtual bool OnError                        ()                  { return true; }
    virtual bool OnStart                        ()                  { return true; }
    virtual bool OnResume                       ()                  { return true; }
    virtual bool OnPause                        ()                  { return true; }
    virtual bool OnStop                         ()                  { return true; }
    virtual bool OnDestroy                      ()                  { return true; }
    virtual bool OnFocus                        ()                  { return true; }
    virtual bool OnLostFocus                    ()                  { return true; }
    virtual bool OnKey                          ()                  { return true; }
    virtual bool OnResourcesLoaded              ()                  { return true; }
    virtual bool OnLoadFail                     (XLOADERTASK*)      { this->SetExitStatus(XAPPLICATIONEXITTYPE_APPERROR); return true; }
    virtual bool OnLowMemory                    ()                  { return true; }
    virtual bool OnConfigurationFileLoaded      ()                  { return true; }
    virtual bool OnScreenConfigurationChanged   (XDWORD, XDWORD)    { return true; }


    //---------- methods

    GRPLOADERINTERFACE*           loaderInterface;
    GRPSCREEN*                    mainscreen;
    GRPCONTEXT*                   maincontext;

    GRPSCENE*                     mainscene;    // #Imanol : left behind for
    GRPRENDERER*                  mainrenderer; // #Imanol : for global renderer

    XDWORD                        resolutionx;
    XDWORD                        resolutiony;

    XDWORD                        positionx;
    XDWORD                        positiony;

    INPDEVICE*                    keyboard;
    INPDEVICE*                    mouse;

    XDWORD                        multisampling;

    bool                          isfullscreen;

    XPATH                         rootpath;

    XDWORD                        initoptions;
    XDWORD                        version[3];
    GRPAPPLICATION_STATE          state;

    XSTRING                       configurationfile;
    GRPAPPLICATIONCONFIG*         graphicsettings;

  private:

    void Clean()
    {
      applicationname   = __L("unnamed");
      configurationfile = __L("");

      version[0] = 0;
      version[1] = 0;
      version[2] = 0;

      initoptions = GRPAPPLICATION_OPTIONS_DEFAULT;
      mainscreen  = NULL;
      maincontext = NULL;
      mainscene   = NULL;
      mainrenderer= NULL;

      resolutionx   = 1920>>1;
      resolutiony   = 1080>>1;

      positionx=0;
      positiony=0;

      isfullscreen = true;

      #ifdef HW_PC
        isfullscreen = false;
      #endif

        #if defined(LINUX) && defined(HW_PC)
            multisampling = 0;
        #else
            multisampling = 4;
        #endif

        keyboard        = NULL;
        mouse           = NULL;
        loaderInterface = NULL;

        state           = GRPAPPLICATION_STATE_INIT;
        graphicsettings = NULL;
    }


};
#endif

#endif