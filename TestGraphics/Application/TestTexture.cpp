/*------------------------------------------------------------------------------------------
// TESTTEXTURE.CPP
*/
/**
// \class
//
// Testes texture operations
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 12/03/2018 12:39:10 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPShape.h"
#include "TestTexture.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

TESTTEXTURE::TESTTEXTURE(GRPAPPLICATION* xapplication) : TESTBASE(xapplication)
{

}

bool TESTTEXTURE::IniResources()
{
		if (!xapplication)										 return NULL;
		if (!xapplication->GetMainScene())		 return NULL;

		GRPSCENE* scene = xapplication->GetMainScene();

		PathManagement(__L("texture"));
		

		GRPOPENGLCTRL::CULLING::SetBackFaceCulling(false);

		scene->GetCamera()->SetPosition(0.0, 0.0, 1.0);
		scene->GetCamera()->Setinterestposition(0.0, 0.0, 0.0);


		GRPNODE* node = scene->CreateNode();
		GRPELEMENT* spline = GRPOBJECTBUILDER::Get()->CreateBox(1.0,1.0);
		spline->SetNode(node);

		XPATH xpath;
		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("gen_logo.jpg"));
		GRPTEXTURE* texture=GRPMATERIALLIBRARY::Get()->LoadTexture(xpath.Get(), xpath.Get());
		if (!texture)
				return false;
		GRPMATERIAL* material = new GRPMATERIAL();
								 material->AddLayer(GRPTEXTURELAYERTYPE_DIFFUSE,texture);
								 GRPMATERIALLIBRARY::Get()->Add(material);
		
		scene->GetCamera()->SetPosition(0.0, 0.0,4.0);		
		GRPOBJECT* obj = new GRPOBJECT();
		if (obj)
		{
				obj->SetNode(scene->CreateNode());
				obj->Add(spline);				
				obj->GetNode()->SetScale(1.0);
				obj->GetNode()->SetPosition(0.0, 0.0, 0.0);
		}

		obj->SetMaterial(material);

		node->UsesTextureMatrix = true;
		node->SetTexturePosition(0.5, 0.5, 0.0);		

		obj->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("TextureOperation")));
		scene->Add(obj);
		return true;
}


bool TESTTEXTURE::EndResources()
{
		return true;
}


bool TESTTEXTURE::Do()
{
		static float x = 0.0f;
		x += 0.1f;
		GRPELEMENT* element = xapplication->GetMainScene()->GetObjects()->GetLast()->GetElements()->GetLast();
		
		
		return true;
}
bool TESTTEXTURE::InputManagement()
{
		return true;
}
bool TESTTEXTURE::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{
		return true;
}
