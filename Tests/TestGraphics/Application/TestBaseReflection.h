/*------------------------------------------------------------------------------------------
//	TESTBASEREFLECTION.H
*/	
/**	
// \class 
//   
//  Reflection
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 05/08/2016 13:40:22
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEREFLECTION_H_
#define _TESTBASEREFLECTION_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASEREFLECTION : public TESTBASE
{
	public:																				
										TESTBASEREFLECTION				(GRPAPPLICATION* xapplication) : TESTBASE(xapplication){};
	virtual bool			IniResources							();
	virtual	bool			EndResources							();
	virtual bool			Do												();     	
	virtual bool			InputManagement						() { return false;}
	virtual bool			Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

