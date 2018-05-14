/*------------------------------------------------------------------------------------------
//  UIWIDGETTOAST.H
*/
/**
// \file
//
//  A Widget Toast
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/07/2016 14:05:45
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETTOAST_H_
#define _UIWIDGETTOAST_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class XTIMER;

class UIWIDGETTOAST : public UIWIDGETFORM
{
public:

                                                UIWIDGETTOAST               (XCHAR* name, UIMANAGER * uimanager);

                                                UIWIDGETTOAST(UIWIDGETTOAST* rhs);

  virtual                                      ~UIWIDGETTOAST               ();

  virtual       UIWIDGET*                       Clone                       ()
                                                {
                                                  return new UIWIDGETTOAST(this);
                                                }

  void                                          SetTime                     (XDWORD milliseconds);
  XDWORD                                        GetTime                     ();

  virtual       bool                            InternalUpdate              (XDWORD widgetlevel);

protected:

  XTIMER*                                       xtimer;
  XDWORD                                        milliseconds;
  bool                                          updated;

private:
  void                                          Clean                       ()
                                                {
                                                  xtimer        = NULL;
                                                  milliseconds  = 0;
                                                  updated       = false;
                                                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

