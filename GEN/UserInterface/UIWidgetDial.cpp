
/*------------------------------------------------------------------------------------------
//  UIWIDGETDIAL.CPP
//
//  dial widget
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 20/03/2017 10:12:34
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#include "XBase.h"

#include "UIWidgetDial.h"
#include "UIWidgetImage.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETDIAL::UIWIDGETDIAL
*/
/**
//
//  Class Constructor UIWIDGETDIAL
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/03/2017 10:21:30
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETDIAL::UIWIDGETDIAL(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETFORM(name, uimanager)
{
  Clean();
  SetType();
}



/*-------------------------------------------------------------------
//  UIWIDGETDIAL::UIWIDGETDIAL
*/
/**
//
//  Class Constructor UIWIDGETDIAL
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/03/2017 10:21:37
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETDIAL::UIWIDGETDIAL(UIWIDGETDIAL* rhs) : UIWIDGETFORM(rhs)
{
  Clean();
  SetType();

  // copy widget in here
  XDWORD size = rhs->childrenbyindex.GetSize();
  int index = rhs->childrenbyindex.Find(rhs->dial);
  if(index != NOTFOUND)
    {
      dial = (UIWIDGETIMAGE*)childrenbyindex.FastGet(index);
    }
}



/*-------------------------------------------------------------------
//  UIWIDGETDIAL::~UIWIDGETDIAL
*/
/**
//
//   Class Destructor UIWIDGETDIAL
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/03/2017 10:21:42
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETDIAL::~UIWIDGETDIAL()
{
  Clean();
}



/*-------------------------------------------------------------------
//  UIWIDGETDIAL::SetDialWidget
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/03/2017 10:39:14
//
//  @param        dialwidget :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETDIAL::SetDialWidget(UIWIDGETIMAGE* dialwidget)
{
  if(dial != NULL)
    {
      // remove old dial
      RemoveChildPointer(dial);
      dial = NULL;
    }

  // add new one
  dial = dialwidget;
  AddChild(dial);
}



/*-------------------------------------------------------------------
//  UIWIDGETDIAL::InternalArrangeChildren
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/03/2017 10:47:25
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETDIAL::InternalArrangeChildren()
{
  // get to the function that allows me to rotate though the widget center
  UIWIDGETCONTAINER::InternalArrangeChildren();

  float rx = x + adjustedx/2.0f;
  float ry = y + adjustedy/2.0f;

  float ox = (float)x;
  float oy = (float)y;

  float rox = ox - rx;
  float roy = oy - ry;

  // rotate vector, remember that cos and sin need radians
  // cos - sin
  // sin + cos
  float roxp = rox*cos(DEGREE2RADIAN(rotation)) - roy*sin(DEGREE2RADIAN(rotation));
  float royp = rox*sin(DEGREE2RADIAN(rotation)) + roy*cos(DEGREE2RADIAN(rotation));

  // children are rx + roxp | ry + ryxp
  dial->adjustedx = (int)(rx + roxp);
  dial->adjustedy = (int)(rx + royp);
  dial->adjustedwidth = adjustedwidth;
  dial->adjustedheight = adjustedheight;
  dial->isadjusted = true;
}
