//------------------------------------------------------------------------------------------
//	XLINUXSLEEP.H
//	
/**	
// \class 
//   
//  LINUX Sleep class
//   
//	@author	 Abraham J. Velez
//	@version 04/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XLINUXSLEEP_H_
#define _XLINUXSLEEP_H_
	
						
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSleep.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XLINUXSLEEP : public XSLEEP
{
	public:

												XLINUXSLEEP											();
		virtual						 ~XLINUXSLEEP											();
		        
    void								Seconds													(int seconds);
		void								MilliSeconds										(int milliseconds);
    void								MicroSeconds										(int microseconds);
		void								NanoSeconds											(int nanoseconds);
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------
										
#endif

