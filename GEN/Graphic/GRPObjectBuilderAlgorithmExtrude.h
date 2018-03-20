/*------------------------------------------------------------------------------------------
//	GRPOBJECTBUILDERALGORITHMEXTRUDE.H
*/	
/**	
// \class 
//   
//  Extrudes a 2D spline into 3d
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 12/06/2014 11:22:22
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPOBJECTBUILDERALGORITHMEXTRUDE_H_
#define _GRPOBJECTBUILDERALGORITHMEXTRUDE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObjectBuilder.h"
#include "GRPObjectBuilderAlgorithm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	

	
/*---- CLASS -----------------------------------------------------------------------------*/


class GRPOBJECTBUILDERALGORITHMEXTRUDE : public GRPOBJECTBUILDERALGORITHM
{
	public:								
											GRPOBJECTBUILDERALGORITHMEXTRUDE		(GRPOBJECTBUILDER* builder);
		
		GRPELEMENT*				Execute															(GRPSHAPE* shape);

		GLFLOAT						depth;
		int								nsegments;
		int								closeSpline;
		bool							doublesided;
		bool							UnifyNormals;
		bool							ReverseNormals;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

