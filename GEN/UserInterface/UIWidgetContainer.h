/*------------------------------------------------------------------------------------------
//  UIWIDGETCONTAINER.H
*/
/**
// \file
//
//  container class to inherit from
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 08/10/2015 11:04:30
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETCONTAINER_H_
#define _UIWIDGETCONTAINER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XVector.h"
#include "XMap.h"

#include "UIWidget.h"

class GRPSCENE;

class UIWIDGETLAYOUT;
class UIMOTION;

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
// follow the android convention, one dp is one px in a 160 dip screen
// formula: px = (dp*screendpi)/uidipunit;
const float uidipunit = 160.0f; // need to place this somewhere more useful

enum UIWIDGET_ALIGNFLAGS
{
  UIWIDGET_ALIGNFLAGS_NONE    = 0 ,
  UIWIDGET_ALIGNFLAGS_LEFT        ,
  UIWIDGET_ALIGNFLAGS_RIGHT       ,
  UIWIDGET_ALIGNFLAGS_CENTER      ,
  UIWIDGET_ALIGNFLAGS_TOP         ,
  UIWIDGET_ALIGNFLAGS_BOTTOM      ,
};

enum UIWIDGET_CONSTRAINTS
{
  UIWIDGET_CONSTRAINTS_NONE         = 0 ,
  UIWIDGET_CONSTRAINTS_FILL_PARENT      ,
  UIWIDGET_CONSTRAINTS_WRAP_CONTENT     ,
};

enum UIWIDGET_ANCHOR
{
  UIWIDGET_ANCHOR_UNKNOWN   = 0 ,
  UIWIDGET_ANCHOR_LEFT          ,
  UIWIDGET_ANCHOR_RIGHT         ,
  UIWIDGET_ANCHOR_TOP           ,
  UIWIDGET_ANCHOR_BOTTOM        ,
};

#define UIWIDGET_LAYOUT_WEIGTH_WRAP -1;

/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETCONTAINER : public UIWIDGET
{
  public:

                                                  UIWIDGETCONTAINER(XCHAR* name, UIMANAGER * uimanagerarg);

                                                  UIWIDGETCONTAINER               (UIWIDGETCONTAINER* rhs);

    virtual                                      ~UIWIDGETCONTAINER();

    virtual       UIWIDGET*                       Clone()
                                                  {
                                                    return new UIWIDGETCONTAINER(this);
                                                  }

    virtual       bool                            SetChildOrder                   (XDWORD index, UIWIDGET* child)
                                                  {
                                                    if(childrenbyindex.Find(child) != NOTFOUND)
                                                      {
                                                        childrenbyindex.Delete(child);
                                                      }

                                                    childrenbyindex.Set(index, child);

                                                    return true;
                                                  }

    // Hierarchy functions
    virtual       UIWIDGET*                       GetChild                        (XWORD childindex);

    virtual       UIWIDGET*                       GetChildByName                  (XSTRING* childname);
    virtual       XVECTOR<UIWIDGET*>*             GetChildren                     (XSTRING* childrenname);
    virtual       XVECTOR<UIWIDGET*>*             GetAllChildren                  ();

    virtual       UIWIDGET*                       FindChildByName                 (XSTRING* childname);
    virtual       UIWIDGET*                       FindChildByName                 (XCHAR* childname);
    virtual       XVECTOR<UIWIDGET*>*             FindChildren                    (XSTRING* childrenname);
    virtual       XVECTOR<UIWIDGET*>*             FindChildren                    (XCHAR* childrenname);
    virtual       UIWIDGET*                       FindChildByNameRec              (XSTRING* childname);
    virtual       UIWIDGET*                       FindChildByNameRec              (XCHAR* childname);
    virtual       XVECTOR<UIWIDGET*>*             FindChildrenRec                 (XSTRING* childrenname);
    virtual       XVECTOR<UIWIDGET*>*             FindChildrenRec                 (XCHAR* childrenname);

    virtual       bool                            IsChild                         (UIWIDGET* uiwidget);

    virtual       bool                            RemoveChild                     (XWORD childindex);
    virtual       bool                            RemoveChild                     (XSTRING* childname);
    virtual       bool                            RemoveChildPointer              (UIWIDGET* childpointer);
    virtual       bool                            RemoveAll                       ();

    // Drawing functions
    virtual       bool                            Draw                            (XDWORD& widgetlevel);
    virtual       bool                            Update                          (XDWORD widgetlevel);

    // Event Handling
    virtual       bool                            HandleInternalEvent             (XEVENT* event);

    virtual       void                            SetPosition                     (int x, int y);
    virtual       void                            SetPositionX                    (int x);
    virtual       void                            SetPositionY                    (int y);
                  XDWORD                          GetPositionX                    ()                                    { return x;                                                 }
                  XDWORD                          GetPositionY                    ()                                    { return y;                                                 }
                  void                            SetPositionDIP                  (float x, float y);
                  void                            SetPositionXDIP                 (float x);
                  void                            SetPositionDIPY                 (float y);

    virtual       void                            SetSize                         (XDWORD width, XDWORD height);
    virtual       void                            SetWidth                        (XDWORD width);
    virtual       void                            SetHeight                       (XDWORD height);
                  XDWORD                          GetWidth                        ()                                    { return width;                                             }
                  XDWORD                          GetHeight                       ()                                    { return height;                                            }
                  void                            SetSizeDIP                      (float width, float height);
                  void                            SetWidthDIP                     (float width);
                  void                            SetHeightDIP                    (float height);

    virtual       void                            SetRotation                     (float x, float y, float z);
    virtual       void                            SetRotationPivot                (int x, int y);
                  float                           GetRotationX                    ()                                    { return this->rotationx;                                   }
                  float                           GetRotationY                    ()                                    { return this->rotationy;                                   }
                  float                           GetRotationZ                    ()                                    { return this->rotationz;                                   }
                  int                             GetRotationPivotX               ()                                    { return rotationpivotx;                                    }
                  int                             GetRotationPivotY               ()                                    { return rotationpivoty;                                    }

    virtual       void                            SetLayoutWeight                 (int layoutweigth)                    { this->layoutweigth = layoutweigth;                        }
    virtual       int                             GetLayoutWeight                 ()                                    { return this->layoutweigth;                                }

    virtual       void                            SetHorizontalAligment           (UIWIDGET_ALIGNFLAGS flag)            { this->horizontalaligment = flag;                          }
    virtual       void                            SetVerticalAligment             (UIWIDGET_ALIGNFLAGS flag)            { this->verticalaligment = flag;                            }
    virtual       UIWIDGET_ALIGNFLAGS             GetHorizontalAligment           () const                              { return this->horizontalaligment;                          }
    virtual       UIWIDGET_ALIGNFLAGS             GetVerticalAligment             () const                              { return this->verticalaligment;                            }

    virtual       void                            SetHorizontalConstraints        (UIWIDGET_CONSTRAINTS constraint)     { horizontalconstraint = constraint;                        }
    virtual       void                            SetVerticalConstraints          (UIWIDGET_CONSTRAINTS constraints)    { verticalconstraints = constraints;                        }
    virtual       UIWIDGET_CONSTRAINTS            GetHorizontalConstraints        ()                                    { return horizontalconstraint;                              }
    virtual       UIWIDGET_CONSTRAINTS            GetVerticalConstraints          ()                                    { return verticalconstraints;                               }

    virtual       bool                            IsClickable                     ()                                    { return this->isclickable;                                 }
    virtual       void                            SetIsClickable                  (bool isclickable)                    { this->isclickable = isclickable;                          }
    virtual       bool                            IsHoverable                     ()                                    { return this->ishoverable;                                 }
    virtual       void                            SetISHoverable                  (bool ishoverable)                    { this->ishoverable = ishoverable;                          }
    virtual       bool                            IsDragrable                     ()                                    { return this->isdragable;                                  }
    virtual       void                            SetIsDragable                   (bool isdragable)                     { this->isdragable = isdragable;                            }
    virtual       bool                            IsLongClickable                 ()                                    { return islongclickable;                                   }
    virtual       void                            SetIsLongClickable              (bool islongclickable);
    virtual       void                            SetIsSticky                     (bool issticky)                       { this->issticky = issticky;                                }
    virtual       bool                            GetIsIsSticky                   ()                                    { return this->issticky;                                    }
    virtual       void                            SetIsMotionable                 (bool ismotionable)                   { this->ismotionable = ismotionable;                        }
    virtual       bool                            GetIsMotionable                 ()                                    { return ismotionable;                                      }
    virtual       void                            SetLimitChildrenCollisions      (bool limitchildrencollisions)        { this->limitchildrencollisions = limitchildrencollisions;  }

    virtual       void                            GetWidgetLocalCoordinates       (float iptx, float ipty, float & optx, float & opty);

    virtual       void                            SetIsOwnVisible                 (bool isownvisible)                   { this->isownvisible = isownvisible;                        }
    virtual       bool                            IsOwnVisible                    ()                                    { return this->isownvisible;                                }

    virtual       void                            SetAnchors                      (UIWIDGET_ANCHOR horizontalanchor, UIWIDGET_ANCHOR verticalanchor);
    virtual       void                            SetHorizontalAnchor             (UIWIDGET_ANCHOR horizontalanchor);
    virtual       void                            SetVerticalAnchor               (UIWIDGET_ANCHOR verticalanchor);

    virtual       UIWIDGET_ANCHOR                 GetHorizontalAnchor             ()                                    { return this->horizontalanchor;                            }
    virtual       UIWIDGET_ANCHOR                 GetVerticalAnchor               ()                                    { return this->verticalanchor;                              }

    virtual       void                            AttachLayout                    (UIWIDGETLAYOUT* attachedlayout);

    virtual       void                            ComputeCoords                   (); // need to take out and replace by computedesiredsize and arrange children
    virtual       void                            UpdateCoords                    ();
    virtual       UIWIDGET*                       GetHoveredWidget                (bool force = true, float zoom = 1, float x = 0, float y = 0);

    virtual       bool                            CheckCursorOver                 (float zoom = 1, float x = 0, float y = 0); // then check for cursor over recursively, but should be ideally replaced by some space partitioning technique

                  bool                            GetIsTriggered                  ()                                    { return this->istriggered;                                 }

    virtual       GRPPOINT                        ComputeDesiredSize              ();
    virtual       void                            ArrangeChildren                 ();

                  float                           gpt0x;
                  float                           gpt0y;
                  float                           gpt1x;
                  float                           gpt1y;

                  // adjusted to arrangechildren specifications
                  int                             adjustedx;
                  int                             adjustedy;
                  XDWORD                          adjustedwidth;
                  XDWORD                          adjustedheight;
                  bool                            isadjusted;

  protected:
    virtual       bool                            InternalDraw                    (XDWORD& widgetlevel);
    virtual       bool                            InternalPostDraw                (XDWORD& widgetlevel);
    virtual       bool                            InternalUpdate                  (XDWORD widgetlevel);
    virtual       bool                            InternalPostUpdate              (XDWORD widgetlevel);
    virtual       void                            DeleteChildrenAndLink           ();
    virtual       void                            HandleDrag                      ();
    virtual       void                            UpdateWidgetState               ();
    virtual       void                            InternalArrangeChildren         ();

                  //GRPSCENE*                       scene; // need to think of a way to remove this from here, as we shouldn't have any GRP classes on the base classes

                  XVECTOR<UIWIDGET*>              childrenbyindex;

                  int                             x;
                  int                             y;
                  XDWORD                          width;
                  XDWORD                          height;
                  float                           rotationx;
                  float                           rotationy;
                  float                           rotationz;
                  int                             rotationpivotx;
                  int                             rotationpivoty;

                  // aligment and size conditions
                  UIWIDGET_ALIGNFLAGS             horizontalaligment;
                  UIWIDGET_ALIGNFLAGS             verticalaligment;

                  bool                            isclickable;
                  bool                            ishoverable;
                  bool                            isdragable;
                  bool                            islongclickable;
                  bool                            issticky;
                  bool                            ismotionable;
                  bool                            limitchildrencollisions;
                  XQWORD                          triggertime; // only valid for 587k milleniums (XDWORD just admits a running time of ~58 days)

                  bool                            iscursorover;
                  bool                            iscursorontop;
                  bool                            istriggered;
                  bool                            ispressed;
                  bool                            ispressedlong;
                  bool                            isreleased;
                  float                           lastcursorposx;
                  float                           lastcursorposy;
                  float                           currentcursorposx;
                  float                           currentcursorposy;
                  float                           triggerposx;
                  float                           triggerposy;
                  float                           triggerlocalposx;
                  float                           triggerlocalposy;

                  bool                            isownvisible;

                  UIWIDGET_CONSTRAINTS            horizontalconstraint;
                  UIWIDGET_CONSTRAINTS            verticalconstraints;
                  UIWIDGET_ANCHOR                 horizontalanchor;
                  UIWIDGET_ANCHOR                 verticalanchor;

                  int                             layoutweigth;


                  XVECTOR<UIWIDGETLAYOUT*>        attachedlayouts;

                  UIMOTION*                       motion;

  private:
    virtual       bool                            JoinChild                       (UIWIDGET* child);

                  void                            Clean                           ()
                                                  {
                                                    //scene                     = NULL;

                                                    x                         = 0;
                                                    y                         = 0;
                                                    width                     = 0;
                                                    height                    = 0;

                                                    gpt0x                     = 0.0f;
                                                    gpt0y                     = 0.0f;
                                                    gpt1x                     = 0.0f;
                                                    gpt1y                     = 0.0f;

                                                    adjustedx                 = 0;
                                                    adjustedy                 = 0;
                                                    adjustedwidth             = 0;
                                                    adjustedheight            = 0;

                                                    rotationx                 = 0;
                                                    rotationy                 = 0;
                                                    rotationz                 = 0;
                                                    rotationpivotx            = 0;
                                                    rotationpivoty            = 0;

                                                    horizontalaligment        = UIWIDGET_ALIGNFLAGS_NONE;
                                                    verticalaligment          = UIWIDGET_ALIGNFLAGS_NONE;

                                                    isclickable               = false;
                                                    ishoverable               = false;
                                                    isdragable                = false;
                                                    islongclickable           = false;
                                                    issticky                  = false;
                                                    ismotionable              = false;
                                                    limitchildrencollisions   = false;
                                                    triggertime               = 0;

                                                    iscursorover              = false;
                                                    iscursorontop             = false;
                                                    istriggered               = false;
                                                    ispressed                 = false;
                                                    ispressedlong             = false;
                                                    isreleased                = false;
                                                    lastcursorposx            = 0.0f;
                                                    lastcursorposy            = 0.0f;
                                                    currentcursorposx         = 0.0f;
                                                    currentcursorposy         = 0.0f;
                                                    triggerposx               = 0.0f;
                                                    triggerposy               = 0.0f;
                                                    triggerlocalposx          = 0.0f;
                                                    triggerlocalposy          = 0.0f;

                                                    modified                  = false;
                                                    isownvisible              = true;

                                                    horizontalanchor          = UIWIDGET_ANCHOR_LEFT;
                                                    verticalanchor            = UIWIDGET_ANCHOR_TOP;

                                                    horizontalconstraint      = UIWIDGET_CONSTRAINTS_NONE;
                                                    verticalconstraints       = UIWIDGET_CONSTRAINTS_NONE;

                                                    layoutweigth              = 0;

                                                    motion                    = NULL;
                                                  }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

