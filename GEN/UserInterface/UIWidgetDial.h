/*------------------------------------------------------------------------------------------
//  UIWIDGETDIAL.H
*/
/**
// \file
//
//  dial widget
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 20/03/2017 10:08:47
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETDIAL_H_
#define _UIWIDGETDIAL_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETIMAGE;

class UIWIDGETDIAL : public UIWIDGETFORM
{
public:

                                  UIWIDGETDIAL                        (XCHAR* name, UIMANAGER* uimanager);
                                  UIWIDGETDIAL                        (UIWIDGETDIAL* rhs);
  virtual                        ~UIWIDGETDIAL                        ();

  virtual UIWIDGET*               Clone                               ()                                          { return new UIWIDGETDIAL(this);          }

  virtual void                    SetType                             ()                                          { this->widgettype = UI_WIDGET_TYPE_DIAL; }

  void                            SetRotation                         (float rotation)                            { this->rotation = rotation;              }
  float                           GetRotation                         ()                                          { return rotation;                        }

  void                            SetDialWidget                       (UIWIDGETIMAGE* dialwidget);

protected:

  virtual       void              InternalArrangeChildren             ();

  float                           rotation;
  UIWIDGETIMAGE*                  dial;

private:
  void                            Clean()
                                  {
                                    rotation  = 0.0f;
                                    dial      = NULL;
                                  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

