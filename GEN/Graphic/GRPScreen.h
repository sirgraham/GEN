//------------------------------------------------------------------------------------------
//  GRPSCREEN.H
//
/**
// \class
//
//  Graphics Screen Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 28/06/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPSCREEN_H_
#define _GRPSCREEN_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XBase.h"
#include "XFactory.h"
#include "XVector.h"
#include "XEvent.h"
#include "XSubject.h"
#include "XPublisher.h"

#include "GRPScreen.h"
#include "GRPProperties.h"
#include "GRPXEvent.h"

#include "XMemory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum GRPSCREENTYPE
{
  GRPSCREENTYPE_UNKNOW                     =  0               ,

  GRPSCREENTYPE_WINDOWS                                       ,

  GRPSCREENTYPE_LINUX_X11_GL                                  ,
  GRPSCREENTYPE_LINUX_X11_GLES                                ,
  GRPSCREENTYPE_LINUX_FRAMEBUFFER                             ,
  GRPSCREENTYPE_LINUX_FRAMEBUFFER_GLES                        ,
  GRPSCREENTYPE_LINUX_DISPMAN                                 ,

  GRPSCREENTYPE_ANDROID
};



//---- CLASS -------------------------------------------------------------------------------
class GRPVIEWPORT;


class GRPSCREEN : public GRPPROPERTIES, public XSUBJECT
{
  public:
                                  GRPSCREEN                     ();
    virtual                      ~GRPSCREEN                     ();

    GRPSCREENTYPE                 GetType                       ();
    bool                          IsValid                       ();
    bool                          IsActive                      ();

    bool                          SetPropertys                  (int width, int height, int stride, GRPPROPERTYMODE mode);

    bool                          IsFullScreen                  ();
    void                          SetIsFullScreen               (bool isfullscreen);

    virtual bool                  Create                        (void* handle = NULL)         = 0;
    virtual bool                  Update                        ()                            = 0;
    virtual bool                  Delete                        ()                            = 0;

    virtual bool                  Resize                        (int width, int height);
    bool                          UpdateSize                    (int width, int height);

    void*                         GetHandle                     ();
    GRPVIEWPORT*                  GetViewport                   (int index);

    bool                          AddViewport                   (GRPVIEWPORT* viewport);
    bool                          UseViewport                   (int n, GRPVIEWPORT& viewport);

    XVECTOR<GRPVIEWPORT*>*        GetViewports                  ();
    bool                          UpdateViewports               (int width, int height);
    void                          SetDPI                        (float dpi);
    float                         GetDPI                        ();

    virtual void                  ShowCursor                    (bool) {}

  protected:

    GRPSCREENTYPE                 type;
    bool                          isvalid;
    bool                          isactive;
    bool                          isfullscreen;
    void*                         handle;
    float                         dpi;
    XVECTOR<GRPVIEWPORT*>         viewports;

  private:

    void Clean()
    {
        type            = GRPSCREENTYPE_UNKNOW;
        isvalid         = false;
        isactive        = false;
        isfullscreen    = false;

        positionx       = 0;
        positiony       = 0;

        handle          = NULL;
        dpi             = 0.0f;
    }

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

