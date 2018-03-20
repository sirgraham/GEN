
/*------------------------------------------------------------------------------------------
//	TESTBASERENDEROBJECT.CPP
//	
//	Test Base Render Object
//   
//	Author						: Diego Martinez Ruiz de Gaona
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
#include "GRPCamera.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPVisualDebugger.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "TestBaseRenderObject.h"
#include "GRPLight.h"
#include "GRPBlendingPass.h"
#include "GRPShadowPass.h"
#include "GRPProfiler.h"

#include "GRPSmartVector.h"

#include "XMemory.h"

	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::TESTBASERENDEROBJECT
*/	
/**	
//	
//	Class Constructor TESTBASERENDEROBJECT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/02/2016 16:27:24
//	
//  @param				inpmanager : 
//  @param				screen : 
//  @param				scene : 
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASERENDEROBJECT::TESTBASERENDEROBJECT(GRPAPPLICATION* xapplication)  :  TESTBASE(xapplication) 
{
	Clean();


	GRPSMARTVECTOR<GRPNODE*> smartvector;

	GRPNODE* node = new GRPNODE();

	smartvector.Add(node);

	delete(node);
	
	
	timer = xfactory->CreateTimer();
	delta = 0;
}




/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::~TESTBASERENDEROBJECT
*/	
/**	
//	
//	 Class Destructor TESTBASERENDEROBJECT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/02/2016 16:27:19
//	
*/
/*-----------------------------------------------------------------*/
TESTBASERENDEROBJECT::~TESTBASERENDEROBJECT()
{
	if(timer) xfactory->DeleteTimer(timer);

	Clean();
}




/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/04/2015 13:48:43
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASERENDEROBJECT::IniResources()
{		

	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	scene  = xapplication->GetMainScene();
	GRPSETTINGS::LightningDetail = GRPSETTINGS::LOW;

 	PathManagement(__L("renderobject"));
	
	//----------------------------------- load model	
	XPATH			 xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("test.FBX"));
	
			obj=LoadObject(obj,xpath.Get(),GRPFILE3D_INCLUDE_ALL, GRPFILE3D_VERBOSE_LEVEL_ALL,0);
	if(!obj)	{		XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));	return false;	}	
	
	obj->MergeAnimations();
	if (obj->GetAnimationControl()->GetStack(0))
		  obj->GetAnimationControl()->GetStack(0)->SetActive(true);

	scene->Add(obj);
	
	GRPPASS*			debug=GRPPASSMANAGER::Get()->Create(__L("debug"));
								debug->SetValue(GRPDEBUGGERPASS::DEBUG_FLAGS, (XDWORD)GRPVISUALDEBUGGER_FLAG_ALL);
	scene->Add	 (debug);
		
	if (obj)
	if (obj->GetLogic())
			obj->GetLogic()->GetControllers()->Get(0)->SetVerbose(true);
		
	obj->PlayAll();

	GRPCAMERA*cam = scene->GetCamera();

	return true;
}


/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:45:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASERENDEROBJECT::EndResources()
{	
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/04/2015 13:48:47
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASERENDEROBJECT::Do()
{	
		
		static float f = 40000.0;
		if (scene->GetLights()->GetSize())
		scene->GetLights()->GetLast()->bias = 1.0f / f;

		f *= 1.01f;
		
		if (keyboard->GetButton(INPBUTTON_ID_1))
				if (keyboard->GetButton(INPBUTTON_ID_1)->IsPressedWithRelease())
				{
						scene->SelectCamera(scene->GetCameras()->Get(0));
				}

		//----- map
		if (keyboard->GetButton(INPBUTTON_ID_2))
				if (keyboard->GetButton(INPBUTTON_ID_2)->IsPressedWithRelease())
				{
						scene->SelectCamera(scene->GetCameras()->Get(1));
				}
		
		if (keyboard->GetButton(INPBUTTON_ID_3))
				if (keyboard->GetButton(INPBUTTON_ID_3)->IsPressedWithRelease())
				{
						scene->SelectCamera(scene->GetCameras()->Get(2));
				}

		static float z = 0.0f;

				if (keyboard->GetButton(INPBUTTON_ID_W))
				if (keyboard->GetButton(INPBUTTON_ID_W)->GetState()==INPBUTTON_STATE_HOLD)
				{					
					
				}

				if (keyboard->GetButton(INPBUTTON_ID_S))
				if (keyboard->GetButton(INPBUTTON_ID_S)->GetState() == INPBUTTON_STATE_HOLD)
				{
						
				}
				float add = 0.1f;

				if (keyboard->GetButton(INPBUTTON_ID_SHIFT_LEFT))
					if (keyboard->GetButton(INPBUTTON_ID_SHIFT_LEFT)->GetState() == INPBUTTON_STATE_HOLD)
					{
						add = 10.0f;
					}

				if (keyboard->GetButton(INPBUTTON_ID_A))
				if (keyboard->GetButton(INPBUTTON_ID_A)->GetState() == INPBUTTON_STATE_HOLD)
				{
					z += add;
					const GRPVECTOR* v=obj->GetElements()->GetLast()->GetNode()->GetRotation();
					obj->GetElements()->GetLast()->GetNode()->SetRotation(v->vector[0], v->vector[1], z);
				}

				if (keyboard->GetButton(INPBUTTON_ID_D))
				if (keyboard->GetButton(INPBUTTON_ID_D)->GetState() == INPBUTTON_STATE_HOLD)
				{
					z -= add;
					const GRPVECTOR* v = obj->GetElements()->GetLast()->GetNode()->GetRotation();
					obj->GetElements()->GetLast()->GetNode()->SetRotation(v->vector[0], v->vector[1], z);
				}

				if (keyboard->GetButton(INPBUTTON_ID_Z))
						if (keyboard->GetButton(INPBUTTON_ID_Z)->GetState() == INPBUTTON_STATE_HOLD)
						{
								obj->GetAnimationControl()->GetStacks()->Get(0)->HaltFrame(0);
						}
				if (keyboard->GetButton(INPBUTTON_ID_X))
						if (keyboard->GetButton(INPBUTTON_ID_X)->GetState() == INPBUTTON_STATE_HOLD)
						{
								obj->GetAnimationControl()->GetStacks()->Get(0)->HaltFrame(2);
						}
				if (keyboard->GetButton(INPBUTTON_ID_C))
						if (keyboard->GetButton(INPBUTTON_ID_C)->GetState() == INPBUTTON_STATE_HOLD)
						{
								obj->GetAnimationControl()->GetStacks()->Get(0)->HaltFrame(5);
						}
				if (keyboard->GetButton(INPBUTTON_ID_V))
						if (keyboard->GetButton(INPBUTTON_ID_V)->GetState() == INPBUTTON_STATE_HOLD)
						{
								obj->GetAnimationControl()->GetStacks()->Get(0)->HaltFrame(8);
						}
				if (keyboard->GetButton(INPBUTTON_ID_B))
						if (keyboard->GetButton(INPBUTTON_ID_B)->GetState() == INPBUTTON_STATE_HOLD)
						{
								obj->GetAnimationControl()->GetStacks()->Get(0)->HaltFrame(10);
						}
				if (keyboard->GetButton(INPBUTTON_ID_N))
						if (keyboard->GetButton(INPBUTTON_ID_N)->GetState() == INPBUTTON_STATE_HOLD)
						{
								obj->GetAnimationControl()->GetStacks()->Get(0)->HaltFrame(11);
						}

				if (GRPPASSMANAGER::Get()->GetPass(__L("DebuggerPass")))
				{
						if (keyboard->GetButton(INPBUTTON_ID_F1))
								if (keyboard->GetButton(INPBUTTON_ID_F1)->GetState() == INPBUTTON_STATE_RELEASED)
										GRPPASSMANAGER::Get()->GetPass(__L("DebuggerPass"))->SetActive(true);

						if (keyboard->GetButton(INPBUTTON_ID_F2))
								if (keyboard->GetButton(INPBUTTON_ID_F2)->GetState() == INPBUTTON_STATE_RELEASED)
										GRPPASSMANAGER::Get()->GetPass(__L("DebuggerPass"))->SetActive(false);
				}

	if (this->obj)
	if (this->obj->GetLogic())
		this->obj->GetLogic()->Update();

	if (this->obj)
		if (this->obj->GetLogic())
		if (this->obj->GetLogic()->GetControllers()->Get(0))
		for (XDWORD e=0;e<keyboard->GetButtons()->GetSize();e++)
		{
			INPBUTTON* button=keyboard->GetButtons()->Get(e);	
			if (button->IsPressed())
			{											
				if (this->obj->GetLogic()->GetControllers()->Get(0)->Process(button->GetID()))
				{
					button->SetPressed(false);
					return true;
				}
			}
		}

			if(keyboard->GetButton(INPBUTTON_ID_9))
			if(keyboard->GetButton(INPBUTTON_ID_9)->IsPressedWithRelease()) 			
			{
				if (obj)
				{
					
					for (XDWORD e=0;e<obj->GetElements()->GetSize();e++)
					{
						if (obj->GetElements()->Get(e)->GetNode()->GetMaterial())
						{
							if (obj->GetElements()->Get(e)->GetNode()->GetMaterial()->GetLayers()->GetSize()>0)
							obj->GetElements()->Get(e)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOT3PerFragment")));
							else
								obj->GetElements()->Get(e)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOTDiffPerFragment")));
						}
					}
				}
						
			}
			if(keyboard->GetButton(INPBUTTON_ID_8))
			if(keyboard->GetButton(INPBUTTON_ID_8)->IsPressedWithRelease()) 			
			{
				if (obj)
				{
					
					for (XDWORD e=0;e<obj->GetElements()->GetSize();e++)
					{
						if (obj->GetElements()->Get(e)->GetNode()->GetMaterial())
						{
							if (obj->GetElements()->Get(e)->GetNode()->GetMaterial()->GetLayers()->GetSize()>0)
								obj->GetElements()->Get(e)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOT3")));
							else
								obj->GetElements()->Get(e)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOTDiff")));
						}
					}
				}
						
			}
			
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::Notify
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/01/2016 16:21:20
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASERENDEROBJECT::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{
	XQWORD delta = timer->GetMeasureMicroSeconds();
	timer->Reset();

	//XDEBUG_PRINTCOLOR(4,__L("Delta %llu"), delta);

	GRPANIMATIONSTACKMILESTONE * n = (GRPANIMATIONSTACKMILESTONE*)notifier;

	n->disabled = false;

	return false;
}
