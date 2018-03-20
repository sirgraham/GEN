//------------------------------------------------------------------------------------------
//	DIOSTREAMUSB.CPP
//	
//	Data IO Stream USB class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "DIOStreamUSB.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	



/*-------------------------------------------------------------------
//  DIOSTREAMUSB::DIOSTREAMUSB
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:44:24
//	
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUSB::DIOSTREAMUSB() : DIOSTREAM()
{

}


//-------------------------------------------------------------------
//  DIOSTREAMUSB::~DIOSTREAMUSB
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:19:33
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMUSB::~DIOSTREAMUSB()
{ 

}


//-------------------------------------------------------------------
//  DIOSTREAMUSB::GetConfig
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/09/2004 14:33:56
//	
//	@return				DIOSTREAMCONFIG* : 
//	*/
//-------------------------------------------------------------------
DIOSTREAMCONFIG* DIOSTREAMUSB::GetConfig()
{
	return (DIOSTREAMCONFIG*)config;
}



//-------------------------------------------------------------------
//  DIOSTREAMUSB::SetConfig
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
//  @param				config :
*/
//-------------------------------------------------------------------
bool DIOSTREAMUSB::SetConfig(DIOSTREAMCONFIG* config)
{
	if(!config) return false;

	this->config = (DIOSTREAMUSBCONFIG*)config;

	return true;
}



//-------------------------------------------------------------------
//  DIOSTREAMUSB::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:19:50
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void DIOSTREAMUSB::Clean()
{
	config = NULL;
}

