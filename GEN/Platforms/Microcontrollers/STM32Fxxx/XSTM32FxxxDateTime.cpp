//------------------------------------------------------------------------------------------
//	XSTM32FXXXDATETIME.CPP
//	
//	STM32Fxxx date time class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSTM32FxxxDateTime.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  XSTM32FXXXDATETIME::XSTM32FXXXDATETIME
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2002 9:36:29
//	
//	@return 			: 
//	*/
//-------------------------------------------------------------------
XSTM32FXXXDATETIME::XSTM32FXXXDATETIME()
{
	year					= 0;
	month					= 0;
	day						= 0;
	hours					= 0;
	minutes				= 0;
	seconds				= 0;
	milliseconds	= 0;

	//GetActualDateTime(this);
}





/*-------------------------------------------------------------------
//  XSTM32FXXXDATETIME::~XSTM32FXXXDATETIME
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/06/2010 10:05:29
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XSTM32FXXXDATETIME::~XSTM32FXXXDATETIME()
{


}









//-------------------------------------------------------------------
//  XSTM32FXXXDATETIME::Read
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/04/2004 10:10:58
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XSTM32FXXXDATETIME::Read()
{
	//GetActualDateTime(this);

	return true;
}




//-------------------------------------------------------------------
//  XSTM32FXXXDATETIME::Write
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/06/2007 13:31:22
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XSTM32FXXXDATETIME::Write()
{
	//SetActualDateTime(this);

	return true;
}





/*-------------------------------------------------------------------
//  XSTM32FXXXDATETIME::GetDifferenceGMT
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/02/2013 19:50:40
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int XSTM32FXXXDATETIME::GetDifferenceGMT()
{ 		
  return (int)0;	
}




/*-------------------------------------------------------------------
//  XSTM32FXXXDATETIME::IsDayLigthSavingTime
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/02/2013 20:31:47
//	
//	@return 			bool : 
//	@param				bias : 
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXDATETIME::IsDayLigthSavingTime(int* bias)
{
	return false;
}



