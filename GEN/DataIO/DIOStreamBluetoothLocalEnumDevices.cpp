//------------------------------------------------------------------------------------------
//	DIOSTREAMBLUETOOTHLOCALENUMDEVICES.CPP
//	
//	Data IO Stream Bluetooth Local Enum Devices class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOSTREAMBluetoothLocalEnumDevices.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHLOCALENUMDEVICES::DIOSTREAMBLUETOOTHLOCALENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 18:37:55
//	
//	@return 			void : 

//  @param				publisher : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMBLUETOOTHLOCALENUMDEVICES::DIOSTREAMBLUETOOTHLOCALENUMDEVICES() : DIOSTREAMENUMDEVICES()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHLOCALENUMDEVICES::~DIOSTREAMBLUETOOTHLOCALENUMDEVICES
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
DIOSTREAMBLUETOOTHLOCALENUMDEVICES::~DIOSTREAMBLUETOOTHLOCALENUMDEVICES()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTHLOCALENUMDEVICES::Clean
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
void DIOSTREAMBLUETOOTHLOCALENUMDEVICES::Clean()
{
	type			  = DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL;
	issearching = false;
}


