//------------------------------------------------------------------------------------------
//  XSTM32FXXX_HAL.H
//
/**
// \class
//
//  STM32Fxxx_HAL includes
//
//  @author  Abraham J. Velez
//  @version 05/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSTM32FXXX_HAL_H_
#define _XSTM32FXXX_HAL_H_


//---- INCLUDES ----------------------------------------------------------------------------

#if defined(STM32F072xB)
#define HW_STM32F0XX
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#endif

#if defined(STM32F303xC) || defined(STM32F303xE)
#define HW_STM32F3XX
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#endif

#if defined(STM32F407xx)|| defined(STM32F411xE) || defined(STM32F446xx)
#define HW_STM32F4XX
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#endif


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
