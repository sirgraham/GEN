/*------------------------------------------------------------------------------------------
//  UIMANAGER.H
*/
/**
// \file
//
//  Manages all of the UI events
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 05/10/2015 12:13:05
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XObserver.h"
#include "XPublisher.h"
#include "XSubject.h"
#include "XVector.h"
#include "XPair.h"


#include "UIFactory.h"
#include "UIWidget.h"
#include "UIInput.h"
#include "UIEvent.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class XSTRING;
class INPMANAGER;
class INPDEVICE;

class UISTYLE;
class UIWIDGETCREATORHELPER;
class UI3DOBJECTLOADER;
class UIWIDGETIMAGE;

class UIMOTION;

class XTIMER;

class UIMANAGER : public XOBSERVER, public XSUBJECT
{
  public:

                                                                UIMANAGER                     (INPMANAGER * inpmanagerin);
    virtual                                                    ~UIMANAGER                     ();

                    UIWIDGET*                                   CreateWidget                  (UI_WIDGET_TYPE type, XCHAR* name);
                    UIWIDGET*                                   GetRootWidget                 ();
                    void                                        SetRootWidget                 (UIWIDGET * rootwidget);
                    UIWIDGET*                                   GetHoveredWidget              ()                                                { return hoveredwidget;                         }

                    void                                        RegisterOwnEvent              (XDWORD eventtype);
                    void                                        UnRegisterOwnEvent            (XDWORD eventtype);
                    void                                        HandleEvent                   (XEVENT *event);

                    UIINPUT*                                    GetInput                      ()                                                { return uiinput;                               }

                    void                                        Update                        ();
                    void                                        Draw                          ();
                    INPDEVICE*                                  GetKeyboard                   ()                                                { return inpdevicekeyboard;                     }
                    INPDEVICE*                                  GetMouse                      ()                                                { return inpdevicemouse;                        }

                    void                                        AddToDeleteQueue              (UIWIDGET* widget)                                { todelete.Add(widget);                         }
                    void                                        AddToAddQueue                 (UIWIDGET* parent, UIWIDGET* child)
                                                                {
                                                                  XPAIR<UIWIDGET*, UIWIDGET*> add(parent, child);
                                                                  toadd.Add(add);
                                                                }

                    void                                        SetRealScreenSize         (XDWORD realwidth, XDWORD realheight)
                                                                {
                                                                  this->realwidth = realwidth;
                                                                  this->realheight = realheight;
                                                                  this->realaspectratio = (float)realwidth/(float)realheight;
                                                                }

                    void                                        SetVirtualScreenSize          (XDWORD virtualwidth, XDWORD virtualheight)
                                                                {
                                                                  this->virtualwidth = virtualwidth;
                                                                  this->virtualheight = virtualheight;
                                                                  this->virtualaspectratio = (float)virtualwidth/(float)virtualheight;
                                                                  this->virtualheight = (XDWORD)(virtualwidth/realaspectratio);
                                                                }

                    float                                       GetVirtualAspectRatio         ()
                                                                {
                                                                  return this->virtualaspectratio;
                                                                }

                    XDWORD                                      GetRealWidth                  ()                                                { return this->realwidth;                       }
                    XDWORD                                      GetRealHeight                 ()                                                { return this->realheight;                      }

                    XDWORD                                      GetVirtualWidth               ()                                                { return this->virtualwidth;                    }
                    XDWORD                                      GetVirtualHeight              ()                                                { return this->virtualheight;                   }

                    float                                       GetRealWidthRatio             ()                                                { return realwidth/(float)virtualwidth;         }
                    float                                       GetRealHeightRatio            ()                                                { return realheight/(float)virtualheight;       }

                    void                                        SetDPI                        (float DPI)                                       { this->DPI = DPI;                              }
                    float                                       GetDPI                        ()                                                { return this->DPI;                             }

                    void                                        SetModifiedHierarchy          (bool modifiedhierarchy)                          { this->modifiedhierarchy = modifiedhierarchy;  }

                    void                                        SetUIStyle                    (UISTYLE* uistyle);
                    UISTYLE*                                    GetUIStyle                    ();

                    void                                        SetUIWidgetCreator            (UIWIDGETCREATORHELPER* uiwidgetcreator)          { this->uiwidgetcreator = uiwidgetcreator;      }
                    UIWIDGETCREATORHELPER*                      GetUIWIdgetCreator            ()                                                { return this->uiwidgetcreator;                 }

                    UI3DOBJECTLOADER*                           Get3DObjectLoader             ()                                                { return ui3dobjectloader;                      }

                    void                                        SetDisableInput               (bool disableinput)                               { this->disableinput = disableinput;            }
                    void                                        SetDrawMouse                  (bool drawmouse)                                  { this->drawmouse = drawmouse;                  }
                    bool                                        GetDrawMouse                  ()                                                { return drawmouse;                             }
                    UIWIDGETIMAGE*                              GetMouseWidget                ()                                                { return mouse;                                 }

                    bool                                        AquireMouse                   (UIWIDGET* widget);
                    bool                                        ReleaseMouse                  (UIWIDGET* widget);
                    UIWIDGET*                                   GetMouseOwner                 ();

                    bool                                        AquireKeyboard                (UIWIDGET* widget);
                    bool                                        ReleaseKeyboard               (UIWIDGET* widget);
                    UIWIDGET*                                   GetKeyboardOwner              ();

                    XQWORD                                      GetClockTime                  ();
                    void                                        SetLongPressTime              (XDWORD longpresstime)                            { this->longpresstime = longpresstime;          }
                    XDWORD                                      GetLongPressTime              ()                                                { return longpresstime;                         }

                    bool                                        IsInMotion                    ()                                                { return inmotion;                              }
                    void                                        SetIsInMotion                 (bool inmotion)                                   { this->inmotion = inmotion;                    }
  protected:

                    UIFACTORY*                                  uifactory;
                    //XVECTOR<UIWIDGET*>                        globalwidgetlist;
                    UIWIDGET*                                   root;
                    UIINPUT*                                    uiinput;
                    INPMANAGER*                                 inpmanager;
                    INPDEVICE*                                  inpdevicekeyboard;
                    INPDEVICE*                                  inpdevicemouse;
                    XMAP<XDWORD, XDWORD>                        eventsubjects;
                    XVECTOR<UIWIDGET*>                          todelete;
                    XVECTOR<XPAIR<UIWIDGET*, UIWIDGET*> >       toadd;

                    XDWORD                                      virtualwidth;
                    XDWORD                                      virtualheight;
                    float                                       virtualaspectratio;

                    XDWORD                                      realwidth;
                    XDWORD                                      realheight;
                    float                                       realaspectratio;

                    float                                       DPI;

                    bool                                        modifiedhierarchy;
                    bool                                        disableinput;
                    bool                                        drawmouse;

                    bool                                        inmotion;

                    UISTYLE*                                    uistyle;
                    UIWIDGETCREATORHELPER*                      uiwidgetcreator;
                    UI3DOBJECTLOADER*                           ui3dobjectloader;
                    UIWIDGET*                                   hoveredwidget;
                    UIWIDGET*                                   mouseowner;
                    UIWIDGET*                                   keyboardowner;
                    UIWIDGETIMAGE*                              mouse;

                    XDWORD                                      longpresstime;
                    UIMOTION*                                   motion;

                    XTIMER*                                     uiclock;
  private:
                    void                                        Clean                         ()
                                                                {
                                                                  root                = 0;
                                                                  uifactory           = 0;
                                                                  inpmanager          = 0;
                                                                  uiinput             = 0;
                                                                  virtualwidth        = 0;
                                                                  virtualheight       = 0;
                                                                  realwidth           = 0;
                                                                  realheight          = 0;
                                                                  modifiedhierarchy   = true;
                                                                  disableinput        = false;
                                                                  drawmouse           = false;
                                                                  uistyle             = NULL;
                                                                  uiwidgetcreator     = NULL;
                                                                  ui3dobjectloader    = NULL;
                                                                  hoveredwidget       = NULL;

                                                                  mouseowner          = NULL;
                                                                  keyboardowner       = NULL;
                                                                  mouse               = NULL;

                                                                  inmotion            = false;

                                                                  longpresstime       = 0;
                                                                  motion              = NULL;

                                                                  uiclock             = NULL;
                                                                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

