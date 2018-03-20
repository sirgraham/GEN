
/*------------------------------------------------------------------------------------------
//	UIWIDGETTOASTDRAWABLE.CPP
//	
//	draws a toast
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 11/07/2016 16:23:06
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetToastDrawable.h"
	
#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPNode.h"
#include "GRPScene.h"
#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPViewport.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	UIWIDGETTOASTDRAWABLE::UIWIDGETTOASTDRAWABLE
*/	
/**	
//	
//	Class Constructor UIWIDGETTOASTDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 16:27:59
//	
//  @param				name : 
//  @param				uimanager : 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETTOASTDRAWABLE::UIWIDGETTOASTDRAWABLE(XCHAR* name, UIMANAGER * uimanager, GRPSCENE* scene) : UIWIDGETTOAST(name, uimanager)
{
	Clean();

	this->scene = scene;

	obj = new GRPOBJECT();
	if(!obj) return;
	
	obj->SetNode(scene->CreateNode());
	
	element = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	element->SetNode(scene->CreateNode());
	element->GetNode()->LinkTo(obj->GetNode());
	obj->GetNode()->Init();
	element->GetNode()->Init();
	element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	
	// make material with a dummy image? could be
	// create the image material
	GRPMATERIAL * materialproc = new GRPMATERIAL();
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(materialproc);
	
	obj->AddElement(element);
	
	obj->SetMaterial(materialproc);
	
	scene->Add(obj);
}




/*-------------------------------------------------------------------
//	UIWIDGETTOASTDRAWABLE::~UIWIDGETTOASTDRAWABLE
*/	
/**	
//	
//	 Class Destructor UIWIDGETTOASTDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 16:28:04
//	
*/
/*-----------------------------------------------------------------*/
UIWIDGETTOASTDRAWABLE::~UIWIDGETTOASTDRAWABLE()
{
	//delete obj;
	//scene->GetObjects()->Delete(obj);

	Clean();
}




/*-------------------------------------------------------------------
//	UIWIDGETTOASTDRAWABLE::InternalDraw
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 16:28:11
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTOASTDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
	if(modified)
		{
			float scalex = gpt1x - gpt0x;
			float scaley = gpt1y - gpt0y;
	
			GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x, gpt0y, 0.0f);
			scaley /= scene->GetViewport()->GetAspectRatio();
	
			float x = pt.x + scalex;
			float y = pt.y - scaley;
	
			obj->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
			obj->GetNode()->SetScale(scalex, scaley, 1.0f);
			obj->GetNode()->Update();
			obj->GetNode()->UpdateWorldMatrix();
			element->GetNode()->Update();
			element->GetNode()->UpdateWorldMatrix();
	
			//imageobject->GetElements()->Get(0)->Publish(GRPMSG_ORDER);
			element->Publish(GRPMSG_ORDER);
		}

	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETTOASTDRAWABLE::InternalUpdate
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 16:28:17
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTOASTDRAWABLE::InternalUpdate(XDWORD widgetlevel)
{
	UIWIDGETTOAST::InternalUpdate(widgetlevel);

	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETTOASTDRAWABLE::SetActive
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 16:28:23
//	
//  @param				active : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETTOASTDRAWABLE::SetActive(bool active)
{
	UIWIDGET::SetActive(active);

	if(element)
	{
		element->SetVisible(active);
	}
}
