/*------------------------------------------------------------------------------------------
//	TESTBASERENDEROBJECT.H
*/	
/**	
// \class 
//   
//  Test Base Render Object
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTCALIBRATION_H_
#define _TESTCALIBRATION_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
#include "DIOI2CPWMControlerPCA9685.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#include "GRPColor.h"
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTCALIBRATION : public TESTBASE					
{																								
	public:																				
																	TESTCALIBRATION					(GRPAPPLICATION* xapplication);
																 ~TESTCALIBRATION					();

		virtual bool									IniResources							();  
		virtual	bool									EndResources							();
		virtual bool									Do												();

		bool													Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

	private:	

		void                          Clean                     ()
																	{
																		timer	 = NULL;
		XDWORD													delta  = 0;

																		r=100.0f;
																		g=100.0f;
																		b=100.0f;
																		a=100.0f;

																		blinking=false;

																		brate=10;
																	}

		XVECTOR<GRPTEXTCONTAINER*>		texts;

		DIOI2CPWMCONTROLERPCA9685*		pca9685;

		XTIMER*												timer;
		XDWORD												delta;

		GRPELEMENT*		muestra;

		float			speed;
		int				current;
		GRPCOLOR	color;

		double r,g,b,a;
		bool blinking;
		int brate;

};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

