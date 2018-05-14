
/*------------------------------------------------------------------------------------------
//	TESTBASEPASSES.CPP
//	
//	Tests stencil and scissors
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 12/07/2016 18:23:59
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "testbasepasses.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


#include "testbasemultirender.h"
		
#include "XApplication.h"
#include "XDebug.h"

#include "GRPPass.h"

#include "GRPFactory.h"
#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPScreen.h"
#include "GRPRenderTarget.h"
#include "GRPViewport.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPCamera.h"
#include "GRPLight.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"
#include "GRPBackgroundPass.h"

#include "TestBaseMultiRender.h"

#include "GRPStencilPass.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASEPASSES::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/06/2016 12:28:25
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEPASSES::IniResources()
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;
	
	PathManagement(__L("passes"));

	//----------------------------------- 

	GRPSCENE* scene = xapplication->GetMainScene();
	
	//-----------------------------------
	
	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestPasses.FBX"));

	GRPOBJECT* obj=NULL;
	if(!(obj=LoadObject(obj,xpath.Get())))			{			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));			return false;		}
	scene->Add(obj);


	XDWORD testpasses = TEST_ALL;

	if (!(testpasses&TEST_MAIN))
			scene->GetPass()->SetActive(false);

	//----------------------------------- stencil pass (teapot stencils scene)	
	if (testpasses & TEST_STENCIL)
	{
			GRPPASS*	stencilpass = GRPPASSMANAGER::Get()->Create(__L("stencil"), scene->GetPass()->GetPriority() -2);
			scene->Add(stencilpass);
			stencilpass->Add(obj->FindByName(__L("Model::Teapot001")));
	}

//------------------------------------ depth
	if (testpasses & TEST_DEPTH)
	{
			GRPPASS* depthpass= GRPPASSMANAGER::Get()->Create(__L("depth"), scene->GetPass()->GetPriority()-1);
			scene->Add(depthpass);
	}
	//*/
	//----------------------------------- Background pass
	if (testpasses & TEST_BACKGROUND)
	{
			GRPMATERIAL* overlay_material = GRPMATERIALLIBRARY::Get()->Load(__L("background"), 1, GRPTEXTURELAYERTYPE_DIFFUSE, __L("gen_logo.jpg"));

			GRPPASS* bgpass = GRPPASSMANAGER::Get()->Create(__L("background"), scene->GetPass()->GetPriority() + 2);
			bgpass->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("gen_logo.jpg")));
			scene->Add(bgpass);
	}

	//---------------------------------- overlay	
	if (testpasses & TEST_OVERLAY)
	{
			GRPMATERIAL* overlay_material = GRPMATERIALLIBRARY::Get()->Load(
					__L("overlay"), 2,
					GRPTEXTURELAYERTYPE_DIFFUSE, __L("Mascara_camara.png"),
					GRPTEXTURELAYERTYPE_OPACITY, __L("Mascara_camara_alfa.tga")
			);

			GRPPASS*	overlaypass = GRPPASSMANAGER::Get()->Create(__L("overlay"), scene->GetPass()->GetPriority() + 3);
			overlaypass->SetRect(0.75, 0.0, 1.0, 0.25);
			overlaypass->SetMaterial(overlay_material);
			scene->Add(overlaypass);
	}

	//---------------------------------- viewport
	if (testpasses & TEST_VIEWPORT)
	{
			GRPPASS*	viewportpass = GRPPASSMANAGER::Get()->Create(__L("viewport"), scene->GetPass()->GetPriority() + 4);
			viewportpass->SetRect(0.75, 0.0, 1.0, 0.25);
			scene->Add(viewportpass, true);
	}

	//---------------------------------- Debugger passs
	if (testpasses & TEST_DEBUG)
	{
			GRPPASS* dbgpass = GRPPASSMANAGER::Get()->Create(__L("debug"), scene->GetPass()->GetPriority() + 5);
			scene->Add(dbgpass);
	}

	//---------------------------------- Offscreen renderer
	if (testpasses & TEST_OFFSCREEN)
	{
			GRPPASS* offscreen = GRPPASSMANAGER::Get()->Create(__L("offscreen"), scene->GetPass()->GetPriority() - 3);			
			offscreen->SetViewport(scene->GetViewport());
			scene->Add(offscreen);

			//---------------------------------- Offscreen renderer
			if (testpasses & TEST_POSTPROCESS)
			{
					GRPPASS* postprocess = GRPPASSMANAGER::Get()->Create(__L("background"), scene->GetPass()->GetPriority() + 6);
					postprocess->SetMaterial(offscreen->GetMaterial());
					postprocess->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Invert")));
					scene->Add(postprocess);
			}			
	}
	

	//-------------
	Subscribe(xapplication->GetMainScene());

	scene->GetLights()->GetLast()->SetStrength(1.0f);
	scene->GetLights()->GetLast()->SetMultiplier(1.0f);
	obj->PlayAll();		
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASEPASSES::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 12:56:51
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEPASSES::EndResources()
{

	return true;
}



/*-------------------------------------------------------------------
//	TESTBASEPASSES::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/06/2016 12:29:03
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEPASSES::Do()
{
	if (keyboard)
	{
			if(keyboard->GetButton(INPBUTTON_ID_1))
			if(keyboard->GetButton(INPBUTTON_ID_1)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->SetCurrentCamera(0); //reeeemeeeeember this is the main camera and uses diferent axis
				
			if(keyboard->GetButton(INPBUTTON_ID_2))
			if(keyboard->GetButton(INPBUTTON_ID_2)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->SetCurrentCamera(1);

			if(keyboard->GetButton(INPBUTTON_ID_3))
			if(keyboard->GetButton(INPBUTTON_ID_3)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->SetCurrentCamera(2);

			if(keyboard->GetButton(INPBUTTON_ID_4))
			if(keyboard->GetButton(INPBUTTON_ID_4)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->SetCurrentCamera(3);				

			if(keyboard->GetButton(INPBUTTON_ID_5))
			if(keyboard->GetButton(INPBUTTON_ID_5)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->SetCurrentCamera(4);

	}
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEPASSES::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:15:49
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEPASSES::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
	static int i=0;
	switch(msg)
	{
	case GRPMSG_STEP: 
										break;
	case GRPMSG_PASSSTARTED:
									{
										GRPPASS* pass=static_cast<GRPPASS*>(notifier);
										if (pass)
										{
											if (pass->GetName()->Compare(__L("pass2"))==0)
											{
												XDEBUG_PRINTCOLOR(4,__L("PassEvent"));
											}
										}
									}
									break;
	}
	return true;
}