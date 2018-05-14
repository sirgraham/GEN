
/*------------------------------------------------------------------------------------------
//  PHYSXENGINE.CPP
//
//  PhysX concrete engine
//
//  Author            : No se ha podido encontrar el fichero 'MacrosAuthor.txt' en el directorio del IDE. No se han rellenado los datos para las macros.
//  Date Of Creation  : 26/05/2016 18:39:19
//  Last Modification :
//
//  (C) Copyright No se ha podido encontrar el fichero 'MacrosAuthor.txt' en el directorio del IDE. No se han rellenado los datos para las macros.. All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef PHY_ACTIVE

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "physxengine.h"
#include "PHYSXWorld.h"

#include "GRPScene.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

#include <PxPhysicsAPI.h> //PhysX main header file

#ifndef ANDROID
//-------Loading PhysX libraries----------]
#ifdef _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PxFoundationDEBUG_x86.lib")
//#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib")

#else

#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#pragma comment(lib, "PhysX3Extensions.lib")

#endif

#else

#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")

#endif



/*-------------------------------------------------------------------
//  PHYSXENGINE::Init
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 12:36:03
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool PHYSXENGINE::Init(GRPSCENE* scene)
{
    XDEBUG_PRINTCOLOR(0,__L("Initializing PHYSXEngine"));

    this->scene=scene;
    this->scene->AddObserver(this);

#ifdef PX_FOUNDATION_VERSION
  gFoundation = PxCreateFoundation  (PX_FOUNDATION_VERSION, gDefaultAllocatorCallback,  gDefaultErrorCallback);
#else
  gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
#endif
  if (!gFoundation)
  {
      XDEBUG_PRINTCOLOR(4, __L("Unable to locate foundation, version mismatch"));
      return false;
  }
  gPhysicsSDK = PxCreatePhysics     (PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale() );
  if (!gPhysicsSDK)
  {
      XDEBUG_PRINTCOLOR(4, __L("Unable to create physics library"));
      return false;
  }

  /*
  if(gPhysicsSDK->getPvdConnectionManager())
  {
    gPhysicsSDK->getVisualDebugger()->setVisualizeConstraints(true);
    gPhysicsSDK->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
    gPhysicsSDK->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
    gConnection = physx::PxVisualDebuggerExt::createConnection(gPhysicsSDK->getPvdConnectionManager(), "127.0.0.1", 5425, 10);
  }
  */

  this->world=this->CreateWorld();
  if (!this->world)
      return false;

        world->SetGravity(GRPVECTOR(0.0f,-98.0f,0.0f));
        if (!world->Init(this)) return false;

                //Creating material
    physx::PxMaterial* mMaterial =    gPhysicsSDK->createMaterial(0.5f,0.5f,0.5f);            //static friction, dynamic friction, restitution

    this->active=true;

return true;
}

/*-------------------------------------------------------------------
//  PHYSXENGINE::CreateWorld
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 12:36:03
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
PHYWORLD* PHYSXENGINE::CreateWorld()
{
    PHYSXWORLD* pworld=new PHYSXWORLD();
    return pworld;
}

/*-------------------------------------------------------------------
//  PHYSXENGINE::Step
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 12:36:03
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool PHYSXENGINE::Step()
{
    if (active)
    return this->world->Step();
    return false;
}


/*-------------------------------------------------------------------
//  PHYSXENGINE::Destroy
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/06/2016 17:10:35
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool PHYSXENGINE::Destroy()
{
    XDEBUG_PRINTCOLOR(0,__L("Destroying PHYSXEngine"));
    this->active=false;

    if (this->world)
    this->world->Destroy();
    /*
    if (gConnection)
    {
            gConnection->release();
            gConnection=NULL;
    }
    */

    if (gPhysicsSDK)
    {
            gPhysicsSDK->release();
            gPhysicsSDK=NULL;
    }

    if (gFoundation)
    {
            gFoundation->release();
            gFoundation=NULL;
    }
    return true;
}

/*-------------------------------------------------------------------
//  PHYSXENGINE::End
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/06/2016 17:10:44
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool PHYSXENGINE::End()
{
    return true;
}

#endif

/*
  gFoundation = PxCreateFoundation  (PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
  if (!gFoundation) return false;
  gPhysicsSDK = PxCreatePhysics     (PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale() );
  if (!gPhysicsSDK) return false;

              //Creating scene

              PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
                          sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
                          sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
                          sceneDesc.filterShader  = PxDefaultSimulationFilterShader;
              gScene = gPhysicsSDK->createScene(sceneDesc);
              */

              //Creating material
    //PxMaterial* mMaterial =   gPhysicsSDK->createMaterial(0.5,0.5,0.5);           //static friction, dynamic friction, restitution

 /*
        //1-Creating static plane
    PxTransform planePos = PxTransform( PxVec3(0.0f,0.0f,0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
    PxRigidStatic*  plane = gPhysicsSDK->createRigidStatic(planePos);
                    plane->createShape(PxPlaneGeometry(), *mMaterial);
                    gScene->addActor(*plane);
                    gScene->setGravity(PxVec3(0.0,-98.0,0.0));

                */

            //  physx::PxMaterial* mMaterial=dynamic_cast<PHYSXWORLD*>(engine->GetWorld())->mMaterial;
    /*
            GRPAABB* bb=NULL;
            PxBoxGeometry boxGeometry(PxVec3(5.6f,5.6f,5.6f));
            /*
            //2) Create cube
              const GRPVECTOR* pos=obj->GetElements()->Get(1)->GetNode()->GetPosition();
              PxTransform boxPos(PxVec3(pos->vector[0], pos->vector[1], pos->vector[2]));
          //  PxTransform boxPos(PxVec3(25.0f, 25.0f, 25.0f));
            //PxBoxGeometry boxGeometry(PxVec3(5.6f,5.6f,5.6f));
              //PxBoxGeometry boxGeometry(PxVec3(0.5f,0.5f,0.5f));
              GRPAABB* bb=obj->GetElements()->Get(1)->GetBoundingBox();
              obj->GetElements()->Get(1)->UpdateBoundingBox();
              bb->Update();

              PxBoxGeometry boxGeometry(PxVec3(bb->width/2.0f,bb->height/2.0f,bb->depth/2.0f));
            gBox = PxCreateDynamic(*gPhysicsSDK, boxPos, boxGeometry, *mMaterial, 1.0f);
            gScene->addActor(*gBox);
            *//*
              const GRPVECTOR* pos2=obj->GetElements()->Get(2)->GetNode()->GetPosition();
              PxTransform boxPos2(PxVec3(pos2->vector[0], pos2->vector[1], pos2->vector[2]));

          //  PxTransform boxPos2(PxVec3(0.1f, 25.0f, 0.1f));
            //PxRigidStatic* box2 = gPhysicsSDK->createRigidStatic(boxPos2);

            gBox2 = PxCreateDynamic(*gPhysicsSDK, boxPos2, boxGeometry, *mMaterial, 1.0f);
            gScene->addActor(*gBox2);

              bb=obj->GetElements()->Get(2)->GetBoundingBox();
              obj->GetElements()->Get(2)->UpdateBoundingBox();
              bb->Update();
            PxSphereGeometry sphGeometry(bb->width/2.0);
            PxTransform SphPos(PxVec3(0.1f, 50.0f, 0.1f));
            PxRigidStatic* sphere = gPhysicsSDK->createRigidStatic(SphPos);

            gBox3 = PxCreateDynamic(*gPhysicsSDK, SphPos, sphGeometry, *mMaterial, 1.0f);
            gScene->addActor(*gBox3);
            */