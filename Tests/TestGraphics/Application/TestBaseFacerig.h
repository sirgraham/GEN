/*------------------------------------------------------------------------------------------
//	TESTBASEFACERIG.H
*/	
/**	
// \class 
//   
//  Test Base Facering
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEFACERIG_H_
#define _TESTBASEFACERIG_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class SNDELEMENT;		
class	SNDFACTORY;

class TESTBASEFACERIG : public TESTBASE								
{																								
	public:																				
																	TESTBASEFACERIG					(GRPAPPLICATION* xapplication);
															   ~TESTBASEFACERIG					();

		virtual bool									IniResources							();     
		virtual bool									EndResources							();  
		virtual bool									Do												();     

	private:

		void													Clean											()
																	{																		
																		sndelement		= NULL;		
																		soundsystem		= NULL;
																		timer					= NULL;
																	}
		
		XPATH													xpath;
		SNDELEMENT*										sndelement;		
		SNDFACTORY*										soundsystem;
		XTIMER*												timer;
		GRPOBJECT*										obj;
}; 

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

