
/*------------------------------------------------------------------------------------------
//  UIWIDGETEXCLUSIVECONTAINER.CPP
//
//  widget exclusive container
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 14/07/2016 11:36:40
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetExclusiveContainer.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  UIWIDGETEXCLUSIVECONTAINER::UIWIDGETEXCLUSIVECONTAINER
*/
/**
//
//  Class Constructor UIWIDGETEXCLUSIVECONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/07/2016 12:11:18
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETEXCLUSIVECONTAINER::UIWIDGETEXCLUSIVECONTAINER(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETFORM(name, uimanager)
{
  Clean();
  SetIsClickable(false);
}


/*-------------------------------------------------------------------
//  UIWIDGETEXCLUSIVECONTAINER::UIWIDGETEXCLUSIVECONTAINER
*/
/**
//
//  Class Constructor UIWIDGETEXCLUSIVECONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/02/2017 10:46:26
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETEXCLUSIVECONTAINER::UIWIDGETEXCLUSIVECONTAINER(UIWIDGETEXCLUSIVECONTAINER* rhs) : UIWIDGETFORM(rhs)
{
  Clean();

  int index = rhs->childrenbyindex.Find(rhs->currentwidget);
  if(index != NOTFOUND)
    {
      currentwidget = childrenbyindex.FastGet(index);
    }
}


/*-------------------------------------------------------------------
//  UIWIDGETEXCLUSIVECONTAINER::~UIWIDGETEXCLUSIVECONTAINER
*/
/**
//
//   Class Destructor UIWIDGETEXCLUSIVECONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/07/2016 12:11:26
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETEXCLUSIVECONTAINER::~UIWIDGETEXCLUSIVECONTAINER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  UIWIDGETEXCLUSIVECONTAINER::SetActive
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/07/2016 12:11:34
//
//  @param        active :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETEXCLUSIVECONTAINER::SetActive(bool active)
{
  UIWIDGETFORM::SetActive(active);
}




/*-------------------------------------------------------------------
//  UIWIDGETEXCLUSIVECONTAINER::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/07/2016 12:11:41
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETEXCLUSIVECONTAINER::InternalUpdate(XDWORD widgetlevel)
{
  return true;
}





/*-------------------------------------------------------------------
//  UIWIDGETEXCLUSIVECONTAINER::HandleInternalEvent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/07/2016 12:11:50
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETEXCLUSIVECONTAINER::HandleInternalEvent(XEVENT* event)
{
  // check event, if UI and it's a widget that got pressed check our children
  // otherwise ignore, but if it is unset old widget and set new one
  if(event->GetEventType() == XEVENTTYPE_UI)
    {
      UIEVENT* uievent = (UIEVENT*)event;

      if(uievent->GetUIEventType() == UIXEVENTTYPE_CHAGESTATE)
        {
          if(uievent->GetSender() != currentwidget)
            {
              if(uievent->GetWidgetState() & UIWIDGET_STATE_PRESSED)
                {
                  UIWIDGET* widget = uievent->GetSender();

                  if(widget != currentwidget)
                    {
                      if(!currentwidget)
                        {
                          currentwidget = widget;
                        }
                      else
                        {
                          // set the widget state(make it unpress)
                          currentwidget->ForceWidgetState(UIWIDGET_STATE_NONE);
                          currentwidget = widget;
                        }
                    }
                }
            }
          else
            {
              currentwidget->ForceWidgetState(UIWIDGET_STATE_PRESSED);
            }
        }
    }

  return true;
}
