
/*------------------------------------------------------------------------------------------
//  GRPLINUXSCREENX11GLES.CPP
//
//  Opens an OpenGL ES context on X11 using EGL
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 02/02/2016 16:14:20
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPLINUXScreenX11GLES.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11GLES::GRPLINUXSCREENX11GLES
*/
/**
//
//  Class Constructor GRPLINUXSCREENX11GLES
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:04:14
//
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENX11GLES::GRPLINUXSCREENX11GLES() : GRPLINUXSCREENX11()
{
  type = GRPSCREENTYPE_LINUX_X11_GLES;
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11GLES::~GRPLINUXSCREENX11GLES
*/
/**
//
//   Class Destructor GRPLINUXSCREENX11GLES
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:04:20
//
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENX11GLES::~GRPLINUXSCREENX11GLES()
{
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11GLES::Create
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:04:26
//
//  @return       bool :
//
//  @param        handle :
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11GLES::Create(void* handle)
{
  static const EGLint           attribute_list[]      = { EGL_RED_SIZE      , 8,
                                                          EGL_GREEN_SIZE    , 8,
                                                          EGL_BLUE_SIZE     , 8,
                                                          EGL_ALPHA_SIZE    , 8,
                                                          EGL_SURFACE_TYPE  , EGL_WINDOW_BIT,
                                                          EGL_DEPTH_SIZE    , 16            ,
                                                          EGL_SAMPLE_BUFFERS, (this->multisampling != 0) ? 1 : 0,
                                                          EGL_SAMPLES, this->multisampling,
                                                          EGL_NONE
                                                        };

  static const EGLint           context_attributes[] =  { EGL_CONTEXT_CLIENT_VERSION, 2,
                                                          EGL_NONE
                                                        };

  EGLint                        num_config;
  EGLConfig                     config;
  EGLint                        vid;
  EGLBoolean                    result;

  setenv("EGL_PLATFORM", "X11", 0);

  if(!OpenDisplay()) return false;

  int major=0,minor=0;

  this->display = eglGetDisplay(GRPLINUXSCREENX11::display);

  // initialize the EGL display connection
  result = eglInitialize(display, &major, &minor);
  XDEBUG_PRINTCOLOR(0,__L("EGL version %d.%d"),major,minor);



  // get an appropriate EGL frame buffer configuration
  result = eglChooseConfig(this->display, attribute_list, &config, 1, &num_config);


  // now obtain an X11 visual from the EGL config

  eglGetConfigAttrib(this->display, config, EGL_NATIVE_VISUAL_ID, &vid);

  XVisualInfo visTemplate;
  visTemplate.visualid = vid;
  int num_visuals;

  this->vi = XGetVisualInfo(GRPLINUXSCREENX11::display, VisualIDMask, &visTemplate, &num_visuals);
  //ifnumvisuals is 0 it means we couldn't  find a valid one

  if(!CreateX11Window()) return false;

  // bind the OpenGL API to the EGL
  result = eglBindAPI(EGL_OPENGL_ES_API);
  if(result == EGL_FALSE)
    {
      XDEBUG_PRINTCOLOR(0,__L("GRPLINUXSCREENX11GLES::Create : unable to bind api"));
      return false;
    }


  // create an EGL rendering context
  context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
  if(context == EGL_NO_CONTEXT)
    {
      XDEBUG_PRINTCOLOR(0,__L("GRPLINUXSCREENX11GLES::Create : unable to create rendering context"));
      return false;
    }

  surface = eglCreateWindowSurface(this->display, config, window, NULL);

  if (surface==EGL_NO_SURFACE)
    {
      XDEBUG_PRINTCOLOR(0,__L("GRPLINUXSCREENX11GLES::Create : unable to create surface"));
      return false;
    }

  eglMakeCurrent(display, surface, surface, context);

  return true;
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11GLES::Update
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:04:33
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11GLES::Update()
{
  eglSwapBuffers(display, surface);
  return true;
}

