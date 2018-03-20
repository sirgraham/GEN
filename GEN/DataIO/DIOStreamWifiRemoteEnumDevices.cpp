//------------------------------------------------------------------------------------------
//	DIOSTREAMWIFIREMOTEENUMDEVICES.CPP
//	
//	Data IO Stream Wifi REMOTE Enum Devices class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamWifiRemoteEnumDevices.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMWIFIREMOTEENUMDEVICES::DIOSTREAMWIFIREMOTEENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 18:37:55
//	
*/
/*-----------------------------------------------------------------*/
DIOSTREAMWIFIREMOTEENUMDEVICES::DIOSTREAMWIFIREMOTEENUMDEVICES() : DIOSTREAMENUMDEVICES()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMWIFIREMOTEENUMDEVICES::~DIOSTREAMWIFIREMOTEENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 18:38:48
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOSTREAMWIFIREMOTEENUMDEVICES::~DIOSTREAMWIFIREMOTEENUMDEVICES()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMWIFIREMOTEENUMDEVICES::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 18:42:16
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOSTREAMWIFIREMOTEENUMDEVICES::Clean()
{
	type = DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE;
}


