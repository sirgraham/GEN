//------------------------------------------------------------------------------------------
//	DIOLINUXSTREAMTCPIP.CPP
//	
//	LINUX Data IO Stream Bluetooth Local Enum Devices class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/01/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	

#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------


#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "DIOStreamDeviceBluetooth.h"

#include "DIOLINUXStreamBluetoothLocalEnumDevices.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOLINUXSTREAMTCPIPLOCALENUMDEVICES::DIOLINUXSTREAMTCPIPLOCALENUMDEVICES
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
DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES::DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES() : DIOSTREAMBLUETOOTHLOCALENUMDEVICES()
{

}
	


/*-------------------------------------------------------------------
//  DIOLINUXSTREAMTCPIPLOCALENUMDEVICES::~DIOLINUXSTREAMTCPIPLOCALENUMDEVICES
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
DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES::~DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES()
{

}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMTCPIPLOCALENUMDEVICES::Search
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
bool DIOLINUXSTREAMBLUETOOTHLOCALENUMDEVICES::Search()
{		
	DelAllDevices();

	struct hci_dev_info	di;
	int									indexdev = 0;

	while(hci_devinfo(indexdev, &di)>=0)
		{
			DIOSTREAMDEVICEBLUETOOTH* device = new DIOSTREAMDEVICEBLUETOOTH();
			if(device)
				{
					char addr[32];
					
					device->GetName()->Set(di.name);

					ba2str(&di.bdaddr, addr);							
					device->GetMAC()->Set(addr);

					devices.Add(device);
				}

			indexdev++;
		}
	
	return true;
}


#endif