
/*------------------------------------------------------------------------------------------
//	TESTBASEGGF.CPP
//	
//	Tests ggf
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 31/03/2017 13:20:07
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBaseGGF.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPScene.h"
#include "GRPCamera.h"
#include "GRPLight.h"
#include "GRPObjectBuilder.h"
#include "GRPFontManager.h"

#include "GRPFile3DGGF.h"

#include "XSleep.h"

#include "XSystem.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASEGGF::TESTBASEGGF
*/	
/**	
//	
//	Class Constructor TESTLAYERS
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/11/2016 10:09:56
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASEGGF::TESTBASEGGF(GRPAPPLICATION* xapplication) : TESTBASE(xapplication)
{	
	blendingpass=NULL;
	loaderInterface=NULL;
	object=NULL;
		file3d=NULL;	

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);
		PathManagement(__L("GGF"));

	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("JackpotCasinoRoulette.xml"));					
	//xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("JackpotCasinoRoulette_GGF.xml"));			
	//xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("GGF_Output.GGF"));			
	//xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("GGF_Output.XML"));
	//xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("morph.FBX"));

}


/*-------------------------------------------------------------------
//	TESTBASEGGF::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/11/2016 10:10:05
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool			TESTBASEGGF::IniResources()
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	scene  = xapplication->GetMainScene();

	object=NULL;

	XDEBUG_PRINTCOLOR(0,__L("------------------ Loading %s"),xpath.Get());

	XTIMER* timer=xfactory->CreateTimer();
	timer->Reset();			
	object= LoadObject(object,xpath.Get());
	if(!object)		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}			
	XDEBUG_PRINTCOLOR(0,__L("------------------- LOAD TIME: %d"),timer->GetMeasureMilliSeconds());

	scene->Add(object);	

		//----------------------------------- setup opacity									
									GRPBLENDINGPASS*	blendingpass=new GRPBLENDINGPASS();																		
																		scene->Add(blendingpass);
	
																		object->PlayAll();
//	if (file3d!=NULL)
//		delete(file3d);

	//	 file3d=new GRPFILE3DGGF();
	
		delete(timer);
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASEGGF::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/11/2016 10:10:14
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool			TESTBASEGGF::EndResources()
{
	delete file3d;
	file3d=NULL;
	this->object=NULL;
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEGGF::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/11/2016 10:10:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool			TESTBASEGGF::Do()
{
	if (this->object)
	if (this->object->GetLogic())
		this->object->GetLogic()->Update();

	return true;
}

/*-------------------------------------------------------------------
//	TESTLAYERS::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/11/2016 10:10:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool			TESTBASEGGF::InputManagement()
{
	if (this->status==STATE_LOADING)
	if(keyboard->GetButton(INPBUTTON_ID_ESCAPE))
	if(keyboard->GetButton(INPBUTTON_ID_ESCAPE)->IsPressedWithRelease()) 			
	{
		this->loaderInterface->Abort();
		xapplication->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER); 
		return true;
	}
	
	if(keyboard->GetButton(INPBUTTON_ID_F1))
		if(keyboard->GetButton(INPBUTTON_ID_F1)->GetState()==INPBUTTON_STATE_RELEASED)
		{
			while (keyboard->GetButton(INPBUTTON_ID_F1)->GetState()!=INPBUTTON_STATE_UP)
				keyboard->Update();

			XDEBUG_PRINTCOLOR(0,__L("Saving"));
			this->Save();
			XDEBUG_PRINTCOLOR(0,__L("Saved"));
			return true;
		}

	if(keyboard->GetButton(INPBUTTON_ID_F1))
		if(keyboard->GetButton(INPBUTTON_ID_F1)->IsPressedWithRelease())
		{
			return true;
		}
	

	if(keyboard->GetButton(INPBUTTON_ID_F4))
	if(keyboard->GetButton(INPBUTTON_ID_F4)->GetState()==INPBUTTON_STATE_RELEASED)		
	{
		while (keyboard->GetButton(INPBUTTON_ID_F4)->GetState()!=INPBUTTON_STATE_UP)
				keyboard->Update();

		XDEBUG_PRINTCOLOR(0,__L("Loading"));
		XTIMER* timer=xfactory->CreateTimer();
			timer->Reset();			
		this->Load();
		XDEBUG_PRINTCOLOR(0,__L("------------------- LOAD TIME: %d"),timer->GetMeasureMilliSeconds());

		delete(timer);

		return true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_1))
	if(keyboard->GetButton(INPBUTTON_ID_1)->GetState()==INPBUTTON_STATE_RELEASED)		
	{
		if (object->GetAnimationControl()->GetStacks()->Get(0))
		{
			object->GetAnimationControl()->GetStacks()->Get(0)->SetActive(true);
			object->GetAnimationControl()->Play();
		}

	}

		if (this->object)
		if (this->object->GetLogic())
		if (this->object->GetLogic()->GetControllers()->Get(0))
		for (XDWORD e=0;e<keyboard->GetButtons()->GetSize();e++)
		{
			INPBUTTON* button=keyboard->GetButtons()->Get(e);	
			if (button->IsPressed())
			{											
				if (this->object->GetLogic()->GetControllers()->Get(0)->Process(button->GetID()))
				{
					button->SetPressed(false);
					return true;
				}
			}
		}
	TESTBASE::InputManagement();
	return true;
}

/*-------------------------------------------------------------------
//	TESTLAYERS::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/11/2016 10:10:49
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool			TESTBASEGGF::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{
	GRPNAMED_TYPE type=(GRPNAMED_TYPE)notifier->GetSubjectType();
	
	switch(type)
	{
	case GRPNAMED_TYPE_TASKMANAGER:
		switch(msg)
		{
			case GRPMSG_END:
				this->status=STATE_SETUP;
				return true;
		}
		break;
	case GRPNAMED_TYPE_TASK:
			switch(msg)
			{
			case GRPMSG_EXIT:
				{
					GRPLOADERTASK* task=dynamic_cast<GRPLOADERTASK*>(notifier);
					if (task)
					{
						if (task->object)
						delete(task->object);
					}
					this->status=STATE_FINISHING;				
				}
				return true;
				break;
			case GRPMSG_STEP:
				return true;
				break;
			
				break;
			};
		break;
	}
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEGGF::Save
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/04/2017 11:15:48
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEGGF::Save()
{
	XPATH xpath;
	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("GGF_Output.GGF"));		

	if (!file3d)
		file3d=new GRPFILE3DGGF();

	if (file3d)
	{
		dynamic_cast<GRPFILE3DGGF*>(file3d)->Init();
		dynamic_cast<GRPFILE3DGGF*>(file3d)->SetScene(this->scene);

		object->GetAnimationControl()->Begin();
		if (!file3d->Save(xpath.Get()))
		{
			XDEBUG_PRINTCOLOR(4,__L("Failed saving GGF file"));
			return false;
		}
		object->GetAnimationControl()->Play();
	}

	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEGGF::Save
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/04/2017 11:15:48
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEGGF::Load()
{
	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("GGF_Output.GGF"));		
	this->EndResources();
	scene->End();
	
	scene->Ini(GRPSCENE_INIT_FLAGS_CREATE_PASS | GRPSCENE_INIT_FLAGS_CREATE_VIEWPORT);
	this->IniResources();

	return true;
}