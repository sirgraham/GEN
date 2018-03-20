//------------------------------------------------------------------------------------------
//	DIOSTREAMENUMDEVICES.CPP
//	
//	Data IO Stream Enum Devices class
//	
//
//	@author	 Abraham J. Velez
//	@version 12/3/2003
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"	
#include "XPublisher.h"	

#include "DIOStreamDevice.h"

#include "DIOStreamEnumDevices.h"

#include "XMemory.h"
	

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOSTREAMENUMDEVICES::DIOSTREAMENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 16:43:25
//	
//	@return 			

 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMENUMDEVICES::DIOSTREAMENUMDEVICES()
{
	Clean();

	;

	xtimerout = xfactory->CreateTimer();
}



/*-------------------------------------------------------------------
//  DIOSTREAMENUMDEVICES::~DIOSTREAMENUMDEVICES
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 16:43:38
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOSTREAMENUMDEVICES::~DIOSTREAMENUMDEVICES()
{
	if(xtimerout) xfactory->DeleteTimer(xtimerout);

	DelAllDevices();

	Clean();
}




/*-------------------------------------------------------------------
//  DIOSTREAMENUMDEVICES::GetType
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/05/2013 1:06:24
//	
//	@return 			DIOSTREAMENUMTYPE : 
//	*/
/*-----------------------------------------------------------------*/
DIOSTREAMENUMTYPE DIOSTREAMENUMDEVICES::GetType()
{
	return type;
}



/*-------------------------------------------------------------------
//  DIOSTREAMENUMDEVICES::SetType
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/05/2013 1:06:46
//	
//	@return 			bool : 
//	@param				type : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMDEVICES::SetType(DIOSTREAMENUMTYPE type)
{
	this->type = type;

	return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMENUMDEVICES::AddDevice
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 16:46:39
//	
//	@return 			bool : 
//	@param				device : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMDEVICES::AddDevice(DIOSTREAMDEVICE* device)
{
	devices.Add(device);

	return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMENUMDEVICES::DelAllDevices
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 16:46:46
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMDEVICES::DelAllDevices()
{
	if(devices.IsEmpty()) return false;

	devices.DeleteContents();	
	devices.DeleteAll();

	return true;	
}



/*-------------------------------------------------------------------
//	DIOSTREAMENUMDEVICES::GetDeviceByname
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/10/2014 16:06:35
//	
//	@return 			DIOSTREAMDEVICE* : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICE* DIOSTREAMENUMDEVICES::GetDeviceByName(XCHAR* name)
{
	if(devices.IsEmpty()) return NULL;

	for(XDWORD c=0; c<devices.GetSize(); c++)
		{
			DIOSTREAMDEVICE* device = devices.Get(c);
			if(device)
				{
					if(!device->GetName()->Compare(name)) return device;
				}
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	DIOSTREAMENUMDEVICES::GetDeviceByIndex
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/01/2017 22:14:05
//	
//	@return 			DIOSTREAMDEVICE* : 
//
//  @param				index : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICE* DIOSTREAMENUMDEVICES::GetDeviceByIndex(int index)
{
	for(int c=0;c<(int)devices.GetSize();c++)
		{
			DIOSTREAMDEVICE* device = devices.Get(c);
			if(device)
				{
					if(device->GetIndex() == index) return device;
				}
		}

	return NULL;
}