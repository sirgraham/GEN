//------------------------------------------------------------------------------------------
//	XLINUXRAND.CPP
//	
//	LINUX rand class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <time.h>

#include "XLINUXRand.h"

#include "XMemory.h"
	

//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  XLINUXRAND::XLINUXRAND
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XLINUXRAND::XLINUXRAND()
{
	Ini();
}


//-------------------------------------------------------------------
//  XLINUXRAND::~XLINUXRAND
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XLINUXRAND::~XLINUXRAND()
{

}


/*-------------------------------------------------------------------
//  XLINUXRAND::Ini
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
bool XLINUXRAND::Ini()
{
	srand((unsigned)time(NULL));
	
	return true;
}


//-------------------------------------------------------------------
//  XLINUXRAND::MaxElements
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
int XLINUXRAND::MaxElements(int max)
{
	return GetRandom(0,max-1);;
}


//-------------------------------------------------------------------
//  XLINUXRAND::Max
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
int XLINUXRAND::Max(int max)
{
	return GetRandom(0,max);
}


//-------------------------------------------------------------------
//  XLINUXRAND::Between
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
int XLINUXRAND::Between(int min,int max)
{
	return GetRandom(min,max);
}



