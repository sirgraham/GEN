/*------------------------------------------------------------------------------------------
//	GRPOBJECTBUILDERALGORITHMLATHE.H
*/	
/**	
// \class 
//   
//  Creates a revolution Surface from mesh spline
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 12/06/2014 11:05:35
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPOBJECTBUILDERALGORITHMLATHE_H_
#define _GRPOBJECTBUILDERALGORITHMLATHE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObjectBuilder.h"
#include "GRPObjectBuilderAlgorithm.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/


class GRPOBJECTBUILDERALGORITHMLATHE : public GRPOBJECTBUILDERALGORITHM
{
	public:		
											GRPOBJECTBUILDERALGORITHMLATHE			(GRPOBJECTBUILDER* builder);
					
		GRPELEMENT*				Execute															(GRPSHAPE* shape);

		float							InitialAngle;
		float							nmeridians;
		float							meridianangle;
		float							maxangle;

private:
		void Clean()
		{
			this->InitialAngle=0.0f;
			this->nmeridians=10;
			this->maxangle=360.0f;
			this->meridianangle=maxangle/nmeridians;
		}

};
	
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

