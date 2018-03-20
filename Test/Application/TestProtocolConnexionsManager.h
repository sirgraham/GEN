/*------------------------------------------------------------------------------------------
//	TESTPROTOCOLCONNEXIONSMANAGER.H
*/	
/**	
// \class 
//   
//  Test Protocol Connexions Manager
//   
//	@author	 Abraham J. Velez
//	@version 30/01/2012 12:05:03
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTPROTOCOLCONNEXIONSMANAGER_H_
#define _TESTPROTOCOLCONNEXIONSMANAGER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

#include "DIOProtocolConnexionsManager.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define TESTPROTOCOLCONNEXIONSMANAGER_LOCALENUMID          0xFAAFFAAA
#define TESTPROTOCOLCONNEXIONSMANAGER_LOCALENUMPORT        4050
#define TESTPROTOCOLCONNEXIONSMANAGER_PROTOCOLPORT         15580
#define TESTPROTOCOLCONNEXIONSMANAGER_PROTOCOLMASKID       0x01000000


/*---- CLASS -----------------------------------------------------------------------------*/
	
class DIOPROTOCOL;
class CIPHER;
class APPLICATIONDATA;


class TESTPROTOCOLCONNEXIONSMANAGER : public DIOPROTOCOLCONNEXIONSMANAGER
{
  public:
																			  TESTPROTOCOLCONNEXIONSMANAGER						();
																	 		 ~TESTPROTOCOLCONNEXIONSMANAGER						();
		
		bool																Ini																			(bool isserver, TESTPROTOCOLAPPLICATIONDATA* protocolapplicationdata);		
		bool																End																			();
	
		DIOPROTOCOL*												CreateProtocol													();
		bool																DeleteProtocol													(DIOPROTOCOL*	protocol);
				
	private:

    void																Clean																		()
																				{																				
																					protocolapplicationdata					= NULL;
																					diostreamudplocalenumservers		= NULL;
																					diostreamTCPIPcfg								= NULL;			
																				}	

		TESTPROTOCOLAPPLICATIONDATA*				protocolapplicationdata;
		DIOSTREAMUDPLOCALENUMSERVERS*				diostreamudplocalenumservers;
		DIOSTREAMTCPIPCONFIG*								diostreamTCPIPcfg;			
};

	

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

