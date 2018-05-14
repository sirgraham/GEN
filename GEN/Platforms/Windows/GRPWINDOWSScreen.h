//------------------------------------------------------------------------------------------
//  GRPWINDOWSSCREEN.H
//
/**
// \class
//
//  WINDOWS Graphics Screen class
//
//  @author  Abraham J. Velez
//  @version 28/06/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPWINDOWSSCREEN_H_
#define _GRPWINDOWSSCREEN_H_


//---- INCLUDES ----------------------------------------------------------------------------


#include "GRPOpenGL.h"
#include "GRPScreen.h"

#include "XString.h"

#include "XApplication.h"

#include "XMemory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;

class GRPWINDOWSSCREEN : public GRPSCREEN
{
  public:
                                          GRPWINDOWSSCREEN            ();
    virtual                              ~GRPWINDOWSSCREEN            ();

    bool                                  Create                      (void* handle = NULL);

    bool                                  Update                      ();
    bool                                  Delete                      ();

    bool                                  Resize                      (int width, int height);

    bool                                  CreateWIN32Window           (void * handle = NULL);
    void                                  ChooseValidPixelFormat      ();
    void                                  CreateFinalWIN32Window      ();

    HDC                                   GetHDC                      ();

    void                                  SetPixelFormat              (int nPixelFormat);

    virtual void                          ShowCursor                  (bool h);

  private:

    void                                  Clean                       ();
    static  LRESULT CALLBACK              BaseWndProc                 (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static  LRESULT CALLBACK              DummyWndProc                (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


    WNDCLASSEX                            wndclass;
    XSTRING                               classname;
    XSTRING                               title;

    HINSTANCE                             hinstance;
    HWND                                  hwnd;
    HDC                                   hdc;

    PIXELFORMATDESCRIPTOR                 pixelformatdescriptor;
    int                                   nPixelFormat;

    DEVMODE                               devmode;

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

