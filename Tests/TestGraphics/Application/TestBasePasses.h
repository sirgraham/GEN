/*------------------------------------------------------------------------------------------
//	TESTBASEPASSES.H
*/	
/**	
// \class 
//   
//  Tests stencil and scissors test
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 12/07/2016 17:30:50
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEPASSES_H_
#define _TESTBASEPASSES_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "TestBase.h"	
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASEPASSES : public TESTBASE, public XOBSERVER							
{																								
	public:		

			enum
			{					
					TEST_OVERLAY			= 1 << 0,
					TEST_STENCIL			= 1 << 1, 
					TEST_BACKGROUND		= 1 << 2,
					TEST_DEBUG				= 1 << 3,
					TEST_VIEWPORT			= 1 << 4,
					TEST_DEPTH				= 1 << 5,
					TEST_OFFSCREEN		= 1 << 6,
					TEST_POSTPROCESS  = 1 << 7,
					TEST_MAIN					= 1 << 16,
					TEST_ALL					= 0xFF
				};

																								TESTBASEPASSES						(GRPAPPLICATION* xapplication) : TESTBASE(xapplication) {}
																								~TESTBASEPASSES						(){};

		virtual bool																IniResources							();
		virtual	bool																EndResources							();
		virtual bool																Do												();
		bool																				Notify										(GRPMSGTYPE msg,GRPSUBJECT* notifier);
		void 																				HandleEvent								(XEVENT* event){};

	private:

		void																				Clean											()	{	}
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

