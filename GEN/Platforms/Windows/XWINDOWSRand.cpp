//------------------------------------------------------------------------------------------
//	XWINDOWSRAND.CPP
//	
//	WINDOWS rand class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <time.h>

#include "XWINDOWSRand.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------

//-------------------------------------------------------------------
//  XWINDOWSRAND::XWINDOWSRAND
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XWINDOWSRAND::XWINDOWSRAND()
{
	Ini();
}


//-------------------------------------------------------------------
//  XWINDOWSRAND::~XWINDOWSRAND
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XWINDOWSRAND::~XWINDOWSRAND()
{

}


/*-------------------------------------------------------------------
//  XWINDOWSRAND::Ini
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
bool XWINDOWSRAND::Ini()
{
	srand((unsigned)time(NULL));
	
	return true;
}


//-------------------------------------------------------------------
//  XWINDOWSRAND::MaxElements
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			int :
//	@param				max :
//	*/
//-------------------------------------------------------------------
int XWINDOWSRAND::MaxElements(int max)
{
	return GetRandom(0,max-1);
}


//-------------------------------------------------------------------
//  XWINDOWSRAND::Max
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			int :
//	@param				max :
//	*/
//-------------------------------------------------------------------
int XWINDOWSRAND::Max(int max)
{
	return GetRandom(0,max);
}


//-------------------------------------------------------------------
//  XWINDOWSRAND::Between
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			int :
//	@param				min :
//	@param				max :
//	*/
//-------------------------------------------------------------------
int XWINDOWSRAND::Between(int min,int max)
{
	return GetRandom(min,max);
}



