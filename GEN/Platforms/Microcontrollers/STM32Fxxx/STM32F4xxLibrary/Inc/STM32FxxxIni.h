/*------------------------------------------------------------------------------------------
//	STM32F4XXMINI.H
*/	
/**	
// \class 
//   
//  STM32Fxxx INI for STM32F0xx 
//   
//	@author	 Abraham J. Velez
//	@version 16/09/2012 12:43:04
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _STM32FXXXINI_H_
#define _STM32FXXXINI_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "Main.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
extern TIM_HandleTypeDef htim3;

extern void 		SystemClock_Config	 (void);
extern void 		MX_NVIC_Init         (void);
extern void 		MX_GPIO_Init				 (void);
extern void 		MX_TIMER3_Init       (void);
extern void 		Error_Handler				 (void);

#endif

