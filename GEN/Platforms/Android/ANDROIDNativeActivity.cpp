/*------------------------------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY.CPP
//
//  ANDROID Native Activity class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 28/09/2013 17:06:15
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "ANDROIDNativeActivity.h"
#include "XDebug.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::ANDROIDNATIVEACTIVITY_EVENTLOOP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/09/2013 17:09:55
//
//  @return
//  @param        app :
*/
/*-----------------------------------------------------------------*/
ANDROIDNATIVEACTIVITY_EVENTLOOP::ANDROIDNATIVEACTIVITY_EVENTLOOP(android_app* application)
{
  Clean();

  this->application = application;

  application->userData     = this;
  application->onAppCmd     = CallbackEvent;
  application->onInputEvent = CallbackInput;
}




/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::~ANDROIDNATIVEACTIVITY_EVENTLOOP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/09/2013 17:10:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
ANDROIDNATIVEACTIVITY_EVENTLOOP::~ANDROIDNATIVEACTIVITY_EVENTLOOP()
{
  Clean();
}





/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::Run
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/10/2013 23:19:18
//
//  @return       void :
//  @param        activityhandler :
//  @param        inputhandler :
*/
/*-----------------------------------------------------------------*/
void ANDROIDNATIVEACTIVITY_EVENTLOOP::Run(ANDROIDNATIVEACTIVITY_HANDLER* activityhandler, ANDROIDNATIVEINPUT_HANDLER* inputhandler)
{
  int32_t               result;
  int32_t               events;
  android_poll_source*  source;

  app_dummy();

  this->activityhandler = activityhandler;
  this->inputhandler    = inputhandler;

  while (true)
    {
      // Event processing loop.
      while((result = ALooper_pollOnce(enabled?0:-1, NULL, &events, (void**)&source)) >= 0)
        {
          // An event has to be processed.
          if(source != NULL) source->process(application, source);

          // Application is getting destroyed.
          if(application->destroyRequested) return;
        }

      if((enabled) && (!quit))
        {
          if(activityhandler->OnStep() != STATUS_OK)
            {
              quit = true;
              ANativeActivity_finish(application->activity);
            }
        }
    }
}





/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::Activate
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 0:21:09
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDNATIVEACTIVITY_EVENTLOOP::Activate()
{
  // Enables activity only if a window is available.
  if((!enabled) && (application->window != NULL))
    {
      quit    = false;
      enabled = true;

      if(activityhandler->OnActivate() != STATUS_OK)
        {
          quit = true;

          Deactivate();

          ANativeActivity_finish(application->activity);
        }
    }
}




/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::Deactivate
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 0:22:19
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDNATIVEACTIVITY_EVENTLOOP::Deactivate()
{
  if(enabled)
    {
      activityhandler->OnDeactivate();
      enabled = false;
    }
}





/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::ProcessInputEvent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/10/2013 19:56:31
//
//  @return       int32_t :
//  @param        event :
//  @param        application :
*/
/*-----------------------------------------------------------------*/
int32_t ANDROIDNATIVEACTIVITY_EVENTLOOP::ProcessInputEvent(AInputEvent* event, android_app* application)
{
  int32_t eventtype = AInputEvent_getType(event);

  this->application = application;

  switch(eventtype)
    {
      case AINPUT_EVENT_TYPE_MOTION : switch (AInputEvent_getSource(event))
                                        {
                                          case AINPUT_SOURCE_TOUCHSCREEN  : return inputhandler->OnTouchEvent(event);       break;
                                          case AINPUT_SOURCE_TRACKBALL    : return inputhandler->OnTrackballEvent(event);   break;
                                        }
                                      break;

      case AINPUT_EVENT_TYPE_KEY    : return inputhandler->OnKeyboardEvent(event);  break;
    }

  return 0;
}



/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::ProcessApplicationEvent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/10/2013 19:56:17
//
//  @return       void :
//  @param        command :
//  @param        application :
*/
/*-----------------------------------------------------------------*/
void ANDROIDNATIVEACTIVITY_EVENTLOOP::ProcessApplicationEvent(int32_t command, android_app* application)
{
  this->application = application;

  switch(command)
    {
      case APP_CMD_CONFIG_CHANGED   : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_CONFIG_CHANGED"));
                                      activityhandler->OnConfigurationChanged();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_INIT_WINDOW      : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_INIT_WINDOW"));
                                      activityhandler->OnCreateWindow();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_DESTROY          : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_DESTROY"));
                                      activityhandler->OnDestroy();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_GAINED_FOCUS     : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_GAINED_FOCUS"));
                                      Activate();
                                      activityhandler->OnGainFocus();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_LOST_FOCUS       : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_LOST_FOCUS"));
                                      activityhandler->OnLostFocus();
                                      Deactivate();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_LOW_MEMORY       : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_LOW_MEMORY"));
                                      activityhandler->OnLowMemory();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_PAUSE            : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_PAUSE"));
                                      activityhandler->OnPause();
                                      Deactivate();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_RESUME           : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_RESUME"));
                                      Activate();
                                      activityhandler->OnResume();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_SAVE_STATE       : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_SAVE_STATE"));
                                      activityhandler->OnSaveState(&application->savedState, &application->savedStateSize);
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_START            : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_START"));
                                      activityhandler->OnStart();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_STOP             : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_STOP"));
                                      activityhandler->OnStop();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

      case APP_CMD_TERM_WINDOW      : XDEBUG_PRINTCOLOR(2, __L("> ANDROID APP_CMD_TERM_WINDOW"));
                                      activityhandler->OnDestroyWindow();
                                      Deactivate();
                                      XDEBUG_PRINTCOLOR(2, __L("<"));
                                      break;

                    default         : break;
    }
}




/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/09/2013 17:54:32
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void ANDROIDNATIVEACTIVITY_EVENTLOOP::Clean()
{
  enabled       = false;
  quit          = false;

  application     = NULL;
  activityhandler = NULL;
  inputhandler    = NULL;
}




/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::CallbackInput
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/10/2013 23:08:03
//
//  @return       int32_t :
//  @param        application :
//  @param        event :
*/
/*-----------------------------------------------------------------*/
int32_t ANDROIDNATIVEACTIVITY_EVENTLOOP::CallbackInput(android_app* application, AInputEvent* event)
{
  ANDROIDNATIVEACTIVITY_EVENTLOOP& eventloop = *(ANDROIDNATIVEACTIVITY_EVENTLOOP*) application->userData;

  return eventloop.ProcessInputEvent(event, application);
}



/*-------------------------------------------------------------------
//  ANDROIDNATIVEACTIVITY_EVENTLOOP::CallbackEvent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2013 0:25:00
//
//  @return       void :
//  @param        application :
//  @param        command :
*/
/*-----------------------------------------------------------------*/
void ANDROIDNATIVEACTIVITY_EVENTLOOP::CallbackEvent(android_app* application, int32_t command)
{
  ANDROIDNATIVEACTIVITY_EVENTLOOP& eventloop = *(ANDROIDNATIVEACTIVITY_EVENTLOOP*)application->userData;

  eventloop.ProcessApplicationEvent(command, application);
}
