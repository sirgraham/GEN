/*------------------------------------------------------------------------------------------
//  UIWIDGETFORMDRAWABLE.H
*/
/**
// \file
//
//  class that can draw a widgetform
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 22/10/2015 10:50:19
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETFORMDRAWABLE_H_
#define _UIWIDGETFORMDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class GRPOBJECT;
class GRPTEXTBOX;
class GRPELEMENT;
class GRPFONT;
class GRPNODE;


class UIWIDGETFORMDRAWABLE : public UIWIDGETFORM
{
public:

                                UIWIDGETFORMDRAWABLE    (XCHAR* name, UIMANAGER * uimanagerargin, GRPSCENE* scene);

  virtual                      ~UIWIDGETFORMDRAWABLE    ()
                                {
                                  Clean();
                                }

  virtual       bool            InternalDraw            (XDWORD& widgetlevel);

  GRPNODE*                      GetNode                 ();

protected:

                //GRPSCENE*       scene;

                GRPOBJECT*      window;
                GRPTEXTBOX*     titledraw;
                GRPFONT*        font;
                GRPELEMENT*     plane;
                GRPELEMENT*     topbar;
                GRPELEMENT*     close;

private:

                void            Clean                   ()
                                {
                                  window  = NULL;
                                  plane   = NULL;
                                  topbar  = NULL;
                                  close   = NULL;
                                  font    = NULL;
                                }



};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

