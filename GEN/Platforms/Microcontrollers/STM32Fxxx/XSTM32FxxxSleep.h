//------------------------------------------------------------------------------------------
//	XSTM32FXXXSLEEP.H
//	
/**	
// \class 
//   
//  STM32Fxxx Sleep class
//   
//	@author	 Abraham J. Velez
//	@version 04/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XSTM32FXXXSLEEP_H_
#define _XSTM32FXXXSLEEP_H_
	
						
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSleep.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XSTM32FXXXSLEEP : public XSLEEP
{
	public:

												XSTM32FXXXSLEEP									();
		virtual						 ~XSTM32FXXXSLEEP									();
		        
    void								Seconds													(int seconds);
		void								MilliSeconds										(int milliseconds);
    void								MicroSeconds										(int microseconds);
		void								NanoSeconds											(int nanoseconds);
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------
										
#endif

