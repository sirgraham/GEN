/*------------------------------------------------------------------------------------------
//  UIWIDGETLISTCONTAINERDRAWABLE.H
*/
/**
// \file
//
//  drawable instance of linear container
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 30/06/2016 9:45:43
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETLISTCONTAINERDRAWABLE_H_
#define _UIWIDGETLISTCONTAINERDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetListContainer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSCISSORELEMENT;

class UIWIDGETLISTCONTAINERDRAWABLE : public UIWIDGETLISTCONTAINER
{
public:

                                                UIWIDGETLISTCONTAINERDRAWABLE                               (XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene);
  virtual                                      ~UIWIDGETLISTCONTAINERDRAWABLE                               ();

protected:
  virtual       bool                            InternalDraw                                                (XDWORD& widgetlevel);
  virtual       bool                            InternalPostDraw                                            (XDWORD& widgetlevel);

  // save the scissor old parameters to restore them
  GRPSCISSORELEMENT*                            scissor;

  bool                                          scissoractive;
  XDWORD                                        x0;
  XDWORD                                        y0;
  XDWORD                                        x1;
  XDWORD                                        y1;

private:
  void                                          Clean                                                       ()
                                                {
                                                  scissor         = NULL;
                                                  scissoractive   = false;
                                                  x0              = 0;
                                                  y0              = 0;
                                                  x1              = 0;
                                                  y1              = 0;
                                                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

