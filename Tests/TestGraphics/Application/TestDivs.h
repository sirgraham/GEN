/*------------------------------------------------------------------------------------------
//	TESTDIVS.H
*/	
/**	
// \class 
//   
//  Manages space divisions
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 16/08/2017 13:46:39
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTDIVS_H_
#define _TESTDIVS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
#include "GRPTextBox.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
class GRPWIDGETPASS;

/*---- CLASS -----------------------------------------------------------------------------*/


class TESTDIVS : public TESTBASE
{
public:

											TESTDIVS									(GRPAPPLICATION* xapplication);
	virtual bool				IniResources							();
		
	virtual	bool				EndResources							();
	virtual bool				Do												();     	

	virtual bool				Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

protected:

		GRPSCENE*				scene;
		GRPWIDGETPASS*	pass;
		XSTRING					loadpage;
		int							currentpage;

private:

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

