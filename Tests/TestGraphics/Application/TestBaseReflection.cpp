
/*------------------------------------------------------------------------------------------
//	TESTBASEREFLECTION.CPP
//	
//	Reflection
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 05/08/2016 13:40:09
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBaseReflection.h"

#include "XApplication.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPVisualDebugger.h"
#include "GRPReflectionPass.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



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
bool TESTBASEREFLECTION::IniResources()
{		
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE*  scene  = xapplication->GetMainScene();
	XPATH			 xpath;

	PathManagement(__L("reflection"));

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestBaseReflection.fbx"));		
	GRPOBJECT* obj =NULL;
	obj=LoadObject(obj,xpath.Get());
	if(!obj)		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}			
	scene->Add(obj);

	obj->PlayAll();		


	XDEBUG_PRINTCOLOR(0,__L("Prepare pass"));

	scene->GetPass()->AddType(GRPPASS_TYPE_ALPHABLEND);	
	scene->GetPass()->AddType(GRPPASS_TYPE_STENCIL);
	scene->GetPass()->AddType(GRPPASS_TYPE_COLORWRITE);
	scene->GetPass()->AddType(GRPPASS_TYPE_DEPTHTEST);
	scene->GetPass()->AddType(GRPPASS_TYPE_DEPTHWRITE);
	scene->GetPass()->AddType(GRPPASS_TYPE_DEPTHFUNC);
	scene->GetPass()->AddType(GRPPASS_TYPE_CULLFACE);
	scene->GetPass()->AddType(GRPPASS_TYPE_ALPHABLEND);

	scene->GetPass()->SetClear(GRPPASS_CLEAR_FLAG_NONE);
	


											GRPELEMENT* mirror=scene->FindElementByNodeName(__L("Model::Plane001"));
											if (mirror)
											{
													mirror->SetCastsReflections(true);
													//GRPREFLECTIONPASS*	reflectpass=new GRPREFLECTIONPASS();												
													GRPPASS* reflectpass = GRPPASSMANAGER::Get()->Create(__L("reflection"));
													xapplication->GetMainScene()->Add(reflectpass);

													dynamic_cast<GRPREFLECTIONPASS*>(reflectpass)->GetReflectionCasters();

													xapplication->GetMainScene()->GetPass()->Remove(mirror);
													mirror->SetBlending(true);
											}

								
XDEBUG_PRINTCOLOR(0,__L("Start"));
											
	//scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_ALL);  
	//scene->ShowDebug(false);

	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEREFLECTION::EndResources
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
bool TESTBASEREFLECTION::EndResources()
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
bool TESTBASEREFLECTION::Do()
{	
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/01/2016 16:21:20
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASEREFLECTION::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{

	return false;
}
