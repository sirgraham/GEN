
/*------------------------------------------------------------------------------------------
//  GRPLINUXSCREENX11.CPP
//
//  LINUX graphics screen class
//
//  Author            : Diego Martinez
//  Date Of Creation  : 13/03/2014 13:23:05
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/


#include "XDebug.h"
#include "GRPScreen.h"

#include "GRPLinuxScreenX11.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11::GRPLINUXSCREENX11
*/
/**
//
//  Class Constructor GRPLINUXSCREENX11
//
//  @author       Abraham J. Velez
//  @version      21/05/2014 15:58:18
//


*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENX11::GRPLINUXSCREENX11() : GRPSCREEN()
{
  Clean();

  type = GRPSCREENTYPE_LINUX_X11_GL;

  display = XOpenDisplay(NULL);
  if(display)
    {
      int width  = DisplayWidth  (display, DefaultScreen (display));
      int height = DisplayHeight (display, DefaultScreen (display));
      int depth  = DefaultDepth  (display, DefaultScreen (display));

      SetWidth(width);
      SetHeight(height);

      switch(depth)
        {
          case   8 : SetMode(GRPPROPERTYMODE_08_INDEX);     break;
          case  16 : SetMode(GRPPROPERTYMODE_16_RGB_565);   break;
          case  24 : SetMode(GRPPROPERTYMODE_24_BGR_888);   break;
          case  32 : SetMode(GRPPROPERTYMODE_32_BGRA_8888); break;
        }
    }
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11::~GRPLINUXSCREENX11
*/
/**
//
//  Class Destructor GRPLINUXSCREENX11
//
//  @author       Abraham J. Velez
//  @version      27/03/2014 13:34:26
//
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENX11::~GRPLINUXSCREENX11()
{
  Delete();

  if(display) XCloseDisplay(display);

  Clean();
}



/*-------------------------------------------------------------------
//   GRPLINUXSCREENX11::Delete
*/
/**
//  Calls system destroyer
//
//  @author       Diego Martinez
//  @version      13/03/2014 13:31:56
//
//  @return       bool :
//
/*-----------------------------------------------------------------*/
bool  GRPLINUXSCREENX11::Delete()
{
  if(!display) return false;
  if(!window)  return false;

  XDestroyWindow(display, window);

  isactive = false;

  return true;
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11::Resize
*/
/**
//
//  resizes an X11 window
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      18/11/2015 11:38:43
//
//  @return       bool :
//
//  @param        width :
//  @param        height :
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Resize(int width, int height)
{
  XResizeWindow(display, window, width, height);

  return UpdateSize(width, height);
}


/*-------------------------------------------------------------------
//   GRPLINUXSCREENX11::GetDisplay
*/
/**
//  Returns current display
//
//  @author       Diego Martinez
//  @version      13/03/2014 13:31:56
//
//  @return       Display* : current display
//
/*-----------------------------------------------------------------*/
Display* GRPLINUXSCREENX11::GetDisplay()
{
    return display;
}



/*-------------------------------------------------------------------
//   GRPLINUXSCREENX11::GetWindow
*/
/**
//  Returns window handle
//
//  @author       Diego Martinez
//  @version      13/03/2014 13:37:04
//
//  @return       Window* : current window handle
//
/*-----------------------------------------------------------------*/
Window* GRPLINUXSCREENX11::GetWindow()
{
  return &window;
}





/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11::Create
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 17:24:15
//
//  @return       bool :
//
//  @param        handle :
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11::Create(void* handle)
{
  OpenDisplay();
  CreateX11Window();
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11::GetVisualInfo
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 13:28:17
//
//  @return       XVisualInfo* :
//
*/
/*-----------------------------------------------------------------*/
XVisualInfo** GRPLINUXSCREENX11::GetVisualInfo()
{
  return &vi;
}







/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11::GetRootWindow
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/04/2016 13:28:24
//
//  @return       Window* :
//
*/
/*-----------------------------------------------------------------*/
Window* GRPLINUXSCREENX11::GetRootWindow()
{
  return &root;
}



/*-------------------------------------------------------------------
//    GRPLINUXSCREENX11::HasFocus
*/
/**
//  returns if we have focus
//
//  @author       Diego Martinez
//  @version      13/03/2014 13:37:21
//
//  @return       bool :
//
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11::HasFocus()
{
  Window focus;
  int    status = 0;

  XGetInputFocus(display, &focus, &status);

  if(focus ==  window) return true;

  return false;
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11::Clean
*/
/**
//  Clears the window data
//
//  @author       Diego Martinez
//  @version      13/03/2014 13:37:31
//
//
/*-----------------------------------------------------------------*/
void GRPLINUXSCREENX11::Clean()
{
  display   = NULL;
  window    = 0;
  vi        = NULL;
  isdesktop = false;
}


/*-------------------------------------------------------------------
//   GRPLINUXSCREENX11::OpenDisplay
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  07/11/2017 13:58:10
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11::OpenDisplay()
{
  display = XOpenDisplay(NULL);

  if(display == NULL)
    {
      XDEBUG_PRINTCOLOR(4,__L("\n\tCannot connect to X server\n\n"));
      return false;
    }

  root = DefaultRootWindow(display);
  return true;
}


/*-------------------------------------------------------------------
//   GRPLINUXSCREENX11::CreateX11Window
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  07/11/2017 13:58:05
//  @return
bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11::CreateX11Window()
{
  //GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
  Colormap                cmap;
  XSetWindowAttributes    swa;
  Window                  win;
  XWindowAttributes       gwa;
  XEvent                  xev;

  //if(!OpenDisplay()) return false;

  int wndwidth  = DisplayWidth (display, DefaultScreen (display));
  int wndheight = DisplayHeight (display, DefaultScreen (display));
  int x = (wndwidth  - width)  / 2;
  int y = (wndheight - height) / 2;

  //win = XCreateWindow(dpy, root, x, y, width, height, 0, vi->depth, InputOutput, vi->visual,  CWBackPixel | CWBorderPixel | CWOverrideRedirect | CWColormap | CWEventMask, &swa);


  if(IsFullScreen())
    {
      XDEBUG_PRINTCOLOR(0,__L("Start Video Modes query"));
      // get the video modes
      XF86VidModeModeInfo **modes;
      int modeNum = 0;
      if (!XF86VidModeGetAllModeLines(display, DefaultScreen (display), &modeNum, &modes))
        {
          XDEBUG_PRINTCOLOR(4,__L("\tCould not query the video modes"));
          return false;
        }

      int bestMode = -1;
      for (int i = 0; i < modeNum; i++)
        {
          if ((modes[i]->hdisplay == width) &&
              (modes[i]->vdisplay == height))
            {
              bestMode = i;
            }
        }

      if (bestMode == -1)
        {
          XDEBUG_PRINTCOLOR(4,__L("\tCould not find a suitable graphics mode"));
          return false;
        }

      XDEBUG_PRINTCOLOR(0,__L("Start Mode Switching mode: %d"), bestMode);

      // set the display mode for fullscreen
      XF86VidModeSwitchToMode(display, DefaultScreen(display), modes[bestMode]);
      XDEBUG_PRINTCOLOR(0,__L("Start Viewport"));
      XF86VidModeSetViewPort(display, DefaultScreen(display), 0, 0);

      Colormap cmap = XCreateColormap(display, RootWindow(display, vi->screen),vi->visual, AllocNone);

      XSetWindowAttributes m_XSetAttr;
      m_XSetAttr.colormap = cmap;
      m_XSetAttr.border_pixel = 0;
      m_XSetAttr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | StructureNotifyMask;
      m_XSetAttr.override_redirect = True;

      XDEBUG_PRINTCOLOR(0,__L("XWindow Creation"));
      // last step
      win = XCreateWindow(display, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &m_XSetAttr);

      // remove borders from the window:
      HINTS hints;
      Atom  property;

      hints.flags       = 2;        // Specify that we're changing the window decorations.
      hints.decorations = 0;

      if(isdesktop)
        {
          XDEBUG_PRINTCOLOR(0,__L("Start Motif Hints"));
          property = XInternAtom(display,"_MOTIF_WM_HINTS",True);
          XChangeProperty(display, win, property, property, 32, PropModeReplace, (unsigned char*)&hints,5);

          Atom atoms[2] = { XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False), None};
          XChangeProperty(display, win, XInternAtom(display, "_NET_WM_STATE", False), XA_ATOM, 32, PropModeReplace, (unsigned char*)atoms, 1);
        }


      // wrap and capture mouse/keyboard, for some reason it doesn't seem to work properly
      XDEBUG_PRINTCOLOR(0,__L("XWarpPointer"));
      XWarpPointer(display, None, win, 0, 0, 0, 0, 0, 0);
      XDEBUG_PRINTCOLOR(0,__L("XMapRaised"));
      XMapRaised(display, win);
      XDEBUG_PRINTCOLOR(0,__L("XGrabKeyboard"));
      XGrabKeyboard(display, win, True, GrabModeAsync, GrabModeAsync, CurrentTime);
      XDEBUG_PRINTCOLOR(0,__L("XGrabPointer"));
      XGrabPointer(display, win, True, ButtonPressMask,
                 GrabModeAsync, GrabModeAsync, win, None, CurrentTime);
    }
  else
    {
      cmap = XCreateColormap(display, root, vi->visual, AllocNone);

      memset(&swa, 0, sizeof(swa));

      swa.colormap = cmap;
      swa.event_mask = ExposureMask | KeyPressMask;
      win = XCreateWindow(display, root, x, y, width, height, 0, vi->depth, InputOutput, vi->visual,  CWBackPixel | CWBorderPixel | CWOverrideRedirect | CWColormap | CWEventMask, &swa);
      XMapWindow(display, win);
    }

  XGetWindowAttributes(display, win, &gwa);

  this->window  = win;
  this->vi      = vi;

  this->SetWidth(width);
  this->SetHeight(height);

  ShowCursor(false);

  return true;
}


/*-------------------------------------------------------------------
//   GRPLINUXSCREENX11::ShowCursor
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  07/11/2017 13:58:44
//  @return   void :
//
//  @param    bool :
//
*//*-----------------------------------------------------------------*/
void GRPLINUXSCREENX11::ShowCursor(bool c)
{

    if (!c)
    {
        // vars to make blank cursor
        Pixmap blank;
        XColor dummy;
        char data[1] = { 0 };
        Cursor cursor;

        // make a blank cursor
        blank = XCreateBitmapFromData(display, window, data, 1, 1);
        if (blank == None) fprintf(stderr, "error: out of memory.\n");
        cursor = XCreatePixmapCursor(display, blank, blank, &dummy, &dummy, 0, 0);
        XFreePixmap(display, blank);

        XDefineCursor(display, window,cursor);
    }
    else
    {
        XUndefineCursor(display, window);
    }

}


