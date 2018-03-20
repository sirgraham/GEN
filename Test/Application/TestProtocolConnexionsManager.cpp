/*------------------------------------------------------------------------------------------
//	PROTOCOLCONNEXIONSMANAGER.CPP
//	
//	Protocol Connexions Manager
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/01/2012 12:04:07
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XPublisher.h"

#include "DIOFactory.h"
#include "DIOStreamUDPLocalEnumServers.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOStreamCipher.h"

#include "CipherAES.h"

#include "TestProtocolApplicationData.h"
#include "TestProtocol.h"

#include "TestProtocolConnexionsManager.h"

#include "XMemory.h"

	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	TESTPROTOCOLCONNEXIONSMANAGER::TESTPROTOCOLCONNEXIONSMANAGER
*/	
/**	
//	
//	Class Constructor TESTPROTOCOLCONNEXIONSMANAGER
//	
//	@author				Abraham J. Velez
//	@version			20/03/2016 17:34:39
//	
*/
/*-----------------------------------------------------------------*/
TESTPROTOCOLCONNEXIONSMANAGER::TESTPROTOCOLCONNEXIONSMANAGER() : DIOPROTOCOLCONNEXIONSMANAGER()
{
	Clean();	
}



/*-------------------------------------------------------------------
//	TESTPROTOCOLCONNEXIONSMANAGER::~TESTPROTOCOLCONNEXIONSMANAGER
*/	
/**	
//	
//	 Class Destructor TESTPROTOCOLCONNEXIONSMANAGER
//	
//	@author				Abraham J. Velez
//	@version			20/03/2016 17:34:49
//	
*/
/*-----------------------------------------------------------------*/
TESTPROTOCOLCONNEXIONSMANAGER::~TESTPROTOCOLCONNEXIONSMANAGER()
{	
	Clean();
}




/*-------------------------------------------------------------------
//	TESTPROTOCOLCONNEXIONSMANAGER::Ini
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/03/2016 17:35:00
//	
//	@return 			bool : 
//
//  @param				isserver : 
//  @param				protocolapplicationdata : 
*/
/*-----------------------------------------------------------------*/
bool TESTPROTOCOLCONNEXIONSMANAGER::Ini(bool isserver, TESTPROTOCOLAPPLICATIONDATA* protocolapplicationdata)
{
	this->protocolapplicationdata = protocolapplicationdata;

	diostreamTCPIPcfg = new  DIOSTREAMTCPIPCONFIG();
	if(!diostreamTCPIPcfg) return false;
		 			
	diostreamudplocalenumservers = new DIOSTREAMUDPLOCALENUMSERVERS();
	if(!diostreamudplocalenumservers)  return false;
						
	diostreamudplocalenumservers->SetID(0xFAAFFAFA);
	diostreamudplocalenumservers->SetPort(4001);
	diostreamudplocalenumservers->SetSearchTimeout(3);
	
	diostreamTCPIPcfg->SetRemotePort(14567);
	
	return DIOPROTOCOLCONNEXIONSMANAGER::Ini(isserver, diostreamTCPIPcfg, diostreamudplocalenumservers);
}



/*-------------------------------------------------------------------
//	TESTPROTOCOLCONNEXIONSMANAGER::End
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/03/2016 17:35:16
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTPROTOCOLCONNEXIONSMANAGER::End()
{
	bool status = DIOPROTOCOLCONNEXIONSMANAGER::End();

	if(diostreamTCPIPcfg)			
		{
			delete diostreamTCPIPcfg;
			diostreamTCPIPcfg = NULL;
		}

	if(diostreamudplocalenumservers)	
		{
			delete diostreamudplocalenumservers;
			diostreamudplocalenumservers = NULL;
		}

	return status;
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexion_Create
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 13:57:34
//	
//	@return 			DIOPROTOCOLCONNEXION* : 
//
*/
/*-----------------------------------------------------------------*/
/*
DIOPROTOCOL* TESTPROTOCOLCONNEXIONSMANAGER::CreateProtocol()
{ 
	CIPHERAES* cipher = new CIPHERAES();
	if(!cipher) return NULL;

	XBYTE	inivector[]	= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
	XBYTE	keydata[]		= { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 }; 
		
	CIPHERKEYSYMMETRICAL* key = new CIPHERKEYSYMMETRICAL();	
	if(!key) return NULL;

	cipher->SetChainingMode(CIPHERCHAININGMODE_CBC);				
	cipher->SetPaddingType(XBUFFER_PADDINGTYPE_ZEROS); 
	cipher->SetInitVector(inivector,sizeof(inivector));		

	key->Set(keydata, sizeof(keydata));	
	
	cipher->SetKey(key);
	
	DIOSTREAMCIPHER* diostreamcipher = new DIOSTREAMCIPHER(diostreamcfg, cipher);
	if(diostreamcipher)
		{				
			DIOSTREAMTCPIP* diostreamTCPIP = (DIOSTREAMTCPIP*)diostreamcipher->GetDIOStreamBase();
			diostreamTCPIP->SetEnumServers(diostreamudplocalenumservers);			

			TESTPROTOCOL* protocol = new TESTPROTOCOL(diostreamcipher);																																																																																					
			if(protocol) 
				{	
					protocol->SetApplicationData(protocolapplicationdata);																																						
					return (DIOPROTOCOL*)protocol;
				}
					
			delete diostreamcipher;
		}

	return NULL;
}
*/

DIOPROTOCOL* TESTPROTOCOLCONNEXIONSMANAGER::CreateProtocol()
{ 
	if(!xfactory)	  return NULL;
	if(!diofactory) return NULL;
				
	DIOSTREAMTCPIP* diostreamTCPIP = (DIOSTREAMTCPIP*)diofactory->CreateStreamIO(diostreamcfg);
	if(diostreamTCPIP)
		{	
			diostreamTCPIP->SetEnumServers(diostreamenumservers);			
			

			TESTPROTOCOL* protocol = new TESTPROTOCOL(diostreamTCPIP);																																																																																					
			if(protocol) 
				{	
					protocol->SetMaskID(TESTPROTOCOLCONNEXIONSMANAGER_PROTOCOLMASKID | (isserver?DIOPROTOCOL_CMDTYPE_ISSERVER:0)); 
					protocol->SetApplicationData(protocolapplicationdata);																																			
					return (DIOPROTOCOL*)protocol;	
				}					

			diofactory->DeleteStreamIO(diostreamTCPIP);
		}

	return NULL;
}





/*-------------------------------------------------------------------
//	TESTPROTOCOLCONNEXIONSMANAGER::DeleteProtocol
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/09/2015 16:42:00
//	
//	@return 			bool : 
//
//  @param				protocol : 
*/
/*-----------------------------------------------------------------*/			
/*
bool TESTPROTOCOLCONNEXIONSMANAGER::DeleteProtocol(DIOPROTOCOL* dioprotocol)
{
	if(!dioprotocol)		return false;

	DIOSTREAMCIPHER*			diostreamcipher = (DIOSTREAMCIPHER*)dioprotocol->GetDIOStream();																																																																		
	CIPHERAES*						cipher				  = (CIPHERAES*)diostreamcipher->GetCipher();
	CIPHERKEYSYMMETRICAL* key							=	(CIPHERKEYSYMMETRICAL*)cipher->GetKey();

	delete cipher;
	delete key;	
	delete dioprotocol;
		
	if(diostreamcipher) delete diostreamcipher;			

	return true;
}	
*/

bool TESTPROTOCOLCONNEXIONSMANAGER::DeleteProtocol(DIOPROTOCOL* dioprotocol)
{ 
	if(!xfactory)			return false;
	if(!diofactory)		return false;
	if(!dioprotocol)	return false;

	DIOSTREAMTCPIP*	diostreamTCPIP = (DIOSTREAMTCPIP*)dioprotocol->GetDIOStream();																																																																		
	
	delete dioprotocol;
		
	if(diostreamTCPIP) diofactory->DeleteStreamIO(diostreamTCPIP);			

	return true;
}	
