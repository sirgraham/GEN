/*------------------------------------------------------------------------------------------
//	TESTBASESPRITES.H
*/	
/**	
// \class 
//   
//  Test Base Sprites
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASESPRITES_H_
#define _TESTBASESPRITES_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
 
class TESTBASESPRITES : public TESTBASE
{
	public:																				
											TESTBASESPRITES						(GRPAPPLICATION* xapplication);
	virtual bool				IniResources							();
		
	virtual	bool				EndResources							();
	virtual bool				Do												();     	
	//virtual bool				InputManagement						();
	virtual bool				Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

	GRPOBJECT* obj;
	XPATH xpath;
};
	

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

