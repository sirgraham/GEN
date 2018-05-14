
/*------------------------------------------------------------------------------------------
//  UIFACTORY.CPP
//
//  creates widgets according to an enum
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 08/10/2015 10:02:15
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIFactory.h"

// widget types
#include "UIWidgetForm.h"
#include "UIWidgetText.h"
#include "UIWidgetButton.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  UIFACTORY::CreateWidget
*/
/**
//
//  Creates a new widget
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/10/2015 9:53:58
//
//  @return       UIWIDGET* :
//
//  @param        type :
//  @param        uimanager :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIFACTORY::CreateWidget(UI_WIDGET_TYPE type, UIMANAGER * uimanager, XCHAR* name)
{
  //UIWIDGET * widget = NULL;
  //
  //// create the appropiate widget as needed
  //switch(type)
  //  {
  //    case UI_WIDGET_TYPE_FORM    :
  //                                  widget = new UIWIDGETFORM(name, uimanager);
  //                                  if(!widget)
  //                                    {
  //                                      return NULL;
  //                                    }
  //                                  break;
  //    case UI_WIDGET_TYPE_TEXT    :
  //                                  widget = new UIWIDGETTEXT(name, uimanager);
  //                                  if(!widget)
  //                                    {
  //                                      return NULL;
  //                                    }
  //                                  break;
  //    case UI_WIDGET_TYPE_BUTTON  :
  //                                  widget = new UIWIDGETBUTTON(name, uimanager);
  //                                  if(!widget)
  //                                    {
  //                                      return NULL;
  //                                    }
  //                                  break;
  //
  //    default                     :
  //                                  return NULL;// failed
  //  }
  //
  //return widget;

  return NULL;
}

/*-------------------------------------------------------------------
//  UIFACTORY::DeleteWidget
*/
/**
//
//  deletes a widget
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/10/2015 9:54:09
//
//  @return       bool :
//
//  @param        widget :
*/
/*-----------------------------------------------------------------*/
bool UIFACTORY::DeleteWidget(UIWIDGET* widget)
{
  if(!widget)
    {
      return false;
    }

  delete widget;

  return true;
}
