//------------------------------------------------------------------------------------------
//	DIOSTREAMTCPIP.H
//	
/**	
// \class 
//   
//  Data IO Stream TCP/IP class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMTCPIP_H_
#define _DIOSTREAMTCPIP_H_
		
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOIP.h"
#include "DIOStream.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSTREAMTCPIP_MAXLISTEN												1024
#define DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL					3			


//---- CLASS -------------------------------------------------------------------------------

class XBUFFER;
class XFACTORY;
class XTIMER;
class XTHREADCOLLECTED;
class DIOSTREAMTCPIPCONFIG;
class DIOSTREAMENUMSERVERS;
class DIOSTREAMUDP;


class DIOSTREAMTCPIP : public DIOSTREAM
{
	public:
																						DIOSTREAMTCPIP						();
		virtual																 ~DIOSTREAMTCPIP						();		

		DIOSTREAMCONFIG*												GetConfig									();
		bool																		SetConfig									(DIOSTREAMCONFIG* config);	

		
		virtual bool														Open											() 																= 0;
		virtual bool														Disconnect								()																= 0;
		virtual bool														Close											() 																= 0;	

		DIOIP*																	GetClientIP								();

		DIOSTREAMENUMSERVERS*										GetEnumServers						();		
		bool																	  SetEnumServers						(DIOSTREAMENUMSERVERS* localenumservers);

	protected:
		
		DIOSTREAMTCPIPCONFIG*										config;	
		DIOIP																		clientIP;	

		DIOSTREAMENUMSERVERS*										enumservers;

	private:
		
		void																		Clean											();		
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
