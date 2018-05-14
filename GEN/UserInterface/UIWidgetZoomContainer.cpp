
/*------------------------------------------------------------------------------------------
//  UIWIDGETZOOMCONTAINER.CPP
//
//  implements widgets zoom
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 08/03/2017 10:13:57
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetZoomContainer.h"
#include "UIManager.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETZOOMCONTAINER::UIWIDGETZOOMCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETZOOMCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/03/2017 10:23:11
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETZOOMCONTAINER::UIWIDGETZOOMCONTAINER(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETCONTAINER(name, uimanager)
{
  Clean();
  SetType();
}



/*-------------------------------------------------------------------
//  UIWIDGETZOOMCONTAINER::UIWIDGETZOOMCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETZOOMCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/03/2017 10:23:18
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETZOOMCONTAINER::UIWIDGETZOOMCONTAINER(UIWIDGETZOOMCONTAINER* rhs) : UIWIDGETCONTAINER(rhs)
{
  Clean();
  SetType();

  zoom  = rhs->zoom;
  zoomx = rhs->zoomx;
  zoomy = rhs->zoomy;
}



/*-------------------------------------------------------------------
//  UIWIDGETZOOMCONTAINER::~UIWIDGETZOOMCONTAINER
*/
/**
//
//   Class Destructor UIWIDGETZOOMCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/03/2017 10:23:23
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETZOOMCONTAINER::~UIWIDGETZOOMCONTAINER()
{
  Clean();
}



/*-------------------------------------------------------------------
//  UIWIDGETZOOMCONTAINER::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/03/2017 11:35:21
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETZOOMCONTAINER::InternalUpdate(XDWORD widgetlevel)
{
  UIWIDGETCONTAINER::InternalUpdate(widgetlevel);

  //// compute zoom point in terms of gpt
  gptzx = (zoomx/(float)uimanager->GetVirtualWidth())  + gpt0x;
  gptzy = (zoomy/(float)uimanager->GetVirtualHeight()) + gpt0y;

  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETZOOMCONTAINER::GetHoveredWidget
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      31/03/2017 11:10:12
//
//  @return       UIWIDGET* :
//
//  @param        force :
//  @param        zoom :
//  @param        x :
//  @param        y :
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETZOOMCONTAINER::GetHoveredWidget(bool force, float zoom, float x, float y)
{
  return UIWIDGETCONTAINER::GetHoveredWidget(force, this->zoom, gptzx, gptzy);
}
