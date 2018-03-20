 //------------------------------------------------------------------------------------------
//	DIOSTREAMBLUETOOTHCONFIG.H
//	
/**	
// \class 
//   
//  Data IO Stream Bluetooth Config class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMBLUETOOTHCONFIG_H_
#define _DIOSTREAMBLUETOOTHCONFIG_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOMAC.h"
#include "DIOStreamConfig.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;


class DIOSTREAMBLUETOOTHCONFIG : public DIOSTREAMCONFIG
{
	public:					
																									DIOSTREAMBLUETOOTHCONFIG			();
		virtual																			 ~DIOSTREAMBLUETOOTHCONFIG			();

		int																						GetDeviceSelect								();
		bool																					SetDeviceSelect								(int deviceselect);

		DIOMAC*																				GetLocalMAC										();
		DIOMAC*																				GetRemoteMAC									();
		
		int																						GetRemoteChannel							();
		bool																					SetRemoteChannel							(int port);

		XSTRING*																			GetPIN												();

		void*																					GetServerPropertys						(int index);

	protected:
			
		int																						deviceselect;		
		DIOMAC																				localMAC;
		DIOMAC																				remoteMAC;
		int 																					channel;		
		XSTRING																				pin;

		void*																					serverproperties[3];
		
	private:		

		void																					Clean													();								
};		



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
