
/*------------------------------------------------------------------------------------------
//  UIWIDGET3DMODEL.CPP
//
//  shows a 3D model as a widget
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 13/02/2017 17:14:07
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidget3DModel.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGET3DMODEL::UIWIDGET3DMODEL
*/
/**
//
//  Class Constructor UIWIDGET3DMODEL
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/02/2017 17:16:12
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGET3DMODEL::UIWIDGET3DMODEL(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETFORM(name, uimanager)
{
  Clean();
  SetType();
}



/*-------------------------------------------------------------------
//  UIWIDGET3DMODEL::UIWIDGET3DMODEL
*/
/**
//
//  Class Constructor UIWIDGET3DMODEL
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/02/2017 17:16:19
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGET3DMODEL::UIWIDGET3DMODEL(UIWIDGET3DMODEL* rhs) : UIWIDGETFORM(rhs)
{
  Clean();
  SetType();

  modelname = rhs->modelname;
}



/*-------------------------------------------------------------------
//  UIWIDGET3DMODEL::~UIWIDGET3DMODEL
*/
/**
//
//   Class Destructor UIWIDGET3DMODEL
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/02/2017 17:16:24
//
*/
/*-----------------------------------------------------------------*/
UIWIDGET3DMODEL::~UIWIDGET3DMODEL()
{
  Clean();
}
