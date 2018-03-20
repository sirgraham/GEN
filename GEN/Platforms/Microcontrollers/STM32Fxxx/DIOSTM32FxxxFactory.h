//------------------------------------------------------------------------------------------
//	DIOSTM32FXXXFACTORY.H
//	
/**	
// \class 
//   
//  STM32Fxxx utils platform factory class
//   
//	@author	 Abraham J. Velez
//	@version 15/07/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTM32FXXXFACTORY_H_
#define _DIOSTM32FXXXFACTORY_H_
	

#ifdef DIO_ACTIVE
	
//---- INCLUDES ----------------------------------------------------------------------------	

#include "DIOFactory.h"
	
//---- DEFINES & ENUMS  ------------------------------------------------------------------
	
	
//---- CLASS -------------------------------------------------------------------------------
	
class DIOSTM32FXXXFACTORY : public DIOFACTORY
{
	public:

    #ifdef ANYTYPEOFDIOSTREAMIO
		DIOSTREAM*									CreateStreamIO					(DIOSTREAMCONFIG* config);
		bool												DeleteStreamIO					(DIOSTREAM* diostream);		
		#endif

		
		#ifdef DIOGPIO_ACTIVE
		DIOGPIO*							    	CreateGPIO							();
		bool												DeleteGPIO							(DIOGPIO* gpio);
		#endif
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

#endif

