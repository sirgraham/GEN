/*------------------------------------------------------------------------------------------
//	TESTBASEANIMATIONMILESTONE.H
*/	
/**	
// \class 
//   
//  Test Base Animation Milestone
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEANIMATIONMILESTONE_H_
#define _TESTBASEANIMATIONMILESTONE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASEANIMATIONMILESTONE : public TESTBASE
{
	public:																				
										TESTBASEANIMATIONMILESTONE(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  { } ;
	virtual bool			IniResources							();
	virtual	bool			EndResources							();
	virtual bool			Do												();     	
	virtual bool			InputManagement						();
	virtual bool			Notify										()  { return true;}

	GRPOBJECT* obj;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

