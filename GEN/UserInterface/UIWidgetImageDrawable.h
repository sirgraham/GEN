/*------------------------------------------------------------------------------------------
//  UIWIDGETIMAGEDRAWABLE.H
*/
/**
// \file
//
//  Draws an Image Widget
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 07/06/2016 13:09:01
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETIMAGEDRAWABLE_H_
#define _UIWIDGETIMAGEDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetImage.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class GRPOBJECT;
class GRPELEMENT;
class GRPTEXTURE;

class UIWIDGETIMAGEDRAWABLE : public UIWIDGETIMAGE
{
public:

                                UIWIDGETIMAGEDRAWABLE       (XCHAR* name, UIMANAGER * uimanagerargin, GRPSCENE* scene);
  virtual                      ~UIWIDGETIMAGEDRAWABLE       ();

  virtual       bool            InternalDraw                (XDWORD& widgetlevel);
  virtual       bool            InternalUpdate              (XDWORD widgetlevel);

  virtual       void            SetActive                   (bool active);

protected:
  GRPOBJECT*                    imageobject;
  GRPELEMENT*                   imagecanvas;

private:
  void                          Clean                       ()
                                {
                                  imageobject = NULL;
                                  imagecanvas = NULL;
                                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

