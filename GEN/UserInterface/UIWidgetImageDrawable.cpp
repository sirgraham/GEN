
/*------------------------------------------------------------------------------------------
//	UIWIDGETIMAGEDRAWABLE.CPP
//	
//	Draws an Image Widget
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 07/06/2016 13:31:13
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetImageDrawable.h"
	
#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPScene.h"
#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPFontManager.h"
#include "GRPViewport.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	UIWIDGETIMAGEDRAWABLE::UIWIDGETIMAGEDRAWABLE
*/	
/**	
//	
//	Class Constructor UIWIDGETIMAGEDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 15:53:53
//	
//  @param				name : 
//  @param				uimanagerargin : 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETIMAGEDRAWABLE::UIWIDGETIMAGEDRAWABLE(XCHAR* name, UIMANAGER * uimanagerargin, GRPSCENE* scene) : UIWIDGETIMAGE(name, uimanagerargin)
{
	Clean();

	this->scene = scene;
	imageloaded = false;

	// create object and element for the image
	imageobject = new GRPOBJECT();
	imageobject->SetNode(scene->CreateNode());

	imagecanvas = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	imagecanvas->SetNode(scene->CreateNode());
	imagecanvas->GetNode()->LinkTo(imageobject->GetNode());
	imageobject->GetNode()->Init();
	imagecanvas->GetNode()->Init();
	imagecanvas->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));

	// make material with a dummy image? could be
	// create the image material
	GRPMATERIAL * materialproc = new GRPMATERIAL();
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(materialproc);

	imageobject->AddElement(imagecanvas);

	imageobject->SetMaterial(materialproc);

	scene->Add(imageobject);
}





/*-------------------------------------------------------------------
//	UIWIDGETIMAGEDRAWABLE::~UIWIDGETIMAGEDRAWABLE
*/	
/**	
//	
//	 Class Destructor UIWIDGETIMAGEDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 15:54:02
//	
*/
/*-----------------------------------------------------------------*/
UIWIDGETIMAGEDRAWABLE::~UIWIDGETIMAGEDRAWABLE()
{
	// need to check again how to properly delete elements from the world
	delete imageobject;
	scene->GetObjects()->Delete(imageobject);

	Clean();
}






/*-------------------------------------------------------------------
//	UIWIDGETIMAGEDRAWABLE::InternalDraw
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 15:54:10
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGEDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
	// check if image loaded, if not load and set the flag to true
	if(!imageloaded)
		{
			imageloaded = true;
	
			// if texture not loaded load it
			GRPFILEBITMAP*  grpfilebitmap = NULL;
			grpfilebitmap = new GRPFILEBITMAP();
			//grpfilebase = new GRPFILEBITMAPTGA();
	
			XPATH xpath;
			xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,image.Get());
			GRPBITMAP* bitmap = grpfilebitmap->Load(xpath);
	
			GRPTEXTURE* texture = new GRPTEXTURE();
			texture->Create(bitmap);
	
			imageobject->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, texture);
			
			GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&image, texture);
			delete grpfilebitmap;
	
			// consider how to handle the material
			imagecanvas->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
				
			imagecanvas->SetBlending(true);
			imagecanvas->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
			//imagecanvas->SetBlendingMode(GRPBLENDINGMODE_ADDITIVE);
			//imagecanvas->SetVisibility(0.5f);
		}
	
	if(modified)
		{
			float scalex = gpt1x - gpt0x;
			float scaley = gpt1y - gpt0y;
	
			GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x, gpt0y, 0.0f);
			scaley /= scene->GetViewport()->GetAspectRatio();
	
			float x = pt.x + scalex;
			float y = pt.y - scaley;
	
			bool update = true;
	
			//imageobject->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
			//imageobject->GetNode()->SetScale(scalex, scaley, 1.0f);
			imageobject->GetNode()->Update();
			imageobject->GetNode()->UpdateWorldMatrix(&update);
			
			imagecanvas->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
			imagecanvas->SetRenderPriority(widgetlevel);
			imagecanvas->GetNode()->SetScale(scalex, scaley, 1.0f);
			imagecanvas->GetNode()->Update();
			update = true;
			imagecanvas->GetNode()->UpdateWorldMatrix(&update);
	
			//imageobject->GetElements()->Get(0)->Publish(GRPMSG_ORDER);
			
		}
	
	imagecanvas->Publish(GRPMSG_ORDER);

	return true;
}






/*-------------------------------------------------------------------
//	UIWIDGETIMAGEDRAWABLE::InternalUpdate
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 15:54:18
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGEDRAWABLE::InternalUpdate(XDWORD widgetlevel)
{
	return UIWIDGETIMAGE::InternalUpdate(widgetlevel);
}





/*-------------------------------------------------------------------
//	UIWIDGETIMAGEDRAWABLE::SetActive
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/06/2016 12:10:38
//	
//  @param				active : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETIMAGEDRAWABLE::SetActive(bool active)
{
	if(imagecanvas)
		{
			imagecanvas->SetVisible(active);
		}

	UIWIDGET::SetActive(active);
}

