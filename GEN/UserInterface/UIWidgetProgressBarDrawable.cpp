
/*------------------------------------------------------------------------------------------
//	UIWIDGETPROGRESSBARDRAWABLE.CPP
//	
//	draws a progress bar
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 02/11/2015 9:51:12
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetProgressBarDrawable.h"
#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPScene.h"
#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPViewport.h"

#include "GRPScreen.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	UIWIDGETPROGRESSBARDRAWABLE::UIWIDGETPROGRESSBARDRAWABLE
*/	
/**	
//	
//	Class Constructor UIWIDGETPROGRESSBARDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:31:28
//	
//  @param				name : 
//  @param				uimanagerarg : 
 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETPROGRESSBARDRAWABLE::UIWIDGETPROGRESSBARDRAWABLE(XCHAR* name, UIMANAGER * uimanagerarg, GRPSCENE* scene) : UIWIDGETPROGRESSBAR(name, uimanagerarg)
{
	Clean();
	this->scene = scene;
	obj = new GRPOBJECT();

	obj->SetNode(scene->CreateNode());

	background = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	background->SetNode(scene->CreateNode());
	background->GetNode()->LinkTo(obj->GetNode()); // these links are redundant?
	background->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	// Init nodes?

	GRPMATERIAL * materialproc = new GRPMATERIAL();
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(materialproc);
	background->GetNode()->SetMaterial(materialproc);

	obj->AddElement(background);

	bar = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	bar->SetNode(scene->CreateNode());
	bar->GetNode()->LinkTo(obj->GetNode());
	bar->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	
	materialproc = new GRPMATERIAL();
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.2f, 0.6f, 0.2f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(materialproc);
	bar->GetNode()->SetMaterial(materialproc);
	
	obj->AddElement(bar);

	scene->Add(obj);
}





/*-------------------------------------------------------------------
//	UIWIDGETPROGRESSBARDRAWABLE::InternalDraw
*/	
/**	
//	
//	Draws the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:31:56
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETPROGRESSBARDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
	if(!imagesloaded)
		{
			imagesloaded = true;
	
			GRPFILEBITMAP*  grpfilebitmap = NULL;
			grpfilebitmap = new GRPFILEBITMAP();
			//grpfilebase = new GRPFILEBITMAPTGA();
	
			XPATH xpath;
			xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,backgroundimagename.Get());
			GRPBITMAP* bitmap = grpfilebitmap->Load(xpath);
	
			backgroundimage = new GRPTEXTURE();
			backgroundimage->Create(bitmap);
	
			xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,foregroundimagename.Get());
			bitmap = grpfilebitmap->Load(xpath);
	
			foregroundimage = new GRPTEXTURE();
			foregroundimage->Create(bitmap);
	
			delete grpfilebitmap;
	
			// create and set the materials for the progress bar elements
			background->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
			bar->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
	
			background->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, backgroundimage);
			GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&backgroundimagename, backgroundimage);
	
			bar->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, foregroundimage);
			GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&foregroundimagename, foregroundimage);
		}
	
	if(progresschanged)
		{
			XVECTOR<GRPFACE*>* faces = bar->GetMesh()->GetFaces();
			XDWORD size = faces->GetSize();
			faces->Get(0)->texturecoordinates[0]->x  = progress;
			faces->Get(0)->texturecoordinates[2]->x  = progress;
			faces->Get(1)->texturecoordinates[2]->x  = progress;
	
			bar->GetMesh()->SetFaceBuffers();
			bar->GetMesh()->SetIsLoaded(false);
	
			progresschanged = false;
			modified = true;
		}
	
	if(modified)
		{
			float scalex = gpt1x - gpt0x;
			float scaley = gpt1y - gpt0y;
	
			// postions now need to be computed to normalized format
	
			GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x, gpt0y, 0.0f);
	
			scaley /= scene->GetViewport()->GetAspectRatio();
	
			float x = pt.x + scalex;
			float y = pt.y - scaley;
	
			background->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
			background->SetRenderPriority(widgetlevel);
			background->GetNode()->SetScale(scalex, scaley, 1.0f);
	
			bar->GetNode()->SetScale(scalex*progress, scaley, 0.0f);
			bar->GetNode()->SetPosition(pt.x + scalex*progress, y, widgetlevel + 0.2f);
			bar->SetRenderPriority(widgetlevel);
	
			background->GetNode()->Update();
			background->GetNode()->UpdateWorldMatrix();
			
	
			bar->GetNode()->Update();
			bar->GetNode()->UpdateWorldMatrix();
			
		}
	
	background->Publish(GRPMSG_ORDER);
	bar->Publish(GRPMSG_ORDER);

	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETPROGRESSBARDRAWABLE::InternalUpdate
*/	
/**	
//	
//	Updates the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:32:12
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETPROGRESSBARDRAWABLE::InternalUpdate(XDWORD widgetlevel)
{
	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETPROGRESSBARDRAWABLE::SetProgressImages
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/06/2016 17:58:50
//	
//  @param				background : 
//  @param				foreground : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETPROGRESSBARDRAWABLE::SetProgressImages(XCHAR* background, XCHAR* foreground)
{
	backgroundimagename.Set(background);
	foregroundimagename.Set(foreground);
	imagesloaded = false;
}

