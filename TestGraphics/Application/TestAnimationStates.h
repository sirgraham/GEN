/*------------------------------------------------------------------------------------------
//	TESTANIMATIONSTATES.H
*/	
/**	
// \class 
//   
//  Test logic animation states
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 01/12/2016 17:07:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTANIMATIONSTATES_H_
#define _TESTANIMATIONSTATES_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class TESTANIMATIONSTATES : public TESTBASE
{
public:

										TESTANIMATIONSTATES				(GRPAPPLICATION* xapplication) : TESTBASE(xapplication) {};
	virtual						~TESTANIMATIONSTATES			() {}											
	virtual bool			IniResources							();
	virtual	bool			EndResources							();
	virtual bool			Do												();     	
	virtual bool			InputManagement						() { return TESTBASE::InputManagement();}
	virtual bool			Notify										(GRPMSGTYPE,GRPSUBJECT*);
		
protected:

private:

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

