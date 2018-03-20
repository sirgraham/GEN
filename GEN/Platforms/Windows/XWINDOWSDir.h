//------------------------------------------------------------------------------------------
//	XWINDOWSDIR.H
//	
/**	
// \class 
//   
//  WINDOWS Dir class
//   
//	@author	 Abraham J. Velez
//	@version 29/10/2004 10:14:00
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XWINDOWSDIR_H_
#define _XWINDOWSDIR_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>

#include "XBase.h"
#include "XDir.h"
#include "XPath.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
	
//---- CLASS -------------------------------------------------------------------------------
	
class XWINDOWSDIR : public XDIR
{
	public:
											  XWINDOWSDIR						  ();
		virtual						 ~XWINDOWSDIR					    ();

		bool								Exist										(XCHAR* path);
		bool								Make										(XCHAR* path);
		bool								ChangeTo								(XCHAR* path);
		bool								Delete									(XCHAR* path,bool all);		

		bool								FirstSearch							(XCHAR* xpath,XCHAR* patternsearch,XDIRELEMENT* searchelement);
		bool								NextSearch							(XDIRELEMENT* searchelement);

	private:

		void								Clean										();
    bool                ReconvertSearchData			(XDIRELEMENT* searchelement);
	
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

