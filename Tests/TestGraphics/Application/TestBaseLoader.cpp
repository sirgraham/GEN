/*------------------------------------------------------------------------------------------
//	TESTBASELOADER.H
*/	
/**	
// \class 
//   
//  Test Base Loader
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 2/09/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBaseLoader.h"
#include "GRPScene.h"
#include "GRPObject.h"
#include "GRPCamera.h"
#include "GRPScreen.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPProfiler.h"
#include "GRPObjectLibrary.h"
#include "GRPLoader.h"
#include "GRPRenderer.h"
#include "XApplication.h"

#include "GRPLoaderInterface.h"

#include "SNDFactory.h"
#include "SNDElement.h"
#include "SNDSource.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASELOADER::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/09/2016 14:00:12
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASELOADER::IniResources()
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE*		scene  = xapplication->GetMainScene();
	GRPSCREEN*	screen = this->xapplication->GetMainScreen();

 	PathManagement(__L("loader"));

	OPENGLCHECKERROR(__L("TESTBASELOADER::GetMainScene"));
		
	loaderInterface=new GRPLOADERINTERFACE(scene,__L("innomatic_alpha.jpg"),GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"INN"));
	loaderInterface->AddObserver(this);
	loaderInterface->Init();										
	
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASELOADER::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/09/2016 14:00:21
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASELOADER::EndResources()
{
	GRPOBJECTBUILDER::Destroy();
	delete(this->loaderInterface);
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASELOADER::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/09/2016 14:00:36
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASELOADER::Do()
{ 	
	return true;

}

/*-------------------------------------------------------------------
//	TESTBASELOADER::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/09/2016 14:00:43
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/



bool TESTBASELOADER::InputManagement()
{		
#ifndef ANDROID
	if(keyboard->GetButton(INPBUTTON_ID_1))
	if(keyboard->GetButton(INPBUTTON_ID_1)->IsPressed()) 	
#endif
	{
		if (!block)
		{
		block=true;
		XPATH xpath;
		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("facerig.fbx"));	
		GRPOBJECTLIBRARY::Get()->GetLoader()->AddObserver	(this);		
		
		GRPLOADERTASK*	task=(GRPLOADERTASK*)GRPOBJECTLIBRARY::Get()->GetLoader()->CreateTask		(xpath.Get());		
		
		GRPOBJECTLIBRARY::Get()->GetLoader()->AddTask	(task);		
		GRPOBJECTLIBRARY::Get()->GetLoader()->Start				();
		
		
		}
		
	}

	if(keyboard->GetButton(INPBUTTON_ID_2))
	if(keyboard->GetButton(INPBUTTON_ID_2)->IsPressed()) 	
	{
		if (!block)
		{
		block=true;
		XPATH xpath;
		
	 	PathManagement(__L("renderer"));
		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("base.FBX"));	
		GRPOBJECTLIBRARY::Get()->GetLoader()->AddObserver	(this);
		GRPLOADERTASK*	task=(GRPLOADERTASK*)GRPOBJECTLIBRARY::Get()->GetLoader()->CreateTask		(xpath.Get());										
										task->AddLayer(__L("0_00_02_repose_superruleta.FBX"),0,GRPFILE3D_INCLUDE_ANIMATION);
										task->AddLayer(__L("0_00_02_repose_subruleta.FBX")	,1,GRPFILE3D_INCLUDE_ANIMATION);

		GRPOBJECTLIBRARY::Get()->GetLoader()->AddTask	(task);
		GRPOBJECTLIBRARY::Get()->GetLoader()->Start		();
			
		}
		
	}


	return TESTBASE::InputManagement();
}

/*-------------------------------------------------------------------
//	TESTBASELOADER::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/09/2016 14:00:49
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASELOADER::Notify(GRPMSGTYPE msg, GRPSUBJECT* subject)
{
	switch(msg)
	{
	case GRPMSG_EXIT:  //crashed load
				if (subject->GetSubjectType()==GRPNAMED_TYPE_TASK)
				{
					GRPLOADERTASK* task=static_cast<GRPLOADERTASK*>(subject);
					if (task)
					{
						xapplication->SetExitStatus(XAPPLICATIONEXITTYPE_APPERROR);
						return false;
					}
				}
				break;

	case GRPMSG_END: 
		if (subject->GetSubjectType()==GRPNAMED_TYPE_TASK)
		{
			GRPLOADERTASK* task=static_cast<GRPLOADERTASK*>(subject);
			if (task)
			{
				 if (	task->object->GetAnimationControl()->GetStacks()->GetSize()>0)
				 {
							task->object->GetAnimationControl()->GetStacks()->Get(0)->SetActive(true);

				 if (	task->object->GetAnimationControl()->GetStack(0))
							task->object->GetAnimationControl()->GetStack(0)->SetActive(true);
				 if (	task->object->GetAnimationControl()->GetStack(1))
							task->object->GetAnimationControl()->GetStack(1)->SetActive(true);
							task->object->GetAnimationControl()->Play();
				 }
		
				 this->xapplication->GetMainScene()->Add(task->object);				 
			}
		}
		break;
	}
	return true;
}


