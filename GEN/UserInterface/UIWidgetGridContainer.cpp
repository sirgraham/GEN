
/*------------------------------------------------------------------------------------------
//  UIWIDGETGRIDCONTAINER.CPP
//
//  grid container
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 21/02/2017 9:43:36
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetGridContainer.h"
#include "UIListAdapter.h"
#include "UIManager.h"

#include "UIWidgetScissorContainer.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETGRIDCONTAINER::UIWIDGETGRIDCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETGRIDCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/02/2017 9:54:20
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETGRIDCONTAINER::UIWIDGETGRIDCONTAINER(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETCONTAINER(name, uimanager)
{
  Clean();
  SetType();

  //scissor = new UIWIDGETSCISSORCONTAINER(__L("Scissor"), uimanager);
  //scissor->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  //scissor->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  //this->AddChild(scissor);
}



/*-------------------------------------------------------------------
//  UIWIDGETGRIDCONTAINER::UIWIDGETGRIDCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETGRIDCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/02/2017 9:54:27
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETGRIDCONTAINER::UIWIDGETGRIDCONTAINER(UIWIDGETGRIDCONTAINER* rhs) : UIWIDGETCONTAINER(rhs)
{
  SetType();

  margins = rhs->margins;
}



/*-------------------------------------------------------------------
//  UIWIDGETGRIDCONTAINER::~UIWIDGETGRIDCONTAINER
*/
/**
//
//   Class Destructor UIWIDGETGRIDCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/02/2017 9:54:33
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETGRIDCONTAINER::~UIWIDGETGRIDCONTAINER()
{
  Clean();
}



/*-------------------------------------------------------------------
//  UIWIDGETGRIDCONTAINER::ApplyAdapter
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/02/2017 9:54:39
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETGRIDCONTAINER::ApplyAdapter()
{
  if(listadapter)
    {
      // clear all current data
      XDWORD childrensize = childrenbyindex.GetSize();
      for(XDWORD i = 0; i < childrensize; i++)
        {
          UIWIDGET* w = childrenbyindex.FastGet(i);
          //if(w != scissor)
          {
            uimanager->AddToDeleteQueue(w);
          }
        }

      // iterate through all adapter elements and add them to the list
      XDWORD listsize = listadapter->GetSize();
      for(XDWORD i = 0; i < listsize; i++)
        {
          UIWIDGET* w = listadapter->GetWidget(i);
          this->AddChild(w);
        }
    }
}




/*-------------------------------------------------------------------
//  UIWIDGETGRIDCONTAINER::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/02/2017 10:27:26
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETGRIDCONTAINER::InternalUpdate(XDWORD widgetlevel)
{
  //// need to fix the coordinates of all the children to place them appropiately on the list
  //// (but we need the ending point of a widget to compute the starting point of the next)
  //if(!scene) return false;
  //

  XDWORD size = childrenbyindex.GetSize();
  XDWORD finalcoordx = margins;
  XDWORD finalcoordy = margins;
  XDWORD currentwidth = margins;

  // place children on a grid with margins
  // right and then down
  // need to check sizes(have own scissor?)

  for(XDWORD i = 0; i < size; i++)
    {
      UIWIDGETCONTAINER* w = (UIWIDGETCONTAINER*)childrenbyindex.FastGet(i);

      if((currentwidth + w->GetWidth()) > adjustedwidth/*GetWidth()*/)
        {
          finalcoordx = margins;
          finalcoordy += w->GetHeight() + margins;
          currentwidth = margins;
        }

      // keep track of width to know when to start next row
      w->SetPosition(finalcoordx, finalcoordy);
      currentwidth += w->GetWidth();
      currentwidth += margins;
      finalcoordx = currentwidth;
    }

  if(size)
  {
    UIWIDGETCONTAINER* w= (UIWIDGETCONTAINER*)childrenbyindex.FastGet(size-1);
    finalcoordy += w->GetHeight() + margins;
    ysize = finalcoordy;
  }

  //
  //if(scrollmode == UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL)
  //  {
  //    //finalcoordx = margin/2.0f;
  //    finalcoordx -= scroll;
  //  }
  //else if(scrollmode == UIWIDGETLISTCONTAINER_SCROLL_VERTICAL)
  //  {
  //    //finalcoordy = margin/2.0f;
  //    finalcoordy -= scroll;
  //  }
  //
  //// ?? COULD THIS WORK ??
  //for(XDWORD i = 0; i < size; i++)
  //  {
  //    UIWIDGETCONTAINER* widget = (UIWIDGETCONTAINER*)childrenbyindex.FastGet(i);
  //
  //    widget->SetPosition(finalcoordx, finalcoordy);
  //
  //    if(scrollmode == UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL)
  //      {
  //        finalcoordx += widget->GetWidth();
  //      }
  //    else if(scrollmode == UIWIDGETLISTCONTAINER_SCROLL_VERTICAL)
  //      {
  //        finalcoordy += widget->GetHeight();
  //      }
  //  }

  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETGRIDCONTAINER::SetMarginDIP
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/02/2017 10:42:40
//
//  @param        margin :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETGRIDCONTAINER::SetMarginDIP(float margin)
{
  margins = (int)(margin*uimanager->GetDPI()/uidipunit);
}
