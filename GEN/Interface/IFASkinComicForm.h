//------------------------------------------------------------------------------------------
//	IFASKINCOMICFORM.H
//	
/**	
// \class 
//   
//  Interface Skin Default Form CLASS
//   
//	@author	 Abraham J. Velez
//	@version 09/11/2004 9:35:24
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFASKINCOMICFORM_H_
#define _IFASKINCOMICFORM_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "IFASkinManager.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
	
//---- CLASS -------------------------------------------------------------------------------

class IFASKINCOMICFORM : public IFASKINOBJ
{
	public:
											IFASKINCOMICFORM		();
		virtual					 ~IFASKINCOMICFORM		();

		virtual	bool			Draw								(IFAOBJ* obj,int x, int y);	
};


	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

