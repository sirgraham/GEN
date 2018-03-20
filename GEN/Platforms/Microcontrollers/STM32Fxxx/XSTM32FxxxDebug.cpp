
//------------------------------------------------------------------------------------------
//	XSTM32FXXXDEBUG.CPP
//	
//	STM32Fxxx debug class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 05/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------

#ifdef XDEBUG	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"	
#include "XBuffer.h"	
#include "XSleep.h"	

#include "XSTM32FxxxDateTime.h"

#include "XSTM32FxxxDebug.h"

	
//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XSTM32FXXXDEBUGCTRL::XSTM32FXXXDEBUGCTRL
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:03:05
//	
//	@return 			
*/
//-------------------------------------------------------------------
XSTM32FXXXDEBUGCTRL::XSTM32FXXXDEBUGCTRL() 
{	
	Clean();
	
	XDebugCtrl = this;	 	
}				



//-------------------------------------------------------------------
//  XDEBUGCTRL::~XDEBUGCTRL
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:03:22
//	
//	@return 			
*/
//-------------------------------------------------------------------
XSTM32FXXXDEBUGCTRL::~XSTM32FXXXDEBUGCTRL()
{		  
	Clean();

	XDebugCtrl =  NULL;
}




/*-------------------------------------------------------------------
//	XSTM32FXXXDEBUGCTRL::PrintSpecial
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/02/2017 13:52:02
//	
//  @param				target : 
//  @param				level : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void XSTM32FXXXDEBUGCTRL::PrintSpecial(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{	 
  /*
  XBUFFER	             xbufferpacket;     
  XSTM32FXXXDATETIME   xdatetime;  														  		
   
  SetDebugToXBuffer(0xFFFFFFFF, 0xFFFFFFFF, level, sequence, &xdatetime, string, xbufferpacket);  
   
	HAL_UART_Transmit(&huart, (uint8_t*)xbufferpacket.Get(), (uint16_t)xbufferpacket.GetSize(), 10);     
  */
  
 
  XSTM32FXXXDATETIME   xdatetime;  														  		
  static bool          reenter = false;
   
  if(reenter) 
    { 
      XBUFFER	             xbufferpackettmp;
      static XBUFFER       xbuffercache; 
      XSTM32FXXXDATETIME   xdatetime;  														  		
      
      SetDebugToXBuffer(0xFFFFFFFF, 0xFFFFFFFF, level, sequence, &xdatetime, string, xbufferpackettmp);        
      xbuffercache.Add(xbufferpackettmp);             
    }
   else
    {
      reenter = true;
        
      if(xbuffercache.GetSize())
        {
          HAL_UART_Transmit(&huart, (uint8_t*)xbuffercache.Get(), (uint16_t)xbuffercache.GetSize(), 1000);     
          xbuffercache.Delete();
        } 
  
      XBUFFER	             xbufferpacket;        
      XSTM32FXXXDATETIME   xdatetime;  														  		

      SetDebugToXBuffer(0xFFFFFFFF, 0xFFFFFFFF, level, sequence, &xdatetime, string, xbufferpacket);    
      HAL_UART_Transmit(&huart, (uint8_t*)xbufferpacket.Get(), (uint16_t)xbufferpacket.GetSize(), 1000);     
  
      reenter = false;    
    }  
}



/*-------------------------------------------------------------------
//	XSTM32FXXXDEBUGCTRL::PrintFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/02/2017 13:52:08
//	
//  @param				target : 
//  @param				level : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void XSTM32FXXXDEBUGCTRL::PrintFile(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)				
{
	XSTRING xstring;

	xstring  = string;
  xstring += __L("\n\r");

	XSTRING_CREATEOEM(xstring, charstr);
	
	HAL_UART_Transmit(&huart, (uint8_t*)charstr, (uint16_t)xstring.GetSize(), 10);     
	
	XSTRING_DELETEOEM(charstr);
}




/*-------------------------------------------------------------------
//	XSTM32FXXXDEBUGCTRL::IniUARTSpecial
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			26/08/2016 10:37:23
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXDEBUGCTRL::IniUARTSpecial()
{
  #ifdef HW_STM32F0XX
	huart.Instance										= USART4;
  #endif  
  #ifdef HW_STM32F3XX
  huart.Instance										= UART4;
  #endif
  #ifdef HW_STM32F4XX
  huart.Instance										= USART2;
  #endif
  huart.Init.BaudRate								= 115200;
  huart.Init.WordLength							= UART_WORDLENGTH_8B;
  huart.Init.StopBits								= UART_STOPBITS_1;
  huart.Init.Parity									= UART_PARITY_NONE;
  huart.Init.Mode										= UART_MODE_TX;
  huart.Init.HwFlowCtl							= UART_HWCONTROL_NONE;
  huart.Init.OverSampling						= UART_OVERSAMPLING_16;
    
  
  #if defined(HW_STM32F0XX) || defined(HW_STM32F3XX)
  huart.Init.OneBitSampling					= UART_ONE_BIT_SAMPLE_DISABLE;
  huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  #endif

  if(HAL_UART_Init(&huart) == HAL_OK)
		{
      return true;
		}

	return false;
}


/*-------------------------------------------------------------------
//  XSTM32FXXXDEBUGCTRL::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/07/2012 12:42:37
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void XSTM32FXXXDEBUGCTRL::Clean()
{
 
}


#endif
