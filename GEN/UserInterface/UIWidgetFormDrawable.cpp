
/*------------------------------------------------------------------------------------------
//	UIWIDGETFORMDRAWABLE.CPP
//	
//	draws a uiwidgetform
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 22/10/2015 11:06:14
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetFormDrawable.h"

#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPFontManager.h"
#include "GRPViewport.h"
#include "GRPScene.h"
#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPTextBox.h"
#include "GRPShape.h"
#include "GRPObjectBuilderAlgorithmTriangulationEar.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	UIWIDGETFORMDRAWABLE::UIWIDGETFORMDRAWABLE
*/	
/**	
//	
//	Class Constructor UIWIDGETFORMDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			29/10/2015 17:12:44
//	
//  @param				name : 
//  @param				uimanagerarg : 
 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETFORMDRAWABLE::UIWIDGETFORMDRAWABLE (XCHAR* name, UIMANAGER * uimanagerarg, GRPSCENE* scene) : UIWIDGETFORM(name, uimanagerarg)
{
	Clean();
	this->scene = scene;

	window = new GRPOBJECT();
	window->SetNode(this->scene->CreateNode());

	//plane = scene->renderer->GetObjectBuilder()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	GRPSPLINE* rounded = GRPOBJECTBUILDER::Get()->CreateBoxSpline(GRPPOINT(-1.0f, -1.0f, 0.0f), GRPPOINT(1.0f, 1.0f, 0.0f), 0.0f); // rounded corner
	GRPSHAPE* shape = new GRPSHAPE();
	shape->AddContour(rounded);
	GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR* tri = new GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR(GRPOBJECTBUILDER::Get());
	//GRPOBJECTBUILDERALGORITHMSTROKE* tri = new GRPOBJECTBUILDERALGORITHMSTROKE (scene->renderer->GetObjectBuilder());
	plane = GRPOBJECTBUILDER::Get()->TriangulizeShape(shape, tri);
	delete tri;

	//delete rounded;
	delete shape;

	plane->SetNode(scene->CreateNode());
	plane->GetNode()->LinkTo(window->GetNode());
	window->GetNode()->Init();
	plane->GetNode()->Init();
	
	// create and add basic material and shader
	plane->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));

	GRPMATERIAL * materialproc = new GRPMATERIAL();
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	//this->scene->GetMaterialLibrary()->Add(materialproc);
	GRPMATERIALLIBRARY::Get()->Add(materialproc);

	window->SetMaterial(materialproc);
	window->GetNode()->SetMaterial(materialproc);
	plane->GetNode()->SetMaterial(materialproc);

	
	//window->Update();

	window->AddElement(plane);
	this->scene->Add(window);


	//// create the top bar
	//topbar = scene->renderer->GetObjectBuilder()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	//topbar->SetNode(scene->CreateNode());
	//topbar->GetNode()->LinkTo(window->GetNode());
	//topbar->GetNode()->Init();
	//plane->GetNode()->Init();
	//topbar->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	//
	//materialproc = new GRPMATERIAL();
	//materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//materialproc->GetAttributes()->isupdated = true;
	//topbar->GetNode()->SetMaterial(materialproc);
	//this->scene->GetMaterialLibrary()->Add(materialproc);
	//
	//
	//window->AddElement(topbar);
	//
	//// create the close button
	//close = scene->renderer->GetObjectBuilder()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	//close->SetNode(scene->CreateNode());
	//close->GetNode()->LinkTo(window->GetNode());
	//close->GetNode()->Init();
	//close->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	//
	//materialproc = new GRPMATERIAL();
	//materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	//materialproc->GetAttributes()->isupdated = true;
	//close->GetNode()->SetMaterial(materialproc);
	//this->scene->GetMaterialLibrary()->Add(materialproc);
	//
	//
	//// window title
	//
	//titledraw = new GRPTEXTBOX(scene);
	////titledraw->SetNode(scene->CreateNode());
	////titledraw->GetNode()->LinkTo(window->GetNode());
	////titledraw->GetNode()->Init();
	//titledraw->SetNode(scene->CreateNode());
	//window->GetNode()->Init();
	//
	//// this means the font must be accessible
	//GRPFONTMANAGER* fontmanager = scene->renderer->GetObjectBuilder()->GetFontManager();
	//font = fontmanager->GetFont(__L("UI"));
	//
	//
	//titledraw->SetFont(font);
	////titledraw->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	//titledraw->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));
	//
	//materialproc = new GRPMATERIAL();
	//materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//materialproc->GetAttributes()->isupdated = true;
	//titledraw->GetNode()->SetMaterial(materialproc);
	//titledraw->SetMaterial(materialproc);
	//this->scene->GetMaterialLibrary()->Add(materialproc);
	//
	//titledraw->SetContent(title.Get());
	//titledraw->MergeElements();
	//// #Imanol : NEED TO FIX THID WITH NEW GRAPHICS
	////scene->InsertNewElementInRenderOrder(titledraw->GetElements()->Get(0));
	//this->scene->Add(titledraw);
	//
	//window->AddElement(close);
}




/*-------------------------------------------------------------------
//	UIWIDGETFORMDRAWABLE::InternalDraw
*/	
/**	
//	
//	draws the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			29/10/2015 17:13:00
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETFORMDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
	//if(modified)
	//	{
	//		float scalex = gpt1x - gpt0x;
	//		float scaley = gpt1y - gpt0y;
	//
	//		// postions now need to be computed to normalized format
	//
	//		GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x, gpt0y, 0.0f);
	//
	//		//
	//		////float x = this->positionx*2.0f - 1.0f;// + this->scalex;
	//		////float y = -this->positiony*(1.0f/scene->GetViewport()->GetAspectRatio())*2.0f + 1.0f/scene->GetViewport()->GetAspectRatio();// - this->scaley;
	//		//
	//		//GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(this->positionx, this->positiony, 0.0f);
	//		//
	//		//float x = pt.x + this->scalex;
	//		//float y = pt.y - this->scaley/scene->GetViewport()->GetAspectRatio();
	//
	//		scaley /= scene->GetViewport()->GetAspectRatio();
	//
	//		float x = pt.x + scalex;
	//		float y = pt.y - scaley;
	//
	//		//
	//		////plane->GetNode()->SetPosition(this->positionx - 1.0f + this->scalex, (-this->positiony + 1.0f/scene->GetViewport()->GetAspectRatio() - this->scaley), widgetlevel + 0.0f);
	//		if(!isownvisible)
	//		{
	//			plane->GetNode()->SetVisible(false);
	//		}
	//
	//		plane->GetNode()->SetPosition(x, y, widgetlevel + 0.0f);
	//		plane->GetNode()->SetScale(scalex, scaley, 1.0f);
	//		plane->GetNode()->Update();
	//		plane->GetNode()->UpdateWorldMatrix();
	//
	//		
	//	}
	//
	//plane->Publish(GRPMSG_ORDER);
	//
	//// this scales are relative to the top window
	//// they should be independent, or change appropiately depending on it's scale
	//// no sense centering these element at top left, that's just for positionx, positiony
	//topbar->GetNode()->SetScale(this->scalex, 0.020f, 1.0f);
	//topbar->GetNode()->SetPosition(plane->GetNode()->GetPosition()->vector[0], plane->GetNode()->GetPosition()->vector[1] + this->scaley/scene->GetViewport()->GetAspectRatio() + topbar->GetNode()->GetScale()->vector[1], widgetlevel + 0.1f);
	//
	//close->GetNode()->SetScale(topbar->GetNode()->GetScale()->vector[1], topbar->GetNode()->GetScale()->vector[1], 1.0f);
	//close->GetNode()->SetPosition(plane->GetNode()->GetPosition()->vector[0] + this->scalex - close->GetNode()->GetScale()->vector[0], topbar->GetNode()->GetPosition()->vector[1], widgetlevel + 0.2f);
	//
	//float lineheight = font->GetLineHeight();
	//float scale = topbar->GetNode()->GetScale()->vector[1]/lineheight;
	//
	//titledraw->SetContent(title.Get());
	//titledraw->MergeElements();
	//scene->Add(titledraw);
	////scene->InsertNewElementInRenderOrder(titledraw->GetElements()->Get(0));
	//titledraw->GetNode()->SetPosition(topbar->GetNode()->GetPosition()->vector[0], topbar->GetNode()->GetPosition()->vector[1] - topbar->GetNode()->GetScale()->vector[1]/2.0f, widgetlevel + 0.3f);
	////titledraw->GetNode()->SetPosition(0.0f, 0.0f, 3.0f);
	//
	//titledraw->GetNode()->SetScale(scale*2.0f, scale*2.0f, 1.0f);

	plane->SetRenderPriority(widgetlevel);

	return true;
}

GRPNODE* UIWIDGETFORMDRAWABLE::GetNode()
{
	return window->GetNode();
}
