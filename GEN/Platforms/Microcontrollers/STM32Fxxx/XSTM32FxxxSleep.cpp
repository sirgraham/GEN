//------------------------------------------------------------------------------------------
//	XSTM32FXXXSLEEP.CPP
//	
//	STM32Fxxx Sleep class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSTM32Fxxx_HAL.h"

#include "XSTM32FxxxSleep.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XSTM32FXXXSLEEP::XSTM32FXXXSLEEP
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XSTM32FXXXSLEEP::XSTM32FXXXSLEEP()
{
	
}



/*-------------------------------------------------------------------
//	XSTM32FXXXSLEEP::~XSTM32FXXXSLEEP
*/	
/**	
//	
//	 Class Destructor XSTM32FXXXSLEEP
//	
//	@author				Abraham J. Velez
//	@version			03/06/2014 6:56:26
//	
*/
/*-----------------------------------------------------------------*/
XSTM32FXXXSLEEP::~XSTM32FXXXSLEEP()
{

}




/*-------------------------------------------------------------------
//  XSTM32FXXXSLEEP::Seconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 17:10:01
//	
//	@return 			void : 
//	@param				seconds : 
*/
/*-----------------------------------------------------------------*/
void XSTM32FXXXSLEEP::Seconds(int seconds)
{
	for(int c=0; c<seconds; c++)
    {
    	HAL_Delay(1000); 
    }
}



 
/*-------------------------------------------------------------------
//  XSTM32FXXXSLEEP::MilliSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 17:10:18
//	
//	@return 			void : 
//	@param				milliseconds : 
*/
/*-----------------------------------------------------------------*/
void XSTM32FXXXSLEEP::MilliSeconds(int milliseconds)
{  
	HAL_Delay(milliseconds);  
}




/*-------------------------------------------------------------------
//  XSTM32FXXXSLEEP::MicroSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 17:10:54
//	
//	@return 			void : 
//	@param				microseconds : 
*/
/*-----------------------------------------------------------------*/
void XSTM32FXXXSLEEP::MicroSeconds(int microseconds)
{

}



/*-------------------------------------------------------------------
//  XSTM32FXXXSLEEP::SleepNanoSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/12/2013 18:32:39
//	
//	@return 			void : 
//	@param				nanoseconds : 
*/
/*-----------------------------------------------------------------*/
void XSTM32FXXXSLEEP::NanoSeconds(int nanoseconds)
{										
	
}




