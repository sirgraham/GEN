/*------------------------------------------------------------------------------------------
//	DIOSTM32FXXXGPIO.CPP
//	
//	STM32Fxxx DIO port data class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 25/07/2009 07:14:17 p.m.
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XSTM32Fxxx_HAL.h"
#include "XDebug.h"

#include "DIOSTM32FXXXGPIO.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

//static volatile uint32_t* RPI_gpio;

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOSTM32FXXXGPIO::DIOSTM32FXXXGPIO
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:18:20 p.m.
//	
//	@return				
*/
/*-----------------------------------------------------------------*/
DIOSTM32FXXXGPIO::DIOSTM32FXXXGPIO() : DIOGPIO()
{
	Clean();
}


/*-------------------------------------------------------------------
//  DIOSTM32FXXXGPIO::~DIOSTM32FXXXGPIO
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:18:44 p.m.
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOSTM32FXXXGPIO::~DIOSTM32FXXXGPIO()
{
	Clean();
}



/*-------------------------------------------------------------------
//	DIOSTM32FXXXGPIO::Ini
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/10/2015 14:25:06
//	
//	@return 			bool : 
//
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTM32FXXXGPIO::Ini(XPATH* xpath)
{	
  #ifdef STM32F072xB
  
	/*PA0 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 0] = GPIO_PIN_0;
	/*PA1 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 1] = DIOGPIO_PINNOTUSE;    // UART PORT 4 RX
	/*PA2 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 2] = DIOGPIO_PINNOTUSE;    // UART PORT 2 TX
	/*PA3 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 3] = DIOGPIO_PINNOTUSE;    // UART PORT 3 RX
	/*PA4 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 4] = GPIO_PIN_4;
	/*PA5 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 5] = DIOGPIO_PINNOTUSE;    // SPI 1 SCK
	/*PA6 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 6] = DIOGPIO_PINNOTUSE;    // SPI 1 MISO
	/*PA7 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 7] = DIOGPIO_PINNOTUSE;    // SPI 1 MOSI
	/*PA8 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 8] = GPIO_PIN_8;
	/*PA9 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 9] = DIOGPIO_PINNOTUSE;    // UART PORT 1 TX
	/*PA10*/ GPIOvalue[DIOGPIO_PINSGROUP_A][10] = DIOGPIO_PINNOTUSE;    // UART PORT 1 RX
	/*PA11*/ GPIOvalue[DIOGPIO_PINSGROUP_A][11] = DIOGPIO_PINNOTUSE;    // USB  DM
	/*PA12*/ GPIOvalue[DIOGPIO_PINSGROUP_A][12] = DIOGPIO_PINNOTUSE;    // USB  DP
	/*PA13*/ GPIOvalue[DIOGPIO_PINSGROUP_A][13] = DIOGPIO_PINNOTUSE;
	/*PA14*/ GPIOvalue[DIOGPIO_PINSGROUP_A][14] = DIOGPIO_PINNOTUSE;
	/*PA15*/ GPIOvalue[DIOGPIO_PINSGROUP_A][15] = DIOGPIO_PINNOTUSE;
		
	/*PB0 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 0] = GPIO_PIN_0; 
	/*PB1 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 1] = GPIO_PIN_1; 
	/*PB2 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 2] = DIOGPIO_PINNOTUSE; 
	/*PB3 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 3] = DIOGPIO_PINNOTUSE; 
	/*PB4 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 4] = DIOGPIO_PINNOTUSE; 
	/*PB5 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 5] = GPIO_PIN_5; 
	/*PB6 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 6] = DIOGPIO_PINNOTUSE;    // I2C  PORT 1 SDA
	/*PB7 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 7] = DIOGPIO_PINNOTUSE;    // I2C  PORT 1 SCL
	/*PB8 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 8] = GPIO_PIN_8; 
	/*PB9 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 9] = GPIO_PIN_9; 
	/*PB10*/ GPIOvalue[DIOGPIO_PINSGROUP_B][10] = GPIO_PIN_10;
	/*PB11*/ GPIOvalue[DIOGPIO_PINSGROUP_B][11] = GPIO_PIN_11;
	/*PB12*/ GPIOvalue[DIOGPIO_PINSGROUP_B][12] = GPIO_PIN_12;
	/*PB13*/ GPIOvalue[DIOGPIO_PINSGROUP_B][13] = DIOGPIO_PINNOTUSE;    // SPI 2 SCK
	/*PB14*/ GPIOvalue[DIOGPIO_PINSGROUP_B][14] = DIOGPIO_PINNOTUSE;    // SPI 2 MISO
	/*PB15*/ GPIOvalue[DIOGPIO_PINSGROUP_B][15] = DIOGPIO_PINNOTUSE;    // SPI 2 MOSI
	
	/*PC0 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 0] = GPIO_PIN_0; 
	/*PC1 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 1] = GPIO_PIN_1; 
	/*PC2 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 2] = GPIO_PIN_2; 
	/*PC3 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 3] = GPIO_PIN_3; 
	/*PC4 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 4] = DIOGPIO_PINNOTUSE;    // UART PORT 3 TX   
	/*PC5 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 5] = GPIO_PIN_5; 
	/*PC6 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 6] = GPIO_PIN_6; 
	/*PC7 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 7] = GPIO_PIN_7; 
	/*PC8 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 8] = GPIO_PIN_8; 
	/*PC9 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 9] = GPIO_PIN_9; 
	/*PC10*/ GPIOvalue[DIOGPIO_PINSGROUP_C][10] = DIOGPIO_PINNOTUSE;    // UART PORT 4 TX   (Debug)
	/*PC11*/ GPIOvalue[DIOGPIO_PINSGROUP_C][11] = DIOGPIO_PINNOTUSE;    // UART PORT 3 RX   (Debug)
 	/*PC12*/ GPIOvalue[DIOGPIO_PINSGROUP_C][12] = GPIO_PIN_12;
	/*PC13*/ GPIOvalue[DIOGPIO_PINSGROUP_C][13] = DIOGPIO_PINNOTUSE;
	/*PC14*/ GPIOvalue[DIOGPIO_PINSGROUP_C][14] = DIOGPIO_PINNOTUSE;
	/*PC15*/ GPIOvalue[DIOGPIO_PINSGROUP_C][15] = DIOGPIO_PINNOTUSE;
  
  #endif
  
  
  
  
  #ifdef STM32F303xC
  
	/*PA0 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 0] = GPIO_PIN_0;
	/*PA1 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 1] = GPIO_PIN_1;
	/*PA2 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 2] = GPIO_PIN_2;
	/*PA3 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 3] = GPIO_PIN_3;
	/*PA4 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 4] = GPIO_PIN_4;
	/*PA5 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 5] = DIOGPIO_PINNOTUSE;    // SPI 1 SCK
	/*PA6 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 6] = DIOGPIO_PINNOTUSE;    // SPI 1 MISO
	/*PA7 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 7] = DIOGPIO_PINNOTUSE;    // SPI 1 MOSI
	/*PA8 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 8] = GPIO_PIN_8;
	/*PA9 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 9] = GPIO_PIN_9;
	/*PA10*/ GPIOvalue[DIOGPIO_PINSGROUP_A][10] = GPIO_PIN_10;
	/*PA11*/ GPIOvalue[DIOGPIO_PINSGROUP_A][11] = DIOGPIO_PINNOTUSE;    // USB  DM
	/*PA12*/ GPIOvalue[DIOGPIO_PINSGROUP_A][12] = DIOGPIO_PINNOTUSE;    // USB  DP
	/*PA13*/ GPIOvalue[DIOGPIO_PINSGROUP_A][13] = DIOGPIO_PINNOTUSE;    // SWDIO
	/*PA14*/ GPIOvalue[DIOGPIO_PINSGROUP_A][14] = DIOGPIO_PINNOTUSE;    // SWCLK
	/*PA15*/ GPIOvalue[DIOGPIO_PINSGROUP_A][15] = GPIO_PIN_15;
		
	/*PB0 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 0] = GPIO_PIN_0; 
	/*PB1 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 1] = GPIO_PIN_1; 
	/*PB2 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 2] = GPIO_PIN_2; 
	/*PB3 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 3] = DIOGPIO_PINNOTUSE;    // SWO
	/*PB4 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 4] = GPIO_PIN_4; 
	/*PB5 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 5] = GPIO_PIN_5; 
	/*PB6 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 6] = DIOGPIO_PINNOTUSE;    // I2C  PORT 1 SDA
	/*PB7 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 7] = DIOGPIO_PINNOTUSE;    // I2C  PORT 1 SCL
	/*PB8 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 8] = GPIO_PIN_8; 
	/*PB9 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 9] = GPIO_PIN_9; 
	/*PB10*/ GPIOvalue[DIOGPIO_PINSGROUP_B][10] = GPIO_PIN_10;
	/*PB11*/ GPIOvalue[DIOGPIO_PINSGROUP_B][11] = GPIO_PIN_11;
	/*PB12*/ GPIOvalue[DIOGPIO_PINSGROUP_B][12] = GPIO_PIN_12;
	/*PB13*/ GPIOvalue[DIOGPIO_PINSGROUP_B][13] = GPIO_PIN_13;
	/*PB14*/ GPIOvalue[DIOGPIO_PINSGROUP_B][14] = GPIO_PIN_14;
	/*PB15*/ GPIOvalue[DIOGPIO_PINSGROUP_B][15] = DIOGPIO_PINNOTUSE;    // SPI 2 MOSI
	
	/*PC0 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 0] = GPIO_PIN_0; 
	/*PC1 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 1] = GPIO_PIN_1; 
	/*PC2 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 2] = GPIO_PIN_2; 
	/*PC3 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 3] = GPIO_PIN_3; 
	/*PC4 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 4] = DIOGPIO_PINNOTUSE;   // UART PORT 1 TX   
	/*PC5 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 5] = DIOGPIO_PINNOTUSE;   // UART PORT 1 RX   
	/*PC6 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 6] = GPIO_PIN_6; 
	/*PC7 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 7] = GPIO_PIN_7; 
	/*PC8 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 8] = GPIO_PIN_8; 
	/*PC9 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 9] = GPIO_PIN_9; 
	/*PC10*/ GPIOvalue[DIOGPIO_PINSGROUP_C][10] = DIOGPIO_PINNOTUSE;    // UART PORT 4 TX   (Debug)
	/*PC11*/ GPIOvalue[DIOGPIO_PINSGROUP_C][11] = DIOGPIO_PINNOTUSE;    // UART PORT 4 RX   (Debug)
 	/*PC12*/ GPIOvalue[DIOGPIO_PINSGROUP_C][12] = GPIO_PIN_12;
	/*PC13*/ GPIOvalue[DIOGPIO_PINSGROUP_C][13] = GPIO_PIN_13;
	/*PC14*/ GPIOvalue[DIOGPIO_PINSGROUP_C][14] = DIOGPIO_PINNOTUSE;
	/*PC15*/ GPIOvalue[DIOGPIO_PINSGROUP_C][15] = DIOGPIO_PINNOTUSE;
  
  /*PD0 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 0] = GPIO_PIN_0; 
	/*PD1 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 1] = GPIO_PIN_1; 
	/*PD2 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 2] = GPIO_PIN_2; 
	/*PD3 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 3] = GPIO_PIN_3; 
	/*PD4 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 4] = GPIO_PIN_4; 
	/*PD5 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 5] = GPIO_PIN_5; 
	/*PD6 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 6] = GPIO_PIN_6; 
	/*PD7 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 7] = GPIO_PIN_7; 
	/*PD8 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 8] = GPIO_PIN_8; 
	/*PD9 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 9] = GPIO_PIN_9; 
	/*PD10*/ GPIOvalue[DIOGPIO_PINSGROUP_D][10] = GPIO_PIN_10;
	/*PD11*/ GPIOvalue[DIOGPIO_PINSGROUP_D][11] = GPIO_PIN_11; 
 	/*PD12*/ GPIOvalue[DIOGPIO_PINSGROUP_D][12] = GPIO_PIN_12;
	/*PD13*/ GPIOvalue[DIOGPIO_PINSGROUP_D][13] = GPIO_PIN_13;
	/*PD14*/ GPIOvalue[DIOGPIO_PINSGROUP_D][14] = GPIO_PIN_14;
	/*PD15*/ GPIOvalue[DIOGPIO_PINSGROUP_D][15] = GPIO_PIN_15;
  
  /*PE0 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 0] = DIOGPIO_PINNOTUSE;  
	/*PE1 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 1] = DIOGPIO_PINNOTUSE; 
	/*PE2 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 2] = DIOGPIO_PINNOTUSE;    
	/*PE3 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 3] = DIOGPIO_PINNOTUSE;    
	/*PE4 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 4] = DIOGPIO_PINNOTUSE;    
	/*PE5 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 5] = DIOGPIO_PINNOTUSE;    
	/*PE6 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 6] = GPIO_PIN_6; 
	/*PE7 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 7] = GPIO_PIN_7; 
	/*PE8 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 8] = GPIO_PIN_8; 
	/*PE9 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 9] = GPIO_PIN_9; 
	/*PE10*/ GPIOvalue[DIOGPIO_PINSGROUP_E][10] = GPIO_PIN_10; 
	/*PE11*/ GPIOvalue[DIOGPIO_PINSGROUP_E][11] = GPIO_PIN_11; 
 	/*PE12*/ GPIOvalue[DIOGPIO_PINSGROUP_E][12] = GPIO_PIN_12;
	/*PE13*/ GPIOvalue[DIOGPIO_PINSGROUP_E][13] = GPIO_PIN_13; 
	/*PE14*/ GPIOvalue[DIOGPIO_PINSGROUP_E][14] = GPIO_PIN_14; 
	/*PE15*/ GPIOvalue[DIOGPIO_PINSGROUP_E][15] = GPIO_PIN_15; 
  
  /*PF0 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 0] = DIOGPIO_PINNOTUSE;
	/*PF1 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 1] = DIOGPIO_PINNOTUSE;
	/*PF2 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 2] = GPIO_PIN_2; 
	/*PF3 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 3] = DIOGPIO_PINNOTUSE;    
	/*PF4 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 4] = GPIO_PIN_4;   
	/*PF5 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 5] = DIOGPIO_PINNOTUSE;   
	/*PF6 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 6] = GPIO_PIN_6; 
	/*PF7 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 7] = DIOGPIO_PINNOTUSE;   
	/*PF8 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 8] = DIOGPIO_PINNOTUSE;   
	/*PF9 */ GPIOvalue[DIOGPIO_PINSGROUP_F][ 9] = DIOGPIO_PINNOTUSE;    // SPI 1 SCK
	/*PF10*/ GPIOvalue[DIOGPIO_PINSGROUP_F][10] = GPIO_PIN_10; 
	/*PF11*/ GPIOvalue[DIOGPIO_PINSGROUP_F][11] = DIOGPIO_PINNOTUSE;    
 	/*PF12*/ GPIOvalue[DIOGPIO_PINSGROUP_F][12] = DIOGPIO_PINNOTUSE;   
	/*PF13*/ GPIOvalue[DIOGPIO_PINSGROUP_F][13] = DIOGPIO_PINNOTUSE;
	/*PF14*/ GPIOvalue[DIOGPIO_PINSGROUP_F][14] = DIOGPIO_PINNOTUSE;
	/*PF15*/ GPIOvalue[DIOGPIO_PINSGROUP_F][15] = DIOGPIO_PINNOTUSE;
  
  #endif
  
  
  #ifdef STM32F303xE
  
	/*PA0 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 0] = GPIO_PIN_0;
	/*PA1 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 1] = GPIO_PIN_1;
	/*PA2 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 2] = DIOGPIO_PINNOTUSE;    
	/*PA3 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 3] = DIOGPIO_PINNOTUSE;    
	/*PA4 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 4] = GPIO_PIN_4;
	/*PA5 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 5] = GPIO_PIN_5;
	/*PA6 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 6] = GPIO_PIN_6;
	/*PA7 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 7] = GPIO_PIN_7;
	/*PA8 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 8] = GPIO_PIN_8;
	/*PA9 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 9] = DIOGPIO_PINNOTUSE;    
	/*PA10*/ GPIOvalue[DIOGPIO_PINSGROUP_A][10] = DIOGPIO_PINNOTUSE;     
	/*PA11*/ GPIOvalue[DIOGPIO_PINSGROUP_A][11] = DIOGPIO_PINNOTUSE;    
	/*PA12*/ GPIOvalue[DIOGPIO_PINSGROUP_A][12] = DIOGPIO_PINNOTUSE;    
	/*PA13*/ GPIOvalue[DIOGPIO_PINSGROUP_A][13] = DIOGPIO_PINNOTUSE;
	/*PA14*/ GPIOvalue[DIOGPIO_PINSGROUP_A][14] = DIOGPIO_PINNOTUSE;
	/*PA15*/ GPIOvalue[DIOGPIO_PINSGROUP_A][15] = DIOGPIO_PINNOTUSE;
		
	/*PB0 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 0] = GPIO_PIN_0; 
	/*PB1 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 1] = GPIO_PIN_1; 
	/*PB2 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 2] = GPIO_PIN_2; 
	/*PB3 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 3] = DIOGPIO_PINNOTUSE; 
	/*PB4 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 4] = GPIO_PIN_4; 
	/*PB5 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 5] = GPIO_PIN_5; 
	/*PB6 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 6] = GPIO_PIN_6; 
	/*PB7 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 7] = DIOGPIO_PINNOTUSE;    
	/*PB8 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 8] = GPIO_PIN_8; 
	/*PB9 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 9] = GPIO_PIN_9; 
	/*PB10*/ GPIOvalue[DIOGPIO_PINSGROUP_B][10] = GPIO_PIN_10;
	/*PB11*/ GPIOvalue[DIOGPIO_PINSGROUP_B][11] = GPIO_PIN_11;
	/*PB12*/ GPIOvalue[DIOGPIO_PINSGROUP_B][12] = GPIO_PIN_12;
	/*PB13*/ GPIOvalue[DIOGPIO_PINSGROUP_B][13] = DIOGPIO_PINNOTUSE;    // SPI 2 SCK
	/*PB14*/ GPIOvalue[DIOGPIO_PINSGROUP_B][14] = DIOGPIO_PINNOTUSE;    // SPI 2 MISO
	/*PB15*/ GPIOvalue[DIOGPIO_PINSGROUP_B][15] = DIOGPIO_PINNOTUSE;    // SPI 2 MOSI
	
	/*PC0 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 0] = GPIO_PIN_0; 
	/*PC1 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 1] = GPIO_PIN_1; 
	/*PC2 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 2] = GPIO_PIN_2; 
	/*PC3 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 3] = GPIO_PIN_3; 
	/*PC4 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 4] = DIOGPIO_PINNOTUSE;    // UART PORT 1 TX   
	/*PC5 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 5] = DIOGPIO_PINNOTUSE;    // UART PORT 1 TX  
	/*PC6 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 6] = GPIO_PIN_6; 
	/*PC7 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 7] = GPIO_PIN_7; 
	/*PC8 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 8] = GPIO_PIN_8; 
	/*PC9 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 9] = GPIO_PIN_9; 
	/*PC10*/ GPIOvalue[DIOGPIO_PINSGROUP_C][10] = DIOGPIO_PINNOTUSE;    // UART PORT 4 TX   (Debug)
	/*PC11*/ GPIOvalue[DIOGPIO_PINSGROUP_C][11] = DIOGPIO_PINNOTUSE;    // UART PORT 3 RX   (Debug)
 	/*PC12*/ GPIOvalue[DIOGPIO_PINSGROUP_C][12] = GPIO_PIN_12;
	/*PC13*/ GPIOvalue[DIOGPIO_PINSGROUP_C][13] = GPIO_PIN_13;
	/*PC14*/ GPIOvalue[DIOGPIO_PINSGROUP_C][14] = DIOGPIO_PINNOTUSE;
	/*PC15*/ GPIOvalue[DIOGPIO_PINSGROUP_C][15] = DIOGPIO_PINNOTUSE;
  
  #endif
  
  #if defined(STM32F411xE) || defined(STM32F407xx)
  
 	/*PA0 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 0] = GPIO_PIN_0;
	/*PA1 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 1] = GPIO_PIN_1;
	/*PA2 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 2] = DIOGPIO_PINNOTUSE;   // USART PORT 2 TX 
	/*PA3 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 3] = DIOGPIO_PINNOTUSE;   // USART PORT 2 RX  
	/*PA4 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 4] = GPIO_PIN_4;
	/*PA5 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 5] = DIOGPIO_PINNOTUSE;    // SPI 1 SCK
	/*PA6 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 6] = DIOGPIO_PINNOTUSE;    // SPI 1 MISO
	/*PA7 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 7] = DIOGPIO_PINNOTUSE;    // SPI 1 MOSI
	/*PA8 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 8] = GPIO_PIN_8;
	/*PA9 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 9] = DIOGPIO_PINNOTUSE;   // USART PORT 1 TX 
	/*PA10*/ GPIOvalue[DIOGPIO_PINSGROUP_A][10] = DIOGPIO_PINNOTUSE;   // USART PORT 1 RX 
	/*PA11*/ GPIOvalue[DIOGPIO_PINSGROUP_A][11] = DIOGPIO_PINNOTUSE;    // USB  DM
	/*PA12*/ GPIOvalue[DIOGPIO_PINSGROUP_A][12] = DIOGPIO_PINNOTUSE;    // USB  DP
	/*PA13*/ GPIOvalue[DIOGPIO_PINSGROUP_A][13] = DIOGPIO_PINNOTUSE;    // SWDIO
	/*PA14*/ GPIOvalue[DIOGPIO_PINSGROUP_A][14] = DIOGPIO_PINNOTUSE;    // SWCLK
	/*PA15*/ GPIOvalue[DIOGPIO_PINSGROUP_A][15] = GPIO_PIN_15;
		
	/*PB0 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 0] = GPIO_PIN_0; 
	/*PB1 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 1] = GPIO_PIN_1; 
	/*PB2 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 2] = GPIO_PIN_2; 
	/*PB3 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 3] = GPIO_PIN_3; 
	/*PB4 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 4] = GPIO_PIN_4; 
	/*PB5 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 5] = GPIO_PIN_5; 
	/*PB6 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 6] = DIOGPIO_PINNOTUSE;    // I2C  PORT 1 SDA
	/*PB7 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 7] = DIOGPIO_PINNOTUSE;    // I2C  PORT 1 SCL
	/*PB8 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 8] = GPIO_PIN_8; 
	/*PB9 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 9] = GPIO_PIN_9; 
	/*PB10*/ GPIOvalue[DIOGPIO_PINSGROUP_B][10] = GPIO_PIN_10;
	/*PB11*/ GPIOvalue[DIOGPIO_PINSGROUP_B][11] = DIOGPIO_PINNOTUSE; 
	/*PB12*/ GPIOvalue[DIOGPIO_PINSGROUP_B][12] = GPIO_PIN_12;
	/*PB13*/ GPIOvalue[DIOGPIO_PINSGROUP_B][13] = GPIO_PIN_13;
	/*PB14*/ GPIOvalue[DIOGPIO_PINSGROUP_B][14] = GPIO_PIN_14;
	/*PB15*/ GPIOvalue[DIOGPIO_PINSGROUP_B][15] = GPIO_PIN_15;
	
	/*PC0 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 0] = GPIO_PIN_0; 
	/*PC1 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 1] = GPIO_PIN_1; 
	/*PC2 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 2] = GPIO_PIN_2; 
	/*PC3 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 3] = GPIO_PIN_3; 
	/*PC4 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 4] = GPIO_PIN_4;
	/*PC5 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 5] = GPIO_PIN_5;
	/*PC6 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 6] = GPIO_PIN_6; 
	/*PC7 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 7] = GPIO_PIN_7; 
	/*PC8 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 8] = GPIO_PIN_8; 
	/*PC9 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 9] = GPIO_PIN_9; 
	/*PC10*/ GPIOvalue[DIOGPIO_PINSGROUP_C][10] = GPIO_PIN_10;
	/*PC11*/ GPIOvalue[DIOGPIO_PINSGROUP_C][11] = GPIO_PIN_11;
 	/*PC12*/ GPIOvalue[DIOGPIO_PINSGROUP_C][12] = GPIO_PIN_12;
	/*PC13*/ GPIOvalue[DIOGPIO_PINSGROUP_C][13] = GPIO_PIN_13;
	/*PC14*/ GPIOvalue[DIOGPIO_PINSGROUP_C][14] = DIOGPIO_PINNOTUSE;
	/*PC15*/ GPIOvalue[DIOGPIO_PINSGROUP_C][15] = DIOGPIO_PINNOTUSE;
  
  /*PD0 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 0] = GPIO_PIN_0; 
	/*PD1 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 1] = GPIO_PIN_1; 
	/*PD2 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 2] = GPIO_PIN_2; 
	/*PD3 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 3] = GPIO_PIN_3; 
	/*PD4 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 4] = GPIO_PIN_4; 
	/*PD5 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 5] = GPIO_PIN_5; 
	/*PD6 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 6] = GPIO_PIN_6; 
	/*PD7 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 7] = GPIO_PIN_7; 
	/*PD8 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 8] = GPIO_PIN_8; 
	/*PD9 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 9] = GPIO_PIN_9; 
	/*PD10*/ GPIOvalue[DIOGPIO_PINSGROUP_D][10] = GPIO_PIN_10;
	/*PD11*/ GPIOvalue[DIOGPIO_PINSGROUP_D][11] = GPIO_PIN_11; 
 	/*PD12*/ GPIOvalue[DIOGPIO_PINSGROUP_D][12] = GPIO_PIN_12;
	/*PD13*/ GPIOvalue[DIOGPIO_PINSGROUP_D][13] = GPIO_PIN_13;
	/*PD14*/ GPIOvalue[DIOGPIO_PINSGROUP_D][14] = GPIO_PIN_14;
	/*PD15*/ GPIOvalue[DIOGPIO_PINSGROUP_D][15] = GPIO_PIN_15;
  
  /*PE0 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 0] = GPIO_PIN_0;
	/*PE1 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 1] = GPIO_PIN_1;
	/*PE2 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 2] = GPIO_PIN_2;    
	/*PE3 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 3] = GPIO_PIN_3; 
	/*PE4 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 4] = GPIO_PIN_4; 
	/*PE5 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 5] = GPIO_PIN_5; 
	/*PE6 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 6] = GPIO_PIN_6; 
	/*PE7 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 7] = GPIO_PIN_7; 
	/*PE8 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 8] = GPIO_PIN_8; 
	/*PE9 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 9] = GPIO_PIN_9; 
	/*PE10*/ GPIOvalue[DIOGPIO_PINSGROUP_E][10] = GPIO_PIN_10; 
	/*PE11*/ GPIOvalue[DIOGPIO_PINSGROUP_E][11] = GPIO_PIN_11; 
 	/*PE12*/ GPIOvalue[DIOGPIO_PINSGROUP_E][12] = GPIO_PIN_12;
	/*PE13*/ GPIOvalue[DIOGPIO_PINSGROUP_E][13] = GPIO_PIN_13; 
	/*PE14*/ GPIOvalue[DIOGPIO_PINSGROUP_E][14] = GPIO_PIN_14; 
	/*PE15*/ GPIOvalue[DIOGPIO_PINSGROUP_E][15] = GPIO_PIN_15; 
      
  #endif
  
  
  
  #ifdef STM32F446xx
  
  /*PA0 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 0] = GPIO_PIN_0;
	/*PA1 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 1] = GPIO_PIN_1;
	/*PA2 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 2] = DIOGPIO_PINNOTUSE;   // USART TX 
	/*PA3 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 3] = DIOGPIO_PINNOTUSE;   // USART RX  
	/*PA4 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 4] = GPIO_PIN_4;
	/*PA5 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 5] = DIOGPIO_PINNOTUSE;    // SPI 1 SCK
	/*PA6 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 6] = DIOGPIO_PINNOTUSE;    // SPI 1 MISO
	/*PA7 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 7] = DIOGPIO_PINNOTUSE;    // SPI 1 MOSI
	/*PA8 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 8] = GPIO_PIN_8;
	/*PA9 */ GPIOvalue[DIOGPIO_PINSGROUP_A][ 9] = DIOGPIO_PINNOTUSE;   // USART PORT 1 TX 
	/*PA10*/ GPIOvalue[DIOGPIO_PINSGROUP_A][10] = DIOGPIO_PINNOTUSE;   // USART PORT 1 RX 
	/*PA11*/ GPIOvalue[DIOGPIO_PINSGROUP_A][11] = DIOGPIO_PINNOTUSE;   // USB  DM
	/*PA12*/ GPIOvalue[DIOGPIO_PINSGROUP_A][12] = DIOGPIO_PINNOTUSE;   // USB  DP
	/*PA13*/ GPIOvalue[DIOGPIO_PINSGROUP_A][13] = DIOGPIO_PINNOTUSE;   // SWDIO
	/*PA14*/ GPIOvalue[DIOGPIO_PINSGROUP_A][14] = DIOGPIO_PINNOTUSE;   // SWCLK
	/*PA15*/ GPIOvalue[DIOGPIO_PINSGROUP_A][15] = GPIO_PIN_15;
		
	/*PB0 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 0] = GPIO_PIN_0; 
	/*PB1 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 1] = GPIO_PIN_1; 
	/*PB2 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 2] = GPIO_PIN_2; 
	/*PB3 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 3] = GPIO_PIN_3; 
	/*PB4 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 4] = GPIO_PIN_4; 
	/*PB5 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 5] = GPIO_PIN_5; 
	/*PB6 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 6] = DIOGPIO_PINNOTUSE;   // I2C  PORT 1 SDA
	/*PB7 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 7] = DIOGPIO_PINNOTUSE;   // I2C  PORT 1 SCL
	/*PB8 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 8] = GPIO_PIN_8; 
	/*PB9 */ GPIOvalue[DIOGPIO_PINSGROUP_B][ 9] = GPIO_PIN_9; 
	/*PB10*/ GPIOvalue[DIOGPIO_PINSGROUP_B][10] = GPIO_PIN_10;
	/*PB11*/ GPIOvalue[DIOGPIO_PINSGROUP_B][11] = DIOGPIO_PINNOTUSE; 
	/*PB12*/ GPIOvalue[DIOGPIO_PINSGROUP_B][12] = GPIO_PIN_12;
	/*PB13*/ GPIOvalue[DIOGPIO_PINSGROUP_B][13] = GPIO_PIN_13;
	/*PB14*/ GPIOvalue[DIOGPIO_PINSGROUP_B][14] = GPIO_PIN_14;
	/*PB15*/ GPIOvalue[DIOGPIO_PINSGROUP_B][15] = GPIO_PIN_15;
	
	/*PC0 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 0] = GPIO_PIN_0; 
	/*PC1 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 1] = GPIO_PIN_1; 
	/*PC2 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 2] = GPIO_PIN_2; 
	/*PC3 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 3] = GPIO_PIN_3; 
	/*PC4 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 4] = GPIO_PIN_4;
	/*PC5 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 5] = GPIO_PIN_5;
	/*PC6 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 6] = GPIO_PIN_6; 
	/*PC7 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 7] = GPIO_PIN_7; 
	/*PC8 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 8] = GPIO_PIN_8; 
	/*PC9 */ GPIOvalue[DIOGPIO_PINSGROUP_C][ 9] = GPIO_PIN_9; 
	/*PC10*/ GPIOvalue[DIOGPIO_PINSGROUP_C][10] = GPIO_PIN_10;
	/*PC11*/ GPIOvalue[DIOGPIO_PINSGROUP_C][11] = GPIO_PIN_11;
 	/*PC12*/ GPIOvalue[DIOGPIO_PINSGROUP_C][12] = GPIO_PIN_12;
	/*PC13*/ GPIOvalue[DIOGPIO_PINSGROUP_C][13] = GPIO_PIN_13;
	/*PC14*/ GPIOvalue[DIOGPIO_PINSGROUP_C][14] = DIOGPIO_PINNOTUSE;
	/*PC15*/ GPIOvalue[DIOGPIO_PINSGROUP_C][15] = DIOGPIO_PINNOTUSE;
  
  /*PD0 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 0] = DIOGPIO_PINNOTUSE; 
	/*PD1 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 1] = DIOGPIO_PINNOTUSE; 
	/*PD2 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 2] = DIOGPIO_PINNOTUSE; 
	/*PD3 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 3] = DIOGPIO_PINNOTUSE; 
	/*PD4 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 4] = DIOGPIO_PINNOTUSE; 
	/*PD5 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 5] = DIOGPIO_PINNOTUSE; 
	/*PD6 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 6] = DIOGPIO_PINNOTUSE; 
	/*PD7 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 7] = DIOGPIO_PINNOTUSE; 
	/*PD8 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 8] = DIOGPIO_PINNOTUSE; 
	/*PD9 */ GPIOvalue[DIOGPIO_PINSGROUP_D][ 9] = DIOGPIO_PINNOTUSE; 
	/*PD10*/ GPIOvalue[DIOGPIO_PINSGROUP_D][10] = DIOGPIO_PINNOTUSE;
	/*PD11*/ GPIOvalue[DIOGPIO_PINSGROUP_D][11] = DIOGPIO_PINNOTUSE; 
 	/*PD12*/ GPIOvalue[DIOGPIO_PINSGROUP_D][12] = DIOGPIO_PINNOTUSE;
	/*PD13*/ GPIOvalue[DIOGPIO_PINSGROUP_D][13] = DIOGPIO_PINNOTUSE;
	/*PD14*/ GPIOvalue[DIOGPIO_PINSGROUP_D][14] = DIOGPIO_PINNOTUSE;
	/*PD15*/ GPIOvalue[DIOGPIO_PINSGROUP_D][15] = DIOGPIO_PINNOTUSE;
  
  /*PE0 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 0] = DIOGPIO_PINNOTUSE;
	/*PE1 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 1] = DIOGPIO_PINNOTUSE;
	/*PE2 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 2] = DIOGPIO_PINNOTUSE;    
	/*PE3 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 3] = DIOGPIO_PINNOTUSE; 
	/*PE4 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 4] = DIOGPIO_PINNOTUSE; 
	/*PE5 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 5] = DIOGPIO_PINNOTUSE; 
	/*PE6 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 6] = DIOGPIO_PINNOTUSE; 
	/*PE7 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 7] = DIOGPIO_PINNOTUSE; 
	/*PE8 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 8] = DIOGPIO_PINNOTUSE; 
	/*PE9 */ GPIOvalue[DIOGPIO_PINSGROUP_E][ 9] = DIOGPIO_PINNOTUSE; 
	/*PE10*/ GPIOvalue[DIOGPIO_PINSGROUP_E][10] = DIOGPIO_PINNOTUSE; 
	/*PE11*/ GPIOvalue[DIOGPIO_PINSGROUP_E][11] = DIOGPIO_PINNOTUSE; 
 	/*PE12*/ GPIOvalue[DIOGPIO_PINSGROUP_E][12] = DIOGPIO_PINNOTUSE;
	/*PE13*/ GPIOvalue[DIOGPIO_PINSGROUP_E][13] = DIOGPIO_PINNOTUSE; 
	/*PE14*/ GPIOvalue[DIOGPIO_PINSGROUP_E][14] = DIOGPIO_PINNOTUSE; 
	/*PE15*/ GPIOvalue[DIOGPIO_PINSGROUP_E][15] = DIOGPIO_PINNOTUSE; 
  
  #endif
  
  
	return true;
}





/*-------------------------------------------------------------------
//	DIOSTM32FXXXGPIO::SetMode
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 17:20:41
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				isinput : 
//  @param				block : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTM32FXXXGPIO::SetMode(XBYTE pin, bool isinput, DIOGPIO_PINSGROUP block)
{  
  if(GPIOvalue[block][pin] == DIOGPIO_PINNOTUSE) return false;
   	
  GPIO_InitTypeDef  GPIO_inistruct;     
  GPIO_TypeDef*     GPIOblock;
  
	GPIO_inistruct.Pin        = GPIOvalue[block][pin];  
  GPIO_inistruct.Mode       = (isinput)?GPIO_MODE_INPUT:GPIO_MODE_OUTPUT_PP;
  GPIO_inistruct.Pull       = GPIO_NOPULL;
  GPIO_inistruct.Speed      = GPIO_SPEED_FREQ_LOW;
  
  switch(block)
    {
      case DIOGPIO_PINSGROUP_A  : GPIOblock = GPIOA;   break;
      case DIOGPIO_PINSGROUP_B	: GPIOblock = GPIOB;   break;
      case DIOGPIO_PINSGROUP_C	:	GPIOblock = GPIOC;   break;		
      case DIOGPIO_PINSGROUP_D	:	GPIOblock = GPIOD;   break;		
      case DIOGPIO_PINSGROUP_E	:	GPIOblock = GPIOE;   break;
      #ifndef HW_STM32F4XX
      case DIOGPIO_PINSGROUP_F	:	GPIOblock = GPIOF;   break;		
      #endif
    }
    
  HAL_GPIO_Init(GPIOblock, &GPIO_inistruct);
	
	return true;
}



/*-------------------------------------------------------------------
//	DIOSTM32FXXXGPIO::Get
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 17:20:31
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				block : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTM32FXXXGPIO::Get(XBYTE pin, DIOGPIO_PINSGROUP block)
{
  if(GPIOvalue[block][pin] == DIOGPIO_PINNOTUSE) return false;
  
  GPIO_TypeDef* GPIOblock;
  
  switch(block)
    {
      case DIOGPIO_PINSGROUP_A  : GPIOblock = GPIOA;   break;
      case DIOGPIO_PINSGROUP_B	: GPIOblock = GPIOB;   break;
      case DIOGPIO_PINSGROUP_C	:	GPIOblock = GPIOC;   break;
      case DIOGPIO_PINSGROUP_D	:	GPIOblock = GPIOD;   break;		
      case DIOGPIO_PINSGROUP_E	:	GPIOblock = GPIOE;   break;
      #ifndef HW_STM32F4XX
      case DIOGPIO_PINSGROUP_F	:	GPIOblock = GPIOF;   break;		
      #endif
    }
  
  GPIO_PinState state = HAL_GPIO_ReadPin(GPIOblock, GPIOvalue[block][pin]);
  if(state == GPIO_PIN_RESET) return false;
  
  return true;
}



/*-------------------------------------------------------------------
//	DIOSTM32FXXXGPIO::Set
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 17:21:09
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				active : 
//  @param				block : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTM32FXXXGPIO::Set(XBYTE pin,bool active, DIOGPIO_PINSGROUP block)
{
 if(GPIOvalue[block][pin] == DIOGPIO_PINNOTUSE) return false;
  
  GPIO_TypeDef* GPIOblock;
  
  switch(block)
    {
      case DIOGPIO_PINSGROUP_A  : GPIOblock = GPIOA;   break;
      case DIOGPIO_PINSGROUP_B	: GPIOblock = GPIOB;   break;
      case DIOGPIO_PINSGROUP_C	:	GPIOblock = GPIOC;   break;
      case DIOGPIO_PINSGROUP_D	:	GPIOblock = GPIOD;   break;		
      case DIOGPIO_PINSGROUP_E	:	GPIOblock = GPIOE;   break;
      #ifndef HW_STM32F4XX
      case DIOGPIO_PINSGROUP_F	:	GPIOblock = GPIOF;   break;		
      #endif
    }
  
  HAL_GPIO_WritePin(GPIOblock, GPIOvalue[block][pin], active?GPIO_PIN_SET:GPIO_PIN_RESET);
  
  return true;
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXGPIO::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 08:24:53 p.m.
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOSTM32FXXXGPIO::End()
{	
	return false;
}
