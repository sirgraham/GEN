/*------------------------------------------------------------------------------------------
//	GRPPARTICLE.H
*/	
/**	
// \class 
//   
//  Particle unit
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 15/10/2015 11:58:24
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPPARTICLE_H_
#define _GRPPARTICLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPColor.h"
#include "GRPVector.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPPARTICLE
{
public:

								GRPPARTICLE		(XDWORD id) : ID(id) { Clean(); }
	virtual				~GRPPARTICLE	() {}

	void					SetPosition		(float x,float y,float z)  
								{ 
									this->position.vector[0]=x;
									this->position.vector[1]=y;
									this->position.vector[2]=z;
								};
	void					SetSize				(float x) { size=x;}


		XDWORD			ID;
		GRPCOLOR		color;
		GRPVECTOR		speed;
		GRPVECTOR		position;
		GLFLOAT			size;
		GLFLOAT			frame;
		GLFLOAT			zrotation;
		GLFLOAT			life;
		GLFLOAT			mass;
		GLFLOAT			bounciness;

private:

		void				Clean					()
		{
			zrotation		=0.0f;
			life				=1.0f;
			mass				=1.0f;
			bounciness	=0.0f;
			frame				=0.0f;
		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

