/*------------------------------------------------------------------------------------------
//  UIWIDGETBUTTONDRAWABLE.H
*/
/**
// \file
//
//  draws a button
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 26/10/2015 9:22:58
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETBUTTONDRAWABLE_H_
#define _UIWIDGETBUTTONDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetButton.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPOBJECT;
class GRPELEMENT;

class UIWIDGETBUTTONDRAWABLE : public UIWIDGETBUTTON
{
public:

                                UIWIDGETBUTTONDRAWABLE      (XCHAR* name, UIMANAGER * uimanagerargin, GRPSCENE* scene);
  virtual                      ~UIWIDGETBUTTONDRAWABLE      ();

  virtual       bool            InternalDraw                (XDWORD& widgetlevel);
  virtual       bool            InternalUpdate              (XDWORD widgetlevel);

  virtual       void            SetStateWidgets             (UIWIDGET* widgetnormal = NULL, UIWIDGET* widgethover = NULL, UIWIDGET* widgetpressed = NULL);

  virtual       UIWIDGET*       GetWidgetNormal             ()                                                                                                  { return this->widgetnormal;  }
  virtual       UIWIDGET*       GetWidgetHover              ()                                                                                                  { return this->widgethover;   }
  virtual       UIWIDGET*       GetWidgetPressed            ()                                                                                                  { return this->widgetpressed; }

  virtual       bool            SubstituteNormal            (UIWIDGET* widgetnormal);
  virtual       bool            SubstituteHover             (UIWIDGET* widgethover);
  virtual       bool            SubstitutePressed           (UIWIDGET* widgetpressed);

  virtual       void            SetActive                   (bool active);

protected:

                GRPOBJECT*      button;
                GRPELEMENT*     buttonback;

                UIWIDGET*       widgetnormal;
                UIWIDGET*       widgethover;
                UIWIDGET*       widgetpressed;

private:
                void            Clean()
                                {
                                  button          = NULL;
                                  buttonback      = NULL;
                                  scene           = NULL;
                                  widgetnormal    = NULL;
                                  widgethover     = NULL;
                                  widgetpressed   = NULL;
                                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


