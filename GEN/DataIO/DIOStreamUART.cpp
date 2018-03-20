//------------------------------------------------------------------------------------------
//	DIOSTREAMUART.CPP
//	
//	Data IO Stream UART class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "DIOStreamUART.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	



/*-------------------------------------------------------------------
//  DIOSTREAMUART::DIOSTREAMUART
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:44:24
//	
//	@return 		

 
 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUART::DIOSTREAMUART() : DIOSTREAM()
{
	Clean();

	xtimer = xfactory->CreateTimer();	
}


//-------------------------------------------------------------------
//  DIOSTREAMUART::~DIOSTREAMUART
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:19:33
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMUART::~DIOSTREAMUART()
{ 
	if(xtimer)	xfactory->DeleteTimer(xtimer);

	Clean();
}


//-------------------------------------------------------------------
//  DIOSTREAMUART::GetConfig
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/09/2004 14:33:56
//	
//	@return				DIOSTREAMCONFIG* : 
//	*/
//-------------------------------------------------------------------
DIOSTREAMCONFIG* DIOSTREAMUART::GetConfig()
{
	return (DIOSTREAMCONFIG*)config;
}



//-------------------------------------------------------------------
//  DIOSTREAMUART::SetConfig
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
bool DIOSTREAMUART::SetConfig(DIOSTREAMCONFIG* config)
{
	if(!config) return false;

	this->config = (DIOSTREAMUARTCONFIG*)config;

	return true;
}



//-------------------------------------------------------------------
//  DIOSTREAMUART::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:19:50
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void DIOSTREAMUART::Clean()
{	
	config							= NULL;
	xtimer							= NULL;	
}

