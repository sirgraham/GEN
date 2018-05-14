//------------------------------------------------------------------------------------------
//	TEST.H
//
/**
// \class
//
//  Test
//
//	@author	 Abraham J. Velez
//	@version 06/10/2005 20:06:20
*/
//	GEN (C) Copyright  (All right reserved).
//------------------------------------------------------------------------------------------
	
#ifndef _TEST_H_
#define _TEST_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XDateTime.h"
#include "XTimer.h"
#include "XFSMachine.h"
#include "XSubject.h"
#include "XApplication.h"
#include "XString.h"

#include "DIOStream.h"
#include "DIOURL.h"

#include "Hash.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum TESTXFSMEVENTS
{
	TEST_XFSMEVENT_NONE										= 0 ,
	TEST_XFSMEVENT_INI												,
	TEST_XFSMEVENT_END												,
	
	TEST_LASTEVENT
};


enum TESTXFSMSTATES
{
	TEST_XFSMSTATE_NONE										= 0 ,	
	TEST_XFSMSTATE_INI												,
	TEST_XFSMSTATE_END												,
	
	TEST_LASTSTATE													
};



#define APPLICATION_VERSION								1
#define APPLICATION_SUBVERSION						0
#define APPLICATION_SUBVERSIONERR					0

#define APPLICATION_NAMEAPP			 					__L("Test")
#define APPLICATION_CFGNAMEFILE	 					__L("test")
#define APPLICATION_LOGNAMEFILE	 					APPLICATION_CFGNAMEFILE
#define APPLICATION_ENTERPRISE						__L("Abraham J. Velez")

#define APPLICATION_DIRECTORYMAIN					__L("resources")


#define APPLICATION_LOGSECTIONIDGENERIC		__L("Generic")


#if HW_PC

  #define TEST_PINSCREEN_RESET  			 0
	#define TEST_PINSCREEN_DC  					 0
  #define TEST_PINSPI_MOSI						 1	
  #define TEST_PINSPI_MISO						 4
  #define TEST_PINSPI_CLK							 2
  #define TEST_PINSPI_CS							 3

  #define TEST_PIN_LED1								 5
  #define TEST_PIN_LED2								 6
  #define TEST_PIN_LED3								 7
  #define TEST_PIN_BUTTON						  -1	

#endif


 #if HW_RASPBERRYPI
		
	#define TEST_PINSCREEN_RESET				22
	#define TEST_PINSCREEN_DC  					18
  #define TEST_PINSPI_CLK							16
  #define TEST_PINSPI_MISO						21
  #define TEST_PINSPI_MOSI						19	
  #define TEST_PINSPI_CS							24	

  #define TEST_PIN_LED1								12
  #define TEST_PIN_LED2								16
  #define TEST_PIN_LED3								18
  #define TEST_PIN_BUTTON							22

#endif




enum RASPBERRYPI_MODEL
{
	RASPBERRYPI_MODEL_UNKNOWN						=   0 ,	
	RASPBERRYPI_MODEL_A												,
	RASPBERRYPI_MODEL_B												,
	RASPBERRYPI_MODEL_A_PLUS									,
	RASPBERRYPI_MODEL_B_PLUS									,
	RASPBERRYPI_MODEL_COMPUTERMODULE					,
	RASPBERRYPI_MODEL_B_2											,
	RASPBERRYPI_MODEL_ZERO										,
};




//---- CLASS -------------------------------------------------------------------------------

class XTIME;
class XTIMER;
class XRAND;
class XTHREADCOLLECTED;
class CIPHER;
class DIOSTREAMCONFIG;
class	DIOSTREAM;			
class DIOPROTOCOLCONNEXIONS;
class	DIOATCMDGSM;
class APPLICATIONDATA;


class TEST : public XAPPLICATION , public XFSMACHINE, public XSUBJECT
{
	public:	
																		TEST																	  ();
		virtual												 ~TEST																		();

		bool														Ini 																		();

		bool														InitFSMachine														();
		
		bool														FirstUpdate															();
    
		bool														Update																	();

		bool														End																			();
				

		void														Clean																		();		

		bool														Test_WaitThread													();
		bool														Test_Exceptions													();
		bool														Test_Hash																(HASH* hash, XBUFFER& input, XCHAR* leyend);
		bool														Test_Cipher															(CIPHER* cipher, XBUFFER& input, XCHAR* leyend);						
		bool														Test_Threads														();
		bool														Test_XBuffer1														();
		bool														Test_XBuffer2														();
		bool														Test_XBuffer3														();
		bool														Test_WEBClient													();	
		bool														Test_WEBServerRequest										(DIOWEBSERVERXEVENT* xevent);
		bool														Test_WEBServerSendFile									(DIOWEBSERVER_CONNEXION* connexion, DIOWEBSERVER_REQUEST* request, XSTRING& resource, bool& iscached);					
		bool														Test_WEBServerTag												(DIOWEBSERVERXEVENT* xevent);
		bool														Test_WEBServer													();	
		bool														Test_DIOStreamEnumLocal									();				
		bool														Test_DIOStreamUART											(bool modeserver);
		bool														Test_DIOStreamUARTForMicroReadCommand		(DIOSTREAM*	diostream, XBUFFER& command);
		bool														Test_DIOStreamUARTForMicro							();
		bool														Test_DIOStreamTCPIP											(bool modeserver);
		bool														Test_OBEX																();
		bool														Test_ATCommand													();		
		bool														Test_ClientBTModuleCSR									();
		bool														Test_SSHReverse                         ();
		bool														Test_HASHGeneric												();
		bool														Test_HASHCOMP128												();
		bool														Test_CIPHERSimetric											();
		bool														Test_CIPHERRSA													();
		bool														Test_HASHFile														();
		bool														Test_Ping																();
		bool														Test_DIOStreamTCPIP2										();
		bool														Test_WebScrapers												();
		bool														Test_MACManufactured										();
		bool														Test_Date1															();
		bool														Test_Date2															();
		bool														Test_NTP																();
		bool														Test_Log																();
		bool														Test_Scheduler													();
		bool														Test_Script															();
		bool														Test_OrganicCipherDB										();
		bool														Test_AOSONGSensor												();
		bool														Test_Rele220V														();
		bool														Test_TimeString													();
		bool														Test_EnumWifiRemote											();
		bool														Test_XMPIntegers												();
		bool														Test_ConvertBase64											();
		bool														Test_ReadLDRDigitalIO										();
		bool														Test_RSASignCert												();
		bool														Test_StringFormat												();
		bool														Test_I2CDevices													();
		bool														Test_PiFace															();
		bool														Test_Compress														();
		bool														Test_DynDNS															();
		bool														Test_License														();
		bool														Test_SMTP																();
		bool														Test_LCDPhilipsPCF8833									();
		bool														Test_LCDIlitekILI9341										();
		bool														Test_OLEDSSD1331												();
		bool														Test_DIOStreamUSB											  ();
		bool														Test_MemControl													();
		bool														Test_SystemCommand											();	
		bool														Test_DIOStreamUDP												(bool modeserver);
		bool														Test_DIOProtocolConnexionsManager				(bool modeserver);
		bool														Test_DIOStreamCipher										(bool modeserver);
		bool														Test_DIOProtocol												(bool modeserver);
		bool														Test_GSMATCommand												();
		bool														Test_GSMATCommand2											();
		bool														Test_DIOAlerts													();
		bool														Test_BusPirate													();
		bool														Test_WiiMote														();
		bool														Test_DIOStreamUDPAcknowledge						(bool modeserver);
		bool														Test_DIOAlertServer											();
		bool														Test_DIOCheckInternetConnexion					();
		bool														Test_DIOCheckConnexions									();
		bool														Test_FileTXT														();
		bool														Test_ApplicationUpdate									();
		bool														Test_XSystem														();		
		bool														Test_DatabaseDBF												();
		bool														Test_DatabaseSQL												(XDWORD type);
		bool														Test_XFile															();
		bool														Test_DIOPCap														();
		bool														Test_RPI_RevisionBoard									(RASPBERRYPI_MODEL& model, int& megabytes, float& revision);
		bool														Test_DIOGPIO														();
		bool														Test_Lists															();
		bool														Test_Audio															();
		bool														Test_AVLTree														();
		bool														Test_BinaryMap													();	
		bool														Test_Variants														();
		bool														Test_VariantProtocol										(bool modeserver);
		bool														Test_Map																();
		bool														Test_Vector															();
		bool														Test_DIOStreamTCPIPConnection						();
		bool														Test_Zip																();
		bool														Test_StringProtocol											(bool modeserver);	
		bool														Test_XDebug                             ();
		bool														Test_String															();
		bool														Test_Dictionary													();

			
		void														HandleEvent															(XEVENT* event);	
		static void											ThreadRunFunction												(void* thread);			
			
		XDATETIME*											xdatetime;
		XTIMER*													xtimer;
		XRAND*													xrand;

		XCONSOLE*												xconsole;

		TESTPROTOCOLCONNEXIONSMANAGER*	protocolconnexionmanager;
		CIPHER*													cipher;
		TESTPROTOCOLAPPLICATIONDATA*		protocolapplicationdata;
	
		XTHREADCOLLECTED*								threadwebclient[50];
		int															threadwebactual;
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------

extern TEST* test;

#endif

