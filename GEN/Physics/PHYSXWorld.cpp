/*------------------------------------------------------------------------------------------
//	PHYSXWORLD.cpp
*/	
/**	
// \class 
//   
//  PhysX world
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 26/05/2016 19:52:35
//	Last Modification	:	
*/	
/*	(C) Copyright GEN. All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifdef PHY_ACTIVE

/*---- INCLUDES --------------------------------------------------------------------------*/
		
#include "PHYSXWorld.h"
#include "PHYSXEntity.h"
#include "GRPScene.h"
#include "XTimer.h"
#include "XFactory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
bool PHYSXWORLD::Init(PHYENGINE* engine)
{
		timer = xfactory->CreateTimer();
		timer->Reset();

		this->engine=dynamic_cast<PHYSXENGINE*>(engine);
		if (!this->engine) return false;

												//Creating scene
											  sceneDesc=new physx::PxSceneDesc(this->engine->gPhysicsSDK->getTolerancesScale());
												sceneDesc->gravity = physx::PxVec3(0.0f, -98.0f, 0.0f);
												sceneDesc->cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
												sceneDesc->filterShader	= physx::PxDefaultSimulationFilterShader;
						gScene = this->engine->gPhysicsSDK->createScene(*sceneDesc);

		
							//Creating material
		mMaterial =		this->engine->gPhysicsSDK->createMaterial(0.5,0.5,0.5);						//static friction, dynamic friction, restitution

				//1-Creating static plane
		physx::PxTransform planePos = physx::PxTransform(	physx::PxVec3(0.0f,0.0f,0.0f),	physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f)));
		physx::PxRigidStatic*	plane = this->engine->gPhysicsSDK->createRigidStatic(planePos);			
										plane->createShape(physx::PxPlaneGeometry(), *mMaterial);
								
										
						gScene->addActor(*plane);

/*
						const GRPVECTOR* pos=this->engine->scene->FindNode(__L("Model::Box001"))->GetPosition();


						physx::PxTransform boxPos(physx::PxVec3(pos->vector[0], pos->vector[1], pos->vector[2]));
						physx::PxBoxGeometry boxGeometry(physx::PxVec3(0.5f,0.5f,0.5f));
						gBox = PxCreateDynamic(*this->engine->gPhysicsSDK, boxPos, boxGeometry, *mMaterial, 1.0f);
						gScene->addActor(*gBox);

						physx::PxTransform spherePos(physx::PxVec3(25.0f, 2.0f, 0.1f));
						physx::PxRigidStatic* sphere = this->engine->gPhysicsSDK->createRigidStatic(spherePos);
														sphere->createShape(physx::PxSphereGeometry(0.5f), *mMaterial);

						gBox2 = PxCreateDynamic(*this->engine->gPhysicsSDK, spherePos, physx::PxSphereGeometry(0.5f), *mMaterial, 1.0f);
						gScene->addActor(*gBox2);
								*/
						return true;
}
PHYENTITY*	PHYSXWORLD::CreateStatic(GRPELEMENT* element,PHYENVELOPETYPE envelope, PHYENTITYTYPE type)
{
		PHYSXENTITY* entity=new PHYSXENTITY();
		AddEntity(entity);
		entity->SetElement(element);
		entity->SetType(PHYENTITYTYPE_STATIC);
		

		const GRPVECTOR* pos=element->GetNode()->GetPosition();

								//Creating material
		physx::PxMaterial* mMaterial =		this->engine->gPhysicsSDK->createMaterial(0.5,0.5,0.5);						//static friction, dynamic friction, restitution

		switch(envelope)
		{
		case PHYENVELOPETYPE_UNKNOWN: return entity;
		case PHYENVELOPETYPE_PLANE:
				{
						physx::PxTransform planePos = physx::PxTransform(	physx::PxVec3(pos->vector[0],pos->vector[1],pos->vector[2]),	physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f)));	
						physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*this->engine->gPhysicsSDK, physx::PxPlane(0,1,0,0), *mMaterial);							
						entity->envelope=groundPlane;
						gScene->addActor(*groundPlane);
				}
				break;

		case PHYENVELOPETYPE_BOX:				
				break;

		case PHYENVELOPETYPE_SPHERE:				
				break;
		case PHYENVELOPETYPE_CYLINDER:
				break;
		case PHYENVELOPETYPE_CAPSULE:
				break;
		case PHYENVELOPETYPE_CHARACTER:
				break;
		case PHYENVELOPETYPE_TRIMESH:

		case PHYENVELOPETYPE_COMPOUND:		
				break;
		};

		element->CollapseTransforms();

		return entity;
};

/*-------------------------------------------------------------------
//	PHYSXWORLD::CreateEntity
*/	
/**	
//	
//	Creates entity
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/06/2016 21:25:46
//	
//	@return 			PHYENTITY* : 
//
//  @param				element : 
//  @param				envelope : 
//  @param				type : 
*/
/*-----------------------------------------------------------------*/
PHYENTITY*	PHYSXWORLD::CreateEntity(GRPELEMENT* element,PHYENVELOPETYPE envelope, PHYENTITYTYPE type)
{
		PHYSXENTITY* entity=new PHYSXENTITY();
		AddEntity(entity);
		entity->SetElement(element);
		entity->SetType(PHYENTITYTYPE_DYNAMIC);

		const GRPVECTOR* pos=element->GetNode()->GetPosition();		
		//element->CollapseTransforms();
		//element->UpdateBoundingBox();
		//element->GetNode()->UpdateWorldMatrix();
								//Creating material
		//physx::PxMaterial* mMaterial =		this->engine->gPhysicsSDK->createMaterial(0.5,0.5,1.0);						//static friction, dynamic friction, restitution
		physx::PxMaterial* mMaterial =		this->engine->gPhysicsSDK->createMaterial(0.5f,1.0f,0.1f);						//static friction, dynamic friction, restitution

		switch(envelope)
		{
		case PHYENVELOPETYPE_UNKNOWN:
				return entity;
		case PHYENVELOPETYPE_PLANE:
				{
						physx::PxTransform planePos = physx::PxTransform(	physx::PxVec3(pos->vector[0],pos->vector[1],pos->vector[2]),	physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f)));		
						physx::PxRigidStatic*	plane = this->engine->gPhysicsSDK->createRigidStatic(planePos);			
						plane->createShape(physx::PxPlaneGeometry(), *mMaterial);		
						entity->envelope=plane;
						gScene->addActor(*plane);
				}
				break;

		case PHYENVELOPETYPE_BOX:
				{						
						GRPPOINT q;
						element->GetNode()->GetWorldMatrix().CalculateQ(q);
						physx::PxTransform boxPos(physx::PxVec3(pos->vector[0], pos->vector[1], pos->vector[2]));

		
						GRPAABB* bb=element->GetBoundingBox();
						element->UpdateBoundingBox();
						bb->Update();
						const GRPVECTOR* sc=element->GetNode()->GetScale();


						physx::PxBoxGeometry boxGeometry(physx::PxVec3(bb->width/2.0f,bb->height/2.0f,bb->depth/2.0f));
						//physx::PxBoxGeometry boxGeometry(physx::PxVec3(1.0,1.0,1.0));
						//physx::PxBoxGeometry boxGeometry(physx::PxVec3(5.6f,5.6f,5.6f));
						physx::PxRigidDynamic *gBox = PxCreateDynamic(*this->engine->gPhysicsSDK, boxPos, boxGeometry, *mMaterial, 1.0f);						
						entity->envelope=gBox;
						gScene->addActor(*gBox);
				}
				break;

		case PHYENVELOPETYPE_SPHERE:
				{
						GRPAABB* bb=element->GetBoundingBox();
						element->UpdateBoundingBox();
						bb->Update();
						//physx::PxTransform SphPos(physx::PxVec3(0.1f, 50.0f, 0.1f));
						physx::PxTransform SphPos(physx::PxVec3(pos->vector[0], pos->vector[1], pos->vector[2]));
						physx::PxSphereGeometry sphereGeometry(bb->width/2.0f);
						physx::PxRigidDynamic *gSphere = PxCreateDynamic(*this->engine->gPhysicsSDK, SphPos, sphereGeometry, *mMaterial, 1.0f);
						entity->envelope=gSphere;
						gScene->addActor(*gSphere);
				}
				break;
		case PHYENVELOPETYPE_CYLINDER:
				break;
		case PHYENVELOPETYPE_CAPSULE:
				{
						physx::PxTransform capsulePos(physx::PxVec3(pos->vector[0], pos->vector[1], pos->vector[2]));
						physx::PxCapsuleGeometry capsuleGeometry(0.5,0.5);
						physx::PxRigidDynamic *gCapsule = PxCreateDynamic(*this->engine->gPhysicsSDK, capsulePos, capsuleGeometry, *mMaterial, 1.0f);
						entity->envelope=gCapsule;
						gScene->addActor(*gCapsule);
				}
				break;
		case PHYENVELOPETYPE_CHARACTER:
				break;
		case PHYENVELOPETYPE_TRIMESH:

		case PHYENVELOPETYPE_COMPOUND:		
				break;
		};

		

		return entity;
};

bool	PHYSXWORLD::SetGravity  (GRPVECTOR vector)
{
		return true;
}

	

/*-------------------------------------------------------------------
//	PHYSXENGINE::Step
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
bool PHYSXWORLD::Step()
{
			//Stepping PhysX
		//physx::PxReal myTimestep = 1.0f/120.0f;


		physx::PxReal myTimestep = timer->GetMeasureMilliSeconds()/1000.0f;
		timer->Reset();

		if(gScene)
		{
				gScene->simulate(myTimestep);
				gScene->fetchResults(true);
		}

		for (XDWORD e=0;e<this->Entities.GetSize();e++)
		{
						if (this->Entities.Get(e)->GetType()==PHYENTITYTYPE_DYNAMIC)
								this->Entities.Get(e)->Update();
		}


/*
						physx::PxVec3 boxPos2 = gBox2->getGlobalPose().p;
						physx::PxQuat boxQ2 = gBox2->getGlobalPose().q;

						obj->GetElements()->Get(1)->GetNode()->SetPosition(boxPos.x,boxPos.y,boxPos.z);
						obj->GetElements()->Get(1)->GetNode()->SetRotation(boxQ.x*boxQ.w,boxQ.y*boxQ.w,boxQ.z*boxQ.w);
						obj->GetElements()->Get(1)->GetNode()->positionChanged=true;
						obj->GetElements()->Get(1)->GetNode()->rotationChanged=true;

						obj->GetElements()->GetLast()->GetNode()->SetPosition(boxPos2.x,boxPos2.y,boxPos2.z);
						obj->GetElements()->GetLast()->GetNode()->SetRotation(boxQ2.x*boxQ2.w,boxQ2.y*boxQ2.w,boxQ2.z*boxQ2.w);
						obj->GetElements()->GetLast()->GetNode()->positionChanged=true;
						obj->GetElements()->GetLast()->GetNode()->rotationChanged=true;

						*/
					//	XDEBUG_PRINTCOLOR(0,__L("Box: Position(%0.3f,%0.3f,%0.3f) Rotation(%0.3f,%0.3f,%0.3f,%0.3f)"),boxPos.x,boxPos.y,boxPos.z,boxQ.x,boxQ.y,boxQ.z,boxQ.w);

						return true;
}

/*-------------------------------------------------------------------
//	PHYSXENGINE::Step
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
bool PHYSXWORLD::End() 
{
			
		return true;
}

/*-------------------------------------------------------------------
//	PHYSXENGINE::Step
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
bool PHYSXWORLD::DestroyWorld()
{
				XDEBUG_PRINTCOLOR(0,__L("Destroying PHYSXWorld"));
		if (gScene)
		gScene->release();
		gScene=NULL;
		sceneDesc=NULL;
		return true;
}


#endif