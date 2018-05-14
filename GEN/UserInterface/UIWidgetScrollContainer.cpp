
/*------------------------------------------------------------------------------------------
//  UIWIDGETSCROLLCONTAINER.CPP
//
//  scroll container
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 22/02/2017 16:09:03
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#include "UIWidgetScrollContainer.h"

#include "UIManager.h"
#include "UIMotion.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETSCROLLCONTAINER::UIWIDGETSCROLLCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETSCROLLCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      22/02/2017 16:11:27
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETSCROLLCONTAINER::UIWIDGETSCROLLCONTAINER(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETCONTAINER(name, uimanager)
{
  Clean();
  SetType();
}




/*-------------------------------------------------------------------
//  UIWIDGETSCROLLCONTAINER::UIWIDGETSCROLLCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETSCROLLCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      22/02/2017 16:11:34
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETSCROLLCONTAINER::UIWIDGETSCROLLCONTAINER(UIWIDGETSCROLLCONTAINER* rhs) : UIWIDGETCONTAINER(rhs)
{
  Clean();
  SetType();
}



/*-------------------------------------------------------------------
//  UIWIDGETSCROLLCONTAINER::~UIWIDGETSCROLLCONTAINER
*/
/**
//
//   Class Destructor UIWIDGETSCROLLCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      22/02/2017 16:11:40
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETSCROLLCONTAINER::~UIWIDGETSCROLLCONTAINER()
{
  Clean();
}




/*-------------------------------------------------------------------
//   UIWIDGETSCROLLCONTAINER::SetXScroll
*/
/**
//
//
//
//  @author   Imanol Celaya Ruiz de Alegria
//  @version

//  @return   void :
//
//  @param    int :
//
*//*-----------------------------------------------------------------*/
void UIWIDGETSCROLLCONTAINER::SetXScroll(int xscroll)
{
  if(limitx && ((desiredsize.x) > adjustedwidth))
    {
      this->xscroll = xscroll;

      // control lower bound(not scroll upwards too much)
      if(this->xscroll < -fabs((adjustedwidth - desiredsize.x)))
        {
          this->xscroll = (int)(-fabs((adjustedwidth - desiredsize.x)));
        }

      // control upper bound
      if(this->xscroll > 0)
        {
          this->xscroll = 0;
        }
    }
}



/*-------------------------------------------------------------------
//   UIWIDGETSCROLLCONTAINER::SetYScroll
*/
/**
//
//
//
//  @author   Imanol Celaya Ruiz de Alegria
//  @version

//  @return   void :
//
//  @param    int :
//
*//*-----------------------------------------------------------------*/
void UIWIDGETSCROLLCONTAINER::SetYScroll(int yscroll)
{
  if(limity && ((desiredsize.y) > adjustedheight))
    {
      this->yscroll = yscroll;

      // control lower bound(not scroll upwards too much)
      if(this->yscroll < -fabs((adjustedheight - desiredsize.y)))
        {
          this->yscroll = (int)(-fabs((adjustedheight - desiredsize.y)));
        }

      // control upper bound
      if(this->yscroll > 0)
        {
          this->yscroll = 0;
        }
    }
}


/*-------------------------------------------------------------------
//  UIWIDGETSCROLLCONTAINER::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      22/02/2017 16:46:38
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETSCROLLCONTAINER::InternalUpdate(XDWORD widgetlevel)
{

  //if(uimanager->IsInMotion())
  //  {
  //    UIMOTION* motion = uimanager->GetInput()->GetMotion();
  //
  //    float diffx = (motion->GetMotionDiffX());
  //    float diffy = (motion->GetMotionDiffY());
  //
  //    XDEBUG_PRINTCOLOR(3, __L("%f  |  %f"), diffx, diffy);
  //
  //    //if((diffx > 10) || (diffy > 10))
  //      {
  //        UIMOTION::UIMOTIONENTRY* lastentry = motion->GetLastMotion();
  //        if(lastentry->type == UIMOTION_TYPE_MOVE)
  //          {
  //            XDWORD size = motion->GetMotionNumber();
  //            if(size > 1)
  //              {
  //                SetYScroll((int)(GetYScroll() - diffy));
  //                //UIMOTION::UIMOTIONENTRY* preentry = motion->GetMotion(size - 2);
  //                //float ydelta = preentry->y - lastentry->y;
  //                //if(ydelta < 0)
  //                //  {
  //                //    SetYScroll(GetYScroll() - 5.0f);
  //                //  }
  //                //else if(ydelta > 0)
  //                //  {
  //                //    SetYScroll(GetYScroll() + 5.0f);
  //                //  }
  //              }
  //          }
  //      }
  //  }

  return true;
}




/*-------------------------------------------------------------------
//   UIWIDGETSCROLLCONTAINER::HandleInternalEvent
*/
/**
//
//
//
//  @author   Imanol Celaya Ruiz de Alegria
//  @version

//  @return   bool :
//
//  @param    XEVENT* :
//
*//*-----------------------------------------------------------------*/
bool UIWIDGETSCROLLCONTAINER::HandleInternalEvent(XEVENT* event)
{
  if(event->GetEventFamily() == XEVENTTYPE_UI)
    {
      UIEVENT* uievent = (UIEVENT*)event;
      if(uievent->GetSender() == this)
        {
          if(uievent->GetEventType() == UIXEVENTTYPE_MOTION)
            {
              UIMOTION* m = uievent->GetMotion();
              if(m->GetLastMotion()->type == UIMOTION_TYPE_MOVE)
                {
                  XDWORD size = m->GetMotionNumber();
                  UIMOTION::UIMOTIONENTRY* e0 = m->GetMotion(size - 2);
                  UIMOTION::UIMOTIONENTRY* e1 = m->GetLastMotion();
                  float xdelta = e1->x - e0->x;
                  float ydelta = e1->y - e0->y;

                  SetYScroll((int)(GetYScroll() - ydelta));
                }
            }
        }
    }

  return true;
}


/*-------------------------------------------------------------------
//  UIWIDGETSCROLLCONTAINER::InternalArrangeChildren
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      22/02/2017 17:05:10
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSCROLLCONTAINER::InternalArrangeChildren()
{
  // move children widget adjusted x and y from theit positions and our scroll values
  if(!isadjusted)
    {
      UIWIDGETCONTAINER* p = (UIWIDGETCONTAINER*)parent;
      adjustedx       = x;
      adjustedy       = y;
      adjustedwidth   = width;
      adjustedheight  = height;

      isadjusted = true;

      if(p)
        {
          if(horizontalconstraint == UIWIDGET_CONSTRAINTS_FILL_PARENT)
            {
              adjustedx = 0;
              adjustedwidth = p->adjustedwidth;//p->GetWidth();
            }

          if(verticalconstraints == UIWIDGET_CONSTRAINTS_FILL_PARENT)
            {
              adjustedy = 0;
              adjustedheight = p->adjustedheight;//p->GetHeight();
            }
        }
    }


  XDWORD size = childrenbyindex.GetSize();
  for(XDWORD i = 0; i < size; i++)
    {
      UIWIDGETCONTAINER* w = (UIWIDGETCONTAINER*)childrenbyindex.FastGet(i);
      w->ArrangeChildren();
      w->isadjusted = true;

      w->adjustedx = w->GetPositionX() + xscroll;
      w->adjustedy = w->GetPositionY() + yscroll;
    }
}
