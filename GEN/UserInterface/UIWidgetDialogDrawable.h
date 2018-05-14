/*------------------------------------------------------------------------------------------
//  UIWIDGETDIALOGDRAWABLE.H
*/
/**
// \file
//
//  drawable version of dialog
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 08/07/2016 14:01:55
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETDIALOGDRAWABLE_H_
#define _UIWIDGETDIALOGDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetDialog.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSCENE;

class UIWIDGETDIALOGDRAWABLE : public UIWIDGETDIALOG
{
public:

  UIWIDGETDIALOGDRAWABLE(XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene);
  virtual ~UIWIDGETDIALOGDRAWABLE();

protected:

private:
  void Clean()
  {
  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

