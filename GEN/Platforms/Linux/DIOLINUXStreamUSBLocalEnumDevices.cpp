//------------------------------------------------------------------------------------------
//	DIOLINUXSTREAMUSBLOCALENUMDEVICES.CPP
//	
//	LINUX Data IO Stream USB Local Enum Devices class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <libudev.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libusb-1.0/libusb.h>

#include "XDebug.h"
#include "XPath.h"
#include "DIOStreamDeviceUSB.h"

#include "DIOLINUXStreamUSBLocalEnumDevices.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	

	
//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOLINUXSTREAMUSBLOCALENUMDEVICES::DIOLINUXSTREAMUSBLOCALENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 19:00:01
//	
//	@return 			

 
*/
/*-----------------------------------------------------------------*/
DIOLINUXSTREAMUSBLOCALENUMDEVICES::DIOLINUXSTREAMUSBLOCALENUMDEVICES() : DIOSTREAMUSBLOCALENUMDEVICES()
{

}
	


/*-------------------------------------------------------------------
//  DIOLINUXSTREAMUSBLOCALENUMDEVICES::~DIOLINUXSTREAMUSBLOCALENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 19:00:12
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOLINUXSTREAMUSBLOCALENUMDEVICES::~DIOLINUXSTREAMUSBLOCALENUMDEVICES()
{

}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMUSBLOCALENUMDEVICES::Search
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 19:00:27
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOLINUXSTREAMUSBLOCALENUMDEVICES::Search()
{			
	DelAllDevices();

  struct udev*							udev								= NULL;
	struct udev_enumerate*		udevenumerate				= NULL;
	struct udev_list_entry*		udevdeviceslist			= NULL;
	struct udev_list_entry*		udevdevicelistentry = NULL;	
	int                       index = 0;

	// Create the udev object 
	udev = udev_new();
	if (!udev) return false;

	udevenumerate		= udev_enumerate_new(udev);		
//udev_enumerate_add_match_subsystem(udev_enumerate, "usb");
	udev_enumerate_scan_devices(udevenumerate);

	udevdeviceslist = udev_enumerate_get_list_entry(udevenumerate);

	udev_list_entry_foreach(udevdevicelistentry, udevdeviceslist) 
		{
			struct udev_device*	udevdevice					= NULL;
			struct udev_device*	udevdeviceinterface = NULL;
			struct udev_device*	udevsearchdevice		= NULL;	
			const char*					path;
			XSTRING							classstr;
			
			path			 				= udev_list_entry_get_name(udevdevicelistentry);			
			udevsearchdevice	= udev_device_new_from_syspath(udev, path); 
			
			udevdeviceinterface =  udev_device_get_parent_with_subsystem_devtype(udevsearchdevice, "usb", "usb_interface");						
			if(udevdeviceinterface) classstr	= udev_device_get_sysattr_value(udevdeviceinterface, "bInterfaceClass");
			
			udevdevice = udev_device_get_parent_with_subsystem_devtype(udevsearchdevice, "usb", "usb_device");
			if(udevdevice) 
				{	
					XSTRING		devicepath;				
					XSTRING		typestr;	
					XSTRING		hubstr;
					XSTRING		resource;								
					XSTRING		vid;
					XSTRING		pid;									
					int				vendorID	= 0;
					int				productID = 0;																
					
					devicepath = path;
														
					typestr		= udev_device_get_devtype(udevdevice);	
					hubstr		= udev_device_get_sysattr_value(udevdevice, "busnum");				
					resource	= udev_device_get_devnode(udevsearchdevice);
					vid				= udev_device_get_sysattr_value(udevdevice, "idVendor");
					pid				= udev_device_get_sysattr_value(udevdevice, "idProduct");
		
					if(!resource.IsEmpty())
						{														
							if(resource.Find(__L("/dev/bus/"), true) == XSTRING_NOTFOUND)
								{	
									int				port;
									int				empty[2];									
									XSTRING   string[2];					

									string[0].AdjustSize(_MAXSTR);
									string[1].AdjustSize(_MAXSTR);

									devicepath.UnFormat(__L("%s-%d.%d:%d%s"), string[0].Get(), &empty[0], &port, &empty[1], string[1].Get());

									string[0].AdjustSize();
									string[1].AdjustSize();

									//XDEBUG_PRINTCOLOR(2, __L("%s - [port %d] class[%s]"), devicepath.Get(), port, classstr.Get());													

									DIOSTREAMDEVICEUSB* device = new DIOSTREAMDEVICEUSB();
									if(device)
										{		
											vid.UnFormat(__L("%08X"), &vendorID);
											pid.UnFormat(__L("%08X"), &productID);																							

											device->SetIndex(index);
											device->SetType(DIOSTREAMDEVICE_TYPE_USB);																																							

											device->GetLocation()->Format(__L("%s#%04d.%s#%04d"), DIOSTREAMUSBLOCALENUMDEVICES_PORTKEY, port, DIOSTREAMUSBLOCALENUMDEVICES_PORTHUB, hubstr.ConvertToInt());
											device->GetDescription()->Set(udev_device_get_sysattr_value(udevdevice, "product"));																				
											device->GetResource()->Set(resource);										

											device->SetVendorID(vendorID); 
											device->SetProductID(productID);

											device->SetClass((DIOSTREAMDEVICEUSBCLASS)classstr.ConvertToInt());
											device->GetSerialString()->Set(udev_device_get_sysattr_value(udevdevice, "serial"));
																		
											devices.Add(device);			
						
											index++;
										}														
								}
						}										
				}	

			if(udevsearchdevice)
				{
					udev_device_unref(udevsearchdevice);														
					udevsearchdevice    = NULL;				
				}
		}

	// Free the enumerator object 
	if(udevenumerate) udev_enumerate_unref(udevenumerate);
	if(udev)					udev_unref(udev);

	return true;
}




/*-------------------------------------------------------------------
//  DIOLINUXSTREAMUSBLOCALENUMDEVICES::IsDeviceAvailable
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 19:00:32
//	
//	@return 			bool : 
//	@param				resource : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXSTREAMUSBLOCALENUMDEVICES::IsDeviceAvailable(XCHAR* resource)
{
	
	return true;
}
				    




/*-------------------------------------------------------------------
//	DIOLINUXSTREAMUSBLOCALENUMDEVICES::SearchAndDeleteEqualDevices
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/11/2014 13:41:56
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXSTREAMUSBLOCALENUMDEVICES::SearchAndDeleteEqualDevices()
{
	if(devices.IsEmpty()) return false;

	bool found;

	do{ found = false;

			for(int c=0; c<devices.GetSize(); c++)
				{
					DIOSTREAMDEVICEUSB* device1 = (DIOSTREAMDEVICEUSB*)devices.Get(c);
					if(device1)
						{
							for(int d=0; d<devices.GetSize(); d++)
								{
									DIOSTREAMDEVICEUSB* device2 = (DIOSTREAMDEVICEUSB*)devices.Get(d);
									if(device2 && (c!=d))
										{
											if(!device1->GetName()->Compare(device2->GetName()->Get()))
												{
													delete device1;
													devices.Delete(device1);

													found = true;
													break;
												}
										}
								}

							if(found) break;													
						}
				}

			if(!found) break;

		} while(1);

	return true;
}


#endif