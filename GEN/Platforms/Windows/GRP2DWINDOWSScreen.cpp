
/*------------------------------------------------------------------------------------------
//  GRP2DWINDOWSCREEN.CPP
//
//  windows 2D screen
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 22/07/2016 16:33:35
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRP2DWINDOWSScreen.h"
#include "GRPBitmap.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  GRP2DWINDOWSSCREEN::GRP2DWINDOWSSCREEN
*/
/**
//
//  Class Constructor GRP2DWINDOWSSCREEN
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/07/2016 11:30:00
//
*/
/*-----------------------------------------------------------------*/
GRP2DWINDOWSSCREEN::GRP2DWINDOWSSCREEN()
{
  Clean();
}





/*-------------------------------------------------------------------
//  GRP2DWINDOWSSCREEN::~GRP2DWINDOWSSCREEN
*/
/**
//
//   Class Destructor GRP2DWINDOWSSCREEN
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/07/2016 11:30:06
//
*/
/*-----------------------------------------------------------------*/
GRP2DWINDOWSSCREEN::~GRP2DWINDOWSSCREEN()
{
  Clean();
}





/*-------------------------------------------------------------------
//  GRP2DWINDOWSSCREEN::Create
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/07/2016 11:30:13
//
//  @return       bool :
//
//  @param        handle :
*/
/*-----------------------------------------------------------------*/
bool GRP2DWINDOWSSCREEN::Create(void* handle)
{
  bitmap = new GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGBA32>(1920, 1080, GRPPROPERTYMODE_32_RGBA_8888);

  XSTRING unistrclassname(classname);

  wndclass.lpszClassName = unistrclassname.Get();
  wndclass.hInstance     = hinstance;

  RegisterClassEx(&wndclass);

  //sx = shiftx;
  //sy = shifty;
  //
  //createscreen = false;

  if(!handle)
    {

      if(IsFullScreen())
        {
          DEVMODE devmode;

          memset(&devmode,0,sizeof(devmode));

          devmode.dmSize       = sizeof(devmode);
          devmode.dmPelsWidth  = width;
          devmode.dmPelsHeight = height;
          devmode.dmBitsPerPel = 32;
          devmode.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

          if(ChangeDisplaySettings(&devmode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) return NULL;


          hwnd=CreateWindow(unistrclassname.Get() ,
                            NULL                  ,
                            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP ,
                            0,0                   ,
                            width,height          ,
                            NULL                  ,
                            NULL                  ,
                            hinstance             ,
                            (void*)this);

        }
       else
        {
          RECT   rect;
          XDWORD  style;

          //createscreen = true;

          GetClientRect(GetDesktopWindow(),&rect);

          rect.right  = (rect.right - width)/2;
          rect.bottom = (rect.bottom- height)/2;

          #ifdef XDEBUG
          //rect.right -= (rect.right + 1024);
          //rect.bottom = 0;
          #endif

          hwnd=CreateWindowEx(0                       , //WS_EX_CLIENTEDGE        ,
                              unistrclassname.Get()   ,
                              NULL                    ,
                              WS_OVERLAPPED           , //WS_POPUP|WS_VISIBLE|WS_SYSMENU ,
                              rect.right,rect.bottom  ,
                              width ,height           ,
                              NULL                    ,
                              NULL                    ,
                              hinstance               ,
                              (void*)this);

          if(!hwnd) return false;

          GetClientRect(hwnd,&rect);

          rect.right  = rect.left + width;    //+ 16;
          rect.bottom = rect.top  + height;   //+ 16;

          style = GetWindowLong(hwnd, GWL_STYLE);

          AdjustWindowRect(&rect,style,false);

          SetWindowPos(hwnd,NULL,0,0, (rect.right-rect.left), (rect.bottom-rect.top) , SWP_NOMOVE | SWP_NOZORDER);
        }

    } else hwnd = (HWND)(handle);

  if(!hwnd) return false;

  ShowWindow(hwnd, SW_SHOW);

  SetForegroundWindow(hwnd);

  SetFocus(hwnd);

  hdc=GetDC(hwnd);
  if(!hdc) return  false;

  memset(&hinfo,0,sizeof(BITMAPINFO));

  hinfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
  hinfo.bmiHeader.biWidth         = width;
  hinfo.bmiHeader.biHeight        = height;
  hinfo.bmiHeader.biPlanes        = 1;
  hinfo.bmiHeader.biBitCount      = bitmap->GetBitsperPixel();
  hinfo.bmiHeader.biCompression   = BI_RGB;
  hinfo.bmiHeader.biSizeImage     = (XDWORD)(hinfo.bmiHeader.biWidth*hinfo.bmiHeader.biHeight*hinfo.bmiHeader.biBitCount)/8;

  //hbmp=CreateDIBSection(hdc,&hinfo,DIB_RGB_COLORS,(void **)&buffer,NULL,(XDWORD)0);
  hbmp=CreateDIBSection(hdc,&hinfo,DIB_RGB_COLORS,(void **)bitmap->GetBufferPointer(),NULL,(XDWORD)0);
  if(!hbmp) return false;

  //size = hinfo.bmiHeader.biSizeImage;

  ohbmp=(HBITMAP)SelectObject(hdc,(HGDIOBJ)hbmp);

  return true;
}





/*-------------------------------------------------------------------
//  GRP2DWINDOWSSCREEN::Update
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/07/2016 11:30:24
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRP2DWINDOWSSCREEN::Update()
{
  // copy over to the screen using SetDIBitsToDevice



  SetDIBitsToDevice(hdc,0,0,1920  ,
                            1080  ,
                            0,0,0,
                            1080  ,
                            bitmap->GetBuffer(),&hinfo,DIB_RGB_COLORS);

  return true;
}




/*-------------------------------------------------------------------
//  GRP2DWINDOWSSCREEN::Delete
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/07/2016 11:30:36
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRP2DWINDOWSSCREEN::Delete()
{
  return true;
}
