/*------------------------------------------------------------------------------------------
//	GRPMAPPINGPROJECTIONCYLINDER.H
*/	
/**	
// \class 
//   
//  Cylindrical U,V projection
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 02/07/2014 16:18:16
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPMAPPINGPROJECTIONCYLINDER_H_
#define _GRPMAPPINGPROJECTIONCYLINDER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPMappingProjection.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPPOINT;
class GRPELEMENT;


class GRPMAPPINGPROJECTIONCYLINDER : public GRPMAPPINGPROJECTION
{
	public:	
											GRPMAPPINGPROJECTIONCYLINDER					();
	
		float							GetAngleToHorizontalAxis							(GRPPOINT *p1,GRPPOINT *p2);
		
		virtual bool			ProjectMapping												(GRPELEMENT* object);
	
		GLFLOAT						radius;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

