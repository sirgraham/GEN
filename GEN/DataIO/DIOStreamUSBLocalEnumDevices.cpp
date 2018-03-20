//------------------------------------------------------------------------------------------
//	DIOSTREAMUSBLOCALENUMDEVICES.CPP
//	
//	Data IO Stream USB Local Enum Device class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamDeviceUSB.h"
#include "DIOStreamUSBConfig.h"
#include "DIOStreamUSBLocalEnumDevices.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	


/*-------------------------------------------------------------------
//  DIOSTREAMUSBLOCALENUMDEVICES::DIOSTREAMUSBLOCALENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 18:37:55
//	
//	@return 			void : 

 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUSBLOCALENUMDEVICES::DIOSTREAMUSBLOCALENUMDEVICES() : DIOSTREAMENUMDEVICES()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMUSBLOCALENUMDEVICES::~DIOSTREAMUSBLOCALENUMDEVICES
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
DIOSTREAMUSBLOCALENUMDEVICES::~DIOSTREAMUSBLOCALENUMDEVICES()
{
	Clean();
}




/*-------------------------------------------------------------------
//	DIOSTREAMUSBLOCALENUMDEVICES::FindDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2016 17:07:23
//	
//	@return 			DIOSTREAMDEVICEUSB* : 
//
//  @param				location : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICEUSB* DIOSTREAMUSBLOCALENUMDEVICES::FindDevice(XCHAR* location)
{
	if(devices.IsEmpty()) return NULL;

	for(XDWORD c=0; c<devices.GetSize(); c++)
		{
			DIOSTREAMDEVICEUSB* device = (DIOSTREAMDEVICEUSB*)devices.Get(c);
			if(device)
				{
					if(!device->GetLocation()->Compare(location)) 
						return (DIOSTREAMDEVICEUSB*)device;
				}
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	DIOSTREAMUSBLOCALENUMDEVICES::FindDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2016 17:07:39
//	
//	@return 			DIOSTREAMDEVICEUSB* : 
//
//  @param				location : 
//  @param				classdev : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICEUSB* DIOSTREAMUSBLOCALENUMDEVICES::FindDevice(XCHAR* location, DIOSTREAMDEVICEUSBCLASS classdev)
{
	if(devices.IsEmpty()) return NULL;

	for(XDWORD c=0; c<devices.GetSize(); c++)
		{
			DIOSTREAMDEVICEUSB* device = (DIOSTREAMDEVICEUSB*)devices.Get(c);
			if(device)
				{
					if(!device->GetLocation()->Compare(location))
						{						
							if(device->GetClass() == classdev)  return (DIOSTREAMDEVICEUSB*)device;
						}
				}
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	DIOSTREAMUSBLOCALENUMDEVICES::FindDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2016 17:10:22
//	
//	@return 			DIOSTREAMDEVICEUSB* : 
//
//  @param				vendorID : 
//  @param				productID : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICEUSB* DIOSTREAMUSBLOCALENUMDEVICES::FindDevice(XDWORD vendorID, XDWORD productID)
{
	if(devices.IsEmpty()) return NULL;

	for(XDWORD c=0; c<devices.GetSize(); c++)
		{
			DIOSTREAMDEVICEUSB* device = (DIOSTREAMDEVICEUSB*)devices.Get(c);
			if(device)
				{
					if((device->GetVendorID() == vendorID) && (device->GetProductID() == productID)) return (DIOSTREAMDEVICEUSB*)device;					
				}
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	DIOSTREAMUSBLOCALENUMDEVICES::FindDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2016 17:10:38
//	
//	@return 			DIOSTREAMDEVICEUSB* : 
//
//  @param				vendorID : 
//  @param				productID : 
//  @param				classdev : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICEUSB* DIOSTREAMUSBLOCALENUMDEVICES::FindDevice(XDWORD vendorID, XDWORD productID, DIOSTREAMDEVICEUSBCLASS classdev)
{
	if(devices.IsEmpty()) return NULL;

	for(XDWORD c=0; c<devices.GetSize(); c++)
		{
			DIOSTREAMDEVICEUSB* device = (DIOSTREAMDEVICEUSB*)devices.Get(c);
			if(device)
				{
					if((device->GetVendorID() == vendorID) && (device->GetProductID() == productID) && (device->GetClass() == classdev))  return (DIOSTREAMDEVICEUSB*)device;					
				}
		}

	return NULL;

}


/*-------------------------------------------------------------------
//	DIOSTREAMUSBLOCALENUMDEVICES::FindDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2016 17:15:39
//	
//	@return 			DIOSTREAMDEVICEUSB* : 
//
//  @param				config : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICEUSB* DIOSTREAMUSBLOCALENUMDEVICES::FindDevice(DIOSTREAMUSBCONFIG& config)
{
	if(devices.IsEmpty()) return NULL;

	if(config.GetVendorID()  && config.GetProductID())
		{
			return FindDevice(config.GetVendorID(), config.GetProductID(), config.GetClass());
		}

	if(!config.GetLocation()->IsEmpty())
		{
			return FindDevice(config.GetLocation()->Get(), config.GetClass());
		}

	return NULL;
}





/*-------------------------------------------------------------------
//  DIOSTREAMUSBLOCALENUMDEVICES::Clean
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
void DIOSTREAMUSBLOCALENUMDEVICES::Clean()
{
	type			  = DIOSTREAMENUMTYPE_USB_LOCAL;
	issearching = false;
}

