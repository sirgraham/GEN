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
	
#ifndef _TESTBASERENDEROBJECT_H_
#define _TESTBASERENDEROBJECT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASERENDEROBJECT : public TESTBASE					
{																								
	public:																				
																	TESTBASERENDEROBJECT			(GRPAPPLICATION* xapplication);
																 ~TESTBASERENDEROBJECT			();

		virtual bool									IniResources							();  
		virtual	bool									EndResources							();

		virtual bool									Do												();

		bool													Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

	private:	

		void                          Clean                     ()
																	{
																		timer	 = NULL;
																		delta  = 0;
																		obj			= NULL;
																	}

		GRPOBJECT*										obj;
		XTIMER*												timer;
		XDWORD												delta;
		GRPSCENE*											scene;
		GRPTEXTURE*										texture;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

