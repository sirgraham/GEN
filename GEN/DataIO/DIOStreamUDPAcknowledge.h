/*------------------------------------------------------------------------------------------
//	DIOSTREAMUDPACKNOWLEDGE.H
*/	
/**	
// \class 
//   
//  Data IO Stream UDP with Acknowledge
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 24/11/2014 10:26:28
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOSTREAMUDPACKNOWLEDGE_H_
#define _DIOSTREAMUDPACKNOWLEDGE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOStreamUDP.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define DIOSTREAMUDPACKNOWLEDGE_DEFAULTTIMEOUT 5

/*---- CLASS -----------------------------------------------------------------------------*/

class XTHREADCOLLECTED;
class HASHCRC32;
class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDP;


class DIOSTREAMUDPACKNOWLEDGE : public DIOSTREAMUDP
{
	public:

																		DIOSTREAMUDPACKNOWLEDGE								(DIOSTREAMUDPCONFIG* config);
		virtual												 ~DIOSTREAMUDPACKNOWLEDGE								();
		
		DIOSTREAMCONFIG*								GetConfig															()																															{ return (DIOSTREAMCONFIG*)config;		}
		bool														SetConfig															(DIOSTREAMCONFIG* config)			
																		{ 
																			this->config = (DIOSTREAMUDPCONFIG*)config;
																			return true;
																		}

		DIOSTREAMSTATUS									GetConnectStatus											()
																		{
																			if(!diostream) return DIOSTREAMSTATUS_DISCONNECTED;
																			return diostream->GetConnectStatus();
																		}

		int															GetTimeout                            ()																															{ return timeout;						         }
		void														SetTimeout                            (int timeout  = DIOSTREAMUDPACKNOWLEDGE_DEFAULTTIMEOUT)					{ this->timeout = timeout;		       }
		
		bool														Open																	();
			
		XDWORD													Write																	(XBYTE* buffer, XDWORD size);

		bool														WriteDatagram													(XSTRING& address, XWORD port, XBYTE* buffer, int size);
		bool														WriteDatagram													(XSTRING& address, XWORD port, XBUFFER& xbuffer);

		bool														Disconnect														();																	
		bool														Close																	();
																	
		DIOSTREAM*											GetDIOStreamBase											()																														{ return diostream;									}

	private:
		
		void														Clean																	()
																		{																			
																			config						= NULL;		
																			
																			timeout           = 0;

																			URLremote         = NULL;
																			portremote				= 0;
																			diostream					= NULL;																																						
																			xthreadconnexion  = NULL;
																		}

		static void											ThreadRunFunction											(void* param);	

		
		DIOSTREAMUDPCONFIG*							config;
		
		int                             timeout;

		DIOURL*													URLremote;
		XSTRING                         addressremote;
		XWORD			                      portremote;
		DIOSTREAMUDP*										diostream;				
		XTHREADCOLLECTED*								xthreadconnexion;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

