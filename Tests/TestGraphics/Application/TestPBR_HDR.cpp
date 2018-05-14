/*------------------------------------------------------------------------------------------
// TESTPBR_HDR.CPP
*/
/**
// \class
//
// Tests HDR buffer
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 15/11/2017 14:41:41 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPMaterialLibrary.h"
#include "GRPScene.h"
#include "GRPLight.h"
#include "TestPBR_HDR.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTHDR::TESTHDR
*/
/**
//
//	constructor
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	15/11/2017 14:45:16
//	@return		 : 
//
//	@param		GRPAPPLICATION* : 
//
*//*-----------------------------------------------------------------*/
TESTHDR::TESTHDR(GRPAPPLICATION* xapplication): TESTBASE(xapplication)
{
		Clean();
}

/*-------------------------------------------------------------------
//	 TESTHDR::IniResources
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	15/11/2017 14:45:21
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TESTHDR::IniResources()
{
		xapplication->mainscene->SetScreen(xapplication->mainscreen);

		PathManagement(__L("PBR/HDR"));

		//----------------------------------- material default location
		XPATH xpath;
		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L(""));
		GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

		if (!xapplication)										 return NULL;
		if (!xapplication->GetMainScene())		 return NULL;

		PathManagement(__L("passes"));

		//----------------------------------- 

		GRPSCENE* scene = xapplication->GetMainScene();

		//-----------------------------------
		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestPasses.FBX"));

		GRPOBJECT* obj = NULL;
		if (!(obj = LoadObject(obj, xpath.Get()))) { XDEBUG_PRINTCOLOR(4, __L("Loading FBX Failed!"));			return false; }
		scene->Add(obj);

		//-------------
		Subscribe(xapplication->GetMainScene());

		scene->GetLights()->GetLast()->SetStrength(1.0f);
		scene->GetLights()->GetLast()->SetMultiplier(1.0f);

		scene->GetObjects()->GetLast()->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOT3PerFragment")));

		//---------------------------------- Offscreen renderer

		hdr = GRPPASSMANAGER::Get()->Create(__L("hdr"),0);
		hdr->SetViewport(scene->GetViewport());
		scene->Add(hdr);
		scene->GetPass()->SetActive(false);

		//---------------------------------- postprocess
		
		postprocess = GRPPASSMANAGER::Get()->Create(__L("background"), 1);
		postprocess->SetMaterial(hdr->GetMaterial());
		postprocess->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("hdr")));
		scene->Add(postprocess);
		
		hdrprogram		 = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("hdr"));
		defaultprogram = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("PostProcessing"));

		obj->PlayAll();
		return true;

}

/*-------------------------------------------------------------------
//	 TESTHDR::EndResources
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	15/11/2017 14:45:26
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TESTHDR::EndResources()
{
		return true;
}

/*-------------------------------------------------------------------
//	 TESTHDR::Do
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	15/11/2017 14:45:30
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TESTHDR::Do()
{		
		hdrprogram->Set((char*)"gamma", gamma);
		hdrprogram->Set("exposure", exposure);
		this->xapplication->GetMainScene()->GetLights()->GetLast()->SetStrength(intensity);
		return true;
}


/*-------------------------------------------------------------------
//	 TESTHDR::InputManagement
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	28/11/2017 16:57:04
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TESTHDR::InputManagement()
{

		if (keyboard->GetButton(INPBUTTON_ID_1))
				if (keyboard->GetButton(INPBUTTON_ID_1)->GetState() == INPBUTTON_STATE_RELEASED)
				{
						gamma = 1.0f/2.2f;
						exposure = 1.0f;
						hdrprogram->Set("gamma", gamma);
						this->postprocess->SetProgram(hdrprogram);
				}
		if (keyboard->GetButton(INPBUTTON_ID_2))
				if (keyboard->GetButton(INPBUTTON_ID_2)->GetState() == INPBUTTON_STATE_RELEASED)
				{						
						this->postprocess->SetProgram(defaultprogram);
				}

		if (keyboard->GetButton(INPBUTTON_ID_3))
				if (keyboard->GetButton(INPBUTTON_ID_3)->GetState() == INPBUTTON_STATE_HOLD)
				{
						intensity += 0.01f;
				}
		if (keyboard->GetButton(INPBUTTON_ID_4))
				if (keyboard->GetButton(INPBUTTON_ID_4)->GetState() == INPBUTTON_STATE_HOLD)
				{
						intensity -= 0.01f;
				}

		if (mouse)
		{
				if (mouse->GetButton(1)->GetState() == INPBUTTON_STATE_HOLD)
				{
						gamma = mouse->GetCursor(0)->GetX() / float(this->xapplication->GetMainScreen()->GetWidth());
						gamma *= 2.2f;					
						exposure = mouse->GetCursor(0)->GetY() / float(this->xapplication->GetMainScreen()->GetHeight());
						exposure *= 2.0;						
						return true;
				}
		}

		return TESTBASE::InputManagement();
}
/*-------------------------------------------------------------------
//	 TESTHDR::Notify
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	15/11/2017 14:45:36
//	@return		bool : 
//
//	@param		GRPMSGTYPE : 
//	@param		GRPSUBJECT* : 
//
*//*-----------------------------------------------------------------*/
bool TESTHDR::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{
		return true;
}
