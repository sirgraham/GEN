/*------------------------------------------------------------------------------------------
//	TESTBASETEXTAREA.H
*/	
/**	
// \class 
//   
//  Test Base Font TTF
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASETEXTAREA_H_
#define _TESTBASETEXTAREA_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPTEXTAREA;
	
class TESTTEXTAREA : public TESTBASE
{
	public:																				
										TESTTEXTAREA							(GRPAPPLICATION* xapplication) : TESTBASE(xapplication) {};
	virtual bool			IniResources							();
	virtual	bool			EndResources							();
	virtual bool			Do												();     	
	virtual bool			InputManagement						() { return TESTBASE::InputManagement();}
	virtual bool			Notify										(GRPMSGTYPE,GRPSUBJECT*);
	
	GRPTEXTAREA*			text;
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

