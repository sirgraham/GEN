/*------------------------------------------------------------------------------------------
//	TESTBASECONSOLE.H
*/	
/**	
// \class 
//   
//  Test Base Console
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASECONSOLE_H_
#define _TESTBASECONSOLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPCONSOLE;


class TESTBASECONSOLE : public TESTBASE								
{																								
	public:																				
																	TESTBASECONSOLE						(GRPAPPLICATION* xapplication);
																 ~TESTBASECONSOLE						();

		virtual bool									IniResources							();
		virtual bool									EndResources							();
		virtual bool									InputManagement						();
		virtual bool									Do												();     

	private:

		void													Clean                     ()
																	{																		
																		timer				= NULL;
																		console			= NULL;
																		change			= false;
																		auto_scroll	=	false;
																	}

		XPATH													xpath;		
		XTIMER*												timer;
		//GRPCONSOLE*										console;
		bool													change;
		bool													auto_scroll;

};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

