//------------------------------------------------------------------------------------------
//	XSTM32FXXXTIMER.CPP
//	
//	STM32Fxxx timer class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSTM32Fxxx_HAL.h"

#include "XSTM32FxxxTimer.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XSTM32FXXXTIMER::XSTM32FXXXTIMER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XSTM32FXXXTIMER::XSTM32FXXXTIMER()
{
	Reset();
}



/*-------------------------------------------------------------------
//	XSTM32FXXXTIMER::~XSTM32FXXXTIMER
*/	
/**	
//	
//	 Class Destructor XSTM32FXXXTIMER
//	
//	@author				Abraham J. Velez
//	@version			03/06/2014 6:56:26
//	
*/
/*-----------------------------------------------------------------*/
XSTM32FXXXTIMER::~XSTM32FXXXTIMER()
{

}



/*-------------------------------------------------------------------
//  XSTM32FXXXTIMER::GetMicroSecondsTickCounter
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 17:13:33
//	
//	@return 			XQWORD : 
//	*/
/*-----------------------------------------------------------------*/
XQWORD XSTM32FXXXTIMER::GetMicroSecondsTickCounter()
{	
	XQWORD elapsedtime = HAL_GetTick();

	elapsedtime *= 1000;  // Pass to MicroSeconds
  
  return elapsedtime;
}

	

