
/*------------------------------------------------------------------------------------------
//	TESTBASEENVIRONMENTMAPPING.CPP
//	
//	Tests Environment mapping
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 03/07/2017 11:40:06
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBaseEnvironmentMapping.h"
#include "GRPMaterialLibrary.h"
#include "GRPScene.h"
#include "GRPLight.h"
#include "GRPEnvMapPass.h"

	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASEENVIRONMENTMAPPING::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 11:47:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEENVIRONMENTMAPPING::IniResources()
{
	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("shadows"));

	XPATH xpath;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("EnvMap.FBX"));
	GRPOBJECT* obj=NULL;
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}
	scene->Add(obj);

	target=NULL;

		//-------------

	GRPNODE*				envnode	=	scene->FindNode(__L("Model::EnvMapTarget"));

														envpass = GRPPASSMANAGER::Get()->Create(__L("environment"),0);																		
														envpass->SetNode(envnode);

														scene->Add				(envpass,true);																	
														GRPELEMENT* element	=		scene->FindElementByNodeName(__L("Model::EnvMapTarget"));														
														if (element)
																element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("CubeMap")));

														obj->PlayAll();

														//scene->GetCamera()->SetPosition(-40.0,50.0,14.0);
														scene->GetLights()->GetLast()->SetStrength(30.0f);
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEENVIRONMENTMAPPING::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 11:47:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEENVIRONMENTMAPPING::EndResources()
{
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEENVIRONMENTMAPPING::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 11:47:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEENVIRONMENTMAPPING::Do()
{ 
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEENVIRONMENTMAPPING::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 11:47:50
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASEENVIRONMENTMAPPING::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
	return true;
}
