//------------------------------------------------------------------------------------------
//	DIOSTREAMTCPIP.CPP
//	
//	Data IO Stream TCP/IP class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOFactory.h"

#include "DIOStreamTCPIP.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMTCPIP::DIOSTREAMTCPIP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:46:00
//	
//	@return 			

 
//  @param				publisher : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMTCPIP::DIOSTREAMTCPIP() : DIOSTREAM()
{
	Clean();	
}



//-------------------------------------------------------------------
//  DIOSTREAMTCPIP::~DIOSTREAMTCPIP
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:15:51
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMTCPIP::~DIOSTREAMTCPIP()
{ 	
	Clean();
}



//-------------------------------------------------------------------
//  DIOSTREAMTCPIP::GetConfig
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/09/2004 14:32:49
//	
//	@return				DIOSTREAMCONFIG* : 
//	*/
//-------------------------------------------------------------------
DIOSTREAMCONFIG* DIOSTREAMTCPIP::GetConfig()
{
	return (DIOSTREAMCONFIG*)config;
}



//-------------------------------------------------------------------
//  DIOSTREAMTCPIP::SetConfig
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
bool DIOSTREAMTCPIP::SetConfig(DIOSTREAMCONFIG* config)
{	
	if(!config) return false;
	this->config = (DIOSTREAMTCPIPCONFIG  *)config;
	
	return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMTCPIP::GetClientIP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2013 9:59:22
//	
//	@return 			DIOIP* : 
//	*/
/*-----------------------------------------------------------------*/
DIOIP* DIOSTREAMTCPIP::GetClientIP()
{
	return &clientIP;
}



/*-------------------------------------------------------------------
//  DIOSTREAMTCPIP::GetEnumServers
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2013 11:28:21
//	
//	@return 			DIOSTREAMENUMSERVERS* : 
//	*/
/*-----------------------------------------------------------------*/
DIOSTREAMENUMSERVERS* DIOSTREAMTCPIP::GetEnumServers()
{
	return enumservers;
}



/*-------------------------------------------------------------------
//  DIOSTREAMTCPIP::SetEnumServers
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2013 11:28:46
//	
//	@return 			bool : 
//	@param				enumservers : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMTCPIP::SetEnumServers(DIOSTREAMENUMSERVERS* enumservers)
{
	this->enumservers = enumservers;

	return true;
}



//-------------------------------------------------------------------
//  DIOSTREAMTCPIP::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:16:00
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void DIOSTREAMTCPIP::Clean()
{
	config			= NULL;
	enumservers	= NULL;
}



