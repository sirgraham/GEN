
/*------------------------------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFERGLES.CPP
//
//  GLES on framebuffer
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 10/02/2016 11:10:52
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPLINUXScreenFrameBufferGLES.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFERGLES::GRPLINUXSCREENFRAMEBUFFERGLES
*/
/**
//
//  Class Constructor GRPLINUXSCREENFRAMEBUFFERGLES
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:03:14
//
//  @param         :
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENFRAMEBUFFERGLES::GRPLINUXSCREENFRAMEBUFFERGLES( ) : GRPLINUXSCREENFRAMEBUFFER()
{
  type = GRPSCREENTYPE_LINUX_FRAMEBUFFER_GLES;
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFERGLES::~GRPLINUXSCREENFRAMEBUFFERGLES
*/
/**
//
//   Class Destructor GRPLINUXSCREENFRAMEBUFFERGLES
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:03:18
//
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENFRAMEBUFFERGLES::~GRPLINUXSCREENFRAMEBUFFERGLES()
{
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFERGLES::Create
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:03:25
//
//  @return       bool :
//
//  @param        handle :
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFERGLES::Create(void* handle)
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

  const EGLNativeWindowType native_win = (EGLNativeWindowType) NULL;
  EGLint major, minor;
  EGLConfig                     config;
  EGLint                        num_config;
  EGLBoolean                    result;

  // here to make MESA happy it seems
  // (taken from one of their official demos with the comment "make Mesa/EGL happy")
  setenv("EGL_PLATFORM", "fbdev", 0);

  //this->display = eglGetDisplay((EGLNativeDisplayType) handlefb);
  this->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  result = eglInitialize(this->display, &major, &minor);
  XDEBUG_PRINTCOLOR(0,__L("EGL version %d.%d"),major,minor);


  struct fbdev_window nwin;

  nwin.width = GetWidth();
  nwin.height = GetHeight(),

  result = eglChooseConfig(this->display, attribute_list, &config, 1, &num_config);
  this->context = eglCreateContext(this->display, config, EGL_NO_CONTEXT, context_attributes);
  //this->surface = eglCreateWindowSurface(this->display, config, native_win, NULL);
  this->surface = eglCreateWindowSurface(this->display, config, &nwin, NULL);
  eglMakeCurrent(this->display, this->surface, this->surface, this->context);

  return true;
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFERGLES::Update
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:03:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFERGLES::Update()
{
  //eglMakeCurrent(this->display, this->surface, this->surface, this->context);
  eglSwapBuffers(this->display, this->surface);
  return true;
}




/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFERGLES::Delete
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/02/2016 17:03:40
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFERGLES::Delete()
{

}
