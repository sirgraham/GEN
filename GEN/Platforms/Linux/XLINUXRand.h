//------------------------------------------------------------------------------------------
//	XLINUXRAND.H
//	
/**	
// \class 
//   
//  LINUX rand class
//   
//	@author	 Abraham J. Velez
//	@version 22/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XLINUXRAND_H_
#define _XLINUXRAND_H_
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include <stdlib.h>

#include "XRand.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	

//---- CLASS -------------------------------------------------------------------------------


class XLINUXRAND : public XRAND
{
	public:
									XLINUXRAND				();
		virtual			 ~XLINUXRAND				();

		bool					Ini								();
		int						MaxElements				(int max);
		int     			Max								(int max);
		int     			Between						(int min, int max);
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------

	
#endif

