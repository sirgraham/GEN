//------------------------------------------------------------------------------------------
//	STM32FXXXINI.CPP
//	
//	STM32Fxxx INI for STM32F0xx 
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 05/10/2005 16:00:46
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>

#include "stm32f0xx_hal.h"

#include "STM32FxxxIni.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

TIM_HandleTypeDef  htim3;
	
//---- INTERNAL PROTOTYPES -----------------------------------------------------------------

void SystemClock_Config		(void);
void MX_NVIC_Init         (void);
void MX_GPIO_Init					(void);
void MX_TIMER3_Init       (void);
void Error_Handler				(void);

//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//	SystemClock_Config
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/08/2016 17:08:45
//	
*/
/*-----------------------------------------------------------------*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef 				RCC_OscInitStruct;
  RCC_ClkInitTypeDef 				RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef 	PeriphClkInit;


  RCC_OscInitStruct.OscillatorType 			= RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState 						= RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State 					= RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState 				= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 			= RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL 					= RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV 					= RCC_PREDIV_DIV2;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  	{
    	Error_Handler();
  	}


  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;  
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  	{
    	Error_Handler();
  	}


  PeriphClkInit.PeriphClockSelection 	= RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection 	= RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection 		= RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.UsbClockSelection 		= RCC_USBCLKSOURCE_HSI48;  
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  	{
    	Error_Handler();
  	}

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



/*-------------------------------------------------------------------
//	MX_NVIC_Init
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/08/2016 17:08:45
//	
*/
/*-----------------------------------------------------------------*/
void MX_NVIC_Init(void)
{
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}




/*-------------------------------------------------------------------
//	MX_GPIO_Init
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/08/2016 17:08:53
//	
//	@return 			static : 
//
*/
/*-----------------------------------------------------------------*/
/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA2   ------> TSC_G1_IO3
     PA3   ------> TSC_G1_IO4
     PA6   ------> TSC_G2_IO3
     PA7   ------> TSC_G2_IO4
     PB0   ------> TSC_G3_IO2
     PB1   ------> TSC_G3_IO3
     PB10   ------> I2C2_SCL
     PB11   ------> I2C2_SDA
     PB13   ------> SPI2_SCK
     PB14   ------> SPI2_MISO
     PB15   ------> SPI2_MOSI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, NCS_MEMS_SPI_Pin | EXT_RESET_Pin | LD3_Pin | LD6_Pin | LD4_Pin | LD5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : NCS_MEMS_SPI_Pin EXT_RESET_Pin LD3_Pin LD6_Pin LD4_Pin LD5_Pin */
  GPIO_InitStruct.Pin 			= NCS_MEMS_SPI_Pin|EXT_RESET_Pin|LD3_Pin|LD6_Pin|LD4_Pin|LD5_Pin;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : MEMS_INT1_Pin MEMS_INT2_Pin */
  GPIO_InitStruct.Pin 			= MEMS_INT1_Pin|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode			= GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin 			= B1_Pin;
  GPIO_InitStruct.Mode 			= GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA3 PA6 PA7 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode 			= GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TSC;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode 			= GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed			= GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TSC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode 			= GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull 			= GPIO_PULLUP;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_I2C2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB13 PB14 PB15 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode 			= GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}





/*-------------------------------------------------------------------
//	MX_TIM3_Init
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/08/2016 10:45:02
//	
//	@return 			static : 
//
*/
/*-----------------------------------------------------------------*/
void MX_TIMER3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 48000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 50; //499;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}


/*-------------------------------------------------------------------
//	Error_Handler
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/08/2016 17:08:59
//	
*/
/*-----------------------------------------------------------------*/
void Error_Handler(void)
{
  while(1) 
		{
    
		}
}
