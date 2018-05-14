
/*------------------------------------------------------------------------------------------
//	TESTBASEMERGEANIMATIONLAYERS.CPP
//	
//	Test Base Merge Animation Layers
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
#include "GRPCamera.h"
#include "GRPLight.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"

#include "TestBaseMergeAnimationLayers.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	TESTBASEMERGEANIMATIONLAYERS::IniResources
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
bool TESTBASEMERGEANIMATIONLAYERS::IniResources()
{	

	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("mergeanimationlayers"));

	XPATH xpath;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	GRPOBJECT* obj=NULL;

 	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestMergeAnimationLayers_A.fbx"));
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestMergeAnimationLayers_B.fbx"));
	if(!(LoadObject(obj,xpath.Get(),GRPFILE3D_INCLUDE_ANIMATION | GRPFILE3D_INCLUDE_NEW_GEOMETRY | GRPFILE3D_INCLUDE_MATERIALS,GRPFILE3D_VERBOSE_LEVEL_LOW,1)))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}

	/*
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestMergeAnimationLayers_B.fbx"));
	if(!(LoadObject(obj,xpath.Get(),GRPFILE3D_INCLUDE_ANIMATION | GRPFILE3D_INCLUDE_NEW_GEOMETRY | GRPFILE3D_INCLUDE_MATERIALS)))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}
	*/
	obj->MergeAnimations();		
	for (XDWORD e=0;e<obj->GetAnimationControl()->GetStacks()->GetSize();e++)
		obj->GetAnimationControl()->GetStacks()->Get(e)->SetActive(true);

	obj->PlayAll();		

	scene->Add(obj);

	for(XDWORD e=0; e<scene->GetNodes()->GetSize(); e++)
		{
			GRPNODE*   node = scene->GetNodes()->Get(e);
			GRPCAMERA* cam  = dynamic_cast<GRPCAMERA*>(node);
			if(cam != NULL)
				{
					XDEBUG_PRINTCOLOR(0,__L("Camera : [%d] %s "),e,node->GetName()->Get());
				}
			 else
				{
					GRPLIGHT* light=dynamic_cast<GRPLIGHT*>(node);

					if(light)
						{
							XDEBUG_PRINTCOLOR(0,__L("Light  : [%d] %s "),e,node->GetName()->Get());
						}
					 else
						{
						  XDEBUG_PRINTCOLOR(0,__L("Node	  : [%d] %s "),e,node->GetName()->Get());
						}
				}
		
		}

	scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_ALL);  scene->ShowDebug(GRPDEBUG_FLAG_NORMAL);
	obj->GetLights()->GetLast()->SetMultiplier(5.0f);
	XDEBUG_PRINTCOLOR(0,__L("Resources Initialized"));

	return true;
}





/*-------------------------------------------------------------------
//	TESTBASEMERGEANIMATIONLAYERS::Do
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
bool TESTBASEMERGEANIMATIONLAYERS::Do()
{
	
	
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEMERGEANIMATIONLAYERS::Notify
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
bool TESTBASEMERGEANIMATIONLAYERS::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEMERGEANIMATIONLAYERS::Notify
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
bool TESTBASEMERGEANIMATIONLAYERS::EndResources()
{
	return true;
}