/*------------------------------------------------------------------------------------------
//	GRPMAPPINGPROJECTIONCUBIC.H
*/	
/**	
// \class 
//   
//  Cubic U,Vs projection
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 02/07/2014 16:33:10
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPMAPPINGPROJECTIONCUBIC_H_
#define _GRPMAPPINGPROJECTIONCUBIC_H_
		
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPMappingProjection.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
		
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPELEMENT;

class GRPMAPPINGPROJECTIONCUBIC : public GRPMAPPINGPROJECTION
{
	public:
		
		virtual bool							ProjectMapping				(GRPELEMENT* object);
	
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

