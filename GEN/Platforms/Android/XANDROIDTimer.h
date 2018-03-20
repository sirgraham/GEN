//------------------------------------------------------------------------------------------
//	XANDROIDTIMER.H
//	
/**	
// \class 
//   
//  ANDROID timer class
//   
//	@author	 Abraham J. Velez
//	@version 04/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XANDROIDTIMER_H_
#define _XANDROIDTIMER_H_
	
						
//---- INCLUDES ----------------------------------------------------------------------------

#include "XTimer.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------


class XANDROIDTIMER : public XTIMER
{
	public:

												XANDROIDTIMER										();
		virtual						 ~XANDROIDTIMER										();
		       
		XQWORD							GetMicroSecondsTickCounter			();
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------
										
#endif

