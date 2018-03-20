//------------------------------------------------------------------------------------------
//	XWINDOWSSLEEP.H
//	
/**	
// \class 
//   
//  WINDOWS sleep class
//   
//	@author	 Abraham J. Velez
//	@version 04/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XWINDOWSSLEEP_H_
#define _XWINDOWSSLEEP_H_
	
						
//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>
#include <time.h>

#include "XBase.h"
#include "XSleep.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XWINDOWSSLEEP : public XSLEEP
{
	public:

																XWINDOWSSLEEP											();
		virtual										 ~XWINDOWSSLEEP											();

		void												Seconds														(int seconds);
		void												MilliSeconds											(int milliseconds);
    void												MicroSeconds											(int microseconds);
		
		void												Clean															()
																{																
																	PCfrequencymilliseconds = 0.0;
																	PCfrequencymicroseconds	= 0.0;
																}

	private:

		LARGE_INTEGER								frequency;
		double											PCfrequencymilliseconds;
		double											PCfrequencymicroseconds;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------
										
#endif

