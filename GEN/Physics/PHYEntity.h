/*------------------------------------------------------------------------------------------
//	PHYENTITY.H
*/	
/**	
// \class 
//   
//  Physics abstract entity
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 26/05/2016 19:29:49
//	Last Modification	:	
*/	
/*	(C) Copyright GEN. All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _PHYENTITY_H_
#define _PHYENTITY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPElement.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class PHYENVELOPE;
class GRPELEMENT;
	
enum PHYENTITYTYPE
{
		PHYENTITYTYPE_UNKNOWN,
		PHYENTITYTYPE_STATIC,
		PHYENTITYTYPE_DYNAMIC,
		PHYENTITYTYPE_KINEMATIC,
		PHYENTITYTYPE_TRIGGER
};

enum PHYENVELOPETYPE
{
		PHYENVELOPETYPE_UNKNOWN,
		PHYENVELOPETYPE_PLANE,
		PHYENVELOPETYPE_BOX,
		PHYENVELOPETYPE_SPHERE,
		PHYENVELOPETYPE_CYLINDER,
		PHYENVELOPETYPE_CAPSULE,
		PHYENVELOPETYPE_CHARACTER,
		PHYENVELOPETYPE_TRIMESH,
		PHYENVELOPETYPE_COMPOUND,
		PHYENVELOPETYPE_MAX
};

/*---- CLASS -----------------------------------------------------------------------------*/
class PHYENTITY
{
public:		

		virtual	void		Update				()							=0;
		virtual void		SetMass				(float mass)		=0;
		virtual void		SetFriction		(float friction)=0;
		virtual void		SetElement		(GRPELEMENT* element)		{				this->Entity=element;		}
						void		SetType				(PHYENTITYTYPE t)				{				this->Type=t;						}
		PHYENTITYTYPE		GetType				()											{				return this->Type;			}

		virtual void		Destroy				()=0;

		virtual void		AddForce			(GRPVECTOR& vector)=0;
		virtual void		AddForce			(GRPVECTOR& vector,GRPVECTOR& position)=0;

protected:

		PHYENTITYTYPE		Type;
		GRPELEMENT*			Entity;
		
		float						mass;
		float						friction;

private:

		void Clean()
		{
				mass				=1.0f;
				friction		=1.0f;
				Entity			=NULL;
				Type				=PHYENTITYTYPE_UNKNOWN;
		}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


