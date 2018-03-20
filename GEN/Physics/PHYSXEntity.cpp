/*------------------------------------------------------------------------------------------
//	PHYSXENTITY.CPP
*/	
/**	
// \class 
//   
//  PhyX concrete entity
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 26/05/2016 19:29:49
//	Last Modification	:	
*/	
/*	(C) Copyright GEN. All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifdef PHY_ACTIVE
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#include "PHYSXEntity.h"
#include "GRPElement.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	PHYSXENTITY::Update
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/04/2016 12:36:03
*/
/*-----------------------------------------------------------------*/
void PHYSXENTITY::Update					()
{
	/*
		position = envelope->getGlobalPose().p;
		rotation = envelope->getGlobalPose().q;

		Entity->GetNode()->SetPosition(position.x,position.y,position.z);		
		Entity->GetNode()->rotationmatrix.BuildRotationMatrixQ(rotation.x,rotation.y,rotation.z,rotation.w);

*/
						position	= envelope->getGlobalPose().p;
						rotation	= envelope->getGlobalPose().q;

						Entity->GetNode()->SetPosition(position.x,position.y,position.z);
						Entity->GetNode()->rotationmatrix.BuildRotationMatrixQ(rotation.x,rotation.y,rotation.z,rotation.w);

		Entity->GetNode()->positionChanged=true;
}

/*-------------------------------------------------------------------
//	PHYSXENTITY::SetMass
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/04/2016 12:36:03
*/
/*-----------------------------------------------------------------*/
void  PHYSXENTITY::SetMass(float mass) 
{

};

/*-------------------------------------------------------------------
//	PHYSXENTITY::SetFriction
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/04/2016 12:36:03
*/
/*-----------------------------------------------------------------*/
void  PHYSXENTITY::SetFriction(float friction)
{

};

/*-------------------------------------------------------------------
//	PHYSXENTITY::Destroy
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/04/2016 12:36:03
*/
/*-----------------------------------------------------------------*/
void  PHYSXENTITY::Destroy()
{

}
	

/*-------------------------------------------------------------------
//	PHYSXENTITY::AddForce
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/06/2016 21:50:39
//	
//  @param				vector : 
*/
/*-----------------------------------------------------------------*/
void  PHYSXENTITY::AddForce(GRPVECTOR& vector)
{
		if (static_cast<physx::PxRigidDynamic*>(this->envelope))
				static_cast<physx::PxRigidDynamic*>(this->envelope)->addForce(physx::PxVec3(vector.vector[0],vector.vector[1],vector.vector[2]), physx::PxForceMode::eFORCE);
}

/*-------------------------------------------------------------------
//	PHYSXENTITY::AddForce
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/06/2016 21:50:48
//	
//  @param				vector : 
//  @param				position : 
*/
/*-----------------------------------------------------------------*/
void  PHYSXENTITY::AddForce(GRPVECTOR& vector,GRPVECTOR& position)
{

}

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif





