
/*------------------------------------------------------------------------------------------
//  UIWIDGETSCISSORCONTAINER.CPP
//
//  implements a scissor container
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 13/09/2016 13:24:41
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetScissorContainer.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETSCISSORCONTAINER::UIWIDGETSCISSORCONTAINER
*/
/**
//
//  Class Constructor UIWIDGETSCISSORCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 10:00:43
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETSCISSORCONTAINER::UIWIDGETSCISSORCONTAINER(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETCONTAINER(name, uimanager)
{
  Clean();

  this->widgettype = UI_WIDGET_TYPE_SCISSORCONTAINER;
}


UIWIDGETSCISSORCONTAINER::UIWIDGETSCISSORCONTAINER(UIWIDGETSCISSORCONTAINER* rhs) : UIWIDGETCONTAINER(rhs)
{
  Clean();
  this->widgettype = UI_WIDGET_TYPE_SCISSORCONTAINER;
}


/*-------------------------------------------------------------------
//  UIWIDGETSCISSORCONTAINER::~UIWIDGETSCISSORCONTAINER
*/
/**
//
//   Class Destructor UIWIDGETSCISSORCONTAINER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 10:00:35
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETSCISSORCONTAINER::~UIWIDGETSCISSORCONTAINER()
{
  Clean();
}



/*-------------------------------------------------------------------
//  UIWIDGETSCISSORCONTAINER::InternalArrangeChildren
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 10:00:30
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSCISSORCONTAINER::InternalArrangeChildren()
{
  UIWIDGETCONTAINER::InternalArrangeChildren();
}
