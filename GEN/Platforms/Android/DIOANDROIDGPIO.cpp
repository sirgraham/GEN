/*------------------------------------------------------------------------------------------
//	DIOANDROIDGPIO.CPP
//	
//	ANDROID DIO port data class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 25/07/2009 07:14:17 p.m.
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>	
#include <stdint.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "DIOANDROIDGPIO.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOANDROIDGPIO::DIOANDROIDGPIO
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:18:20 p.m.
//	
//	@return				
*/
/*-----------------------------------------------------------------*/
DIOANDROIDGPIO::DIOANDROIDGPIO() : DIOGPIO()
{
	Clean();
}


/*-------------------------------------------------------------------
//  DIOANDROIDGPIO::~DIOANDROIDGPIO
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:18:44 p.m.
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOANDROIDGPIO::~DIOANDROIDGPIO()
{
	Clean();
}



/*-------------------------------------------------------------------
//	DIOANDROIDGPIO::Ini
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/10/2015 14:25:06
//	
//	@return 			bool : 
//
//  @param				hardwaretype : 
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDGPIO::Ini(XPATH* xpath)
{
	
  
	

	return false;
}




/*-------------------------------------------------------------------
//  DIOANDROIDGPIO::SetMode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/06/2012 17:08:35
//	
//	@return 			bool : 
//	@param				bit : 
//  @param				isinput : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDGPIO::SetMode(XBYTE pin,bool isinput)
{
  	
	return false;
}



/*-------------------------------------------------------------------
//  DIOANDROIDGPIO::Get
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/06/2012 23:49:52
//	
//	@return 			bool : 
//	@param				pin : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDGPIO::Get(XBYTE pin)
{
  
  return false;
}



/*-------------------------------------------------------------------
//  DIOANDROIDGPIO::Set
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/06/2012 17:43:29
//	
//	@return 			bool : 
//	@param				pin : 
//  @param				active : 
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDGPIO::Set(XBYTE pin,bool active)
{
  
  return false;
}



/*-------------------------------------------------------------------
//  DIOANDROIDGPIO::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 08:24:53 p.m.
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDGPIO::End()
{	
  
	return false;
}



/*-------------------------------------------------------------------
//  DIOANDROIDGPIO::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:19:12 p.m.
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOANDROIDGPIO::Clean()
{

  

}


