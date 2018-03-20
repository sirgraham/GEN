
/*------------------------------------------------------------------------------------------
//	UIWIDGETEXCLUSIVECONTAINERDRAWABLE.CPP
//	
//	drawable exlcusive container
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 14/07/2016 12:33:15
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetExclusiveContainerDrawable.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
UIWIDGETEXCLUSIVECONTAINERDRAWABLE::UIWIDGETEXCLUSIVECONTAINERDRAWABLE(XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene) : UIWIDGETEXCLUSIVECONTAINER(name, uimanager)
{
	Clean();

	this->scene = scene;
}

UIWIDGETEXCLUSIVECONTAINERDRAWABLE::~UIWIDGETEXCLUSIVECONTAINERDRAWABLE()
{
	Clean();
}

