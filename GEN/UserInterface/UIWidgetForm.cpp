
/*------------------------------------------------------------------------------------------
//  UIWIDGETFORM.CPP
//
//  base widget class with size and drawable
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 08/10/2015 10:04:29
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"
#include "XDebug.h"

#include "UIManager.h"
#include "UIWidgetForm.h"
#include "UIWidget.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  UIWIDGETWINDOW::InternalDraw
*/
/**
//
//  Draw function for this specific widget
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      05/10/2015 9:16:07
//
//  @return       bool : true if succeeded
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETFORM::InternalDraw(XDWORD& widgetlevel)
{
  return false;
}

/*-------------------------------------------------------------------
//  UIWIDGETWINDOW::InternalUpdate
*/
/**
//
//  Update function for this specific widget
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      05/10/2015 9:16:21
//
//  @return       bool : true if succeeded
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETFORM::InternalUpdate(XDWORD widgetlevel)
{
  return true;
}

/*-------------------------------------------------------------------
//  UIWIDGETFORM::HandleInternalEvent
*/
/**
//
//  this function eats the pushbutton click event
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/10/2015 10:34:33
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETFORM::HandleInternalEvent(XEVENT* event)
{
  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETFORM::SetActive
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/07/2016 16:22:29
//
//  @param        active :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETFORM::SetActive(bool active)
{
  UIWIDGETCONTAINER::SetActive(active);

  XDWORD children = childrenbyindex.GetSize();

  for(XDWORD i = 0; i < children; i++)
    {
      childrenbyindex.FastGet(i)->SetActive(active);
    }
}

