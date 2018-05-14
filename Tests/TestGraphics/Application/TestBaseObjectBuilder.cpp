/*------------------------------------------------------------------------------------------
//	TESTBASEOBJECTBUILDER.H
*/	
/**	
// \class 
//   
//  Test Base Object Builder
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 2/09/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPScene.h"
#include "GRPCamera.h"
#include "GRPObject.h"
#include "GRPNode.h"

#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "TestBaseObjectBuilder.h"

#include "GRPObjectLibrary.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASEOBJECTBUILDER::TESTBASEOBJECTBUILDER
*/	
/**	
//	
//	Class Constructor TESTBASEOBJECTBUILDER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:09:42
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASEOBJECTBUILDER::TESTBASEOBJECTBUILDER(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  
{
block=false;
scale = 10.0f;
} ;

/*-------------------------------------------------------------------
//	TESTBASEOBJECTBUILDER::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:09:49
//	
//	@return 			ool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJECTBUILDER::IniResources()
{
	PathManagement(__L("objectbuilder"));

	xapplication->GetMainScene()->GetPass()->SetClearColor(GRPCOLOR(0.2f,0.3f,0.4f));

	scene  = xapplication->GetMainScene();	
	
	GRPELEMENT* element=GRPOBJECTBUILDER::Get()->CreatePlane();
							element->SetNode(scene->CreateNode());
							element->GetNode()->SetScale(scale*2.0f);
	elements.Add(element);
	scene->GetPass()->Add(element);
	

	GRPSHADERPROGRAM* shader=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX");
										element->SetProgram(shader);

	
	//----------------------------------- material default location

	XPATH xpath;
				xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("coord.png"));	

	if(!GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),	__L("coord.png")))
		{
			XDEBUG_PRINTCOLOR(4,__L("TESTBASEOBJECTBUILDER::IniResources() : Error loading Texture data"));
			return false;
		}				
				xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("gen_logo.jpg"));	
	if(!GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),	__L("gen_logo.jpg")))
		{
			XDEBUG_PRINTCOLOR(4,__L("TESTBASEOBJECTBUILDER::IniResources() : Error loading Texture data"));
			return false;
		}				
	GRPMATERIAL * material=GRPMATERIALLIBRARY::Get()->Select(	__L("coord.png"));
	element->GetNode()->SetMaterial(material);

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("obj1.fbx"));	
	
//	scene->Add(this->LoadObject(NULL,xpath.Get()));
//	scene->Add(this->LoadObject(NULL,xpath.Get()));
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEOBJECTBUILDER::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:09:56
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJECTBUILDER::EndResources()
{
	elements.DeleteContents();
	elements.DeleteAll();


	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEOBJECTBUILDER::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:10:01
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJECTBUILDER::Do()
{
	/*
	for (XDWORD e=0;e<this->tasks.GetSize();e++) //epic application thread manager
	{
		if (tasks.Get(e)->thread->GetStatusFunc()==XTHREADSTATUS_END)
		{
			ObjectLoadTask* thread=tasks.Get(e);
			tasks.DeleteIndex(e);
			delete(thread);
			return true;
		}
	}

	for (XDWORD e=0;e<scene->GetObjects()->GetSize();e++)
	{
		GRPOBJECT* object=scene->GetObjects()->Get(e);
		const GRPVECTOR* pos=object->GetNode()->GetPosition();
		if (pos->vector[1]>0.01f)
			object->GetNode()->SetPosition(pos->vector[0],pos->vector[1]-0.1f,pos->vector[2]);
	}
	*/
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEOBJECTBUILDER::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:10:08
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJECTBUILDER::InputManagement()
{	
	if(keyboard->GetButton(INPBUTTON_ID_1))
	if(keyboard->GetButton(INPBUTTON_ID_1)->IsPressedWithRelease()) 	
	{
			GRPELEMENT*	element=new GRPELEMENT();
			element->Reference(GRPOBJECTBUILDER::Get()->CreateCube());
									element->SetNode(scene->CreateNode());
									element->GetNode()->SetScale(10.0f);
									GRPSHADERPROGRAM* shader=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX");
									element->SetProgram(shader);
									element->GetNode()->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(	__L("gen_logo.jpg")));
			scene->GetPass()->Add(element);

			GRPOBJECTLIBRARY::Get()->IndexElement(__L("master"),element);
			elements.Add(element);

	}
	if(keyboard->GetButton(INPBUTTON_ID_2))
	if(keyboard->GetButton(INPBUTTON_ID_2)->IsPressedWithRelease()) 	
	{
		static int i=0;
		GRPELEMENT*	element=GRPOBJECTLIBRARY::Get()->CreateElement(__L("master"));	
		if (element)
		{											
									element->GetNode()->SetScale(scale);
									element->GetNode()->SetPosition(scale*i++,0.0f,0.0f);
									scene->Add(element->GetNode());
			scene->GetPass()->Add(element);
			elements.Add(element);			
		}
	}

	if(keyboard->GetButton(INPBUTTON_ID_3))
	if(keyboard->GetButton(INPBUTTON_ID_3)->IsPressed()) 	
	{
		if (!block)
		{
		block=true;
		XPATH xpath;
		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("obj1.fbx"));	
		//this->LoadObjectParallel(NULL,xpath.Get());
		//scene->Add(this->LoadObject(NULL,xpath.Get())); 

		}
	}

	if(keyboard->GetButton(INPBUTTON_ID_4))
	if(keyboard->GetButton(INPBUTTON_ID_4)->IsPressed()) 	
	{
		GRPELEMENT*	element=GRPOBJECTLIBRARY::Get()->CreateElement(__L("master"));	
		if (element)
		{											
									
									GRPOBJECT*	obj=new GRPOBJECT();
															obj->SetNode(scene->CreateNode());															
															element->GetNode()->SetScale(0.1f);
															element->GetNode()->SetPosition(0.1f,500.5f,0.0f);
															element->GetNode()->SetParent(obj->GetNode());
															obj->Add(element);

									//scene->Add(element->GetNode());
									scene->Add(element->GetNode());
									scene->Add(obj->GetNode());
									scene->Add(obj);									
		}
	}


return TESTBASE::InputManagement();
}

/*-------------------------------------------------------------------
//	TESTBASEOBJECTBUILDER::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:10:23
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEOBJECTBUILDER::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{

	if (msg==GRPMSG_END)
	{
		/*
		ObjectLoadTask* olt=static_cast<ObjectLoadTask*>(notifier);
		if (olt)
		{
			xapplication->GetMainScene()->Add(olt->object);
			olt->object->GetNode()->SetScale(0.2f);
			olt->object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
			block=false;
		}
		*/
	}
	return true;					

}


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

