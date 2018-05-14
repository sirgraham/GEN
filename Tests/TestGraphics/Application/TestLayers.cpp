
/*------------------------------------------------------------------------------------------
//	TESTLAYERS.CPP
//	
//	Test layer loading from xml
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 28/11/2016 10:05:40
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPFile3DXML.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "testlayers.h"
#include "GRPScene.h"
#include "GRPCamera.h"
#include "GRPBlendingPass.h"
#include "GRPObjectBuilder.h"
#include "GRPObjectLibrary.h"
 
#include "GRPConsole.h"


#include "SNDFactory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTLAYERS::TESTLAYERS
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
TESTLAYERS::TESTLAYERS(GRPAPPLICATION* xapplication) : TESTBASE(xapplication)
{	
	blendingpass=NULL;
	loaderInterface=NULL;
	object=NULL;
}

/*-------------------------------------------------------------------
//	TESTLAYERS::IniResources
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
bool			TESTLAYERS::IniResources()
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;
	
	scene  = xapplication->GetMainScene();


	PathManagement(__L("layers"));

	if (!Load())
		return false;
	
	
	return true;
}

/*-------------------------------------------------------------------
//	TESTLAYERS::Load
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/12/2016 17:47:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	TESTLAYERS::Load()
{
	//----------------------------------- material default location		
	XPATH			 xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);



	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("JackpotCasinoRoulette.xml"));		

	loaderInterface=new GRPLOADERINTERFACE(scene,__L("gen_logo.jpg"),GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"LDB"));
	loaderInterface->AddObserver(this);
	loaderInterface->Init();	

	if (!loaderInterface->AddTask(xpath.Get()))
		return false;
	
//	InitConsole();
	//this->loaderInterface->AttachConsole(console);


	loaderInterface->GetLoader()->Start();
	this->status=STATE_LOADING;

	return true;
}



/*-------------------------------------------------------------------
//	TESTLAYERS::EndResources
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
bool			TESTLAYERS::EndResources()
{
	return true;
}

/*-------------------------------------------------------------------
//	TESTLAYERS::Do
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
bool			TESTLAYERS::Do()
{
	switch(this->status)
	{
	case STATE_NONE:
	case STATE_LOADING:
		break;

	case STATE_SETUP:
		{
			
					GRPLOADERTASK* loaderTask=static_cast<GRPLOADERTASK*>(loaderInterface->GetLoader()->GetTasks()->Get(0));								
					this->object=loaderTask->object;
					
					this->loaderInterface->Unsubscribe();
					this->loaderInterface->GetObservers()->DeleteAll();
					this->loaderInterface->End();
					delete(this->loaderInterface);
					this->loaderInterface=NULL;
						
					this->object->GetAnimationControl()->Play();														
																		
																		blendingpass=GRPPASSMANAGER::Get()->Create(__L("blending"));
																		scene->Add(blendingpass);

							scene->ReorderScene();
							
							
								for (XDWORD e=0;e<blendingpass->GetRenderOrder()->GetSize();e++)
								{
									GRPELEMENT* element=blendingpass->GetRenderOrder()->Get(e);
									element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
								}
								
							blendingpass->SetActive(true);

							if (console)
								console->Clear();

				this->status=STATE_IDLE;
				
				
			
				}
		break;
	case STATE_IDLE:
		if (this->object)
		this->object->GetLogic()->Update();
		break;

	case STATE_FINISHING:
		{
					if (this->loaderInterface)
					{						
							this->loaderInterface->Unsubscribe();
							this->loaderInterface->GetObservers()->DeleteAll();
							this->loaderInterface->End();
							delete(this->loaderInterface);
							this->loaderInterface=NULL;
					}
					
		}
		return false;	
	}

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
bool			TESTLAYERS::InputManagement()
{
	if (this->status==STATE_LOADING)
	if(keyboard->GetButton(INPBUTTON_ID_ESCAPE))
	if(keyboard->GetButton(INPBUTTON_ID_ESCAPE)->IsPressedWithRelease()) 			
	{
		this->loaderInterface->Abort();
		xapplication->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER); 
		return true;
	}

	if (this->status==STATE_IDLE)
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

		//*/
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
bool			TESTLAYERS::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
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
