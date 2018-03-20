/*------------------------------------------------------------------------------------------
//	STM32FXXXMAIN.H
*/	
/**	
// \class 
//   
//  STM32Fxxx MAIN
//   
//	@author	 Abraham J. Velez
//	@version 16/09/2012 12:43:04
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _STM32FXXXMAIN_H_
#define _STM32FXXXMAIN_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "Main.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class XSTM32FXXXSYSTEM;
class XAPPLICATION;

class STM32FXXXMAIN : public MAIN
{
	public:
																	STM32FXXXMAIN					();
		virtual											 ~STM32FXXXMAIN					();

		bool													Ini										();
				
		bool													Update								();
		bool													End										();

		XAPPLICATION*			      			GetXApplication				();	
		
	private:

		void													Clean									();
				
		XSTM32FXXXSYSTEM*							xsystem;
		XAPPLICATION*	 						    xapplication;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif

