
/*------------------------------------------------------------------------------------------
//  UIWIDGETSPINBOX.CPP
//
//  handles a spinbox integer/float
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 02/11/2015 12:18:34
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIManager.h"
#include "UIWidgetCreatorHelper.h"
#include "UIWidgetSpinBox.h"

#include "UIWidgetImage.h"
#include "UIWidgetButton.h"
#include "UIWidgetText.h"
#include "UIWidgetLayoutFitParent.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::UIWIDGETSPINBOX
*/
/**
//
//  Class Constructor UIWIDGETSPINBOX
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/10/2016 16:02:49
//
//  @param        name :
//  @param        uimanagerarg :
*/
/*-----------------------------------------------------------------*/
UIWIDGETSPINBOX::UIWIDGETSPINBOX(XCHAR* name, UIMANAGER * uimanagerarg) : UIWIDGETFORM(name, uimanagerarg)
{
  Clean();
  minimum = -1000000.0;
  maximum = 1000000.0;
  delta   = 1.0;
  value   = 0.0;

  // need some kind of event warning about size changes to position the different child widgets

  // create child widgets and their structure
  background = new UIWIDGETIMAGE(name, uimanager);
  background->SetImage(__L(""));
  //
  ////// nned to create the image widgets for the up and down widgets
  up = new UIWIDGETBUTTON(name, uimanager);
  upimage = new UIWIDGETIMAGE(name, uimanager);
  UIWIDGETLAYOUTFITPARENT* upfitparent = new UIWIDGETLAYOUTFITPARENT(name, uimanager);
  up->AttachLayout(upfitparent);
  up->AddChild(upfitparent);
  up->SubstituteNormal(upimage);
  upfitparent->AddChild(upimage);

  down = new UIWIDGETBUTTON(name, uimanager);
  downimage = new UIWIDGETIMAGE(name, uimanager);
  UIWIDGETLAYOUTFITPARENT* downfitparent = new UIWIDGETLAYOUTFITPARENT(name, uimanager);
  down->AttachLayout(downfitparent);
  down->AddChild(downfitparent);
  downfitparent->AddChild(downimage);

  text = new UIWIDGETTEXT(name, uimanager);
  text->SetText(__L("0"));

  this->AddChild(background);
  this->AddChild(text);
  this->AddChild(up);
  this->AddChild(down);
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::UIWIDGETSPINBOX
*/
/**
//
//  Class Constructor UIWIDGETSPINBOX
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/02/2017 11:29:56
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETSPINBOX::UIWIDGETSPINBOX(UIWIDGETSPINBOX* rhs) : UIWIDGETFORM(rhs)
{
  Clean();

  int index = rhs->childrenbyindex.Find(rhs->background);
  if(index != NOTFOUND)
    {
      background = (UIWIDGETIMAGE*)childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->text);
  if(index != NOTFOUND)
    {
      text = (UIWIDGETTEXT*)childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->up);
  if(index != NOTFOUND)
    {
      up = (UIWIDGETBUTTON*)childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->down);
  if(index != NOTFOUND)
    {
      down = (UIWIDGETBUTTON*)childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->upimage);
  if(index != NOTFOUND)
    {
      upimage = (UIWIDGETIMAGE*)childrenbyindex.FastGet(index);
    }

  index = rhs->childrenbyindex.Find(rhs->downimage);
  if(index != NOTFOUND)
    {
      downimage = (UIWIDGETIMAGE*)childrenbyindex.FastGet(index);
    }

  isinteger = rhs->isinteger;
  value = rhs->value;
  delta = rhs->delta;
  minimum = rhs->minimum;
  maximum = rhs->maximum;

  contentmodified = rhs->contentmodified;

  R = rhs->R;
  G = rhs->G;
  B = rhs->B;

  bold = rhs->bold;
  italic = rhs->italic;


  spinboxstyle = rhs->spinboxstyle;
}

/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::~UIWIDGETSPINBOX
*/
/**
//
//   Class Destructor UIWIDGETSPINBOX
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/10/2016 16:03:18
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETSPINBOX::~UIWIDGETSPINBOX()
{
  Clean();
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::IsInteger
*/
/**
//
//  checks if it's limited to integers
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:31:24
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETSPINBOX::IsInteger()
{
  return isinteger;
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetIsInteger
*/
/**
//
//  sets if it's limited to integers
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:31:36
//
//  @param        isinteger :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetIsInteger(bool isinteger)
{
  this->isinteger = isinteger;
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetDelta
*/
/**
//
//  sets the delta for increments and decrements
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:31:51
//
//  @param        delta :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetDelta(double delta)
{
  this->delta = delta;

  if(isinteger)
    {
      this->delta = (XDWORD)this->delta;
    }
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::GetDelta
*/
/**
//
//  gets the delta for increments and decrements
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:32:05
//
//  @return       double :
//
*/
/*-----------------------------------------------------------------*/
double UIWIDGETSPINBOX::GetDelta()
{
  return delta;
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetValue
*/
/**
//
//  sets the spinbox value
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:32:17
//
//  @param        value :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetValue(double value)
{
  this->value = value;
  if(isinteger)
    {
      this->value = (XDWORD)this->value;
    }

  contentmodified = true;
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::GetValue
*/
/**
//
//  gets the spinbox value
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:32:28
//
//  @return       double :
//
*/
/*-----------------------------------------------------------------*/
double UIWIDGETSPINBOX::GetValue()
{
  return value;
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::Increment
*/
/**
//
//  increments the spinbox value by delta
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:32:43
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::Increment()
{
  value += delta;
  if(value < minimum)
    {
      value = minimum;
    }

  if(value > maximum)
    {
      value = maximum;
    }

  contentmodified = true;
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::Decrement
*/
/**
//
//  decrements the spinbox value by delta
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:33:03
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::Decrement()
{
  value -= delta;

  if(value < minimum)
    {
      value = minimum;
    }

  if(value > maximum)
    {
      value = maximum;
    }

  contentmodified = true;
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetRange
*/
/**
//
//  sets the spinbox range
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:33:15
//
//  @param        minimum :
//  @param        maximum :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetRange(double minimum, double maximum)
{
  if(minimum < maximum)
    {
      this->minimum = minimum;
      this->maximum = maximum;

      if(isinteger)
        {
          this->minimum = (XDWORD)this->minimum;
          this->maximum = (XDWORD)this->maximum;
        }
    }
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::GetMinimum
*/
/**
//
//  gets the spinbox minimum value
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:33:28
//
//  @return       double :
//
*/
/*-----------------------------------------------------------------*/
double UIWIDGETSPINBOX::GetMinimum()
{
  return minimum;
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::GetMaximum
*/
/**
//
//  gets the spinbox maximum value
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/11/2015 12:33:40
//
//  @return       double :
//
*/
/*-----------------------------------------------------------------*/
double UIWIDGETSPINBOX::GetMaximum()
{
  return maximum;
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetUpImages
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/10/2016 10:54:09
//
//  @param        normal :
//  @param        hover :
//  @param        clicked :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetUpImages(XCHAR* normal, XCHAR* hover, XCHAR* clicked)
{
  UIWIDGETBUTTON* button = (UIWIDGETBUTTON*)uimanager->GetUIWIdgetCreator()->CreateButton(__L(""), normal, hover, clicked);
  uimanager->AddToDeleteQueue(this->up);
  this->up = button;
  this->AddChild(button);
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetDownImages
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/10/2016 10:54:03
//
//  @param        normal :
//  @param        hover :
//  @param        clicked :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetDownImages(XCHAR* normal, XCHAR* hover, XCHAR* clicked)
{
  UIWIDGETBUTTON* button = (UIWIDGETBUTTON*)uimanager->GetUIWIdgetCreator()->CreateButton(__L(""), normal, hover, clicked);
  uimanager->AddToDeleteQueue(this->down);
  this->down = button;
  this->AddChild(button);
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetBackgroundImages
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/10/2016 10:53:57
//
//  @param        background :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetBackgroundImages(XCHAR* background)
{
  if(this->background)
    {
      this->background->SetImage(background);
    }
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetColorR
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/10/2016 10:51:08
//
//  @param        R :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetColor(XBYTE R, XBYTE G, XBYTE B)
{
  this->R = R;
  if(text)
    {
      text->SetColor(R, G, B);
    }
}




/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetBold
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/10/2016 10:50:03
//
//  @param        bold :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetBold(bool bold)
{
  this->bold = bold;
  if(text)
    {
      text->SetBold(bold);
    }
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetItalic
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/10/2016 10:49:57
//
//  @param        italic :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetItalic(bool italic)
{
  this->italic = italic;
  if(text)
    {
      text->SetItalic(italic);
    }
}


/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/10/2016 12:17:53
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETSPINBOX::InternalUpdate(XDWORD widgetlevel)
{
  if(contentmodified)
    {
      XSTRING string;
      if(isinteger)
        {
          int v = (int)value;
          string.Format(__L("%d"), v);
        }
      else
        {
          string.Format(__L("%f"), value);
        }

      if(text)
        {
          text->SetText(string.Get());
        }
    }

  return true;
}


/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::HandleInternalEvent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/10/2016 9:37:38
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETSPINBOX::HandleInternalEvent(XEVENT* event)
{
  if(event->GetSubject() == this)
    {
      if(event->GetEventType() == XEVENTTYPE_UI)
        {
          UIEVENT* uievent = (UIEVENT*)event;

          if(spinboxstyle == UIWIDGETSPINBOX_STYLE_SPINBOX)
            {
              SetSpinboxStyle();
            }
          else if(spinboxstyle == UIWIDGETSPINBOX_STYLE_PICKERHORIZONTAL)
            {
              SetPickerStyleHorizontal();
            }
          else if(spinboxstyle == UIWIDGETSPINBOX_STYLE_PICKERVERTICAL)
            {
              SetPickerStyleVertical();
            }
        }
    }
  else if(event->GetSubject() == this->up)
    {
      if(event->GetEventType() == XEVENTTYPE_UI)
        {
          UIEVENT* uievent = (UIEVENT*)event;
          if(uievent->GetUIEventType() == UIXEVENTTYPE_CLICK)
            {
              // increase
              this->Increment();
            }
        }
    }
  else if(event->GetSubject() == this->down)
    {
      if(event->GetEventType() == XEVENTTYPE_UI)
        {
          UIEVENT* uievent = (UIEVENT*)event;
          if(uievent->GetUIEventType() == UIXEVENTTYPE_CLICK)
            {
              // decrease
              this->Decrement();
            }
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetSpinboxStyle
*/
/**
//
//  sets the spinbox style
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/10/2016 12:20:49
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetSpinboxStyle()
{
  //first buttons
  if(up)
    {
      up->SetWidth(height);
      up->SetHeight(height/2);
      up->SetPositionX(width-height);
      up->SetPositionY(0);
    }

  if(down)
    {
      down->SetWidth(height);
      down->SetHeight(height/2);
      down->SetPositionX(width-height);
      down->SetPositionY(height/2);
    }

  if(text)
    {
      text->SetHeight(height);
      text->SetWidth(width-height);
      text->SetPositionX(0);
      text->SetPositionY(0);

      text->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
    }

  if(background)
    {
      background->SetSize(width, height);
      background->SetPosition(0, 0);
    }
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetPickerStyleHorizontal
*/
/**
//
//  sets the horizontal style
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/10/2016 12:20:40
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetPickerStyleHorizontal()
{
  // now the interesting thing, scale
  // try if space for everything
  if(up)
    {
      up->SetSize(height, height);
    }

  if(down)
    {
      down->SetSize(height, height);
    }

  if(text)
    {
      text->SetSize(width - height*2, (XDWORD)(height*0.8));
    }

  if(down)
    {
      down->SetPosition(0, 0);
    }

  if(text && down)
    {
      text->SetPosition(down->GetWidth(), (XDWORD)((height*0.2)/2.0));
    }

  if(up && text)
    {
      up->SetPosition(text->GetPositionX() + text->GetWidth(), 0);
    }

  if(text)
    {
      text->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
      text->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
    }

  if(background)
    {
      background->SetSize(width, height);
      background->SetPosition(0, 0);
    }
}



/*-------------------------------------------------------------------
//  UIWIDGETSPINBOX::SetPickerStyleVertical
*/
/**
//
//  sets the vertical style
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      21/10/2016 12:20:31
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOX::SetPickerStyleVertical()
{
}
