
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
	
#include "TestBaseBlending.h"
		
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
#include "GRPBlendingPass.h"


#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASEBLENDING::IniResources
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
bool TESTBASEBLENDING::IniResources()
{
	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("blending"));

	XPATH xpath;


	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestBlending.FBX"));
	GRPOBJECT* obj=NULL;
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}
	
		obj->SetProgram			(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
		
	
		for (XDWORD e=0;e<obj->GetElements()->GetSize();e++)
		{
			XDEBUG_PRINTCOLOR(0,__L("element: %s"),obj->GetElements()->Get(e)->GetName()->Get());
			obj->GetElements()->Get(e)->SetBlending(true);		
		}
		

		scene->Add(obj);

		scene->ReorderScene();

		GRPCAMERA* cam=obj->GetCameras()->Get(obj->GetCurrentCamera());
		scene->SelectCamera(cam);

		
		obj->FindElement(__L("Model::Plane"))->SetBlending(false);	
		
		GRPPASS*					blending=GRPPASSMANAGER::Get()->Create(__L("blending"));
											scene->Add(blending);																			
											
											XDEBUG_PRINTCOLOR(3,__L(" Main %d blending %d"),scene->GetPass()->GetRenderOrder()->GetSize(),blending->GetRenderOrder()->GetSize());

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
bool TESTBASEBLENDING::EndResources()
{
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASEBLENDING::Do
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
bool TESTBASEBLENDING::Do()
{
	if (keyboard)
	{
		GRPBLENDINGMODE mode=GRPBLENDINGMODE_UNKNOWN;

			if(keyboard->GetButton(INPBUTTON_ID_1))
			if(keyboard->GetButton(INPBUTTON_ID_1)->IsPressedWithRelease()) 			
			{
				XDEBUG_PRINTCOLOR(0,__L("Mode blending set to : GRPBLENDINGMODE_NONE"));
				mode=GRPBLENDINGMODE_NONE;
			}
			if(keyboard->GetButton(INPBUTTON_ID_2))
			if(keyboard->GetButton(INPBUTTON_ID_2)->IsPressedWithRelease()) 			
			{
				XDEBUG_PRINTCOLOR(0,__L("Mode blending set to : GRPBLENDINGMODE_ADDITIVE"));
				mode=GRPBLENDINGMODE_ADDITIVE;
			}
			if(keyboard->GetButton(INPBUTTON_ID_3))
			if(keyboard->GetButton(INPBUTTON_ID_3)->IsPressedWithRelease()) 			
			{
				XDEBUG_PRINTCOLOR(0,__L("Mode blending set to : GRPBLENDINGMODE_MULTIPLICATIVE"));
				mode=GRPBLENDINGMODE_MULTIPLICATIVE;
			}
			if(keyboard->GetButton(INPBUTTON_ID_4))
			if(keyboard->GetButton(INPBUTTON_ID_4)->IsPressedWithRelease()) 			
			{
				XDEBUG_PRINTCOLOR(0,__L("Mode blending set to : GRPBLENDINGMODE_SUBSTRACTIVE"));
				mode=GRPBLENDINGMODE_SUBSTRACTIVE;
			}
			if(keyboard->GetButton(INPBUTTON_ID_5))
			if(keyboard->GetButton(INPBUTTON_ID_5)->IsPressedWithRelease()) 			
			{
				XDEBUG_PRINTCOLOR(0,__L("Mode blending set to : GRPBLENDINGMODE_ALPHABLEND"));
				mode=GRPBLENDINGMODE_ALPHABLEND;
			}

			if (mode!=GRPBLENDINGMODE_UNKNOWN)
			{
				blending->SetBlending(true,mode);
				/*
				for (XDWORD e=1;e<xapplication->GetMainScene()->GetObjects()->Get(0)->GetElements()->GetSize();e++)		
				{
					xapplication->GetMainScene()->GetObjects()->Get(0)->GetElements()->Get(e)->SetBlending(true);				
					xapplication->GetMainScene()->GetObjects()->Get(0)->GetElements()->Get(e)->SetBlendingMode(mode);
				}
				*/
				
			}
	}
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEBLENDING::Notify
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
bool TESTBASEBLENDING::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
	return true;
}