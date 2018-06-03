//------------------------------------------------------------------------------------------
//  ANDROIDMAIN.CPP
//
//  Android Main Functions class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/10/2010 16:00:46
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "XPath.h"
#include "XDir.h"
#include "XFileZIP.h"
#include "XApplication.h"

#include "XANDROIDFactory.h"
#include "XANDROIDSleep.h"
#include "XANDROIDSystem.h"

#include "XPath.h"
#include "XPathsManager.h"
#include "XString.h"
#include "XLog.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XLanguage.h"
#include "XApplication.h"
#include "XDebug.h"

#ifdef GRP_ACTIVE
#include "GRPANDROIDFactory.h"
#include "GRPANDROIDScreen.h"
#endif

#ifdef INP_ACTIVE
#include "INPButton.h"
#include "INPANDROIDDeviceKeyboard.h"
#include "INPANDROIDDeviceMouse.h"
#include "INPManager.h"
#include "INPFactory.h"
#include "INPANDROIDFactory.h"
#endif

#ifdef SND_ACTIVE
#include "SNDAndroidFactory.h"
#endif

#ifdef DIO_ACTIVE
#include "DIOANDROIDFactory.h"
#endif

#include "ANDROIDMain.h"
#include "ANDROIDJNI.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

#ifdef XDEBUG
XANDROIDDEBUGCTRL debug;
#endif

ANDROIDMAIN       androidmain;
void*             java_vm;



//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  ANDROIDMAIN::ANDROIDMAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
ANDROIDMAIN::ANDROIDMAIN()
{
  Clean();
}


//-------------------------------------------------------------------
//  ANDROIDMAIN::~ANDROIDMAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
ANDROIDMAIN::~ANDROIDMAIN()
{
  Clean();
}



/*-------------------------------------------------------------------
//   ANDROIDMAIN::Ini
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  26/02/2018 10:08:47
//
//  @return   bool :
//
//  @param    XSTRING* :
//  @param    XSTRING* :
//
*//*-----------------------------------------------------------------*/
bool ANDROIDMAIN::Ini(XSTRING* apkpath, XSTRING* xpath)
{
  // #Imanol : required for openal as static library
  java_vm = this->application->activity->vm;

  //JNIEnv* env = this->application->activity->env;

  xfactory = new XANDROIDFACTORY();
  if(!xfactory)   return false;

  timer = xfactory->CreateTimer();
  timer->Reset();

  xsleep  = new XANDROIDSLEEP();
  if(!xsleep)  return false;

  if(xpath)
    {
      XPATHSMANAGER::GetInstance().GetAppExecPath()->Set(xpath->Get());
      XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT , (*xpath));
    }

  #ifdef XDEBUG_VIRTUALCLOCKTICK
  xtimerclock = new XTIMERCLOCK();
  if(!xtimerclock) return false;
  #endif

  xsystem = new XANDROIDSYSTEM();
  if(!xsystem)    return false;

  XBUFFER::SetHardwareUseLittleEndian(xsystem->HardwareUseLittleEndian());

  xlanguage = new XLANGUAGE();
  if(!xlanguage) return false;

  GetDPI(this->application);

  #ifdef DIO_ACTIVE
  diofactory = new DIOANDROIDFACTORY();
  if(!diofactory) return false;
  #endif

  #ifdef INP_ACTIVE
  XDEBUG_PRINTCOLOR(0,__L("Creating INPANDROIDFACTORY"));
  INPFACTORY::instance=new INPANDROIDFACTORY();
  #endif

  xapplication  = XAPPLICATION::Create();
  if(!xapplication) return false;

  xapplication->SetMain(&androidmain);


  #ifdef SND_ACTIVE
  SNDFACTORY::Instance = new SNDANDROIDFACTORY();
  if(!SNDFACTORY::Instance)
    {
      return false;
    }
  SNDFACTORY::Get()->IniEvents();
  #endif


  XPATH xpathroot;
  XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);

  if(apkpath)  OverturnAssetsToExternalLocation( apkpath->Get(), xpathroot.Get());

  if(!xapplication->IniApplication(xsystem, GetExecParams()))  return false;

  GetXApplication()->state = GRPAPPLICATION::GRPAPPLICATION_STATE_WAITING_FOR_WINDOW;

  return true;
}



//-------------------------------------------------------------------
//  ANDROIDMAIN::Update
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:17:1
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool ANDROIDMAIN::Update()
{
  if(!xapplication) return false;

  xapplication->UpdateApplication();

  if(xapplication->GetExitStatus() != XAPPLICATIONEXITTYPE_NONE)
    {
      xapplication->LastUpdate();

      return false;
    }

  return true;
}




//-------------------------------------------------------------------
//  ANDROIDMAIN::End
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:10:57
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool ANDROIDMAIN::End()
{
  #ifdef SND_ACTIVE
  if(SNDFACTORY::Get()) SNDFACTORY::Get()->EndEvents();
  #endif

  if(xapplication) xapplication->EndApplication();

  #ifdef GRP_ACTIVE
  if(grpfactory)
    {
      delete grpfactory;
      grpfactory = NULL;
    }
  #endif

  #ifdef SND_ACTIVE
  if(SNDFACTORY::Get())
    {
      delete SNDFACTORY::Get();
    }
  #endif

  #ifdef DIO_ACTIVE
  if(diofactory)
    {
      delete diofactory;
      diofactory = NULL;
    }
  #endif

  if(xapplication)
    {
      delete xapplication;
      xapplication = NULL;
    }

  #ifdef XDEBUG_VIRTUALCLOCKTICK
  if(xtimerclock)
    {
      delete xtimerclock;
      xtimerclock = NULL;
    }
  #endif

  if(xlanguage)
    {
      delete xlanguage;
      xlanguage = NULL;
    }


  if(xsystem)
    {
      delete xsystem;
      xsystem = NULL;
    }

  XPUBLISHER::DelInstance();

  XPATHSMANAGER::DelInstance();

  if(xsleep)
    {
      delete xsleep;
      xsleep = NULL;
    }

  if(xfactory)
    {
      delete xfactory;
      xfactory = NULL;
    }

  DeleteAllExecParams();

  XFILE_DISPLAYNOTCLOSEFILES
  XMEMORY_DISPLAYMEMORYLEAKS

  return true;
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::GetXApplication
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/11/2010 17:56:36
//
//  @return       XAPPLICATION* :
//  */
/*-----------------------------------------------------------------*/
GRPAPPLICATION* ANDROIDMAIN::GetXApplication()
{
  return  static_cast<GRPAPPLICATION*>(xapplication);
}




#ifdef INP_ACTIVE
/*-------------------------------------------------------------------
//  ANDROIDMAIN::GetKeyboard
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/11/2010 17:56:36
//
//  @return       INPANDROIDDEVICEKEYBOARD* :
//  */
/*-----------------------------------------------------------------*/
INPDEVICE* ANDROIDMAIN::GetKeyboard()
{
  return keyboard;
}


/*-------------------------------------------------------------------
//  ANDROIDMAIN::GetTouchscreen
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/11/2010 17:56:36
//
//  @return       INPANDROIDDEVICETOUCHSCREEN* :
//  */
/*-----------------------------------------------------------------*/
INPDEVICE* ANDROIDMAIN::GetTouchscreen()
{
  return mouse;
}
#endif





/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnTouchEvent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/10/2013 23:28:15
//
//  @return       bool :
//  @param        event :
*/
/*-----------------------------------------------------------------*/
#include <android/log.h>
bool ANDROIDMAIN::OnTouchEvent(AInputEvent* event)
{
//  XDEBUG_PRINTCOLOR(2, __L("> ANDROID OnTouchEvent"));

  int32_t action            = AMotionEvent_getAction(event);
  size_t  pointercount      = AMotionEvent_getPointerCount(event);

  if    (!mouse)
        mouse = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_MOUSE);//androidmain.GetTouchscreen();
  else
  for(size_t c = 0; c<pointercount; ++c)
    {
      int32_t pointerID         = c;
      int32_t pointerIndex      = AMotionEvent_getPointerId(event,c);

      if (pointerIndex >= this->mouse->GetCursors()->GetSize())
      {
      INPCURSOR* cursor = new INPCURSOR();
      if(!cursor) return false;

      cursor->SetID(INPCURSOR_ID(INPCURSOR_ID_TOUCHSCREEN1+pointerIndex));
      cursor->SetHavePreSelect(false);
      this->mouse->GetCursors()->Add(cursor);
      }

      if (pointerIndex >= this->mouse->GetButtons()->GetSize())
      {
      INPBUTTON* button = new INPBUTTON();
      if(!button) return false;

      button->SetID(INPBUTTON_ID_TOUCH);
      this->mouse->GetButtons()->Add(button);
      }

      float x       = AMotionEvent_getX       (event, pointerID);
      float y       = AMotionEvent_getY       (event, pointerID);
      float pressure= AMotionEvent_getPressure(event, pointerID);



      //__android_log_print(ANDROID_LOG_INFO, "MyTag", "Motion %d x %f y %f p %f a %d", pointerID,x,y,pressure,action);

      if(mouse)
        {
          switch(action & AMOTION_EVENT_ACTION_MASK)
            {
              case AMOTION_EVENT_ACTION_UP            :
              case AMOTION_EVENT_ACTION_POINTER_UP    :
                                                      {
                                                        mouse->AddPosition(pointerIndex,x,y, false);
                                                        if (mouse->GetButton(pointerIndex))
                                                          {
                                                          mouse->GetButton(pointerIndex)->SetPressed(false);
                                                          mouse->GetButton(pointerIndex)->SetPressure(pressure);

                                                            if (mouse->GetButton(pointerIndex)->state!=INPBUTTON_STATE_UP)
                                                            {
                                                              mouse->GetButton(pointerIndex)->state=INPBUTTON_STATE_RELEASED;
                                                              //XDEBUG_PRINTCOLOR(3, __L("> ANDROID Motion %d x %f y %f p %f a %d RELEASED"), pointerID,x,y,pressure,action);

                                                            }
                                                            else
                                                            {
                                                              mouse->GetButton(pointerIndex)->state=INPBUTTON_STATE_UP;
                                                              //XDEBUG_PRINTCOLOR(2, __L("> ANDROID Motion %d x %f y %f p %f a %d UP"), pointerID,x,y,pressure,action);
                                                            }
                                                          }
                                                      }
                                                        break;
              case AMOTION_EVENT_ACTION_MOVE          :
              case AMOTION_EVENT_ACTION_DOWN          :
              case AMOTION_EVENT_ACTION_POINTER_DOWN  :
                                                      {
                                                          mouse->AddPosition(pointerIndex,x,y, true);
                                                          if (mouse->GetButton(pointerIndex))
                                                          {
                                                          mouse->GetButton(pointerIndex)->SetPressed(true);
                                                          mouse->GetButton(pointerIndex)->SetPressure(pressure);
                                                          if (mouse->GetButton(pointerIndex)->state==INPBUTTON_STATE_UP)
                                                          {
                                                              mouse->GetButton(pointerIndex)->state=INPBUTTON_STATE_PRESSED;
                                                              //XDEBUG_PRINTCOLOR(0, __L("> ANDROID Motion %d x %f y %f p %f a %d PRESSED"), pointerID,x,y,pressure,action);
                                                          }
                                                          else
                                                          {
                                                              mouse->GetButton(pointerIndex)->state=INPBUTTON_STATE_HOLD;
                                                                //XDEBUG_PRINTCOLOR(1, __L("> ANDROID Motion %d x %f y %f p %f a %d HOLD"), pointerID,x,y,pressure,action);
                                                          }


                                                          }
                                                      }
                                                        break;
              case AMOTION_EVENT_ACTION_CANCEL:
                                                        if (mouse->GetButton(pointerIndex))
                                                        mouse->GetButton(pointerIndex)->state=INPBUTTON_STATE_UP;
                                                        break;
            }
        }
        //XDEBUG_PRINTCOLOR(2, __L("Pointer %d [%f,%f]"),c,x,y);
    }


  return true;
}





/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnKeyboardEvent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/10/2013 23:28:51
//
//  @return       bool :
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::OnKeyboardEvent(AInputEvent* event)
{
  XDEBUG_PRINTCOLOR(2, __L("> ANDROID OnKeyboardEvent"));

  INPDEVICE* keyboard = androidmain.GetKeyboard();
  if(!keyboard) return false;

  INPBUTTON* button = keyboard->GetButtonByCode(AKeyEvent_getKeyCode(event));
  if(!button) return false;

  XDEBUG_PRINTCOLOR(2, __L("     Pressed %d"), button->GetID());

  button->SetMarkToRelase(false);
  //button->SetPressed((AKeyEvent_getMetaState(event) == AKEY_EVENT_ACTION_UP)?true:false);
  XDWORD keyEvent = AKeyEvent_getMetaState(event);

  switch (keyEvent)
  {
  case AKEY_EVENT_ACTION_UP:
      if (button->state != INPBUTTON_STATE_UP)
      {
          button->state = INPBUTTON_STATE_RELEASED;   XDEBUG_PRINTCOLOR(3, __L("> ANDROID Keyboard %d ID %d RELEASED"), keyEvent, button->GetID());
      }
      else
      {
          button->state = INPBUTTON_STATE_UP;         XDEBUG_PRINTCOLOR(3, __L("> ANDROID Keyboard %d ID %d UP"), keyEvent, button->GetID());
      }
      break;
  case AKEY_EVENT_ACTION_DOWN:
      if (button->state != INPBUTTON_STATE_HOLD)
      {
          button->state = INPBUTTON_STATE_PRESSED;        XDEBUG_PRINTCOLOR(3, __L("> ANDROID Keyboard %d ID %d PRESSED"), keyEvent, button->GetID());
      }
      else
      {
          button->state = INPBUTTON_STATE_HOLD;         XDEBUG_PRINTCOLOR(3, __L("> ANDROID Keyboard %d ID %d HOLD"), keyEvent, button->GetID());
      }
      break;
  }

  XDEBUG_PRINTCOLOR(2, __L("<"));

  return false;
}





/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnTrackballEvent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/10/2013 23:28:58
//
//  @return       bool :
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::OnTrackballEvent(AInputEvent* event)
{
  XDEBUG_PRINTCOLOR(2, __L("> ANDROID OnTrackballEvent"));


  XDEBUG_PRINTCOLOR(2, __L("<"));

  return true;
}







/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnActivate
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 0:54:58
//
//  @return       STATUS :
//  */
/*-----------------------------------------------------------------*/
STATUS ANDROIDMAIN::OnActivate()
{
  return STATUS_OK;
}




/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnDeactivate
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 0:55:49
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnDeactivate()
{

}




/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnStep
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:01:53
//
//  @return       STATUS :
//  */
/*-----------------------------------------------------------------*/
STATUS ANDROIDMAIN::OnStep()
{
  STATUS status = STATUS_OK;

  if(!androidmain.Update())
    status = STATUS_KO;

  return status;
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnStart
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:07:47
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnStart()
{
  XSTRING apkpath;
  XSTRING datapath;

  androidmain.GetPackageResourcePath(application, apkpath);
  androidmain.GetAbsolutePath(application, datapath);

  if(!androidmain.Ini(&apkpath, &datapath)) return;

  //if(androidmain.GetXApplication()->IsFullScreen())

      ANativeActivity_setWindowFlags(application->activity, AWINDOW_FLAG_FULLSCREEN, 0);

      /*
      GRPAPPLICATION* gap = dynamic_cast<GRPAPPLICATION*>(xapplication);
      if (gap)
          gap->OnStart();
          */

      GetXApplication()->OnStart();
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnResume
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:07:52
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnResume()
{
    /*
    GRPAPPLICATION* gap = dynamic_cast<GRPAPPLICATION*>(xapplication);
    if (gap)
        gap->OnResume();
        */
    GetXApplication()->OnPause();
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnPause
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:07:59
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnPause()
{
    /*
    GRPAPPLICATION* gap = dynamic_cast<GRPAPPLICATION*>(xapplication);
    if (gap)
        gap->OnPause();
        */
    GetXApplication()->OnPause();
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnStop
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:08:04
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnStop()
{
    /*
    GRPAPPLICATION* gap = dynamic_cast<GRPAPPLICATION*>(xapplication);
    if (gap)
        gap->OnStop();
        */
    GetXApplication()->OnStop();
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnDestroy
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/10/2013 11:50:36
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnDestroy()
{
    /*
    GRPAPPLICATION* gap = dynamic_cast<GRPAPPLICATION*>(xapplication);
    if (gap)
        gap->OnDestroy();
        */
    GetXApplication()->OnDestroy();
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnSaveState
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:08:15
//
//  @return       void :
//  @param        data :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnSaveState(void** data, size_t* size)
{

}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnConfigurationChanged
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:08:23
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnConfigurationChanged()
{
  if(!xapplication) return;



  // Big Ñapa. Waiting to Smart Solution.
  // if(xapplication->GetTimerGlobal()) xapplication->GetTimerGlobal()->SleepMilliSeconds(150);
  xsleep->MilliSeconds(200);

  int width  = 0;
  int height = 0;

  width  = ANativeWindow_getWidth(application->window);
  height = ANativeWindow_getHeight(application->window);

  if (!GetXApplication()->OnScreenConfigurationChanged((XDWORD)width,(XDWORD)height))
      return;

  GRPSCREEN* mainscreen = static_cast<GRPAPPLICATION*>(xapplication)->GetMainScreen();

  if (!mainscreen) return;
  if (!application->window)
  {
      XDEBUG_PRINTCOLOR(4, __L("Error!! Change screen  Width: %d, Height: %d"), mainscreen->GetWidth(), mainscreen->GetHeight());
      return;
  }
  mainscreen->UpdateSize(width, height);

  XDEBUG_PRINTCOLOR(2, __L("Change screen  Width: %d, Height: %d")  , width, height);

  if(androidmain.GetXApplication()->IsFullScreen())
    {
      AConfiguration *config = AConfiguration_new();
      AConfiguration_fromAssetManager(config, application->activity->assetManager);
      AConfiguration_setNavHidden(config, ACONFIGURATION_NAVHIDDEN_YES);
      AConfiguration_setKeysHidden(config, ACONFIGURATION_KEYSHIDDEN_YES);
    }
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnLowMemory
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:08:31
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnLowMemory()
{

}




/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnCreateWindow
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/10/2013 11:50:27
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnCreateWindow()
{

  XDEBUG_PRINTCOLOR(2,__L("ANDROIDMAIN::OnCreateWindow()"));

  GRPPROPERTIES         mainscreenproperties;
  ANativeWindow_Buffer  buffer;

  if(GetXApplication()->IniMainScreen(&mainscreenproperties, (void*)application->window))
    {

    //  if (!GetXApplication()->FirstUpdate())
    //      return;
      /*
      GRPSCREEN* screen = androidmain.GetXApplication()->GetMainScreen();
      if(screen)
        {
          #ifdef INP_ACTIVE
          if(!InputDevices(INPMANAGER::GetInstance(), (GRPANDROIDSCREEN*)screen, true)) return;
          #endif

          if(!GetXApplication()->CreateRenderer())  return;
          if(!GetXApplication()->FirstUpdate())     return;
        }
      */
      initialized = true;
    }
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnDestroyWindow
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:08:46
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnDestroyWindow()
{
  if(xapplication)
    {
      #ifdef INP_ACTIVE
      InputDevices(INPMANAGER::GetInstance(),NULL,false);
      #endif

      GetXApplication()->DeleteRenderer();
      GetXApplication()->EndMainScreen();
    }
}




/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnGainFocus
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:08:55
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnGainFocus()
{
    GRPAPPLICATION* gap = dynamic_cast<GRPAPPLICATION*>(xapplication);
    if (gap)
        gap->OnFocus();
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OnLostFocus
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 1:08:59
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDMAIN::OnLostFocus()
{
    GRPAPPLICATION* gap = dynamic_cast<GRPAPPLICATION*>(xapplication);
    if (gap)
        gap->OnLostFocus();
}





/*-------------------------------------------------------------------
//  ANDROIDMAIN::GetPackageResourcePath
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2013 13:31:46
//
//  @return       bool :
//  @param        app :
//  @param        path :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::GetPackageResourcePath(struct android_app* app, XSTRING& path)
{
  JNIEnv* env;
  JavaVM* vm     = app->activity->vm;
  bool    status = false;

  vm->AttachCurrentThread(&env, NULL);

  jclass    clazz   = env->GetObjectClass(app->activity->clazz);
  jmethodID methodj = env->GetMethodID(clazz, "getPackageResourcePath", "()Ljava/lang/String;");

  jstring   jstr    = (jstring)env->CallObjectMethod(app->activity->clazz, methodj);
  if(jstr != NULL)
    {
      const char* str = env->GetStringUTFChars(jstr, NULL);
      path =  str;
      env->ReleaseStringUTFChars(jstr, str);

      status = true;
    }

  vm->DetachCurrentThread();

  return status;
}




/*-------------------------------------------------------------------
//  ANDROIDMAIN::GetAbsolutePath
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2013 13:41:48
//
//  @return       bool :
//  @param        app :
//  @param        path :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::GetAbsolutePath(struct android_app* app, XSTRING& path)
{
  JNIEnv* env;
  JavaVM* vm     = app->activity->vm;
  bool    status = false;

  vm->AttachCurrentThread(&env, NULL);

  jclass    activityclass   = env->GetObjectClass(app->activity->clazz);
  jmethodID getfilesdir     = env->GetMethodID(activityclass, "getFilesDir", "()Ljava/io/File;");
  jobject   file            = env->CallObjectMethod(app->activity->clazz, getfilesdir);
  jclass    fileclass       = env->FindClass("java/io/File");
  jmethodID getabsolutepath = env->GetMethodID(fileclass, "getAbsolutePath", "()Ljava/lang/String;");

  jstring jpath = (jstring)env->CallObjectMethod(file, getabsolutepath);
  if(jpath != NULL)
    {
      const char* str = env->GetStringUTFChars(jpath, NULL);
      path =  str;
      env->ReleaseStringUTFChars(jpath, str);
      status = true;
    }

  vm->DetachCurrentThread();

  return status;
}


/*-------------------------------------------------------------------
//   ANDROIDMAIN::GetDiP
*/
/**
//
//
//
//  @author   Imanol Celaya Ruiz de Alegria
//  @version

//  @return   bool :
//
//  @param    struct :
//
*//*-----------------------------------------------------------------*/
bool ANDROIDMAIN::GetDPI(struct android_app* app)
{
    #ifdef GRP_ACTIVE
  grpfactory  = new GRPANDROIDFACTORY();
  if(!grpfactory) return false;

  // get the screen dpi  //DIEGO : supongo que meter esto en una función consumía tinta de monitor, no? de hecho , no hay una clase para manejar el JNI????
  JNIEnv* env = NULL;

  jint      res     = this->application->activity->vm->AttachCurrentThread(&env, NULL);

  jclass    clazz   = env->GetObjectClass(this->application->activity->clazz);
  jfieldID  fid     = env->GetStaticFieldID(clazz, "dpi", "I");
  jint      i       = env->GetStaticIntField(clazz, fid);

  this->application->activity->vm->DetachCurrentThread();

                    XDEBUG_PRINTCOLOR(2, __L("jni %lld"), timer->GetMeasureMilliSeconds());
  // assign it

  grpfactory->dpi = i;

  #endif
}


//-------------------------------------------------------------------
//  ANDROIDMAIN::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2003 12:09:01
//
//  @return
//  */
//-------------------------------------------------------------------
void ANDROIDMAIN::Clean()
{
  xsystem             = NULL;

  #ifdef INP_ACTIVE
  keyboard            = NULL;
  mouse               = NULL;
  #endif

  application         = NULL;
  initialized         = false;

  xapplication        = NULL;

  timer               = NULL;
}





/*-------------------------------------------------------------------
//  ANDROIDMAIN::OverturnAssetsToExternalLocation
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2012 13:07:28
//
//  @return       bool :

//  @param        origin :
//  @param        target :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::OverturnAssetsToExternalLocation( XPATH& origin, XPATH& target)
{
  XFILEUNZIP* unzip = new XFILEUNZIP();
  if(!unzip) return false;

  XSTRING  assetsstring;
  bool     status  = false;

  assetsstring = __L("assets");

  XDIR* xdir = xfactory->Create_Dir();
  if(xdir)
    {
      if(unzip->Open(origin))
        {
          XFILECMPZIP* filecmp;
          int          index = 0;

          status = true;

          do{
              filecmp = unzip->GotoFile(index);
              if(filecmp)
                {
                  XPATH targetpath;

                  targetpath  = target;
                  targetpath += filecmp->GetName();

                  targetpath.SetOnlyPath();

                  int position=targetpath.Find(assetsstring,true);
                  if(position!=-1)
                    {
                      XPATH targetfile;
                      XPATH namefile;

                      namefile = filecmp->GetName();
                      namefile.SetOnlyNamefileExt();

                      targetpath.DeleteCharacters(position,assetsstring.GetSize()+1);

                      targetfile  = targetpath;
                      targetfile += namefile.Get();

                      XFILE* xfile = xfactory->Create_File();
                      if(xfile)
                        {
                          if(!xfile->Exist(targetfile.Get()))
                            {
                              xdir->Make(targetpath.Get());
                              status = unzip->DecompressFile(filecmp,targetpath,namefile.Get());
                            }

                          xfactory->Delete_File(xfile);
                        }
                    }

                  delete filecmp;

                  index++;

                } else break;

            } while(status);

          unzip->Close();

        }

      xfactory->Delete_Dir(xdir);
    }

  delete unzip;

  return status;
}




/*-------------------------------------------------------------------
//  ANDROIDMAIN::OverturnAssetsToExternalLocation
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2012 13:07:48
//
//  @return       bool :

//  @param        origin :
//  @param        target :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::OverturnAssetsToExternalLocation(XPATH& origin,XCHAR* target)
{
  if(!target) return false;

  XPATH _target;

  _target = target;

  return OverturnAssetsToExternalLocation(origin,_target);
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OverturnAssetsToExternalLocation
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2012 13:07:56
//
//  @return       bool :

//  @param        origin :
//  @param        target :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::OverturnAssetsToExternalLocation(XCHAR* origin,XPATH& target)
{
  if(!origin) return false;

  XPATH _origin;

  _origin = origin;

  return OverturnAssetsToExternalLocation(_origin,target);
}



/*-------------------------------------------------------------------
//  ANDROIDMAIN::OverturnAssetsToExternalLocation
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2012 13:08:03
//
//  @return       bool :

//  @param        origin :
//  @param        target :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::OverturnAssetsToExternalLocation(XCHAR* origin,XCHAR* target)
{
  if(!origin) return false;
  if(!target) return false;

  XPATH _origin;
  XPATH _target;

  _origin = origin;
  _target = target;

  return OverturnAssetsToExternalLocation(_origin,_target);
}




#ifdef INP_ACTIVE
/*-------------------------------------------------------------------
//  ANDROIDMAIN::InputDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/03/2012 12:06:22
//
//  @return       bool :
//  @param        inpmanager :
//  @param        grpscreen :
//  @param        create :
*/
/*-----------------------------------------------------------------*/
bool ANDROIDMAIN::InputDevices(INPMANAGER* inpmanager, GRPANDROIDSCREEN* screen, bool create)
{

  /*
  if(create)
    {
      keyboard =  new INPANDROIDDEVICEKEYBOARD();
      if(!keyboard) return false;
      inpmanager->AddDevice(keyboard);

      mouse =  new INPANDROIDDEVICEMOUSE( screen);
      if(!mouse) return false;
      inpmanager->AddDevice(mouse);
    }
   else
    {
      inpmanager->DelDevice(keyboard);
      delete keyboard;
      keyboard = NULL;

      inpmanager->DelDevice(mouse);
      delete mouse;
      mouse = NULL;
    }

    */

  if(create)
  {
  mouse     = INPANDROIDFACTORY::GetInstance()->CreateDevice(__L("mouse"),screen);
  keyboard  = INPANDROIDFACTORY::GetInstance()->CreateDevice(__L("keyboard"),screen);


  if(keyboard)    INPMANAGER::GetInstance()->AddDevice(keyboard);
  if(mouse)       INPMANAGER::GetInstance()->AddDevice(mouse);
  }
  else
  {
    delete(keyboard);
    delete(mouse);
  }
  return true;
}

#endif



/*-------------------------------------------------------------------
//  JNI_OnLoad
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/04/2016 16:11:15
//
//  @return       JNIEXPORT :
//
//  @param        InJavaVM :
//  @param        InReserved :
*/
/*-----------------------------------------------------------------*/
JNIEXPORT jint JNI_OnLoad(JavaVM* InJavaVM, void* InReserved)
{
  return JNI_VERSION_1_6;
}


/*-------------------------------------------------------------------
//  android_main
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 0:42:12
//
//  @return       void :
//  @param        application :
*/
/*-----------------------------------------------------------------*/
void android_main(android_app* application)
{
  ANDROIDJNI::InitializeJNI(application, JNI_VERSION_1_6);

  ANDROIDNATIVEACTIVITY_EVENTLOOP eventloop(application);

  androidmain.SetAndroidApplication(application);


  //XDEBUG_ADDTARGET(XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM);

  XDEBUG_PRINTCOLOR(4, __L("ANDROID Init APP"));


  eventloop.Run(&androidmain, &androidmain);

  androidmain.End();

  XDEBUG_PRINTCOLOR(4, __L("ANDROID End APP"));
}
