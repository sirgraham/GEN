//------------------------------------------------------------------------------------------
//	DIOSTREAMBLUETOOTH.CPP
//	
//	Stream IO Bluetooth (Class)
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

#include "DIOStreamBluetoothConfig.h"

#include "DIOStreamBluetooth.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH::DIOSTREAMBLUETOOTH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:53:48
//	
//	@return 		

 
 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMBLUETOOTH::DIOSTREAMBLUETOOTH() : DIOSTREAM()
{
	Clean();
}


//-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH::~DIOSTREAMBLUETOOTH
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:15:51
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMBLUETOOTH::~DIOSTREAMBLUETOOTH()
{ 
	Clean();
}


//-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH::GetConfig
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/09/2004 14:32:49
//	
//	@return				DIOSTREAMCONFIG* : 
//	*/
//-------------------------------------------------------------------
DIOSTREAMCONFIG* DIOSTREAMBLUETOOTH::GetConfig()
{
	return (DIOSTREAMCONFIG*)config;
}



//-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH::SetConfig
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
bool DIOSTREAMBLUETOOTH::SetConfig(DIOSTREAMCONFIG* config)
{	
	if(!config) return false;

	this->config = (DIOSTREAMBLUETOOTHCONFIG*)config;
	
	return true;
}





/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH::IsRefusedConnexion
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/05/2013 10:53:32
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMBLUETOOTH::IsRefusedConnexion()
{
	return isrefusedconnexion;
}




/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH::SetIsRefusedConnexion
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/05/2013 10:53:48
//	
//	@return 			bool : 
//	@param				isrefused : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMBLUETOOTH::SetIsRefusedConnexion(bool isrefused)
{
	this->isrefusedconnexion = isrefused;

	return true;
}




//-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:16:00
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void	DIOSTREAMBLUETOOTH::Clean()
{	
	config							= NULL;
	isrefusedconnexion  = false;
}





