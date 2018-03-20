//------------------------------------------------------------------------------------------
//	DIOSTREAMUARTLOCALENUMDEVICES.CPP
//	
//	Data IO Stream UART Local Enum Device class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "DIOStreamUARTLocalEnumDevices.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	


/*-------------------------------------------------------------------
//  DIOSTREAMUARTLOCALENUMDEVICES::DIOSTREAMUARTLOCALENUMDEVICES
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
DIOSTREAMUARTLOCALENUMDEVICES::DIOSTREAMUARTLOCALENUMDEVICES() : DIOSTREAMENUMDEVICES()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMUARTLOCALENUMDEVICES::~DIOSTREAMUARTLOCALENUMDEVICES
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
DIOSTREAMUARTLOCALENUMDEVICES::~DIOSTREAMUARTLOCALENUMDEVICES()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMUARTLOCALENUMDEVICES::Clean
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
void DIOSTREAMUARTLOCALENUMDEVICES::Clean()
{
	type			  = DIOSTREAMENUMTYPE_UART_LOCAL;
	issearching = false;
}

