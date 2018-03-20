//------------------------------------------------------------------------------------------
//	XWINDOWSCONSOLE.H
//	
/**	
// \class 
//   
//  WINDOWS system class
//   
//	@author	 Abraham J. Velez
//	@version 03/03/2004 12:15:55
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XWINDOWSCONSOLE_H_
#define _XWINDOWSCONSOLE_H_
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XConsole.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XWINDOWSCONSOLE : public XCONSOLE
{
	public:
												XWINDOWSCONSOLE									();
		virtual						 ~XWINDOWSCONSOLE									();

		bool								GetSize													(int& width, int& height);
		bool								SetSize													(int width, int height);

		bool								Maximize												();
		bool								Minimize												();

		bool                Hide														();
		bool								IsHide													();
		bool								UnHide													();

		bool								Print														(XCHAR* string);
		bool								Clear														();

		bool								KBHit														();
		int									GetChar													();
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

