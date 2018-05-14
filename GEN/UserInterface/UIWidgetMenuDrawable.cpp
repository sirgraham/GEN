
/*------------------------------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE.CPP
//
//  Drawable menu
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 12/07/2016 9:34:11
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetMenuDrawable.h"

#include "UIWidgetListContainerDrawable.h"
#include "UIWidgetButtonDrawable.h"
#include "UIWidgetTextDrawable.h"
#include "UIWidgetImageDrawable.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::UIWIDGETMENUDRAWABLE
*/
/**
//
//  Class Constructor UIWIDGETMENUDRAWABLE
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      12/07/2016 9:47:22
//
//  @param        name :
//  @param        uimanager :
//  @param        scene :
*/
/*-----------------------------------------------------------------*/
UIWIDGETMENUDRAWABLE::UIWIDGETMENUDRAWABLE(XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene) : UIWIDGETMENU(name, uimanager)
{
  Clean();

  container = new UIWIDGETLISTCONTAINERDRAWABLE(name, uimanager, scene);
  if(!container) return;

  //container->FitParent(true);
  container->SetScrollMode(UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL);

  this->AddChild(container);

  this->scene = scene;
}




/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::~UIWIDGETMENUDRAWABLE
*/
/**
//
//   Class Destructor UIWIDGETMENUDRAWABLE
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      12/07/2016 9:47:13
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETMENUDRAWABLE::~UIWIDGETMENUDRAWABLE()
{
  Clean();


}




/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::InternalDraw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      12/07/2016 9:47:08
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETMENUDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      12/07/2016 9:47:02
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETMENUDRAWABLE::InternalUpdate(XDWORD widgetlevel)
{
  //UIWIDGETMENU::InternalUpdate(widgetlevel);
  //
  //if(orientation == UIWIDGET_MENU_ORIENTATION_HORIZONTAL)
  //  {
  //    container->SetScrollMode(UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL);
  //  }
  //else
  //  {
  //    container->SetScrollMode(UIWIDGETLISTCONTAINER_SCROLL_VERTICAL);
  //  }
  //
  //// if different number of elements than items that are added to the menu, create the necessary elements
  //
  //XDWORD itemsnum = items.GetSize();
  //
  //XDWORD loopindex = container->GetAllChildren()->GetSize();
  //
  //loopindex = addedelemenets; // required as there seems to be a mismatch between an element being added initially and the following loop
  //
  ////while(container->GetAllChildren()->GetSize() < itemsnum) // need to fix this while as it makes an infinite loop because elements are added to the container at the next uimanager update pass
  //for(; loopindex < itemsnum; loopindex++)
  //  {
  //    // create button
  //    //XSTRING* str = items.Get(container->GetAllChildren()->GetSize());
  //    XSTRING* str = items.Get(loopindex);
  //
  //    UIWIDGETBUTTONDRAWABLE* btn = new UIWIDGETBUTTONDRAWABLE(str->Get(), uimanager, scene);
  //    btn->SetISHoverable(true);
  //    btn->SetIsClickable(true);
  //
  //    btn->SetOrigin(0.0f, 0.0f);
  //    btn->SetDIPScale(150.0f, dipheight);
  //
  //    if(this->IsScalePxY())
  //      {
  //        btn->SetPxScaleY(pxheight);
  //      }
  //
  //    //btn->SetPxScale(150, 50);
  //
  //    UIWIDGETIMAGEDRAWABLE* normalimg = NULL;
  //    UIWIDGETIMAGEDRAWABLE* hoverimg = NULL;
  //    UIWIDGETIMAGEDRAWABLE* clickimg = NULL;
  //
  //    if(normal.GetSize())
  //      {
  //        normalimg = new UIWIDGETIMAGEDRAWABLE(__L("normal"), uimanager, scene);
  //        normalimg->FitParent(true);
  //        normalimg->SetImage(normal.Get());
  //      }
  //
  //    if(hover.GetSize())
  //      {
  //        hoverimg = new UIWIDGETIMAGEDRAWABLE(__L("hover"), uimanager, scene);
  //        hoverimg->FitParent(true);
  //        hoverimg->SetImage(hover.Get());
  //      }
  //
  //    if(clicked.GetSize())
  //      {
  //        clickimg = new UIWIDGETIMAGEDRAWABLE(__L("clicked"), uimanager, scene);
  //        clickimg->FitParent(true);
  //        clickimg->SetImage(clicked.Get());
  //      }
  //
  //    btn->SetStateWidgets(normalimg, hoverimg, clickimg);
  //
  //    UIWIDGETTEXTDRAWABLE* txt = new UIWIDGETTEXTDRAWABLE(str->Get(), uimanager, scene);
  //    txt->SetText(str->Get());
  //    txt->SetOrigin(0.1f, 0.1f);
  //    txt->SetEndingPoint(0.9f, 0.9f);
  //    txt->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
  //    txt->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
  //    btn->AddChild(txt);
  //
  //    container->AddChild(btn);
  //    addedelemenets++;
  //  }

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::SetActive
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      12/07/2016 9:46:55
//
//  @param        active :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETMENUDRAWABLE::SetActive(bool active)
{
  UIWIDGET::SetActive(active);

  if(container)
  {
    container->SetActive(active);
  }
}




/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::SetMenuOrientation
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/07/2016 12:10:16
//
//  @param        orientation :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETMENUDRAWABLE::SetMenuOrientation(UIWIDGET_MENU_ORIENTATION orientation)
{
  this->orientation = orientation;
}






/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::GetMenuOrientation
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/07/2016 12:10:22
//
//  @return       UIWIDGET_MENU_ORIENTATION :
//
*/
/*-----------------------------------------------------------------*/
UIWIDGET_MENU_ORIENTATION UIWIDGETMENUDRAWABLE::GetMenuOrientation()
{
  return this->orientation;
}


/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::SetButtonsImages
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      12/07/2016 13:07:33
//
//  @param        normal :
//  @param        hover :
//  @param        clicked :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETMENUDRAWABLE::SetButtonsImages(XCHAR* normal, XCHAR* hover, XCHAR* clicked)
{
  if(normal)
    {
      this->normal.Set(normal);
    }

  if(hover)
    {
      this->hover.Set(hover);
    }

  if(clicked)
    {
      this->clicked.Set(clicked);
    }
}




/*-------------------------------------------------------------------
//  UIWIDGETMENUDRAWABLE::HandleInternalEvent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/07/2016 12:30:33
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETMENUDRAWABLE::HandleInternalEvent(XEVENT* event)
{
  //bool issubmenu = !UIWIDGETMENU::HandleInternalEvent(event);
  //
  //if(issubmenu)
  //  {
  //    // submenu got called, so we need to show it and place it correctly
  //    UIEVENT* uievent = (UIEVENT*)event;
  //
  //    // find the child
  //    XDWORD children = childrenbyindex.GetSize();
  //    for(XDWORD i = 0; i < children; i++)
  //      {
  //        if(childrenbyindex.FastGet(i)->GetName()->Compare(uievent->GetSender()->GetName()->Get()) == 0)
  //          {
  //            UIWIDGETMENUDRAWABLE* submenu = (UIWIDGETMENUDRAWABLE*)childrenbyindex.FastGet(i);
  //
  //            // place it in the correct position, but wait, we could be getting the button? nope, because that one is a child of the container
  //            UIWIDGETBUTTONDRAWABLE* button = (UIWIDGETBUTTONDRAWABLE*)container->GetChildByName(uievent->GetSender()->GetName());
  //
  //            // need to compute position depending on current orientation
  //
  //            float inix, iniy;
  //            float finalx , finaly;
  //            button->GetFinalPoint(finalx, finaly);
  //            button->GetOrigin(inix, iniy);
  //
  //            if(orientation == UIWIDGET_MENU_ORIENTATION_HORIZONTAL)
  //              {
  //                //submenu->SetOrigin(inix, finaly);
  //                //submenu->SetOrigin(0.0f, finaly); // this does nothing, even if changed by hand(but X seems to work correctly)
  //                submenu->SetOrigin(inix, button->gpt1y); // this does nothing, even if changed by hand(but X seems to work correctly)
  //              }
  //            else
  //              {
  //                submenu->SetOrigin(finalx, iniy);
  //              }
  //
  //            if(submenu->GetIsActive())
  //              {
  //                submenu->SetActive(false);
  //              }
  //            else
  //              {
  //                submenu->SetActive(true);
  //              }
  //        }
  //      }
  //  }

  return true;
}
