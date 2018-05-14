/*------------------------------------------------------------------------------------------
//  UIWIDGETTEXTDRAWABLE.H
*/
/**
// \file
//
//  draws a text widget
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 30/10/2015 10:41:03
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETTEXTDRAWABLE_H_
#define _UIWIDGETTEXTDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetText.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPOBJECT;
class GRPTEXTBOX;
class GRPFONT;

class UIWIDGETTEXTDRAWABLE : public UIWIDGETTEXT
{
public:

                                UIWIDGETTEXTDRAWABLE      (XCHAR* name, UIMANAGER * uimanagerargin, GRPSCENE* scene);
  virtual                      ~UIWIDGETTEXTDRAWABLE      ();

  virtual       bool            InternalDraw              (XDWORD& widgetlevel);

  virtual       void            SetActive                 (bool active);

protected:
                GRPTEXTBOX*     textbox;
                GRPFONT*        font;

private:
                void            Clean()
                                {
                                  textbox   = NULL;
                                  font      = NULL;
                                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

