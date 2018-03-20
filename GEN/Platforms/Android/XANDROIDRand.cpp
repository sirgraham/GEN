//------------------------------------------------------------------------------------------
//	XANDROIDTIME.CPP
//	
//	Time Functions
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <time.h>

#include "xandroidrand.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  XANDROIDRAND::Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2009 07:14:11 p.m.
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XANDROIDRAND::Ini()
{
	srand((unsigned)time(NULL));
	
	return true;
}
