/*------------------------------------------------------------------------------------------
//	GRPMAPPINGPROJECTIONPLANAR.H
*/	
/**	
// \class 
//   
//  Planar UV's projection
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 02/07/2014 15:51:34
//	Last Modification	:	
*/	
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPMAPPINGPROJECTIONPLANAR_H_
#define _GRPMAPPINGPROJECTIONPLANAR_H_
		
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPMappingProjection.h"
#include "GRPNode.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPMAPPINGPROJECTIONPLANAR : public GRPMAPPINGPROJECTION
{
	public:		
		GRPNODE* node;

		GRPMAPPINGPROJECTIONPLANAR()
		{
			this->direction=GRPVECTOR(0.0,0.0,1.0);			
		}

		virtual	bool						ProjectMapping				(GRPELEMENT* object);
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

