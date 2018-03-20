/*------------------------------------------------------------------------------------------
//	PHYSXWORLD.H
*/	
/**	
// \class 
//   
//  PhysX world
//   
//	@author	 No se ha podido encontrar el fichero 'MacrosAuthor.txt' en el directorio del IDE. No se han rellenado los datos para las macros.
//
//	Date Of Creation	: 26/05/2016 19:52:35
//	Last Modification	:	
*/	
/*	(C) Copyright No se ha podido encontrar el fichero 'MacrosAuthor.txt' en el directorio del IDE. No se han rellenado los datos para las macros.. All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _PHYSXWORLD_H_
#define _PHYSXWORLD_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <math.h>

#include "PHYWorld.h"
#include "PHYSXEngine.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class PHYSXWORLD : public PHYWORLD
{
public:

						PHYSXWORLD() { Clean(); }
		virtual ~PHYSXWORLD	() {}

		bool				Init						(PHYENGINE* engine);
		bool				Step						();
		bool				End							();
		bool				DestroyWorld		();

		PHYENTITY*	CreateEntity		(GRPELEMENT* element=NULL,	PHYENVELOPETYPE envelope=PHYENVELOPETYPE_BOX,		PHYENTITYTYPE type=PHYENTITYTYPE_DYNAMIC);
		PHYENTITY*	CreateStatic		(GRPELEMENT* element,				PHYENVELOPETYPE envelope=PHYENVELOPETYPE_PLANE,	PHYENTITYTYPE type=PHYENTITYTYPE_STATIC);
		bool				SetGravity			(GRPVECTOR vector);



		physx::PxScene*			gScene;
		physx::PxSceneDesc* sceneDesc;
		PHYSXENGINE*				engine;
		physx::PxMaterial*	mMaterial;

		XTIMER* timer;

private:

	void Clean()
	{
		gScene = NULL;
		sceneDesc = NULL;
		engine = NULL;
		mMaterial = NULL;
	 }

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

