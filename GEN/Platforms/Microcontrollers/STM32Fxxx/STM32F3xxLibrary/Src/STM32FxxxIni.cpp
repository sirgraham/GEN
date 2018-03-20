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


#include "XSTM32Fxxx_HAL.h"

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
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_ClkInitTypeDef        RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInit;

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL12;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

  RCC_ClkInitStruct.ClockType           = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource        = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider       = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider      = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider      = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
      Error_Handler();
    }

  PeriphClkInit.PeriphClockSelection    = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection    = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Uart4ClockSelection     = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection      = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection       = RCC_USBCLKSOURCE_PLL;
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
//	 Configure pins as 
//        * Analog 
//        * Input 
//        * Output
//        * EVENT_OUT
//        * EXTI
//     PA5   ------> SPI1_SCK
//     PA6   ------> SPI1_MISO
//     PA7   ------> SPI1_MOSI
//	
//	
//	@author				Abraham J. Velez
//	@version			11/08/2016 17:08:53
//	
//	@return 			static : 
//
*/
/*-----------------------------------------------------------------*/
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT1_Pin 
                           MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT1_Pin 
                          |MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin 
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin 
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin 
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin 
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 SPI1_MISO_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|SPI1_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin 
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin 
                          |LD6_Pin, GPIO_PIN_RESET);

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

  htim3.Instance            = TIM3;
  htim3.Init.Prescaler      = 4800;
  htim3.Init.CounterMode    = TIM_COUNTERMODE_UP;
  htim3.Init.Period         = 50; //499;
  htim3.Init.ClockDivision  = TIM_CLOCKDIVISION_DIV1;
  if(HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
      Error_Handler();
    }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if(HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
      Error_Handler();
    }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if(HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
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
