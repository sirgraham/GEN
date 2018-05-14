
/*------------------------------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR.CPP
//
//  implements a progress bar
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 02/11/2015 9:26:22
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetProgressBar.h"

#include "UIWidgetScissorContainer.h"
#include "UIWidgetLayoutFitParent.h"
#include "UIWidgetImage.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::UIWIDGETPROGRESSBAR
*/
/**
//
//  Class Constructor UIWIDGETPROGRESSBAR
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:30:58
//
//  @param        name :
//  @param        uimanagerarg :
*/
/*-----------------------------------------------------------------*/
UIWIDGETPROGRESSBAR::UIWIDGETPROGRESSBAR(XCHAR* name, UIMANAGER * uimanagerarg) : UIWIDGETFORM(name, uimanagerarg)
{
  Clean();

  widgettype = UI_WIDGET_TYPE_PROGRESSBAR;

  XSTRING compose;

  //fitparentbackground = new UIWIDGETLAYOUTFITPARENT(name, uimanager);
  background = new UIWIDGETIMAGE(name, uimanager);
  background->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  background->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  background->SetIsClickable(false);

  scissor  = new UIWIDGETSCISSORCONTAINER(name, uimanager);
  //fitparentprogress = new UIWIDGETLAYOUTFITPARENT(name, uimanager);
  progressbar = new UIWIDGETIMAGE(name, uimanager);

  scissor->SetIsClickable(false);
  //fitparentprogress->SetIsClickable(false);
  progressbar->SetIsClickable(false);

  // need to fix this, as ideally layouts can't jump in the hierarchy(simplify)
  this->AddChild(background);

  this->AddChild(scissor);
  //scissor->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);

  //scissor->AddChild(fitparentprogress);
  scissor->AddChild(progressbar);
  //fitparentprogress->AddChild(progressbar);

  //this->AttachLayout(fitparentprogress);

  ProgressChange();
}



/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::UIWIDGETPROGRESSBAR
*/
/**
//
//  Class Constructor UIWIDGETPROGRESSBAR
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/02/2017 11:06:48
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETPROGRESSBAR::UIWIDGETPROGRESSBAR(UIWIDGETPROGRESSBAR* rhs) : UIWIDGETFORM(rhs)
{
  Clean();
  SetType();

  progress = rhs->progress;
  progresschanged = rhs->progresschanged;

  int index = rhs->childrenbyindex.Find(rhs->scissor);
  if(index != NOTFOUND)
    {
      scissor = (UIWIDGETSCISSORCONTAINER*)childrenbyindex.FastGet(index);
    }


  index = rhs->childrenbyindex.Find(rhs->progressbar);
  if(index != NOTFOUND)
    {
      progressbar = (UIWIDGETIMAGE*)childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->background);
  if(index != NOTFOUND)
    {
      background = (UIWIDGETIMAGE*)childrenbyindex.FastGet(index);
    }

  direction = rhs->direction;
}

UIWIDGETPROGRESSBAR::~UIWIDGETPROGRESSBAR()
{


  Clean();
}

/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::GetProgress
*/
/**
//
//  gets the current progress in normalized form
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:31:20
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float UIWIDGETPROGRESSBAR::GetProgress()
{
  return progress;
}


/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::SetProgress
*/
/**
//
//  sets the current progress in normalized form
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:31:34
//
//  @param        progress :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBAR::SetProgress(float progress)
{
  this->progress = progress;

  if(progress > 1.0f)
    {
      progress = 1.0f;
    }

  if(progress < 0.0f)
    {
      progress = 0.0f;
    }

  progresschanged = true;

  ProgressChange();
}

/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::IncrementProgress
*/
/**
//
//  increments internal progress in normalized form
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:31:50
//
//  @return       float :
//
//  @param        increment :
*/
/*-----------------------------------------------------------------*/
float UIWIDGETPROGRESSBAR::IncrementProgress(float increment)
{
  progress += increment;

  if(progress > 1.0f)
    {
      progress = 1.0f;
    }

  if(progress < 0.0f)
    {
      progress = 0.0f;
    }

  progresschanged = true;

  ProgressChange();

  return progress;
}





/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::SetBackgroundImage
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/09/2016 17:05:44
//
//  @param        background :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBAR::SetBackgroundImage(XCHAR* background)
{
  this->background->SetImage(background);
}




/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::SetProgressBarImage
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/09/2016 17:05:38
//
//  @param        progressbar :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBAR::SetProgressBarImage(XCHAR* progressbar)
{
  this->progressbar->SetImage(progressbar);
}




/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::SetProgressBarDirection
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/09/2016 10:02:26
//
//  @param        direction :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBAR::SetProgressBarDirection(UIWIDGETPROGRESSBAR_DIRECTION direction)
{
  this->direction = direction;

  if(direction == UIWIDGETPROGRESSBAR_DIRECTION_HORIZONTAL)
    {
      scissor->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_NONE);
      scissor->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
    }
  else
    {
      scissor->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_NONE);
      scissor->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
    }

  ProgressChange();
}


/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::InternalDraw
*/
/**
//
//  draws the widget, or not
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:32:02
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETPROGRESSBAR::InternalDraw(XDWORD& widgetlevel)
{
  return false;
}

/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::InternalUpdate
*/
/**
//
//  updates the class if necessary
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:33:00
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETPROGRESSBAR::InternalUpdate(XDWORD widgetlevel)
{
  return true;
}

/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::UnRegisterOwnEvents
*/
/**
//
//  unregisters its own events
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:33:14
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBAR::UnRegisterOwnEvents()
{
}

/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::RegisterOwnEvents
*/
/**
//
//  registers its own events
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:33:24
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBAR::RegisterOwnEvents()
{
}



/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::InternalArrangeChildren
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/02/2017 16:39:52
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBAR::InternalArrangeChildren()
{
  UIWIDGETCONTAINER::InternalArrangeChildren();

  //// but these positions are relative to the scissor, which breaks as when we move the scissor we move the progress image
  //
  //progressbar->adjustedx = 0;
  //progressbar->adjustedy = 0;
  //
  //progressbar->adjustedwidth = adjustedwidth;
  //progressbar->adjustedheight = adjustedheight;
  //progressbar->isadjusted = true;
  if(direction == UIWIDGETPROGRESSBAR_DIRECTION_HORIZONTAL)
    {
      // scissor properties are not saved properly, getb reset into fill
      progressbar->adjustedx = 0;
      progressbar->adjustedy = 0;

      progressbar->adjustedwidth = adjustedwidth;
      progressbar->adjustedheight = adjustedheight;
      progressbar->isadjusted = true;
    }
  else
    {
      int newheight = (int)((1.0f - progress)*height);
      progressbar->adjustedx = 0;
      progressbar->adjustedy = -newheight;

      progressbar->adjustedwidth = adjustedwidth;
      progressbar->adjustedheight = adjustedheight;
      progressbar->isadjusted = true;
    }
}


/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::HandleInternalEvent
*/
/**
//
//  event handler
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 9:33:55
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETPROGRESSBAR::HandleInternalEvent(XEVENT* event)
{
  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETPROGRESSBAR::ProgressChange
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/09/2016 17:11:30
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBAR::ProgressChange()
{
  if(direction == UIWIDGETPROGRESSBAR_DIRECTION_HORIZONTAL)
    {
      scissor->SetPosition(0, 0); // change position
      XDWORD newwidth = (XDWORD)(progress*width);
      scissor->SetWidth(newwidth);
      scissor->SetHeight(height);
    }
  else
    {
      int newheight = (int)((1.0f - progress)*height);
      UIWIDGETIMAGE* progressimg = (UIWIDGETIMAGE*)scissor->GetChild(0);
      progressimg->SetPosition(0, -newheight);
      scissor->SetPosition(0, newheight); // can't be the same height as position !!
      scissor->SetWidth(width);
      scissor->SetHeight(((int)height) - newheight);
    }
}
