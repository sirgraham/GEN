/*------------------------------------------------------------------------------------------
//	GRPMESHLOD.H
*/	
/**	
// \class 
//   
//  Manages Mesh Lod
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 09/09/2016 11:14:23
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPMESHLOD_H_
#define _GRPMESHLOD_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XVector.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
class GRPMESH;
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPMESHLOD
{
public:

	int									currentlod;
	int									maxLod;	
	int									minLod;
	float								distanceSetting;

	void								SelectLOD	() { currentlod=0;											}
	GRPMESH*						GetMesh		() { return lods.FastGet(currentlod);		}

protected:

	XVECTOR<GRPMESH*>		lods;

private:

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

