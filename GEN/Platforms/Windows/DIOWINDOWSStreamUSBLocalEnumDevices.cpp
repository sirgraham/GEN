//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMUSBLOCALENUMDEVICES.CPP
//	
//	WINDOWS Data IO Stream USB Local Enum Devices class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <setupapi.h>
#include <initguid.h>
#include <stdio.h>

#include "XDebug.h" 

#include "DIOStreamDeviceUSB.h"
#include "DIOWINDOWSStreamUSBLocalEnumDevices.h"

#include "XMemory.h"
 

	
//---- GENERAL VARIABLE --------------------------------------------------------------------

//This is the GUID for the USB device class
DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE           , 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED); // (A5DCBF10-6530-11D2-901F-00C04FB951ED)
DEFINE_GUID(GUID_DEVINTERFACE_USB_PRINT						 , 0x28D78FAD , 0x5A12, 0x11D1, 0xAE, 0x5B, 0x00, 0x00, 0xF8, 0x03, 0xA8, 0xC2);



//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::DIOWINDOWSSTREAMUSBLOCALENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 19:00:01
//	
//	@return 			

//  @param				publisher : 
*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::DIOWINDOWSSTREAMUSBLOCALENUMDEVICES() : DIOSTREAMUSBLOCALENUMDEVICES()
{
	Clean();
}
	


/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::~DIOWINDOWSSTREAMUSBLOCALENUMDEVICES
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
DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::~DIOWINDOWSSTREAMUSBLOCALENUMDEVICES()
{
	Clean();
}






/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::Search
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
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::Search()
{
	DelAllDevices();
	
	SearchGUID(&GUID_DEVINTERFACE_USB_DEVICE);
	SearchGUID(&GUID_DEVINTERFACE_USB_PRINT);
	
	SearchAndDeleteEqualDevices();

	return true;
}



/*-------------------------------------------------------------------
//	DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::SearchGUID
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/02/2015 17:41:06
//	
//	@return 			bool : 
//
//  @param				interfaceclassguid : 
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::SearchGUID(const GUID* interfaceclassguid)
{
	HDEVINFO                         hdevinfo;
	SP_DEVICE_INTERFACE_DATA         devinterfacedata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA devinterfacedetaildata;
	SP_DEVINFO_DATA                  devinfodata;
	DWORD														 size;
	DWORD                            memberindex;
	
	// We will try to get device information set for all USB devices that have a device interface and are currently present on the system (plugged in).
	hdevinfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT | DIGCF_ALLCLASSES);	
	if(hdevinfo != INVALID_HANDLE_VALUE)
		{
			memset(&devinterfacedata, 0, sizeof(SP_DEVICE_INTERFACE_DATA));

			devinterfacedata.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			memberindex = 0;
		
			// Next, we will keep calling this SetupDiEnumDeviceInterfaces(..) until this function causes GetLastError() to return  ERROR_NO_MORE_ITEMS. With each call the memberindex value needs to be incremented to retrieve the next device interface information.
			SetupDiEnumDeviceInterfaces(hdevinfo, NULL, interfaceclassguid, memberindex, &devinterfacedata);

			while(GetLastError() != ERROR_NO_MORE_ITEMS)
				{
					// As a last step we will need to get some more details for each of device interface information we are able to retrieve. This device interface detail gives us the information we need to identify the device (VID/PID), and decide if it's useful to us. It will also provide a DEVINFO_DATA structure which we can use to know the serial port name for a virtual com port.
					devinfodata.cbSize = sizeof(devinfodata);
			
					// Get the required buffer size. Call SetupDiGetDeviceInterfaceDetail with a NULL devinterfacedetaildata pointer, a devinterfacedetaildataSize  of zero, and a valid RequiredSize variable. In response to such a call, this function returns the required buffer size at size.
					SetupDiGetDeviceInterfaceDetail(hdevinfo, &devinterfacedata, NULL, 0, &size, NULL);

					// Allocate memory for the DeviceInterfaceDetail struct. Don't forget to deallocate it later!
					devinterfacedetaildata = (PSP_DEVICE_INTERFACE_DETAIL_DATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
					if(devinterfacedetaildata) 
						{
							devinterfacedetaildata->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

							if(SetupDiGetDeviceInterfaceDetail(hdevinfo, &devinterfacedata, devinterfacedetaildata, size, &size, &devinfodata))
								{					
									DIOSTREAMDEVICEUSB* device = new DIOSTREAMDEVICEUSB();
									if(device)
										{
											device->SetIndex(devices.GetSize());
											device->SetType(DIOSTREAMDEVICE_TYPE_USB);

											device->GetResource()->Set(devinterfacedetaildata->DevicePath);									

											XSTRING   trash[2];									
											XSTRING   serial;
											int				vendorID;
											int				productID;

											trash[0].AdjustSize(_MAXSTR);
											trash[1].AdjustSize(_MAXSTR);									
											serial.AdjustSize(_MAXSTR);								

											device->GetResource()->UnFormat(__L("%s#vid_%04x&pid_%04x#%s#%s"),trash[0].Get(), &vendorID, &productID, serial.Get(), trash[1].Get());							

											trash[0].AdjustSize();
											trash[1].AdjustSize();									
											serial.AdjustSize();
									
											device->GetSerialString()->Set(serial);									
											device->SetVendorID(vendorID); 
											device->SetProductID(productID);

											GetInfoFromRegistry(device);
																										
											devices.Add(device);

											//XDEBUG_PRINTCOLOR(1, __L("%02d %-20s %s"), index, device->GetName()->Get(), device->GetResource()->Get());									
										}										
								}
						}

					HeapFree(GetProcessHeap(), 0, devinterfacedetaildata);

					SetupDiEnumDeviceInterfaces( hdevinfo, NULL, interfaceclassguid, ++memberindex, &devinterfacedata);
				}

			SetupDiDestroyDeviceInfoList(hdevinfo);
		}

	return true;
}





/*-------------------------------------------------------------------
//	DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::GetInfoFromRegistry
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/10/2014 11:03:59
//	
//	@return 			bool : 
//
//  @param				device : 
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::GetInfoFromRegistry(DIOSTREAMDEVICEUSB* device)
{	
	
	XSTRING	name;
	HKEY		hkey; 
	LONG		statusreg;		
	XCHAR   value[_MAXSTR];
	DWORD		sizevalue;	
	DWORD		type;	
	int			indexcut;
	bool		status = true;

	name.Format(__L("SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_%04X&PID_%04X\\%s"), device->GetVendorID(), device->GetProductID(), device->GetSerialString()->Get());
	name.ToUpperCase();

	statusreg = RegOpenKeyEx(HKEY_LOCAL_MACHINE, name.Get(), 0, KEY_QUERY_VALUE, &hkey);
	if(statusreg == ERROR_SUCCESS)
		{						
			sizevalue = _MAXSTR-1;
			memset(value, 0,_MAXSTR);
			statusreg = RegQueryValueEx(hkey,__L("LocationInformation"), NULL, &type, (BYTE*)value, &sizevalue); 
			if(statusreg == ERROR_SUCCESS) device->GetLocation()->Set(value);

			sizevalue = _MAXSTR-1;
			memset(value, 0,_MAXSTR);
			statusreg = RegQueryValueEx(hkey,__L("ParentIdPrefix"), NULL, &type, (BYTE*)value, &sizevalue); 
			if(statusreg == ERROR_SUCCESS) device->GetParentID()->Set(value);
			
			sizevalue = _MAXSTR-1;
			memset(value, 0,_MAXSTR);
			statusreg = RegQueryValueEx(hkey,__L("DeviceDesc"), NULL, &type, (BYTE*)value, &sizevalue); 			
			if(statusreg == ERROR_SUCCESS) device->GetDescription()->Set(value);				
			indexcut = device->GetDescription()->Find(__L(";"), true);
			if(indexcut !=  XSTRING_NOTFOUND) device->GetDescription()->DeleteCharacters(0, indexcut+1);		

			XSTRING USBclassstring;
			int			USBclass = 0x00;

			sizevalue = _MAXSTR-1;
			memset(value, 0,_MAXSTR);
			statusreg = RegQueryValueEx(hkey,__L("CompatibleIDs"), NULL, &type, (BYTE*)value, &sizevalue); 			
			if(statusreg == ERROR_SUCCESS) USBclassstring.Set(value);
			if(USBclassstring.GetSize()) USBclassstring.UnFormat(__L("USB\\Class_%02x"), &USBclass);
					
			device->SetClass((DIOSTREAMDEVICEUSBCLASS)USBclass);

			
			RegCloseKey(hkey);

			status = device->GetName()->GetSize()?true:false;

		} 
	 else 
	  {
			for(int c=0;c<(int)devices.GetSize();c++)
				{
					DIOSTREAMDEVICEUSB* tmpdevice = (DIOSTREAMDEVICEUSB*)devices.Get(c);
					if(tmpdevice)
						{
							if((tmpdevice->GetVendorID()== device->GetVendorID() && (tmpdevice->GetProductID())== device->GetProductID()))
								{									
									if(device->GetSerialString()->Find((*tmpdevice->GetParentID()), true) != XSTRING_NOTFOUND)
										{										
											for(int d=0; d<99; d++)
												{
													name.Format(__L("SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_%04X&PID_%04X&MI_%02d\\%s"), device->GetVendorID(), device->GetProductID(), d, device->GetSerialString()->Get());
													name.ToUpperCase();

													statusreg = RegOpenKeyEx(HKEY_LOCAL_MACHINE, name.Get(), 0, KEY_QUERY_VALUE, &hkey);
													if(statusreg == ERROR_SUCCESS)
														{
															device->GetLocation()->Set((*tmpdevice->GetLocation()));

															sizevalue = _MAXSTR-1;
															memset(value, 0,_MAXSTR);
															statusreg = RegQueryValueEx(hkey,__L("DeviceDesc"), NULL, &type, (BYTE*)value, &sizevalue); 			
															if(statusreg == ERROR_SUCCESS) device->GetDescription()->Set(value);
															indexcut = device->GetDescription()->Find(__L(";"), true);
															if(indexcut !=  XSTRING_NOTFOUND) device->GetDescription()->DeleteCharacters(0, indexcut+1);	

															XSTRING USBclassstring;
															int			USBclass = 0x00;

															sizevalue = _MAXSTR-1;
															memset(value, 0,_MAXSTR);
															statusreg = RegQueryValueEx(hkey,__L("CompatibleIDs"), NULL, &type, (BYTE*)value, &sizevalue); 			
															if(statusreg == ERROR_SUCCESS) USBclassstring.Set(value);
															if(USBclassstring.GetSize()) USBclassstring.UnFormat(__L("USB\\Class_%02x"), &USBclass);
					
															device->SetClass((DIOSTREAMDEVICEUSBCLASS)USBclass);

															RegCloseKey(hkey);
															break;
														}
												}														
										}
								}
						}
				}

			 status = false;
		}

	return status;
}




/*-------------------------------------------------------------------
//	DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::SearchAndDeleteEqualDevices
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/02/2015 14:00:28
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::SearchAndDeleteEqualDevices()
{
	if(devices.IsEmpty()) return false;

	bool found;

	do{ found = false;

			for(int c=0; c<(int)devices.GetSize(); c++)
				{
					DIOSTREAMDEVICEUSB* device1 = (DIOSTREAMDEVICEUSB*)devices.Get(c);
					if(device1)
						{
							for(int d=0; d<(int)devices.GetSize(); d++)
								{
									DIOSTREAMDEVICEUSB* device2 = (DIOSTREAMDEVICEUSB*)devices.Get(d);
									if(device2 && (c!=d))
										{
											if((!device1->GetLocation()->Compare(device2->GetLocation()->Get()) && (device1->GetClass() == device2->GetClass())))												 
												{
													devices.Delete(device1);
													delete device1;
													
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