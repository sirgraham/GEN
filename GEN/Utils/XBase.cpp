//------------------------------------------------------------------------------------------
//	XBASE.CPP
//	
//	eXtended Base
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 16/03/2006 21:14:26
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <math.h>

#include "XBase.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//	Truncate
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/01/2015 11:30:39
//	
//	@return 			double : 
//
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
double Truncate(double value)
{
	double truncate = 0;
	
	modf(value, &truncate);

	return truncate;
}


/*-------------------------------------------------------------------
//	Fraction
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/01/2015 12:05:50
//	
//	@return 			double : 
//
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
double Fraction(double value)
{
	double truncate = 0;

	return modf(value, &truncate);
}




//-------------------------------------------------------------------
//  SwapWORD
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2006 21:14:35
//	
//	@return				XWORD : 
//	@param				data : 
*/
//-------------------------------------------------------------------
XWORD SwapWORD(XWORD data)
{
	const XWORD hi = (XWORD)((data&0xFF00)>>8);
	const XWORD lo = (XWORD)((data&0x00FF)<<8);

	return (XWORD)(hi|lo);
}


//-------------------------------------------------------------------
//  SwapDWORD
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2006 21:14:38
//	
//	@return				XDWORD : 
//	@param				data : 
*/
//-------------------------------------------------------------------
XDWORD SwapDWORD(XDWORD data)
{
	const XDWORD hi1 = (data&0xFF000000)>>24;
	const XDWORD lo1 = (data&0x00FF0000)>>8;
	const XDWORD hi2 = (data&0x0000FF00)<<8;
	const XDWORD lo2 = (data&0x000000FF)<<24;

	return (hi1|lo1|hi2|lo2);
}



/*-------------------------------------------------------------------
//  SwapQWORD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/06/2013 19:33:39
//	
//	@return 			XDWORD : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
XQWORD SwapQWORD(XQWORD data)
{
	const XQWORD hi1 = (data&0xFF00000000000000ULL) >> (24 + 32);
	const XQWORD lo1 = (data&0x00FF000000000000ULL) >> ( 8 + 32);	
	const XQWORD hi2 = (data&0x0000FF0000000000ULL) >> (     24);
	const XQWORD lo2 = (data&0x000000FF00000000ULL) >> (      8);

	const XQWORD hi3 = (data&0x00000000FF000000ULL) << (      8);
	const XQWORD lo3 = (data&0x0000000000FF0000ULL) << (     24);
	const XQWORD hi4 = (data&0x000000000000FF00ULL) << ( 8 + 32);
	const XQWORD lo4 = (data&0x00000000000000FFULL) << (24 + 32);

	return (hi1|lo1|hi2|lo2|hi3|lo3|hi4|lo4);
}




/*-------------------------------------------------------------------
//	RoundFloat
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/11/2014 15:55:40
//	
//	@return 			float : 
//
//  @param				x : 
*/
/*-----------------------------------------------------------------*/
float RoundFloat(float x)
{
   return float(x >= 0.0f ? floor(x + 0.5f) : ceil(x - 0.5f));
}





/*-------------------------------------------------------------------
//	DWORDToBCD
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			02/01/2015 13:34:44
//	
//	@return 			XQWORD : 
//
//  @param				usi : 
*/
/*-----------------------------------------------------------------*/
XQWORD DWORDToBCD(XDWORD dword)    
{
	XQWORD shift	= 16;  
  XQWORD result = (dword % 10);

  while((dword = (dword/10)))
    {
      result += (dword % 10) * shift;
      shift *= 16; // weirdly, it's not possible to left shift more than 32 bits
    }
    
	return result;
}



/*-------------------------------------------------------------------
//	BCDToDWORD
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			02/01/2015 13:35:37
//	
//	@return 			XDWORD : 
//
//  @param				bcd : 
*/
/*-----------------------------------------------------------------*/
XDWORD BCDToDWORD(XQWORD bcd)  
{
  XQWORD mask = 0x000f;
  XQWORD pwr	= 1;
  XQWORD i		= (bcd & mask);

  while((bcd = (bcd >> 4)))
		{
	    pwr *= 10;
      i += (bcd & mask) * pwr;
    }
  
	return (XDWORD)i;
}


