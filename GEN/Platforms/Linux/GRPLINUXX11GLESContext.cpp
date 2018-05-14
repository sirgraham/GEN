
/*------------------------------------------------------------------------------------------
//  GRPLINUXX11GLESCONTEXT.CPP
//
//  LINUX X11 GLES Context
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 08/04/2016 17:28:03
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPLINUXX11GLESContext.h"
#include "GRPLINUXScreenX11.h"

#include <stdlib.h>

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/






/*-------------------------------------------------------------------
//  GRPLINUXX11GLESCONTEXT::GRPLINUXX11GLESCONTEXT
*/
/**
//
//  Class Constructor GRPLINUXX11GLESCONTEXT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 17:38:50
//
*/
/*-----------------------------------------------------------------*/
GRPLINUXX11GLESCONTEXT::GRPLINUXX11GLESCONTEXT() : GRPCONTEXT()
{
}







/*-------------------------------------------------------------------
//  GRPLINUXX11GLESCONTEXT::~GRPLINUXX11GLESCONTEXT
*/
/**
//
//   Class Destructor GRPLINUXX11GLESCONTEXT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 17:38:43
//
*/
/*-----------------------------------------------------------------*/
GRPLINUXX11GLESCONTEXT::~GRPLINUXX11GLESCONTEXT()
{



}







/*-------------------------------------------------------------------
//  GRPLINUXX11GLESCONTEXT::Create
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 17:38:37
//
//  @return       bool :
//
//  @param        handle :
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXX11GLESCONTEXT::Create(void* handle)
{
  GRPLINUXSCREENX11* linuxscreen = (GRPLINUXSCREENX11*)this->screen;


  EGLBoolean                    result;
  EGLint                        num_config;

  static const EGLint           attribute_list[]      = { EGL_RED_SIZE        , 8                                   ,
                                                          EGL_GREEN_SIZE      , 8                                   ,
                                                          EGL_BLUE_SIZE       , 8                                   ,
                                                          EGL_ALPHA_SIZE      , 8                                   ,
                                                          EGL_SURFACE_TYPE    , EGL_WINDOW_BIT                      ,
                                                          EGL_DEPTH_SIZE      , 16                                  ,
                                                          EGL_SAMPLE_BUFFERS  , (this->multisampling != 0) ? 1 : 0  ,
                                                          EGL_SAMPLES         , this->multisampling                 ,
                                                          EGL_NONE
                                                        };

  static const EGLint           context_attributes[] =  { EGL_CONTEXT_CLIENT_VERSION, 2,
                                                          EGL_NONE
                                                        };

  setenv("EGL_PLATFORM", "X11", 0);

  if(!linuxscreen->OpenDisplay()) return false;

  // get an EGL display connection
  display = eglGetDisplay((EGLNativeDisplayType)linuxscreen->GetDisplay());

  // initialize the EGL display connection
  int major=0,minor=0;
  result = eglInitialize(display, &major, &minor);
  XDEBUG_PRINTCOLOR(0,__L("EGL version %d.%d"),major,minor);

  // get an appropriate EGL frame buffer configuration
  result = eglChooseConfig(this->display, attribute_list, &config, 1, &num_config);


  // now obtain an X11 visual from the EGL config

  GRPLINUXSCREENX11* mainscreen = (GRPLINUXSCREENX11*)(this->screen);

  EGLint vid;
  eglGetConfigAttrib(this->display, config, EGL_NATIVE_VISUAL_ID, &vid);

  XVisualInfo visTemplate;
  visTemplate.visualid = vid;
  int num_visuals;

  *(mainscreen->GetVisualInfo()) = XGetVisualInfo(linuxscreen->GetDisplay(), VisualIDMask, &visTemplate, &num_visuals);
  //ifnumvisuals is 0 it means we couldn't  find a valid one

  if(!mainscreen->CreateX11Window()) return false;

  // bind the OpenGL API to the EGL
  result = eglBindAPI(EGL_OPENGL_ES_API);
  if(result == EGL_FALSE)
    {
      XDEBUG_PRINTCOLOR(0,__L("GRPLINUXX11GLESCONTEXT::Create : unable to bind api"));
      return false;
    }


  // create an EGL rendering context
  context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
  if(context == EGL_NO_CONTEXT)
    {
      XDEBUG_PRINTCOLOR(0,__L("GRPLINUXX11GLESCONTEXT::Create : unable to create rendering context"));
      return false;
    }

  surface = eglCreateWindowSurface(this->display, config, (EGLNativeWindowType)*(mainscreen->GetWindow()), NULL);

  if (surface==EGL_NO_SURFACE)
    {
      XDEBUG_PRINTCOLOR(0,__L("GRPLINUXX11GLESCONTEXT::Create : unable to create surface"));
      return false;
    }

  eglMakeCurrent(display, surface, surface, context);

  return true;
}






/*-------------------------------------------------------------------
//  GRPLINUXX11GLESCONTEXT::Destroy
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 17:38:28
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXX11GLESCONTEXT::Destroy()
{
  eglDestroyContext(display, this->context);

  // Release OpenGL resources
  eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
  eglDestroySurface(display, surface);
  eglDestroyContext(display, context);
  eglTerminate(display);

  return true;
}






/*-------------------------------------------------------------------
//  GRPLINUXX11GLESCONTEXT::IsLost
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 17:38:21
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXX11GLESCONTEXT::IsLost()
{
  return eglGetCurrentContext() == EGL_NO_CONTEXT;
}






/*-------------------------------------------------------------------
//  GRPLINUXX11GLESCONTEXT::Update
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 17:38:14
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXX11GLESCONTEXT::Update()
{
  eglSwapBuffers(display, surface);
  return true;
}


