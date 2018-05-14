
/*------------------------------------------------------------------------------------------
//	TESTBASEFACERIG.CPP
//	
//	Test Base Facerig
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

#include "SNDFactory.h"
#include "SNDElement.h"
#include "SNDSource.h"

#include "TestBaseFacerig.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	TESTBASEFACERIG::TESTBASEFACERIG
*/	
/**	
//	
//	Class Constructor TESTBASEFACERIG
//	
//	@author				Abraham J. Velez
//	@version			31/05/2016 16:05:56
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASEFACERIG::TESTBASEFACERIG(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)
{
	Clean();
}




/*-------------------------------------------------------------------
//	TESTBASEFACERIG::~TESTBASEFACERIG
*/	
/**	
//	
//	 Class Destructor TESTBASEFACERIG
//	
//	@author				Abraham J. Velez
//	@version			31/05/2016 16:10:02
//	
*/
/*-----------------------------------------------------------------*/
TESTBASEFACERIG::~TESTBASEFACERIG()
{
	Clean();
}


/*-------------------------------------------------------------------
//	TESTBASEFACERIG::IniResources
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
bool TESTBASEFACERIG::IniResources()
{	
#ifndef GRPDEFORMERS_ACTIVE

		XDEBUG_PRINTCOLOR(4,__L(""));
		XDEBUG_PRINTCOLOR(4,__L(""));
		XDEBUG_PRINTCOLOR(4,__L("\t\tThis Demo requires to be compiled with GRPDEFORMER_ACTIVE preprocessor directive. Exiting now."));
	return false;
#endif

	if(!xapplication)										 return false;
	if(!xapplication->GetMainScene())		 return false;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("facerig"));

	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("facerig2.fbx"));

	obj=NULL;
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}		
	scene->Add(obj);

	#ifdef SND_ACTIVE

	soundsystem = SNDFACTORY::Get();
	if(!soundsystem)
		{
			return false;
		}
	XPATH soundpath;
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SOUNDS, soundpath);

	if(!soundsystem->AddFile(soundpath, __L("GAMMD_01.ogg")))
		{
			return false;
		}
	
	sndelement = soundsystem->GetFile(__L("GAMMD_01.ogg"));
	if(!sndelement)
		{
			return false;
		}

	sndelement->SetLoop(false);
	soundsystem->SetMasterVolume(1.0f);
	soundsystem->PlaySound(sndelement);
#endif
	
	scene->SelectCamera(scene->GetCameras()->Get(0));

	
	//scene->GetCameras()->Get(0)->SetPosition(-428.0f, -825.0f, 200.0f);
	scene->GetCamera()->SetPosition(-214.0f, -412.0f, 100.0f);	
	scene->GetCamera()->SetUpVector(0.0f, 0.0f, 1.0f);
	
	obj->PlayAll();	
	
	obj->GetAnimationControl()->GetStack(0)->GetLayer(0)->SetLoopMode(GRPANIMATIONLOOPMODE_NONE);
	
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEFACERIG::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:31:42
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEFACERIG::EndResources()
{
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEFACERIG::Do
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
bool TESTBASEFACERIG::Do()
{
	if(!xapplication)										 return false;
	if(!xapplication->GetMainScene())		 return false;

	GRPSCENE* scene  = xapplication->GetMainScene();
	
	if(obj)
	if(obj->GetAnimationControl()->GetStack(0)->GetLayer(0)->IsFinished())
		{
			obj->GetAnimationControl()->GetStack()->Rewind();
			obj->GetAnimationControl()->GetStack()->Play();

#ifdef SND_ACTIVE
			soundsystem->PlaySound(sndelement);
#endif

		}

	return true;
}

