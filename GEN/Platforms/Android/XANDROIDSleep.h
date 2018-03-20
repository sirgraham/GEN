//------------------------------------------------------------------------------------------
//	XANDROIDSLEEP.H
//	
/**	
// \class 
//   
//  ANDROID Sleep class
//   
//	@author	 Abraham J. Velez
//	@version 04/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XANDROIDSLEEP_H_
#define _XANDROIDSLEEP_H_
	
						
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSleep.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------


class XANDROIDSLEEP : public XSLEEP
{
	public:

												XANDROIDSLEEP										();
		virtual						 ~XANDROIDSLEEP										();
		        
    void								Seconds													(int seconds);
		void								MilliSeconds										(int milliseconds);
    void								MicroSeconds										(int microseconds);
		void								NanoSeconds											(int nanoseconds);
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------
										
#endif

