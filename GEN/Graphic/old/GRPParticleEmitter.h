/*------------------------------------------------------------------------------------------
//	GRPPARTICLEEMITTER.H
*/	
/**	
// \class 
//   
//  Particle emitter, generates particle pools
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 15/10/2015 13:02:40
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPPARTICLEEMITTER_H_
#define _GRPPARTICLEEMITTER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPNode.h"
#include "GRPParticlePool.h"
#include "XRand.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPPARTICLEEMITTER
{
public:

					GRPPARTICLEEMITTER	() {}
	virtual ~GRPPARTICLEEMITTER	() {}

	GRPNODE*					GetNode() { return this->Node;	}

protected:

	GLFLOAT						MinEmitRate;			
	GLFLOAT						MaxEmitRate;			

	GLFLOAT						MinStrength;		
	GLFLOAT						MaxStrength;		

	GRPVECTOR					MinAngle;				
	GRPVECTOR					MaxAngle;				

	GLFLOAT						MinRotation;				
	GRPVECTOR					MaxRotation;			

	GLFLOAT						MinScale;				
	GRPVECTOR					MaxScale;			

	GRPPARTICLEPOOL*	Pool;
	GRPNODE*					Node;
	XRAND*						Random;

private:

	void							Clean			()
	{
		MinEmitRate=1.0f;			
		MaxEmitRate=1.0f;			

		MinStrength=1.0f;		
		MaxStrength=1.0f;		

		MinAngle=GRPVECTOR(0.0f,0.0f,0.0f);
		MaxAngle=GRPVECTOR(0.0f,0.0f,0.0f);

		MinRotation=0.0f;
		MaxRotation=0.0f;			

		MinScale=1.0f;			
		MaxScale=1.0f;			

		Pool		=NULL;
		Node		=NULL;
		Random	=NULL;
	}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


