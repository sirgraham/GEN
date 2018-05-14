/*------------------------------------------------------------------------------------------
//  PHYSXENTITY.H
*/
/**
// \class
//
//  PhyX concrete entity
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 26/05/2016 19:29:49
//  Last Modification :
*/
/*  (C) Copyright GEN. All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef PHY_ACTIVE

#ifndef _PHYSXENTITY_H_
#define _PHYSXENTITY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#include "PHYEntity.h"
#include <PxPhysicsAPI.h>

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class PHYSXENTITY : public PHYENTITY
{
public:

            void Update         ();
    virtual void SetMass        (float mass);
    virtual void SetFriction    (float friction);
    virtual void Destroy        ();

    virtual void AddForce       (GRPVECTOR& vector);
    virtual void AddForce       (GRPVECTOR& vector,GRPVECTOR& position);

    physx::PxRigidActor*    envelope;
    physx::PxMaterial*      mMaterial;

    physx::PxVec3           position;
    physx::PxQuat           rotation;


private:

    void Clean()
    {
        envelope=NULL;
    }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

#endif