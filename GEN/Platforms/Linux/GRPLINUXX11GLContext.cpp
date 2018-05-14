
/*------------------------------------------------------------------------------------------
//  GRPLINUXX11GLCONTEXT.CPP
//
//  Linux X11 GL Context
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 08/04/2016 12:49:26
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPLINUXX11GLContext.h"
#include "GRPLINUXScreenX11.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
GRPLINUXX11GLCONTEXT::GRPLINUXX11GLCONTEXT()
{
}

GRPLINUXX11GLCONTEXT::~GRPLINUXX11GLCONTEXT()
{
}


bool GRPLINUXX11GLCONTEXT::Create(void* handle)
{
  GRPLINUXSCREENX11* linuxscreen = (GRPLINUXSCREENX11*)screen;

  XDEBUG_PRINTCOLOR(2, __L("Opening Display"));
  if(!linuxscreen->OpenDisplay()) return false;

  //Window root = DefaultRootWindow(this->display);

  // --- Find a MSAA FBConfig ---
  static const int Visual_attribs[] =
    {
      GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , True,
      GLX_SAMPLE_BUFFERS  , (this->multisampling != 0) ? 1 : 0,     // <-- MSAA
      GLX_SAMPLES         , this->multisampling,                    // <-- MSAA
      None
    };

  int attribs [ 100 ] ;
  memcpy( attribs, Visual_attribs, sizeof( Visual_attribs ) );

  GLXFBConfig fbconfig = 0;
  int         fbcount;
  GLXFBConfig *fbc = glXChooseFBConfig( linuxscreen->GetDisplay(), 0, attribs, &fbcount );
  if ( fbc )
    {
      if ( fbcount >= 1 )
        {
          fbconfig = fbc[0];
        }
      XFree( fbc );
    }

  if ( !fbconfig )
    {
      XDEBUG_PRINTCOLOR(4,__L("\n\tFailed to get MSAA GLXFBConfigr\n\n"));
      return false;
    }

  // --- Get its VisualInfo ---
  //XVisualInfo *visinfo = glXGetVisualFromFBConfig( display, fbconfig );
  *linuxscreen->GetVisualInfo() = glXGetVisualFromFBConfig( linuxscreen->GetDisplay(), fbconfig );
  if ( !*linuxscreen->GetVisualInfo() )
  {
    XDEBUG_PRINTCOLOR(4,__L("\n\tFailed to get XVisualInfo\n\n"));
    return false;
  }

  //vi = glXChooseVisual(dpy, 0, att);
  linuxscreen->CreateX11Window();


  glc = glXCreateContext(linuxscreen->GetDisplay(), *linuxscreen->GetVisualInfo(), NULL, GL_TRUE);
  bool worked = glXMakeCurrent(linuxscreen->GetDisplay(), *linuxscreen->GetWindow(), glc);

  glewExperimental = true;
  if(GLEW_OK != glewInit())
    {
      return false;
    }

  openglctrl.Ini();

  const GLubyte* cv = glGetString(GL_VERSION);
  XSTRING v((const char*)cv);


  XDEBUG_PRINTCOLOR(2, __L("OpenGL Version: %s"), v.Get());

  return true;
}

bool GRPLINUXX11GLCONTEXT::Destroy()
{
  GRPLINUXSCREENX11* linuxscreen = (GRPLINUXSCREENX11*)screen;
  glXDestroyContext(linuxscreen->GetDisplay(), glc);
  return true;
}

bool GRPLINUXX11GLCONTEXT::IsLost()
{
  return glXGetCurrentContext() == NULL;
}


bool GRPLINUXX11GLCONTEXT::Update()
{
  GRPLINUXSCREENX11* linuxscreen = (GRPLINUXSCREENX11*)screen;
  glXSwapBuffers(linuxscreen->GetDisplay(), *linuxscreen->GetWindow());

  return true;
}
