
/*------------------------------------------------------------------------------------------
//  UIWIDGETLISTCONTAINER.CPP
//
//  widget linear container
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 28/06/2016 13:04:18
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIManager.h"
#include "UIListAdapter.h"
#include "UIWidgetListContainer.h"
#include "UIMotion.h"

#include "GRPScene.h"
#include "GRPScreen.h"
#include "GRPViewport.h"




/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::UIWIDGETLINEARCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETLINEARCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:12:59
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETLISTCONTAINER::UIWIDGETLISTCONTAINER(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETCONTAINER(name, uimanager)
{
  Clean();

  widgettype = UI_WIDGET_TYPE_LIST;
}



/*-------------------------------------------------------------------
//  UIWIDGETLISTCONTAINER::UIWIDGETLISTCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETLISTCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/02/2017 10:59:37
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETLISTCONTAINER::UIWIDGETLISTCONTAINER(UIWIDGETLISTCONTAINER* rhs) : UIWIDGETCONTAINER(rhs)
{
  Clean();
  SetType();

  scrollmode      = rhs->scrollmode;
  scroll          = rhs->scroll;

  usepxmargin     = rhs->usepxmargin;
  usedipmargin    = rhs->usedipmargin;

  pxmargin        = rhs->pxmargin;
  dipmargin       = rhs->dipmargin;
  // adapter not ehere as we cloned the children and we don't really use the stored adapter
}


/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::~UIWIDGETLINEARCONTAINER
*/
/**
//
//   Class Destructor UIWIDGETLINEARCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:13:08
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETLISTCONTAINER::~UIWIDGETLISTCONTAINER()
{
  Clean();
}





/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::GetScrollMode
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:13:21
//
//  @return       UIWIDGETLINEARCONTAINER_SCROLL :
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETLISTCONTAINER_SCROLL UIWIDGETLISTCONTAINER::GetScrollMode()
{
  return this->scrollmode;
}




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::SetScrollMode
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:13:29
//
//  @param        scrollmode :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::SetScrollMode(UIWIDGETLISTCONTAINER_SCROLL scrollmode)
{
  this->scrollmode = scrollmode;
}




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::GetScrolling
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:13:39
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
XDWORD UIWIDGETLISTCONTAINER::GetScrolling()
{
  return this->scroll;
}





/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::SetScrolling
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:14:23
//
//  @param        scroll :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::SetScrolling(XDWORD scroll)
{
  this->scroll = scroll;
  this->modified = true;
}




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::SetPxMargin
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      30/06/2016 9:33:26
//
//  @param        pxmargin :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::SetPxMargin(XDWORD pxmargin)
{
  this->pxmargin = pxmargin;
  usedipmargin = false;
  usepxmargin = true;
}




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::GetPxMargin
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      30/06/2016 9:33:33
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD UIWIDGETLISTCONTAINER::GetPxMargin()
{
  return pxmargin;
}




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::SetDIPMargin
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      30/06/2016 9:33:40
//
//  @param        dipmargin :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::SetDIPMargin(float dipmargin)
{
  this->dipmargin = dipmargin;
  usepxmargin = false;
  usedipmargin = true;
}




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::GetDIPMargin
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      30/06/2016 9:33:47
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float UIWIDGETLISTCONTAINER::GetDIPMargin()
{
  return dipmargin;
}



/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::InternalDraw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:14:33
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETLISTCONTAINER::InternalDraw(XDWORD& widgetlevel)
{
  return true;
}





/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:14:43
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETLISTCONTAINER::InternalUpdate(XDWORD widgetlevel)
{
  //// need to fix the coordinates of all the children to place them appropiately on the list
  //// (but we need the ending point of a widget to compute the starting point of the next)
  //if(!scene) return false;
  //

  XDWORD size = childrenbyindex.GetSize();
  XDWORD finalcoordx = 0;
  XDWORD finalcoordy = 0;

  if(scrollmode == UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL)
    {
      //finalcoordx = margin/2.0f;
      finalcoordx -= scroll;
    }
  else if(scrollmode == UIWIDGETLISTCONTAINER_SCROLL_VERTICAL)
    {
      //finalcoordy = margin/2.0f;
      finalcoordy -= scroll;
    }

  // ?? COULD THIS WORK ??
  for(XDWORD i = 0; i < size; i++)
    {
      UIWIDGETCONTAINER* widget = (UIWIDGETCONTAINER*)childrenbyindex.FastGet(i);

      widget->SetPosition(finalcoordx, finalcoordy);

      if(scrollmode == UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL)
        {
          finalcoordx += widget->GetWidth();
        }
      else if(scrollmode == UIWIDGETLISTCONTAINER_SCROLL_VERTICAL)
        {
          finalcoordy += widget->GetHeight();
          finalcoordy += pxmargin;
        }
    }

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETLISTCONTAINER::InternalPostUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      01/08/2016 15:59:26
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETLISTCONTAINER::InternalPostUpdate(XDWORD widgetlevel)
{
  //float tmpscalex = oldgpt1x - oldgpt0x;
  //float tmpscaley = oldgpt1y - oldgpt0y;
  //
  //gpt0x = oldgpt0x;
  //gpt0y = oldgpt0y;
  //gpt1x = gpt0x + tmpscalex;
  //gpt1y = gpt0y + tmpscaley;

  return true;
}


/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::UnRegisterOwnEvents
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:14:51
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::UnRegisterOwnEvents()
{
}




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::RegisterOwnEvents
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:14:58
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::RegisterOwnEvents()
{
}




/*-------------------------------------------------------------------
//  UIWIDGETLINEARCONTAINER::HandleInternalEvent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/06/2016 13:15:05
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETLISTCONTAINER::HandleInternalEvent(XEVENT* event)
{
  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETLISTCONTAINER::SetActive
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/07/2016 13:42:04
//
//  @param        active :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::SetActive(bool active)
{
  UIWIDGETCONTAINER::SetActive(active);

  XDWORD children = childrenbyindex.GetSize();

  for(XDWORD i = 0; i < children; i++)
    {
      childrenbyindex.FastGet(i)->SetActive(active);
    }
}



/*-------------------------------------------------------------------
//  UIWIDGETLISTCONTAINER::SetAdapter
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/10/2016 9:18:18
//
//  @param        adapter :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::SetAdapter(UILISTADAPTERBASE* adapter)
{
  this->adapter = adapter;
}


/*-------------------------------------------------------------------
//  UIWIDGETLISTCONTAINER::ApplyAdapter
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/10/2016 9:19:54
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETLISTCONTAINER::ApplyAdapter()
{
  // clear all current data
  XDWORD childrensize = childrenbyindex.GetSize();
  for(XDWORD i = 0; i < childrensize; i++)
    {
      UIWIDGET* w = childrenbyindex.FastGet(i);
      uimanager->AddToDeleteQueue(w);
    }

  // iterate through all adapter elements and add them to the list
  XDWORD listsize = adapter->GetSize();
  for(XDWORD i = 0; i < listsize; i++)
    {
      UIWIDGET* w = adapter->GetWidget(i);
      this->AddChild(w);
    }
}



/*-------------------------------------------------------------------
//  UIWIDGETLISTCONTAINER::ComputeDesiredSize
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      16/06/2017 12:13:43
//
//  @return       GRPPOINT :
//
*/
/*-----------------------------------------------------------------*/
GRPPOINT UIWIDGETLISTCONTAINER::ComputeDesiredSize()
{
  GRPPOINT p, np;
  //p = UIWIDGETCONTAINER::ComputeDesiredSize();
  p.x = (float)width;
  p.y = (float)height;

  XDWORD size = childrenbyindex.GetSize();

  for(XDWORD i = 0; i < size; i++)
    {
      UIWIDGET* w = childrenbyindex.FastGet(i);
      GRPPOINT wp = w->ComputeDesiredSize();
      np += wp;
      np.y += pxmargin;
    }

  p.x = __MAX(p.x, np.x);
  p.y = __MAX(p.y, np.y);

  desiredsize = p;

  return p;
}
