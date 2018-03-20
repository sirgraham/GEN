
/*------------------------------------------------------------------------------------------
//	UIWIDGETDIALOGDRAWABLE.CPP
//	
//	dialog drawable
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 08/07/2016 14:04:02
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetDialogDrawable.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
UIWIDGETDIALOGDRAWABLE::UIWIDGETDIALOGDRAWABLE(XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene) : UIWIDGETDIALOG(name, uimanager)
{
	Clean();

	this->scene = scene;
}

UIWIDGETDIALOGDRAWABLE::~UIWIDGETDIALOGDRAWABLE()
{
	Clean();
}

