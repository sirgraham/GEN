/*------------------------------------------------------------------------------------------
//  UIWIDGETCREATORHELPERDRAWABLE.H
*/
/**
// \file
//
//  helper class to create complex drawable widgets
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 22/06/2016 11:06:23
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETCREATORHELPERDRAWABLE_H_
#define _UIWIDGETCREATORHELPERDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetCreatorHelper.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGET;
class GRPSCENE;


class UIWIDGETCREATORHELPERDRAWABLE : UIWIDGETCREATORHELPER
{
public:

                              UIWIDGETCREATORHELPERDRAWABLE                                 (UIMANAGER* uimanager, GRPSCENE* scene);
  virtual                    ~UIWIDGETCREATORHELPERDRAWABLE                                 ();

  UIWIDGET*                   CreateButton                                                  (XCHAR* text, XCHAR* background, XCHAR* hover, XCHAR* click);
  UIWIDGET*                   CreateMessageDialog                                           (XCHAR* text, XCHAR* backimage, XCHAR* background, XCHAR* hover, XCHAR* click);

protected:
  GRPSCENE* scene;
private:
  void                        Clean                                                         ()
                              {
                                scene = NULL;
                              }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

