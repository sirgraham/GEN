
/*------------------------------------------------------------------------------------------
//  UIWIDGETPUSHBUTTON.CPP
//
//  push button class
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 08/10/2015 10:04:58
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/


#include "XDebug.h"

#include "INPDevice.h"
#include "INPButton.h"

#include "UIManager.h"
#include "UIWidgetButton.h"
#include "UIWidgetText.h"
#include "UIEvent.h"

#include "UIInput.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::UIWIDGETBUTTON
*/
/**
//
//  Class Constructor UIWIDGETPUSHBUTTON
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      05/10/2015 11:04:28
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETBUTTON::UIWIDGETBUTTON(XCHAR* name, UIMANAGER * uimanagerarg) : UIWIDGETFORM(name, uimanagerarg)
{
  Clean();

  // add default button label
  /*UIWIDGETTEXT * label = dynamic_cast<UIWIDGETTEXT*>(uimanager->CreateWidget(UI_WIDGET_TYPE_TEXT, __L("ButtonText")));

  if(label)
    {
      label->SetText(__L("Button"));
      AddChild(label);
    }*/

  widgettype = UI_WIDGET_TYPE_BUTTON;
  ismouseaware = true;
  reactstomouse = true;
}




/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::UIWIDGETBUTTON
*/
/**
//
//  Class Constructor UIWIDGETBUTTON
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/02/2017 17:41:48
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETBUTTON::UIWIDGETBUTTON(UIWIDGETBUTTON* rhs) : UIWIDGETFORM(rhs)
{
  Clean();

  // assign same indexes as on rhs to widgets

  int index = rhs->childrenbyindex.Find(rhs->widgetnormal);
  if(index != NOTFOUND)
    {
      widgetnormal = childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->widgethover);
  if(index != NOTFOUND)
    {
      widgethover = childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->widgetpressed);
  if(index != NOTFOUND)
    {
      widgetpressed = childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->activewidget);
  if(index != NOTFOUND)
    {
      activewidget = childrenbyindex.FastGet(index);
    }

  issticky = rhs->issticky;
}



/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::Clone
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      15/06/2017 11:48:11
//
//  @return       UIWIDGET* :
//
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETBUTTON::Clone()
{
  return new UIWIDGETBUTTON(this);
}


/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::SetActive
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/05/2017 10:09:47
//
//  @param        active :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETBUTTON::SetActive(bool active)
{
  UIWIDGETFORM::SetActive(active);

  if(widgetnormal)
    {
      widgetnormal->SetActive(false);
    }

  if(widgethover)
    {
      widgethover->SetActive(false);
    }

  if(widgetpressed)
    {
      widgetpressed->SetActive(false);
    }

  if(activewidget)
    {
      activewidget->SetActive(active);
    }
}

/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::SetStateWidgets
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/08/2016 17:14:34
//
//  @param        NULL :
//  @param        NULL :
//  @param        NULL :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETBUTTON::SetStateWidgets(UIWIDGET* widgetnormal, UIWIDGET* widgethover, UIWIDGET* widgetpressed)
{
  if(widgetnormal)
    {
      //AddChild(widgetnormal);
      this->widgetnormal = widgetnormal;
    }

  if(widgethover)
    {
      //AddChild(widgethover);
      this->widgethover = widgethover;
    }

  if(widgetpressed)
    {
      //AddChild(widgetpressed);
      this->widgetpressed = widgetpressed;
    }
}




/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::SubstituteNormal
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/08/2016 17:14:28
//
//  @return       bool :
//
//  @param        widgetnormal :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTON::SubstituteNormal(UIWIDGET* widgetnormal)
{
  // REDO !!

  if(!this->widgetnormal)
    {
      this->widgetnormal = widgetnormal;
      //return this->AddChild(widgetnormal);
    }

  int index = childrenbyindex.Find(this->widgetnormal);

  if(index == NOTFOUND)
    {
      return false; // should not be possible at this stage
    }

  //childrenbyindex.Delete(this->widgetnormal);
  this->RemoveChildPointer(this->widgetnormal);
  delete this->widgetnormal; // enough?

  this->widgetnormal = widgetnormal;
  AddChild(widgetnormal);
  childrenbyindex.Delete(widgetnormal);

  childrenbyindex.Insert(index, widgetnormal);

  //widgetnormal->SetParent(this);

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::SubstituteHover
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/08/2016 17:14:21
//
//  @return       bool :
//
//  @param        widgethover :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTON::SubstituteHover(UIWIDGET* widgethover)
{
  if(!this->widgethover)
    {
      this->widgethover = widgethover;
      return this->AddChild(widgethover);
    }

  int index = childrenbyindex.Find(this->widgethover);
  if(index == NOTFOUND)
    {
      return false;
    }

  //childrenbyindex.Delete(this->widgethover);
  this->RemoveChildPointer(this->widgethover);
  delete this->widgethover;

  this->widgethover = widgethover;
  if(widgethover)
    {
      AddChild(widgethover);
      childrenbyindex.Delete(widgethover);

      childrenbyindex.Insert(index, widgethover);
    }
  //widgethover->SetParent(this);

  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::SubstitutePressed
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/08/2016 17:14:14
//
//  @return       bool :
//
//  @param        widgetpressed :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTON::SubstitutePressed(UIWIDGET* widgetpressed)
{
    if(!this->widgetpressed)
    {
      this->widgetpressed = widgetpressed;
      return this->AddChild(widgetpressed);
    }

  int index = childrenbyindex.Find(this->widgetpressed);
  if(index == NOTFOUND)
    {
      return false;
    }

  //childrenbyindex.Delete(this->widgetpressed);
  this->RemoveChildPointer(this->widgetpressed);
  delete this->widgetpressed;

  this->widgetpressed = widgetpressed;

  if(widgetpressed)
    {
      AddChild(widgetpressed);
      childrenbyindex.Delete(widgetpressed);

      childrenbyindex.Insert(index, widgetpressed);
    }
  //widgetpressed->SetParent(this);

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::CheckCursorOver
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      04/10/2016 13:40:26
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTON::CheckCursorOver(float zoom, int x, int y)
{
  if(activewidget)
    {
      UIWIDGETCONTAINER::CheckCursorOver();
      return ((UIWIDGETCONTAINER*)activewidget)->CheckCursorOver();
    }

  return UIWIDGETCONTAINER::CheckCursorOver();
}



/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::InternalDraw
*/
/**
//
//  Draws the button
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      05/10/2015 10:38:27
//
//  @return       bool : true if succeeded
//
//  @param        widgetlevel : level of the widget in the hierarchy
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTON::InternalDraw(XDWORD& widgetlevel)
{
  if(widgetnormal)
    {
      widgetnormal->SetActive(true);
      activewidget = widgetnormal;
    }

  if(widgethover)
    {
      widgethover->SetActive(false);
    }

  if(widgetpressed)
    {
      widgetpressed->SetActive(false);
    }


  // probably better to consider hovering as a separate state that has a separate image? but implies a knowledge of what's going to be displayed
  // need to consider this, although this function is the drawing one after all, you might want to modify it in that case anyways
  if(widgetstate & UIWIDGET_STATE_HOVERED)
    {
      if(widgethover)
        {
          widgethover->SetActive(true);
          activewidget = widgethover;
          if(widgetnormal)
            {
              widgetnormal->SetActive(false);
            }
        }
      else
        {
          if(widgetnormal)
            {
              widgetnormal->SetActive(true);
            }
        }
    }

  if(widgetstate & UIWIDGET_STATE_PRESSED)
    {
      if(widgethover)
        {
          widgethover->SetActive(false);
        }

      if(widgetnormal)
        {
          widgetnormal->SetActive(false);
        }

      if(widgetpressed)
        {
          widgetpressed->SetActive(true);
          activewidget = widgetpressed;
        }
      else
        {
          if(widgetnormal)
            {
              widgetnormal->SetActive(true);
            }
        }
    }

  return true;
}

/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::InternalUpdate
*/
/**
//
//  updates and sends the button events
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      05/10/2015 10:38:41
//
//  @return       bool : true is succeeded
//
//  @param        widgetlevel : level of the widget in the hierarchy
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTON::InternalUpdate(XDWORD widgetlevel)
{


  return true;
}

/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::RegisterOwnEvents
*/
/**
//
//  registers the events of this widget in the tree
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/10/2015 9:27:52
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETBUTTON::RegisterOwnEvents()
{
  //RegisterOwnEvent(XEVENTTYPE_UI);
}

/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::UnRegisterOwnEvents
*/
/**
//
//  unregisters the events of this widget in the tree
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/10/2015 9:28:11
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETBUTTON::UnRegisterOwnEvents()
{
  //UnRegisterOwnEvent(XEVENTTYPE_UI);
}



/*-------------------------------------------------------------------
//  UIWIDGETBUTTON::HandleInternalEvent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 15:50:58
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTON::HandleInternalEvent(XEVENT* event)
{
  return true;
}
