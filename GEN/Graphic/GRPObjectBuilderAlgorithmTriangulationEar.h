/*------------------------------------------------------------------------------------------
//	GRPOBJECTBUILDERALGORITHMTRIANGULIZEEAR.H
*/	
/**	
// \class 
//   
//  Triangulation algorithms
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 06/06/2014 16:01:49
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPOBJECTBUILDERALGORITHMTRIANGULIZEEAR_H_
#define _GRPOBJECTBUILDERALGORITHMTRIANGULIZEEAR_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XVector.h"

#include "GRPOpenGL.h"

#include "GRPObjectBuilder.h"
#include "GRPObjectBuilderAlgorithm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class GRPMATERIAL;	
class GRPOBCTJ;
class GRPFACE;
class GRPPOINT;
class GRPSHAPE;
class GRPOBJECTBUILDER;

class GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR : public GRPOBJECTBUILDERALGORITHM
{
	public:
													GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR		(GRPOBJECTBUILDER* builder) 
													{ 
														this->builder	= builder; 
														z							= 0.0;
														normal				= GRPVECTOR(0.0,0.0,1.0);
													}
						
		bool									GetIsEar																			(XVECTOR<GRPPOINT*>* vertex, GRPPOINT* point);
		bool									ClipEar																				(XVECTOR<GRPPOINT*>* vertexlist ,	XVECTOR<GRPFACE*>* faceList);		
		
		GRPELEMENT*						Execute																				(GRPSHAPE* shape);
						
		GRPVECTOR							normal;
		GLFLOAT								z;						

	private:

		float									DotProduct																		(GLFLOAT ax, GLFLOAT ay, GLFLOAT bx, GLFLOAT by, GLFLOAT cx, GLFLOAT cy);
		float									CrossProductLength														(GLFLOAT ax, GLFLOAT ay, GLFLOAT bx, GLFLOAT by, GLFLOAT cx, GLFLOAT cy);
		float									GetAngleToHorizontalaxis											(GRPPOINT *point1, GRPPOINT *point2);
};


#endif