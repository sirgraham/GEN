/*------------------------------------------------------------------------------------------
//	TESTBASETEXTANIMATION.H
*/	
/**	
// \class 
//   
//  Test text animations
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 2/09/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASETEXTANIMATION_H_
#define _TESTBASETEXTANIMATION_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPObserver.h"
#include "TestBase.h"
#include "GRPTextBox.h"
#include "GRPSlotNumber.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASETEXTANIMATION : public TESTBASE
{
	public:																				
										TESTBASETEXTANIMATION			(GRPAPPLICATION* xapplication);
	virtual bool			IniResources							();
	virtual	bool			EndResources							();
	virtual bool			Do												();     	
	virtual bool			InputManagement						();
	virtual bool			Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

	XVECTOR<GRPSLOTNUMBER*> slots;	

	XTIMER*						timer;
	XINTERPOLATOR*		interpolator;

};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif
