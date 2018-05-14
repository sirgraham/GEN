/*------------------------------------------------------------------------------------------
//	TESTBASEPHY.H
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
	
#ifndef _TESTBASEPHY_H_
#define _TESTBASEPHY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "stdint.h"
#include "PHYEngine.h"
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASEPHY : public TESTBASE								
{																								
	public:																				
																	TESTBASEPHY								(GRPAPPLICATION* xapplication) : TESTBASE(xapplication) {}
																~TESTBASEPHY								() {}

		virtual bool									IniResources							();
		virtual bool									EndResources							();
		virtual bool									InputManagement						();
		virtual bool									Do												();     

	private:

		void													Clean                     ()
																	{																		

																	}

		XPATH													xpath;				
		GRPSCENE*	 scene;
		PHYENGINE* engine;
		GRPOBJECT* loc;
		GRPOBJECT* loc2;
		GRPELEMENT* Player;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

