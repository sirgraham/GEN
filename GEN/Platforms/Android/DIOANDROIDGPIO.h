/*------------------------------------------------------------------------------------------
//	DIOANDROIDGPIO.H
*/	
/**	
// \class 
//   
//  ANDROID DIO port data class
//   
//	@author	 Abraham J. Velez
//	@version 25/07/2009 07:13:30 p.m.
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOANDROIDGPIO_H_
#define _DIOANDROIDGPIO_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOGPIO.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/




/*---- CLASS -----------------------------------------------------------------------------*/
	
class DIOANDROIDGPIO : public DIOGPIO
{
	public:
										 DIOANDROIDGPIO							();
		virtual	  			~DIOANDROIDGPIO							();

		bool						 Ini													(XPATH* xpath = NULL);

		bool						 SetMode											(XBYTE pin, bool isinput);

		bool						 Get													(XBYTE pin);
		bool						 Set													(XBYTE pin, bool active);
		
		bool						 End													();

	private:

		void						 Clean												();	
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

