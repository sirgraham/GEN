/*------------------------------------------------------------------------------------------
//  UIWIDGET.H
*/
/**
// \file
//
//  User Interface base class
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 02/10/2015 16:15:10
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGET_H_
#define _UIWIDGET_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XString.h"
#include "XVector.h"
#include "XMAP.h"
#include "XSubject.h"
#include "XObserver.h"

#include "GRPPoint.h"

#include "UIFactory.h"
#include "UIEvent.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

// for the widget state bitfield
#define UIWIDGET_STATE_NONE         (0u)
#define UIWIDGET_STATE_HOVERED      (1<<0u)
#define UIWIDGET_STATE_TRIGGERED    (1<<1u)
#define UIWIDGET_STATE_PRESSED      (1<<2u)
#define UIWIDGET_STATE_LONGPRESSED  (1<<3u)
#define UIWIDGET_STATE_RELEASED     (1<<4u)
#define UIWIDGET_STATE_SELECTED     (1<<5u)
#define UIWIDGET_STATE_PRESELECTED  (1<<6u)
#define UIWIDGET_STATE_DRAGPRESSED  (1<<7u)
#define UIWIDGET_STATE_MOTION       (1<<8u)
#define UIWIDGET_STATE_UNKNOWN      (1<<9u)

// Widget types here

/*---- CLASS -----------------------------------------------------------------------------*/
class XFACTORY;

class UIMANAGER;
class UIFACTORY;

class UIWIDGET : public XSUBJECT, public XOBSERVER
{
  friend class UIFACTORY;

  public:
                                                  UIWIDGET                (XCHAR* namestr, UIMANAGER * uimanagerin)
                                                  {
                                                    Clean           ();
                                                    name.Set        (namestr);
                                                    uimanager       = uimanagerin;
                                                    isactive        = true;
                                                    ishidden        = false;
                                                    reactstomouse   = false;
                                                    SetType         ();
                                                    SetIsComposable (false);
                                                  }

                                                  UIWIDGET(UIWIDGET* rhs)
                                                  {
                                                    Clean();

                                                    name            = rhs->name;
                                                    widgetclass     = rhs->widgetclass;
                                                    uimanager       = rhs->uimanager;
                                                    parent          = NULL; // because we are not assigning it to a parent
                                                    isactive        = rhs->isactive;
                                                    ishidden        = rhs->ishidden;
                                                    ismouseaware    = rhs->ismouseaware;
                                                    reactstomouse   = rhs->reactstomouse;
                                                    widgettype      = rhs->widgettype;
                                                    iscomposable    = rhs->iscomposable;

                                                    oldwidgetstate  = rhs->oldwidgetstate;
                                                    widgetstate     = rhs->widgetstate;

                                                    desiredsize     = rhs->desiredsize;
                                                  }

    virtual                                      ~UIWIDGET                ()                                    { Clean();                                    }

    virtual       void                            SetType                 ()                                    { this->widgettype = UI_WIDGET_TYPE_UNKNOWN;  }
    // need to make extra checks for runtime change
    virtual       void                            SetName                 (XCHAR* widgetname)                   { name.Set(widgetname);                       }
    virtual       XSTRING*                        GetName                 ()                                    { return &name;                               }

    virtual       void                            SetClass                (XCHAR* widgetclass)                  { this->widgetclass.Set(widgetclass);         }
    virtual       XSTRING*                        GetClass                ()                                    { return &widgetclass;                        }

    virtual       UIWIDGET*                       Clone                   ()                                    { return new UIWIDGET(this);                  }

    virtual       bool                            AddChild                (UIWIDGET* child);


    // Empty as the widget doesn't really need any of this logic, here to make easier the structure
    virtual       UIWIDGET*                       GetChild                (XWORD childindex)                    {   return NULL;                                }
    virtual       UIWIDGET*                       GetChildByName          (XSTRING* childname)                  {   return NULL;                                }
    virtual       XVECTOR<UIWIDGET*>*             GetChildren             (XSTRING* childrenname)               {   return NULL;                                }
    virtual       XVECTOR<UIWIDGET*>*             GetAllChildren          ()                                    {   return NULL;                                }

    virtual       UIWIDGET*                       FindChildByName         (XSTRING* childname)                  {   return NULL;                                }
    virtual       XVECTOR<UIWIDGET*>*             FindChildren            (XSTRING* childrenname)               {   return NULL;                                }
    virtual       UIWIDGET*                       FindChildByNameRec      (XSTRING* childname)                  {   return NULL;                                }
    virtual       XVECTOR<UIWIDGET*>*             FindChildrenRec         (XSTRING* childrenname)               {   return NULL;                                }

    virtual       UIWIDGET*                       FindChildByName         (XCHAR* childname)                    {   return NULL;                                }
    virtual       XVECTOR<UIWIDGET*>*             FindChildren            (XCHAR* childrenname)                 {   return NULL;                                }
    virtual       UIWIDGET*                       FindChildByNameRec      (XCHAR* childname)                    {   return NULL;                                }
    virtual       XVECTOR<UIWIDGET*>*             FindChildrenRec         (XCHAR* childrenname)                 {   return NULL;                                }

    virtual       bool                            IsChild                 (UIWIDGET* uiwidget)                  {   return false;                               }

    virtual       bool                            RemoveChild             (XWORD childindex)                    {   return false;                               }
    virtual       bool                            RemoveChild             (XSTRING* childname)                  {   return false;                               }
    virtual       bool                            RemoveChildPointer      (UIWIDGET* childpointer)              {   return false;                               }
    virtual       bool                            RemoveAll               ()                                    {   return false;                               }

    virtual       bool                            SetParent               (UIWIDGET* p);
    virtual       void                            SetParentNoCheck        (UIWIDGET* p)                         {   parent = p;                                 }
    virtual       UIWIDGET*                       GetParent               ()                                    {   return parent;                              }

    virtual       bool                            Draw                    (XDWORD& widgetlevel)                 {   return true;                                }
    virtual       bool                            Update                  (XDWORD widgetlevel)                  {   return true;                                }

    // Event handling helpers, usaless at the moment
    virtual       void                            UnRegisterOwnEvents     ()                                    {                                               }
    virtual       void                            RegisterOwnEvents       ()                                    {                                               }

                  void                            RegisterOwnEvent        (XDWORD eventtype, XOBSERVER* observer);
                  void                            UnRegisterOwnEvent      (XDWORD eventtype, XOBSERVER* observer);
                  void                            HandleEvent             (XEVENT* event);
    virtual       bool                            HandleInternalEvent     (XEVENT* event)                       {   return true;                                }

    // functions to control display and update
    virtual       void                            SetActive               (bool active)                         {   isactive = active;                          }
                  void                            SetHidden               (bool hidden)                         {   ishidden = hidden;                          }
                  void                            ToggleActive            ()                                    {   isactive = !isactive;                       }
                  void                            ToggleHidden            ()                                    {   ishidden = !ishidden;                       }
                  bool                            GetIsActive             ()                                    {   return isactive;                            }
                  bool                            GetIsHidden             ()                                    {   return ishidden;                            }

                  UI_WIDGET_TYPE                  GetType                 ()                                    {   return widgettype;                          }

    virtual       void                            ForceWidgetState        (XDWORD widgetstate);
                  XDWORD                          GetOldWidgetState       ()                                    {   return this->oldwidgetstate;                }
                  XDWORD                          GetWidgetState          ()                                    {   return this->widgetstate;                   }

    virtual       void                            SetSelected             (bool selected);
    virtual       bool                            IsSelected              ();
    virtual       void                            SetPreSelected          (bool preselected);
    virtual       bool                            GetPreSelected          ();


    virtual       void                            UpdateCoords            ()                                    {                                           }
    virtual       UIWIDGET*                       GetHoveredWidget        ( bool force  = true,
                                                                            float zoom  = 1,
                                                                            float x     = 0,
                                                                            float y     = 0)                    { return NULL;                              }
    virtual       GRPPOINT                        ComputeDesiredSize      ()                                    { return GRPPOINT(0, 0, 0, 0);              }
    virtual       GRPPOINT                        GetDesiredSize          ()                                    { return desiredsize;                       }
    virtual       void                            ArrangeChildren         ()                                    {                                           }

                  bool                            ReactsToMouse           ()                                    { return this->reactstomouse;               }
                  bool                            IsComposable            ()                                    { return this->iscomposable;                }

                  bool                            modified;

  protected:

    virtual       void                            DeleteChildrenAndLink   ()                                    {                                           }

                  void                            SetIsComposable         (bool iscomposable)                   { this->iscomposable = iscomposable;        }

                  XSTRING                         name;
                  XSTRING                         widgetclass;
                  UIMANAGER*                      uimanager;
                  UIWIDGET*                       parent;
                  bool                            isactive;
                  bool                            ishidden;
                  bool                            ismouseaware;
                  bool                            reactstomouse;
                  UI_WIDGET_TYPE                  widgettype;
                  bool                            iscomposable;

                  XDWORD                          oldwidgetstate;
                  XDWORD                          widgetstate;

                  GRPPOINT                        desiredsize;

  private:

    virtual       bool                            JoinChild               (UIWIDGET* child)                     { return false;                           }

                  void                            Clean                   ()
                                                  {
                                                    uimanager       = 0;
                                                    parent          = 0;
                                                    oldwidgetstate  = 0;
                                                    widgetstate     = 0;
                                                    modified        = false;
                                                    isactive        = true;
                                                    ishidden        = false;
                                                    ismouseaware    = false;
                                                    reactstomouse   = false;
                                                    widgettype      = UI_WIDGET_TYPE_UNKNOWN;
                                                  }



};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

