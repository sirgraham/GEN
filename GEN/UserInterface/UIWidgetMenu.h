/*------------------------------------------------------------------------------------------
//  UIWIDGETMENU.H
*/
/**
// \file
//
//  menu widget
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/07/2016 17:48:46
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETMENU_H_
#define _UIWIDGETMENU_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
#include "XList.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
enum UIWIDGET_MENU_ORIENTATION
{
  UIWIDGET_MENU_ORIENTATION_HORIZONTAL  = 1,
  UIWIDGET_MENU_ORIENTATION_VERTICAL    = 2,
};

/*---- CLASS -----------------------------------------------------------------------------*/

class UIWIDGETLISTCONTAINER;
class UIWIDGETIMAGE;

class UIWIDGETMENU : public UIWIDGETFORM
{
public:

                                                UIWIDGETMENU                        (XCHAR* name, UIMANAGER* uimanager);
  virtual                                      ~UIWIDGETMENU                        ();

  virtual       void                            AddAction                           (XCHAR* action);
  virtual       void                            AddMenu                             (UIWIDGETMENU* widgetmenu);

  void                                          SetMenuOrientation                  (UIWIDGET_MENU_ORIENTATION orientation);
  UIWIDGET_MENU_ORIENTATION                     GetMenuOrientation                  ();
  void                                          SetButtonsImages                    (XCHAR* normal, XCHAR* hover, XCHAR* clicked);

  virtual       bool                            HandleInternalEvent                 (XEVENT* event);

protected:

  UIWIDGET_MENU_ORIENTATION                     orientation;
  XSTRING                                       normal;
  XSTRING                                       hover;
  XSTRING                                       clicked;

  // need to keep on a list the names of all possible items
  // if an item is a menu don't send the event ?
  XLIST<XSTRING*>                               items;
  XLIST<XSTRING*>                               actions; // if not in action list the item is a menu

  UIWIDGETLISTCONTAINER*                        container;

private:
  void                                          Clean                               ()
                                                {
                                                  orientation   = UIWIDGET_MENU_ORIENTATION_HORIZONTAL;
                                                  container     = NULL;
                                                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

