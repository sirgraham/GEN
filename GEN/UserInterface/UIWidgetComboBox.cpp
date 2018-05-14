
/*------------------------------------------------------------------------------------------
//  UIWIDGETCOMBOBOX.CPP
//
//  combo box
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 06/02/2017 11:27:15
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIManager.h"
#include "UIWidgetCreatorHelper.h"
#include "UIWidgetComboBox.h"
#include "UIWidgetButton.h"
#include "UIWidgetImage.h"
#include "UIListAdapter.h"

#include "XDebug.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::UIWIDGETCOMBOBOX
*/
/**
//
//  Class Constructor UIWIDGETCOMBOBOX
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 11:35:10
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETCOMBOBOX::UIWIDGETCOMBOBOX(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETCONTAINER(name, uimanager)
{
  Clean();
  SetType();

  // nned to create the image widgets for the up and down widgets
  combobackground = new UIWIDGETIMAGE(name, uimanager);
  combobackground->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  combobackground->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  combobackground->SetImage(__L("background_move.png"));
  this->AddChild(combobackground);

  combobutton = new UIWIDGETBUTTON(name, uimanager);
  UIWIDGETIMAGE* comboimage = new UIWIDGETIMAGE(name, uimanager);
  comboimage->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  comboimage->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  combobutton->SubstituteNormal(comboimage);
  combobutton->AddChild(comboimage);
  combobutton->SetIsSticky(true);
  this->AddChild(combobutton);
}



/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::UIWIDGETCOMBOBOX
*/
/**
//
//  Class Constructor UIWIDGETCOMBOBOX
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/02/2017 14:01:12
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETCOMBOBOX::UIWIDGETCOMBOBOX(UIWIDGETCOMBOBOX* rhs) : UIWIDGETCONTAINER(rhs)
{
  Clean();
  SetType();

  selectedindex = rhs->selectedindex;
  int index = rhs->childrenbyindex.Find(rhs->displayedwidget);
  if(index != NOTFOUND)
    {
      displayedwidget = childrenbyindex.FastGet(index);
    }

  oldlockedmouse = rhs->oldlockedmouse;
  XDWORD size = rhs->entries.GetSize();
  for(XDWORD i = 0; i < size; i++)
    {
      int index = rhs->childrenbyindex.Find(rhs->entries.FastGet(i));
      if(index != NOTFOUND)
        {
          entries.Add((UIWIDGETCONTAINER*)childrenbyindex.FastGet(index));
        }
    }
}




/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::~UIWIDGETCOMBOBOX
*/
/**
//
//   Class Destructor UIWIDGETCOMBOBOX
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 11:35:17
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETCOMBOBOX::~UIWIDGETCOMBOBOX()
{
  Clean();
}



/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::SetAdapter
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:03:56
//
//  @param        adapter :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCOMBOBOX::SetAdapter(UILISTADAPTERBASE* adapter)
{
  this->adapter = adapter;
}




/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::ApplyAdapter
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:04:02
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCOMBOBOX::ApplyAdapter()
{
  // clear all current data, but careful as we already have children !!
  XDWORD childrensize = childrenbyindex.GetSize();
  for(XDWORD i = 0; i < childrensize; i++)
    {
      UIWIDGET* w = childrenbyindex.FastGet(i);
      if((w != combobackground) && (w != combobutton))
        {
          uimanager->AddToDeleteQueue(w);
        }
    }

  // ideally here we should only have the background and the button
  // background should always be the first and the button the last
  // so, if we leave the background without modification it should still be there

  // iterate through all adapter elements and add them to the list
  XDWORD listsize = adapter->GetSize();
  for(XDWORD i = 0; i < listsize; i++)
    {
      UIWIDGETCONTAINER* w = (UIWIDGETCONTAINER*)adapter->GetWidget(i);
      this->AddChild(w);
      entries.Add(w);
      w->SetActive(false);
    }

  if(entries.GetSize() > 0)
    {
      UIWIDGETCONTAINER* w = entries.Get(selectedindex);
      if(w)
        {
          UIWIDGETCONTAINER* c= (UIWIDGETCONTAINER*)w->Clone();
          c->SetActive(true);
          c->SetParent(this);
          c->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
          c->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
          childrenbyindex.Add(c);
          displayedwidget = c;
        }
    }

  // need to insert the DropButton at the end though, and keep track of which one is the button!
  childrenbyindex.Delete(combobutton);
  childrenbyindex.Add(combobutton);
}




/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::SetActive
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:04:08
//
//  @param        active :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCOMBOBOX::SetActive(bool active)
{
  this->isactive = active; // also activete or not children
}




/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::SetUpComboButton
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:51:25
//
//  @param        normal :
//  @param        hover :
//  @param        clicked :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCOMBOBOX::SetUpComboButton(XCHAR* normal, XCHAR* hover, XCHAR* clicked)
{
  UIWIDGETBUTTON* button = (UIWIDGETBUTTON*)uimanager->GetUIWIdgetCreator()->CreateButton(__L(""), normal, hover, clicked);
  button->SetName(__L("DropButton"));
  uimanager->AddToDeleteQueue(this->combobutton);
  this->combobutton = button;
  this->AddChild(button);
  button->SetIsSticky(true);
}



/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::InternalDraw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:04:15
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCOMBOBOX::InternalDraw(XDWORD& widgetlevel)
{
  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:04:21
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCOMBOBOX::InternalUpdate(XDWORD widgetlevel)
{
  XDWORD comboentries = entries.GetSize();
  for(XDWORD i = 0; i < comboentries; i++)
    {
      UIWIDGETCONTAINER* w = (UIWIDGETCONTAINER*)entries.FastGet(i);

      if(w->GetIsActive())
        {
          if(w->CheckCursorOver())
            {
              w->ForceWidgetState(UIWIDGET_STATE_HOVERED); // if clicked we must select this widget as selected
            }
          else
            {
              w->ForceWidgetState(UIWIDGET_STATE_NONE);
            }
        }
    }

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::InternalPostUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:04:28
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCOMBOBOX::InternalPostUpdate(XDWORD widgetlevel)
{
  // if we locked the mouse we must check if we are hovering or selecting any of our children!!

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::UnRegisterOwnEvents
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:04:34
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCOMBOBOX::UnRegisterOwnEvents()
{
}




/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::RegisterOwnEvents
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:04:40
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCOMBOBOX::RegisterOwnEvents()
{
}



/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::HandleInternalEvent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 12:04:48
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCOMBOBOX::HandleInternalEvent(XEVENT* event)
{
  // need to get mouse event and lock it to us
  bool lockedmouse = false;
  if(event->GetEventType() == XEVENTTYPE_UI)
    {
      UIEVENT* uievent = (UIEVENT*)event;
      if(uievent->GetSender() == combobutton)
        {
          if(uievent->GetUIEventType() == UIXEVENTTYPE_CHAGESTATE)
            {
              if(uievent->GetWidgetState() & UIWIDGET_STATE_TRIGGERED)
                {
                  uimanager->AquireMouse(this);
                  lockedmouse = true;

                  // [1, n-1] must be set to active
                  XDWORD size = entries.GetSize();
                  if(size > 2)
                    {
                      for(XDWORD i = 0; i < size; i++)
                        {
                          entries.FastGet(i)->SetActive(true);
                        }
                    }
                }
            }
        }
    }

  if(!lockedmouse)
    {
      if(uimanager->GetMouseOwner() == this)
        {
          // look for mosue trigger
          // we are getting mosue left trigger on the next frame(but not always)
          if(uimanager->GetInput()->IsTriggered(INPBUTTON_ID_MOUSE_LEFT) && !oldlockedmouse)
            {
              XDWORD comboentries = entries.GetSize();
              for(XDWORD i = 0; i < comboentries; i++)
                {
                  UIWIDGETCONTAINER* w = (UIWIDGETCONTAINER*)entries.FastGet(i);

                  if(w->GetIsActive())
                    {
                      if(w->CheckCursorOver())
                        {
                          w->ForceWidgetState(UIWIDGET_STATE_HOVERED); // if clicked we must select this widget as selected

                          selectedindex = i;
                          // now remove the displayed widget and place the new one
                          //RemoveChildPointer(displayedwidget);
                          uimanager->AddToDeleteQueue(displayedwidget);
                          displayedwidget = NULL;
                          displayedwidget = entries.Get(i)->Clone();
                          displayedwidget->SetActive(true);
                          displayedwidget->SetParent(this);
                          ((UIWIDGETCONTAINER*)displayedwidget)->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
                          ((UIWIDGETCONTAINER*)displayedwidget)->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
                          childrenbyindex.Add(displayedwidget);
                          childrenbyindex.Delete(combobutton);
                          childrenbyindex.Add(combobutton);
                        }
                      else
                        {
                          w->ForceWidgetState(UIWIDGET_STATE_NONE);
                        }
                    }
                }

              uimanager->ReleaseMouse(this);
              combobutton->ForceWidgetState(UIWIDGET_STATE_NONE);
              // [1, n-1] must be removed from active
              XDWORD size = childrenbyindex.GetSize(); // better, place as active only those that are not the saved ones
              for(XDWORD i = 0; i < size; i++)
                {
                  UIWIDGET* w = childrenbyindex.FastGet(i);
                  if((w != combobackground) && (w != combobutton) && (w != displayedwidget))
                    {
                      childrenbyindex.FastGet(i)->SetActive(false);
                    }
                }
            }
        }
    }

  oldlockedmouse = lockedmouse;

  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETCOMBOBOX::InternalArrangeChildren
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 14:25:14
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCOMBOBOX::InternalArrangeChildren()
{
  // set the button size
  UIWIDGETCONTAINER::InternalArrangeChildren();

  combobutton->adjustedx        = adjustedwidth - adjustedheight;
  combobutton->adjustedy        = 0;
  combobutton->adjustedwidth    = adjustedheight;
  combobutton->adjustedheight   = adjustedheight;
  combobutton->isadjusted       = true;

  XDWORD size = entries.GetSize();
  XDWORD yoffset = adjustedheight;
  for(XDWORD i = 0; i < size; i++)
    {
      UIWIDGETCONTAINER* w = entries.FastGet(i);

      w->adjustedx = 0;
      w->adjustedy = yoffset;
      w->adjustedwidth = adjustedwidth;
      w->adjustedheight = adjustedheight;
      w->isadjusted = true;

      yoffset += adjustedheight;
    }
}
