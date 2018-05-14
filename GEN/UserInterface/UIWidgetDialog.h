/*------------------------------------------------------------------------------------------
//  UIWIDGETDIALOG.H
*/
/**
// \file
//
//  dialog class, mainly for self closing
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 08/07/2016 11:49:49
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETDIALOG_H_
#define _UIWIDGETDIALOG_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETDIALOG : public UIWIDGETFORM
{
public:

                                                UIWIDGETDIALOG(XCHAR* name, UIMANAGER * uimanager);
  virtual                                      ~UIWIDGETDIALOG();

  virtual       bool                            HandleInternalEvent             (XEVENT* event);

protected:

private:
  void Clean()
  {
  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

