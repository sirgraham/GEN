/*------------------------------------------------------------------------------------------
//  UIWIDGETSCROLLCONTAINER.H
*/
/**
// \file
//
//  scroll container
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 22/02/2017 16:03:12
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETSCROLLCONTAINER_H_
#define _UIWIDGETSCROLLCONTAINER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETSCROLLCONTAINER : public UIWIDGETCONTAINER
{
public:

                                      UIWIDGETSCROLLCONTAINER                   (XCHAR* name, UIMANAGER* uimanager);
                                      UIWIDGETSCROLLCONTAINER                   (UIWIDGETSCROLLCONTAINER* rhs);
  virtual                            ~UIWIDGETSCROLLCONTAINER                   ();

  virtual UIWIDGET*                   Clone                                     ()
                                      {
                                        return new UIWIDGETSCROLLCONTAINER(this);
                                      }

  virtual void                        SetType                                   ()                                    { widgettype = UI_WIDGET_TYPE_SCROLLCONTAINER;  }

  void                                SetLimitX                                 (bool limitx)                         { this->limitx = limitx;                        }
  bool                                GetLimitX                                 ()                                    { return limitx;                                }

  void                                SetLimitY                                 (bool limity)                         { this->limity = limity;                        }
  bool                                GetLimitY                                 ()                                    { return limity;                                }

  void                                SetXScroll                                (int xscroll);
  int                                 GetXScroll                                ()                                    { return xscroll;                               }

  void                                SetYScroll                                (int yscroll);
  int                                 GetYScroll                                ()                                    { return yscroll;                               }

  virtual       bool                  HandleInternalEvent                       (XEVENT* event);

protected:

  virtual bool                        InternalUpdate                            (XDWORD widgetlevel);
  virtual void                        InternalArrangeChildren                   ();

  bool                                limitx;
  bool                                limity;

  int                                 xscroll;
  int                                 yscroll;

private:
  void                                Clean                                     ()
                                      {
                                        limitx = false;
                                        limity = false;
                                        xscroll = 0;
                                        yscroll = 0;
                                      }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

