//------------------------------------------------------------------------------------------
//	DIOSTREAMUSBCONFIG.H
//	
/**	
// \class 
//   
//  Data IO Stream USB Config class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMUSBCONFIG_H_
#define _DIOSTREAMUSBCONFIG_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"
#include "DIOStreamConfig.h"
#include "DIOStreamDeviceUSB.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;

class DIOSTREAMUSBCONFIG : public DIOSTREAMCONFIG
{
	public:					
															 	  DIOSTREAMUSBCONFIG								();
		virtual											 ~DIOSTREAMUSBCONFIG								();


		XSTRING*											GetLocation												()																		{ return &location;                  }
		
		XDWORD												GetVendorID												()																		{ return vendorID;									 }
		void													SetVendorID												(XDWORD vendorID)											{ this->vendorID = vendorID;				 }
		
		XDWORD												GetProductID											()																		{	return productID;									 }
		void													SetProductID											(XDWORD productID)										{ this->productID=productID;				 }
		
		DIOSTREAMDEVICEUSBCLASS				GetClass													()																		{ return classdev;									 }
		void													SetClass													(DIOSTREAMDEVICEUSBCLASS classdev)		{ this->classdev = classdev;				 }

		XSTRING*											GetResource												()																		{ return &resource;							     }

		bool													GetToString												(XSTRING* string);
		bool													GetToString												(XSTRING& string)											{ return GetToString(&string);			 }
		
		bool													SetFromString											(XCHAR* string);											
		bool													SetFromString											(XSTRING* string)                
																	{ 
																		if(!string) return false;
																		return SetFromString(string->Get());										
																	} 
		bool													SetFromString											(XSTRING& string)											{ return SetFromString(&string);		 }
			

	protected:
	
		XSTRING												location;
		XDWORD												vendorID;
		XDWORD												productID;
		DIOSTREAMDEVICEUSBCLASS				classdev;		
		XSTRING                       resource;
		
	private:
		
		void													Clean															()
																	{
																		vendorID		= 0;
																		productID   = 0;		
																		classdev    = DIOSTREAMDEVICEUSBCLASS_DEVICE_USECLASSINFORMATIONINTERFACEDESCRIPTORS;
																	}
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
