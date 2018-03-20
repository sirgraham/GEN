//------------------------------------------------------------------------------------------
//	XWINDOWSDATETIME.H
//	
/**	
// \class 
//   
//  WINDOWS date time class
//   
//	@author	 Abraham J. Velez
//	@version 04/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XWINDOWSDATETIME_H_
#define _XWINDOWSDATETIME_H_
	
						
//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>
#include <time.h>

#include "XBase.h"
#include "XDateTime.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------



class XWINDOWSDATETIME : public XDATETIME
{
	public:
																XWINDOWSDATETIME									();
		virtual										 ~XWINDOWSDATETIME									();

		bool												Read															();
		bool												Write															();

		bool												GetFileDateTime										(XPATH& xpath, void* tmzip, XDWORD* dt);
		bool												GetFileDateTime										(XPATH& xpath);

		int													GetDifferenceGMT									();

		bool												IsDayLigthSavingTime							(int* bias = NULL);
		
	protected:

		void												GetActualDateTime									(XWINDOWSDATETIME* time);
		void												SetActualDateTime									(XWINDOWSDATETIME* time);
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------
										
#endif

