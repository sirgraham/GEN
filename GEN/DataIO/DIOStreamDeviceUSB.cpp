//------------------------------------------------------------------------------------------
//	DIOSTREAMDEVICEUSB.CPP
//	
//	Data IO Stream Device Bluetooth class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XString.h"
#include "XDebug.h"
#include "XConsole.h"

#include "DIOStreamDeviceUSB.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  DIOSTREAMDEVICEUSB:: DIOSTREAMDEVICEUSB
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/01/2007 19:14:31
//	
//	@return		
//	*/
//-------------------------------------------------------------------
 DIOSTREAMDEVICEUSB::DIOSTREAMDEVICEUSB()
{
	Clean();
}
		

//-------------------------------------------------------------------
//  DIOSTREAMDEVICEUSB::~ DIOSTREAMDEVICEUSB
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/01/2007 19:14:20
//	
//	@return				
//	*/
//-------------------------------------------------------------------
 DIOSTREAMDEVICEUSB::~DIOSTREAMDEVICEUSB()
{
	Clean();
}






/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEUSB::DebugPrintInfo
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2013 11:28:09
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICEUSB::DebugPrintInfo(XCONSOLE* xconsole)
{
	DIOSTREAMDEVICE::DebugPrintInfo(xconsole);

	XSTRING line;
	
	line.Format(__L("Location      : %s")		,	location.Get());				  DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());		
	line.Format(__L("Vendor ID     : %08X")	,	vendorID);							  DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());		
	line.Format(__L("Product ID    : %08X")	, productID);								DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());		
	line.Format(__L("Class         : %02x")	, classdev);								DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());		
	line.Format(__L("Serial        : %s")		, serialstring.Get());			DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());		

	return true;
}



