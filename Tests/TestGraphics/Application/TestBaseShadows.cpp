
/*------------------------------------------------------------------------------------------
//	TESTBASESHADOWS.CPP
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

#include "TestBaseShadows.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASESHADOWS::IniResources
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
bool TESTBASESHADOWS::IniResources()
{
	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("shadows"));

	XPATH xpath;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("testShadows5.fbx"));
	//xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("omni.fbx"));
	GRPOBJECT* obj=NULL;
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}


	target=NULL;


		//-------------
	
	Subscribe(xapplication->GetMainScene());
	
	GRPLIGHT* light=obj->GetLights()->GetLast();
						light->shadowquality=1.0;
						scene->Add(obj);
						light->SetViewport(scene->GetViewport());
						
						GRPSHADERLIBRARY::Get()->GetProgramByName(__L("LightSpace"))->Compile();
	
	GRPPASS*	shadowpass=NULL;
	switch (light->GetType())
	{
	case GRPLIGHTTYPE_SPOTLIGHT:
	case GRPLIGHTTYPE_DIRECTIONAL:	
															{
																	shadowpass = GRPPASSMANAGER::Get()->Create(__L("shadows"), 99);
																	shadowpass->SetLight(light);
																	scene->GetObjects()->Get(0)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("ShadowCasting")));
															}
															break;

	case GRPLIGHTTYPE_POINT:
															{
																	shadowpass = GRPPASSMANAGER::Get()->Create(__L("pointshadows"), 99);
																	//shadowpass=new GRPSHADOWPOINTLIGHTPASS();	
																	shadowpass->SetLight(light);
																	scene->GetObjects()->Get(0)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("ShadowCasting")));
															}
															break;
	case GRPLIGHTTYPE_DAYLIGHT: 
															{
																XDEBUG_PRINTCOLOR(4, __L("Daylight shadows unimplemented!"));
															}
															break;
	default:
															{
																XDEBUG_PRINTCOLOR(4, __L("Unknown light type"));
															}
															break;
	}
								
	scene->Add(shadowpass);
	//*/

  light = obj->GetLights()->GetLast();
	light->SetStrength(100.0);

	scene->Add(obj);
	obj->PlayAll();		
	lockcamera = true;

	scene->SelectCamera(scene->GetCameras()->Get(1));

	return true;
}



/*-------------------------------------------------------------------
//	TESTBASESHADOWS::EndResources
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
bool TESTBASESHADOWS::EndResources()
{
	if (fullviewport)
		delete(fullviewport);

	if (target)
		delete(target);
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASESHADOWS::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/07/2017 11:51:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESHADOWS::InputManagement()
{

	if (keyboard)
	{
			if(keyboard->GetButton(INPBUTTON_ID_1))
			if(keyboard->GetButton(INPBUTTON_ID_1)->GetState()==INPBUTTON_STATE_RELEASED) 			
			{				
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=0.5f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
				return true;
			}

			if(keyboard->GetButton(INPBUTTON_ID_2))
			if(keyboard->GetButton(INPBUTTON_ID_2)->GetState() == INPBUTTON_STATE_RELEASED)
			{
				xapplication->GetMainScene()->GetLights()->GetLast()->shadowquality=2.00f;
				xapplication->GetMainScene()->GetLights()->GetLast()->BuildShadowMap();
				return true;
			}


			if(keyboard->GetButton(INPBUTTON_ID_UP))
			if(keyboard->GetButton(INPBUTTON_ID_UP)->IsPressedWithRelease()) 	
			{
					 xapplication->GetMainScene()->GetLights()->GetLast()->bias+=0.00001f;
					 return true;
			}

			if(keyboard->GetButton(INPBUTTON_ID_LEFT))
			if(keyboard->GetButton(INPBUTTON_ID_LEFT)->IsPressedWithRelease()) 	
			{
					 xapplication->GetMainScene()->GetLights()->GetLast()->bias+=0.0000001f;
					 return true;
			}

			if(keyboard->GetButton(INPBUTTON_ID_DOWN))
			if(keyboard->GetButton(INPBUTTON_ID_DOWN)->IsPressedWithRelease()) 	
			{
					 xapplication->GetMainScene()->GetLights()->GetLast()->bias-=0.00001f;
					 return true;
			}

			if(keyboard->GetButton(INPBUTTON_ID_RIGHT))
			if(keyboard->GetButton(INPBUTTON_ID_RIGHT)->IsPressedWithRelease()) 	
			{
					 xapplication->GetMainScene()->GetLights()->GetLast()->bias-=0.0000001f;
					 return true;
			}

	}

	return TESTBASE::InputManagement();
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
bool TESTBASESHADOWS::Do()
{

	return true;
}


/*-------------------------------------------------------------------
//	TESTBASESHADOWS::Notify
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
bool TESTBASESHADOWS::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{

	return true;
}