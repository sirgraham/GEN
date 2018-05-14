/*------------------------------------------------------------------------------------------
//  UIWIDGETTABDRAWABLE.H
*/
/**
// \file
//
//  draws a tab widget
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 27/06/2016 17:31:05
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETTABDRAWABLE_H_
#define _UIWIDGETTABDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetTab.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSCENE;
class GRPELEMENT;

class UIWIDGETTABDRAWABLE : public UIWIDGETTAB
{
public:

  UIWIDGETTABDRAWABLE(XCHAR* name, UIMANAGER * uimanagerarg, GRPSCENE* scene);
  virtual ~UIWIDGETTABDRAWABLE();

protected:

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

