/**-------------------------------------------------------------------------------------------------------------------
*
* @file        GRPScreen.h
*
* @class       GRPSCREEN
* @brief       Graphics Screen Class
* @ingroup     GRAPHIC
*
* @author      Abraham J. Velez 
* @date        01/06/2018 12:59:25
*
* @copyright   Copyright(c) 2005 - 2018 GEN Group.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
*---------------------------------------------------------------------------------------------------------------------*/

#ifndef _GRPSCREEN_H_
#define _GRPSCREEN_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "XBase.h"
#include "XFactory.h"
#include "XVector.h"
#include "XEvent.h"
#include "XSubject.h"
#include "XPublisher.h"

#include "GRPProperties.h"
#include "GRPXEvent.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPSCREENTYPE
{
  GRPSCREENTYPE_UNKNOW                        =  0  ,

  GRPSCREENTYPE_WINDOWS                             ,

  GRPSCREENTYPE_LINUX_X11_GL                        ,
  GRPSCREENTYPE_LINUX_X11_GLES                      ,
  GRPSCREENTYPE_LINUX_FRAMEBUFFER                   ,
  GRPSCREENTYPE_LINUX_FRAMEBUFFER_GLES              ,
  GRPSCREENTYPE_LINUX_DISPMAN                       ,

  GRPSCREENTYPE_ANDROID
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

#ifdef GRPVIEWPORTS_ACTIVE
class GRPVIEWPORT;
#endif


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
    
    virtual void                  ShowCursor                    (bool active);
   
    bool                          UpdateSize                    (int width, int height);

    void*                         GetHandle                     (); 
        
    void                          SetDPI                        (float dpi);
    float                         GetDPI                        ();


    #ifdef GRPVIEWPORTS_ACTIVE
    GRPVIEWPORT*                  GetViewport                   (int index);
    bool                          AddViewport                   (GRPVIEWPORT* viewport);
    bool                          UseViewport                   (int n, GRPVIEWPORT& viewport);

    XVECTOR<GRPVIEWPORT*>*        GetViewports                  ();
    bool                          UpdateViewports               (int width, int height);   
    #endif  

    
  protected:

    GRPSCREENTYPE                 type;
    bool                          isvalid;
    bool                          isactive;
    bool                          isfullscreen;
    void*                         handle;
    float                         dpi;

    #ifdef GRPVIEWPORTS_ACTIVE
    XVECTOR<GRPVIEWPORT*>         viewports;
    #endif  

  private:

    void                           Clean                          ();   
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

