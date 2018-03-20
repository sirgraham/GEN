//------------------------------------------------------------------------------------------
//	DIOSTREAMUDPLOCALENUMSERVERS.H
//	
/**	
// \class 
//   
//  Data IO Stream UDP Local Enum Servers class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMUDPLOCALENUMSERVERS_H_
#define _DIOSTREAMUDPLOCALENUMSERVERS_H_
		
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamEnumServers.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSTREAMUDPLOCALENUMSERVERSDEFAULTSEARCHTIMEOUT    30     // seconds
#define DIOSTREAMUDPLOCALENUMSERVERSID											__L("GEN Broadcast message ID [%08X]")

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XBUFFER;
class XTIMER;
class XTHREADCOLLECTED;
class DIOFACTORY;
class DIOURL;
class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDP;


class DIOSTREAMUDPLOCALENUMSERVERS :  public DIOSTREAMENUMSERVERS
{
	public:
															DIOSTREAMUDPLOCALENUMSERVERS					();
		virtual									 ~DIOSTREAMUDPLOCALENUMSERVERS					();

		bool											Search																();
		bool											StopSearch														(bool waitend);
		
		bool											IsDeviceAvailable											(XCHAR* resource);	
		
		DIOSTREAMUDPCONFIG*				GetDIOStreamUDPCFG										();
		DIOSTREAMUDP*							GetDIOStreamUDP												();

		bool											SendSignal														();
				
		int												GetPort																();				
		bool											SetPort																(int port);
				    
	private:

		void											Clean																	();
		
		static void 							ThreadDiscoveryLocalServer						(void* data);		
				
		DIOSTREAMUDPCONFIG*				diostreamudpcfg;
		DIOSTREAMUDP*							diostreamudp;
		
		int												port;
				
		XTHREADCOLLECTED*					xthread;		
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
