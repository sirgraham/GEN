//------------------------------------------------------------------------------------------
//	DIOSTREAMBLUETOOTHREMOTEENUMDEVICES.H
//	
/**	
// \class 
//   
//  Data IO Stream Bluetooth REMOTE Enum Devices class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMBLUETOOTHREMOTEENUMDEVICES_H_
#define _DIOSTREAMBLUETOOTHREMOTEENUMDEVICES_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"
#include "DIOStreamEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;

class DIOSTREAMBLUETOOTHREMOTEENUMDEVICES :  public DIOSTREAMENUMDEVICES
{
	public:
														DIOSTREAMBLUETOOTHREMOTEENUMDEVICES				();
		virtual								 ~DIOSTREAMBLUETOOTHREMOTEENUMDEVICES				();
		
		virtual bool						Search																		()																	{ return false;		};
		virtual bool						StopSearch																(bool waitend)											{ return false;		};
		virtual bool						IsSearching																()																	{ return false;	  };

		virtual bool						IsDeviceAvailable													(XCHAR* resource)										{ return false;		};			
				    
	private:

		void										Clean																			();		
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
