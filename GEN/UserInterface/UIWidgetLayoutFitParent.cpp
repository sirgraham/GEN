
/*------------------------------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT.CPP
//
//  layout that fits its parent size
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 03/08/2016 16:59:37
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetLayoutFitParent.h"
#include "UIManager.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::UIWIDGETLAYOUTFITPARENT
*/
/**
//
//  Class Constructor UIWIDGETLAYOUTFITPARENT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/08/2016 17:01:03
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETLAYOUTFITPARENT::UIWIDGETLAYOUTFITPARENT(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETLAYOUT(name, uimanager)
{
  Clean();
}



/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::~UIWIDGETLAYOUTFITPARENT
*/
/**
//
//   Class Destructor UIWIDGETLAYOUTFITPARENT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/08/2016 17:01:10
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETLAYOUTFITPARENT::~UIWIDGETLAYOUTFITPARENT()
{
  Clean();
}


/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::HandleEvent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/08/2016 17:01:15
//
//  @param        xevent :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETLAYOUTFITPARENT::HandleInternalEvent(XEVENT* xevent)
{
  if(xevent->GetSubject() == this)
    {
      return true;
    }

  if(xevent->GetEventType() == XEVENTTYPE_UI)
    {
      UIEVENT* uievent = (UIEVENT*)xevent;

      if(uievent->GetUIEventType() == UIXEVENTTYPE_RESIZE)
        {
          UIWIDGETCONTAINER* sender = (UIWIDGETCONTAINER*)uievent->GetSender();

          if(sender)
            {
              // resize own widget as our parent got resized;
              this->SetPosition(0, 0);
              this->SetSize(sender->GetWidth(), sender->GetHeight()); // will probably have to fix this according to layout position with the window? or are attached widget always at 0,0?

              // make the child get our own size

              XDWORD widgets = childrenbyindex.GetSize();

              for(XDWORD i = 0; i < widgets; i++)
                {
                  UIWIDGETCONTAINER* widget = (UIWIDGETCONTAINER*)childrenbyindex.FastGet(i);
                  if(widget)
                    {
                      if(positionconstrains == UIWIDGETLAYOUTFITPARENT_CONSTRAINS_ALL)
                        {
                          widget->SetPosition(this->GetPositionX(), this->GetPositionY());
                        }
                      else if(positionconstrains == UIWIDGETLAYOUTFITPARENT_CONSTRAINS_VERTICAL)
                        {
                          widget->SetPosition(widget->GetPositionX(), this->GetPositionY());
                        }
                      else if(positionconstrains == UIWIDGETLAYOUTFITPARENT_CONSTRAINS_HORIZONTAL)
                        {
                          widget->SetPosition(this->GetPositionX(), widget->GetPositionY());
                        }

                      if(scaleconstrains == UIWIDGETLAYOUTFITPARENT_CONSTRAINS_ALL)
                        {
                          widget->SetSize(this->GetWidth(), this->GetHeight());
                        }
                      else if(scaleconstrains == UIWIDGETLAYOUTFITPARENT_CONSTRAINS_VERTICAL)
                        {
                          widget->SetSize(widget->GetWidth(), this->GetHeight());
                        }
                      else if(scaleconstrains == UIWIDGETLAYOUTFITPARENT_CONSTRAINS_HORIZONTAL)
                        {
                          widget->SetSize(this->GetWidth(), widget->GetHeight());
                        }

                      widget->ComputeCoords();
                    }
                }
            }

          return false;
        }
    }

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::AddChild
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/08/2016 17:40:05
//
//  @return       bool :
//
//  @param        child :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETLAYOUTFITPARENT::AddChild(UIWIDGET* child)
{
  // handle layout specific child owning in here(like only keeping one)

  // better a nope
  //if(childrenbyindex.GetSize())
  //  {
  //    XDWORD size = childrenbyindex.GetSize();
  //    for(XDWORD i = 0; i < size; i++)
  //      {
  //        uimanager->AddToDeleteQueue(childrenbyindex.FastGet(i));
  //      }
  //  }

  UIWIDGETCONTAINER::AddChild(child);

  // manually set child size now
  ((UIWIDGETCONTAINER*)child)->SetPosition(this->GetPositionX(), this->GetPositionY());
  ((UIWIDGETCONTAINER*)child)->SetSize(this->GetWidth(), this->GetHeight());

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::SetPositionConstrains
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/09/2016 12:04:13
//
//  @param        constrains :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLAYOUTFITPARENT::SetPositionConstrains(UIWIDGETLAYOUTFITPARENT_CONSTRAINS positionconstrains)
{
  this->positionconstrains = positionconstrains;
}



/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::GetPositionConstrains
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/09/2016 12:04:06
//
//  @return       UIWIDGETLAYOUTFITPARENT_CONSTRAINS :
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETLAYOUTFITPARENT_CONSTRAINS UIWIDGETLAYOUTFITPARENT::GetPositionConstrains()
{
  return this->positionconstrains;
}



/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::SetScaleConstrains
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/09/2016 12:04:13
//
//  @param        constrains :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLAYOUTFITPARENT::SetScaleConstrains(UIWIDGETLAYOUTFITPARENT_CONSTRAINS scaleconstrains)
{
  this->scaleconstrains = scaleconstrains;
}



/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::GetScaleConstrains
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/09/2016 12:04:06
//
//  @return       UIWIDGETLAYOUTFITPARENT_CONSTRAINS :
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETLAYOUTFITPARENT_CONSTRAINS UIWIDGETLAYOUTFITPARENT::GetScaleConstrains()
{
  return this->scaleconstrains;
}



/*-------------------------------------------------------------------
//  UIWIDGETLAYOUTFITPARENT::InternalArrangeChildren
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/02/2017 17:10:39
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLAYOUTFITPARENT::InternalArrangeChildren()
{
  UIWIDGETCONTAINER* p = (UIWIDGETCONTAINER*)parent; // if we are a child and have no parent is not a container something is very very wrong

  if(p)
    {
      adjustedx       = 0;
      adjustedy       = 0;
      adjustedwidth   = p->GetWidth();
      adjustedheight  = p->GetHeight();
    }
}
