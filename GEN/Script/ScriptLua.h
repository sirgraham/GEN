//------------------------------------------------------------------------------------------
//	SCRIPTLUA.H
//	
/**	
// \class 
//   
//  Script Interpreter  LUA (Lua interpreter)
//   
//	@author	 Abraham J. Velez
//	@version 17/07/2007 17:44:28
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _SCRIPTLUA_H_
#define _SCRIPTLUA_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "Script.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------



class SCRIPTLUA : public SCRIPT
{
	public:
																				SCRIPTLUA								();
		virtual														 ~SCRIPTLUA								();

		virtual	bool												Load										(XPATH& xpath);
		virtual	bool												Save										(XPATH& xpath);
		
		virtual	SCRIPTERRORMSG	 						Run											(int* returnval = NULL);
				
		virtual bool												HaveError								(SCRIPTERRORMSG errorcode);
			

	private:
			
		void																Clean										()
																				{	
																					
																				}		
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	

#endif

