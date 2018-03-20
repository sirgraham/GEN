
/*------------------------------------------------------------------------------------------
//	TESTBASEOBJMERGING.CPP
//	
//	Test Base OBJ Merging
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/05/2016 16:19:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XApplication.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"
#include "GRPCamera.h"

#include "TestBaseOBJMerging.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	TESTBASEOBJMERGING::IniResources
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/05/2016 22:31:29
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJMERGING::IniResources()
{	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("objmerguing"));

	//----------------------------------- material default location
	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

		//----------------------------------- Cargar un material		
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("coord.png"));	XDEBUG_PRINTCOLOR(1,__L("%s"),xpath.Get());

	if(!GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),	__L("coord.png")))
		{
			XDEBUG_PRINTCOLOR(1,__L("Error loading Texture data"));
			return false;
		}			

	GRPOBJECT* obj = new GRPOBJECT();
	if (!obj) return false;

	obj->SetNode(scene->CreateNode());
	obj->GetNode()->SetScale		(50.0f,50.0f,50.0f);	
							
	GRPELEMENT* cube = new GRPELEMENT();
	cube->Reference(GRPOBJECTBUILDER::Get()->CreateCube());
	if (!cube) return false;

			cube->SetNode(scene->CreateNode());
			cube->GetNode()->SetPosition(-0.5f,0.0f,0.0f);
			cube->GetNode()->SetRotation(45.0f,0.5f,0.5f);
			cube->CollapseTransforms();

			obj->AddElement(cube);
	
	GRPELEMENT* cube2 = new GRPELEMENT();
	if (!cube2) return false;

	cube2->Reference(cube);

			cube2->SetNode(scene->CreateNode());
			cube2->GetNode()->SetPosition(0.5f,0.0f,0.0f);
			cube2->GetNode()->SetRotation(0.5f,45.0f,0.5f);
			cube2->GetNode()->Update();

			cube2->CollapseTransforms();
	
	obj->AddElement			(cube2);
	obj->SetMaterial		(GRPMATERIALLIBRARY::Get()->Select(__L("coord.png")));
	obj->SetProgram			(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
	obj->MergeElements	();
	
	scene->Add(obj);

	scene->GetVisualDebugger()->DebugSize=0.3f;
	scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_BBS | GRPVISUALDEBUGGER_FLAG_LOCATORS | GRPVISUALDEBUGGER_FLAG_NORMALS);


	scene->GetCamera()->SetPosition(100.0f,100.0f,100.0f);
	return true;
}





/*-------------------------------------------------------------------
//	TESTBASEOBJMERGING::Do
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/05/2016 22:31:18
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJMERGING::Do()
{
	static float degree=0.0f;
	degree+=0.0001f;

	xapplication->GetMainScene()->GetCamera()->SetPosition(100.0f*(float)sin(degree),100.0f,100.0f*(float)cos(degree));
	return true;	
}

/*-------------------------------------------------------------------
//	TESTBASEOBJMERGING::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:48:13
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJMERGING::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEOBJMERGING::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:52:06
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJMERGING::EndResources()
{
	return true;
}