/*------------------------------------------------------------------------------------------
//	DIOSTM32FXXXGPIO.H
*/	
/**	
// \class 
//   
//  STM32FXXX DIO port data class
//   
//	@author	 Abraham J. Velez
//	@version 25/07/2009 07:13:30 p.m.
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOSTM32FXXXGPIO_H_
#define _DIOSTM32FXXXGPIO_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOGPIO.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
	
class DIOSTM32FXXXGPIO : public DIOGPIO
{
	public:
													DIOSTM32FXXXGPIO						();
		virtual	  				 	 ~DIOSTM32FXXXGPIO						();

		bool									Ini													(XPATH* xpath = NULL);

		bool									SetMode											(XBYTE pin, bool isinput, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A);
    bool							    SetMode							        (XBYTE pin, XBYTE mode, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A)						{ return false;											}		
		bool									Get													(XBYTE pin, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A);
		bool									Set													(XBYTE pin, bool active, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A);
		
		bool									End													();

	private:

		void									Clean												()
													{


													}
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

