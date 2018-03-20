/*------------------------------------------------------------------------------------------
//	TESTBASESCREEN.H
*/	
/**	
// \class 
//   
//  Test Base Minimun screen rendering context
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 2/09/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASESCREEN_H_
#define _TESTBASESCREEN_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASESCREEN : public TESTBASE
{
	public:																				
										TESTBASESCREEN						(GRPAPPLICATION* xapplication);
	virtual bool			IniResources							();
	virtual	bool			EndResources							();
	virtual bool			Do												();     	
	virtual bool			InputManagement						();
	virtual bool			Notify										();
	
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif
