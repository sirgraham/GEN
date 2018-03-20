/*------------------------------------------------------------------------------------------
//	PHYSXMATERIAL.H
*/	
/**	
// \class 
//   
//  Physics material abstract entity
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 26/05/2016 19:29:49
//	Last Modification	:	
*/	
/*	(C) Copyright GEN. All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifdef PHY_ACTIVE

#ifndef _PHYSXMATERIAL_H_
#define _PHYSXMATERIAL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <PxPhysicsAPI.h>
#include "PHYMaterial.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class PHYSXMATERIAL : public PHYMATERIAL
{
public:
			PHYSXMATERIAL() { Clean(); }
				
		physx::PxMaterial* physxmaterial;

		void Clean()
		{
				physxmaterial=NULL;
		}
};

#endif

#endif