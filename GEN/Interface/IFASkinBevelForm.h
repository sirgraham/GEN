//------------------------------------------------------------------------------------------
//	IFASKINBEVELFORM.H
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
	
#ifndef _IFASKINBEVELFORM_H_
#define _IFASKINBEVELFORM_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "IFASkinManager.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
	
//---- CLASS -------------------------------------------------------------------------------

class IFASKINBEVELFORM : public IFASKINOBJ
{
	public:
											IFASKINBEVELFORM		();
		virtual					 ~IFASKINBEVELFORM		();

		virtual	bool			Draw								(IFAOBJ* obj,int x, int y);	
};


	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

