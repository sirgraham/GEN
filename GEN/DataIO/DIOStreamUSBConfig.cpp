//------------------------------------------------------------------------------------------
//	DIOSTREAMUSBCONFIG.CPP
//	
//	Data IO Stream USB Config class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "DIOStreamUSBLocalEnumDevices.h"

#include "DIOStreamUSBConfig.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	

/*-------------------------------------------------------------------
//	DIOSTREAMUSBCONFIG::DIOSTREAMUSBCONFIG
*/	
/**	
//	
//	Class Constructor DIOSTREAMUSBCONFIG
//	
//	@author				Abraham J. Velez
//	@version			29/05/2014 17:29:06
//	
 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUSBCONFIG::DIOSTREAMUSBCONFIG() : DIOSTREAMCONFIG()
{
	Clean();

	type = DIOSTREAMTYPE_USB;
}



//-------------------------------------------------------------------
//  DIOSTREAMUSBCONFIG::~DIOSTREAMUSBCONFIG
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:18:59
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMUSBCONFIG::~DIOSTREAMUSBCONFIG()
{
	Clean();
}




/*-------------------------------------------------------------------
//	DIOSTREAMUARTCONFIG::GetToString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/02/2016 9:19:10
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUSBCONFIG::GetToString(XSTRING* string)
{
	if(!string) return false;

	string->Empty();

	if(vendorID && productID && classdev)
		{
			string->Format(__L("0x%04X,0x%04X,%d"), vendorID, productID, classdev);			
			return true;
		}
	
	if(!location.IsEmpty() && classdev)
		{
			string->Format(__L("%s,%d"), location.Get(), classdev);			
			return true;
		}
	
	return false;
}




/*-------------------------------------------------------------------
//	DIOSTREAMUARTCONFIG::SetFromString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/02/2016 9:19:43
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUSBCONFIG::SetFromString(XCHAR* string)
{
	if(!string) return false;

	XSTRING  _string   = string;
	int			 _classdev = 0;
	
	if(_string.Find(DIOSTREAMUSBLOCALENUMDEVICES_PORTKEY, true) != XSTRING_NOTFOUND)
		{
			if(_string.Find(DIOSTREAMUSBLOCALENUMDEVICES_PORTHUB, true) != XSTRING_NOTFOUND)
				{
					XSTRING _location;
				
					_string.UnFormat(__L("%s,%d"), location.Get(), &classdev);

					location = _location;
					classdev = (DIOSTREAMDEVICEUSBCLASS)_classdev;

					return true;
				}
		}
	
	XDWORD _vendorID  = 0;
	XDWORD _productID = 0;

	_string.UnFormat(__L("0x%04X,0x%04X,%d"), &_vendorID, &_productID, &_classdev);			

	vendorID  = _vendorID;
	productID = _productID;
	classdev	= (DIOSTREAMDEVICEUSBCLASS)_classdev;

	return ((vendorID && productID)?true:false);
}


