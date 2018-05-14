/*------------------------------------------------------------------------------------------
//  UIWIDGETTAB.H
*/
/**
// \file
//
//  tab widget
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 24/06/2016 17:57:21
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETTAB_H_
#define _UIWIDGETTAB_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETTAB : public UIWIDGETCONTAINER
{
public:

                                                UIWIDGETTAB                 (XCHAR* name, UIMANAGER * uimanagerarg);
  virtual                                      ~UIWIDGETTAB                 ();

  virtual       bool                            InternalDraw                (XDWORD& widgetlevel);
  virtual       bool                            InternalUpdate              (XDWORD widgetlevel);
  virtual       void                            UnRegisterOwnEvents         ();
  virtual       void                            RegisterOwnEvents           ();

  virtual       bool                            HandleInternalEvent         (XEVENT* event);

  virtual       void                            SetActive                   (bool active);

  virtual       bool                            SetActiveTab                (XDWORD index);
  virtual       bool                            AddTab                      ();
  virtual       bool                            RemoveTab                   (XDWORD index);
  virtual       XDWORD                          GetNumberOfTabs             ();
  virtual       XVECTOR<UIWIDGET*>*             GetTab                      (XDWORD index);


protected:
  XDWORD                                        activetab;
  XVECTOR<XVECTOR<UIWIDGET*>* >                 tabs;
private:
  void                                          Clean                       ()
                                                {
                                                  activetab = 0;
                                                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

