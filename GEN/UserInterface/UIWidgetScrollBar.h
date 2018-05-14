/*------------------------------------------------------------------------------------------
//  UIWIDGETSCROLLBAR.H
*/
/**
// \file
//
//  Scrollbar widget
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 23/02/2017 10:06:25
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETSCROLLBAR_H_
#define _UIWIDGETSCROLLBAR_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class UIWIDGETBUTTON;
class UIWIDGETIMAGE;

class UIWIDGETSCROLLBAR : public UIWIDGETCONTAINER
{
public:

                                        UIWIDGETSCROLLBAR                                   (XCHAR* name, UIMANAGER* uimanager);
                                        UIWIDGETSCROLLBAR                                   (UIWIDGETSCROLLBAR* rhs);

  virtual                              ~UIWIDGETSCROLLBAR                                   ();

  virtual UIWIDGET*                     Clone                                               ()
                                        {
                                          return new UIWIDGETSCROLLBAR(this);
                                        }

  void                                  SetType                                             ()                                                            { widgettype = UI_WIDGET_TYPE_SCROLLBAR;      }

  void                                  SetWidgets                                          (UIWIDGETBUTTON* increase, UIWIDGETBUTTON* decrease, UIWIDGETIMAGE* background, UIWIDGETBUTTON* movebar);
  void                                  SetWidgetIncrease                                   (UIWIDGETBUTTON* increase);
  void                                  SetWidgetDecrease                                   (UIWIDGETBUTTON* decrease);
  void                                  SetWidgetBackground                                 (UIWIDGETIMAGE* background);
  void                                  SetWidgetMoveBar                                    (UIWIDGETBUTTON* movebar);

  void                                  SetMinimumBarSize                                   (XDWORD minimumbarsize)                                       { this->minimumbarsize = minimumbarsize;      }
  int                                   GetMinimumBarSize                                   ()                                                            { return minimumbarsize;                      }

  void                                  SetCurrentExtraSize                                 (int currentextrasize)
                                        {
                                          if(currentextrasize < 0)
                                            {
                                              currentextrasize = 0;
                                            }
                                          this->currentextrasize = currentextrasize;
                                        }

  int                                   GetCurrentExtraSize                                 ()                                                            { return currentextrasize;                    }

  void                                  SetScrollPosition                                   (float scrollposition)
                                        {
                                          if(scrollposition < 0.0f)
                                            {
                                              scrollposition = 0.0f;
                                            }
                                          else if(scrollposition > 1.0f)
                                            {
                                              scrollposition = 1.0f;
                                            }

                                          this->scrollposition = scrollposition;
                                        }

  float                                 GetScrollPosition                                   ()                                                            { return scrollposition;                      }


  virtual bool                          HandleInternalEvent                                 (XEVENT* event);
protected:

  bool                                  InternalUpdate                                      (XDWORD widgetlevel);
  void                                  InternalArrangeChildren                             ();

  UIWIDGETBUTTON*                       increase;
  UIWIDGETBUTTON*                       decrease;
  UIWIDGETIMAGE*                        background;
  UIWIDGETBUTTON*                       movebar;

  XDWORD                                minimumbarsize;
  int                                   currentextrasize;
  float                                 scrollposition;
  float                                 scrolldelta;

  bool                                  widgetschanged;

  bool                                  mousepressed;
  int                                   oldx;
  int                                   oldy;
  int                                   deltay;

private:
  void                                  Clean                                               ()
                                        {
                                          increase          = NULL;
                                          decrease          = NULL;
                                          background        = NULL;
                                          movebar           = NULL;

                                          minimumbarsize    = 0;
                                          currentextrasize  = 0;
                                          scrollposition    = 0;
                                          scrolldelta       = 0;
                                          widgetschanged    = false;
                                          mousepressed      = false;
                                          oldx              = 0;
                                          oldy              = 0;
                                          deltay            = 0;
                                        }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

