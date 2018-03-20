/*------------------------------------------------------------------------------------------
//	PHYSXENGINE.H
*/	
/**	
// \class 
//   
//  PHYSX concrete engine
//   
//	@author	 Diego Martinez 
//
//	Date Of Creation	: 26/05/2016 18:38:21
//	Last Modification	:	
*/	
/*	(C) Copyright GEN All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifdef PHY_ACTIVE

#ifndef _PHYSXENGINE_H_
#define _PHYSXENGINE_H_
	


#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdint.h>
#include <math.h>
#include <PxPhysicsAPI.h>
#include <foundation\PxFoundation.h>
#include <extensions\PxDefaultErrorCallback.h>
	
#include "PHYEngine.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPOBJECT;
class PHYMATERIAL;


/*---- CLASS -----------------------------------------------------------------------------*/
class PHYSXENGINE : public PHYENGINE
{
		friend class PHYSXWORLD;

public:
		

										PHYSXENGINE			()			{ Clean(); }
		virtual					~PHYSXENGINE		()			{ Clean(); }

		bool						Init						(GRPSCENE* scene);		
		bool						Step						();
		PHYWORLD*				CreateWorld			();
		PHYMATERIAL*		CreateMaterial	();		

		bool						Destroy			();
		bool						End					();
		

		XDWORD				GetSubjectType	() { return GRPNAMED_TYPE_GRPLAYER; }
		

						physx::PxFoundation*									gFoundation;
						physx::PxPhysics*											gPhysicsSDK;
						
						//physx::PxVisualDebuggerConnection*		gConnection;
						physx::PxDefaultErrorCallback					gDefaultErrorCallback;
						physx::PxDefaultAllocator							gDefaultAllocatorCallback;

private:

		void Clean()
		{
						gFoundation									=NULL;
						gPhysicsSDK									=NULL;
						//gConnection									=NULL;
		}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

#endif

