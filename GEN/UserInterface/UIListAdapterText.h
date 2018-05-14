/*------------------------------------------------------------------------------------------
//  UILISTADAPTERTEXT.H
*/
/**
// \file
//
//  text based list adapter helper
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 06/02/2017 17:24:06
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UILISTADAPTERTEXT_H_
#define _UILISTADAPTERTEXT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIListAdapter.h"
#include "UIManager.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/



class UILISTADAPTERTEXT : public UILISTADAPTER<XSTRING*>
{
public:

                                    UILISTADAPTERTEXT                                       (UIMANAGER* uimanager);
  virtual                          ~UILISTADAPTERTEXT                                       ();

  virtual UIWIDGET*                 GetWidget                                               (XDWORD position);

  void                              SetSize                                                 (XDWORD size)                                   { this->size = size; }

protected:

  XDWORD                            size;

private:
  void                              Clean                                                   ()
                                    {

                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

