/*------------------------------------------------------------------------------------------
//	TESTBASERENDEROBJECT.H
*/	
/**	
// \class 
//   
//  Test Base Render Object
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASERENDERER_H_
#define _TESTBASERENDERER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASERENDERER : public TESTBASE					
{																								
	public:																				
																	TESTBASERENDERER			(GRPAPPLICATION* xapplication);
																 ~TESTBASERENDERER			();

		virtual bool									IniResources							();  
		virtual	bool									EndResources							();
		virtual bool									Do												();

		bool													Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

	private:	

		void                          Clean                     ()
																	{
																		timer	 = NULL;
		XDWORD													delta  = 0;
																	}

		XTIMER*												timer;
		XDWORD												delta;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

