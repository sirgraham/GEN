
/*------------------------------------------------------------------------------------------
//	TESTBASEMULTIRENDER.CPP
//	
//	Test multi viewport and multi cam rendering
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 02/06/2016 12:18:06
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
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

#include "TestBaseMultiRender.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASEMULTIRENDER::IniResources
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
bool TESTBASEMULTIRENDER::IniResources()
{
	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("multirender"));

	XPATH xpath;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestMultirender.FBX"));
	GRPOBJECT* obj=NULL;
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}

	scene->Add(obj);

	//------------- we create 4 viewports


	scene->GetPass()->SetCamera		(scene->GetCameras()->GetLast());

	
	GRPPASS*			pass = NULL;
	

	GRPVIEWPORT*	vp = NULL;
								
	scene->GetPass()->SetActive(false);
	
								pass = GRPPASSMANAGER::Get()->Create(__L("viewport")); 								
								pass->SetCamera(scene->GetCameras()->Get(1));
								pass->SetRect(0.5f, 0.5f, 1.0f, 1.0f);
								scene->Add(pass);
				
								pass = GRPPASSMANAGER::Get()->Create(__L("viewport"));
								pass->SetCamera(scene->GetCameras()->Get(2));
								pass->SetRect(0.5f, 0.0f, 1.0f, 0.5f);
								scene->Add(pass);
							
								pass = GRPPASSMANAGER::Get()->Create(__L("viewport"));
								pass->SetCamera(scene->GetCameras()->Get(3));
								pass->SetRect(0.0f, 0.0f, 0.5f, 0.5f);
								scene->Add(pass);

								pass = GRPPASSMANAGER::Get()->Create(__L("viewport"));
								pass->SetCamera(scene->GetCameras()->Get(4));
								pass->SetRect(0.0f, 0.5f, 0.5f, 1.0f);
								scene->Add(pass);


	target=NULL;

		//------------- create a texture render target

			GRPBITMAP* bitmap		= grpfactory->CreateBitmap(xapplication->GetMainScreen()->GetWidth(), xapplication->GetMainScreen()->GetHeight(), GRPPROPERTYMODE_32_RGBA_8888);
			GRPTEXTURE* texture = new GRPTEXTURE();
			texture->Create(bitmap);
			texture->SetName(__L("RenderTarget"));
			
			GRPMATERIALLIBRARY::Get()->Create(texture);
				
														fullviewport=new GRPVIEWPORT();
														fullviewport->SetProjection	(GRPPROJECTIONTYPE_FRUSTUM);
														fullviewport->SetPosition		(0.0f,  0.0f);	
														fullviewport->SetSize				(0.2f,	0.2f);
														fullviewport->Select				();
														fullviewport->AttachCam			(1);
														xapplication->GetMainScreen()->AddViewport(fullviewport);

			target=scene->CreateRenderTarget(xapplication->GetMainScreen()->GetWidth(), xapplication->GetMainScreen()->GetHeight());
			target->InitFrameBuffers(texture);
			target->Deselect();

			//*/
		//-------------

	Subscribe(xapplication->GetMainScene());

	//obj->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"NRL"));

	scene->SelectCamera(obj->GetCamera());

	scene->GetLights()->GetLast()->SetMultiplier(1.0f);
	//scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_ALL);  scene->ShowDebug(false);
	obj->PlayAll();		
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASEMULTIRENDER::EndResources
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
bool TESTBASEMULTIRENDER::EndResources()
{
	if (fullviewport)
		delete(fullviewport);

	if (target)
		delete(target);
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASEMULTIRENDER::Do
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
bool TESTBASEMULTIRENDER::Do()
{
	if (keyboard)
	{
			if(keyboard->GetButton(INPBUTTON_ID_1))
			if(keyboard->GetButton(INPBUTTON_ID_1)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->GetPass()->SetCamera(xapplication->GetMainScene()->GetCameras()->Get(0)); //reeeemeeeeember this is the main camera and uses diferent axis
				
			if(keyboard->GetButton(INPBUTTON_ID_2))
			if(keyboard->GetButton(INPBUTTON_ID_2)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->GetPass()->SetCamera(xapplication->GetMainScene()->GetCameras()->Get(1));

			if(keyboard->GetButton(INPBUTTON_ID_3))
			if(keyboard->GetButton(INPBUTTON_ID_3)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->GetPass()->SetCamera(xapplication->GetMainScene()->GetCameras()->Get(2));

			if(keyboard->GetButton(INPBUTTON_ID_4))
			if(keyboard->GetButton(INPBUTTON_ID_4)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->GetPass()->SetCamera(xapplication->GetMainScene()->GetCameras()->Get(3));				

			if(keyboard->GetButton(INPBUTTON_ID_5))
			if(keyboard->GetButton(INPBUTTON_ID_5)->IsPressedWithRelease()) 				
					xapplication->GetMainScene()->GetPass()->SetCamera(xapplication->GetMainScene()->GetCameras()->Get(4));

			if(keyboard->GetButton(INPBUTTON_ID_0))
			if(keyboard->GetButton(INPBUTTON_ID_0)->IsPressedWithRelease()) 				
			{
				target->CaptureFrameData();
				XPATH xpath;
				xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS,1, __L("target.raw"));
				target->SaveAsRaw(xpath.Get());
			}

	}
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEMULTIRENDER::Notify
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
bool TESTBASEMULTIRENDER::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
	static int i=0;
	switch(msg)
	{
	case GRPMSG_STEP:
		/*
											target->SelectAsTarget();
											xapplication->GetMainScene()->RenderFrame();
											target->Deselect();
											*/
										break;
	}
	return true;
}