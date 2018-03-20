
/*------------------------------------------------------------------------------------------
//	UIWIDGETLISTCONTAINERDRAWABLE.CPP
//	
//	Draws a Linear Container
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 30/06/2016 9:47:58
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetListContainerDrawable.h"
	
#include "GRPOpenGL.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	UIWIDGETLISTCONTAINERDRAWABLE::UIWIDGETLISTCONTAINERDRAWABLE
*/	
/**	
//	
//	Class Constructor UIWIDGETLISTCONTAINERDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/08/2016 17:18:25
//	
//  @param				name : 
//  @param				uimanager : 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETLISTCONTAINERDRAWABLE::UIWIDGETLISTCONTAINERDRAWABLE(XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene) : UIWIDGETLISTCONTAINER(name, uimanager)
{
	Clean();

	this->scene = scene;

	//this->scissor = new GRPSCISSORELEMENT();
}




/*-------------------------------------------------------------------
//	UIWIDGETLISTCONTAINERDRAWABLE::~UIWIDGETLISTCONTAINERDRAWABLE
*/	
/**	
//	
//	 Class Destructor UIWIDGETLISTCONTAINERDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/08/2016 17:18:32
//	
*/
/*-----------------------------------------------------------------*/
UIWIDGETLISTCONTAINERDRAWABLE::~UIWIDGETLISTCONTAINERDRAWABLE()
{
	Clean();
}



/*-------------------------------------------------------------------
//	UIWIDGETLISTCONTAINERDRAWABLE::InternalDraw
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/08/2016 17:18:39
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETLISTCONTAINERDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
	//  save old parameters
	scissoractive = GRPOPENGLCTRL::SCISSOR::IsActive;
	GRPRECTINT rect = GRPOPENGLCTRL::SCISSOR::Rect;

	x0 = rect.x1;
	y0 = rect.y1;
	x1 = rect.x2;
	y1 = rect.y2;

	if(scissor)
		{
			scissor->Set(true, this->GetPositionX(), this->GetPositionY(), this->GetPositionX() + this->GetWidth(), this->GetPositionY() + GetHeight());
			scissor->SetRenderPriority(widgetlevel);
		}

	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETLISTCONTAINERDRAWABLE::InternalPostDraw
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/08/2016 17:18:46
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETLISTCONTAINERDRAWABLE::InternalPostDraw(XDWORD& widgetlevel)
{
	if(scissor)
		{
			scissor->Set(scissoractive, x0, y0, x1, y1);
		}

	return true;
}
