
/*------------------------------------------------------------------------------------------
//	TESTBASEPOINTLIGHTSHADOWS.CPP
//	
//	Test shadow casting
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
#include "GRPShadowPass.h"

#include "GRPOpenGL.h"

#include "TestBasePointLightShadow.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASEPOINTLIGHTSHADOWS::IniResources
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
bool TESTBASEPOINTLIGHTSHADOWS::IniResources()
{
	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("shadows"));

	XPATH xpath;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("testShadows5.fbx"));
	GRPOBJECT* obj=NULL;
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}
	scene->Add(obj);

	target=NULL;


		//-------------

	Subscribe(xapplication->GetMainScene());
	
	GRPLIGHT* light=scene->GetLights()->GetLast();
						light->shadowquality=1.0;
						light->SetViewport(scene->GetViewport());
						//light->SetMultiplier(0.5f);
						
						//GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"LSP")->Compile();
						GRPSHADERLIBRARY::Get()->GetProgramByName(__L("LightSpace"))->Compile();

	
	GRPSHADOWPOINTLIGHTPASS*	shadowpass=new GRPSHADOWPOINTLIGHTPASS();									
														scene->Add(shadowpass);
														shadowpass->GetShadowCasters();

	//scene->GetObjects()->Get(0)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SHC"));								
	scene->GetObjects()->Get(0)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOT3PerFragment")));								
	

	light->SetRotation(90.0,0.0,0.0);
	light->Setinterestposition(0.0,0.0,100.0);
	light->prerotation=GRPVECTOR(0.0,0.0,0.0);
	light->postrotation=GRPVECTOR(0.0,0.0,0.0);
	light->UsesPostRotation=light->UsesPostRotation=false;

	light->UpdateWorldMatrix();	
  light->BuildLookAt();


	scene->GetCameras()->Add(light);
	scene->SelectCamera(light);

	return true;
}



/*-------------------------------------------------------------------
//	TESTBASEPOINTLIGHTSHADOWS::EndResources
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
bool TESTBASEPOINTLIGHTSHADOWS::EndResources()
{
	if (fullviewport)
		delete(fullviewport);

	if (target)
		delete(target);
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASESHADOWS::Do
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
bool TESTBASEPOINTLIGHTSHADOWS::Do()
{
	if (keyboard)
	{
			if(keyboard->GetButton(INPBUTTON_ID_1))
			if(keyboard->GetButton(INPBUTTON_ID_1)->IsPressedWithRelease()) 			
			{				
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=0.25f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
			}

			if(keyboard->GetButton(INPBUTTON_ID_2))
			if(keyboard->GetButton(INPBUTTON_ID_2)->IsPressedWithRelease()) 	
			{
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=0.5f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
			}

			if(keyboard->GetButton(INPBUTTON_ID_3))
			if(keyboard->GetButton(INPBUTTON_ID_3)->IsPressedWithRelease()) 	
			{
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=0.75f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
			}

			if(keyboard->GetButton(INPBUTTON_ID_4))
			if(keyboard->GetButton(INPBUTTON_ID_4)->IsPressedWithRelease()) 
			{
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=1.0f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
			}

			if(keyboard->GetButton(INPBUTTON_ID_5))
			if(keyboard->GetButton(INPBUTTON_ID_5)->IsPressedWithRelease()) 				
			{
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=1.25f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
			}

			if(keyboard->GetButton(INPBUTTON_ID_6))
			if(keyboard->GetButton(INPBUTTON_ID_6)->IsPressedWithRelease()) 				
			{
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=1.5f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
			}

			if(keyboard->GetButton(INPBUTTON_ID_7))
			if(keyboard->GetButton(INPBUTTON_ID_7)->IsPressedWithRelease())
			{
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=1.75f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
			}


			if(keyboard->GetButton(INPBUTTON_ID_UP))
			if(keyboard->GetButton(INPBUTTON_ID_UP)->IsPressedWithRelease()) 	
					 xapplication->GetMainScene()->GetLights()->GetLast()->bias+=0.00001f;

			if(keyboard->GetButton(INPBUTTON_ID_LEFT))
			if(keyboard->GetButton(INPBUTTON_ID_LEFT)->IsPressedWithRelease()) 	
					 xapplication->GetMainScene()->GetLights()->GetLast()->bias+=0.0000001f;

			if(keyboard->GetButton(INPBUTTON_ID_DOWN))
			if(keyboard->GetButton(INPBUTTON_ID_DOWN)->IsPressedWithRelease()) 	
					 xapplication->GetMainScene()->GetLights()->GetLast()->bias-=0.00001f;

			if(keyboard->GetButton(INPBUTTON_ID_RIGHT))
			if(keyboard->GetButton(INPBUTTON_ID_RIGHT)->IsPressedWithRelease()) 	
					 xapplication->GetMainScene()->GetLights()->GetLast()->bias+=0.0000001f;

	}
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEPOINTLIGHTSHADOWS::Notify
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
bool TESTBASEPOINTLIGHTSHADOWS::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{

	return true;
}

