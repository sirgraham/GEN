//------------------------------------------------------------------------------------------
//	DIOSTREAMDEVICEIP.H
//	
/**	
// \class 
//   
//  Data IO Stream Device IP class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMDEVICEIP_H_
#define _DIOSTREAMDEVICEIP_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOIP.h"
#include "DIOMAC.h"

#include "DIOStreamDevice.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOSTREAMIPDEVICE_TYPE
{
	DIOSTREAMIPDEVICE_TYPE_UNKNOWN						= 0	,
	DIOSTREAMIPDEVICE_TYPE_ETHERNET								,
	DIOSTREAMIPDEVICE_TYPE_WIFI										,
	DIOSTREAMIPDEVICE_TYPE_PPP										,
	DIOSTREAMIPDEVICE_TYPE_LOOPBACK								,		
};


//---- CLASS -------------------------------------------------------------------------------


class DIOSTREAMDEVICEIP : public DIOSTREAMDEVICE
{
	public:

																	DIOSTREAMDEVICEIP				();
		virtual											 ~DIOSTREAMDEVICEIP				();
		
		DIOMAC*												GetMAC									();
		DIOIP*												GetIP										();

		DIOSTREAMIPDEVICE_TYPE				GetIPType								();
		bool													SetIPType								(DIOSTREAMIPDEVICE_TYPE iptype);

		bool													DebugPrintInfo					(XCONSOLE* xconsole);

	protected:

		DIOMAC												MAC;
		DIOIP													IP;	
		DIOSTREAMIPDEVICE_TYPE				iptype;
	
	private:

		void													Clean										();			
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
