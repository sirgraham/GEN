
/*------------------------------------------------------------------------------------------
//	TESTBASEPHY.CPP
//	
//	Test Base Physical
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/05/2016 16:19:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	

	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include <inttypes.h>
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

#include "GRPCamera.h"
#include "GRPViewport.h"

#include "PHYEngine.h"
#include "PHYWorld.h"

#include "TestBasePhy.h"

#include "XMemory.h"

	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	TESBASEPHY::IniResources
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			31/05/2016 16:29:03
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEPHY::IniResources()
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	 scene  = xapplication->GetMainScene();

	PathManagement(__L("phy"));

	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestPHY2.fbx"));

	GRPOBJECT* obj=NULL;

	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("\t\tLoading FBX Failed!"));
			return false;
		}			

	scene->Add(obj);
	
#ifdef PHY_ACTIVE
							
							XDEBUG_PRINTCOLOR(3, __L("Init physics engine"));						
							engine= PHYENGINE::Create(__L("PHYSX"));
							if (!engine->Init(scene))
							{
									XDEBUG_PRINTCOLOR(4, __L("Init physics engine failed!"));
									return false;
							}

							XDEBUG_PRINTCOLOR(3, __L("Init Objects"));

							if (obj->GetElements()->Get(1))						engine->GetWorld()->CreateEntity(obj->GetElements()->Get(1),PHYENVELOPETYPE_BOX,		PHYENTITYTYPE_DYNAMIC);
							if (obj->GetElements()->Get(2))						engine->GetWorld()->CreateEntity(obj->GetElements()->Get(2),PHYENVELOPETYPE_BOX,		PHYENTITYTYPE_DYNAMIC);
							if (obj->GetElements()->Get(3))						engine->GetWorld()->CreateEntity(obj->GetElements()->Get(3),PHYENVELOPETYPE_SPHERE,	PHYENTITYTYPE_DYNAMIC);
							/*
								Player = obj->FindByName(__L("Model::Player"));

							if (Player)
									engine->GetWorld()->CreateEntity(Player,PHYENVELOPETYPE_BOX, PHYENTITYTYPE_KINEMATIC);
						*/	
							loc=scene->GetVisualDebugger()->CreateLocator(scene->CreateNode());
							loc2=scene->GetVisualDebugger()->CreateLocator(scene->CreateNode());
							scene->Add(loc);
							scene->Add(loc2);
#else

		XDEBUG_PRINTCOLOR(4,__L(""));
		XDEBUG_PRINTCOLOR(4,__L(""));
		XDEBUG_PRINTCOLOR(4,__L("\t\tThis Demo requires to be compiled with PHY_ACTIVE preprocessor directive and PHYSX."));
		XDEBUG_PRINTCOLOR(4,__L(""));
		XDEBUG_PRINTCOLOR(4,__L(""));		
#endif
	//*/
	return true;
}




/*-------------------------------------------------------------------
//	TESTPHY::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/04/2016 12:36:03
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEPHY::Do()
{
	#ifdef PHY_ACTIVE		
		this->engine->Step();
	#endif
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEPHY::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2016 13:22:21
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEPHY::EndResources()
{
return true;
}


/*-------------------------------------------------------------------
//	TESTBASEPHY::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/05/2017 20:51:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/


bool TESTBASEPHY::InputManagement()
{
#ifdef PHY_ACTIVE

		GRPCAMERA* cam=scene->GetCameras()->Get(scene->GetCurrentCamera());
		if (cam)
				if (mouse)
				{
						if (mouse->GetButtons()->GetSize())
						if (mouse->GetButton(0)->IsPressed())
						{
								/*
								INPCURSOR* cursor=mouse->GetCursor(0);
	
								cam->SetViewport(scene->GetViewport());
								const GRPRAY r=cam->CreateRayFromScreenPoint(cursor->GetX(),cursor->GetY());
								//const GRPRAY r=cam->CreateRayFromScreenPoint(1280/4,780/4);

physx::PxRaycastBuffer hit;                 // [out] Raycast results

physx::PxVec3 origin		=physx::PxVec3(r.start.x,							r.start.y,							r.start.z);
//physx::PxVec3 origin		=physx::PxVec3(cam->GetPosition()->vector[0],cam->GetPosition()->vector[1],cam->GetPosition()->vector[2]);
physx::PxVec3 unitDir		=physx::PxVec3(r.direction.vector[0], r.direction.vector[1],	r.direction.vector[2]);

bool status = dynamic_cast<PHYSXWORLD*>(engine->GetWorld())->gScene->raycast(origin, unitDir, 10000.0f, hit,physx::PxSceneQueryFlags(0xffff));
		/*
								char debug[100];
								sprintf(debug,"Mouse %f %f Ray [%f,%f,%f]->[%f,%f,%f] Hit %f %f %f \n\n",
										cursor->GetX(),cursor->GetY(),
										r.start.x,r.start.y,r.start.z,
										r.direction.vector[0],r.direction.vector[1],r.direction.vector[2],
										hit.block.position.x,hit.block.position.y,hit.block.position.z);
							//	OutputDebugStringA(debug);
							     
								if (status)
								{
									loc->GetElements()->Get(0)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"GRN"));
									loc->GetElements()->Get(0)->GetNode()->SetPosition(hit.block.position.x,hit.block.position.y,hit.block.position.z);
									loc2->GetElements()->Get(0)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BLU"));
									loc2->GetElements()->Get(0)->GetNode()->SetPosition(hit.block.position.x,0.0,hit.block.position.z);
								}
								else
									loc->GetElements()->Get(0)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"RED"));
									*/
								return true;
						}
				}

		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_W))
				if (keyboard->GetButton(INPBUTTON_ID_W)->GetState() == INPBUTTON_STATE_HOLD)
				{
						const GRPVECTOR* v = Player->GetNode()->GetPosition();
						Player->GetNode()->SetPosition(v->vector[0]+0.01f, v->vector[1], v->vector[2]);
				}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_S))
				if (keyboard->GetButton(INPBUTTON_ID_S)->GetState()==INPBUTTON_STATE_HOLD)
				{
						const GRPVECTOR* v = Player->GetNode()->GetPosition();
						Player->GetNode()->SetPosition(v->vector[0] + 0.01f, v->vector[1], v->vector[2]);
				}
#endif
				return TESTBASE::InputManagement();
}

