
/*------------------------------------------------------------------------------------------
//	UIWIDGETBUTTONDRAWABLE.CPP
//	
//	draws a button
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 26/10/2015 9:27:27
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetButtonDrawable.h"
#include "UIWidgetFormDrawable.h"
#include "UIWidgetText.h"
#include "UIManager.h"
#include "UIInput.h"
#include "UIEvent.h"
#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPScene.h"
#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPFontManager.h"
#include "GRPViewport.h"
#include "GRPTextBox.h"
#include "GRP2DCollisionDetection.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	UIWIDGETBUTTONDRAWABLE::UIWIDGETBUTTONDRAWABLE
*/	
/**	
//	
//	Class Constructor UIWIDGETBUTTONDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:31:16
//	
//  @param				name : 
//  @param				uimanagerarg : 
 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETBUTTONDRAWABLE::UIWIDGETBUTTONDRAWABLE(XCHAR* name, UIMANAGER * uimanagerarg, GRPSCENE* scene) : UIWIDGETBUTTON(name, uimanagerarg)
{
	Clean();

	this->scene = scene;

	button = new GRPOBJECT();
	button->SetNode(scene->CreateNode());

	buttonback = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	buttonback->SetNode(scene->CreateNode());
	buttonback->GetNode()->LinkTo(button->GetNode());
	button->GetNode()->Init();
	buttonback->GetNode()->Init();
	buttonback->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));

	GRPMATERIAL * materialproc = new GRPMATERIAL();
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.8f, 0.0f, 0.8f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	//this->scene->GetMaterialLibrary()->Add(materialproc);
	GRPMATERIALLIBRARY::Get()->Add(materialproc);

	button->SetMaterial(materialproc);
	button->GetNode()->SetMaterial(materialproc);
	buttonback->GetNode()->SetMaterial(materialproc);

	button->AddElement(buttonback);

	scene->Add(button);
}


UIWIDGETBUTTONDRAWABLE::~UIWIDGETBUTTONDRAWABLE()
{
	// delete own elements for graphics
	delete button;

	scene->GetObjects()->Delete(button);

	Clean();
}



/*-------------------------------------------------------------------
//	UIWIDGETBUTTONDRAWABLE::InternalDraw
*/	
/**	
//	
//	draws the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:31:07
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTONDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
	if(modified)
		{
			float scalex = gpt1x - gpt0x;
			float scaley = gpt1y - gpt0y;
	
			GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x, gpt0y, 0.0f);
			scaley /= scene->GetViewport()->GetAspectRatio();
	
			float x = pt.x + scalex;
			float y = pt.y - scaley;
	
			button->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
			button->GetNode()->SetScale(scalex, scaley, 1.0f);
			button->GetNode()->Update();
			button->GetNode()->UpdateWorldMatrix();
		}
	
	button->GetElements()->Get(0)->SetRenderPriority(widgetlevel);
	button->GetElements()->Get(0)->Publish(GRPMSG_ORDER);
	
	if(widgetnormal)
		{
			widgetnormal->SetActive(true);
			buttonback->SetVisible(false);
		}
	else
		{
			buttonback->SetVisible(true);
		}
	
	if(widgethover)
		{
			widgethover->SetActive(false);
		}
	
	if(widgetpressed)
		{
			widgetpressed->SetActive(false);
		}
	
	
	// probably better to consider hovering as a separate state that has a separate image? but implies a knowledge of what's going to be displayed
	// need to consider this, although this function is the drawing one after all, you might want to modify it in that case anyways
	if(widgetstate & UIWIDGET_STATE_HOVERED)
		{
			if(widgethover)
				{
					widgethover->SetActive(true);
					if(widgetnormal)
						{
							widgetnormal->SetActive(false);
							buttonback->SetVisible(false);
						}
				}
		}
	
	if(widgetstate & UIWIDGET_STATE_PRESSED)
		{
			if(widgethover)
				{
					widgethover->SetActive(false);
				}
	
			if(widgetnormal)
				{
					widgetnormal->SetActive(false);
					buttonback->SetVisible(false);
				}
	
			if(widgetpressed)
				{
					widgetpressed->SetActive(true);
				}
		}

	return true;
}





/*-------------------------------------------------------------------
//	UIWIDGETBUTTONDRAWABLE::InternalUpdate
*/	
/**	
//	
//	button behavior
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:30:55
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTONDRAWABLE::InternalUpdate (XDWORD widgetlevel)
{
	// #Imanol: Need to simplify this function as it could be repeated plenty of times
	// (few elements will be dragable and those that are most will be completely so)
	// unlike a window that has a top bar
	UIWIDGETBUTTON::InternalUpdate(widgetlevel);

	return true;
}






/*-------------------------------------------------------------------
//	UIWIDGETBUTTONDRAWABLE::SetStateWidgets
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/06/2016 11:58:47
//	
//  @param				widgetnormal : 
//  @param				widgethover : 
//  @param				widgetpressed : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETBUTTONDRAWABLE::SetStateWidgets(UIWIDGET* widgetnormal, UIWIDGET* widgethover, UIWIDGET* widgetpressed)
{
	if(widgetnormal)
		{
			//AddChild(widgetnormal);
			this->widgetnormal = widgetnormal;
		}

	if(widgethover)
		{
			//AddChild(widgethover);
			this->widgethover = widgethover;
		}

	if(widgetpressed)
		{
			//AddChild(widgetpressed);
			this->widgetpressed = widgetpressed;
		}
}




/*-------------------------------------------------------------------
//	UIWIDGETBUTTONDRAWABLE::SubstituteNormal
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/06/2016 10:52:08
//	
//	@return 			bool : 
//
//  @param				widgetnormal : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTONDRAWABLE::SubstituteNormal(UIWIDGET* widgetnormal)
{
	modified = true;

	if(!this->widgetnormal)
		{
			this->widgetnormal = widgetnormal;
			return this->AddChild(widgetnormal);
		}

	int index = childrenbyindex.Find(this->widgetnormal);

	if(index == NOTFOUND)
		{
			return false; // should not be possible at this stage
		}

	//childrenbyindex.Delete(this->widgetnormal);
	this->RemoveChildPointer(this->widgetnormal);
	delete this->widgetnormal; // enough?

	this->widgetnormal = widgetnormal;
	AddChild(widgetnormal);
	childrenbyindex.Delete(widgetnormal);

	childrenbyindex.Insert(index, widgetnormal);

	//widgetnormal->SetParent(this);

	return true;
}





/*-------------------------------------------------------------------
//	UIWIDGETBUTTONDRAWABLE::SubstituteHover
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/06/2016 10:52:00
//	
//	@return 			bool : 
//
//  @param				widgethover : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTONDRAWABLE::SubstituteHover(UIWIDGET* widgethover)
{
	modified = true;

	if(!this->widgethover)
		{
			this->widgethover = widgethover;
			return this->AddChild(widgethover);
		}

	int index = childrenbyindex.Find(this->widgethover);
	if(index == NOTFOUND)
		{
			return false;
		}

	//childrenbyindex.Delete(this->widgethover);
	this->RemoveChildPointer(this->widgethover);
	delete this->widgethover;

	this->widgethover = widgethover;
	AddChild(widgethover);
	childrenbyindex.Delete(widgethover);

	childrenbyindex.Insert(index, widgethover);

	//widgethover->SetParent(this);

	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETBUTTONDRAWABLE::SubstitutePressed
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/06/2016 10:51:51
//	
//	@return 			bool : 
//
//  @param				widgetpressed : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETBUTTONDRAWABLE::SubstitutePressed(UIWIDGET* widgetpressed)
{
	modified = true;

	if(!this->widgetpressed)
		{
			this->widgetpressed = widgetpressed;
			return this->AddChild(widgetpressed);
		}

	int index = childrenbyindex.Find(this->widgetpressed);
	if(index == NOTFOUND)
		{
			return false;
		}

	//childrenbyindex.Delete(this->widgetpressed);
	this->RemoveChildPointer(this->widgetpressed);
	delete this->widgetpressed;

	this->widgetpressed = widgetpressed;
	AddChild(widgetpressed);
	childrenbyindex.Delete(widgetpressed);

	childrenbyindex.Insert(index, widgetpressed);

	//widgetpressed->SetParent(this);

	return true;
}


void UIWIDGETBUTTONDRAWABLE::SetActive(bool active)
{
	UIWIDGETBUTTON::SetActive(active);

	// now set active to elements that are child ?
	this->SetHidden(!active);
	// need to set all child widgets as not active

	button->SetVisible(active);

	widgetnormal->SetActive(active);
	widgethover->SetActive(active);
	widgetpressed->SetActive(active);
}
