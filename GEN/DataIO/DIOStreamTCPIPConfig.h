//------------------------------------------------------------------------------------------
//	DIOSTREAMTCPIP.H
//	
/**	
// \class 
//   
//  Data IO Stream TCP/IP Config class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMTCPIPCONFIG_H_
#define _DIOSTREAMTCPIPCONFIG_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIOStreamConfig.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class DIOFACTORY;

class DIOSTREAMTCPIPCONFIG  : public DIOSTREAMCONFIG
{
	public:					
														DIOSTREAMTCPIPCONFIG 				();
		virtual								 ~DIOSTREAMTCPIPCONFIG 				();

    DIOIP*									GetLocalIP									();		
		DIOURL*									GetRemoteURL								();
		
		int											GetRemotePort								(); 
		bool										SetRemotePort								(int port);

		bool										GetToString									(XSTRING* string);
		bool										GetToString									(XSTRING& string)									{ return GetToString(&string);														}
		
		bool										SetFromString								(XCHAR* string);											
		bool										SetFromString								(XSTRING* string)                
														{ 
															if(!string) return false;
															return SetFromString(string->Get());										
														} 
		bool										SetFromString								(XSTRING& string, bool addslash)	{ return SetFromString(&string);												  }

		int											GetCounterMultiServer				()																{ return countermultiserver;															}
		void										SetCounterMultiServer				(int countermultiserver)					{ this->countermultiserver = countermultiserver;          }

		XMUTEX*									GetXMutexMultiServer				()																{ return xmutexmultiserver;																}

		int											GetHandleMultiServer				()																{ return handlemultiserver;																}
		void										SetHandleMultiServer				(int handlemultiserver)						{ this->handlemultiserver = handlemultiserver;						}
					
	protected:

    DIOIP										localIP;
		DIOURL*									remoteURL;
		int 										remoteport;

		XMUTEX*									xmutexmultiserver;
		int											countermultiserver;
		int											handlemultiserver;
		
	private:

		void										Clean												()
														{	
															type 										= DIOSTREAMTYPE_TCPIP;																

															remoteURL								= NULL;
															remoteport							= 0;

															xmutexmultiserver				= NULL;

															countermultiserver			= 0;
															handlemultiserver       = -1;
														}
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
