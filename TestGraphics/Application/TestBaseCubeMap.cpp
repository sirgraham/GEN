
/*------------------------------------------------------------------------------------------
//	TESTBASECUBEMAP.CPP
//	
//	Test Base Cubemap
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 30/05/2016 16:19:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	

#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"

#include "TestBaseCubeMap.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	TESTBASECUBEMAP::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/05/2016 22:31:29
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASECUBEMAP::IniResources()
{	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("cubemap"));

	//----------------------------------- material default location
	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	GRPOBJECT* obj=NULL;
	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestCubeMap.fbx"));		
 	obj= LoadObject(obj,xpath.Get());
	if(!obj)		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			delete(obj);
			return false;
		}			
	scene->Add(obj);




	XVECTOR<XFSTRING*> cube;
	XFSTRING * s=NULL;
	
	
						s=new XFSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("sky_right.png"));		s->Set(xpath.Get());	cube.Add(s);
						s=new XFSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("sky_left.png"));			s->Set(xpath.Get());	cube.Add(s);
						s=new XFSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("sky_bottom.png"));		s->Set(xpath.Get());	cube.Add(s);
						s=new XFSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("sky_top.png"));			s->Set(xpath.Get());	cube.Add(s);
						s=new XFSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("sky_front.png"));		s->Set(xpath.Get());	cube.Add(s);
						s=new XFSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("sky_back.png"));			s->Set(xpath.Get());	cube.Add(s);
						

/*						
						s=new XSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("left.jpg"));			s->Set(xpath.Get());	cube.Add(s);
						s=new XSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("right.jpg"));		s->Set(xpath.Get());	cube.Add(s);										
						s=new XSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("down.jpg"));			s->Set(xpath.Get());	cube.Add(s);						
						s=new XSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("up.jpg"));				s->Set(xpath.Get());	cube.Add(s);						
						s=new XSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("front.jpg"));		s->Set(xpath.Get());	cube.Add(s);
						s=new XSTRING();	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("back.jpg"));			s->Set(xpath.Get());	cube.Add(s);
*/
									
	if (GRPMATERIALLIBRARY::Get()->Load(&cube,__L("cubemap"))==GRPMATERIALLOADRESULT_ERROR)
		return false;

	GRPMATERIAL* skycube=GRPMATERIALLIBRARY::Get()->Select(__L("cubemap"));

	obj->SetMaterial(skycube);
	obj->SetProgram	(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"CUB"));
	obj->GetAnimationControl()->SetStateStack(0,true);
	obj->PlayAll();		

	
//	GRPSKYBOXPASS* pass=new GRPSKYBOXPASS();
	GRPPASS* pass=GRPPASSMANAGER::Get()->Create(__L("Skybox"));
								 pass->SetMaterial(skycube);
								 scene->Add(pass);
	

	cube.DeleteContents();
	cube.DeleteAll();

	scene->visualdebugger->CreateScene(GRPVISUALDEBUGGER_FLAG_ALL);  
	scene->ShowDebug(GRPDEBUG_FLAG_NORMAL);
	//scene->GetLights()->GetLast()->SetMultiplier(50.0f);
	//scene->SelectCamera(obj->GetCamera());
	//lockcamera = true;

	return true;
}





/*-------------------------------------------------------------------
//	TESTBASECUBEMAP::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/05/2016 22:31:18
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASECUBEMAP::Do()
{
	return true;	
}

/*-------------------------------------------------------------------
//	TESTBASECUBEMAP::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:48:13
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASECUBEMAP::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEOBJMERGING::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:52:06
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASECUBEMAP::EndResources()
{
	return true;
}