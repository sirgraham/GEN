/*------------------------------------------------------------------------------------------
//  PHYENGINE.H
*/
/**
// \class
//
//  Physics abstract engine
//
//  @author Diego Martinez Ruiz de Gaona

//  Date Of Creation  : 26/05/2016 17:57:05
//  Last Modification :
*/
/*  (C) Copyright All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef PHY_ACTIVE

/*---- INCLUDES --------------------------------------------------------------------------*/


#include "PHYEngine.h"
#include "PHYSXEngine.h"
#include "PHYSXWorld.h"
#include "GRPVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  PHYENGINE::Create
*/
/**
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/05/2016 19:36:40
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
PHYENGINE* PHYENGINE::Create(XCHAR* name)
{
    PHYENGINE* engine=NULL;

    if (XSTRING::Compare(name,__L("PHYSX"))==0)
        engine=new PHYSXENGINE();


    if (engine)
    {
      PHYWORLD* world=engine->CreateWorld ();
      if (world)
      {
        engine->AddWorld(world);
      }
    }
    return engine;
}

/*-------------------------------------------------------------------
//  PHYENGINE::GetWorld
*/
/**
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/05/2016 19:36:40
//
//  @return       bool : success
//
//  @param        PHYWORLD* world : world to add
*/
/*-----------------------------------------------------------------*/
PHYWORLD*  PHYENGINE::GetWorld        ()
{
    return this->world;
}

/*-------------------------------------------------------------------
//  PHYENGINE::AddWorld
*/
/**
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/05/2016 19:36:40
//
//  @return       bool : success
//
//  @param        PHYWORLD* world : world to add
*/
/*-----------------------------------------------------------------*/
bool PHYENGINE::AddWorld(PHYWORLD* world)
{
    bool b=this->Worlds.Add(world);
    if (b)
        this->world=world;

    return b;
}

/*-------------------------------------------------------------------
//  PHYENGINE::Notify
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/05/2016 19:36:40
//
//  @return       bool :
//
//  @param        GRPMSGTYPE msg : message
                  GRPSUBJECT* notifier : notifiing subject
*/
/*-----------------------------------------------------------------*/
bool  PHYENGINE::Notify       (GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
    switch(msg)
    {
    case GRPMSG_DESTROYED:      return Destroy();
    case GRPMSG_STEP:           return Step   ();
    }
    return true;
}

#endif