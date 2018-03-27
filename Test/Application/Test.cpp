//------------------------------------------------------------------------------------------
//	TEST.CPP
//	
//	Test
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 06/10/2005 20:05:56
//	Last Mofificacion	:	
//	
//	GEN (C) Copyright  (All right reserved).			 
//------------------------------------------------------------------------------------------

	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XPath.h"
#include "XPathsManager.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XRand.h"
#include "XDir.h"
#include "XBER.h"
#include "XLog.h"
#include "XString.h"
#include "XBuffer.h"
#include "XSystem.h"
#include "XScheduler.h"
#include "XFileTXT.h"
#include "XFileCSV.h"
#include "XFileZIP.h"
#include "XFileDBF.h"
#include "XDebug.h"
#include "XThread.h"
#include "XFileHash.h"
#include "XConsole.h"
#include "XApplication.h"
#include "XMPInteger.h"
#include "XEvent.h"
#include "XSubject.h"
#include "XObserver.h"
#include "XPublisher.h"
#include "XLicense.h"
#include "XAVLTree.h"
#include "XBinaryMap.h"

#include "DIOFactory.h"
#include "DIOMACManufactured.h"
#include "DIOGPIO.h"
#include "DIOStreamDevice.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamDeviceBluetooth.h"
#include "DIOStreamDeviceWifi.h"

#include "DIOStreamEnumDevices.h"

#include "DIOStreamUARTConfig.h"
#include "DIOStreamUART.h"

#include "DIOStreamUSBLocalEnumDevices.h"
#include "DIOStreamDeviceUSB.h"
#include "DIOStreamUSBConfig.h"
#include "DIOStreamUSB.h"

#include "DIOStreamUDPLocalEnumServers.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"
#include "DIOStreamUDPAcknowledge.h"

#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"

#include "DIOStreamBluetoothConfig.h"
#include "DIOStreamBluetooth.h"

#include "DIOStreamCipher.h"

#include "DIOATCMDGSM.h"

#include "DIODynDNS.h"
#include "DIOSMTP.h"
#include "DIOWebServer.h"
#include "DIOWebServerXEvent.h"
#include "DIOWebServerQueryString.h"

#include "DIONTP.h"
#include "DIOPing.h"
#include "DIOURL.h"

#include "DIOOBEXPush.h"
#include "DIOCLIClientBTModuleCSR.h"

#include "DIOWebClientXEvent.h"
#include "DIOWebScraper.h"
#include "DIOWebScraperPublicIP.h"
#include "DIOWebScraperGeolocationIP.h"
#include "DIOWebScraperSexName.h"
#include "DIOWebScraperUserAgentID.h"
#include "DIOWebScraperTranslation.h"
#include "DIOWebScraperMACManufacturer.h"
#include "DIOWebScraperWeather.h"

#include "HashCRC32.h"
#include "HashMD5.h"
#include "HashSHA1.h"
#include "HashCOMP128v1.h"
#include "HashSHA2.h"
#include "HashWhirlpool.h"
#include "HashCKS16.h"
#include "HashCRC16.h"

#include "Cipher.h"
#include "CipherDES.h"
#include "Cipher3DES.h"
#include "CipherAES.h"
#include "CipherBlowfish.h"
#include "CipherRSA.h"
#include "CipherFileKeys.h"

#include "ScriptG.h"
#include "ScriptLua.h"
#include "ScriptLibBase.h"

#include "DIOTemHumSensorAM2301.h"
#include "DIOLightSensorLDRAnalog.h"
					
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"
#include "DIOI2CEEprom24XXX.h"
#include "DIOI2CADDAConverterPCF8591.h"
#include "DIOI2CTemHumSensorAM2315.h"
#include "DIOI2CGPIOMCP2317.h"
#include "DIOI2CGPIOPCF8574.h"
#include "DIOI2CPWMControlerPCA9685.h"
#include "DIOI2C6AxisTrackingLSM303DLHC.h"
#include "DIOI2COLEDScreenSSD1306.h"
#include "DIOI2CLightSensorTSL2561.h"

#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"
#include "DIOPiFace.h"
#include "DIOSPILCDScreenPCF8833.h"
#include "DIOSPILCDScreenILI9341.h"
#include "DIOSPIOLEDScreenSSD1331.h"
#include "DIOSPITouchScreenSTMPE610.h"

#include "DIOStreamTCPIP.h"
#include "DIOATCMDGSM.h"

#include "DIOSSHReverse.h"
#include "DIOBackdoor.h"

#include "DIOCheckInternetConnexion.h"
#include "DIOCheckConnexions.h"

#include "DIOAlerts.h"

#include "DIOWebHeader.h"

#include "DIOApplicationUpdateXEvent.h"
#include "DIOApplicationUpdate.h"

#include "DIOProtocol.h"
#include "DIOProtocolConnexionsManager.h"

#include "DIOStringProtocol.h"

#include "DIOBusPirate.h"
#include "DIOWiiMote.h"

#include "DIOPCap.h"
#include "DIOGPIO.h"

#include "TestProtocolApplicationData.h"
#include "TestProtocolConnexionsManager.h"
#include "TestProtocol.h"

#include "DBSQLQuery.h"
#include "DBSQLResult.h"
#include "DBSQLFactory.h"

#include "XList.h"
#include "XSerializable.h"

#include "SNDFactory.h"
#include "SNDElement.h"
#include "SNDSource.h"

#include "CompressManager.h"

#include "Test.h"

#include "XMemory.h"
	

//---- GENERAL VARIABLE --------------------------------------------------------------------
	
TEST* test = NULL;
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  TEST::TEST
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:15:15
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
TEST::TEST() :  XFSMACHINE(0)
{		
	Clean();
}



//-------------------------------------------------------------------
//  TEST::~TEST
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:15:40
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
TEST::~TEST()
{
	Clean();
}


//-------------------------------------------------------------------
//  XAPPLICATION::Create
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 11:02:38
//	
//	@return 			XAPPLICATION* : 
//	*/
//-------------------------------------------------------------------
XAPPLICATION* XAPPLICATION::Create()
{
	test = new TEST();

	return test;
}



//-------------------------------------------------------------------
//  TEST::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:17:31
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool TEST::Ini()
{		
	//-------------------------------------------------------------------------------------------------

	GetApplicationName()->Set(APPLICATION_NAMEAPP);

	//-------------------------------------------------------------------------------------------------

	XSTRING string;
	
	string = APPLICATION_NAMEAPP;			XDEBUG_SETAPPLICATIONNAME(string);																		
	XDEBUG_SETAPPLICATIONVERSION(APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR);
	string = __L("");							  	XDEBUG_SETAPPLICATIONID(string);

	//-------------------------------------------------------------------------------------------------

	if(!XPATHSMANAGER::GetInstance().AdjustRootPathDefault(APPLICATION_NAMEAPP, APPLICATION_DIRECTORYMAIN)) return false;

	XPATHSMANAGER::GetInstance().CreateAllPathSectionOnDisk();

	//----------------------------------------------------------------	-------------------------------

	XDEBUG_SETTARGET(0, XDEBUGCTRLTYPE_NET			, XDEBUG_DEFAULT_NETAIM1);	
	XDEBUG_SETTARGET(1, XDEBUGCTRLTYPE_NET			, XDEBUG_DEFAULT_NETAIM2);	
	XDEBUG_SETTARGET(2, XDEBUGCTRLTYPE_NET			, XDEBUG_DEFAULT_NETAIM3);	
	XDEBUG_SETTARGET(3, XDEBUGCTRLTYPE_NET			, XDEBUG_DEFAULT_NETAIM4);	
	XDEBUG_SETTARGET(4, XDEBUGCTRLTYPE_NET			, XDEBUG_DEFAULT_NETAIM5);	
	
	InitFSMachine();

	SetEvent(TEST_XFSMEVENT_INI);

	return true;
}
  




//-------------------------------------------------------------------
//  TEST::InitFSMachine
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 16:34:54
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool TEST::InitFSMachine()
{
	if(!AddState(	TEST_XFSMSTATE_NONE							, 												 												
								TEST_XFSMEVENT_INI							, TEST_XFSMSTATE_INI						,
								TEST_XFSMEVENT_END							, TEST_XFSMSTATE_END						,
								EVENTDEFEND)) return false;


	if(!AddState(	TEST_XFSMSTATE_INI							, 												 												
								TEST_XFSMEVENT_NONE							, TEST_XFSMSTATE_NONE						,
								TEST_XFSMEVENT_END							, TEST_XFSMSTATE_END						,
								EVENTDEFEND)) return false;


	if(!AddState(	TEST_XFSMSTATE_END							,								
								TEST_XFSMEVENT_NONE							, TEST_XFSMSTATE_NONE						,
								TEST_XFSMEVENT_INI							, TEST_XFSMSTATE_INI						,
								EVENTDEFEND)) return false;

	return true;
}




//-------------------------------------------------------------------
//  TEST::FirstUpdate
/**
//	
//	@author        Abraham J. Velez
//	@version       20/2/2003   16:26:12
//	
//	@return        bool : 
*/
//-------------------------------------------------------------------
bool TEST::FirstUpdate()
{
	if(!xfactory) return false;

	//--------------------------------------------------------------------------------------	
	
	xconsole = xfactory->CreateConsole();
	if(!xconsole) return false;

	xdatetime = xfactory->CreateDateTime();
	if(!xdatetime) return false;

	xtimer = xfactory->CreateTimer();
	if(!xtimer) return false;

	xrand = xfactory->CreateRand();
	if(!xrand) return false;

	cipher = new CIPHERAES();
	if(!cipher) return false;


	protocolapplicationdata = new TESTPROTOCOLAPPLICATIONDATA();
	if(!protocolapplicationdata) return false;

	protocolapplicationdata->SetVersion(1, 22, 33);


	//--------------------------------------------------------------------------------------
	// Params

	bool modeserver = false;

	if(GetExecParams())
		{
			XSTRING* param = (XSTRING*)GetExecParams()->Get(0);	
			if(param)
				{					
					if(!param->Compare(__L("SERVER"), true))  modeserver = true;
				}
		}

	XSTRING string;

	if(modeserver)
		{
			string.Format(__L("%s Server"), APPLICATION_NAMEAPP);			
			XDEBUG_SETAPPLICATIONNAME(string);
		}
 	 else
		{
			XSTRING string;
			
			string.Format(__L("%s Client"), APPLICATION_NAMEAPP);			
			XDEBUG_SETAPPLICATIONNAME(string);
		}

	//--------------------------------------------------------------------------------------


	xconsole->TipicalHeader_Show(2012, APPLICATION_NAMEAPP, APPLICATION_VERSION
																							, APPLICATION_SUBVERSION
																							, APPLICATION_SUBVERSIONERR
																							, APPLICATION_ENTERPRISE);

	xconsole->PrintMessage(__L("\n\n"),0,false,false);
	
	XDEBUG_PRINTCOLOR(1, __L("Init App..."));

	//--------------------------------------------------------------------------------------

	XPATH xpath;
	XPATH xpathgeneric;

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathgeneric);
	xpath.Create(3 , xpathgeneric.Get(), APPLICATION_LOGNAMEFILE,__L(".log"));	

	if(!XLOG::GetInstance().Ini(xpath, (*GetApplicationName()), true)) 
		{
			return false;
		}
			
	XLOG::GetInstance().SetLimit(XLOGTYPELIMIT_SIZE, 500*1000, 10);
	XLOG::GetInstance().SetBackup(true, 3, true);
	XLOG::GetInstance().SetFilters(__L("Status, Generic, Update"), 0x0F);
	

	//--------------------------------------------------------------------------------------

	//if(!Test_Exceptions())																return false;
	//if(!Test_WaitThread())																return false;
	//if(!Test_Threads())																		return false;
	//if(!Test_XBuffer1())																	return false;
	//if(!Test_XBuffer2())																	return false;
	//if(!Test_XBuffer3())																	return false;
	//if(!Test_WEBClient())																	return false;
	//if(!Test_WEBServer())																	return false;
	//if(!Test_DIOStreamEnumLocal())												return false;
	//if(!Test_DIOStreamUART(modeserver))										return false;
	//if(!Test_DIOStreamUARTForMicro())											return false;
	//if(!Test_DIOStreamTCPIP(modeserver))									return false;
	//if(!Test_OBEX())																			return false;
	//if(!Test_ATCommand())																	return false;
	if(!Test_SSHReverse())																return false;
	//if(!Test_ClientBTModuleCSR())													return false;
	//if(!Test_HASHGeneric())																return false;		
	//if(!Test_HASHCOMP128())																return false;
	//if(!Test_CIPHERSimetric())														return false;
	//if(!Test_CIPHERRSA())																	return false;
	//if(!Test_HASHFile())																	return false;
	//if(!Test_Ping())																			return false;
	//if(!Test_DIOStreamTCPIP2())														return false;	
	//if(!Test_WebScrapers())																return false;		
	//if(!Test_MACManufactured())														return false;
	//if(!Test_Date1())																			return false;
	//if(!Test_Date2())																			return false;			
	//if(!Test_NTP())																				return false;		
	//if(!Test_Log())																				return false;
	//if(!Test_Scheduler())																	return false;
	   //if(!Test_Script())																		return false;
	//if(!Test_OrganicCipherDB())														return false;
	//if(!Test_AOSONGSensor())															return false;
	//if(!Test_Rele220V())																	return false;
	//if(!Test_TimeString())																return false;
	//if(!Test_EnumWifiRemote())														return false;
	//if(!Test_XMPIntegers())																return false;
	//if(!Test_ConvertBase64())															return false;		
	//if(!Test_ReadLDRDigitalIO())													return false;
	//if(!Test_RSASignCert())																return false;
	//if(!Test_StringFormat())															return false;
	//if(!Test_I2CDevices())																return false;
	//if(!Test_PiFace())																		return false;
	//if(!Test_Compress())																	return false;
	//if(!Test_DynDNS())																		return false;
	//if(!Test_License())																		return false;
	//if(!Test_SMTP())																			return false;
	//if(!Test_LCDPhilipsPCF8833())													return false;
	//if(!Test_LCDIlitekILI9341())													return false;
	//if(!Test_OLEDSSD1331())																return false;	
	//if(!Test_DIOStreamUSB())															return false;
	//if(!Test_MemControl())																return false;
	//if(!Test_SystemCommand())															return false;
	//if(!Test_DIOStreamUDP(modeserver))										return false;
	//if(!Test_DIOProtocolConnexionsManager(modeserver))		return false;
	//if(!Test_DIOStreamCipher(modeserver))									return false;		
	//if(!Test_GSMATCommand())															return false;
	//if(!Test_GSMATCommand2())															return false;
	//if(!Test_DIOAlerts())																	return false;
	//if(!Test_BusPirate())																	return false;
	//if(!Test_WiiMote())																		return false;
	//if(!Test_DIOStreamUDPAcknowledge(modeserver))					return false;
	//if(!Test_DIOAlertServer())														return false;
	//if(!Test_DIOCheckInternetConnexion())									return false;
	//if(!Test_DIOCheckConnexions())												return false;
	//if(!Test_FileTXT())																		return false;
	//if(!Test_ApplicationUpdate())													return false;
	//if(!Test_XSystem())																		return false;		  	
	//if(!Test_DatabaseDBF())																return false;			
	//if(!Test_DatabaseSQL(DBSQLDATABASE_TYPE_MYSQL))				return false;
	//if(!Test_DatabaseSQL(DBSQLDATABASE_TYPE_POSTGRESQL))	return false;	
	//if(!Test_DatabaseSQL(DBSQLDATABASE_TYPE_SQLITE))			return false;
	//if(!Test_XFile())																			return false;		
	//if(!Test_DIOPCap())																		return false;	
	//if(!Test_DIOGPIO())																		return false;	
	//if(!Test_Lists())																			return false;	
	//if(!Test_Audio())																			return false;	
	//if(!Test_AVLTree())																		return false;
	//if(!Test_BinaryMap())																	return false;
	//if(!Test_Variants())																	return false;
	//if(!Test_VariantProtocol(modeserver))									return false;
	//if(!Test_Map())																				return false;
	//if(!Test_Vector())																		return false;
	//if(!Test_DIOStreamTCPIPConnection())									return false;
	//if(!Test_TCP())																				return false;
	//if(!Test_Zip())																				return false;
	//if(!Test_StringProtocol(modeserver))									return false;
	//if(!Test_XDebug())																		return false;	
	//if(!Test_String())																		return false;

	return true;
}






//-------------------------------------------------------------------
//  TEST::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 16:53:03
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool TEST::Update()
{
  if(GetEvent()==TEST_XFSMEVENT_NONE) // Not new event
		{			
		  switch(GetCurrentState())
				{
					case TEST_XFSMSTATE_NONE 			: break;

					case TEST_XFSMSTATE_INI	 			: if(xconsole->KBHit()) 
																						{ 
																							SetExitStatus( XAPPLICATIONEXITTYPE_BYUSER); 
																							SetEvent(TEST_XFSMEVENT_END);																							
																						}
																					
					case TEST_XFSMSTATE_END	 			:	break;	
					
				}												
		}
	 else //  New event
	  {
			if(GetEvent()<TEST_LASTEVENT)
				{
					CheckTransition();

					switch(GetCurrentState())
						{
							case TEST_XFSMSTATE_NONE 	: break;

							case TEST_XFSMSTATE_INI	 	: break;		

							case TEST_XFSMSTATE_END		:	break;
						}
				}
		}

	return true;
}



/*-------------------------------------------------------------------
//  TEST::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/05/2012 19:02:07
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::End()
{		
	
	xconsole->PrintMessage(__L(" "),1,false,true);
	xconsole->PrintMessage(__L("Saliendo de la aplicacion"),1,true,true);
	
  SetCurrentState(TEST_XFSMSTATE_END);

	//--------------------------------------------------------------------------------------

	XLOG::GetInstance().End();

	//--------------------------------------------------------------------------------------

	delete protocolapplicationdata;

	delete cipher;

	//--------------------------------------------------------------------------------------

	if(xrand) 
		{
			xfactory->DeleteRand(xrand);
			xrand = NULL;
		}

	if(xtimer) 
		{
			xfactory->DeleteTimer(xtimer);
			xtimer = NULL;
		}
	
	if(xdatetime) 
		{
			xfactory->DeleteDateTime(xdatetime);
			xdatetime = NULL;
		}

	xconsole->PrintMessage(__L("\n\n"),0,false,false);
	xfactory->DeleteConsole(xconsole);
	xconsole = NULL;
	
	return true;
}




//-------------------------------------------------------------------
//  TEST::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/11/2002 11:49:57
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void TEST::Clean()
{
	xdatetime									= NULL;
	xtimer										= NULL;	
	xrand											= NULL;

	protocolconnexionmanager	= NULL;
	cipher							      = NULL;
	protocolapplicationdata		= NULL;
}




/*-------------------------------------------------------------------
//	TEST::Test_Exceptions
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/04/2017 10:57:59
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
void A()
{
	int* a=0;

	*&(a[1])=1;

}

void B()
{
	A();
}

void C()
{
	B();
}

bool TEST::Test_Exceptions()
{
	C();
	return true;
}





/*-------------------------------------------------------------------
//  TEST::Test_Hash
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/12/2013 14:01:23
//	
//	@return 			bool : 
//	@param				HASH : 
//  @param				input : 
//  @param				leyend : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Hash(HASH* HASH, XBUFFER& input, XCHAR* leyend)
{
	if(!HASH) return false;
	
	XSTRING result;

	HASH->Do(input);		

	HASH->GetResultString(result);
			
	xconsole->Printf(__L("%-12s : %s\n"),leyend, result.Get());

	return true;
}



/*-------------------------------------------------------------------
//  TEST::Test_Cipher
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/12/2013 14:01:45
//	
//	@return 			bool : 
//	@param				cipher : 
//  @param				input : 
//  @param				leyend : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Cipher(CIPHER* cipher, XBUFFER& input, XCHAR* leyend)
{
	if(!cipher) return false;
	
	XSTRING result;

	bool status = cipher->Cipher(input);
	
	xconsole->Printf(__L("%-12s : "), leyend);

	if(!status)
		{
			xconsole->Printf(__L("Error!\n"));	
			return false;
		}

	XBUFFER* resultcipher = cipher->GetResult();
	if(resultcipher)
		{
			for(int c=0;c<(int)(resultcipher->GetSize()-resultcipher->Padding_GetSize());c++)
				{
				  xconsole->Printf(__L("%02X"), resultcipher->GetByte(c));
				}
		}

	xconsole->Printf(__L("\n"));

	/*
	xbuffer.Delete();
	xbuffer.Add((*cipher->GetResult()));

	cipher->Uncipher(xbuffer);

	resultcipher = cipher->GetResult();
	if(resultcipher)
		{
			for(int c=0;c<(int)(resultcipher->GetSize());c++)
				{
					console->Printf(__L("%02X"), resultcipher->GetByte(c));
				}
		}
	*/

	return true;
}




/*-------------------------------------------------------------------
//  TEST::Test_Threads
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:47:25
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Threads()
{
  memset(threadwebclient,0,sizeof(XTHREAD*)*50);

	for(int c=0;c<1;c++)
		{			
			threadwebactual = c;
			threadwebclient[c] = CREATEXTHREAD(XTHREADGROUPID_APPOWNER, __L("TEST::Test_Threads"), ThreadRunFunction ,(void*)this);						
			if(threadwebclient[c]) threadwebclient[c]->Ini();
		}

	xsleep->MilliSeconds(150000);	

	for(int c=0;c<1;c++)
		{			
			DELETEXTHREAD(XTHREADGROUPID_APPOWNER, threadwebclient[c]);
		}

	return true;
}

void iterationthreadfunction(void * p)
{
		TEST* test = (TEST*)p;

		for (int e = 0; e < 100; e++)
				test->xconsole->Printf(__L("%d\n"), e);
}

bool TEST::Test_WaitThread()
{
		XTHREAD* thread = xfactory->CreateThread((XTHREADGROUPID)0, __L("testWait"), iterationthreadfunction, this);

		thread->Ini();
		for(int c = 0; c < 6; c++)
			{
				if(c==3) thread->Run(false);
				xsleep->Seconds(1);
			}
		thread->End();
		//thread->WaitToEnd();

		xconsole->Printf(__L("end"));
		return true;
}





/*-------------------------------------------------------------------
//  TEST::Test_XBuffer1
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:51:19
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_XBuffer1()
{
	XBUFFER xbuffer;
	XBUFFER xbuffer2;

	for(int c=0;c<16;c++)
		{
			xbuffer.Add((XWORD)0x03AA);
		}
	
	xbuffer.AdjustToNBits(10);

  xconsole->Printf(__L("\n"));

	for(int c=0;c<(int)xbuffer.GetSize();c+=2)
		{
			xconsole->Printf(__L("%1X%02X "),xbuffer.GetByte(c), xbuffer.GetByte(c+1));
		}

	xconsole->Printf(__L("\n\n"));

	for(int c=0;c<(int)xbuffer2.GetSize();c++)
		{
			xconsole->Printf(__L("%02X "),xbuffer2.GetByte(c));
		}

	xconsole->Printf(__L("------------"));

	return true;	
}



/*-------------------------------------------------------------------
//  TEST::Test_XBuffer2
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:51:13
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_XBuffer2()
{	
	XBUFFER	data;
	XWORD		testa1  = 0x0155; 
	XWORD		testa2  = 0x01AA; 
				
	for(int c=0;c<18;c++)
		{
			data.Add((XWORD)testa1);
			data.Add((XWORD)testa2);
	  }
	
	data.AdjustToNBits(9);

	return true;
}
	


/*-------------------------------------------------------------------
//	TEST::Test_XBuffer3
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			26/04/2016 9:33:09
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_XBuffer3()
{	
	XDWORD value    = 12345;
	XDWORD BCDvalue = (XDWORD)DWORDToBCD(value);

	XBUFFER	data;

	data.Add((XBYTE)0xFF);
	data.Add((XBYTE)0xFF);
	data.Add((XBYTE)0xFF);
	data.Add((XBYTE)0xFF);
	data.Add((XBYTE)0xFF);
	data.Add((XBYTE)0xFF);
	data.Add((XBYTE)0xFF);
	data.Add((XBYTE)0xFF);

	XDWORD number = data.DecodeBCD(0, data.GetSize());


	return true;
}
	



/*-------------------------------------------------------------------
//  TEST::Test_WEBClient
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/12/2013 14:02:20
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_WEBClient()
{	
	DIOWEBCLIENT  webclient;
	DIOURL			  url;
	XBUFFER				webpage;
	XSTRING				handle;
	
	SubscribeEvent(DIOWEBCLIENTXEVENTTYPE_OPENWEB				, &webclient);
	SubscribeEvent(DIOWEBCLIENTXEVENTTYPE_WRITEHEADER		, &webclient);
	SubscribeEvent(DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA	, &webclient);
	SubscribeEvent(DIOWEBCLIENTXEVENTTYPE_READHEADER		, &webclient);
	SubscribeEvent(DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK	, &webclient);
	SubscribeEvent(DIOWEBCLIENTXEVENTTYPE_CLOSEWEB			,	&webclient);

	while(1)
		{
		//	url = __L("http://192.168.1.3/@connecttcpip?address=82.130.213.31;port=40000;addressfield=11393;addressPM=1;keyPM=7;");	
			url = __L("http://www.google.es");
			webclient.SetPort(80);

			if(webclient.Get(url,webpage))
				{			
					handle.Set(webpage.Get(), webpage.GetSize());
					XDEBUG_PRINTCOLOR(0,__L("recibir debug es muy mainstream"));
					xconsole->Print(handle.Get());
					//break;
					/*
					int error = !handle.Compare(__L("ERROR"));
					if(error)
						{
							url.Format(__L("http://192.168.1.3/@readinstantvalues?handle=%s;"), handle.Get());	
							if(webclient.Get(url,webpage))
								{							
							
								}

							url.Format(__L("http://192.168.1.3/@disconnecttcpip?handle=%s;"), handle.Get());	
							if(webclient.Get(url,webpage))
								{							
							
								}					
						}			
					 else
					  {
							xconsole->Printf(__L("Error!!!!\n"));
						}
						*/
				}
		}
	
	UnSubscribeEvent(DIOWEBCLIENTXEVENTTYPE_OPENWEB				, &webclient);
	UnSubscribeEvent(DIOWEBCLIENTXEVENTTYPE_WRITEHEADER		, &webclient);
	UnSubscribeEvent(DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA	, &webclient);
	UnSubscribeEvent(DIOWEBCLIENTXEVENTTYPE_READHEADER		, &webclient);
	UnSubscribeEvent(DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK	, &webclient);
	UnSubscribeEvent(DIOWEBCLIENTXEVENTTYPE_CLOSEWEB			,	&webclient);
	
	return true;
}




/*-------------------------------------------------------------------
//	TEST::WEBServerRequest
*/	
/**	
//	
//	
//	 
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/06/2016 10:52:52
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_WEBServerRequest(DIOWEBSERVERXEVENT* xevent)
{
	if(!xevent)	return false;	
	
	DIOWEBSERVER*           server		= xevent->GetServer();
	DIOWEBSERVER_CONNEXION* connexion = xevent->GetConnexion();
	DIOWEBSERVER_REQUEST*   request   = xevent->GetRequest();

	if(!server)		 return false;
	if(!connexion) return false;
	if(!request)	 return false;

	bool isoutputHTML = true;

	XSTRING* resource = request->GetResource();
	if(!resource) return false;
	
	
	DIOWEBSERVER_HEADER				webserverheader;
	DIOURL										resourceconv;
	bool											isindexpage = (!resource->Compare(__L("/"),true))?true:false;
	bool											nparams;
	XSTRING										resourceout;
	XSTRING										login;
	XSTRING										password;	
	DIOWEBSERVERQUERYSTRING		querystring;		
	bool											status = false;

	resourceconv.Set(resource->Get());
	resourceconv.DecodeUnsafeChars();
			
	nparams = querystring.GetParamsFromURL(resourceconv.Get())?true:false;

	bool iscached;

	if(nparams || isindexpage)
		{	
			XSTRING remoteIP;
			XSTRING	browser;
			XSTRING	SO;			
			
			connexion->GetDIOStream()->GetClientIP()->GetXString(remoteIP);
			XSTRING indexpage("index.html");
			status = Test_WEBServerSendFile(connexion, request, indexpage, iscached);								
		}
 	 else
		{
			status = Test_WEBServerSendFile(connexion, request, (*resource), iscached);																
		}
		
	return true;
}




/*-------------------------------------------------------------------
//	TEST::WEBServerSendFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/08/2016 18:17:53
//	
//	@return 			bool : 
//
//  @param				connexion : 
//  @param				request : 
//  @param				resource : 
//  @param				iscached : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_WEBServerSendFile(DIOWEBSERVER_CONNEXION* connexion, DIOWEBSERVER_REQUEST* request, XSTRING& resource, bool& iscached)
{	
	XPATH								xpathfile;	
	DIOWEBSERVER_HEADER	webserverheader;
	XSTRING							string;			
	HASHMD5							hashmd5;
	bool								status = false;

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_WEB, xpathfile);
	xpathfile.Slash_Add();
	xpathfile += resource;	

	iscached = false;

	if(hashmd5.Do(xpathfile))
		{
			XSTRING entitytag;
			int			sizefile = -1;

			hashmd5.GetResultString(entitytag);

			if(request->GetIfNoneMatch()->Compare(entitytag))
				{			
					int sizefile;
					XFILE_GETFILESIZE(xpathfile, sizefile)
					
					XSTRING extension;
					xpathfile.GetExt(extension); 
					
					webserverheader.Create(connexion->GetServer(), connexion, request, &resource, 1, 1, DIOWEBHEADER_RESULT_OK, -1, &entitytag, connexion->GetServer()->IsPlayablePage(extension));					
					webserverheader.Write(connexion->GetDIOStream(), 5);

					status = connexion->SendFile(xpathfile);															
				}
			 else
				{
					webserverheader.Create(connexion->GetServer(), connexion, request, &resource, 1, 1, DIOWEBHEADER_RESULT_NOTMODIFIED, request->GetSize());
					webserverheader.Write(connexion->GetDIOStream(), 5);									

					iscached = true;

					status = true;
				}

		} else status = false;


	return status;
}





/*-------------------------------------------------------------------
//	TEST::Test_WEBServerTag
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/08/2016 18:33:30
//	
//	@return 			bool : 
//
//  @param				xevent : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_WEBServerTag(DIOWEBSERVERXEVENT* xevent)
{
	xevent->GetOutput()->Set(__L("<!-- TAG REMOVED -->"));
	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_WEBServer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/06/2016 15:53:11
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_WEBServer()
{
	DIOWEBSERVER*		webserver	=	NULL;
	XSTRING					string;
	XSTRING					stringresult;
	bool						status		= false;
	XSTRING					autoaddress;
	int							port			=8080;


	

	webserver = new DIOWEBSERVER(); 
	xconsole->PrintMessage(__L("Activando servidor web  "), 1, true, false);	
	if(webserver) status = webserver->Ini(port, DIOWEBSERVER_DEFAULTTIMEOUTSERVERPAGE, &autoaddress);									
		
	stringresult = (status)?__L("Ok."):__L("ERROR!"); 		
	xconsole->PrintMessage(stringresult.Get(), 0, false, true);

	if(status)
	{			
	SubscribeEvent(DIOWEBSERVERXEVENTTYPE_REQUEST	    , (XSUBJECT*)webserver);
	SubscribeEvent(DIOWEBSERVERXEVENTTYPE_TAGFOUND	  , (XSUBJECT*)webserver);
	}
	xconsole->PrintMessage(__L("Press a key to exit"), 0, false, true);	

	while (!xconsole->KBHit());
	delete(webserver);

	
	

	return status;
}




/*-------------------------------------------------------------------
//  TEST::Test_UARTDIOStream
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 9:31:00
//	
//	@return 			bool : 
//	@param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamEnumLocal()
{
	DIOSTREAMENUMDEVICES* enumdevices;

	enumdevices = diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_UART_LOCAL);
	if(enumdevices)
		{
			enumdevices->Search();

			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("UART Ports:"));

			for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
				{				
					DIOSTREAMDEVICE* device = (DIOSTREAMDEVICE*)enumdevices->GetDevices()->Get(c);
					if(device) device->DebugPrintInfo(xconsole);										
				}

			diofactory->DeleteStreamEnumDevices(enumdevices);			
		}


	enumdevices = diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_USB_LOCAL);
	if(enumdevices) 
		{
			enumdevices->Search();
	
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));	
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("USB enum:"));	

			for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
				{				
					DIOSTREAMDEVICE* device = (DIOSTREAMDEVICE*)enumdevices->GetDevices()->Get(c);
					if(device) device->DebugPrintInfo(xconsole);				
				}
	
			diofactory->DeleteStreamEnumDevices(enumdevices);			
		}

	
	enumdevices = diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_IP_LOCAL);
	if(enumdevices)
		{
			enumdevices->Search();
			
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));			
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Local Ethernet output:"));

			for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
				{				
					DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetDevices()->Get(c);
					if(device) device->DebugPrintInfo(xconsole);										
				}

			diofactory->DeleteStreamEnumDevices(enumdevices);				
		}

	enumdevices = diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL);
	if(enumdevices)
		{
			enumdevices->Search();

			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Local Bluetooth modules:"));

			for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
				{					
					DIOSTREAMDEVICEBLUETOOTH* device = (DIOSTREAMDEVICEBLUETOOTH*)enumdevices->GetDevices()->Get(c);
					if(device) device->DebugPrintInfo(xconsole);							
				}

			diofactory->DeleteStreamEnumDevices(enumdevices);			
		}
		
	xconsole->Printf(__L("\n End Enumeration ...\n\n"));

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_TCPIPDIOStream
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/10/2014 17:06:45
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamUART(bool modeserver)
{		
	int uartspeed[] = { 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 };
	int c=6;

	//for(int c=0; c<(sizeof(uartspeed)/sizeof(int)); c++)
		{
			DIOSTREAMUARTCONFIG* diostreamcfg;
			DIOSTREAMUART*			 diostream;
	
			diostreamcfg = new DIOSTREAMUARTCONFIG();
			if(diostreamcfg)
				{	
																
					#ifdef HW_RASPBERRYPI	
					diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyACM0"));
					//diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyUSB0"));						
					//diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyAMA0"));	
					//diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyS0"));											
					#endif		
					
					diostreamcfg->SetPort(8);		
					//diostreamcfg->SetBaudRate(115200);
					diostreamcfg->SetBaudRate(9600);
					diostreamcfg->SetDataBits(DIOSTREAMUARTDATABIT_8);
					diostreamcfg->SetParity(DIOSTREAMUARTPARITY_NONE);
					diostreamcfg->SetStopBits(DIOSTREAMUARTSTOPBITS_ONE);
					diostreamcfg->SetFlowControl(DIOSTREAMUARTFLOWCONTROL_NONE);	

					diostream = (DIOSTREAMUART*)diofactory->CreateStreamIO(diostreamcfg);
					if(diostream)
						{				
							if(diostream->Open())
								{							
									xconsole->Print(__L("\nOpen conexion...\n"));

									while(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
										{
											if(xconsole->KBHit()) break;
											xsleep->MilliSeconds(50);
										}

									xconsole->Printf(__L("\nOpen (%d bauds) ...\n"), uartspeed[c]);

									bool		weakbit = true;							
									XBUFFER xbuffer;

									while(1)
										{																								
											if(xconsole->KBHit())
												{
													XBYTE character = xconsole->GetChar();
													if(character == 27) break;
													if(!character)			break;

													xbuffer.Add(character);

													if(diostream->Write(xbuffer)) diostream->WaitToFlushOutXBuffer(5);	
													
													xbuffer.Delete();												

													//xconsole->Printf(__L("%02x"),  character);
													//xconsole->Printf(__L("Send data...\n"));
												}

											int size = diostream->GetInXBuffer()->GetSize();
											if(size)
												{
													xbuffer.Resize(size);

													if(diostream->Read(xbuffer))
														{	
															for(XDWORD c=0; c<xbuffer.GetSize(); c++)
																{
																	xconsole->Printf(__L("%c"), xbuffer.GetByte(c));			
																}																			
														}			

													xbuffer.Delete();
												}												
										}

									xconsole->Print(__L("\nClose...\n"));

									diostream->Close();
								}
					
							diofactory->DeleteStreamIO(diostream);
						}
							
					delete diostreamcfg;
				}
		}

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_DIOStreamUARTForMicro
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/01/2017 11:37:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/

bool TEST::Test_DIOStreamUARTForMicroReadCommand(DIOSTREAM*	diostream, XBUFFER& command)
{
	#define IDLITTLEPROTOCOL 0xFE

	if(!diostream) return false;

	if(diostream->GetInXBuffer()->GetSize()<2) return false;
	 					
	XBYTE ID    = 0;
	XBYTE size  = 0;

	ID		= diostream->GetInXBuffer()->GetByte(0);
	size	= diostream->GetInXBuffer()->GetByte(1);

	if(ID != IDLITTLEPROTOCOL) return false;
	if(!size)									 return false;

	if(!diostream->WaitToFilledReadingBuffer(size, 4)) return false;

	command.Resize(size);
	
	if(diostream->Read(command) != size) return false;
																							 
	return true;
}


//------------------------------------------------------------------

bool TEST::Test_DIOStreamUARTForMicro()
{		
	DIOSTREAMUARTCONFIG* diostreamcfg;
	DIOSTREAMUART*			 diostream;
	XBUFFER              xbuffer;
	int									 port = 0;
	
	diostreamcfg = new DIOSTREAMUARTCONFIG();
	if(!diostreamcfg) return false;

	#ifdef HW_RASPBERRYPI	



	XSTRING* param = (XSTRING*)GetExecParams()->Get(0);	
	if(param)
		{				
			diostreamcfg->GetLocalDeviceName()->Set(param->Get());
		}
	 else diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyACM0"));

	 xconsole->Printf(__L("\nDevice to open: %s\n"), diostreamcfg->GetLocalDeviceName()->Get());


	//diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyUSB0"));						
	//diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyAMA0"));	
	//diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyS0"));											
	#endif		

	#ifdef HW_PC
	XSTRING* param = (XSTRING*)GetExecParams()->Get(0);	
	if(param) port = param->ConvertToInt();		

	xconsole->Printf(__L("\nDevice to open: COM%d\n"), port);

	#endif
					
	diostreamcfg->SetPort(port);		
	diostreamcfg->SetBaudRate(115200);
	diostreamcfg->SetDataBits(DIOSTREAMUARTDATABIT_8);
	diostreamcfg->SetParity(DIOSTREAMUARTPARITY_NONE);
	diostreamcfg->SetStopBits(DIOSTREAMUARTSTOPBITS_ONE);
	diostreamcfg->SetFlowControl(DIOSTREAMUARTFLOWCONTROL_NONE);	

	diostream = (DIOSTREAMUART*)diofactory->CreateStreamIO(diostreamcfg);
	if(diostream)
		{				
			if(diostream->Open())
				{							
					XRAND* xrand = xfactory->CreateRand();
					if(xrand)
						{
							xconsole->Printf(__L("\nOpen conexion...\n"));

							while(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
								{
									if(xconsole->KBHit()) break;
									xsleep->MilliSeconds(50);
								}

							while(!xconsole->KBHit())
								{			
									XBYTE size = 0;

									// --- Send -----------------------

									size = xrand->Between(1, 10);

									xbuffer.Delete();	

									xbuffer.Add((XBYTE)IDLITTLEPROTOCOL);	
									xbuffer.Add((XBYTE)(size+2));	

									for(int c=0; c<size; c++)
										{
											 xbuffer.Add((XBYTE)xrand->Max(255));	
										}
									
									diostream->Write(xbuffer);
																											
									// --- Read -----------------------
																		
									while(1)
										{ 
											if(Test_DIOStreamUARTForMicroReadCommand(diostream, xbuffer))
												{
													xconsole->Printf(__L("Size %02d: "), xbuffer.GetSize());
													
													for(XDWORD c=0; c<xbuffer.GetSize(); c++)
														{
															xconsole->Printf(__L("%02X "), xbuffer.GetByte(c));			
														}																								

													xconsole->Printf(__L("\n"));	

													break;
												} 
										}
																														
								}
							
						xfactory->DeleteRand(xrand);	
					}

				xconsole->Print(__L("\nClose...\n"));
				diostream->Close();
			}
					
			diofactory->DeleteStreamIO(diostream);
		}
							
	delete diostreamcfg;

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_DIOStreamTCPIP
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/10/2014 17:06:17
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamTCPIP(bool modeserver)
{	
	DIOSTREAMTCPIPCONFIG * diostreamcfg;
	DIOSTREAMTCPIP*				 diostream;
	
	diostreamcfg = new DIOSTREAMTCPIPCONFIG(); 
	if(diostreamcfg)
		{
			xconsole->Printf(__L("Modo Servidor: %s\n"), modeserver?__L("Si"):__L("No"));

			diostreamcfg->SetMode(modeserver?DIOSTREAMMODE_SERVER:DIOSTREAMMODE_CLIENT);				
			diostreamcfg->SetRemotePort(4000);

			diostream = (DIOSTREAMTCPIP*)diofactory->CreateStreamIO( diostreamcfg);
			if(diostream)
				{
					DIOSTREAMUDPLOCALENUMSERVERS* localenumservers = new DIOSTREAMUDPLOCALENUMSERVERS;
					if(localenumservers)
						{
							localenumservers->SetID(0xFAAFFAFA);
							localenumservers->SetPort(4001);

							if(diostreamcfg->IsServer())
								{
									diostream->SetEnumServers(localenumservers);			
									
									xconsole->Printf(__L("Listening on port: %d"),diostreamcfg->GetRemotePort());

									if(diostream->Open())
										{
											
											XSTRING line;

											while(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
											{
												if(xconsole->KBHit()) break;

												xsleep->MilliSeconds(50);
											}

											xconsole->Print(__L("Open Server...\n"));

											while(!xconsole->KBHit())
												{
													diostream->ReadStr(line);
													if(!line.IsEmpty()) 
														{	
															xconsole->Printf(__L("< Line: %s \n"), line.Get());
															break;
														}
												}

											xconsole->Print(__L("Close...\n"));

											diostream->Close();
										}
								}
							 else
							  {
									localenumservers->Search();
																			
									while(localenumservers->IsSearching())
										{
											if(xconsole->KBHit()) break;

											xsleep->MilliSeconds(50);
										}	

									localenumservers->StopSearch(true);

									if(localenumservers->AreDevicesAvailable())
										{
											DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)localenumservers->GetDevices()->Get(0);
											if(device) 
												{
													XSTRING IPstring;

													device->GetIP()->GetXString(IPstring);

													xconsole->Printf(__L("Found Server in: %s \n"), IPstring.Get());
													diostreamcfg->GetRemoteURL()->Set(IPstring.Get());												
												
													if(diostream->Open())
														{
															XSTRING line;

															while(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
															{
																if(xconsole->KBHit()) break;

																xsleep->MilliSeconds(50);
															}

															xconsole->Print(__L("Open Client...\n"));

															line = __L("Hola radiola\n");
									
															diostream->WriteStr(line.Get());
									
															xconsole->Printf(__L("> Line: %s \n"), line.Get());

															xconsole->Print(__L("Close...\n"));

															xsleep->MilliSeconds(1000);

															diostream->Close();

														}
												}
										} 									 
								}

							delete localenumservers;
						}
					
					diofactory->DeleteStreamIO(diostream);
				}

			delete diostreamcfg;
		}
		
	return true;
}
	
	



/*-------------------------------------------------------------------
//  TEST::Test_OBEX
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 9:29:25
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_OBEX()
{
	xconsole->Printf(__L("Inicializando conexion...\n"));
	
	DIOSTREAMENUMDEVICES*			 enumdeviceslocal	 = NULL;
	DIOSTREAMENUMDEVICES*			 enumdevicesremote = NULL;
	DIOSTREAMDEVICEBLUETOOTH*  devicelocal			 = NULL;
	DIOSTREAMDEVICEBLUETOOTH*  deviceremote			 = NULL;

	enumdeviceslocal = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL);
	if(enumdeviceslocal)
		{					
			enumdeviceslocal->Search();

			if(enumdeviceslocal->GetDevices()->GetSize())
				{
					devicelocal = (DIOSTREAMDEVICEBLUETOOTH*)enumdeviceslocal->GetDevices()->Get(0);
					devicelocal->DebugPrintInfo(xconsole);	
				}
			 else devicelocal = NULL;

			if(devicelocal)
				{
					enumdevicesremote = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE);
					if(enumdevicesremote)
						{					
							enumdevicesremote->Search();

							DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));
							DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Remote Bluetooth modules:"));
			
							xtimer->Reset();
			
							while(enumdevicesremote->IsSearching())
								{
									xsleep->MilliSeconds(10);
								}
					
							for(int c=0;c<(int)enumdevicesremote->GetDevices()->GetSize();c++)
								{					
									DIOSTREAMDEVICEBLUETOOTH* devicesearch = (DIOSTREAMDEVICEBLUETOOTH*)enumdevicesremote->GetDevices()->Get(c);
									if(devicesearch) 
										{												
											if(!devicesearch->GetName()->Compare(__L("Nexus 5")))
												{																																		
													deviceremote = devicesearch;
													break;
												} 											
										}
								}
			
							if(deviceremote)		
								{					
									DIOSTREAMBLUETOOTHCONFIG* diostreamcfg = new DIOSTREAMBLUETOOTHCONFIG();
									if(diostreamcfg)
										{	
											/*
											DIOMAC mac;
											XBYTE  addrmac[] = { 0x00, 0x12, 0xD1, 0x6C, 0x80, 0x79  };

											mac.Set(addrmac);
							
											diostreamcfg->GetRemoteMAC()->Set(mac.Get());
											diostreamcfg->SetRemoteChannel(9);
											*/
											
											diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
											diostreamcfg->GetLocalMAC()->Set(devicelocal->GetMAC()->Get());																		
											diostreamcfg->GetRemoteMAC()->Set(deviceremote->GetMAC()->Get());						
											diostreamcfg->SetRemoteChannel(deviceremote->GetChannelObexPush());
											//diostreamcfg->GetPIN()->Set(__L("1234"));							

											DIOSTREAMBLUETOOTH* diostream = (DIOSTREAMBLUETOOTH*)diofactory->CreateStreamIO( diostreamcfg);
											if(diostream)
												{										
													DIOOBEXPUSH* obexpush = new DIOOBEXPUSH(diostream, xsystem->HardwareUseLittleEndian());
													if(obexpush)
														{
															if(obexpush->Ini(15))
																{
																	XPATH		xpath;
																	XSTRING name;
																	XDWORD  ID;
																																					
																	xconsole->Printf(__L("Conectada...\n"));
																																				
													
																	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
																	xpath += __L("Lord_Vader.jpg");												
																	name  = __L("Lord_Vader.jpg");

																	ID = obexpush->GenerateFileID(xpath);

																	obexpush->SendFileList_AddFile(xpath, name, ID);

																	obexpush->SendFiles(900);
																	
																	while(obexpush->IsSending())
																		{
																			if(xconsole->KBHit()) break;
																			xsleep->MilliSeconds(10);
																		}
													
																	obexpush->End();											
																}		

															delete obexpush;
														}

													diofactory->DeleteStreamIO(diostream);
												}

											delete diostreamcfg;
										}
								}

							diofactory->DeleteStreamEnumDevices(enumdevicesremote);			
						}
				}

			diofactory->DeleteStreamEnumDevices(enumdeviceslocal);			
		}

	xconsole->Printf(__L("Terminada conexion OBEX.\n"));

	return true;
}	




/*-------------------------------------------------------------------
//  TEST::Test_ATCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 9:34:10
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
/*
bool TEST::Test_ATCommand()
{
	DIOSTREAMENUMDEVICES*			 enumdeviceslocal	 = NULL;
	DIOSTREAMENUMDEVICES*			 enumdevicesremote = NULL;
	DIOSTREAMDEVICEBLUETOOTH*  devicelocal			 = NULL;
	DIOSTREAMDEVICEBLUETOOTH*  deviceremote			 = NULL;

	enumdeviceslocal = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL);
	if(enumdeviceslocal)
		{					
			enumdeviceslocal->Search();

			if(enumdeviceslocal->GetDevices()->GetSize())
				{
					devicelocal = (DIOSTREAMDEVICEBLUETOOTH*)enumdeviceslocal->GetDevices()->Get(0);
					devicelocal->DebugPrintInfo(xconsole);	
				}
			 else devicelocal = NULL;

			if(devicelocal)
				{
					enumdevicesremote = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE);
					if(enumdevicesremote)
						{					
							enumdevicesremote->Search();

							DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));
							DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Remote Bluetooth modules:"));
			
							xtimer->Reset();
			
							while(enumdevicesremote->IsSearching())
								{
									xsleep->MilliSeconds(10);
								}
					
							for(int c=0;c<(int)enumdevicesremote->GetDevices()->GetSize();c++)
								{					
									DIOSTREAMDEVICEBLUETOOTH* devicesearch = (DIOSTREAMDEVICEBLUETOOTH*)enumdevicesremote->GetDevices()->Get(c);
									if(devicesearch) 
										{												
											if(!devicesearch->GetName()->Compare(__L("Nexus 5")))
												{																																		
													deviceremote = devicesearch;
													break;
												} 											
										}
								}

							if(deviceremote)				
								{					
									DIOSTREAMBLUETOOTHCONFIG* diostreamcfg = new DIOSTREAMBLUETOOTHCONFIG();
									if(diostreamcfg)
										{			
											
											//DIOMAC mac;
											//XBYTE  addrmac[] = { 0x00, 0x12, 0xD1, 0x6C, 0x80, 0x79  };

											//mac.Set(addrmac);

											
							
											//diostreamcfg->GetRemoteMAC()->Set(mac.Get());
											//diostreamcfg->SetRemoteChannel(2);
											

											//DIOSTREAMDEVICEBLUETOOTH_NOCHANNEL

											diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
											diostreamcfg->GetLocalMAC()->Set(devicelocal->GetMAC()->Get());																		
											diostreamcfg->GetRemoteMAC()->Set(deviceremote->GetMAC()->Get());						
											diostreamcfg->SetRemoteChannel(deviceremote->GetChannelDialUpNetworking());
											
											diostreamcfg->GetPIN()->Set(__L("1234"));

											DIOSTREAMBLUETOOTH* diostream = (DIOSTREAMBLUETOOTH*)diofactory->CreateStreamIO( diostreamcfg);
											if(diostream)
												{																		 									
													DIOATCMDGSM* dioatcmdgsm =  new DIOATCMDGSM(diostream);
													if(!dioatcmdgsm) return false;
					
													xconsole->PrintMessage(__L(" Comprobando dispositivo... "),1,false,false);

													if(dioatcmdgsm->Ini(15, true, true))
														{																	
															xconsole->PrintMessage(__L("\r Dispositivo comandos AT accesible..."),1,false,true);
															xconsole->PrintMessage(__L(""),0,false,true);

															XSTRING string;
															XSTRING value;

															for(int c=0;c<10;c++)
																{
																	string.Empty();
							
																	switch(c)
																		{
																			case  0 : if(dioatcmdgsm->GetManufactured(value)==DIOATCMD_ERROR_NONE)					string.Format(__L("Fabricante             : %s"),value.Get());   
																								break;

																			case  1 : if(dioatcmdgsm->GetModel(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("Modelo de terminal     : %s"),value.Get());   
																								break;
											
																			case  2 : if(dioatcmdgsm->GetIMEI(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("IMEI                   : %s"),value.Get());   
																								break;

																			case  3 : if(dioatcmdgsm->GetVersion(value)==DIOATCMD_ERROR_NONE)								string.Format(__L("Version                : %s"),value.Get());   
																								break;	
														
																			case  4 : { int RSSI = 0;
																									int BER  = 0;
																			
																									if(dioatcmdgsm->GetSignalQuality(RSSI,BER)==DIOATCMD_ERROR_NONE)		string.Format(__L("Calidad de senal       : (RSSI %d)  (BER %d)"),RSSI,BER);																																							
																								}
																								break;		

																			case  5 : { bool avaible;

																									if(dioatcmdgsm->IsAvailableServiceGSM(avaible)==DIOATCMD_ERROR_NONE)	string.Format(__L("Disp. Servicios GSM    : %s") , avaible?__L("Si"):__L("No"));                           
																								}
																								break;

																			case  6 : if(dioatcmdgsm->GetIMSI(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("IMSI                   : %s"),value.Get());   
																								break;													
																									

																			case  7 : { XBYTE support;
															
																									if(dioatcmdgsm->GetSMSSupport(support)==DIOATCMD_ERROR_NONE)				string.Format(__L("Soporte SMS            : servicio %d, destino %s, origen %s, broadcast %s")	, (support>>4)  
																																																																																																											, support&DIOATCMDGSM_SMSSUPPORT_MOVILETERMINATED ?__L("Si"):__L("No")
																																																																																																											, support&DIOATCMDGSM_SMSSUPPORT_MOVILEORIGINATED ?__L("Si"):__L("No")
																																																																																																											, support&DIOATCMDGSM_SMSSUPPORT_BROADCAST			   ?__L("Si"):__L("No"));																				
																								}
																								break;	
													
																			case  8 : { XBYTE format;
																									XBYTE support;
																																						
																									if(dioatcmdgsm->GetSMSFormat(format,false) ==DIOATCMD_ERROR_NONE) 
																										{
																											if(dioatcmdgsm->GetSMSFormat(support,true)==DIOATCMD_ERROR_NONE) 
																												{
																													XSTRING strformat;
																													XSTRING strsupport;

																													strformat = __L("desconocido");
																													if(format == DIOATCMDGSM_SMSFORMAT_PDU)  strformat = __L("PDU");
																													if(format == DIOATCMDGSM_SMSFORMAT_TXT)  strformat = __L("Texto");

																													if(support&DIOATCMDGSM_SMSFORMAT_PDU)		 strsupport = __L("PDU");
																													if(support&DIOATCMDGSM_SMSFORMAT_TXT)     
																														{
																															if(!strsupport.IsEmpty()) strsupport += __L(" + ");
																															strsupport += __L("Texto");
																														}


																													string.Format(__L("Formato de SMS         : %s  (valido %s)"),strformat.Get(),strsupport.Get());																																							
																												}
																										}
																								}
																								break;	

																			 case 9 : if(dioatcmdgsm->GetSMSCenter(value)==DIOATCMD_ERROR_NONE)						string.Format(__L("Centro de mensajes     : %s"),value.Get());   
																								break;		
																		}
											
																	if(!string.IsEmpty()) xconsole->PrintMessage(string.Get(),1,true,true);
																}												
											
															xconsole->PrintMessage(__L("Enviando mensaje ... "),1,true,false);	

															if(dioatcmdgsm->SendSMS(__L("649668886"),__L("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$%&/{}[]()\\*#@+-\"=;:.,_|")) == DIOATCMD_ERROR_NONE) 	 //  ¿?¡!																		
																			xconsole->PrintMessage(__L("Ok."),0,false,true);	
																 else xconsole->PrintMessage(__L("Error!"),0,false,true);	
											 
																
															dioatcmdgsm->End();

														} else xconsole->PrintMessage(__L("\r Dispositivo de comandos AT NO accesible!"),1,true,true);	
									

													delete dioatcmdgsm;
												}
											
											diofactory->DeleteStreamIO(diostream);
										}

									delete diostreamcfg;
								}

						}

					diofactory->DeleteStreamEnumDevices(enumdevicesremote);			
				}

			diofactory->DeleteStreamEnumDevices(enumdeviceslocal);			
		}

	xconsole->Printf(__L("Terminada conexion AT.\n"));

	return true;
}
*/


bool TEST::Test_ATCommand()
{				
	xconsole->PrintMessage(__L("Creando dispositivo... "), 1 , true, true);	

	DIOSTREAMUARTCONFIG* diostreamcfg = new DIOSTREAMUARTCONFIG();
	if(!diostreamcfg) return false;

	diostreamcfg->SetFromString(__L("/dev/ttyUSB1,115200,8,N,1,NONE"));

	DIOSTREAMUART* diostream = (DIOSTREAMUART*)diofactory->CreateStreamIO(diostreamcfg);
	if(!diostream) return false;

	DIOATCMDGSM* dioatcmdgsm =  new DIOATCMDGSM(diostream);
	if(!dioatcmdgsm) return false;
					
	xconsole->PrintMessage(__L("Comprobando dispositivo... "), 1 , true, true);	

	if(dioatcmdgsm->Ini(15, true, true))
		{																	
			xconsole->PrintMessage(__L("Dispositivo comandos AT accesible..."), 1 , true, true);	
			
			XSTRING string;
			XSTRING value;

			for(int c=0;c<10;c++)
				{
					string.Empty();
							
					switch(c)
						{
							case  0 : if(dioatcmdgsm->GetManufactured(value)==DIOATCMD_ERROR_NONE)					string.Format(__L("Fabricante             : %s"),value.Get());   
												break;

							case  1 : if(dioatcmdgsm->GetModel(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("Modelo de terminal     : %s"),value.Get());   
												break;
											
							case  2 : if(dioatcmdgsm->GetIMEI(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("IMEI                   : %s"),value.Get());   
												break;

							case  3 : if(dioatcmdgsm->GetVersion(value)==DIOATCMD_ERROR_NONE)								string.Format(__L("Version                : %s"),value.Get());   
												break;	
														
							case  4 : { int RSSI = 0;
													int BER  = 0;
																			
													if(dioatcmdgsm->GetSignalQuality(RSSI,BER)==DIOATCMD_ERROR_NONE)		string.Format(__L("Calidad de senal       : (RSSI %d)  (BER %d)"),RSSI,BER);																																							
												}
												break;		

							case  5 : { bool avaible;

													if(dioatcmdgsm->IsAvailableServiceGSM(avaible)==DIOATCMD_ERROR_NONE)	string.Format(__L("Disp. Servicios GSM    : %s") , avaible?__L("Si"):__L("No"));                           
												}
												break;

							case  6 : if(dioatcmdgsm->GetIMSI(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("IMSI                   : %s"),value.Get());   
												break;													
																									

							case  7 : { XBYTE support;
															
													if(dioatcmdgsm->GetSMSSupport(support)==DIOATCMD_ERROR_NONE)				string.Format(__L("Soporte SMS            : servicio %d, destino %s, origen %s, broadcast %s")	, (support>>4)  
																																																																																															, support&DIOATCMDGSM_SMSSUPPORT_MOVILETERMINATED ?__L("Si"):__L("No")
																																																																																															, support&DIOATCMDGSM_SMSSUPPORT_MOVILEORIGINATED ?__L("Si"):__L("No")
																																																																																															, support&DIOATCMDGSM_SMSSUPPORT_BROADCAST			   ?__L("Si"):__L("No"));																				
												}
												break;	
													
							case  8 : { XBYTE format;
													XBYTE support;
																																						
													if(dioatcmdgsm->GetSMSFormat(format,false) ==DIOATCMD_ERROR_NONE) 
														{
															if(dioatcmdgsm->GetSMSFormat(support,true)==DIOATCMD_ERROR_NONE) 
																{
																	XSTRING strformat;
																	XSTRING strsupport;

																	strformat = __L("desconocido");
																	if(format == DIOATCMDGSM_SMSFORMAT_PDU)  strformat = __L("PDU");
																	if(format == DIOATCMDGSM_SMSFORMAT_TXT)  strformat = __L("Texto");

																	if(support&DIOATCMDGSM_SMSFORMAT_PDU)		 strsupport = __L("PDU");
																	if(support&DIOATCMDGSM_SMSFORMAT_TXT)     
																		{
																			if(!strsupport.IsEmpty()) strsupport += __L(" + ");
																			strsupport += __L("Texto");
																		}


																	string.Format(__L("Formato de SMS         : %s  (valido %s)"),strformat.Get(),strsupport.Get());																																							
																}
														}
												}
												break;	

							case 9 :  if(dioatcmdgsm->GetSMSCenter(value)==DIOATCMD_ERROR_NONE)						string.Format(__L("Centro de mensajes     : %s"),value.Get());   
												break;		
						}
											
					if(!string.IsEmpty()) xconsole->PrintMessage(string.Get(), 1 , true, true);	
				}												
											
			xconsole->PrintMessage(__L("Enviando mensaje ... "),1,true,false);	

			if(dioatcmdgsm->SendSMS(__L("649668886"),__L("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$%&/{}[]()\\*#@+-\"=;:.,_|")) == DIOATCMD_ERROR_NONE) 	 //  ¿?¡!																		
						xconsole->PrintMessage(__L("Ok."),0,false,true);	
				else xconsole->PrintMessage(__L("Error!"),0,false,true);	
											 
																
			dioatcmdgsm->End();

		} else xconsole->PrintMessage(__L("Dispositivo de comandos AT NO accesible!"), 1 , true, true);	
									

	delete dioatcmdgsm;
																							
	diofactory->DeleteStreamIO(diostream);
									
	delete diostreamcfg;
	
	xconsole->PrintMessage(__L("Terminada conexion AT."), 1 , true, true);	

	return true;
}




/*-------------------------------------------------------------------
//  TEST::Test_ClientBTModuleCSR
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 9:36:59
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_ClientBTModuleCSR()
{

	xconsole->Printf(__L("Inicializando conexion...\n"));
	
	DIOSTREAMENUMDEVICES*			 enumdeviceslocal	 = NULL;
	DIOSTREAMENUMDEVICES*			 enumdevicesremote = NULL;
	DIOSTREAMDEVICEBLUETOOTH*  devicelocal			 = NULL;
	DIOSTREAMDEVICEBLUETOOTH*  deviceremote			 = NULL;

	enumdeviceslocal = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL);
	if(enumdeviceslocal)
		{					
			enumdeviceslocal->Search();

			if(enumdeviceslocal->GetDevices()->GetSize())
				{
					devicelocal = (DIOSTREAMDEVICEBLUETOOTH*)enumdeviceslocal->GetDevices()->Get(0);
					devicelocal->DebugPrintInfo(xconsole);	
				}
			 else devicelocal = NULL;

			if(devicelocal)
				{
					enumdevicesremote = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE);
					if(enumdevicesremote)
						{					
							enumdevicesremote->Search();

							DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));
							DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Remote Bluetooth modules:"));
										
							DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Search ..."));

							xtimer->Reset();
							while(enumdevicesremote->IsSearching())
								{
									xsleep->MilliSeconds(10);
								}
					
							for(int c=0;c<(int)enumdevicesremote->GetDevices()->GetSize();c++)
								{					
									DIOSTREAMDEVICEBLUETOOTH* device = (DIOSTREAMDEVICEBLUETOOTH*)enumdevicesremote->GetDevices()->Get(c);
									if(device) 
										{						
											device->DebugPrintInfo(xconsole);	

											if(!device->GetName()->Compare(__L("WT11")))
												{																																		
													deviceremote = device;												
												} 											
										}
								}
											
							if(deviceremote && (deviceremote->GetChannelSerialPort() != DIOSTREAMDEVICEBLUETOOTH_NOCHANNEL))													
								{
									deviceremote->DebugPrintInfo(xconsole);		
									DIOSTREAMBLUETOOTHCONFIG* diostreamcfg = new DIOSTREAMBLUETOOTHCONFIG();
									if(diostreamcfg)
										{				
											diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
											diostreamcfg->GetLocalMAC()->Set(devicelocal->GetMAC()->Get());																		
											diostreamcfg->GetRemoteMAC()->Set(deviceremote->GetMAC()->Get());						
											diostreamcfg->SetRemoteChannel(deviceremote->GetChannelSerialPort());

											diostreamcfg->GetPIN()->Set(__L("1234"));							

											DIOSTREAMBLUETOOTH* diostream = (DIOSTREAMBLUETOOTH*)diofactory->CreateStreamIO( diostreamcfg);
											if(diostream)
												{						
													DIOCLICLIENTBTMODULECSR* CLIclient = new DIOCLICLIENTBTMODULECSR(diostream);
													if(CLIclient->Ini(20))
														{
															xconsole->Printf(__L("Conectada...\n"));

															while(!xconsole->KBHit())
																{
																	int RSSI;

																	CLIclient->RSSI(RSSI);

																	xconsole->Printf(__L("RSSI: %d\n"), RSSI);
																}
											

															CLIclient->End();
														}		

													delete CLIclient;

													diofactory->DeleteStreamIO(diostream);
												}

											delete diostreamcfg;
										}
								}

							diofactory->DeleteStreamEnumDevices(enumdevicesremote);			
						}
				}

			diofactory->DeleteStreamEnumDevices(enumdeviceslocal);			
		}

	xconsole->Printf(__L("Terminada conexion.\n"));

	return true;
}



/*-------------------------------------------------------------------
//	 TEST::Test_SSHReverse
*/
/**
//
//	
//
//	@author		Abraham J. Velez 
//	@version	03/01/2018 14:18:05
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TEST::Test_SSHReverse()
{
	XSTRING		string;
	XSTRING		stringresult;
	XSTRING		localIP;
	DIOIP			publicIP;
	XSTRING		publicIPstring;
	bool			status = false;
	
	DIOSTREAMIPLOCALENUMDEVICES* enumdevices = (DIOSTREAMIPLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_IP_LOCAL);
	if(enumdevices)
		{			
			DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetFirstActiveDevice();
			if(device) device->GetIP()->GetXString(localIP);
				
			diofactory->DeleteStreamEnumDevices(enumdevices);				
		}

	DIOWEBSCRAPERPUBLICIP* webscraperpublicip	= new DIOWEBSCRAPERPUBLICIP;	
	if(webscraperpublicip)
		{	
			if(webscraperpublicip->Get(publicIP, 5)) 
				{								
					publicIP.GetXString(publicIPstring);
				}			

			delete webscraperpublicip;
		}
	
	
		
	
	string.Format(__L("public IP            : [%s]"), publicIPstring.Get());
	xconsole->PrintMessage(string.Get(), 1, true, true);
	
	string.Format(__L("local IP             : [%s]"), localIP.Get());
	xconsole->PrintMessage(string.Get(), 1, true, true);

	int c = 0;
	while(1)
		{			
			
			DIOSSHREVERSE::GetInstance().GetURLTarget()->Set(__L("dns.com"));
			DIOSSHREVERSE::GetInstance().GetLocalIP()->Set(__L("localhost"));
			DIOSSHREVERSE::GetInstance().GetLogin()->Set(__L("root"));
			DIOSSHREVERSE::GetInstance().GetPassword()->Set(__L("[pass]"));
			DIOSSHREVERSE::GetInstance().SetPort(2210);
			

			XBYTE key = 0;
			while(!xconsole->KBHit())
				{
					xsleep->MilliSeconds(100);
				}
				
			key = xconsole->GetChar();						
			switch(key)
				{
					case 'Q': DIOSSHREVERSE::GetInstance().DelInstance();	
										return true; 

					case 'A': DIOSSHREVERSE::GetInstance().Activate();			break;										
					case 'D': DIOSSHREVERSE::GetInstance().DeActivate();		break;

				}

			string.Format(__L("\n[%2d] Execute reverse SSH  : "), c);
			xconsole->PrintMessage(string.Get(), 1, true, true);

			stringresult = (DIOSSHREVERSE::GetInstance().IsRunning()) ? __L("Ok.") : __L("ERROR!");
			xconsole->PrintMessage(stringresult.Get(), 0, false, true);

			c++;
		}

	DIOSSHREVERSE::GetInstance().DelInstance();	

	return true;
}






/*-------------------------------------------------------------------
//  TEST::Test_HASHGeneric
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 9:42:04
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_HASHGeneric()
{
	
	XBUFFER  input;
	XSTRING  leyend;
	XSTRING  string;	
		
	string = __L("The quick brown fox jumps over the lazy dog");

	xconsole->Printf(__L("Sentence: \"%s\"\n\n"), string.Get());

	XSTRING_CREATEOEM(string, charstr)
	input.Add((XBYTE*)charstr, string.GetSize());
	XSTRING_DELETEOEM(charstr)
	
	for(int c=0;c<8;c++)
		{
			HASH* HASH = NULL;
			switch(c)
				{
					case  0 :  HASH = new HASHCRC32();										leyend = __L("CRC32");			break;
					case  1 :  HASH = new HASHMD5();											leyend = __L("MD5");				break;
					case  2 :  HASH = new HASHSHA1();											leyend = __L("SHA1");				break;
					case  3 :  HASH = new HASHSHA2(HASHSHA2TYPE_224);			leyend = __L("SHA2 224");		break;
					case  4 :  HASH = new HASHSHA2(HASHSHA2TYPE_256);			leyend = __L("SHA2 256");		break;
					case  5 :  HASH = new HASHSHA2(HASHSHA2TYPE_384);			leyend = __L("SHA2 384");		break;
					case  6 :  HASH = new HASHSHA2(HASHSHA2TYPE_512);			leyend = __L("SHA2 512");		break;
					case  7 :  HASH = new HASHWHIRLPOOL();								leyend = __L("Whirpool");		break;	
				}

			if(!HASH) return false;

			Test_Hash(HASH, input, leyend.Get());
			delete HASH;
		}



	/*	
  #define RETURN	"\x0D\x0A"

	char* data[] = {	"/ISk5\2MT382-1000" "\x0D\x0A"
										"\x0D\x0A"
										"1-3:0.2.8(50)" RETURN
										"0-0:1.0.0(101209113020W)" RETURN
										"0-0:96.1.1(4B384547303034303436333935353037)" RETURN
										"1-0:1.8.1(123456.789*kWh)" RETURN
										"1-0:1.8.2(123456.789*kWh)" RETURN
										"1-0:2.8.1(123456.789*kWh)" RETURN
										"1-0:2.8.2(123456.789*kWh)" RETURN
										"0-0:96.14.0(0002)" RETURN
										"1-0:1.7.0(01.193*kW)" RETURN
										"1-0:2.7.0(00.000*kW)" RETURN
										"0-0:96.7.21(00004)" RETURN
										"0-0:96.7.9(00002)" RETURN
										"1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)" RETURN
										"1-0:32.32.0(00002) 1-0:52.32.0(00001)" RETURN
										"1-0:72.32.0(00000) 1-0:32.36.0(00000)" RETURN
										"1-0:52.36.0(00003) 1-0:72.36.0(00000)" RETURN
										"0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3" "C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)" RETURN
										"1-0:32.7.0(220.1*V)" RETURN
										"1-0:52.7.0(220.2*V)" RETURN
										"1-0:72.7.0(220.3*V)" RETURN
										"1-0:31.7.0(001*A)" RETURN
										"1-0:51.7.0(002*A)" RETURN
										"1-0:71.7.0(003*A)" RETURN
										"1-0:21.7.0(01.111*kW)" RETURN
										"1-0:41.7.0(02.222*kW)" RETURN
										"1-0:61.7.0(03.333*kW)" RETURN
										"1-0:22.7.0(04.444*kW)" RETURN
										"1-0:42.7.0(05.555*kW)" RETURN
										"1-0:62.7.0(06.666*kW)" RETURN
										"0-1:24.1.0(003)" RETURN
										"0-1:96.1.0(3232323241424344313233343536373839)" RETURN
										"0-1:24.2.1(101209112500W)(12785.123*m3)" RETURN
										"!"
									};
									
									

	XWORD result = CRC16((XBYTE*)data, sizeof(data));

	*/

	return true;
}




/*-------------------------------------------------------------------
//  TEST::Test_HASHCOMP128
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 9:42:00
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_HASHCOMP128()
{ 
	// Rand					: 58 E3 5D D5 D0 DD F8 5E CF F0 58 1C FA 14 DB A6
	// Key					: 9A AA FE 5B F5 6D 6B C8 BB 1E B7 C6 37 75 BE 62	
	// A3A8 result	:	14 0D AB 04 44 AB A8 0B 2C D1 30 00
		
	XBYTE						rand[HASHCOMP128V1_RAND_SIZE] = { 0x58, 0xE3, 0x5D, 0xD5, 0xD0, 0xDD, 0xF8, 0x5E, 0xCF, 0xF0, 0x58, 0x1C, 0xFA, 0x14, 0xDB, 0xA6 };
	XBYTE						ki[HASHCOMP128V1_KI_SIZE]		  = { 0x9A, 0xAA, 0xFE, 0x5B, 0xF5, 0x6D, 0x6B, 0xC8, 0xBB, 0x1E, 0xB7, 0xC6, 0x37, 0x75, 0xBE, 0x62 };
	XSTRING					result;
	HASHCOMP128V1* HASH = new HASHCOMP128V1();	
	if(HASH)
		{				
			XBUFFER input;

			HASH->SetInput(rand,ki,input);

			HASH->Do(input);		
				
			HASH->GetResultString(result);
			
			xconsole->Printf(__L("COMP128v1    : %s  "),result.Get());

			XBUFFER SRES;
			XBUFFER Kc;

			if(HASH->GetSRES(SRES))
				{
					if(HASH->GetKc(Kc))									
						{								
							xconsole->Printf(__L("SRES: "));

							for(int c=0;c<(int)SRES.GetSize();c++)
								{
									xconsole->Printf(__L("%02X"), SRES.GetByte(c));
								}

							xconsole->Printf(__L(" Kc: "));

							for(int c=0;c<(int)Kc.GetSize();c++)
								{
									xconsole->Printf(__L("%02X"),Kc.GetByte(c));
								}
						}
				}

			xconsole->Printf(__L("\n"));	

			delete HASH;
		}

	return true;
}
	



/*-------------------------------------------------------------------
//  TEST::Test_CIPHERSimetric
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2014 10:45:32
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_CIPHERSimetric()
{
	XBUFFER								input;
	XSTRING								leyend;
	CIPHERKEYSYMMETRICAL  key;	
	
	XBYTE									inivector[]	= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
	XBYTE									inputdata[]	= { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a, 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
  XBYTE									keydata[]		= { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 }; // { 0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b }; // { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };  

	input.Delete();
	input.Add((XBYTE*)inputdata, sizeof(inputdata));
	key.Set(keydata, sizeof(keydata));	
 
	for(int c=0;c<5;c++)
		{
			CIPHER*		cipher = NULL;
			switch(c)
				{
					case  0 :  cipher = new CIPHER();						leyend = __L("XOR");			 break;
					case  1 :  cipher = new CIPHERDES();				leyend = __L("DES");			 break;
					case  2 :  cipher = new CIPHER3DES();				leyend = __L("3DES");			 break;										
					case  3 :  cipher = new CIPHERAES();				leyend = __L("AES");			 break;	
					case  4 :  cipher = new CIPHERBLOWFISH();		leyend = __L("Blowfish");	 break;						
				}
					
			if(cipher) 
				{
					cipher->SetChainingMode(CIPHERCHAININGMODE_CBC);				
					cipher->SetPaddingType(XBUFFER_PADDINGTYPE_ZEROS); 
					cipher->SetInitVector(inivector,sizeof(inivector));			

					cipher->SetKey(&key);

					Test_Cipher(cipher, input, leyend.Get());

					delete cipher;
				}
		}

	return true;
}




/*-------------------------------------------------------------------
//  TEST::Test_CIPHERRSA
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2014 10:46:03
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_CIPHERRSA()
{
	//rsa_self_test(1);


	XBYTE ans1formatSHA1[] = {  0x30, 0x21, 
		
															0x30, 0x09, 
															
															0x06, 0x05, 
															0x2b, 0x0e, 0x03, 0x02, 0x1a, 

															0x05, 0x00, 

															0x04, 0x14, 
															0x7b, 0x0e, 0xbd, 0x44, 0x45, 0xc6, 0x4f, 0x64, 0xc5, 0x70, 0x2e, 0xde, 0x8b, 0x29, 0xaf, 0x83, 0x6a, 0x6c, 0x6f, 0x14
													 };
	
	XBER			beroui;
	XBER			bernull;
	XBER			bersha1;
	
	XBER			berseq1;
	XBER			berseq2;
		
	XBUFFER		xbufferans;
	
	beroui.SetOID(__L("1.3.14.3.2.26"));																						
	bernull.SetNULL();																														
	bersha1.SetOCTETSTRING(__L("\x7b\x0e\xbd\x44\x45\xc6\x4f\x64\xc5\x70\x2e\xde\x8b\x29\xaf\x83\x6a\x6c\x6f\x14"));   
	


	berseq1.Sequence_AddTo(beroui);
	berseq1.Sequence_AddTo(bernull);	

	berseq1.SetType(berseq1.GetType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

	berseq2.Sequence_AddTo(berseq1);	
	berseq2.Sequence_AddTo(bersha1);
	berseq2.SetType(berseq2.GetType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

	berseq2.GetDump(xbufferans);


	XBUFFER								input;
	
	//CIPHERKEYRSAPUBLIC		publickey;
	//CIPHERKEYRSAPRIVATE		privatekey;		

	/*
	XMPINTEGER						publicmodulus;
	XMPINTEGER						publicexponent;
	
	XMPINTEGER						privateprime1factor;
	XMPINTEGER						privateprime2factor; 
	XMPINTEGER						privateexponent;
	*/
	
//XBYTE									inputdata[]	= { 0xAA, 0xBB, 0xCC, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x11, 0x22, 0x33, 0x0A, 0x0B, 0x0C, 0xCC, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD };
//XBYTE									inputdata[]	= { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a, 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
	XBYTE									inputdata[]	= "Esta es una prueba de un texto largo para ser encriptado usando la RSA. La prueba consiste en que sea mas largo que la clave, osea mas de 128 bytes de largo, y que ademas tenga una longitud que no sea complemento de 2, para que queden bien los Paddings internos de la funcion. Pues eso. se acabo el rollo de pollo.";

	bool									status      = false;
	
	
	input.Delete();
	input.Add((XBYTE*)inputdata, sizeof(inputdata));
	
	/*
	publicmodulus.SetFromString (16, __L("9292758453063D803DD603D5E777D7888ED1D5BF35786190FA2F23EBC0848AEADDA92CA6C3D80B32C4D109BE0F36D6AE7130B9CED7ACDF54CFC7555AC14EEBAB93A89813FBF3C4F8066D2D800F7C38A81AE31942917403FF4946B0A83D3D3E05EE57C6F5F5606FB5D4BC6CD34EE0801A5E94BB77B07507233A0BC7BAC8F90F79"));
	publicexponent.SetFromString(16, __L("10001"));
	
	privateprime1factor.SetFromString(16, __L("C36D0EB7FCD285223CFB5AABA5BDA3D82C01CAD19EA484A87EA4377637E75500FCB2005C5C7DD6EC4AC023CDA285D796C3D9E75E1EFC42488BB4F1D13AC30A57"));
	privateprime2factor.SetFromString(16, __L("C000DF51A7C77AE8D7C7370C1FF55B69E211C2B9E5DB1ED0BF61D0D9899620F4910E4168387E3C30AA1E00C339A795088452DD96A9A5EA5D9DCA68DA636032AF"));
	privateexponent.SetFromString		 (16, __L("24BF6185468786FDD303083D25E64EFC66CA472BC44D253102F8B4A9D3BFA75091386C0077937FE33FA3252D28855837AE1B484A8A9A45F7EE8C0C634F99E8CDDF79C5CE07EE72C7F123142198164234CABB724CF78B8173B9F880FC86322407AF1FEDFDDE2BEB674CA15F3E81A1521E071513A1E85B5DFA031F21ECAE91A34D")); 

	publickey.Set(publicmodulus, publicexponent);
	privatekey.Set(privateprime1factor, privateprime2factor, privateexponent);

	xconsole->Printf(__L("Check RSA keys : "));	
	if(!privatekey.Check(publickey)) 		
		{
			xconsole->Printf(__L("Fail.\n"));	
			return false;
		}

	xconsole->Printf(__L("OK.\n"));	
	*/


	/*
	XPATH	 	 xpath;
	XPATH		 xpathgeneric;	

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GENERIC, xpathgeneric);
	xpath.Create(3 , xpathgeneric.Get(), __L("keys"),__L(".key"));	
	
	CIPHERFILEKEYS* filekeys = new CIPHERFILEKEYS( xpath);	
	CIPHERRSA*			cipher = new CIPHERRSA();
	if(!cipher) return false;
		
	
	xconsole->Printf(__L("Create RSA keys: "));	

  if(!cipher->GenerateKeys(1024, 65537, publickey, privatekey)) return false;
  if(!privatekey.Check(publickey)) 
		{
			xconsole->Printf(__L("Fail.\n"));	
			return false;
		}
		
	xconsole->Printf(__L("OK.\n"));	

	filekeys->AddKey(publickey);
	filekeys->AddKey(privatekey);
	
	filekeys->UpdateFile();

	delete filekeys;
	*/


	 
	// ----------------------------------------------------

	CIPHERRSA*			cipher = new CIPHERRSA();
	if(!cipher) return false;

	xconsole->Printf(__L("Read RSA keys  : "));	
	
	XPATH	 	 xpath;
	XPATH		 xpathgeneric;	

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathgeneric);
	xpath.Create(3 , xpathgeneric.Get(), __L("keys"),__L(".key"));	
	
	CIPHERFILEKEYS*				filekeys   = new CIPHERFILEKEYS(xpath);	
	CIPHERKEYRSAPUBLIC*		publickey  = NULL;
	CIPHERKEYRSAPRIVATE*	privatekey = NULL;		
		
	publickey  = (CIPHERKEYRSAPUBLIC*)filekeys->GetKey(CIPHERKEYTYPE_PUBLIC);
	privatekey = (CIPHERKEYRSAPRIVATE*)filekeys->GetKey(CIPHERKEYTYPE_PRIVATE);

	if(!publickey || !privatekey)
		{
			xconsole->Printf(__L("Fail.\n"));	
			return false;
		}

	xconsole->Printf(__L("OK.\n"));	
	
	//----------------------------------------------------
	
	if(cipher->SetKey(publickey))
		{
			if(cipher->SetKey(privatekey)) 
				{
					xconsole->Printf(__L("Test RSA       : "));	

					if(cipher->Cipher(input, CIPHERKEYTYPE_PRIVATE))
						{
							cipher->Uncipher((*cipher->GetResult()), CIPHERKEYTYPE_PUBLIC); 
						}

					if(input.Compare(cipher->GetResult()))
						{
							xconsole->Printf(__L("OK.\n"));	
							status = true;
						} 
				}
		}
		
	if(!status)  xconsole->Printf(__L("Fail.\n"));	

	delete filekeys;
	
	delete cipher;

	return status;
}




/*-------------------------------------------------------------------
//  TEST::Test_HASHFile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 9:59:50
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_HASHFile()
{
	XPATH pathname;

	pathname = __L("prueba.dat");

  HASHWHIRLPOOL* HASH = new HASHWHIRLPOOL();
	XFILEHASH* fileHASH  = new XFILEHASH(HASH);

	if(fileHASH)
		{
			XBYTE data[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

			fileHASH->Set(0xFFFF, 0xFFFF, 0xAABB,  __L("Prueba de ID"));

			//fileHASH->Create(pathname);			
			//fileHASH->GetPrimaryFile()->Write(data,16);			
			//fileHASH->Close();		

			if(fileHASH->Open(pathname, false))
				{
					fileHASH->GetPrimaryFile()->SetPosition(XFILE_SEEKEND);
					fileHASH->GetPrimaryFile()->Write(data,16);	

					fileHASH->UpdateHash();
					
					fileHASH->Close();		
				}
		}

	delete fileHASH;
	delete HASH;
	
	return true;
}




/*-------------------------------------------------------------------
//  TEST::Test_Ping
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:37:37
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Ping()
{
	/*
	DIOPING* ping = (DIOPING*)diofactory->CreatePing();
	if(ping)
		{
			SubscribeEvent(DIOPINGXEVENTTYPE_DOPING, ping);

			while(!xconsole->KBHit())
				{
					xconsole->Printf(__L("do...\r"));

					ping->DeleteAllReplys();

					ping->Set(__L("8.8.8.8"));

					ping->Do(8);
				
					xconsole->Printf(__L("Respuestas %d \n"), ping->GetReplys()->GetSize());

					for(int c=0;c<(int)ping->GetReplys()->GetSize();c++)
						{
							DIOPINGREPLY* reply = (DIOPINGREPLY*)ping->GetReplys()->Get(c);
							if(reply)
								{
									xconsole->Printf(__L("Respuesta desde %s: bytes=%d tiempo=%dms TTL=%d \n")	, reply->GetFromIP()->Get()
																																															,	reply->GetSizeRequest()
																																															, reply->GetTimeElapsed()
																																															,	reply->GetTTL());									
								}
						}


				}

			UnSubscribeEvent(DIOPINGXEVENTTYPE_DOPING, ping);
			diofactory->DeletePing(ping);
		}
	*/
	
	DIOPING* ping = (DIOPING*)diofactory->CreatePing();
	if(ping)
		{
			SubscribeEvent(DIOPINGXEVENTTYPE_DOPING, ping);

			while(!xconsole->KBHit())
				{
					xconsole->Printf(__L("do...\r"));

					ping->DeleteAllReplys();

					ping->Set(__L("192.168.1.24"));

					ping->Do(1, 0, true);
				
					xconsole->Printf(__L("Respuestas %d \n"), ping->GetReplys()->GetSize());

					for(int c=0;c<(int)ping->GetReplys()->GetSize();c++)
						{
							DIOPINGREPLY* reply = (DIOPINGREPLY*)ping->GetReplys()->Get(c);
							if(reply)
								{
									xconsole->Printf(__L("Respuesta desde %s: bytes=%d tiempo=%dms TTL=%d \n")	, reply->GetFromIP()->Get()
																																															,	reply->GetSizeRequest()
																																															, reply->GetTimeElapsed()
																																															,	reply->GetTTL());									
								}
						}
				}

			UnSubscribeEvent(DIOPINGXEVENTTYPE_DOPING, ping);
			diofactory->DeletePing(ping);
		}

	return true;



	/*
	DIOPING* ping = diofactory->CreatePing;
	if(ping)
		{
			SubscribeEvent(DIOPINGXEVENTTYPE_DOPING, ping);

			while(!xconsole->KBHit())
				{

					xconsole->Printf(__L("do...\r"));


					ping->DeleteAllReplys();

					ping->Set(__L("www.google.es"));

					ping->Do(2,3);

				
					xconsole->Printf(__L("Respuestas %d \n"), ping->GetReplys()->GetSize());

					for(int c=0;c<(int)ping->GetReplys()->GetSize();c++)
						{
							DIOPINGREPLY* reply = (DIOPINGREPLY*)ping->GetReplys()->Get(c);
							if(reply)
								{
									if(reply->WasConnected())
										{
											xconsole->Printf(__L("Respuesta desde %s: bytes=%d tiempo=%dms TTL=%d \n")	, reply->GetFromIP()->Get()
																																																	,	reply->GetSizeRequest()
																																																	, reply->GetTimeElapsed()
																																																	,	reply->GetTTL());
										}
								}
						}
				}

			UnSubscribeEvent(DIOPINGXEVENTTYPE_DOPING, ping);

			diofactory->DeletePing(ping);
		}

	return true;
	*/
}		
	



/*-------------------------------------------------------------------
//  TEST::Test_DIOStreamTCPIP2
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:24:14
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamTCPIP2()
{
	xconsole->PrintMessage(__L(" Init ..."),1,true,true);	

  DIOSTREAMTCPIPCONFIG * diostreamcfg = new DIOSTREAMTCPIPCONFIG();
	if(diostreamcfg) 
		{			
			diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
			diostreamcfg->GetLocalIP()->Set(__L("192.168.101.96"));
			diostreamcfg->SetRemotePort(50008);

			DIOSTREAM* diostream = (DIOSTREAM*)diofactory->CreateStreamIO( diostreamcfg);
			if(diostream) 
				{	
					while(1)
						{
						  xconsole->PrintMessage(__L(" Connecting ..."),1,true,true);	

					 		if(diostream->Open()) 
								{
									xtimer->Reset();
									while(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_CONNECTED)
										{
											xconsole->Printf(__L("    Check conexion [%d] ...  \r") , xtimer->GetMeasureSeconds());		

											xsleep->MilliSeconds(500);

											if(xtimer->GetMeasureSeconds()>10)  
												{
													xconsole->PrintMessage(__L(" Time Out Connexion."),1,true,true);	
													break;												
												}

											if(xconsole->KBHit()) break;	
										}

									if(diostream->GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED) xconsole->PrintMessage(__L(" Connected!               "),1,true,true);	
									
									while(diostream->GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED)
										{
											XBYTE buffer[128];

											memset(buffer,0,128);

											XDWORD br = diostream->Read(buffer,128);												
											if(br) 
												{
													XSTRING string;

												  string = (char*)buffer;	
													xconsole->Printf(__L("[%s]"),string.Get(), br);		
												}

											if(xconsole->KBHit()) break;					
										}

									diostream->Close();
									xconsole->PrintMessage(__L(" Disconnected."),1,true,true);																		
								}

							if(xconsole->KBHit()) break;																
						}

					diofactory->DeleteStreamIO(diostream);
				}			

			delete diostreamcfg;			
		}
					
	return false;
}	




/*-------------------------------------------------------------------
//  TEST::Test_WebScrapers
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:41:55
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_WebScrapers()
{
	DIOWEBSCRAPERPUBLICIP*				publicip				= new DIOWEBSCRAPERPUBLICIP;	
	DIOWEBSCRAPERGEOLOCATIONIP*		geolocationip		= new DIOWEBSCRAPERGEOLOCATIONIP;	
	DIOWEBSCRAPERSEXNAME*					sexname					= new DIOWEBSCRAPERSEXNAME;
	DIOWEBSCRAPERUSERAGENTID*			useragentID			= new DIOWEBSCRAPERUSERAGENTID;
	DIOWEBSCRAPERTRANSLATION*			translation			= new DIOWEBSCRAPERTRANSLATION;
	DIOWEBSCRAPERMACMANUFACTURER* macmanufactured = new DIOWEBSCRAPERMACMANUFACTURER;
	DIOWEBSCRAPERWEATHER*					weather					= new DIOWEBSCRAPERWEATHER;
	

	
	XSTRING localIP;

	//localIP = __L("192.168.1.138");
	//webclient->GetProxyURL()->Set(__L("10.155.192.17"));
	//webclient->SetProxyPort(8080);	

		for(int c=0; c<10; c++)
			{			

				if(publicip)
					{
						DIOIP ip;

						if(publicip->Get(ip, 5, &localIP)) 
							{ 
								XSTRING IPstring;
								
								ip.GetXString(IPstring);

								xconsole->Printf(__L("Public IP : %s\n\n"), IPstring.Get());
												
								if(geolocationip)
									{
										DIOGEOLOCATIONIP geoIP;									
										
										if(geolocationip->Get(ip, geoIP, 5, &localIP))											
											{
												xconsole->Printf(__L("City         : %s\n"),  geoIP.GetCity());
												xconsole->Printf(__L("State        : %s\n"),  geoIP.GetState());
												xconsole->Printf(__L("Contry       : %s\n"),  geoIP.GetCountry());
												xconsole->Printf(__L("ISP          : %s\n"),  geoIP.GetISP());
												xconsole->Printf(__L("Organization : %s\n"),  geoIP.GetOrganization());

											} else xconsole->Printf(__L("Error!!!!\n"));

										xconsole->Printf(__L("\n"));							
									}							
							}
					}
						
				if(sexname)
					{
						DIOWEBSCRAPERSEXNAMETYPE sextype;
						XSTRING									 name;
			
						sexname->Get(__L("Alai"), sextype, &name,  5, &localIP);					
						xconsole->Printf(__L("El nombre %.15s "), name.Get());

						switch(sextype)
							{
								case DIOWEBSCRAPERSEXNAMETYPE_NOTOBTAINED	: xconsole->Printf(__L("no obtenido."));							break;
								case DIOWEBSCRAPERSEXNAMETYPE_NONAME			: xconsole->Printf(__L("no es un nombre propio."));		break;
								case DIOWEBSCRAPERSEXNAMETYPE_MALE				: xconsole->Printf(__L("es de hombre"));							break;
								case DIOWEBSCRAPERSEXNAMETYPE_FEMALE			: xconsole->Printf(__L("es de mujer"));								break;			
							}

						xconsole->Printf(__L("\n\n"));
					}			
			
			
				if(useragentID)
					{
						XSTRING browser;
						XSTRING SO;

						useragentID->Get(__L("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31"), browser, SO, 5, &localIP);

						xconsole->Printf(__L("Browser          : %s\n"), browser.Get());
						xconsole->Printf(__L("System Operative : %s\n"), SO.Get());
	
						xconsole->Printf(__L("\n"));
					}
			
					
				if(translation)
					{
						XSTRING        translate;
						XLANGUAGE_CODE languajes[] = {  XLANGUAGE_SPANISH							,																				
																						XLANGUAGE_AFRIKAANS						,
																						XLANGUAGE_ALBANIAN						,																					
																						XLANGUAGE_BASQUE							,
																						
																						XLANGUAGE_BENGALI							,
																						XLANGUAGE_BELARUSIAN					,
																						XLANGUAGE_BULGARIAN						,
																						XLANGUAGE_CATALAN							,
																						XLANGUAGE_CHINESE_SIMPLIFIED	,
																						XLANGUAGE_CHINESE_TRADITIONAL	,
																						XLANGUAGE_CROATIAN						,
																						XLANGUAGE_CZECH								,
																						XLANGUAGE_DANISH							,
																						XLANGUAGE_DUTCH								,
																						XLANGUAGE_ENGLISH							,	
																						XLANGUAGE_ESPERANTO						,
																						XLANGUAGE_ESTONIAN						,
																						XLANGUAGE_FILIPINO						,
																						XLANGUAGE_FINNISH							,
																						XLANGUAGE_FRENCH							,
																						XLANGUAGE_GALICIAN						,
																						XLANGUAGE_GEORGIAN						,
																						XLANGUAGE_GERMAN							,
																						XLANGUAGE_GREEK								,
																						XLANGUAGE_GUJARATI						,	
																						XLANGUAGE_HAITIAN_CREOLE			,
																						XLANGUAGE_HEBREW							,
																						XLANGUAGE_HINDI								,
																						XLANGUAGE_HUNGARIAN						,
																						XLANGUAGE_ICELANDIC						,
																						XLANGUAGE_INDONESIAN					,
																						XLANGUAGE_IRISH								,
																						XLANGUAGE_ITALIAN							,
																						XLANGUAGE_JAPANESE						,
																						XLANGUAGE_KANNADA							,
																						XLANGUAGE_KOREAN							,
																						XLANGUAGE_LATIN								,
																						XLANGUAGE_LATVIAN							,	
																						XLANGUAGE_LITHUANIAN					,
																						XLANGUAGE_MACEDONIAN					,
																						XLANGUAGE_MALAY								,
																						XLANGUAGE_MALTESE							,	
																						XLANGUAGE_NORWEGIAN						,
																						XLANGUAGE_PERSIAN							,
																						XLANGUAGE_POLISH							,
																						XLANGUAGE_PORTUGUESE					,
																						XLANGUAGE_ROMANIAN						,
																						XLANGUAGE_RUSSIAN							,
																						XLANGUAGE_SERBIAN							,
																						XLANGUAGE_SLOVAK							,
																						XLANGUAGE_SLOVENIAN						,		
																						XLANGUAGE_SPANISH							,
																						XLANGUAGE_SWAHILI							,
																						XLANGUAGE_SWEDISH							,
																						XLANGUAGE_TAMIL								,
																						XLANGUAGE_TELUGU							,
																						XLANGUAGE_THAI								,
																						XLANGUAGE_TURKISH							,		
																						XLANGUAGE_UKRAINIAN						, 
																						XLANGUAGE_URDU								,
																						XLANGUAGE_VIETNAMESE					,
																						XLANGUAGE_WELSH								,
																						XLANGUAGE_YIDDISH							,																																																																							
																						
																						XLANGUAGE_SPANISH							};
					
						XSTRING origin;
						int			index = 0;
						
						origin = __L("La mesa de mi casa es roja");
						xconsole->Printf(__L("> %s\n"),origin.Get());

						for(int c=0;c<(sizeof(languajes)/sizeof(int))-1;c++)
							{									
								if(translation->Get(origin.Get()	, languajes[index] , languajes[c+1], translate, 5, &localIP))							
									{
										if(translate.IsOEM()) 
													 xconsole->Printf(__L("> %s\n"),translate.Get());	
											else xconsole->Printf(__L("> (Special chars)\n"));	

										origin = translate.Get();
										index = c+1;
									}							 
							}
	
						xconsole->Printf(__L("\n"));					
					}
					

				if(macmanufactured)
					{
						XBYTE		 MAC[DIOMAC_MAXSIZE] = { 0x00, 0x04, 0x20, 0x11, 0x22, 0x33 };
						DIOMAC	 deviceMAC;	
						XSTRING	 manufactured;

						deviceMAC.Set(MAC);
			
						xconsole->Printf(__L("MAC Manufactured: [%02X%02X%02X] "), deviceMAC.Get()[0], deviceMAC.Get()[1], deviceMAC.Get()[2]);

						if(macmanufactured->Get(deviceMAC, manufactured, 5, &localIP))
									xconsole->Printf(__L("%s")		  , manufactured.Get());
						 else xconsole->Printf(__L("Unknow"));

						xconsole->Printf(__L("\n"));					
					}
											 
				if(weather)
					{ 
						XSTRING	condition;
						float		temperature;
						float		humidity;

						xconsole->Printf(__L("Estado del tiempo: "));

						if(weather->Get(__L("SPXX0016"), true, condition, temperature,  humidity, 5, &localIP))						
										xconsole->Printf(__L("[%s] Temperatura %3.2f, humedad %3.2f%%%%. \n"), condition.Get(), temperature, humidity);
							else  xconsole->Printf(__L("no obtenido.\n"));

						xconsole->Printf(__L("\n"));		
					}


			}	

	delete publicip;
	delete geolocationip;
	delete sexname;
	delete useragentID;
	delete translation;
	delete macmanufactured;
	delete weather;
	
	return true;
}	



/*-------------------------------------------------------------------
//  TEST::Test_MACManufactured
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:41:09
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_MACManufactured()
{
	DIOMACMANUFACTURED* macmanufactured = new DIOMACMANUFACTURED;
	if(macmanufactured)
		{
			XBYTE			_MAC[DIOMAC_MAXSIZE] = { 0x00, 0x04, 0x20, 0x11, 0x22, 0x33 };
			DIOMAC		MAC;	
			XSTRING		manufactured;

			MAC.Set(_MAC);
			
			xconsole->Printf(__L("MAC Manufactured: [%02X%02X%02X] "), MAC.Get()[0], MAC.Get()[1], MAC.Get()[2]);

			if(macmanufactured->Web_GetManufactured(MAC, manufactured))
						xconsole->Printf(__L("%s")		  , manufactured.Get());
			 else xconsole->Printf(__L("Unknow"));

			xconsole->Printf(__L("\n"));
			
			XBUFFER xbuffer;
			XPATH   xpath;

			xpath = DIOMACMANUFACTURED_FILE_NAME;

			
			// if(macmanufactured->File_Download(xbuffer))	macmanufactured->File_Convert(xbuffer,xpath);
			
			xconsole->Printf(__L("MAC Manufactured: [%02X%02X%02X] "), MAC.Get()[0], MAC.Get()[1], MAC.Get()[2]);

			if(macmanufactured->File_GetManufactured(xpath, MAC , manufactured))
						xconsole->Printf(__L("%s")		  , manufactured.Get());
			 else xconsole->Printf(__L("Unknow"));

			XVECTOR<XDWORD> MACs;
			
			manufactured = __L("APPLE");

			macmanufactured->File_GetManufacturedMACs(xpath, manufactured, MACs);

			delete macmanufactured;
		}

	return true;
}





/*-------------------------------------------------------------------
//  TEST::Test_Date1
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:19:13
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Date1()
{
	if(!xdatetime) return false;

	xdatetime->SetDay(27);
	xdatetime->SetMonth(12);
	xdatetime->SetYear(1969);

	xdatetime->SetHours(21);
	xdatetime->SetMinutes(10);
	xdatetime->SetSeconds(0);
		
	xdatetime->SetDay(13);
	xdatetime->SetMonth(10);
	xdatetime->SetYear(1965);

	xdatetime->SetHours(14);
	xdatetime->SetMinutes(10);
	xdatetime->SetSeconds(0);


	XQWORD t1 = xdatetime->GetSeconsFromDate();

	xdatetime->Read();

	XQWORD t2 = xdatetime->GetSeconsFromDate();
	
	
	xtimer->SetMilliSeconds((t2-t1)*1000);

	int  dt	  = xdatetime->GetDifferenceGMT();
	int  bias; 
	bool dls  = xdatetime->IsDayLigthSavingTime(&bias);

	xconsole->Printf(__L("GMT%c%d  -- (Horario de verano: %s)\n"), (dt>0)?__C('+'):__C(' '), abs(dt), dls?__L("Si"):__L("No"));
	
	XSTRING string;

	
	XQWORD microsec = xtimer->GetMeasureMicroSeconds();
	XQWORD millisec = microsec;
	millisec = millisec / 1000;

	xconsole->Printf(__L("[%llu] \n"), microsec);
	xconsole->Printf(__L("[%llu] \n"), millisec);
				
	while(1)
		{			
			xtimer->GetMeasureString(string,true);			
			xconsole->Printf(__L("%s.\r"), string.Get());			
		}

	xconsole->Printf(__L("\n"));

	return true;
}




/*-------------------------------------------------------------------
//  TEST::Test_Date2
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:18:24
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Date2()
{
	if(!xdatetime)	return false;
	if(!xtimer)			return false;

	XSTRING dayofweekstring;

	xdatetime->Read();

	xdatetime->SetDay(29);
	xdatetime->SetMonth(2);
	xdatetime->SetYear(2012);

	xdatetime->SetHours(14);
	xdatetime->SetMinutes(35);
	xdatetime->SetSeconds(23);
	
	
	xdatetime->GetDayOfWeekString(dayofweekstring);

	xconsole->Printf(__L(">> %02d/%02d/%04d %02d:%02d:%02d %s\n")	, xdatetime->GetDay()		, xdatetime->GetMonth()		, xdatetime->GetYear()
																																, xdatetime->GetHours()	, xdatetime->GetMinutes() , xdatetime->GetSeconds()
																																, dayofweekstring.Get());

	double allseconds = (double)xdatetime->GetSeconsFromDate();

	xconsole->Printf(__L(">> Time Offset: %.10g\n"), allseconds);


	xdatetime->SetDateFromSeconds((XQWORD)allseconds);
	

	while(1)
		{
			xdatetime->SetDay(29);
			xdatetime->AddYears(-1);

			xsleep->MilliSeconds(100);

			xdatetime->GetDayOfWeekString(dayofweekstring);

			xconsole->Printf(__L(">> %02d/%02d/%04d %02d:%02d:%02d %s     \r")	, xdatetime->GetDay()		, xdatetime->GetMonth()		, xdatetime->GetYear()
																																					, xdatetime->GetHours()	, xdatetime->GetMinutes() , xdatetime->GetSeconds()
																																					, dayofweekstring.Get());
		}
	
	return true;
}



/*-------------------------------------------------------------------
//  TEST::Test_NTP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:34:57
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_NTP()
{
	DIONTP* ntp = new DIONTP;
	if(ntp)
		{		
			XDATETIME* xdatetime  = xfactory->CreateDateTime();
			DIOURL*		 url				= diofactory->CreateURL();
			
			if(xdatetime && url)
				{
					xconsole->PrintMessage(__L(" Connecting ..."),1,true,false);	
				
					
					DIOIP ip;
					(*url) = __L("www.google.es");					
					url->ResolveURL(ip);

					url->IsLocalAddress();
					

					(*url) = __L("1.es.pool.ntp.org");
		

					if(ntp->GetTime((*url), 5, xsystem->HardwareUseLittleEndian(), (*xdatetime)))
						{
							xconsole->PrintMessage(__L("OK"),1,false,true);	
						}
					 else
						{
							xconsole->PrintMessage(__L(" Error!"),1,false,true);		
						}

					XSTRING dayofweekstring;
					
	
					xdatetime->GetDayOfWeekString(dayofweekstring);

					xconsole->Printf(__L(">> %02d/%02d/%04d %02d:%02d:%02d %s\n")	, xdatetime->GetDay()		, xdatetime->GetMonth()		, xdatetime->GetYear()
																																				, xdatetime->GetHours()	, xdatetime->GetMinutes() , xdatetime->GetSeconds()
																																				, dayofweekstring.Get());



					xfactory->DeleteDateTime(xdatetime);
					diofactory->DeleteURL(url);
				}

			delete ntp;
		}

	return true;
}





/*-------------------------------------------------------------------
//  TEST::Test_Log
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:12:16
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Log()
{
	XLOG::GetInstance().SetLimit(XLOGTYPELIMIT_SIZE, 1024*500, 10);		
	XLOG::GetInstance().SetBackup(true, 3, true);
	
	XBUFFER xdata;
	XRAND*  xrand = xfactory->CreateRand();	

	if(xrand)
		{
			int nentrys  = 50000; //xrand->Between(50,500);
			
			for(int c=0;c<nentrys;c++)
				{										
					XDWORD ndata = xrand->Between(16,512);
					
					xdata.Delete();					
					for(XDWORD d=0;d<ndata;d++)
						{
							xdata.Add((XBYTE)xrand->Between(32,120));	
						}					
						
					XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPLICATION_LOGSECTIONIDGENERIC , false, __L("Linea %d size(%d)"), c, ndata);
					XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL, true, xdata , 16, true, true);			

					xconsole->Printf(__L("[%06d]   NEntrys: %06d  NLines: %06d  Size: %dk   \n"), c, XLOG::GetInstance().GetNEntrys(), XLOG::GetInstance().GetNLines(), XLOG::GetInstance().GetSize()/1024);

					if(xconsole->KBHit()) break;
				}

			xfactory->DeleteRand(xrand);
	  }
	 
	XLOG::GetInstance().Flush();

	return true;
}





/*-------------------------------------------------------------------
//  TEST::Test_Scheduler
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:09:37
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Scheduler()
{		
	XSCHEDULER* xscheduler = new XSCHEDULER;
	if(xscheduler)
		{
			if(xscheduler->Ini())
				{
					XSCHEDULERTASK* task = new XSCHEDULERTASK( xscheduler);
					if(task)
						{	
							XDATETIME* start;
							XDATETIME* end;
							XDATETIME* cadence;

							start			=	xfactory->CreateDateTime();
							end				=	xfactory->CreateDateTime();
							cadence		= xfactory->CreateDateTime();

							//cadence->SetDateFromSeconds(0);
							//cadence->AddSeconds(2);
							
							task->SetIsActive(true);
							
							//task->SetNCycles(XSCHEDULER_CYCLEFOREVER, cadence);			

							task->SetNCycles(2);
							
							start->Read();
							start->AddSeconds(5);

							end->CopyFrom(start);
							end->AddMinutes(5);

							task->SetTimeLimits(start, end);

							//task->SetConditionDayWeek(XSCHEDULER_DAYWEEK_ISWEEKEND);
							
							xscheduler->Task_Add(task);

							SubscribeEvent(XEVENTTYPE_SCHEDULER, xscheduler);

							xfactory->DeleteDateTime(start);								
							xfactory->DeleteDateTime(end);
							xfactory->DeleteDateTime(cadence);

						}

					while(!xconsole->KBHit())
						{
							xsleep->MilliSeconds(10);
						}

					UnSubscribeEvent(XEVENTTYPE_SCHEDULER, xscheduler);
					
					xscheduler->End();

					delete xscheduler;
					xscheduler = NULL;
				}
		}

	xconsole->Printf(__L("Terminado scheduler.\n"));
	
	return true;
}





/*-------------------------------------------------------------------
//  TEST::Test_Script
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 10:07:51
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Script()
{			
	SubscribeEvent(SCRIPTXEVENTTYPE_ERROR, this);						
	SubscribeEvent(SCRIPTXEVENTTYPE_BREAK, this);

	/*
	SCRIPTG* script = new SCRIPTG();
	if(script)
		{
			SCRIPTLIBBASE	scriptlibbase;

			script->AddLibrary((SCRIPTLIB*)&scriptlibbase);

			XPATH xpath;

			XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
			xpath.Slash_Add();
			xpath += __L("test.g");
			
			if(script->Load(xpath))
				{
					script->Run();
				}
			else xconsole->Print(__L("Error loading script"));

			delete script;
		}
	*/

	SCRIPTLUA* script = new SCRIPTLUA();
	if(script)
		{
			SCRIPTLIBBASE	scriptlibbase;

			script->AddLibrary((SCRIPTLIB*)&scriptlibbase);

			XPATH xpath;

			XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
			xpath.Slash_Add();
			xpath += __L("test.lua");
			
			if(script->Load(xpath))
				{
					script->Run();
				}
			else xconsole->Print(__L("Error loading script"));

			delete script;
		}

	

	UnSubscribeEvent(SCRIPTXEVENTTYPE_ERROR, this);		
	UnSubscribeEvent(SCRIPTXEVENTTYPE_BREAK, this);
		
	return true;	
}




/*-------------------------------------------------------------------
//  TEST::Test_OrganicCipherDB
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/12/2013 14:06:05
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_OrganicCipherDB()
{
	XPATH	xpath;

	CIPHERAES* cipher = new CIPHERAES();
	if(cipher)
		{
			CIPHERKEYSYMMETRICAL key;	
			XBYTE								 keydata[]		  = { 0x53, 0x54, 0x45, 0x56, 0x45, 0x43, 0x53, 0x54, 0x45, 0x56, 0x45, 0x43, 0x53, 0x54, 0x45, 0x56  };
  

			key.Set(keydata, sizeof(keydata));	

			cipher->SetChainingMode(CIPHERCHAININGMODE_CFB);				
			cipher->SetPaddingType(XBUFFER_PADDINGTYPE_ZEROS); 			
			cipher->SetKey(&key);

			xpath = __L("D:\\Projects\\Test\\resources\\Stevec.dat");

			XFILE* xfile = xfactory->Create_File();
			if(xfile)
				{
					if(xfile->Open(xpath))
						{
							XBUFFER xbuffer;
							
							xbuffer.Resize(xfile->GetSize());

							if(xfile->Read(xbuffer.Get(),xfile->GetSize()))
								{


								}

							xbuffer.Resize( (((xfile->GetSize()/16)+1)*16));


							cipher->Uncipher(xbuffer);

							xfile->Close();
						}

					xfactory->Delete_File(xfile);
				}

			delete cipher;
		}

	return true;
}	




/*-------------------------------------------------------------------
//  TEST::Test_AOSONGSensor
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2013 9:16:12
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_AOSONGSensor()
{
	int counter = 0;

	DIOGPIO*	diogpio = diofactory->CreateGPIO();
  if(diogpio) 
		{	
			if(diogpio->Ini()) 
				{
					//#define AOSONGSENSOR_DATAPIN	18
					#define AOSONGSENSOR_DATAPIN	16

					DIOTEMHUMSENSORAM2301* sensor = new DIOTEMHUMSENSORAM2301(diogpio, AOSONGSENSOR_DATAPIN, false);
					if(sensor)
						{
							float	temperature = 0.0;
							float humidity    = 0.0;
												
							do{	counter++;
					
									if(sensor->ReadDirect(temperature, humidity))																		
										{
											xconsole->Printf(__L("[%5d] Temperatura: %2.2f , Humedad: %2.2f												 \r"), counter, temperature, humidity);																			
										}
									 else 
										{
											xconsole->Printf(__L("[%5d] Error al leer, datos no disponibles.											 \r"), counter);																		
										}
										
									xsleep->Seconds(1);

								} while(!xconsole->KBHit());

							delete sensor;
						}
	
					diogpio->End();
				}

			diofactory->DeleteGPIO(diogpio);
		}

	return true;
}





/*-------------------------------------------------------------------
//	TEST::Test_Rele220V
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/02/2016 10:29:19
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Rele220V()
{
	#define RELE220V_DATAPIN	18

	DIOGPIO*	diogpio = diofactory->CreateGPIO();
  if(diogpio) 
		{	
			if(diogpio->Ini())
				{
				
					diogpio->SetMode(RELE220V_DATAPIN,  false);
					
					do{ 
							diogpio->Set(RELE220V_DATAPIN, true);  
							xsleep->Seconds(1);
							
							diogpio->Set(RELE220V_DATAPIN, false);  
							xsleep->Seconds(1);

					} while(!xconsole->KBHit());
						
					diogpio->End();
				}

			diofactory->DeleteGPIO(diogpio);
		}

	return true;
}




/*-------------------------------------------------------------------
//  TEST::Test_TimeString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/12/2013 16:42:17
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_TimeString()
{
	XDATETIME* xdatetimetest = xfactory->CreateDateTime();

	if(!xdatetimetest) return false;

	XSTRING  string;
	XSTRING  string2;
	XWORD		 modificator = 0;
	XRAND*   xrand;
	bool		 status = true;
	int      color  = 0; 

	xrand = xfactory->CreateRand();
	if(xrand) 
		{


			while(!xconsole->KBHit())
				{
					//modificator =	xrand->Between(0x0001, 0x0200);		// XDATETIME_FORMAT_FIRSTDAYOFWWEEK | XDATETIME_FORMAT_FIRSTDATEDAY | XDATETIME_FORMAT_ADDDATE |  XDATETIME_FORMAT_ADDTIME | XDATETIME_FORMAT_ADDDAYOFWWEEK | XDATETIME_FORMAT_TIMEWITHSECONDS;							

					modificator++;
					if(modificator>0x200) modificator = 0;
					
					string.Empty();
					string2.Empty();

					xdatetimetest->Read();
					xdatetimetest->GetDateTimeToString(modificator, string); 	
					xdatetimetest->GetDateTimeFromString(string, modificator);				
					xdatetimetest->GetDateTimeToString(modificator, string2); 

					if(string2.Compare(string)) color = 4; else color=1;
						
					XDEBUG_PRINTCOLOR(color, __L("%04X [1] Fecha Conversion: %s"), modificator, string.Get());
					XDEBUG_PRINTCOLOR(color, __L("     [2] Fecha Conversion: %s"), string2.Get());
					XDEBUG_PRINTCOLOR(color, __L("--------------------------------------------------------------------"));

					xsleep->Seconds(1);
					
				}

			xfactory->DeleteRand(xrand);

		} else status = false;

	xfactory->DeleteDateTime(xdatetimetest);

	return status;
}





/*-------------------------------------------------------------------
//  TEST::Test_EnumWifiRemote
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2014 12:38:56
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_EnumWifiRemote()
{
	DIOSTREAMENUMDEVICES* enumdevices;

	xconsole->Printf(__L("Inicializando...\n"));
		
	enumdevices = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_WIFI_REMOTE);
	if(enumdevices)
		{					
			enumdevices->Search();

			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Remote Wifi networks:"));
			
			xtimer->Reset();
			
			while(enumdevices->IsSearching())
				{
					xsleep->MilliSeconds(10);
				}

			DIOSTREAMDEVICEWIFI* device = NULL;

			for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
				{					
					DIOSTREAMDEVICEWIFI* device = (DIOSTREAMDEVICEWIFI*)enumdevices->GetDevices()->Get(c);
					if(device) device->DebugPrintInfo(xconsole);						
				}

			diofactory->DeleteStreamEnumDevices(enumdevices);			
		}

	xconsole->Printf(__L("Terminado.\n"));

	return true;
	return true;
}



/*-------------------------------------------------------------------
//  TEST::Test_XMPIntegers
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/03/2014 17:49:05
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_XMPIntegers()
{
	//int status = mpi_self_test(1);

	#define GCD_PAIR_COUNT  3

	static const int gcd_pairs[GCD_PAIR_COUNT][3] =	{	{				693,			 609,	 21 },
																										{			 1764,       868,  28 },
																										{ 768454923, 542167814,   1 }
																									};
	XMPINTEGER A;
	XMPINTEGER E;
	XMPINTEGER N;
	XMPINTEGER X;
	XMPINTEGER Y;
	XMPINTEGER U;
	XMPINTEGER V;
	int				 i;
	XSTRING		 value[2];	

	A.Ini(); 
	E.Ini(); 
	N.Ini(); 
	X.Ini();
	Y.Ini();
	U.Ini(); 
	V.Ini();

	A.SetFromString(16, __L("EFE021C2645FD1DC586E69184AF4A31ED5F53E93B5F123FA41680867BA110131944FE7952E2517337780CB0DB80E61AAE7C8DDC6C5C6AADEB34EB38A2F40D5E6"));
	E.SetFromString(16, __L("B2E7EFD37075B9F03FF989C7C5051C2034D2A323810251127E7BF8625A4F49A5F3E27F4DA8BD59C47D6DAABA4C8127BD5B5C25763222FEFCCFC38B832366C29E"));
	N.SetFromString(16, __L("0066A198186C18C10B2F5ED9B522752A9830B69916E535C8F047518A889A43A594B6BED27A168D31D4A52F88925AA8F5"));

	if(!X.Multiplication(&A, &N)) return false;

	U.SetFromString(16, __L("602AB7ECA597A3D6B56FF9829A5E8B859E857EA95A03512E2BAE7391688D264AA5663B0341DB9CCFD2C4C5F421FEC8148001B72E848A38CAE1C65F78E56ABDEFE12D3C039B8A02D6BE593F0BBBDA56F1ECF677152EF804370C1A305CAF3B5BF130879B56C61DE584A0F53A2447A51E"));

	xconsole->Printf(__L("  MPI test #1 (mul_mpi): " ));
	
	/*
	X.GetToString(16, value[0]);
	U.GetToString(16, value[1]);	
	xconsole->Printf(__L("\n%s\n%s\n"), value[0].Get(), value[1].Get());
	*/

	if(X.CompareSignedValues(U) != 0)
		{
			xconsole->Printf(__L("failed\n"));
			return false;

    } else xconsole->Printf(__L("passed\n"));

	if(!X.Division(&X, &Y, &A, &N)) return false;

	U.SetFromString(16, __L("256567336059E52CAE22925474705F39A94"));
  V.SetFromString(16, __L("6613F26162223DF488E9CD48CC132C7A0AC93C701B001B092E4E5B9F73BCD27B9EE50D0657C77F374E903CDFA4C642"));

  xconsole->Printf(__L("  MPI test #2 (div_mpi): "));

  if((X.CompareSignedValues(U)!=0) || (Y.CompareSignedValues(V)!=0))
    {
      xconsole->Printf(__L("failed\n"));
      return false;

    } else xconsole->Printf(__L("passed\n"));

	if(!X.SlidingWindowExponentiation(&A, &E, &N, NULL)) return false;
	
	U.SetFromString(16, __L("36E139AEA55215609D2816998ED020BBBD96C37890F65171D948E9BC7CBAA4D9325D24D6A3C12710F10A09FA08AB87"));

  xconsole->Printf(__L("  MPI test #3 (exp_mod): "));

  if((X.CompareSignedValues(U)!=0))
    {
      xconsole->Printf(__L("failed\n"));
      return false;

    } else xconsole->Printf(__L("passed\n"));


	if(!X.ModularInverse(&A, &N))  return false;

	U.SetFromString(16, __L("003A0AAEDD7E784FC07D8F9EC6E3BFD5C3DBA76456363A10869622EAC2DD84ECC5B8A74DAC4D09E03B5E0BE779F2DF61"));

  xconsole->Printf(__L("  MPI test #4 (inv_mod): "));

	if((X.CompareSignedValues(U)!=0))
    {
      xconsole->Printf(__L("failed\n"));
      return false;

    } else xconsole->Printf(__L("passed\n"));

	xconsole->Printf(__L("  MPI test #5 (simple gcd): "));


	for(i=0; i<GCD_PAIR_COUNT; i++)
    {
			if(!X.LeftSet(gcd_pairs[i][0])) return false;
      if(!Y.LeftSet(gcd_pairs[i][1])) return false;

			if(!A.GreatestCommonDivisor(&A, &X, &Y)) return false;

      if(A.CompareSignedValues(gcd_pairs[i][2] ) != 0)
        {
					xconsole->Printf(__L("failed at %d\n"), i );
          return true;
        }
    }

  xconsole->Printf(__L("passed\n"));

	/*
	XBUFFER xbuffer();
	XSTRING string;
	XSTRING string2;

	string = __L("003A0AAEDD7E784FC07D8F9EC6E3BFD5C3DBA76456363A10869622EAC2DD84ECC5B8A74DAC4D09E03B5E0BE779F2DF61");

	U.SetFromString(16, string.Get());
	U.GetToXBuffer(xbuffer);	
	A.SetFromXBuffer(xbuffer);
	A.GetToString(16,string2);
	*/


	A.End(); 
	E.End(); 
	N.End();
	X.End();
  Y.End(); 
	U.End(); 
	V.End();

  printf( "\n" );

	return true;
}




/*-------------------------------------------------------------------
//  TEST::Test_ConvertBase64
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/03/2014 16:22:49
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TEST::Test_ConvertBase64()
{
	//-------------------------------------------------------------

	XSTRING text;
	XSTRING textbase64;
	XSTRING result;
	
	text = __L("Hola radiola, esto es una prueba codificacion 64 bits");

	text.ConvertToBase64(textbase64);
	result.ConvertFromBase64(textbase64);

	xconsole->Printf(__L("Origin: %s\n"), text.Get());
	xconsole->Printf(__L("Base64: %s\n"), textbase64.Get());
	xconsole->Printf(__L("Result: %s\n"), result.Get());

	//-------------------------------------------------------------

	XMPINTEGER	number;
	XBUFFER			resultbuffer;
	XSTRING			resulthex;
	
	text = __L("MIIBOwIBAAJBALIexofzEbBrVfMju6r8dLcyosoGiAzNmPy+1zUpfVKStqRW3+TfaM90KohsBI1zx6rtgUxzN42bi3FnCWF/8XsCAwEAAQJAamSrnYT5KfHTMxpC/M0zQkJCrn3cQhmEqmOr1Jtlui3cgpvVDXrpqUAd0wYcug5awIzUusqY5FT27Pa+00pV8QIhAOakBtrRTUjJFKw6eV82S1m1r6DhaIDjqI0+chBEha8VAiEAxbRssRYHqSjgpNQWxbk7alDnXLZvXEEKoctv10ONwk8CIHnNEdmpQvk3R/WUg9hmYfCkZHW/1GKM18yPt8kjhr3pAiEAlpAfLoWuc98TpbN3XEECxKa0a+Dr0gAejrUViYSC0vUCIQCEBPoA0iS8gkHiO4mJhj04S8CjqBvPZENo7UHouqVRVA");
	result.ConvertFromBase64(text);
	result.ConvertToHexString(resulthex);
	
	number.SetFromString(16, resulthex.Get());
	number.GetToXBuffer(resultbuffer,number.GetSize());

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_ReadLDRDigitalIO
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2014 12:58:20
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_ReadLDRDigitalIO()
{	
	int counter = 0;

	DIOGPIO*	diogpio = diofactory->CreateGPIO();
  if(diogpio) 
		{	
			if(diogpio->Ini()) 
				{
					#define LDR_DATAPIN	18

					DIOLIGHTSENSORLDRANALOG* sensor = new DIOLIGHTSENSORLDRANALOG( diogpio, LDR_DATAPIN, true);
					if(sensor)
						{
							XDWORD level = 0;
																			
							do{	counter++;
					
									if(sensor->ReadFromCache(level))																		
										{
											xconsole->Printf(__L("[%5d] Light Level: %d                            \r"), counter, level);																			
										}
									 else 
									  {
											xconsole->Printf(__L("[%5d] Error al leer, datos no disponibles.			 \r"), counter);																		
										}
										
									xsleep->MilliSeconds(500);

								} while(!xconsole->KBHit());

							delete sensor;
						}
	
					diogpio->End();
				}

			diofactory->DeleteGPIO(diogpio);
		}


	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_RSASignCert
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/03/2014 9:32:08
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_RSASignCert()
{
	xconsole->PrintMessage(__L(" Init ..."),1,true,true);	

  DIOSTREAMTCPIPCONFIG * diostreamcfg = new DIOSTREAMTCPIPCONFIG();
	if(diostreamcfg) 
		{			
			diostreamcfg->SetMode(DIOSTREAMMODE_SERVER);			
			diostreamcfg->SetRemotePort(12021);

			DIOSTREAM* diostream = (DIOSTREAM*)diofactory->CreateStreamIO( diostreamcfg);
			if(diostream) 
				{	
					while(1)
						{
						  xconsole->PrintMessage(__L(" Connecting ..."),1,true,true);	

					 		if(diostream->Open()) 
								{
									while(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_CONNECTED)
										{
											xconsole->Printf(__L("    Wait conexion [%d] ...  \r") , xtimer->GetMeasureSeconds());													

											if(xconsole->KBHit()) break;	
										}

									if(diostream->GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED) xconsole->PrintMessage(__L(" Connected!               "),1,true,true);	
									
									while(diostream->GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED)
										{
											/*
											CASHMACHINEMULTICASHMSG mcmsg;

											memset(&mcmsg, 0, sizeof(CASHMACHINEMULTICASHMSG));

											XDWORD br = diostream->Read((XBYTE*)&mcmsg, sizeof(CASHMACHINEMULTICASHMSG));												
											if(br) 
												{
													XSTRING string;
													
												  string = (char*)mcmsg.data.request;	
													xconsole->Printf(__L("read [%s]"),string.Get(), string.Get());	

													XPATH									xpath;
													XPATH									xpathgeneric;	
													CIPHERKEYRSAPUBLIC*		publickey;
													CIPHERKEYRSAPRIVATE*	privatekey;	
													XBUFFER								cipherdata();
													XBUFFER								plaindata();
													CIPHERRSA*						cipherRSA;
													CIPHERFILEKEYS*				filekeys;

													cipherRSA = new CIPHERRSA();
													if(cipherRSA)
														{
															XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GENERIC, xpathgeneric);
															xpath.Create(3 , xpathgeneric.Get(), __L("pimaticmulticash"),__L(".key"));
	
															filekeys = new CIPHERFILEKEYS( xpath);	
															if(filekeys)
																{
																	publickey  = (CIPHERKEYRSAPUBLIC*) filekeys->GetKey(CIPHERKEYTYPE_PUBLIC);
																	privatekey = (CIPHERKEYRSAPRIVATE*)filekeys->GetKey(CIPHERKEYTYPE_PRIVATE);

																	if(privatekey && publickey)
																		{
																			cipherRSA->SetKey(publickey);
																			cipherRSA->SetKey(privatekey);
															
																			cipherdata.Add(mcmsg.sign, 128);

																			cipherRSA->Uncipher(cipherdata, CIPHERKEYTYPE_PUBLIC); 

																			memcpy(mcmsg.sign, cipherRSA->GetResult()->Get(), cipherRSA->GetResult()->GetSize());

																			HASHSHA1* sha1 = new HASHSHA1();
																			if(!sha1) return false;

																			sha1->Do((XBYTE*)&mcmsg.data, sizeof(CASHMACHINEMULTICASHMSGDATA));

																			if(sha1->GetResult()->Compare(&mcmsg.sign[15], 20))
																				{
																					xconsole->PrintMessage(__L(" Certificacion valida."),1,true,true);					
																				} 
																			 else xconsole->PrintMessage(__L(" Certificacion INVALIDA."),1,true,true);					

																			XBYTE answer[2] = { 0x31, 0x02 };

																			diostream->Write(answer, 1);

																			delete sha1;

																		}
																}														
														}

													delete filekeys;
													delete cipherRSA;
												}
											*/

											if(xconsole->KBHit()) break;					
										}

									diostream->Close();
									xconsole->PrintMessage(__L(" Disconnected."),1,true,true);																		
								}

							if(xconsole->KBHit()) break;																
						}

					diofactory->DeleteStreamIO(diostream);
				}			

			delete diostreamcfg;			
		}
					
	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_StringFormat
*/	 
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2014 10:56:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_StringFormat()
{	
	XSTRING _params;


	_params = __L("-S egmmanager -d -m /mnt/usbpendrive/egmmanager");



	#define MAXNPARAMS  20

	char* param[MAXNPARAMS] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
															NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL 
														};
	int   start     = 0;
	bool  endfound  = false;
																																			
	for(int c=0; c<MAXNPARAMS; c++)
		{
			XSTRING string;			
			int			found = _params.Find(__L(" "), true, start);

			if(found == XSTRING_NOTFOUND)
				{
					_params.Copy(start, string);					
					endfound = true;
				}
			 else
				{
					_params.Copy(start, found, string);
					start = found+1;
				}

			string.CreateOEM(param[c]);  

			if(endfound) break;
		}
	



	for(int c=0; c<10; c++)
		{
			delete [] param[c];
		}
 


	/*
	double   red;
	double   green;
	double   blue;
	double   alpha;
	XSTRING  string;

	string = __L("0.65,0.07,0.40,1.00");

	string.UnFormat(__L("%f,%f,%f,%f"), &red, &green, &blue, &alpha);
	

	XSTRING s(__L("Model::Point_MachineNumber"));

	int re=s.Find(__L("Point_"),false,0);
	if(re!=XSTRING_NOTFOUND)
		{
			XDEBUG_PRINTCOLOR(0,__L("found"));
		}

	*/

	/*
	XSTRING line;
	int			version;
	int			subversion;
	XSTRING	method(DIOWEBHEADER_MAXLINE);
	XSTRING resource(DIOWEBHEADER_MAXLINE);

	line = __L("GET /header.png HTTP/1.1");

	line.UnFormat(__L("%s %s HTTP/%d.%d"), method.Get(), resource.Get(), &version, &subversion);
	*/

	/*
	XSTRING s;

	s.Set(__L("resources/fonts/Arial Black Italic.ttf,65544,88B5FEDC"));

	XSTRING file,path,path1;
	file.AdjustSize(_MAXSTR);
	path.AdjustSize(_MAXSTR);
	path1.AdjustSize(_MAXSTR);
//	s.UnFormat(__L("%[^/]%[^/]%[^/],65544,88B5FEDC"),path.Get(),path1.Get(),file.Get());
*/
	/*
	XDWORD a,b;

	xconsole->PrintMessage(__L("\n\nUnformating resources/fonts/Arial Black Italic.ttf,65544,88B5FEDC \nwith mask %%s%%s%%s,%%d,%%x"),0,0,0);
		s.UnFormat(__L("%s%s%s,%d,%x"),path.Get(),path1.Get(),file.Get(),&a,&b);
			xconsole->PrintMessage(__L("\n\nFirst string:"),0,0,0);
			xconsole->PrintMessage(path.Get(),0,0,0);
			xconsole->PrintMessage(__L("\r\n"),0,0,0);
			xconsole->PrintMessage(__L("Second string:"),0,0,0);
			xconsole->PrintMessage(path1.Get(),0,0,0);
			xconsole->PrintMessage(__L("\r\n"),0,0,0);
			xconsole->PrintMessage(__L("Third string:"),0,0,0);
			xconsole->PrintMessage(file.Get(),0,0,0);
			xconsole->PrintMessage(__L("\r\n"),0,0,0);						
			XSTRING number;
			number.Format(__L("A: %d  B: %x"),a,b);
			xconsole->PrintMessage(number.Get(),0,0,0);						
			xconsole->PrintMessage(__L("\r\n"),0,0,0);						
			


		char cs[]="resources/fonts/Arial Black Italic.ttf,65544,88B5FEDC";
		char spath[256],spath1[256],sfile[256];
		XDWORD sa=0,sb=0;

	xconsole->PrintMessage(__L("\n\nScanf resources/fonts/Arial Black Italic.ttf,65544,88B5FEDC \nwith mask %%s%%s%%s,%%d,%%x"),0,0,0);	
		sscanf(cs,"%s%s%s,%d,%x",spath,spath1,sfile,&sa,&sb);
			xconsole->PrintMessage(__L("\n\nFirst string:"),0,0,0);
			xconsole->PrintMessage(XSTRING(spath).Get(),0,0,0);
			xconsole->PrintMessage(__L("\r\n"),0,0,0);
			xconsole->PrintMessage(__L("Second string:"),0,0,0);
			xconsole->PrintMessage(XSTRING(spath1).Get(),0,0,0);
			xconsole->PrintMessage(__L("\r\n"),0,0,0);
			xconsole->PrintMessage(__L("Third string:"),0,0,0);
			xconsole->PrintMessage(XSTRING(sfile).Get(),0,0,0);
			xconsole->PrintMessage(__L("\r\n"),0,0,0);

			number.Format(__L("A: %d  B: %x"),a,b);
			xconsole->PrintMessage(number.Get(),0,0,0);						
			xconsole->PrintMessage(__L("\r\n"),0,0,0);		
	*/
		
	/*
	XSTRING string;

	if(string.Compare(__L("")))
		{
			int a=0;
			a++;
		}
	*/


	/*
	int					a;
	XDWORD			b;
	XQWORD			c;
	float				d;
	double			e;
	XSTRING			string;
	
	string = __L("459340.66");

//a = string.Fast_ConvertToInt();
//b = string.Fast_ConvertToDWord();
//c = string.Fast_ConvertToQWord();
//d = string.Fast_ConvertToFloat();
  e = string.Fast_ConvertToDouble();
	*/




	/*
	XSTRING eurosstr;
	XSTRING centsstr;
	double	integerpart; 
	double	decimalpart;
	double  value  = 1.99;

	bool haveeuros = eurosstr.ConvertFromDoubleToSpanishText(value, true , false, integerpart, decimalpart);
	if(haveeuros)  
		{
			eurosstr += __L(" euro");
			if(integerpart>1) eurosstr += __L("s");
		}

	bool havecents = centsstr.ConvertFromDoubleToSpanishText(value, false, true, integerpart, decimalpart);
	if(havecents) 
		{
			centsstr += __L(" centimos");
			if(decimalpart>1) centsstr += __L("s");
		}

	xconsole->Printf(__L("%s%s%s.\n"), haveeuros?eurosstr.Get():__L(""), (haveeuros && havecents)?__L(" con "):__L(""), havecents?centsstr.Get():__L(""));			
		
	return true;
	*/



	/*
	XSTRING string;

	string = __L("\t\tPosition: 0,0,-50");

	string.DeleteCharacter(__C(' '), XSTRINGCONTEXT_ATFIRST);
	*/

	
	/*
	XSTRING test;
	int			data1 = 0;
	//XCHAR		char1;
	//XCHAR		char2;
	XSTRING string;
	int			data2 = 0;	
	float   data  = 3434.50f;
	//int     divisor;
				

	XRAND*  xrand;

	xrand = xfactory->CreateRand();
	if(!xrand) return false;

//	while(!xconsole->KBHit())
		{
							
			//data += (int)xrand->Max(5000);
			//divisor = (int)xrand->Max(125);
			//if(divisor) data /= divisor;

			//xconsole->Printf(__L(" %8d.%02d - ") , (int)Truncate(data), (int)Truncate(Fraction(data)*100));												

			xconsole->Printf(__L(" %6.2f - ") , data);												

			test.ConvertFromDoubleToSpanishText(data, true, true);

			xconsole->Printf(__L("%s. \n") , test.Get());													
		}


	test = __L("Hola radiola 2345AB34 ppp fjkdjf");

	string.AdjustSize(64);

	test.UnFormat(__L("Hola radiola %04d%c%c%02d%sfjkdjf"), &data1, &char1, &char2, &data2, string.Get());

	string.AdjustSize();


	xfactory->DeleteRand(xrand);
	
	return true;
	*/

		return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_I2CStream
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/04/2014 22:38:03
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_I2CDevices()
{	
	xconsole->PrintMessage(__L(" I2C Test START..."),1,true,true);	
	
	/*
	DIOI2C3DACCELEROMETERLSM303DLHC* accelerometerlsm303dlhc = new DIOI2C3DACCELEROMETERLSM303DLHC;
	if(accelerometerlsm303dlhc) 
		{
			DIOI2C3DCOMPASSLSM303DLHC* compasslsm303dlhc = new DIOI2C3DCOMPASSLSM303DLHC;
			if(compasslsm303dlhc)
				{			
					if(accelerometerlsm303dlhc->Ini(1, 0x18, 10))
						{
							if(compasslsm303dlhc->Ini(1, 0x1e, 10))
								{		
									xconsole->PrintMessage(__L(" Ini ..."),1,true,true);	

									if(accelerometerlsm303dlhc->Configure())
										{	
											if(compasslsm303dlhc->Configure())  
												{													
																		
													DIOI2CLSM303DLHCAJUST lsm303dlhcajust(accelerometerlsm303dlhc, compasslsm303dlhc);

													// example calibration
													lsm303dlhcajust.SetOffset(29.50f, -0.50f, 4.00f); 
													lsm303dlhcajust.SetScale(1.00f, 1.03f, 1.21f);		
													
			
													while(!xconsole->KBHit())
														{												
															
															XWORD a_x = 0;
															XWORD a_y = 0; 
															XWORD a_z = 0;
															
															XWORD c_x = 0;
															XWORD c_y = 0;
															XWORD c_z = 0;
															
															float aa_x = 0;
															float aa_y = 0; 
															float aa_z = 0;
															float ac_x = 0;
															float ac_y = 0;
															float ac_z = 0;
															

															bool status;

															status = accelerometerlsm303dlhc->Read(a_x, a_y, a_z);		
															if(status) status = compasslsm303dlhc->Read(c_x, c_y, c_z);							
															if(status) status = lsm303dlhcajust.Adjust(aa_x, aa_y, aa_z, ac_x, ac_y, ac_z);

															if(status)
																{															
																	xconsole->Printf(__L("[Accel: %d,%d,%d  Comp: %d,%d,%d]  Ajust [Accel: %5.3f,%5.3f,%5.3f  Comp: %5.3f,%5.3f,%5.3f]          \r"), a_x, a_y, a_z, c_x, c_y, c_z, aa_x, aa_y, aa_z, ac_x, ac_y, ac_z);	
																} 
															 else
																{
																	xconsole->Printf(__L(" Error read!!!!																																			          	    \r"));										
																}		
														} 

												} else xconsole->Printf(__L(" Error en la configuracion!!!  \n"));	
							
											accelerometerlsm303dlhc->End();

										}  else xconsole->Printf(__L(" Error en la configuracion!!!  \n"));

									compasslsm303dlhc->End();
								}
						}
				}
			
			delete compasslsm303dlhc;
		}

	delete accelerometerlsm303dlhc;
		
	xconsole->PrintMessage(__L(" I2C Test END..."),1,true,true);	
	*/

	/*
	DIOI2C6AXISTRACKINGLSM303DLHC * lsm303dlhc = new DIOI2C6AXISTRACKINGLSM303DLHC();
	if(lsm303dlhc) 
		{
			if(lsm303dlhc->Ini(1, 0x18, 0x1e, 10))
				{
					lsm303dlhc->SetOffset(29.50f, -0.50f, 4.00f); 
					lsm303dlhc->SetScale(1.00f, 1.03f, 1.21f);		
															
					while(!xconsole->KBHit())
						{																										
							float ypos = lsm303dlhc->Heading();
							
							xconsole->Printf(__L(" %6.2f \n"), ypos);	
              
              xsleep->MilliSeconds(200);
						} 

					lsm303dlhc->End();												
				}

			delete lsm303dlhc;
		}
		
	xconsole->PrintMessage(__L(" I2C Test END..."),1,true,true);	
	*/
	 	
	// Motores
	/*
	DIOI2CPWMCONTROLERPCA9685* pca9685 = new DIOI2CPWMCONTROLERPCA9685;
	if(!pca9685) return false;
	
	if(pca9685->Ini(1, 0x40, 10))
		{
			xconsole->PrintMessage(__L(" Ini ..."),1,true,true);	

			if(!pca9685->SetPWMFrequency(60)) 
				{
					xconsole->Printf(__L(" Error PWM frecuency \n"));						
				}

			int   motor[2] = { 0, 0 };
			bool  sense[2] = { false, false };
			XWORD step     = 10;
			
			while(1)
				{													
					int key = 0;
					if(xconsole->KBHit()) 
						{
							key = xconsole->GetChar();
																									
							switch(key)
								{									
									case  97: motor[0] += step;		break;
									case 115: motor[0] -= step;		break;
									case 119: motor[1] += step;		break;
									case 122: motor[1] -= step;		break;	

									case  49: sense[0]  = !sense[0]; break;
									case  50: sense[1]  = !sense[1]; break;
								}

							XDEBUG_PRINTCOLOR(2, __L("[Key: %d] Motor 1 %d %s - Motor 2 %d %s"), key, motor[0], sense[0]?__L("A"):__L("T"), motor[1], sense[1]?__L("A"):__L("T"));

							for(int c=0; c<2; c++)
								{
									if(motor[c] < 0)		 motor[c] = 0;
									if(motor[c] >= 4096) motor[c] = 4090;
								}

							if(key != 32) 
								{
									if(sense[0])
										{
											pca9685->SetPWM(11, 0, motor[0]);
											pca9685->SetPWM(10, 0, 0);
											xsleep->MilliSeconds(50);							
										}	
									 else
										{
											pca9685->SetPWM(11, 0, 0);
											pca9685->SetPWM(10, 0, motor[0]);							
										}

									if(sense[1])
										{
											pca9685->SetPWM(9, 0, motor[1]);
											pca9685->SetPWM(8, 0, 0);						
										}	
									 else
										{
											pca9685->SetPWM(9, 0, 0);
											pca9685->SetPWM(8, 0, motor[1]);							
										}
									
									//GetTimerGlobal()->MilliSeconds(50);		
								}
							 else
								{						
									pca9685->SetPWM(11, 0, 0);
									pca9685->SetPWM(10, 0, 0);	
									pca9685->SetPWM(9 , 0, 0);
									pca9685->SetPWM(8 , 0, 0);

									//GetTimerGlobal()->MilliSeconds(50);		

									break;
								}
						}					
				}
				
			if(pca9685->End())
				{
					xconsole->PrintMessage(__L("\n End ..."),1,true,true);	
				}
			
		}

	delete pca9685;
	*/
	
	
	 	
	// servos til
	/*
	DIOI2CPWMCONTROLERPCA9685* pca9685 = new DIOI2CPWMCONTROLERPCA9685;
	if(!pca9685) return false;
	
	if(pca9685->Ini(__L("/dev/i2c-1"), 0x40, 10))
		{
			xconsole->PrintMessage(__L(" Ini ..."),1,true,true);	

			if(!pca9685->SetPWMFrequency(60)) 
				{
					xconsole->Printf(__L(" Error PWM frecuency \n"));						
				}

			XWORD x			= 355;
			XWORD y			= 355;
			XWORD x_old = 0;
			XWORD y_old = 0;
			XWORD step  = 5;

			
			while(1)
				{
					if(x<140) x = 140;
					if(x>600) x = 600;

					if(y<150) y = 150;
					if(y>480) y = 480;

					if(x != x_old)
						{
							x_old = x;
							if(!pca9685->SetPWM(11, 0, x)) xconsole->Printf(__L(" Error PWN \n"));									
							xsleep->MilliSeconds(50);

							xconsole->Printf(__L("x: %03d, y: %03d        \r"), x, y);
						}	

					if(y != y_old)
						{
							y_old = y;

							if(!pca9685->SetPWM(10, 0, y)) xconsole->Printf(__L(" Error PWN \n"));									
							xsleep->MilliSeconds(50);

							xconsole->Printf(__L("x: %03d, y: %03d        \r"), x, y);
						}	

					int key = 0;
					if(xconsole->KBHit()) key = xconsole->GetChar();
						
					switch(key)
						{
							case  97: x += step; break;
							case 115: x -= step; break;
							case 119: y += step; break;
							case 122: y -= step; break;						  
						}
					
					if(key == 120) break;
					
				}
				
			if(pca9685->End())
				{
					xconsole->PrintMessage(__L("\n End ..."),1,true,true);	
				}
			
		}

	delete pca9685;
	*/


	// LED Jackpot
	
	/*
	DIOI2CPWMCONTROLERPCA9685* pca9685 = new DIOI2CPWMCONTROLERPCA9685;
	if(!pca9685) return false;
	
	if(pca9685->Ini(__L(1, 0x40, 10))
		{
			xconsole->PrintMessage(__L(" Ini device ..."), 1, true, true);	
		
			if(!pca9685->SetPWMFrequency(60)) 
				{
					xconsole->Printf(__L(" Error PWM frecuency \n"));						
				}
						
			
			for(int c=0;c<16; c++)
				{
					if(!pca9685->SetPWM(c, 0, 0))
						{
							xconsole->Printf(__L("Channel %02d Error Set PWM \n"), c);						
						}
				}
			


			//pca9685->SetPWM(15, 0, 0); 
			//pca9685->SetPWM(14, 0, 0);
			//pca9685->SetPWM(13, 0, 0);



			XBYTE red			= 255;
			XBYTE green		= 255;
			XBYTE blue	  = 255;
			
			pca9685->SetPWM(0, 0, ((red*1200)/255)); 
			pca9685->SetPWM(1, 0, ((green*1200)/255));
			pca9685->SetPWM(2, 0, ((blue*1200)/255));
			
			while(!xconsole->KBHit())				
				{				
					if(xtimer) xsleep->MilliSeconds(10);
				}

			
			while(!xconsole->KBHit())				
				{
					int c = 0;
					while(!xconsole->KBHit())				
						{
							pca9685->SetPWM(15, c, 0); 
							pca9685->SetPWM(14, c, 0);
							pca9685->SetPWM(13, c, 0);
												
							c++;
							if(c>800) c=0;
						}
				}
			
			pca9685->SetPWM(0, 0, 0); 
			pca9685->SetPWM(1, 0, 0);
			pca9685->SetPWM(2, 0, 0);	
			if(pca9685->End())
				{
					
					xconsole->PrintMessage(__L(" End ..."),1,true,true);	
				}
			
		}

	delete pca9685;
	*/


	/*
	DIOI2CGPIOPCF8574* pcf8574 = new DIOI2CGPIOPCF8574;
	if(!pcf8574) return false;

	XBYTE data;
	
	xconsole->PrintMessage(__L(" Open device ..."),1,true,true);	

//if(pcf8574->Ini(1, 0x20, 10))
	if(pcf8574->Ini(1, 0xB4, 10))
		{
			xconsole->PrintMessage(__L(" Ini ..."),1,true,true);	
			
			while(!xconsole->KBHit())
				{		
					//XBYTE data = 0x01;
					//XBYTE data2;

					for(int c=0; c<8; c++)
						 {							
							 data = 0x01;

							 if(c) data <<=c;

							 pcf8574->Write(data); 

							 
							 //pcf8574->Read(data2); 							 
							 //xconsole->Printf(__L("pin %d  [%02X]    \r"), data, data2);
							 
							
							 xsleep->MilliSeconds(150);			

							 if(xconsole->KBHit()) break;								
						}					
				}
					
			if(pcf8574->End())
				{
					xconsole->PrintMessage(__L(" End ..."),1,true,true);	
				}

		} else xconsole->PrintMessage(__L(" Error!"),1,true,true);	

	delete pcf8574;
	*/


	/*
	DIOI2CGPIOMCP2317* mcp2317 = new DIOI2CGPIOMCP2317;
	if(!mcp2317) return false;
	
	if(mcp2317->Ini(1, 0x23, 10))
		{
			xconsole->PrintMessage(__L(" Ini ..."),1,true,true);	
			
			mcp2317->Configure();

			XBYTE rvalue = 0;
		  XBYTE wvalue;

			while(!xconsole->KBHit())
				{																
					for(int d=0; d<2; d++)
						{	
							wvalue = (!d)?0x01:0x80;

							for(int c=0; c<8; c++)
								{					
									
									xconsole->Printf(__L("Inputs: "));
									if(mcp2317->ReadInputs(DIOI2CGPIOMCP2317PORT_A, rvalue)) 
													xconsole->Printf(__L("%02X     \r"), rvalue); 
									   else xconsole->Printf(__L("No read. \r"), rvalue); 
									

									mcp2317->WriteOutputs(DIOI2CGPIOMCP2317PORT_B, wvalue);
									
									if(!d) wvalue <<= 1; else wvalue >>= 1;

									xsleep->MilliSeconds(150);			

									if(xconsole->KBHit()) break;
								}
						}					
				}
					
			if(mcp2317->End())
				{
					xconsole->PrintMessage(__L(" End ..."),1,true,true);	
				}
		}

	delete mcp2317;
	*/



	/*
	DIOI2CTEMHUMSENSORAM2315* sensorAM2315 = new DIOI2CTEMHUMSENSORAM2315;
	if(!sensorAM2315) return false;

	if(sensorAM2315->Ini(1, 0x5c, 10))
		{
			xconsole->Printf(__L(" Init ...\n"));	
						
			float	temperature = 0.0;
			float humidity    = 0.0;
			int   counter     = 0;
												
			do{	counter++;
					
					if(sensorAM2315->Read(temperature, humidity))																		
						{
							xconsole->Printf(__L("[%5d] Temperatura: %2.2f , Humedad: %2.2f												 \r"), counter, temperature, humidity);																			
						}
					 else 
						{
							xconsole->Printf(__L("[%5d] Error al leer, datos no disponibles.											 \r"), counter);																		
						}
										
					xsleep->Seconds(3);

				} while(!xconsole->KBHit());
									
			xconsole->Printf(__L("\n End.\n"));	

			sensorAM2315->End();
		}

	delete sensorAM2315;
	*/


	/*	
	DIOI2CADDACONVERTERPCF8591* converterPCF8591 = new DIOI2CADDACONVERTERPCF8591;
	if(!converterPCF8591) return false;

	if(converterPCF8591->Ini(1, 0x48, 10))
		{
		
			xconsole->Printf(__L(" Init ...\n"));	
		
			
			while(!xconsole->KBHit())
				{
					int c = 175;

					for(; c<225; c++)
						{
							converterPCF8591->WriteDAChannel((XBYTE)c);														
						}

					for(; c>175; c--)
						{
							converterPCF8591->WriteDAChannel((XBYTE)c);														
						}
				}

			converterPCF8591->WriteDAChannel(0);	
			

						
			//while(!xconsole->KBHit())
			//	{					
			//		if(!converterPCF8591->ReadADChannels(values)) 
			//			{
			//				xconsole->Printf(__L(" Read Channels Error!!!  \n"));
			//				break;
			//			} 

			//		xconsole->Printf(__L(" [%02X] %2.3f [%02X] [%02X] [%02X]   \r"),  values[0], (values[0]* 3.3)/255, values[1], values[2], values[3]);					
			//				
			//		xsleep->MilliSeconds(100);							
			//	}	
			//		
			//converterPCF8591->WriteDAChannel((XBYTE)value[0]);			
			
						
			xconsole->Printf(__L("\n End.\n"));	

			converterPCF8591->End();
		}

	delete converterPCF8591;
	*/

	/*
	DIOI2CEEPROM24XXX* eeprom24xxx = new DIOI2CEEPROM24XXX;
	if(!eeprom24xxx) return false;

	eeprom24xxx->SetType(DIOI2CEEPROM24XXXTYPE_08);

	if(eeprom24xxx->Ini(1, 0x50, 10))
		{
			XBUFFER  datawrite;
			XBUFFER  dataread;
			XSTRING  string;
			
			xconsole->Printf(__L(" Init ...\n"));	
																													
			XBYTE dataDPO2COMP[] = {	0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x50, 0x4F, 0x32, 0x43, 0x4F, 0x4D, 0x50, 0x00, 0xFF, 0xFF, 0xFF,  // DPO2COMP
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0x76, 0x31, 0x2E, 0x30, 0x30, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // v1.00
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  																
														 };
										
			XBYTE dataDPO2EMBD[] = {	0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x50, 0x4F, 0x32, 0x45, 0x4D, 0x42, 0x44, 0x00, 0xFF, 0xFF, 0xFF,  // DPO2EMBD
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0x76, 0x31, 0x2E, 0x30, 0x30, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // v1.00
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  																
														 };
						
			XBYTE dataDPO2AUTO[] = {	0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x50, 0x4F, 0x32, 0x41, 0x55, 0x54, 0x4F, 0x00, 0xFF, 0xFF, 0xFF,	 // DPO2AUTO
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0x76, 0x31, 0x2E, 0x30, 0x30, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // v1.00
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  																
														 };

			XBYTE dataDPO2EMPTY[] = {	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  
																0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  																
														 };
			
			//datawrite.Add(dataDPO2EMBD , sizeof(dataDPO2EMBD));																
			//datawrite.Add(dataDPO2COMP , sizeof(dataDPO2COMP));					
			datawrite.Add(dataDPO2AUTO , sizeof(dataDPO2AUTO));

			XDWORD c = 0;

			if(!eeprom24xxx->Write(0x0, datawrite)) 						
							 xconsole->Printf(__L("Write %05d Error ...\n"), c);				
					else xconsole->Printf(__L("Write %05d Ok!\n"), c);			

		
			//if(!eeprom24xxx->Write(0x0, dataDPO2COMP, 16)) 						
			//				 xconsole->Printf(__L("Write %05d Error ...\n"), c);				
			//		else xconsole->Printf(__L("Write %05d Ok!\n"), c);									
			//
		

			for(c=0; c<eeprom24xxx->GetSize(); c+=128)
				{		
					dataread.Delete();			

					if(!eeprom24xxx->Read(c, 128, dataread)) 						
								 xconsole->Printf(__L("Read %5d: Error ...\n"), c);				
						else xconsole->Printf(__L("Read %5d: Ok!\n"), c);			
		
					XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(XDEBUG_COLORGREEN), dataread);
				}						

			eeprom24xxx->End();
		}

	delete eeprom24xxx;
	*/

	/*
	DIOI2COLEDSCREENSSD1306* screenOLEDSSD1306 = new DIOI2COLEDSCREENSSD1306();
	if(!screenOLEDSSD1306) return false;
	
	if(screenOLEDSSD1306->Ini(1, 0x78, 10))
		{
			xconsole->Printf(__L("Connect I2C device.. \n"));

			screenOLEDSSD1306->End();

		} else xconsole->Printf(__L("Error to connect I2C device.. \n"));
	*/

	/*
	DIOI2CSENSORLIGHTTSL2561* lightsensorTSL2561 = new DIOI2CSENSORLIGHTTSL2561();
	if(!lightsensorTSL2561) return false;
	
	if(lightsensorTSL2561->Ini(1, 0x49, 3))
		{
			bool status;

			xconsole->Printf(__L("Connect I2C device.. \n"));

			lightsensorTSL2561->SetIntegrationTime(DIOI2CSENSORLIGHTTSL2561INTEGRATIONTIME_101MS);
			lightsensorTSL2561->SetGain(DIOI2CSENSORLIGHTTSL2561GAIN_1X);
		
			while(!xconsole->KBHit())
				{
					XWORD		fullspectrum;
					XWORD		infrared;
					XDWORD	lux;

					status = lightsensorTSL2561->Get(fullspectrum, infrared, lux, true);
					
					xconsole->Printf(__L("Broadband: %d - Inflared: %d - Lux: %d  [%s]\n"), fullspectrum, infrared, lux, status?__L("Ok."):__L("Error!"));					
				}

			lightsensorTSL2561->End();

		} else xconsole->Printf(__L("Error to connect I2C device.. \n"));
	*/
	
	/*
	DIOSTREAMI2CCONFIG	diostreami2cconfig;
	DIOSTREAMI2C*			  diostreami2c = NULL;      

	diostreami2cconfig.SetMode(DIOSTREAMMODE_MASTER);
  diostreami2cconfig.SetPort(1);
	diostreami2cconfig.SetLocalDeviceNameByPort();
	diostreami2cconfig.SetRemoteAddress(0x49);  
  
	diostreami2c = (DIOSTREAMI2C*)diofactory->CreateStreamIO(&diostreami2cconfig);
	if(diostreami2c)
		{	
			if(diostreami2c->Open()) 
				{
					xconsole->PrintMessage(__L(" Open device ..."),1,true,true);	

					XBYTE data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
					                          
          diostreami2c->Write(data, 8);        
					diostreami2c->WaitToFlushOutXBuffer(3);

					
					if(diostreami2c->WaitToFilledReadingBuffer(4, 3))
						{
							XBYTE data2[4];

							if(diostreami2c->Read(data2, 4))
								{
									XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(2), data2, 4); 
								}
						}
					
					diostreami2c->Close();

					xconsole->PrintMessage(__L(" Closen device ..."),1,true,true);	
				}
		}	

	diofactory->DeleteStreamIO(diostreami2c);
	*/


	DIOSTREAMI2CCONFIG	diostreami2cconfig;
  DIOSTREAMI2C*			  diostreami2c;      
    
  diostreami2cconfig.SetMode(DIOSTREAMMODE_MASTER);
  diostreami2cconfig.SetPort(1); 
	diostreami2cconfig.SetLocalDeviceNameByPort();
	diostreami2cconfig.SetRemoteAddress(0x30);  
  
	diostreami2c = (DIOSTREAMI2C*)diofactory->CreateStreamIO(&diostreami2cconfig);
	if(!diostreami2c) return false;
	
	if(!diostreami2c->Open()) return false;
    
 	XBYTE  data[16] = { 1,2,3,4,5,6,7,8,
											0,0,0,0,0,0,0,0 
										};  
	//int    br;
  bool   exit = false;

	while(!exit)
		{       
     	 diostreami2c->Write(data, 8);   
       diostreami2c->WaitToFlushOutXBuffer(3);
			 
			 //XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(2), data, 4);               			 
    }
    
	diostreami2c->Close();
  diofactory->DeleteStreamIO(diostreami2c);

	return true;
	
}



/*-------------------------------------------------------------------
//	TEST::Test_PiFace
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2014 16:50:10
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_PiFace()
{
	xconsole->PrintMessage(__L(" Piface Test ..."),1,true,true);	

	DIOPIFACE* piface = new DIOPIFACE;
	if(!piface) return false;

	if(piface->Ini(0, 10))
		{
			xconsole->PrintMessage(__L(" Ini ..."),1,true,true);	
			
		  XBYTE rvalue = 0x00;
		  XBYTE wvalue;

			while(!xconsole->KBHit())
				{																
					for(int d=0; d<2; d++)
						{	
							wvalue = (!d)?0x01:0x80;

							for(int c=0; c<8; c++)
								{	
									xconsole->Printf(__L("Inputs: "));
									if(piface->ReadInputs(rvalue)) 
													xconsole->Printf(__L("%02X     \r"), rvalue); 
									   else xconsole->Printf(__L("No read. \r"), rvalue); 
								
									piface->WriteOutputs(wvalue);
									
									if(!d) wvalue <<= 1; else wvalue >>= 1;

									xsleep->MilliSeconds(150);			

									if(xconsole->KBHit()) break;
								}
						}
					
				}
					
			if(piface->End())
				{
					xconsole->PrintMessage(__L(" End ..."),1,true,true);	
				}
		}

	delete piface;

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_Compress
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2014 16:50:00
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Compress()
{
	XFILEUNZIP xfileunzip;
	XPATH			 xpathzip;

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathzip);
	xpathzip += __L("prueba.zip");		

	if(xfileunzip.Open(xpathzip))
		{
			xfileunzip.DelFile(0);

			xfileunzip.Close();
		}
		
	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_DynDNS
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/05/2014 17:36:44
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DynDNS()
{
	DIODYNDNS							dyndns;
	DIOURL								url;
	DIOIP*								newip = NULL;
	XSTRING								login;
	bool                  needchange;
	XSTRING								password;
	DIODYNDNS_STATUSCODE	statusupdate;
	bool									status = false;
	
	url.Set(__L("abrahamvelez.dyndns.org"));

	xconsole->Printf(__L("Update %s, "), url.Get()); 

	newip = new DIOIP();
	if(!newip) return false;
	
	dyndns.GetLogin()->Set(__L("sirgraham"));
	dyndns.GetPassword()->Set(__L("sgraham01"));
	
	if(dyndns.IsChangedPublicIP(url, needchange, newip))
		{
			XSTRING IPstring;

			newip->GetXString(IPstring);

			xconsole->Printf(__L("public IP %s "), IPstring.Get()); 

			if(needchange)
				{
					xconsole->Printf(__L(" status change : ")); 

					statusupdate = dyndns.Update(url, (*newip));
					switch(statusupdate)
						{
							case DIODYNDNS_STATUSCODE_UNKNOWN			: xconsole->Printf(__L("Unknown result from dyndns service."));																																																					break;
							case DIODYNDNS_STATUSCODE_NOTCONNECT	: xconsole->Printf(__L("Not connected."));																																																															break; 
							case DIODYNDNS_STATUSCODE_GOOD				: xconsole->Printf(__L("[good] The update was successful, and the hostname is now updated."));																																					break; 
							case DIODYNDNS_STATUSCODE_NOCHG				: xconsole->Printf(__L("[nochg] The update changed no settings, and is considered abusive. (Additional nochg updates will cause the hostname to become blocked)."));		break; 
							case DIODYNDNS_STATUSCODE_BADSYS			: xconsole->Printf(__L("[badsys] The system parameter given is not valid. Valid system parameters are dyndns, statdns and custom."));																		break; 
							case DIODYNDNS_STATUSCODE_BASAGENT		: xconsole->Printf(__L("[badagent] The user agent that was sent has been blocked for not following these specifications or no user agent was specified."));							break; 
							case DIODYNDNS_STATUSCODE_BADAUTH			: xconsole->Printf(__L("[badauth]	The username or password specified are incorrect."));																																									break; 
							case DIODYNDNS_STATUSCODE_DONATOR			: xconsole->Printf(__L("[!donator] An option available only to credited users (suchas offline URL) was specified, but the user is not a credited user."));							break; 
							case DIODYNDNS_STATUSCODE_NOTFQDN			: xconsole->Printf(__L("[notfqdn] The hostname specified is not a fully-qualified domain name (not in the form hostname.dyndns.org or domain.com)."));									break; 
							case DIODYNDNS_STATUSCODE_NOHOST			: xconsole->Printf(__L("[nohost] The hostname specified does not exist (or is not in the service specified in the system parameter)."));																break; 
							case DIODYNDNS_STATUSCODE_YOURS				: xconsole->Printf(__L("[!yours] The hostname specified exists, but not under the username specified."));																																break; 
							case DIODYNDNS_STATUSCODE_ABUSE				: xconsole->Printf(__L("[abuse] The hostname specified is blocked for update abuse."));																																									break; 
							case DIODYNDNS_STATUSCODE_NUMHOST			: xconsole->Printf(__L("[numhost] Too many or too few hosts found."));																																																	break; 
							case DIODYNDNS_STATUSCODE_DNSERR			: xconsole->Printf(__L("[dnserr] DNS error encountered."));																																																							break; 
							case DIODYNDNS_STATUSCODE_911					: xconsole->Printf(__L("[911] There is a serious problem on our side, such as a database or DNS server failure. The client should stop updating until notified via the status page that the service is back up.")); break; 
						}		

				} else xconsole->Printf(__L("It is not neccesary change DNS.")); 

		} else xconsole->Printf(__L("Error to obtain the public IP.")); 

	xconsole->Printf(__L("\n")); 

	delete newip;

	return status;
}



/*-------------------------------------------------------------------
//	TEST::Test_License
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/05/2014 17:12:04
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_License()
{

	XLICENSE		xlicense;
	XLICENSEID  xlicenseID;	
	XBUFFER			licensefile;
	XBUFFER			license;
	XSTRING			string;
	
	
	if(xlicense.GenerateMachineID(xlicenseID))
		{
			xlicenseID.GetXString(string);
			xconsole->Printf(__L("ID licencia : %s \n"), string.Get()); 
			
			xlicense.GetApplicationID()->Set(__L("GEN  Copyright (C).  All right reserved."));		
			xlicense.Generate(xlicenseID);
			xlicense.Get(string);
			xconsole->Printf(__L("Licencia    : %s \n"), string.Get()); 	

			XPATH		xpathgeneric;
			XPATH		xpath;
			DIOURL	url;

			XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathgeneric);
			xpath.Create(3 , xpathgeneric.Get(), __L("test"),__L(".lic"));	
			
			url = __L("http://pictel.es/abraham/test.lic");
	
			//if(xlicense.CreateFile(xpath, xlicenseID)) return false;

			if(!xlicense.LoadFromFile(xpath, (*xlicense.GetApplicationID()),  &license)) return false;
					
			//xlicense.LoadFromURL(url, 10, NULL, &license);

			xconsole->Printf(__L("Check       : ")); 	
			if(license.Compare(xlicense.Get()))
						xconsole->Printf(__L("Ok!\n")); 	
			 else xconsole->Printf(__L("Error!\n")); 	

		  //CHECKLICENSEFULLLOCAL(xpath)		
		}

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_SMTP
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2014 16:28:53
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
/*
bool TEST::Test_SMTP()
{	
	DIOSTREAMTCPIPCONFIG*	diostreamcfg	= NULL;
	DIOSTREAM*						diostream			= NULL;
	DIOSMTP*							smtp					= NULL;
	
	diostreamcfg = new DIOSTREAMTCPIPCONFIG();
	if(diostreamcfg) 
		{
			diostream = (DIOSTREAM*)diofactory->CreateStreamIO(diostreamcfg);
			if(diostream) 
				{	
					smtp = new DIOSMTP(diostream);
					if(smtp)
						{
							DIOURL url;
							
							//smtp->Server_GetURL()->Set(__L("mail.pictel.es"));
							smtp->Server_GetURL()->Set(__L("192.168.2.7"));
							smtp->Server_SetPort(587);
							smtp->Server_GetLogin()->Set(__L("abraham.velez"));
							smtp->Server_GetPassword()->Set(__L("sgraham"));
							smtp->GetSenderName()->Set(__L("Abraham"));
							smtp->GetSenderEmail()->Set(__L("abraham.velez@innomatic.es"));									
							smtp->SetContentType(DIOSSMPTCONTENTTYPE_UTF8);
							
							
							//int nconnexions = 0;
							//while(!xconsole->KBHit())
							//	{
							//		bool avaible = smtp->Server_IsAvailable();

							//		xconsole->Printf(__L("%04d Connexion SMTP: %s"), nconnexions, avaible?__L("Ok             \r"):__L("Error!\n"));
							//		nconnexions++;
							//	}
							

							if(smtp->Ini())
								{
									XPATH xpath;
												
									smtp->GetSubject()->Set(__L("Esto es una prueba."));

									XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
									xpath += __L("Lord_Vader.jpg");															
									smtp->AddAttachment(xpath);

									XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
									xpath += __L("helloworld.c!");															
									smtp->AddAttachment(xpath);


									smtp->GetMessage()->AddLine(__L("Esto es una prueba de envio de email desde el GEN."));
									smtp->GetMessage()->AddLine(__L("Esta seria la segunda linea."));
									smtp->GetMessage()->AddLine(__L("Esta seria la tercera linea."));

									smtp->AddRecipient(DIOSMTPRECIPIENTTYPE_TO, __L("Abraham")		, __L("abraham.velez@innomatic.es"));
									//smtp->AddRecipient(DIOSMTPRECIPIENTTYPE_CC, __L("Abraham")		, __L("ajvelez@endorasoft.com"));
									//smtp->AddRecipient(DIOSMTPRECIPIENTTYPE_CC, __L("Luis")				, __L("luis@pictel.es"));
									//smtp->AddRecipient(DIOSMTPRECIPIENTTYPE_CC, __L("Sir Graham")	, __L("sirgraham2@gmail.com"));
									
									if(!smtp->Send())										
												xconsole->Print(__L("Error al enviar el email!!!\n"));										 
									 else xconsole->Print(__L("email enviado.\n"));
									
									smtp->End();
								}

							delete smtp;
						}

					diofactory->DeleteStreamIO(diostream);
				}

			delete diostreamcfg;
		}
	
	return true;
}
*/


bool TEST::Test_SMTP()
{	
	DIOSTREAMTCPIPCONFIG*	diostreamcfg	= NULL;
	DIOSTREAM*						diostream			= NULL;
	DIOSMTP*							smtp					= NULL;
	
	diostreamcfg = new DIOSTREAMTCPIPCONFIG();
	if(diostreamcfg) 
		{
			diostream = (DIOSTREAM*)diofactory->CreateStreamIO(diostreamcfg);
			if(diostream) 
				{	
					smtp = new DIOSMTP(diostream);
					if(smtp)
						{
							DIOURL url;
							
							smtp->Server_GetURL()->Set(__L("192.168.40.127"));
							smtp->Server_SetPort(25);
							smtp->Server_GetLogin()->Set(__L("alertas.innomatic"));
							smtp->Server_GetPassword()->Set(__L("Sodetic"));
							smtp->GetSenderName()->Set(__L("Alerta"));
							smtp->GetSenderEmail()->Set(__L("alertas.innomatic@navasan.com"));									
							smtp->SetContentType(DIOSSMPTCONTENTTYPE_UTF8);
							
							
							//int nconnexions = 0;
							//while(!xconsole->KBHit())
							//	{
							//		bool avaible = smtp->Server_IsAvailable();

							//		xconsole->Printf(__L("%04d Connexion SMTP: %s"), nconnexions, avaible?__L("Ok             \r"):__L("Error!\n"));
							//		nconnexions++;
							//	}
							

							if(smtp->Ini())
								{							
									xconsole->Print(__L("Init SMTP ...\n"));						

									smtp->GetSubject()->Set(__L("Esto es una prueba."));									

									smtp->GetMessage()->AddLine(__L("Esto es una prueba de envio de email desde el GEN."));
									smtp->GetMessage()->AddLine(__L("Esta seria la segunda linea."));
									smtp->GetMessage()->AddLine(__L("Esta seria la tercera linea."));

									smtp->AddRecipient(DIOSMTPRECIPIENTTYPE_TO, __L("Sistemas")		, __L("sistemas@navasan.com"));
									//smtp->AddRecipient(DIOSMTPRECIPIENTTYPE_CC, __L("Abraham")		, __L("ajvelez@endorasoft.com"));
									//smtp->AddRecipient(DIOSMTPRECIPIENTTYPE_CC, __L("Luis")				, __L("luis@pictel.es"));
									//smtp->AddRecipient(DIOSMTPRECIPIENTTYPE_CC, __L("Sir Graham")	, __L("sirgraham2@gmail.com"));
									
									if(!smtp->Send())										
												xconsole->Print(__L("Error al enviar el email!!!\n"));										 
									 else xconsole->Print(__L("email enviado.\n"));
									
									smtp->End();
								}

							delete smtp;
						}

					diofactory->DeleteStreamIO(diostream);
				}

			delete diostreamcfg;
		}
	
	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_LCDPhilipsPCF8833
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2014 17:54:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_LCDPhilipsPCF8833()
{
	bool status = false;
			
	DIOGPIO* diogpio = diofactory->CreateGPIO();
  if(diogpio) 
		{
			if(diogpio->Ini())
				{							
					DIOSPILCDSCREENPCF8833* lcdscreen = new DIOSPILCDSCREENPCF8833(xsystem);
					if(lcdscreen)     
						{
							lcdscreen->SetDIOGPIO(diogpio);					
							
							lcdscreen->SetResetPinsGroup(DIOGPIO_PINSGROUP_A);   
							lcdscreen->SetResetPin(TEST_PINSCREEN_RESET);   

							status  = lcdscreen->Ini(0, 0, 10);
							xconsole->Printf(__L("Send screen INI   : %s \n"), status?__L("Ok!"):__L("Error!"));
							if(status) 
								{
									status = lcdscreen->Clear(DIOSPILCDSCREENPCF8833_RED);
									xconsole->Printf(__L("Send screen Clean : %s \n"), status?__L("Ok!"):__L("Error!"));		

									xsleep->Seconds(2);
									if(status) 
										{
											status = lcdscreen->Clear(DIOSPILCDSCREENPCF8833_GREEN);
											xconsole->Printf(__L("Send screen Clean : %s \n"), status?__L("Ok!"):__L("Error!"));				

											xsleep->Seconds(2);

											if(status) 
												{
													status = lcdscreen->Clear(DIOSPILCDSCREENPCF8833_BLUE);
													xconsole->Printf(__L("Send screen Clean : %s \n"), status?__L("Ok!"):__L("Error!"));				

													xsleep->Seconds(3);

												  for(int y=0; y< lcdscreen->GetHeight(); y++)
                             {
                               for(int x=0; x< lcdscreen->GetWidth(); x++)
                                 {
                                    lcdscreen->PutPixel(x , y, DIOSPILCDSCREENPCF8833_WHITE);                                       
                                  }
                             } 
																								
													XWORD*  buffer[5];
													XRAND*  random = xfactory->CreateRand();
													int     d;
																	
													for(d=0; d<5; d++)
														{
															buffer[d] = new XWORD[lcdscreen->GetNPixels()];

															for(XDWORD c=0; c<lcdscreen->GetNPixels(); c++)
																{
																	buffer[d][c] = random->Between(0,65535);
																}
														}
											
													int nframes = 0;

													d=0;
													while(!xconsole->KBHit())
														{																																																		
															if(status) status = lcdscreen->Update((XBYTE*)buffer[d]);
															xconsole->Printf(__L("Send screen Update: %05d  %s    \r"), nframes,  status?__L("Ok!"):__L("Error!"));												

															d++;
															if(d>=5) d=0;

															nframes++;
														}

													for(d=0; d<5; d++)
														{
															delete buffer[d];
														}
				
													xfactory->DeleteRand(random);
													


												}
										}
									
									lcdscreen->End();
								}											

							delete lcdscreen;
						}

					diogpio->End();
				}

			diofactory->DeleteGPIO(diogpio);

		}
	
	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_LCDIlitekILI9341
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			07/06/2014 10:28:51
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_LCDIlitekILI9341()
{
	bool status = false;
		
	DIOGPIO* diogpio = diofactory->CreateGPIO();
  if(diogpio) 
		{
			if(diogpio->Ini())
				{										
					DIOSPITOUCHSCREENSTMPE610* touchscreen = new DIOSPITOUCHSCREENSTMPE610;
					if(touchscreen)
						{

							status = touchscreen->Ini(1, 10);
							xconsole->Printf(__L("Touch Screen INI  : %s \n"), status?__L("Ok!"):__L("Error!"));
							xconsole->Printf(__L("Touch Screen Ver. : %04x \n"), touchscreen->GetVersion());

							if(status)
								{															
									DIOSPILCDSCREENILI9341* lcdscreen = new DIOSPILCDSCREENILI9341(xsystem);
									if(lcdscreen)     
										{
											lcdscreen->SetDIOGPIO(diogpio);						
											lcdscreen->SetPinDC(TEST_PINSCREEN_DC);   								

											status  = lcdscreen->Ini(0, 10);
											xconsole->Printf(__L("Screen INI        : %s \n"), status?__L("Ok!"):__L("Error!"));
											if(status) 
												{
													status = lcdscreen->Clear(DIOSPILCDSCREENILI9341_RED);
													xconsole->Printf(__L("Screen clean RED  : %s \n"), status?__L("Ok!"):__L("Error!"));		

													if(status) 
														{
															status = lcdscreen->Clear(DIOSPILCDSCREENILI9341_GREEN);
															xconsole->Printf(__L("Screen clean GREEN: %s \n"), status?__L("Ok!"):__L("Error!"));				

															if(status) 
																{
																	status = lcdscreen->Clear(DIOSPILCDSCREENILI9341_BLUE);
																	xconsole->Printf(__L("Screen clean BLUE : %s \n"), status?__L("Ok!"):__L("Error!"));				
																						
																	XWORD*  buffer[5];
																	XRAND*  random = xfactory->CreateRand();
																	int     d;
																	
																	for(d=0; d<5; d++)
																		{
																			buffer[d] = new XWORD[lcdscreen->GetNPixels()];

																			for(XDWORD c=0; c<lcdscreen->GetNPixels(); c++)
																				{
																					buffer[d][c] = random->Between(0,65535);
																				}
																		}
											
																	int		nframes = 0;

																	XWORD x				= 0;
																	XWORD y				= 0;
																	XBYTE z				= 0; 
																							

																	d=0;
																	while(!xconsole->KBHit())
																		{	
																			if(touchscreen->IsTouched())   
																				{
																					XBYTE ntouchs = touchscreen->GetBufferSize();

																					for(XBYTE c=0; c<ntouchs; c++)
																						{
																							if(touchscreen->ReadPosition(x, y, z)) 
																										 xconsole->Printf(__L("Touch Screen      : X(%d) Y(%d) Z(%d)        \n"), x, y , z);
																								else xconsole->Printf(__L("Touch Screen      : Error!!!                 \r"), x, y , z);
																						}
																				} 																																																	

																			if(status) status = lcdscreen->Update((XBYTE*)buffer[d]);
																		
																			//d++;
																			//if(d>=5) d=0;

																			nframes++;									
																		}

																	for(d=0; d<5; d++)
																		{
																			delete buffer[d];
																		}
				
																	xfactory->DeleteRand(random);
																}
														}
									
													lcdscreen->End();
												}
										
 											delete lcdscreen;
										}								
								}

							delete touchscreen;
						}

					diogpio->End();			
				}			

			diofactory->DeleteGPIO(diogpio);
		}

	return true;
}





/*-------------------------------------------------------------------
//	TEST::Test_OLEDSSD1331
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			05/12/2016 23:10:14
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_OLEDSSD1331()
{
	bool status = false;
			
	DIOGPIO* diogpio = diofactory->CreateGPIO();
  if(diogpio) 
		{
			if(diogpio->Ini())
				{							
					DIOSPIOLEDSCREENSSD1331* lcdscreen = new DIOSPIOLEDSCREENSSD1331();
					if(lcdscreen)     
						{
							lcdscreen->SetDIOGPIO(diogpio);					
							
							lcdscreen->SetResetPinsGroup(DIOGPIO_PINSGROUP_A);   
							lcdscreen->SetResetPin(TEST_PINSCREEN_RESET);   

							lcdscreen->SetDCPinsGroup(DIOGPIO_PINSGROUP_A);   
							lcdscreen->SetDCPin(TEST_PINSCREEN_DC);   

							status  = lcdscreen->Ini(0, 0, 3);
							xconsole->Printf(__L("Send screen INI   : %s \n"), status?__L("Ok!"):__L("Error!"));
							if(status) 
								{			
									//status = lcdscreen->Clear(DIOSPIOLEDSCREENSSD1331_BLUE);
									//xconsole->Printf(__L("Screen clean RED  : %s \n"), status?__L("Ok!"):__L("Error!"));				
									if(status)
										{					
											                       
											XWORD*  buffer[5];
											XRAND*  random = xfactory->CreateRand();
											int     d;
																	
											for(d=0; d<5; d++)
												{
													buffer[d] = new XWORD[lcdscreen->GetNPixels()];

													for(XDWORD c=0; c<lcdscreen->GetNPixels(); c++)
														{
															buffer[d][c] = random->Between(0,65535);
														}
												}
											
											d=0;
											while(!xconsole->KBHit())
												{					
													if(status) status = lcdscreen->Update((XBYTE*)buffer[d]);
																		
													d++;
													if(d>=5) d=0;		
													
													lcdscreen->PutPixel(0												, 0													, DIOSPILCDSCREENPCF8833_BLUE);                                
													lcdscreen->PutPixel(lcdscreen->GetWidth()-1	, 0													, DIOSPILCDSCREENPCF8833_BLUE);                                
													lcdscreen->PutPixel(0												, lcdscreen->GetHeight()-1	, DIOSPILCDSCREENPCF8833_BLUE);                                
													lcdscreen->PutPixel(lcdscreen->GetWidth()-1	, lcdscreen->GetHeight()-1	, DIOSPILCDSCREENPCF8833_BLUE);        																	
												}											
										}

									while(!xconsole->KBHit())
										{
											xsleep->MilliSeconds(100);
										}

									lcdscreen->End();
								}
						}
					
					diogpio->End();
				}

			diofactory->DeleteGPIO(diogpio);
		}

	return true;
}






/*-------------------------------------------------------------------
//	TEST::Test_DIOStreamUSB
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			18/11/2014 13:20:11
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamUSB()
{
	DIOSTREAMUSBLOCALENUMDEVICES* enumdevices;
	DIOSTREAMDEVICEUSB*						device = NULL;
	XSTRING												deviceID;
//XDWORD												PID = 0x5742;
//XDWORD												VID = 0x0484;

	XDWORD												PID = 0x5740;
	XDWORD												VID = 0x0483;


	if(GetExecParams())
		{
			for(int c=0; c<2; c++)
				{
					XSTRING* param = (XSTRING*)GetExecParams()->Get(c);	
					if(param) 
						{
							param->ToUpperCase(); 					

							switch(c)
								{
									case 0: param->UnFormat(__L("%08X"), &PID); 
													break;

									case 1: param->UnFormat(__L("%08X"), &VID); 
													break;
								}
						}
				}
		}

	if(!PID || !VID) 
		{
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Invalid Product ID or Vendor ID!"));	
			return false;
		}

	xconsole->Printf(__L("Search USB device: Product ID %08X, Vendor ID %08X"), PID, VID);	

  DIOSTREAMDEBUGPRINTINFO(xconsole, __L("-----------------------------------------------------"));	

	enumdevices = (DIOSTREAMUSBLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_USB_LOCAL);
	if(!enumdevices) return false;	

	enumdevices->Search();
	
	
	DIOSTREAMDEBUGPRINTINFO(xconsole, __L("USB enum:"));	

	for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
		{				
			device = (DIOSTREAMDEVICEUSB*)enumdevices->GetDevices()->Get(c);
			if(device) 
				{				
					device->DebugPrintInfo(xconsole);	

					if((device->GetProductID() == PID) && (device->GetVendorID() == VID))
						{																				
							 break;

						} else device = NULL;
				}
		}
	

	if(device) 
		{
			DIOSTREAMUSBCONFIG * diostreamcfg = new DIOSTREAMUSBCONFIG();
			if(diostreamcfg) 
				{			
					diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);			
			
					//diostreamcfg->GetDeviceID()->Set(__L("\\\\?\\usb#vid_0dd4&pid_015d#vkp80_200num.:_0#{28d78fad-5a12-11d1-ae5b-0000f803a8c2}"));
					//diostreamcfg->GetDeviceID()->Set(__L("/dev/usb/lp0"));
					//diostreamcfg->GetDeviceID()->Set(__L("/dev/bus/usb/001/002"));

					diostreamcfg->GetResource()->Set(device->GetResource()->Get());

					DIOSTREAMDEBUGPRINTINFO(xconsole, __L("\n"));

					DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Create USB CFG"));
			
					DIOSTREAM* diostream = (DIOSTREAM*)diofactory->CreateStreamIO( diostreamcfg);
					if(diostream) 
						{	
							DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Create USB Stream"));

							if(diostream->Open())
								{	
									DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Open USB Stream"));

									XBUFFER xbuffer;

									while(1)
										{																								
											if(xconsole->KBHit())
												{
													XBYTE character = xconsole->GetChar();
													if(character == 'A') break;
													if(!character)			break;
													
													//xbuffer.Add(character);

													//diostream->Write(xbuffer);
													//xbuffer.Delete();												

													///xconsole->Printf(__L("%c"),  character);																																			
												}

											int size = diostream->GetInXBuffer()->GetSize();
											if(size)
												{
													xbuffer.Resize(size);

													if(diostream->Read(xbuffer))
														{	
															for(XDWORD c=0; c<xbuffer.GetSize(); c++)
																{
																	xconsole->Printf(__L("%c"), xbuffer.GetByte(c));			
																}																			
														}			

													xbuffer.Delete();
												}									
										}
															
									diostream->Close();		
									DIOSTREAMDEBUGPRINTINFO(xconsole, __L("\nClose USB Stream"));							
								}

							diofactory->DeleteStreamIO(diostream);
						}
		
					delete diostreamcfg;
				}

		} 
   else 
		{
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("\nDevice not found!"));	
		}

	diofactory->DeleteStreamEnumDevices(enumdevices);			

	return true;
}





/*-------------------------------------------------------------------
//	TEST::Test_MemControl
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/09/2014 9:03:21
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_MemControl()
{
	XRAND*					xrand;
	XTIMER*					xtimer;
	XQWORD					alltime = 0L;

	xrand = xfactory->CreateRand();
	if(xrand) 
		{			
			xtimer = xfactory->CreateTimer();
			if(xtimer) 
				{	
					xconsole->Printf(__L("Leyendo fichero...\n"), xtimer->GetMeasureMilliSeconds());
		
					XPATH xpath;

					XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
					xpath.Slash_Add();
					xpath.Add(__L("test1.fbx"));
					
					xtimer->Reset();

					XFILETXT* xfiletxt = new XFILETXT();
					if(xfiletxt)
						{
							if(xfiletxt->Open(xpath))
								{
									xfiletxt->ReadAllFile();

									xfiletxt->DeleteAllLines();

									xfiletxt->Close();
								}

							delete xfiletxt;
						}

					xconsole->Printf(__L("%lu millisec\n"), xtimer->GetMeasureMilliSeconds());						
				}
		}
	
	if(xtimer) xfactory->DeleteTimer(xtimer);
	
	if(xrand)  xfactory->DeleteRand(xrand);



	/*
	#define MEMTESTASSIGN  3000

	XVECTOR<XBYTE*> assign;
	XRAND*					xrand;
	XTIMER*					xtimer;
	XQWORD					alltime = 0L;

	xrand = xfactory->CreateRand();
	if(xrand) 
		{			
			xtimer = xfactory->CreateTimer();
			if(xtimer) 
				{	
					int ntrys = 1;

					while(!xconsole->KBHit())
						{							
							alltime = 0L;
							xtimer->Reset();

							xconsole->Printf(__L("--- [ %d ] -------------------------------------------------\n"), ntrys);		
							xconsole->Printf(__L("Realizando asignaciones... "));		
						
							XBYTE* ptr = NULL;

							xtimer->Reset();
							for(int c=0; c<MEMTESTASSIGN; c++)
								{
									ptr = new XBYTE[xrand->Between(10,5000)];
									if(!ptr) break;
									
									assign.Add(ptr);

									//XDEBUG_PRINTCOLOR(1, __L("assign %d"), c);
								}

							alltime += xtimer->GetMeasureMilliSeconds();
							xconsole->Printf(__L("%lu millisec\n"), alltime);
			
							xconsole->Printf(__L("Eliminando asignaciones... "));		
							xtimer->Reset();

							assign.Delete(ptr);
							
							assign.DeleteContents();
							assign.DeleteAll();

							xconsole->Printf(__L("%lu millisec\n"), xtimer->GetMeasureMilliSeconds());

							alltime += xtimer->GetMeasureMilliSeconds();

							xconsole->Printf(__L("Fin total "));		
							xconsole->Printf(__L("%lu millisec\n"), alltime);							

							ntrys++;
						}
				}
		}

	if(xtimer) xfactory->DeleteTimer(xtimer);
	
	if(xrand)  xfactory->DeleteRand(xrand);

	*/

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_ExecApplication
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/11/2014 20:15:15
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_SystemCommand()
{
	XSTRING command;

	//command = __L("C:\\windows\\system32\\calc.exe");
	//command = __L("/mnt/Projects/Test/Platforms/Linux/sakis3g");

	command = __L("./sakis3g --interactive");

	xconsole->Printf(__L(" -------------------------------------------------\n"));		
	xconsole->Printf(__L("Ejecutando command: %s "), command.Get());		

	int returncode = 0;

	bool status = GetXSystem()->MakeCommand(command, &returncode);

	if(status)
				xconsole->Printf(__L("correcto!!  exitcode %d\n"), returncode);										 
	 else xconsole->Printf(__L("error al intentar ejecutar la app.\n"));

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_DIOStreamUDP
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			18/11/2014 13:25:04
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamUDP(bool modeserver)
{
	XSTRING							 autoaddress;
	DIOSTREAMUDPCONFIG * diostreamcfg;
	DIOSTREAMUDP*				 diostream;
	
	diostreamcfg = new DIOSTREAMUDPCONFIG();
	if(!diostreamcfg) return false;

	DIOSTREAMIPLOCALENUMDEVICES* enumdevices = (DIOSTREAMIPLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_IP_LOCAL);
	if(enumdevices)
		{			
			DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetFirstActiveDevice();
			if(device) device->GetIP()->GetXString(autoaddress);
				
			diofactory->DeleteStreamEnumDevices(enumdevices);				
		}	
	
	xconsole->Printf(__L("Modo Servidor: %s\n"), modeserver?__L("Si"):__L("No"));

	diostreamcfg->SetMode(modeserver?DIOSTREAMMODE_SERVER:DIOSTREAMMODE_CLIENT);				
	if(!modeserver) diostreamcfg->GetRemoteURL()->Set(__L("192.168.2.56")  /*autoaddress*/ );		
	diostreamcfg->SetRemotePort(4000);	

	diostream = (DIOSTREAMUDP*)diofactory->CreateStreamIO( diostreamcfg);
	if(diostream)
		{					
			if(diostream->Open())
				{
					if(diostream->WaitToConnected(5))
						{	
							XBUFFER xbuffer;
							XDWORD  br;
							XDWORD  bw;
							int			sizeforbuffer = 200;
							bool	  status;

							xconsole->Print(__L("Open ...\n"));

							if(!modeserver)
								{
									while(!xconsole->KBHit())
										{								
											xbuffer.Delete();
											for(int c=0; c<sizeforbuffer; c++)
												{
													xbuffer.Add((XBYTE)xrand->Between(0,255));
												}

											bw = diostream->Write(xbuffer.Get(), xbuffer.GetSize());
											status = (bw == xbuffer.GetSize())?true:false; 
											if(status) status = diostream->WaitToFlushOutXBuffer(5);
													
											if(!status)	xconsole->Print(__L("Error to Write ...\n"));							
										}
								}
							 else
								{
									while(!xconsole->KBHit())
										{								
											if(diostream->WaitToFilledReadingBuffer(sizeforbuffer, 1))
												{																												
 													xbuffer.Delete();
													xbuffer.Resize(sizeforbuffer);
									
													br = diostream->Read(xbuffer.Get(), xbuffer.GetSize());
													status = (br == xbuffer.GetSize())?true:false; 
													if(status)
														{
															XDEBUG_PRINTDATABLOCK(modeserver?1:2, xbuffer);								

														}	else xconsole->Print(__L("Error to Read ...\n"));																																				
												}
										}
								}

							xconsole->Print(__L("Close...\n"));

							diostream->Close();
						}
				}
			
			diofactory->DeleteStreamIO(diostream);
		}
					
	delete diostreamcfg;
		
	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_DIOProtocolConnexionsManager
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 14:03:09
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOProtocolConnexionsManager(bool modeserver)
{
	bool status	= true;	

	XRAND* xrand = xfactory->CreateRand();
	if(!xrand) return false;

	xconsole->Clear();

	xconsole->TipicalHeader_Show(2012	, APPLICATION_NAMEAPP
																		, APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR
																		, APPLICATION_ENTERPRISE);

	xconsole->PrintMessage(__L("\n\n"),0,false,false);
			
	xconsole->Printf(__L("En modo servidor: %s.\n"), modeserver?__L("Si"):__L("No"));
										
	protocolconnexionmanager = new TESTPROTOCOLCONNEXIONSMANAGER();
	if(protocolconnexionmanager)
		{
			SubscribeEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_CONNECTEDCONNEXION		, protocolconnexionmanager);
			SubscribeEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_INITPROTOCOL					, protocolconnexionmanager);
			SubscribeEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION	, protocolconnexionmanager);									


			if(!modeserver)
				{
					protocolconnexionmanager->ProtocolConnexions_SetNLimit(1);
					protocolconnexionmanager->TargetURL_Add(__L("192.168.2.127"));	
				}
			else protocolconnexionmanager->ProtocolConnexions_SetNLimit(31);
							
			if(protocolconnexionmanager->Ini(modeserver, protocolapplicationdata))
				{					
					xconsole->Print(__L("Open Connexions...\n"));

				} else status = false;

		} else status = false;

		
	if(!status)  return false;

	status = false;

	int		countercommand = 0;
	bool  exitfunc			 = false;
	
	do{	for(int c=0; c< protocolconnexionmanager->ProtocolConnexions_GetNAvailable(); c++)
				{
					DIOPROTOCOLCONNEXION*	protocolconnexion = protocolconnexionmanager->ProtocolConnexions_Get(c);
					if(protocolconnexion) 
						{						
							TESTPROTOCOL* protocol = (TESTPROTOCOL*)protocolconnexion->GetDIOProtocol();
							if(protocol) 
								{
									if(protocol->IsConnected()) 
										{		
											if(protocol->IsOperational()) 
												{											

													//if(modeserver) continue;

													XBYTE	version				= 0;
													XBYTE	subversion		= 0;
													XBYTE	subversionerr = 0;
													XBYTE command				= xrand->Max(4);

													status = false;

													xconsole->Printf(__L("(%5d) "), countercommand);

 													switch(command)
														{
															case 0		: { XSTRING  string;
																						
																						xconsole->Printf(__L("Get String [%s]... "), string.Get());
																						status = protocol->CMD_GetString(string);																																											
																					} 
																					break;

															case 1		: { XSTRING  string;
																
																						switch(xrand->Max(5))
																							{
																								case 0		: string = __L("Hello word!");   
																														break;
																								case 1		: string = __L("Hola Radiola!");   
																													  break;
																								case 2		: string = __L("GEN forever!");   
																														break;
 																								case 3		: string = __L("No conocere el miedo. El miedo mata la mente.");   
																												 	  break;
																									default : break;
																							}

																						xconsole->Printf(__L("Send String [%s]... "), string.Get());
																						status = protocol->CMD_SendString(string);																																												
																					} 
																					break;

															case 2		: { XDWORD		variantID	= xrand->Between(1, 10);
																						XVARIANT  variant;
																
																						xconsole->Printf(__L("Get Variant ID [%02d]... "), variantID);
																						status = protocol->CMD_GetVariant(variantID, variant);																						
																					} 
																					break;
																	
															case 3		: { XDWORD		variantID	= xrand->Between(1, 10);
																						XVARIANT  variant;
																						
																						xconsole->Printf(__L("Send Variant ID [%02d]... "), variantID);	
																						variant = (XDWORD)xrand->Between(1, 1024);																						
																						status = protocol->CMD_SendVariant(variantID, variant);																						
																					}
																					break;	

															case 4    : { XSTRING datetime;
																					
																						xconsole->Printf(__L("Get Datetime [%s]... "), datetime.Get());	
																						status = protocol->CMD_GetDateTime(datetime);																																												
																					}
																					break;

															case 5		: {	XPATH source;
																						XPATH target;

																						XSTRING name1 = __L("test1.fbx");
																						XSTRING name2 = __L("test2.fbx");

																						source.Create(XPATHSMANAGERSECTIONTYPE_ROOT, 1, name1.Get());
																						target.Create(XPATHSMANAGERSECTIONTYPE_ROOT, 1, name2.Get());
																						
																						xconsole->Printf(__L("Get Copy file [%s]... "), name1.Get());	
																						status = protocol->CMD_File(DIOPROTOCOL_DIRECTION_GET, source.Get(), target.Get(), 5, NULL);
																					}
																					break;

																default : status = false;
																					break;
															
														}			

													xconsole->Printf(__L("[%s] \n"), status?__L("Ok!"):__L("Error!"));	
													
													countercommand++;
												} 											
										}
									 else
									  {
											if(!modeserver) 
												{
													exitfunc = true;
													break;
												}
										}
								}
							 else
								{
									if(!modeserver) 
										{
											exitfunc = true;
											break;
										}
								}							
						}
				}

			if(xconsole->KBHit()) exitfunc = true;

		} while(!exitfunc);

	xconsole->Printf(__L("\nDesconectado... \n"));

	if(protocolconnexionmanager)
		{
			UnSubscribeEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_CONNECTEDCONNEXION		, protocolconnexionmanager);
			UnSubscribeEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_INITPROTOCOL					, protocolconnexionmanager);
			UnSubscribeEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION	, protocolconnexionmanager);									

			if(protocolconnexionmanager->End())
				{							
					delete protocolconnexionmanager;	
					protocolconnexionmanager = NULL;
				}	
		}			
	
	if(xrand) xfactory->DeleteRand(xrand);

	return true;
}


/*-------------------------------------------------------------------
//	TEST::Test_DIOStreamCipher
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/11/2014 17:15:46
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamCipher(bool modeserver)
{
	XSTRING									autoaddress;
	DIOSTREAMTCPIPCONFIG*		diostreamcfg;
	DIOSTREAMCIPHER*				diostream;	
	CIPHERKEYSYMMETRICAL		key;	
	XBYTE										inivector[]	= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
	XBYTE										keydata[]		= { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 }; 

	if(!cipher) return false;

	cipher->SetChainingMode(CIPHERCHAININGMODE_CBC);				
	cipher->SetPaddingType(XBUFFER_PADDINGTYPE_ZEROS); 
	cipher->SetInitVector(inivector,sizeof(inivector));		

	key.Set(keydata, sizeof(keydata));	

	cipher->SetKey(&key);

	DIOSTREAMIPLOCALENUMDEVICES* enumdevices = (DIOSTREAMIPLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_IP_LOCAL);
	if(enumdevices)
		{			
			DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetFirstActiveDevice();
			if(device) device->GetIP()->GetXString(autoaddress);
				
			diofactory->DeleteStreamEnumDevices(enumdevices);				
		}	
	
	xconsole->Printf(__L("Modo Servidor: %s\n"), modeserver?__L("Si"):__L("No"));	
	
	diostreamcfg = new DIOSTREAMTCPIPCONFIG();
	if(!diostreamcfg) return false;

	diostreamcfg->SetMode(modeserver?DIOSTREAMMODE_SERVER:DIOSTREAMMODE_CLIENT);				
	if(!modeserver) diostreamcfg->GetRemoteURL()->Set(__L("192.168.2.117")  /*autoaddress*/ );		
	diostreamcfg->SetRemotePort(4000);	

	diostream = new DIOSTREAMCIPHER(diostreamcfg, cipher);
	if(diostream)
		{					
			if(diostream->Open())
				{
					bool connected = false;

					if(!modeserver)
						{
							if(diostream->WaitToConnected(3600)) connected = true;
							
						} else connected = true; 					 					 

					if(connected)
						{
							XBUFFER xbuffer;
							XDWORD  br;
							XDWORD  bw;
							int			sizeforbuffer = xrand->Between(10,255);
							bool	  status;

							xconsole->Print(__L("Open ...\n"));

							if(!modeserver)
								{
									while(!xconsole->KBHit())
										{								
											xbuffer.Delete();
											for(int c=0; c<sizeforbuffer; c++)
												{
													xbuffer.Add((XBYTE)c /*xrand->Between(0,255)*/);
												}

											if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_CONNECTED) break;												

											bw = diostream->Write(xbuffer.Get(), xbuffer.GetSize());
											status = (bw == xbuffer.GetSize())?true:false; 
											if(status) status = diostream->WaitToFlushOutXBuffer(5);
													
											if(!status)	xconsole->Print(__L("Error to Write ...\n"));							
										}
								}
							 else
								{
									while(!xconsole->KBHit())
										{								
											if(diostream->WaitToFilledReadingBuffer(sizeforbuffer, 1))
												{																												
 													xbuffer.Delete();
													xbuffer.Resize(sizeforbuffer);
									
													br = diostream->Read(xbuffer.Get(), xbuffer.GetSize());
													status = (br == xbuffer.GetSize())?true:false; 
													if(status)
														{
															XDEBUG_PRINTDATABLOCK(modeserver?1:2, xbuffer);								

														}	else xconsole->Print(__L("Error to Read ...\n"));																																				
												}
										}
								}

							xconsole->Print(__L("Close...\n"));

							diostream->Close();

						} else xconsole->Print(__L("Error no conexion disponible..\n"));
				}
			
			delete diostream;
		}
					
	delete diostreamcfg;
		
	return true;
}





/*-------------------------------------------------------------------
//	TEST::Test_DIOProtocol
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			25/11/2014 14:18:53
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOProtocol(bool modeserver)
{
	DIOSTREAMUDPLOCALENUMSERVERS* localenumservers;
	XSTRING												serveraddress;
	DIOSTREAMTCPIPCONFIG*					diostreamcfg;
	DIOSTREAMCIPHER*							diostreamcipher;
	TESTPROTOCOL*									protocol;
	CIPHERKEYSYMMETRICAL					key;	
	XBYTE													inivector[]	= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
	XBYTE													keydata[]		= { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 }; 

	if(!cipher) return false;

	cipher->SetChainingMode(CIPHERCHAININGMODE_CBC);				
	cipher->SetPaddingType(XBUFFER_PADDINGTYPE_ZEROS); 
	cipher->SetInitVector(inivector,sizeof(inivector));		

	key.Set(keydata, sizeof(keydata));	

	cipher->SetKey(&key);

	xconsole->Printf(__L("Modo Servidor : %s\n"), modeserver?__L("Si"):__L("No"));	

	localenumservers = new DIOSTREAMUDPLOCALENUMSERVERS;
	if(localenumservers) 
		{	
			localenumservers->SetID(0xFAAFFAFA);
			localenumservers->SetPort(4001);
	
			diostreamcfg = new DIOSTREAMTCPIPCONFIG();
			if(diostreamcfg) 
				{
					diostreamcipher = new DIOSTREAMCIPHER(diostreamcfg, cipher);
					if(diostreamcipher)
						{				
							bool status = false;

							diostreamcfg->SetMode(modeserver?DIOSTREAMMODE_SERVER:DIOSTREAMMODE_CLIENT);				

							if(modeserver)
								{
									DIOSTREAMTCPIP* diostreamtcpip = (DIOSTREAMTCPIP*)diostreamcipher->GetDIOStreamBase();
									if(diostreamtcpip) 
										{
											diostreamtcpip->SetEnumServers(localenumservers);			
											status = true;
										}
								}
							 else
								{							
									localenumservers->Search();
																			
									while(localenumservers->IsSearching())
										{
											if(xconsole->KBHit()) break;

											xsleep->MilliSeconds(50);
										}	

									localenumservers->StopSearch(true);

									if(localenumservers->AreDevicesAvailable())
										{
											DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)localenumservers->GetDevices()->Get(0);
											if(device)  
												{
													device->GetIP()->GetXString(serveraddress);									
													diostreamcfg->GetRemoteURL()->Set(serveraddress);		
													xconsole->Printf(__L("Server en     : %s\n"), serveraddress.Get());	
													status = true;
												}
										} 

									if(!serveraddress.GetSize())  
										{
											xconsole->Print(__L("Error ningun servidor encontrado...\n"));
										}									
								}
		
							if(status)
								{							
									protocol = new TESTPROTOCOL(diostreamcipher); 
									if(protocol)
										{
											protocol->SetApplicationData(protocolapplicationdata);											
											diostreamcfg->SetRemotePort(4000);	
									
											if(diostreamcipher->Open())
												{
													bool connected = false;

													if(!modeserver)
														{
															if(diostreamcipher->WaitToConnected(3600)) connected = true;
							
														} else connected = true; 					 					 

													if(connected)
														{
															do{
																	if(protocol->IsConnected() && (!modeserver))
																		{								
																			XWORD	version					= 0;
																			XWORD	subversion			= 0;
																			XWORD	subversionerr		= 0;

																			status = protocol->CMD_GetProtocolVersion(version, subversion, subversionerr);
																			xconsole->Printf(__L("[%s] Get Version %02d.%02d.%02d ...\n"), status?__L("Ok!"):__L("Error!"), version, subversion, subversionerr);

																			if(xtimer) xsleep->Seconds(1);													
																		}

																} while(!xconsole->KBHit());
														}

													xconsole->Print(__L("Close...\n"));

													diostreamcipher->Close();

												} else xconsole->Print(__L("Error no conexion disponible..\n"));

											delete protocol;

										}										
								}

							delete diostreamcipher;
						} 

					delete diostreamcfg;
				}
				
			delete localenumservers;
		}
							
	return true;
}





/*-------------------------------------------------------------------
//	TEST::Test_GSMATCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/12/2014 23:23:51
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_GSMATCommand()
{	
	DIOSTREAMENUMDEVICES*			enumdevices		= NULL;
	DIOSTREAMCONFIG*					diostreamcfg	= NULL;
	DIOSTREAM*								diostream			= NULL;
	DIOATCMDGSM*							dioatcmdgsm		= NULL;
	DIOSTREAMDEVICEBLUETOOTH* device				= NULL;
	bool											isdeviceatcmdavaible;

	enumdevices = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE);
	if(enumdevices)
		{		
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Buscando..."));						

			enumdevices->Search();
			
			xtimer->Reset();
			
			while(enumdevices->IsSearching())
				{
					xsleep->MilliSeconds(10);
				}

			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("Busqueda concluida..."));						
			
			for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
				{					
					device = (DIOSTREAMDEVICEBLUETOOTH*)enumdevices->GetDevices()->Get(c);
					if(device) 
						{
							if(device->GetName()->Find(__L("Nexus 5"), true) !=  XSTRING_NOTFOUND)
								{
									device->DebugPrintInfo(xconsole);
									break;

								} else device = NULL;
						}
				}			
		}
			
	if(device)	
		{		
			diostreamcfg = new DIOSTREAMBLUETOOTHCONFIG();
			if(diostreamcfg)
				{	
					DIOSTREAMBLUETOOTHCONFIG* scfg = (DIOSTREAMBLUETOOTHCONFIG*)diostreamcfg;										
							
					scfg->GetRemoteMAC()->Set(device->GetMAC()->Get());
					scfg->SetRemoteChannel(device->GetChannelDialUpNetworking());

					diostream = diofactory->CreateStreamIO(diostreamcfg);
					if(diostream)
						{ 						
							isdeviceatcmdavaible = false;

							dioatcmdgsm = new DIOATCMDGSM(diostream);
							if(!dioatcmdgsm) return false;
					
							xconsole->PrintMessage(__L(" Comprobando dispositivo... "),1,false,false);

							if(dioatcmdgsm->Ini(30, false, false))
								{							
									isdeviceatcmdavaible = true;

									xconsole->PrintMessage(__L("\r Dispositivo comandos AT accesible..."),1,false,true);
									xconsole->PrintMessage(__L(""),0,false,true);

									XSTRING string;
									XSTRING value;

									for(int c=0;c<10;c++)
										{
											string.Empty();
							
											switch(c)
												{
													case  0 : if(dioatcmdgsm->GetManufactured(value)==DIOATCMD_ERROR_NONE)					string.Format(__L("Fabricante             : %s"),value.Get());   
																		break;

													case  1 : if(dioatcmdgsm->GetModel(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("Modelo de terminal     : %s"),value.Get());   
																		break;
											
													case  2 : if(dioatcmdgsm->GetIMEI(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("IMEI                   : %s"),value.Get());   
																		break;

													case  3 : if(dioatcmdgsm->GetVersion(value)==DIOATCMD_ERROR_NONE)								string.Format(__L("Version                : %s"),value.Get());   
																		break;	
														
													case  4 : { int RSSI = 0;
																			int BER  = 0;
																			
																			if(dioatcmdgsm->GetSignalQuality(RSSI,BER)==DIOATCMD_ERROR_NONE)		string.Format(__L("Calidad de senal       : (RSSI %d)  (BER %d)"),RSSI,BER);																																							
																		}
																		break;		

													case  5 : { bool avaible;
																			if(dioatcmdgsm->IsAvailableServiceGSM(avaible)==DIOATCMD_ERROR_NONE)	string.Format(__L("Disp. Servicios GSM    : %s") , avaible?__L("Si"):__L("No"));                           
																		}
																		break;

													case  6 : if(dioatcmdgsm->GetIMSI(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("IMSI                   : %s"),value.Get());   
																		break;													
																									
													case  7 : { XBYTE support;
															
																			if(dioatcmdgsm->GetSMSSupport(support)==DIOATCMD_ERROR_NONE)				string.Format(__L("Soporte SMS            : servicio %d, destino %s, origen %s, broadcast %s")	, (support>>4)  
																																																																																																					, support&DIOATCMDGSM_SMSSUPPORT_MOVILETERMINATED ?__L("Si"):__L("No")
																																																																																																					, support&DIOATCMDGSM_SMSSUPPORT_MOVILEORIGINATED ?__L("Si"):__L("No")
																																																																																																					, support&DIOATCMDGSM_SMSSUPPORT_BROADCAST			   ?__L("Si"):__L("No"));																				
																		}
																		break;	
													
													case  8 : { XBYTE format;
																			XBYTE support;
																																						
																			if(dioatcmdgsm->GetSMSFormat(format,false) ==DIOATCMD_ERROR_NONE) 
																				{
																					if(dioatcmdgsm->GetSMSFormat(support,true)==DIOATCMD_ERROR_NONE) 
																						{
																							XSTRING strformat;
																							XSTRING strsupport;

																							strformat = __L("desconocido");
																							if(format == DIOATCMDGSM_SMSFORMAT_PDU)  strformat = __L("PDU");
																							if(format == DIOATCMDGSM_SMSFORMAT_TXT)  strformat = __L("Texto");

																							if(support&DIOATCMDGSM_SMSFORMAT_PDU)		 strsupport = __L("PDU");
																							if(support&DIOATCMDGSM_SMSFORMAT_TXT)     
																								{
																									if(!strsupport.IsEmpty()) strsupport += __L(" + ");
																									strsupport += __L("Texto");
																								}


																							string.Format(__L("Formato de SMS         : %s  (valido %s)"),strformat.Get(),strsupport.Get());																																							
																						}
																				}
																		}
																		break;	

													 case 9 : if(dioatcmdgsm->GetSMSCenter(value)==DIOATCMD_ERROR_NONE)						string.Format(__L("Centro de mensajes     : %s"),value.Get());   
																		break;		
												}
											
											if(!string.IsEmpty()) xconsole->PrintMessage(string.Get(),1,true,true);
										}												
									
									xconsole->PrintMessage(__L("Enviando mensaje ... "),1,true,false);	

									if(dioatcmdgsm->SendSMS(__L("649668886"),__L("Hola radiola. Mensaje de prueba"))==DIOATCMD_ERROR_NONE) 											
												xconsole->PrintMessage(__L("Ok."),0,false,true);	
									 else xconsole->PrintMessage(__L("Error!"),0,false,true);	
									

								} else xconsole->PrintMessage(__L("\r Dispositivo de comandos AT NO accesible!"),1,true,true);	
						}
				}


			if(dioatcmdgsm)
				{
					dioatcmdgsm->End();
					delete dioatcmdgsm;
					dioatcmdgsm = NULL;
				}

			if(diostream) 
				{
					diofactory->DeleteStreamIO(diostream);
					diostream = NULL;
				}

			if(diostreamcfg) 
				{
					delete diostreamcfg;
					diostreamcfg = NULL;
				}

		}
	
	if(enumdevices)		
		{
			diofactory->DeleteStreamEnumDevices(enumdevices);
			enumdevices = NULL;
		}	

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_GSMATCommand2
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/12/2014 16:54:22
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_GSMATCommand2()
{	
	DIOSTREAMENUMDEVICES*			enumdevices		= NULL;
	DIOSTREAMCONFIG*					diostreamcfg	= NULL;
	DIOSTREAM*								diostream			= NULL;
	DIOATCMDGSM*							dioatcmdgsm		= NULL;
	DIOSTREAMDEVICEUSB*				device				= NULL;
	bool											isdeviceatcmdavaible;


	enumdevices = diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_USB_LOCAL);
	if(enumdevices) 
		{
			enumdevices->Search();
	
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("------------------------------------------------------"));	
			DIOSTREAMDEBUGPRINTINFO(xconsole, __L("USB enum:"));	

			for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
				{				
					DIOSTREAMDEVICE* device = (DIOSTREAMDEVICE*)enumdevices->GetDevices()->Get(c);
					if(device) device->DebugPrintInfo(xconsole);				
				}
		}

			
	//if(device)	
		{		
			
			diostreamcfg = new DIOSTREAMUARTCONFIG();
			if(diostreamcfg)
				{ 
					DIOSTREAMUARTCONFIG* scfg = (DIOSTREAMUARTCONFIG*)diostreamcfg;									
			
					scfg->GetLocalDeviceName()->Set(__L("/dev/ttyUSB0"));
					scfg->SetBaudRate(115200);
					scfg->SetDataBits(DIOSTREAMUARTDATABIT_8);
					scfg->SetParity(DIOSTREAMUARTPARITY_NONE);
					scfg->SetStopBits(DIOSTREAMUARTSTOPBITS_ONE);
					scfg->SetFlowControl(DIOSTREAMUARTFLOWCONTROL_NONE);		
			

			/*
			diostreamcfg = (DIOSTREAMUSBCONFIG*)diofactory->CreateStreamIOConfig( DIOSTREAMTYPE_USB);
			if(diostreamcfg)
				{	
					DIOSTREAMUSBCONFIG* scfg = (DIOSTREAMUSBCONFIG*)diostreamcfg;										
							
					scfg->GetDeviceID()->Set(__L("/dev/ttyUSB0"));
					*/

					diostream = diofactory->CreateStreamIO(diostreamcfg);
					if(diostream)
						{ 						
							isdeviceatcmdavaible = false;

							dioatcmdgsm = new DIOATCMDGSM(diostream);
							if(!dioatcmdgsm) return false;
					
							xconsole->PrintMessage(__L(" Comprobando dispositivo... "),1,false,false);

							if(dioatcmdgsm->Ini(10, false, false))
								{							
									isdeviceatcmdavaible = true;

									xconsole->PrintMessage(__L("\r Dispositivo comandos AT accesible..."),1,false,true);
									xconsole->PrintMessage(__L(""),0,false,true);

									XSTRING string;
									XSTRING value;

									for(int c=0;c<10;c++)
										{
											string.Empty();
							
											switch(c)
												{
													case  0 : if(dioatcmdgsm->GetManufactured(value)==DIOATCMD_ERROR_NONE)					string.Format(__L("Fabricante             : %s"),value.Get());   
																		break;

													case  1 : if(dioatcmdgsm->GetModel(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("Modelo de terminal     : %s"),value.Get());   
																		break;
											
													case  2 : if(dioatcmdgsm->GetIMEI(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("IMEI                   : %s"),value.Get());   
																		break;

													case  3 : if(dioatcmdgsm->GetVersion(value)==DIOATCMD_ERROR_NONE)								string.Format(__L("Version                : %s"),value.Get());   
																		break;	
														
													case  4 : { int RSSI = 0;
																			int BER  = 0;
																			
																			if(dioatcmdgsm->GetSignalQuality(RSSI,BER)==DIOATCMD_ERROR_NONE)		string.Format(__L("Calidad de senal       : (RSSI %d)  (BER %d)"),RSSI,BER);																																							
																		}
																		break;		

													case  5 : { bool avaible;
																			if(dioatcmdgsm->IsAvailableServiceGSM(avaible)==DIOATCMD_ERROR_NONE)	string.Format(__L("Disp. Servicios GSM    : %s") , avaible?__L("Si"):__L("No"));                           
																		}
																		break;
													
													case  6 : if(dioatcmdgsm->GetIMSI(value)==DIOATCMD_ERROR_NONE)									string.Format(__L("IMSI                   : %s"),value.Get());   
																		break;													
																									
													case  7 : { XBYTE support;
															
																			if(dioatcmdgsm->GetSMSSupport(support)==DIOATCMD_ERROR_NONE)				string.Format(__L("Soporte SMS            : servicio %d, destino %s, origen %s, broadcast %s")	, (support>>4)  
																																																																																																					, support&DIOATCMDGSM_SMSSUPPORT_MOVILETERMINATED ?__L("Si"):__L("No")
																																																																																																					, support&DIOATCMDGSM_SMSSUPPORT_MOVILEORIGINATED ?__L("Si"):__L("No")
																																																																																																					, support&DIOATCMDGSM_SMSSUPPORT_BROADCAST			   ?__L("Si"):__L("No"));																				
																		}
																		break;	
													
													case  8 : { XBYTE format;
																			XBYTE support;
																																						
																			if(dioatcmdgsm->GetSMSFormat(format,false) ==DIOATCMD_ERROR_NONE) 
																				{
																					if(dioatcmdgsm->GetSMSFormat(support,true)==DIOATCMD_ERROR_NONE) 
																						{
																							XSTRING strformat;
																							XSTRING strsupport;

																							strformat = __L("desconocido");
																							if(format == DIOATCMDGSM_SMSFORMAT_PDU)  strformat = __L("PDU");
																							if(format == DIOATCMDGSM_SMSFORMAT_TXT)  strformat = __L("Texto");

																							if(support&DIOATCMDGSM_SMSFORMAT_PDU)		 strsupport = __L("PDU");
																							if(support&DIOATCMDGSM_SMSFORMAT_TXT)     
																								{
																									if(!strsupport.IsEmpty()) strsupport += __L(" + ");
																									strsupport += __L("Texto");
																								}


																							string.Format(__L("Formato de SMS         : %s  (valido %s)"),strformat.Get(),strsupport.Get());																																							
																						}
																				}
																		}
																		break;	

													 case 9 : if(dioatcmdgsm->GetSMSCenter(value)==DIOATCMD_ERROR_NONE)						string.Format(__L("Centro de mensajes     : %s"),value.Get());   
																		break;		
												}
											
											if(!string.IsEmpty()) xconsole->PrintMessage(string.Get(),1,true,true);
										}			

									while(!xconsole->KBHit())
										{ 
											int RSSI = 0;
											int BER  = 0;
																			
											string = __L("");
											if(dioatcmdgsm->GetSignalQuality(RSSI,BER)==DIOATCMD_ERROR_NONE)		string.Format(__L("Calidad de senal       : (RSSI %d)  (BER %d)"),RSSI,BER);												
											if(!string.IsEmpty()) xconsole->PrintMessage(string.Get(),1,true,true);
										}
									
									/*
									xconsole->PrintMessage(__L("Enviando mensaje ... "),1,true,false);	

									if(dioatcmdgsm->SendSMS(__L("649668886"),__L("Hola radiola. Mensaje de prueba"))==DIOATCMD_ERROR_NONE) 											
												xconsole->PrintMessage(__L("Ok."),0,false,true);	
									 else xconsole->PrintMessage(__L("Error!"),0,false,true);	
									*/

								} else xconsole->PrintMessage(__L("\r Dispositivo de comandos AT NO accesible!"),1,true,true);	
						}
				}


			if(dioatcmdgsm)
				{
					dioatcmdgsm->End();
					delete dioatcmdgsm;
					dioatcmdgsm = NULL;
				}

			if(diostream) 
				{
					diofactory->DeleteStreamIO(diostream);
					diostream = NULL;
				}

			if(diostreamcfg) 
				{
					delete diostreamcfg;
					diostreamcfg = NULL;
				}

		}
	
	if(enumdevices)		
		{
			diofactory->DeleteStreamEnumDevices(enumdevices);
			enumdevices = NULL;
		}	

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_DIOAlerts
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/01/2015 22:56:13
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOAlerts()
{	
	DIOSTREAMENUMDEVICES*			SMSenumdevices  = NULL;
	DIOSTREAMDEVICEBLUETOOTH*	SMSdevice       = NULL;
	DIOSTREAMCONFIG*					SMSdiostreamcfg	= NULL;
	DIOSTREAM*								SMSdiostream		= NULL;	
	bool											status = false;

	/*
	SMSenumdevices = diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE);
	if(SMSenumdevices)
		{					
			SMSenumdevices->Search();
			
			xtimer->Reset();
			
			while(SMSenumdevices->IsSearching())
				{
					xsleep->MilliSeconds(10);
				}
			
			for(int c=0;c<(int)SMSenumdevices->GetDevices()->GetSize();c++)
				{					
					SMSdevice = (DIOSTREAMDEVICEBLUETOOTH*)SMSenumdevices->GetDevices()->Get(c);
					if(SMSdevice) 
						{
							if(SMSdevice->GetName()->Find(__L("EndoraSoft"), true) !=  XSTRING_NOTFOUND)
								{											
									break;
								} else SMSdevice = NULL;
						}
				}																			
		}
	*/
							
	if(SMSdevice) 
		{		
			SMSdiostreamcfg = new DIOSTREAMBLUETOOTHCONFIG();
			if(!SMSdiostreamcfg) return false;
		
			DIOSTREAMBLUETOOTHCONFIG* scfg = (DIOSTREAMBLUETOOTHCONFIG*)SMSdiostreamcfg;					
							
			scfg->GetRemoteMAC()->Set(SMSdevice->GetMAC()->Get());
			scfg->SetRemoteChannel(SMSdevice->GetChannelDialUpNetworking());

			SMSdiostream = diofactory->CreateStreamIO(SMSdiostreamcfg);
		}




	if(DIOALERTS::GetInstance().Ini())
		{								
			//------------------------------------------------------------------
			 						
			if(SMSdiostream) DIOALERTS::GetInstance().Sender_SMSConfig(SMSdiostream, 1, __L("649668886"));
			
			//------------------------------------------------------------------

			DIOALERTS::GetInstance().Sender_SMTPConfig(__L("mail.pictel.es"), 587, __L("appalert"), __L("prometeus1"), __L("appalert@innomatic.es"), 1, __L("1,Abraham,abraham.velez@endorasoft.es"));
			

			//------------------------------------------------------------------

			DIOALERT* alert = DIOALERTS::GetInstance().CreateAlert(__L("Test"), 25, DIOALERTLEVEL_INFO, __L("Mi Home"), __L("Esto es una Alerta"), __L("Alerta de problema en algun sitio"));
			if(alert) 
				{					
					int statussend = DIOALERTS::GetInstance().Send(DIOALERTSSENDER_ALL, alert);

					status = true;
						
				} else status = false;
	
			delete alert;
			
			DIOALERTS::GetInstance().End();				
		}

	if(SMSdiostreamcfg)
		{
			delete SMSdiostreamcfg;
			SMSdiostreamcfg = NULL;
		}

	if(SMSdiostream)
		{
			diofactory->DeleteStreamIO(SMSdiostream);
			SMSdiostream = NULL;
		}
	
	if(SMSenumdevices)
		{
			diofactory->DeleteStreamEnumDevices(SMSenumdevices);
			SMSenumdevices = NULL;
		}

	return status;
}




/*-------------------------------------------------------------------
//	TEST::Test_BusPirate
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			05/02/2015 17:20:06
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_BusPirate()
{
	DIOSTREAMUARTCONFIG* diostreamcfg;
	DIOSTREAMUART*			 diostream;
	
	diostreamcfg = new DIOSTREAMUARTCONFIG();
	if(diostreamcfg)
		{							
			#ifdef HW_RASPBERRYPI
			diostreamcfg->GetLocalDeviceName()->Set(__L("/dev/ttyAMA0"));		
			#endif		

			diostreamcfg->SetPort(10);		
			diostreamcfg->SetBaudRate(115200);
			diostreamcfg->SetDataBits(DIOSTREAMUARTDATABIT_8);
			diostreamcfg->SetParity(DIOSTREAMUARTPARITY_NONE);
			diostreamcfg->SetStopBits(DIOSTREAMUARTSTOPBITS_ONE);
			diostreamcfg->SetFlowControl(DIOSTREAMUARTFLOWCONTROL_NONE);		

			diostream = (DIOSTREAMUART*)diofactory->CreateStreamIO( diostreamcfg);
			if(diostream)
				{				
					DIOBUSPIRATE* buspirate = new DIOBUSPIRATE(diostream);
					if(buspirate)
						{
							if(buspirate->Ini(10))
								{
									buspirate->SetMode(DIOBUSPIRATETYPEPROCOTOL_I2C, true, DIOBUSPIRATESPEEDSNIFFER_100KHZ, true);

									XBUFFER in;
									XBUFFER out;

									while(!xconsole->KBHit())
										{
											buspirate->ReadI2CSnifferCommand(in, out);
										}
									
									buspirate->End();
								}
							
							delete buspirate;
						}
					
					diofactory->DeleteStreamIO(diostream);
				}
							
			delete diostreamcfg;
		}
		
	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_WiiMote
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/02/2015 17:50:58
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_WiiMote()
{
	DIOWIIMOTE* wiimote = new DIOWIIMOTE;
	if(wiimote)
		{
			if(wiimote->Ini())							
				{
					xconsole->Printf(__L("Conected!\n"));

					wiimote->End();

				} else xconsole->Printf(__L("No conected!\n"));

			delete wiimote;
		}

	return true;
}





/*-------------------------------------------------------------------
//	TEST::Test_DIOStreamUDPAcknowledge
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			17/02/2015 19:53:57
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamUDPAcknowledge(bool modeserver)
{
	DIOSTREAMUDPCONFIG*				diostreamudpcfg;
	DIOSTREAMUDPACKNOWLEDGE*	diostreamudpack;
	bool											status = false;
	
	xconsole->Printf(__L("Modo Servidor : %s\n"), modeserver?__L("Si"):__L("No"));	

	diostreamudpcfg = new DIOSTREAMUDPCONFIG();
	if(diostreamudpcfg) 
		{	
			diostreamudpcfg->SetMode(modeserver?DIOSTREAMMODE_SERVER:DIOSTREAMMODE_CLIENT);				
			diostreamudpcfg->GetRemoteURL()->Set(__L(""));
			diostreamudpcfg->SetRemotePort(1234);

			diostreamudpack = new DIOSTREAMUDPACKNOWLEDGE(diostreamudpcfg);
			if(diostreamudpack)
				{	
					XBUFFER xbuffer;			
					bool		status = false;
			
					status = diostreamudpack->Open();		
					if(status)
						{
							if(!modeserver)
								{
									while(!xconsole->KBHit())
										{																				
											xbuffer.Delete();

											for(int c=0;c<xrand->Max(64)+16;c++)
												{
													xbuffer.Add((XBYTE)xrand->Max(255));
												}
									
											XDWORD bw = diostreamudpack->Write(xbuffer.Get(), xbuffer.GetSize());
											if(bw) 
												{
													xconsole->Printf(__L("Escritos %02d bytes \n"), bw);
													xconsole->PrintDataBlock(xbuffer);
													xconsole->Printf(__L("\n"));

												} 
											else 
											 {
												 xconsole->Printf(__L("Write Error!!!\n"), bw);
												 //break;
											 }

											//xsleep->Seconds(1);
										}

								}	
							 else
								{
									while(!xconsole->KBHit())
										{											
											XSTRING address;
											XWORD   port;

											if(diostreamudpack->ReadDatagram(address, port, xbuffer))
												{
													xconsole->Printf(__L("Address: %s \n"), address.Get());
													xconsole->PrintDataBlock(xbuffer);

													xconsole->Printf(__L("\n"));
												}													
										}
								}

							diostreamudpack->Close();

						} else xconsole->Printf(__L("Error to Connect.\n"));
				}

			delete diostreamudpack;
		} 

	delete diostreamudpcfg;

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_DIOAlertServer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/05/2015 19:52:09
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOAlertServer()
{
	DIOALERTSUDPSERVER* alertsUDPserver = NULL;
		
	alertsUDPserver = new DIOALERTSUDPSERVER;
	if(!alertsUDPserver) return false;
	
	xconsole->Printf(__L("Alert Server Open: "));

	if(alertsUDPserver->Ini(__L("192.168.2.117"), 1234))
		{
			xconsole->Printf(__L("OK.\n "));

			while(!xconsole->KBHit())
				{
					xconsole->Printf(__L("N Alerts: %3d.        \r"), alertsUDPserver->GetNAlerts());

					xsleep->MilliSeconds(100);
				}

			alertsUDPserver->End();

		} else xconsole->Printf(__L("Error.\n "));

	delete alertsUDPserver;

	xconsole->Printf(__L("Alert Server Close."));

	return true;
}






/*-------------------------------------------------------------------
//	TEST::Test_DIOCheckInternetConnexion
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			21/05/2015 8:43:09
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOCheckInternetConnexion()
{
	xconsole->Printf(__L("Connecting internet connection status ...\n"));

	DIOCHECKCONNEXIONS* checkconnexions = new DIOCHECKCONNEXIONS;
	if(!checkconnexions) return false;

	if(!checkconnexions->Ini(1)) return false; 

	DIOCHECKINTERNETCONNEXION* checkinternetconnexion	= new DIOCHECKINTERNETCONNEXION(checkconnexions);
	if(!checkinternetconnexion) return false; 
		
	bool haveinternet = false;

	while(!xconsole->KBHit())
		{																		 
			//XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, __L("Status"), false, __L("Checking internet connection status ..."));
					
			xconsole->Printf(__L("Checking internet connection status ..."));

			haveinternet = checkinternetconnexion->Check();	
	
			//XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, __L("Status"), false, __L("Internet connection: %s."), haveinternet?__L("Ok"):__L("Error!") );

			xconsole->Printf(__L("%s.                  \r"), haveinternet?__L("Ok"):__L("Error!"));

			if(checkinternetconnexion->IsChangeConnexionStatus())
				{
					if(haveinternet)
						{
							DIOCHECKCONNEXIONCUT* connexioncut =  checkinternetconnexion->GetLastConnexionsCut();
							XSTRING							  measuretime;

							if(connexioncut)
								{
									connexioncut->GetMeasureTimeString(measuretime, true);																										
									XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING, __L("Status"), false, __L("Restored internet connection: %s of disconnection."), measuretime.Get());
								}
							 else
								{													
									XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING, __L("Status"), false, __L("Connecting to internet made ..."));
								}
						}
					 else
						{
							XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING, __L("Status"), false, __L("Lost connection to internet ..."));
						}
				}
		}

	if(checkinternetconnexion) 
		{			
			delete checkinternetconnexion;
		}

	if(checkconnexions)
		{
			checkconnexions->End();
			delete checkconnexions;
		}

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_DIOCheckConnexions
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			29/05/2015 14:19:13
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOCheckConnexions()
{
	DIOCHECKCONNEXIONS* checkconnexions	= new DIOCHECKCONNEXIONS;
	if(!checkconnexions) return false; 
	
	if(checkconnexions->Ini(5)) 
		{	
			bool   isactive[4]		= { false, false, false, false };
			XDWORD connexionID[4] = { 0, 0, 0, 0 };

			checkconnexions->Connexion_Add(__L("8.8.8.8")					, connexionID[0]);
			checkconnexions->Connexion_Add(__L("www.google.com")	, connexionID[1]);
			checkconnexions->Connexion_Add(__L("192.168.1.3")			, connexionID[2]);
			checkconnexions->Connexion_Add(__L("192.168.1.18")		, connexionID[3]);
			
			xsleep->Seconds(3);

			xconsole->Printf(__L("Checking connection status ...\n"));

			int ntrys = 0;

			while(!xconsole->KBHit())
				{	
					ntrys++;
					xconsole->Printf( __L("%04d "), ntrys);

					for(int c=0; c<4; c++)
						{
							checkconnexions->Connexion_IsActive(connexionID[c], isactive[c]);					
							xconsole->Printf( __L("[%d %s] "), c, isactive[c]?__L("OK"):__L("Error!"));							
						}

					xconsole->Printf( __L("\n"));

					if(!xconsole->KBHit()) xsleep->Seconds(1);
				}		

			checkconnexions->Connexion_DeleteByID(connexionID[0]);
			checkconnexions->Connexion_DeleteByID(connexionID[1]);
			checkconnexions->Connexion_DeleteByID(connexionID[2]);
			checkconnexions->Connexion_DeleteByID(connexionID[3]);

			checkconnexions->End();
		}

	delete checkconnexions;

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_FileTXT
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/06/2015 10:21:43
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_FileTXT()
{
	/*
	XFILETXT* xfiletxt = new XFILETXT();
	if(!xfiletxt) return false;

	XPATH xpath;

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
	xpath.Slash_Add();
	xpath.Add(__L("password.txt"));

	if(xfiletxt->Create(xpath))
		{
			XSTRING password;
			XSTRING number;

			for(int c=0; c<=9999; c++)
				{	
					password = __L("Fondo");
					number.ConvertFromInt(c,__L("%04d"));

					password += number;

					xfiletxt->AddLine(password);
				}

			xfiletxt->WriteAllFile();

			xfiletxt->Close();
		}


	delete xfiletxt;
	*/

	XFILETXT* xfiletxt = new XFILETXT();
	if(!xfiletxt) return false;

	XPATH xpath;

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath); 
	xpath.Slash_Add();
	xpath.Add(__L("test.FBX"));

	if(xfiletxt->Open(xpath, false))
		{
			xfiletxt->ReadAllFile();

			xfiletxt->WriteAllFile();

			xfiletxt->Close();
		}

	delete xfiletxt;

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_UpdateAPP
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/06/2015 15:58:58
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_ApplicationUpdate()
{	
	XPATH xpathrootapp;
							
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathrootapp);							
	xpathrootapp.Slash_Delete();
	xpathrootapp.SetOnlyDriveAndPath();

	DIOAPPLICATIONUPDATE* applicationupdate = new DIOAPPLICATIONUPDATE(APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR, APPLICATION_NAMEAPP, xpathrootapp);
	if(!applicationupdate) return false;

	SubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_GETVERSIONAVAILABLE				, applicationupdate);				
	SubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_DOWNLOADFILE							, applicationupdate);
	SubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_UNZIPFILE									, applicationupdate);
	SubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_BACKUPORIGINALFILE				,	applicationupdate);
	SubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_COPYUPDATEFILE						,	applicationupdate);
	SubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_DELETEBACKUPORIGINALFILE	,	applicationupdate);
		
	xconsole->Printf(__L("Comprobando actualizacion...\n"));

	DIOAPPLICATIONUPDATEVERSIONDATA		applicationversiondata;
	DIOURL														url;
	XPATH															xpathupdate;
	DIOAPPLICATIONUPDATE_ERROR				error;

	url = __L("http://192.168.2.127/updates/JackPotViewer/PC-windows");
	
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathupdate);
	xpathupdate.Slash_Add();
		
	if(applicationupdate->GetVersionAvailable(url, 80, applicationversiondata, 20, error))
		{
			if(applicationupdate->IsNewVersion(applicationversiondata))
				{	
					XPATH xpathupdateadd;

					xconsole->Printf(__L("Nueva version encontrada %d.%d.%d \n"), applicationversiondata.GetVersion(), applicationversiondata.GetSubVersion(), applicationversiondata.GetErrorControl());

					xpathupdateadd.Format(__L("update_[%d.%d.%d]"), applicationversiondata.GetVersion(), applicationversiondata.GetSubVersion(), applicationversiondata.GetErrorControl());
					xpathupdate += xpathupdateadd;

					xconsole->Printf(__L("Descargando version...\n"));

					if(applicationupdate->Download(url, 80, xpathupdate, applicationversiondata, 20, error))
						{							
							if(applicationupdate->ChangeFilesFromDownload(applicationversiondata, xpathupdate, error))
								{
									xconsole->Printf(__L("version actualizada.\n"));

									applicationupdate->RestartApplication(error);									
								}	
							 else 
								{									
									xconsole->Printf(__L("No se han podido actualizar la nueva version.\n"));					
									
									applicationupdate->RestoreUpdate(error);

									xconsole->Printf(__L("Restaurando version anterior.\n"));					
								}

						} else xconsole->Printf(__L("No se ha podido descargar la nueva version disponible.\n"));

				} else xconsole->Printf(__L("No hay nueva version disponible.\n"));

		} else xconsole->Printf(__L("No hay ninguna version disponible.\n"));


	UnSubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_GETVERSIONAVAILABLE					, applicationupdate);				
	UnSubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_DOWNLOADFILE							, applicationupdate);
	UnSubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_UNZIPFILE									, applicationupdate);
	UnSubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_BACKUPORIGINALFILE				,	applicationupdate);
	UnSubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_COPYUPDATEFILE						,	applicationupdate);
	UnSubscribeEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_DELETEBACKUPORIGINALFILE	,	applicationupdate);

	delete applicationupdate;

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_XSystem
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			07/08/2015 12:09:30
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_XSystem()
{

	
	XSYSTEM* xsystem =  GetXSystem();
	if(!xsystem) return false;

	XSTRING string;

	string = __L("Prueba de xsystem\n");
	XDEBUG_PRINTCOLOR(1, string.Get());
	if(xconsole) xconsole->Printf(string.Get(), __L("\n"));			
	
	
	XSTRING		 nameapp;	
	XSYSTEM_SO so = xsystem->GetSO();

	if(so & XSYSTEM_SO_WINDOWS)
		{	
			XSTRING pathapp = __L("c:\\windows\\system32\\");

			nameapp = __L("calc.exe");

			pathapp.Add(nameapp);
			xsystem->ExecuteApplication(pathapp.Get());
		}
	 else
		{
		  if(so & XSYSTEM_SO_LINUX)
				{
					XSTRING pathapp = __L("/usr/bin/");

					nameapp = __L("top");

					pathapp.Add(nameapp);
					xsystem->ExecuteApplication(pathapp.Get());
				}
			 else
			  {				 
					string = __L("sistema operativo invalido...");
					XDEBUG_PRINTCOLOR(1, string.Get());
					if(xconsole) xconsole->Printf(string.Get(), __L("\n"));				
			  }
		}
	
	
	XDWORD	ID				 = 0;
	bool		isrunning = xsystem->IsApplicationRunning(nameapp.Get(), &ID);

	string.Format(__L("esta ejecutandose %s:  %s [%x] "), nameapp.Get(), isrunning?__L("Ok!"):__L("Error!"), ID);
	XDEBUG_PRINTCOLOR(1, string.Get());
	if(xconsole) xconsole->Printf(string.Get(), __L("\n"));			
	
	
	string = __L("Saliendo ...");
	XDEBUG_PRINTCOLOR(1, string.Get());
	if(xconsole) xconsole->Printf(string.Get(), __L("\n"));			

	SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER); 
	SetEvent(TEST_XFSMEVENT_END);
	
	return true;
}





/*-------------------------------------------------------------------
//	TEST::Test_DatabaseDBF
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/10/2015 8:26:50
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DatabaseDBF()
{
	XPATH			 xpath;	
	XFILEDBF*	 xfiledbf;
	bool			 status = false;

	xfiledbf = new XFILEDBF();
	if(!xfiledbf)	return false;
		{					
			xpath.Create(XPATHSMANAGERSECTIONTYPE_ROOT, 2, __L("database"),__L(".dbf"));	
			
			if(xfiledbf->Open(xpath, false)) 
				{
					if(xfiledbf->GetHeader()->Load(xfiledbf->GetPrimaryFile()))
						{
							int nrecords = xfiledbf->GetHeader()->GetNRecords();
							if(nrecords)
								{ 
									XFILEDBF_RECORD* record	= xfiledbf->ReadRecord(25);								
									if(record)
										{
											XFILEDBF_RECORD* newrecord = new XFILEDBF_RECORD(xfiledbf->GetHeader()->GetFields());
											if(newrecord)
												{							
													XSTRING string;

													for(int d=0; d<(int)xfiledbf->GetHeader()->GetFields()->GetSize(); d++)
														{
															if(record->GetDataString(d, string)) 
																{
																	string.DeleteCharacter(0x20, XSTRINGCONTEXT_ATFIRST);
																	string.DeleteCharacter(0x20, XSTRINGCONTEXT_ATEND);

																	newrecord->SetDataString( d, string);
																}
														}	

													xfiledbf->InsertRecord(25,newrecord);															
												}
										}								
							
									status = true;
								}
						}

					xfiledbf->Close();
				}			
		}

	delete xfiledbf;

	return true;
}


class CA
{
protected:
		void prueba()
		{
				static int a = 0;
				a++;
		}
};

class CB : public CA
{
public:		
		void prueba()
		{
				static int b = 0;
				b++;
		}
		
};



/*-------------------------------------------------------------------
//	TEST::Test_Database
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/10/2015 15:42:23
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DatabaseSQL(XDWORD type)
{
		xconsole->Print(__L("\nTesting SQL\n"));
	bool status    = false;	
	
	CB t;
	t.prueba();

	#ifdef DBSQL_ACTIVE
	/*
	DBSQLDATABASE_TYPE type = 
														//DBSQLDATABASE_TYPE_MYSQL;
														DBSQLDATABASE_TYPE_POSTGRESQL;
														//DBSQLDATABASE_TYPE_SQLITE;  
		*/												

	switch (type)
	{
	case DBSQLDATABASE_TYPE_MYSQL			: xconsole->Print(__L("\nTesting MySQL\n")); break;
	case DBSQLDATABASE_TYPE_POSTGRESQL: xconsole->Print(__L("\nTesting PostgreSQL\n")); break;
	case DBSQLDATABASE_TYPE_SQLITE		: xconsole->Print(__L("\nTesting SQLite\n")); break;
	}

	DBSQLDATABASE*				database	= NULL;
	DBSQLCONNECTION*			connexion	= NULL;
	DBSQLQUERY*						query			= NULL;
	XDATETIME*						datetime	= xfactory->CreateDateTime();
	
	datetime->SetDay(3);

	if(!datetime) return false;

	XPATH		xpath;
	XSTRING path;
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
	xpath.GetPath(path);

	xconsole->Print(__L("Creating Database\n\n"));
	XDEBUG_PRINTCOLOR(0,__L("Creating database "));
	database = DBSQLFACTORY::Create( (DBSQLDATABASE_TYPE)type);	

	status = true;


	if(database)
		{			
			xconsole->Print(__L("Creating Connection\n\n"));
			XDEBUG_PRINTCOLOR(0,__L("Creating Connection "));
			connexion	= database->CreateConnection();
			

			if (connexion)
			{
					/*
					connexion->SetOption(__L("URL"),			__L("192.168.9.115"));
					connexion->SetOption(__L("USER"),			__L("root"));
					connexion->SetOption(__L("PASSWORD"),	__L("root"));
					connexion->SetOption(__L("DATABASE"),	__L("gentest"));
					connexion->SetOption(__L("TIMEOUT"),	__L("10"));
					*/
					
					connexion->SetOption(__L("URL"),			__L("192.168.9.115"));
					connexion->SetOption(__L("USER"),			__L("sirgraham"));
					connexion->SetOption(__L("PASSWORD"), __L("sgraham"));
					connexion->SetOption(__L("DATABASE"), __L("CasinosDB"));
					connexion->SetOption(__L("TIMEOUT"),	__L("50"));
					

					switch (type)
					{
					case DBSQLDATABASE_TYPE_MYSQL			: connexion->SetOption(__L("PORT"), __L("3306"));		break;
					case DBSQLDATABASE_TYPE_POSTGRESQL: connexion->SetOption(__L("PORT"), __L("5432"));		break;
					case DBSQLDATABASE_TYPE_SQLITE		: connexion->SetOption(__L("PATH"), path.Get());		break;
					}

					if (database->IsThreadSafe())
					{
							xconsole->Print(__L("Database is ThreadSafe\n\n"));
							XDEBUG_PRINTCOLOR(0, __L("Database is ThreadSafe"));
					}
					else
					{
							xconsole->Print(__L("Database is not ThreadSafe\n\n"));
							XDEBUG_PRINTCOLOR(0, __L("Database is not ThreadSafe"));
					}

					xconsole->Print(__L("Openning Connection \n\n"));
					XDEBUG_PRINTCOLOR(0, __L("Openning Connection "));					

					if (database->Open())
					{



							query = database->CreateQuery();
							if (query)
							{
									if (database->Transaction())
									{
											xconsole->Print(__L("drop table if exists DIEGOTEST \n\n"));
											query->Set(__L("drop table if exists DIEGOTEST"));
											if (database->Execute(query))
											{
													switch (type)
													{
													case DBSQLDATABASE_TYPE_MYSQL: query->Set(__L("CREATE TABLE DIEGOTEST(id int,name varchar(50),value float, fecha DATETIME,  PRIMARY KEY(id))"));						  break;
													case DBSQLDATABASE_TYPE_POSTGRESQL: query->Set(__L("CREATE TABLE DIEGOTEST(id integer CONSTRAINT firstkey PRIMARY KEY,name varchar(50),value float,fecha timestamp)"));	  break;
													case DBSQLDATABASE_TYPE_SQLITE: query->Set(__L("CREATE TABLE DIEGOTEST(id int PRIMARY KEY,name varchar(50),value float, fecha DATETIME)"));							  break;
													}

													if (database->Execute(query))
													{
															xconsole->Print(__L("delete from DIEGOTEST \n\n"));
															query->Set(__L("delete from DIEGOTEST"));
															if (database->Execute(query))
															{
																	switch (type)
																	{
																	case DBSQLDATABASE_TYPE_MYSQL:
																	case DBSQLDATABASE_TYPE_SQLITE:	query->Set(__L("INSERT INTO DIEGOTEST(id,name,value,fecha) VALUES (?,?,?,?)"));					break;
																	case DBSQLDATABASE_TYPE_POSTGRESQL: query->Set(__L("INSERT INTO DIEGOTEST(id,name,value,fecha) VALUES ($1,$2,$3,$4)"));			break;
																	}

																	query->Bind(0, 1);
																	query->Bind(1, __L("Diego"));
																	query->Bind(2, 1.5f);
																	query->Bind(3, (*datetime));
																	
																	xconsole->Print(__L("INSERT INTO DIEGOTEST(id,name,value,fecha) VALUES (?,?,?,?) \n\n"));
																	if (database->Execute(query))
																	{
																			query->UnbindAll();

																			query->Bind(0, 2);
																			query->Bind(1, __L("Nat"));
																			query->Bind(2, 6.66f);
																			query->Bind(3, (*datetime));

																			xconsole->Print(__L("INSERT INTO DIEGOTEST(id,name,value,fecha) VALUES (?,?,?,?) \n\n"));
																			if (database->Execute(query))
																			{
																					query->UnbindAll();

																					query->Bind(0, 3);
																					query->Bind(1, __L("Patata"));
																					query->Bind(2, 25.12f);
																					query->Bind(3, (*datetime));

																					xconsole->Print(__L("INSERT INTO DIEGOTEST(id,name,value,fecha) VALUES (?,?,?,?) \n\n"));
																					if (database->Execute(query))
																					{
																							xconsole->Print(__L("COMMIT \n\n"));
																						//	if (database->Commit())
																							{
																									xconsole->Print(__L("DELETE FROM DIEGOTEST WHERE id=2 \n\n"));
																									query->Set(__L("DELETE FROM DIEGOTEST WHERE id=2"));
																									if (!database->Execute(query))
																									{


																									}


																									xconsole->Print(__L("use cursor"));
																									query->UseCursor(__L("cur"));
																									xconsole->Print(__L("SELECT id,name,value,fecha FROM DIEGOTEST\n\n"));
																									query->Set(__L("SELECT id,name,value,fecha FROM DIEGOTEST"));
																									//		query->Set(__L("SELECT COUNT(*) FROM DIEGOTEST"));

																									if (database->Execute(query))
																									{

																											DBSQLRESULT* result = query->GetResult();

																											if (result)
																											{
																													while (result->HasNext())
																													{
																															result->ProcessRow();

																															DBSQLROW*				row = result->GetRow();
																															DBSQLROWHEADER* header = result->GetHeader();

																															xconsole->Printf(__L("[%d]\n"), (int)result->GetCurrentRowIndex());
																															XVARIANT variant;

																															for (XDWORD e = 0; e < result->GetNumColumns(); e++)
																															{
																																	XDWORD typerow = row->Get(e).GetType();

																																	xconsole->Printf(__L("[%s] = "), header->Get(e)->Get());

																																	switch (typerow)
																																	{
																																	case DBSQLVARIANT_TYPE_DATETIME: { DBSQLDATETIME datetime = (row->Get(e));
																																			XSTRING				string;

																																			datetime.GetDateTimeToString(XDATETIME_FORMAT_STANDARD, string);
																																			xconsole->Printf(__L("%s"), string.Get());
																																	}
																																																	 break;

																																	case DBSQLVARIANT_TYPE_STRING:	xconsole->Printf(__L("%s"), (XCHAR*)row->Get(e));
																																			break;

																																	case DBSQLVARIANT_TYPE_INTEGER:	xconsole->Printf(__L("%d"), (int)row->Get(e));
																																			break;

																																	case DBSQLVARIANT_TYPE_FLOAT:	xconsole->Printf(__L("%f"), (float)(row->Get(e)));
																																			break;

																																	default:	break;
																																	}

																																	xconsole->Printf(__L("\n"));
																															}

																															result->Next();
																													}
																											}
																									}

																									query->ClearCursor();
																									database->Close();

																									status = true;
																							}
																					}
																			}
																	}
															}
													}
											}
											else
											{
													XSTRING s;
													if (database->GetErrorList()->GetSize())
													s.Format(__L("DBSQL Execute failed %s %s"), database->GetName(), database->GetErrorList()->GetLast()->description.Get());
													XDEBUG_PRINTCOLOR(4, s.Get());
													xconsole->Print(s.Get());
											}
									}

							}
							else XDEBUG_PRINTCOLOR(0, __L("DBSQL Create Query %s"), database->GetName());

					}
					else
					{
							XSTRING s;
							if (database->GetErrorList()->GetSize())
							s.Format(__L("DBSQL Open Connection %s %s"), database->GetName(), database->GetErrorList()->GetLast()->description.Get());
							XDEBUG_PRINTCOLOR(4, s.Get());
							xconsole->Print(s.Get());
					}
			}
			else
					{
							XSTRING s;
							if (database->GetErrorList()->GetSize())
							s.Format(__L("DBSQL Create Connection %s %s"), database->GetName(), database->GetErrorList()->GetLast()->description.Get());
							XDEBUG_PRINTCOLOR(4, s.Get());
							xconsole->Print(s.Get());
					}
			
		} else XDEBUG_PRINTCOLOR(4,__L("DBSQL failed to create database (factory)"));
		
		
	delete query;
	delete connexion;
	delete database;

	xfactory->DeleteDateTime(datetime);
	
	#endif
	xconsole->Print(__L("-- SQL test finished --"));
	return status;
}





/*-------------------------------------------------------------------
//	TEST::XFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/10/2015 9:35:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_XFile()
{
	XPATH xpath;

	XFILE* xfile = xfactory->Create_File();
	if(!xfile) return false;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_ROOT, 2, __L("file"),__L(".txt"));	
			
	if(xfile->Open(xpath, false))
		{
			XBUFFER xbuffer;

			for(int c=0; c<10; c++)
				{
					xbuffer.Add((XBYTE)(64 +c));
				}

			xfile->SetPosition(10);
			xfile->InsertBlock(xbuffer.Get(), xbuffer.GetSize());

			xfile->Close();
		}

	xfactory->Delete_File(xfile);

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_PCap
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/10/2015 11:01:03
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOPCap()
{
	#ifdef DIOPCAP_ACTIVE

	DIOPCAP*						 diopcap					 = NULL; 
	DIOPCAPNETINTERFACE* netinterface			 = NULL;  
	int                  indexnetinterface = 0;

	diopcap = diofactory->CreatePCap();
  if(!diopcap)  return false;

	if(diopcap->Ini())
    { 
			xconsole->Printf(__L("Interfaces de red disponibles: \n\n"));
      for(int c=0; c<(int)diopcap->GetNetInterfaces()->GetSize(); c++)
        {
          netinterface = diopcap->GetNetInterface(c);
          if(netinterface) xconsole->Printf(__L("[%2d] %s, [%s]\n"), c, netinterface->GetName()->Get(), netinterface->GetDescription()->Get());						
				}

			xconsole->Printf(__L("\n"));

			if(indexnetinterface != -1) 
				{
					if(diopcap->Capture_Start(indexnetinterface, true, 100)) 
						{
							while(!xconsole->KBHit())
								{
									int nsize = diopcap->Frames_Get()->GetSize();

									for(int c=0;c<nsize;c++)
										{
											DIOPCAPFRAME* frame = diopcap->Frames_Get(c);
											if(!frame) break;

											if(xconsole->KBHit()) break;
									                                                                                                                                   
											DIOPCAPETHERNETHEADER ethernetheader;                                                                                                             
											if(frame->GetHeaderEthernet(ethernetheader)) 
												{                          
													xconsole->Printf(__L("MAC Source: %02X:%02X:%02X:%02X:%02X:%02X  MAC Target: %02X:%02X:%02X:%02X:%02X:%02X ") , ethernetheader.MACsource[0], ethernetheader.MACsource[1], ethernetheader.MACsource[2], ethernetheader.MACsource[3], ethernetheader.MACsource[4], ethernetheader.MACsource[5]
																																																																			  , ethernetheader.MACtarget[0], ethernetheader.MACtarget[1], ethernetheader.MACtarget[2], ethernetheader.MACtarget[3], ethernetheader.MACtarget[4], ethernetheader.MACtarget[5]);

													switch(ethernetheader.type)
														{
															case DIOPCAPETHERNETTYPE_IP			: {	DIOPCAPIPHEADER ipheader;																															
																																	if(frame->GetHeaderIP(ipheader))
																																		{ 
																																			DIOIP			ipsourceaddr;
																																			XSTRING		ipsourcestring;
																																			DIOIP			iptargetaddr;
																																			XSTRING   iptargetstring;
																																																																			
																																			ipsourceaddr.Set(ipheader.sourceaddr.byte1, ipheader.sourceaddr.byte2, ipheader.sourceaddr.byte3, ipheader.sourceaddr.byte4);
																																			iptargetaddr.Set(ipheader.targetaddr.byte1, ipheader.targetaddr.byte2, ipheader.targetaddr.byte3, ipheader.targetaddr.byte4);

																																			ipsourceaddr.GetXString(ipsourcestring);
																																			iptargetaddr.GetXString(iptargetstring);	

																																			xconsole->Printf(__L("IP Source: %s IP Target: %s"), ipsourcestring.Get(), iptargetstring.Get());

																																		}
																																} 																																																															
																																break;

																										default		:	break;
														}	

													xconsole->Printf(__L("\n"));

												}
									
											diopcap->Frames_Delete(c);
										}
								
								}
							
							diopcap->Capture_End();
						}
			
				}	 
			
			diopcap->End();														
		}

	diofactory->DeletePCap(diopcap);

	#endif
  
	return true;
}


/*-------------------------------------------------------------------
//	TEST::RPI_RevisionBoard
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2016 20:28:12
//	
//	@return 			bool : 
//
//  @param				model : 
//  @param				megabytes : 
//  @param				revision : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_RPI_RevisionBoard(RASPBERRYPI_MODEL& model, int& megabytes, float& revision)
{
	XPATH			xpath;
	XFILETXT* xfiletxt;
	bool			status = false;

	//xpath = __L("/proc/cpuinfo");

	xpath = __L("D:\\GENFrameWork\\Test\\resources\\cpuinfo");

	xfiletxt = new XFILETXT();
	if(!xfiletxt) return false;

	if(xfiletxt->Open(xpath))
		{
			xfiletxt->ReadAllFile();

			for(int c=0;c< xfiletxt->GetNLines(); c++)
				{
					XSTRING* line = xfiletxt->GetLine(c);
					if(line)
						{
							if(line->Find(__L("Revision"), false) != XSTRING_NOTFOUND)
								{							
									int position = line->Find(__L(":"), true);
									if(position != XSTRING_NOTFOUND)
										{
											line->DeleteCharacters(0, position+1);
											line->DeleteCharacter(0x20);
											
											model = RASPBERRYPI_MODEL_UNKNOWN;

											if(!line->Compare(__L("0002")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 1.0f; }
											if(!line->Compare(__L("0003")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 1.0f; }
											if(!line->Compare(__L("0004")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0005")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0006")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0007")		, true))  { model = RASPBERRYPI_MODEL_A;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0008")		, true))  { model = RASPBERRYPI_MODEL_A;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0009")		, true))  { model = RASPBERRYPI_MODEL_A;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("000d")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 512;			revision = 2.0f; }
											if(!line->Compare(__L("000e")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 512;			revision = 2.0f; }
											if(!line->Compare(__L("000f")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 512;			revision = 2.0f; }
											if(!line->Compare(__L("0010")		, true))  { model = RASPBERRYPI_MODEL_B_PLUS;						megabytes = 512;			revision = 1.0f; }
											if(!line->Compare(__L("0011")		, true))  { model = RASPBERRYPI_MODEL_COMPUTERMODULE; 	megabytes = 512;			revision = 1.0f; }
											if(!line->Compare(__L("0012")		, true))  { model = RASPBERRYPI_MODEL_A_PLUS;						megabytes = 256;			revision = 1.0f; }
											if(!line->Compare(__L("0013")		, true))  { model = RASPBERRYPI_MODEL_B_PLUS;						megabytes = 512;			revision = 1.2f; }
											if(!line->Compare(__L("a01041")	, true))  { model = RASPBERRYPI_MODEL_B_2;							megabytes = 1000;			revision = 1.1f; }
											if(!line->Compare(__L("a21041")	, true))  { model = RASPBERRYPI_MODEL_B_2;							megabytes = 1000;		  revision = 1.1f; }
											if(!line->Compare(__L("900092")	, true))  { model = RASPBERRYPI_MODEL_ZERO;							megabytes = 512;			revision = 1.2f; }

											if(model != RASPBERRYPI_MODEL_UNKNOWN) 
												{
													status = true;
													break;
												}
										}
								}
						}
				}	

			xfiletxt->Close();
		}

	delete xfiletxt;

	return status;	
}




/*-------------------------------------------------------------------
//	TEST::Test_DIOGPIO
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/10/2015 14:13:15
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOGPIO()
{	
	XPATH							 xpathroot;	
	bool							 status = false;

	/*
	RASPBERRYPI_MODEL	 model;
	int								 megabytes;
	float							 revision;

	Test_RPI_RevisionBoard(model, megabytes, revision);
	*/

	#define PINGPIO 11
	
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);

	DIOGPIO* diogpio = diofactory->CreateGPIO();
  if(diogpio) 
		{			
			if(diogpio->Ini())
				{								
					diogpio->SetMode(PINGPIO, false);
					
					while(!xconsole->KBHit())
						{
							diogpio->Set(PINGPIO, true);
							if(xtimer) xsleep->MilliSeconds(50);
			
							diogpio->Set(PINGPIO, false);
							if(xtimer) xsleep->MilliSeconds(50);
						}				

					diogpio->End();	
					status = true;
				}

			diofactory->DeleteGPIO(diogpio);
		}

	return status;
}





/*-------------------------------------------------------------------
//	TEST::Test_Lists
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/12/2015 15:52:00
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Lists()
{
	// test some lists
	XLIST<int> testlist;

	for(int i = 0; i < 30; i++)
		{
			testlist.Add(i);
		}

	if(testlist.Get(7) != 7)
		{
			XDEBUG_PRINTCOLOR(4,__L("Get failed"));
			return false;
		}

	/*for(XDWORD i = 0; i < testlist.GetSize(); i++)
	{
		XDEBUG_PRINTCOLOR(0,__L("%i"), testlist.Get(i));
	}*/

	testlist.Insert(5, 6);
	if(testlist.Get(5) != 6)
		{
			XDEBUG_PRINTCOLOR(4,__L("Insert failed"));
			return false;
		}

	for(XDWORD i = 0; i < 12; i++)
		{
			XDEBUG_PRINTCOLOR(0,__L("%i"), testlist.Get(i));
		}
	testlist.Delete(6);
	
	if(testlist.Get(6) != 6)
		{
			XDEBUG_PRINTCOLOR(4,__L("Delete failed"));
			return false;
		}

	testlist.DeleteIndex(10);

	if(testlist.Get(10) != 11)
		{
			XDEBUG_PRINTCOLOR(4,__L("DeleteIndex failed"));
			return false;
		}


	testlist.DeleteLast();

	if(testlist.GetLast() == 29)
		{
			XDEBUG_PRINTCOLOR(4,__L("DeleteLast failed"));
			return false;
		}

	testlist.GetSize();

	int previous0 = testlist.Get(0);
	int previous1 = testlist.Get(1);

	testlist.Swap(0, 1);
	if((previous0 != testlist.Get(1))
	|| (previous1 != testlist.Get(0)))
		{
			XDEBUG_PRINTCOLOR(4,__L("Swap failed"));
			return false;
		}

	testlist.ResizeContents(15);

	XLISTDATA<int>* toinsert = testlist.GetData(0);
	testlist.Insert(toinsert, 512);

	if(testlist.Get(0) != 512)
		{
			XDEBUG_PRINTCOLOR(4,__L("Insert with xlistdata failed"));
			return false;
		}

	toinsert = testlist.GetData(1);
	testlist.Insert(toinsert, 1024);

	if(testlist.Get(1) != 1024)
		{
			XDEBUG_PRINTCOLOR(4,__L("Insert with xlistdata failed"));
			return false;
		}

	testlist.Insert(testlist.GetLastData(), 2048);

	if(testlist.Get(testlist.GetSize() -1 -1) != 2048)
		{
			XDEBUG_PRINTCOLOR(4,__L("Insert with xlistdata failed"));
			return false;
		}

	// sample "pseudoiterator" usage
	for(XLISTDATA<int>* first = testlist.GetData(0); first != NULL; first = Increment(first))
		{
			XDEBUG_PRINTCOLOR(0,__L("%i"), first->data);
		}

	XLISTDATA<int>* data = testlist.GetData(6);
	testlist.Delete(data);

	testlist.ResizeContents(50);
	testlist.Resize(50);
	testlist.Set(10, 20);

	if(testlist.Get(10) != 20)
		{
			XDEBUG_PRINTCOLOR(4,__L("Set failed"));
			return false;
		}

	XDWORD location = testlist.Find(2);

	if(testlist.Get(location) != 2)
		{
			XDEBUG_PRINTCOLOR(4,__L("find failed"));
			return false;
		}

	testlist.DeleteAll();

	return true;


}



/*-------------------------------------------------------------------
//	TEST::Test_Audio
*/	
/**	
//	
//	tests the audio output
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 11:07:55
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Audio()
{
	#ifdef SND_ACTIVE
	
	if(!execparams->GetSize())
		{
			xconsole->Printf(__L("usage: parameter as sound name to load\n"));
			return false;
		}
	
	SNDFACTORY* soundsystem = SNDFACTORY::Get()->Instance;
	if(!soundsystem) return false;
		
	xconsole->Printf(__L("Prueba de sonido... %s\n"), execparams->Get(0)->Get());

	XPATH			xpathfile;
	XSTRING   idfile;

	xpathfile = execparams->Get(0)->Get();
	xpathfile.GetNamefileExt(idfile);
	xpathfile.SetOnlyDriveAndPath();
	
	if(!soundsystem->AddFile(xpathfile, idfile.Get()))  return false;

	xconsole->Printf(__L("Sonido cargado.\n"));
		
	SNDELEMENT* sound = soundsystem->GetFile(idfile.Get());
	if(!sound) return false;

	xconsole->Printf(__L("Sonido preparado.\n"));
		
	sound->SetLoop(true);
	sound->SetVolume(1.0f);
	
	soundsystem->PlaySound(sound);

	xconsole->Printf(__L("Reproduciendo sonido.\n"));
	
	while(true)
		{	
			if(xconsole->KBHit()) break;				
			xsleep->MilliSeconds(100);
		}
	
	
	#endif

	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_AVLTree
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2015 9:57:08
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_AVLTree()
{
	XAVLTREE<float> tree;

	XRAND* random = xfactory->CreateRand();

	for(XQWORD i = 0; i < 100; i++)
		{
			float a = random->Between(0.0f, 10000.0f);
			tree.Add(a);
			//tree.Add(i, i);
		}

	/*float a = 0.0f;
	float b = 0.0f;
	tree.Add(a, b);
	a = 3.0f; b = 3.0f;
	tree.Add(a, b);
	a = 8.0f; b = 8.0f;
	tree.Add(a, b);
	a = 6.0f; b = 6.0f;
	tree.Add(a, b);
	a = 8.0f; b = 8.0f;
	tree.Add(a, b);
	a = 7.0f; b = 7.0f;
	tree.Add(a, b);
	a = 3.0f; b = 3.0f;
	tree.Add(a, b);
	a = 8.0f; b = 8.0f;
	tree.Add(a, b);
	a = 8.0f; b = 8.0f;
	tree.Add(a, b);*/

	xfactory->DeleteRand(random);

	XAVLTREE<float>::XITERATOR it;
	
	it = tree.Begin();
	
	int i = 0;

	for(it = tree.Begin(); it != tree.End(); it++)
		{
			XDEBUG_PRINTCOLOR(4,__L("Element: %i : %f"), i, *it);
			i++;
		}

	float f = 8.0f;
	XLIST<float> l = tree.GetMultiple(f);

	XVECTOR<XDWORD> test;
	test.Add(1);
	test.Add(2);

	return true;
}






/*-------------------------------------------------------------------
//	TEST::Test_BinaryMap
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/01/2016 19:05:56
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_BinaryMap()
{
	XBINARYMAP<float, float> map;

	float a = 0.0f;
	float b = 0.0f;
	map.Add(a, b);
	a = 3.0f; b = 3.0f;
	map.Add(a, b);
	a = 8.0f; b = 8.0f;
	map.Add(a, b);
	a = 6.0f; b = 6.0f;
	map.Add(a, b);
	a = 8.0f; b = 8.0f;
	map.Add(a, b);
	a = 7.0f; b = 7.0f;
	map.Add(a, b);
	a = 3.0f; b = 3.0f;
	map.Add(a, b);
	a = 8.0f; b = 8.0f;
	map.Add(a, b);
	a = 8.0f; b = 8.0f;
	map.Add(a, b);
	
	XBINARYMAP<float, float>::XITERATOR it;

	it = map.Begin();
	
	int i = 0;
	
	for(it = map.Begin(); it != map.End(); it++)
		{
			XDEBUG_PRINTCOLOR(4,__L("Element: %i : %f"), i, *it);
			i++;
		}

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_Variants
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/03/2016 10:19:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Variants()
{
	XVARIANT variant;
	XSTRING string;

	xconsole->Clear();
	xconsole->Print(__L("--- Testing Variants --------------------------\n\n\n"));

	xconsole->Print(__L("  >  No assigment\n"));
	string.Format(__L("\t\t\tContent %d\n\n"),(int)variant);	xconsole->Print(string.Get());



	xconsole->Print(__L("  >  Asignment 10 integer\n"));
	variant=10;
	
	string.Format(__L("\t\t\tContent %d\n\n"),(int)variant);	xconsole->Print(string.Get());

	xconsole->Print(__L("  >  Asignment 4.3 float\n"));
	variant=4.3f;
	string.Format(__L("\t\t\tContent %f\n\n"),(float)variant);	xconsole->Print(string.Get());
	
	xconsole->Print(__L("  >  Asignment \"mytext\" literal\n"));
	variant=__L("mytext");
	string.Format(__L("\t\t\tContent %s\n\n"),(XCHAR*)variant);	xconsole->Print(string.Get());
	
	XSTRING s;
	s.Set("XstringText");
	xconsole->Print(__L("  >  Asignment \"XstringText\" xstring\n"));
	variant=s;
	string.Format(__L("\t\t\tContent %s\n\n"),(XCHAR*)variant);	xconsole->Print(string.Get());

	

	class TEST_VARIANT_SUBCLASS : public XSERIALIZABLE
	{
	public:
							TEST_VARIANT_SUBCLASS()
					{
						cabecera=rand()%5;
						data		=rand()%1000;
						delta		=0.33f*(rand()%10);
						bytes		[0]=0;
						bytes		[1]=1;
						bytes		[2]=2;
						bytes		[3]=3;
						bytes		[4]=4;

						XBUFFER buffer;
						
						buffer.Resize(	sizeof(XDWORD)	+
														sizeof(XDWORD)	+
														sizeof(float)		+
														sizeof(XBYTE)*5	);

						buffer.SetPosition(0);

						buffer.Set((XDWORD)cabecera);
						buffer.Set((XDWORD)data);
						buffer.Set((float)delta);
						buffer.Set(bytes[0]);
						buffer.Set(bytes[1]);
						buffer.Set(bytes[2]);
						buffer.Set(bytes[3]);
						buffer.Set(bytes[4]);

						

						HASHCRC32 hasher;
						hasher.Do(buffer);
						crc=hasher.GetResultCRC32();
					}
				 bool	DeSerialize	(XBUFFER* buffer) const
				 {
										buffer->Resize(	sizeof(int)			+
																		sizeof(XDWORD)	+
																		sizeof(float)		+
																		sizeof(XBYTE)*5	+
																		sizeof(XDWORD));

										buffer->SetPosition(0);

										buffer->Set((XDWORD)cabecera);
										buffer->Set(data);
										buffer->Set(delta);
										buffer->Set(bytes[0]);
										buffer->Set(bytes[1]);
										buffer->Set(bytes[2]);
										buffer->Set(bytes[3]);
										buffer->Set(bytes[4]);
										buffer->Set(crc);


										XBUFFER buf;

										buf.Resize(			sizeof(int)			+
																		sizeof(XDWORD)	+
																		sizeof(float)		+
																		sizeof(XBYTE)*5	);

										buf.Set((XDWORD)cabecera);
										buf.Set(data);
										buf.Set(delta);
										buf.Set(bytes[0]);
										buf.Set(bytes[1]);
										buf.Set(bytes[2]);
										buf.Set(bytes[3]);
										buf.Set(bytes[4]);


										HASHCRC32 hasher;
										hasher.Do(buf);
										if (crc!=hasher.GetResultCRC32())
										{
											XDEBUG_PRINTCOLOR(0,(__L("Error con el transporte del Serializable")));
										}

										return true;
				 }
				 bool	Serialize		(XBUFFER* buffer)
 {
		buffer->SetPosition(0);
		buffer->Get((XDWORD&)cabecera);
		buffer->Get((XDWORD&)data);
		buffer->Get((float&)delta);
		buffer->Get(bytes[0]);
		buffer->Get(bytes[1]);
		buffer->Get(bytes[2]);
		buffer->Get(bytes[3]);
		buffer->Get(bytes[4]);
		buffer->Get((XDWORD&)crc);

								return true;
 }

					int			cabecera;
					XDWORD	data;
					float		delta;
					XBYTE		bytes[5];
					XDWORD  crc;
	};

	
	int nTests=9999;
  xconsole->Printf(__L("  >  Stress assignment x%d\n "),nTests);
	XSTRING result;
	result.Set(__L(""));

	XVECTOR<XVARIANT*> vector;
	XCHAR* words[]={
									__L("Lorem"),
									__L("ipsum"),
									__L("dolor"),
									__L("sit"),
									__L("amet"),
									__L("consectetur"),
									__L("adipiscing"),
									__L("elit"),
									__L("Duis"),
									__L("auctor"),
									__L("leo"),
									__L("non"),
									__L("viverra"),
									__L("luctus")
									};


	XVECTOR<XVARIANT*>	variant_list;
	XVECTOR<XSTRING*>		variant_value;

	int fails=0;
	int e=0;
	for (e=0;e<nTests;e++)
	{
		XVARIANT* variant=new XVARIANT();

	//	int r=rand()%6;
		int r=3;

		switch(r)
		{	
		case 0:		{	
									int		i	=	(int)rand();														
									*variant	=	i;		
									string.Format(__L("%d -> Content int   %d %s\n"),i,(int)*variant,((i==(int)*variant)? __L("[OK]") : __L("[ERROR]")));		
										if (i!=(int)*variant)
										fails++;
									result.Add(string.Get()); 
									break;
							};

		case 1:		{	
									float i = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
									*variant=i;  	
									string.Format(__L("%f -> Content float %f %s\n"),i,(float)*variant,((i==(float)*variant)? __L("[OK]") : __L("[ERROR]")));	

									if (i!=(float)*variant)
										fails++;

									result.Add(string.Get()); 
									break;
							}
		case 2:		{
									int w=rand()%14;
									*variant=words[w];  	
									if (XSTRING::Compare(*variant,words[w]))
									{
										string.Format(__L("%s -> Content string %s [ERROR]\n"),words[w],(XCHAR*)*variant);
										fails++;
									}
									else
									string.Format(__L("%s -> Content string %s [OK]\n"),words[w],(XCHAR*)*variant);

									result.Add(string.Get()); 
									break;
							}
			case 4:		{	
									
									variant->Set();
									string.Format(__L("%d -> Content NULL \n"),variant->GetData());		
									if (!variant->IsNull())
										fails++;

									result.Add(string.Get()); 
									break;
							};

			case 3: {
									
								TEST_VARIANT_SUBCLASS* vsc=new TEST_VARIANT_SUBCLASS();
								*variant=*vsc;

								TEST_VARIANT_SUBCLASS res;
								variant->GetSerializable(&res);

								if (vsc->crc!=res.crc)
									fails++;


								string.Format(__L("[Serializable] -> Content CRC: %08x %08x %s\n"),vsc->crc,res.crc, (vsc->crc==res.crc) ? __L("[OK]") : __L("[ERROR]"));
								result.Add(string.Get()); 
								delete(vsc);
							}
							break;

		case 5:		{	
									XDWORD		i	=	(XDWORD)rand();														
									*variant	=	i;		
									string.Format(__L("%u -> Content int   %u %s\n"),i,(XDWORD)*variant,((i==(XDWORD)*variant)? __L("[OK]") : __L("[ERROR]")));		
										if (i!=(XDWORD)*variant)
										fails++;
									result.Add(string.Get()); 
									break;
							};
		}
		vector.Add(variant);
	}


	xconsole->Print(result.Get());
	result.Format(__L("\n\n\n\t\t\tResult %d Succed, %d fails\n\n\n"),e-fails,fails);
	xconsole->Print(result.Get());
	xconsole->Print(__L("  >  Deletion test\n"));
	
	//for (XDWORD e=0;e<vector.GetSize();e++)		*vector.Get(e)=0;

	vector.DeleteContents();
	vector.DeleteAll();

	xconsole->Print(__L("-------------- Test Finished ------------ \n"));
	while (!xconsole->KBHit());
	while (xconsole->KBHit())	xconsole->GetChar();

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_VariantProtocol
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 14:03:09
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_VariantProtocol(bool modeserver)
{
	TESTPROTOCOLCONNEXIONSMANAGER*	protocolconnexionmanager	= NULL;		
	bool														status										= true;	

	xconsole->Clear();
	xconsole->Printf(__L("---- Variant protocol test Start--- \n\n"));
	
	xconsole->Printf(__L("Elegir modo: \n\t\t(s) server\n\t\t(c) client\n"));
	
	while (!xconsole->KBHit());

	while (xconsole->KBHit())
	{
	char mode=xconsole->GetChar();
	if (mode=='s')
		modeserver=true;
	else
		modeserver=false;
	}
	
		xconsole->Printf(__L("En modo servidor: %s.\n"), modeserver?__L("Si"):__L("No"));
	
	protocolconnexionmanager = new TESTPROTOCOLCONNEXIONSMANAGER;
	if(protocolconnexionmanager)
		{
			if(!modeserver) 
				protocolconnexionmanager->ProtocolConnexions_SetNLimit(1);

			protocolconnexionmanager->TargetURL_Add(__L(""));	
							
			if(protocolconnexionmanager->Ini(modeserver, protocolapplicationdata))
				{					
					xconsole->Print(__L("Open Connexions...\n"));

				} else status = false;

		} else status = false;

	//--------Bucle

	if(status) 
		{ 			 
			status = false;
			int countercommand = 0;
		
			do{					
					int indexprotocolconnexion = 0;
							int nconnexions						 = protocolconnexionmanager->ProtocolConnexions_GetNAvailable(); 

							xconsole->Printf(__L("\nConexiones actuales: (%d) \n"), nconnexions);																																								
																																								
							xconsole->Clear();

							xconsole->TipicalHeader_Show(2012	, APPLICATION_NAMEAPP
																								, APPLICATION_VERSION
																								, APPLICATION_SUBVERSION
																								, APPLICATION_SUBVERSIONERR
																								, APPLICATION_ENTERPRISE);

							xconsole->PrintMessage(__L("\n\n"),0,false,false);

							for(int c=0; c<nconnexions; c++)
								{
 									DIOPROTOCOLCONNEXION*	protocolconnexion = protocolconnexionmanager->ProtocolConnexions_Get(c);
									if(protocolconnexion) 
										{
											XSTRING IPstring;

											TESTPROTOCOL* protocol = (TESTPROTOCOL*)protocolconnexion->GetDIOProtocol();
											if(protocol) 
												{													
													DIOSTREAMCIPHER* diostreamcipher =  (DIOSTREAMCIPHER*)protocol->GetDIOStream();
													if(diostreamcipher) 
														{												
															DIOSTREAMTCPIP* diostreamtcpip = (DIOSTREAMTCPIP*)diostreamcipher->GetDIOStreamBase();
															if(diostreamtcpip) 
																{													
																	DIOIP* remoteIP = diostreamtcpip->GetClientIP();
																	if(remoteIP) 
																		{
																			remoteIP->GetXString(IPstring);
																			if(protocol->IsConnected()) 
																				{
																					xconsole->Printf(__L("%02d) %-15s (r:%d / w:%d)\n"), c+1, IPstring.Get(), protocol->GetDIOStream()->GetNBytesRead(), protocol->GetDIOStream()->GetNBytesWrite());																																													
																				}
																		}
																}
														}
												}
										}									
								}
							
							if(xtimer) 
								xsleep->Seconds(1);			
				
				} while(!xconsole->KBHit());
		}

	//----- fin
	xconsole->Printf(__L("---- Variant protocol test End--- \n\n"));
	if(protocolconnexionmanager)
				{
					if(protocolconnexionmanager->End())
						{							
							delete protocolconnexionmanager;	
							protocolconnexionmanager = NULL;
						}
				}		

		/*
	if(status) 
		{ 			 
			status = false;
			int countercommand = 0;
		
			do{					
					if(!modeserver)
						{	
							XBYTE		version     = 1;
							XBYTE		subversion  = 2;
							XDWORD	IDDBversion = 33;

							status = false;

							DIOPROTOCOLCONNEXION*	protocolconnexion = protocolconnexionmanager->ProtocolConnexions_Get(0);
							if(protocolconnexion) 
								{
									protocolconnexion->SetCanDelete(false);

									PROTOCOL* protocol = (PROTOCOL*)protocolconnexion->GetDIOProtocol();
									if(protocol) 
										{
											if(protocol->IsConnected()) 
												{																	
													status = protocol->CMD_GetVersionXP(version, subversion, IDDBversion);										
													countercommand++;
												}																		
										}

									protocolconnexion->SetCanDelete(true);
								}

							if(countercommand)
								{
									xconsole->Printf(__L("(%5d) Get Version %02d.%02d.%02d ...[%s]"), countercommand, version, subversion, IDDBversion, status?__L("Ok!"):__L("Error!"));	
									xconsole->Printf(status?__L("                        \r"):__L("\n"));
								}
						}
					 else
						{
							int indexprotocolconnexion = 0;
							int nconnexions						 = protocolconnexionmanager->ProtocolConnexions_GetNAvailable(); 

							xconsole->Printf(__L("\nConexiones actuales: (%d) \n"), nconnexions);																																								
																																								
							xconsole->Clear();

							TipicalHeader_Show(2012	, APPLICATION_NAMEAPP, APPLICATION_VERSION
																			, APPLICATION_SUBVERSION
																			, APPLICATION_SUBVERSIONERR
																			, APPLICATION_ENTERPRISE);

							xconsole->PrintMessage(__L("\n\n"),0,false,false);

							for(int c=0; c<nconnexions; c++)
								{
									DIOPROTOCOLCONNEXION*	protocolconnexion = protocolconnexionmanager->ProtocolConnexions_Get(c);
									if(protocolconnexion) 
										{
											XSTRING IPstring;

											PROTOCOL* protocol = (PROTOCOL*)protocolconnexion->GetDIOProtocol();
											if(protocol) 
												{													
													DIOSTREAMCIPHER* diostreamcipher =  (DIOSTREAMCIPHER*)protocol->GetDIOStream();
													if(diostreamcipher) 
														{												
															DIOSTREAMTCPIP* diostreamtcpip = (DIOSTREAMTCPIP*)diostreamcipher->GetDIOStreamBase();
															if(diostreamtcpip) 
																{													
																	DIOIP* remoteIP = diostreamtcpip->GetClientIP();
																	if(remoteIP) 
																		{
																			remoteIP->GetXString(IPstring);
																			if(protocol->IsConnected()) 
																				{
																					xconsole->Printf(__L("%02d) %-15s (r:%d / w:%d)\n"), c+1, IPstring.Get(), protocol->GetDIOStream()->GetNBytesRead(), protocol->GetDIOStream()->GetNBytesWrite());																																													
																				}
																		}
																}
														}
												}
										}									
								}
							
							if(xtimer) xsleep->Seconds(1);			
						}
					
				} while(!xconsole->KBHit());

			if(protocolconnexionmanager)
				{
					if(protocolconnexionmanager->End())
						{							
							delete protocolconnexionmanager;	
							protocolconnexionmanager = NULL;
						}
				}		
		}
		*/
	return true;
}

/*-------------------------------------------------------------------
//	 TEST::Test_Map
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	24/01/2018 9:49:54
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TEST::Test_Map()
{
		XMAP<XDWORD, XDWORD>		map;
														map.Add(0, 1);
														map.Add(1, 1);
														map.Add(2, 1);
														map.Add(3, 1);		

		for (XDWORD e = 0; e < map.GetSize(); e++)
		{
				XSTRING s;
				s.Format(__L("Map %d,%d \r\n"), map.GetKey(e), map.GetElement(e));
				xconsole->Print(s.Get());
		}

		return true;
}

/*-------------------------------------------------------------------
//	TEST::Test_Vector
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/03/2016 13:49:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	TEST::Test_Vector()
{
	XVECTOR<int> test;

	test.Add(1);
	test.Add(2);
	test.Add(3);
	test.Add(4);
	test.Add(5);
	test.Add(6);
	test.Add(7);
	test.Add(8);
	test.Add(9);
	test.Add(0);

	int maxsize=test.GetSize();

	xconsole->Print(__L("Created Vector from 1-10\n\n"));

	for (XDWORD e=0;e<test.GetSize();e++)
	{
		XSTRING s;
		s.Format(__L("\tV[%d]=%d\n"),e,test.Get(e));
		xconsole->Print(s.Get());
	}
	
	int nelementstoremove=rand()%6;

		XSTRING s;		s.Format(__L("Removing %d elements randomly\n\n"),nelementstoremove);	xconsole->Print(s.Get());

	for (int e=0;e<nelementstoremove;e++)
	{
	int n=rand()%test.GetSize();
	s.Format(__L("\t\tRemoving %d element\n"),n);	xconsole->Print(s.Get());
	test.DeleteIndex(n);
	}

	for (XDWORD e=0;e<test.GetSize();e++)
	{
		XSTRING s;
		s.Format(__L("\tV[%d]=%d\n"),e,test.Get(e));
		xconsole->Print(s.Get());
	}

	int newSize=rand()%(test.GetSize()*2);
	s.Format(__L("Resizing %d\n\n"),newSize);	xconsole->Print(s.Get());
	
	test.Resize(newSize);

	for (XDWORD e=0;e<test.GetSize();e++)
	{
		XSTRING s;
		s.Format(__L("\tV[%d]=%d\n"),e,test.Get(e));
		xconsole->Print(s.Get());
	}

	int npos=0;
	if (test.GetSize()>0)
			 npos		=rand()%test.GetSize();
	else 
			 npos=0;

	int nvalue	=rand()%10;

	s.Format(__L("Inserting random number %d on %d position\n\n"),nvalue,npos);	xconsole->Print(s.Get());

	test.Insert(npos,nvalue);

	
	for (XDWORD e=0;e<test.GetSize();e++)
	{
		XSTRING s;
		s.Format(__L("\tV[%d]=%d\n"),e,test.Get(e));
		xconsole->Print(s.Get());
	}

	int nSwapPos1=rand()%maxsize;
	int nSwapPos2=rand()%maxsize;

	s.Format(__L("Swapping random position %d with %d position\n\n"),nSwapPos1,nSwapPos2);	xconsole->Print(s.Get());
	test.Swap(nSwapPos1,nSwapPos2);

		for (XDWORD e=0;e<test.GetSize();e++)
	{
		XSTRING s;
		s.Format(__L("\tV[%d]=%d\n"),e,test.Get(e));
		xconsole->Print(s.Get());
	}

		s.Format(__L("Deleting full vector\n\n"),nSwapPos1,nSwapPos2);	xconsole->Print(s.Get());

		if (!test.DeleteAll())
			{
			XSTRING out;
			out.Format(__L("\t\t\t\t *** Deletion failed"));	xconsole->Print(out.Get());
			}

		//-----------------
		s.Format(__L("----------------------- String vector tests --------------\n\n\t"));	xconsole->Print(s.Get());


	XCHAR* words[]={
									__L("Lorem"),
									__L("ipsum"),
									__L("dolor"),
									__L("sit"),
									__L("amet"),
									__L("consectetur"),
									__L("adipiscing"),
									__L("elit"),
									__L("Duis"),
									__L("auctor"),
									__L("leo"),
									__L("non"),
									__L("viverra"),
									__L("luctus")
									};

	s.Format(__L("Inserting words on vector\n\n\t"));	xconsole->Print(s.Get());
	

	XVECTOR<XSTRING*> stringvector;
	for (int e=0;e<14;e++)
	{
	XSTRING* s=new XSTRING();
	s->Set(words[e]);
	if (!stringvector.Add(s))
			{
			XSTRING out;
			out.Format(__L("\t\t\t\t ***Deletion failed\n\n\t"));	xconsole->Print(out.Get());
			}
	}
			
	for (XDWORD e=0;e<stringvector.GetSize();e++)
	{	
		xconsole->Print(stringvector.Get(e)->Get());
		xconsole->Print(__L("\n\t"));
	}


		nelementstoremove=rand()%stringvector.GetSize();
		s.Format(__L("Removing %d elements randomly\n\n"),nelementstoremove);	xconsole->Print(s.Get());
		
	for (int e=0;e<nelementstoremove;e++)
	{
	int n=rand()%stringvector.GetSize();
	s.Format(__L("\t\tRemoving %d element\n\t"),n);	xconsole->Print(s.Get());
		if (!stringvector.DeleteIndex(n))
		{
			s.Format(__L("\t\t\t\t ***Insertion failed"));	xconsole->Print(s.Get());
		}

	}

	for (XDWORD e=0;e<stringvector.GetSize();e++)
	{
		xconsole->Print(stringvector.Get(e)->Get());
		xconsole->Print(__L("\n\t"));
	}

	s.Format(__L("swapping positions %d <-> %d\n\n"),nSwapPos1,nSwapPos2);	xconsole->Print(s.Get());
	

	if (!stringvector.Swap(nSwapPos1,nSwapPos2))
	{
	s.Format(__L("\t\t\t\t ***Swapping failed"));	xconsole->Print(s.Get());
	}

	
	for (XDWORD e=0;e<stringvector.GetSize();e++)
	{
		xconsole->Print(stringvector.Get(e)->Get());
		xconsole->Print(__L("\n\t"));
	}

	s.Format(__L("Deleting Contents"));	xconsole->Print(s.Get());
	
	stringvector.DeleteContents	();
	stringvector.DeleteAll			();	





	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_DIOStreamTCPIPConnection
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/06/2016 12:24:39
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_DIOStreamTCPIPConnection()
{
	DIOSTREAMTCPIPCONFIG diostreamcfg; 
	DIOSTREAMTCPIP*			 diostream    = NULL; 	
	bool								 status;

	diostreamcfg.SetMode(DIOSTREAMMODE_CLIENT);				
	diostreamcfg.GetRemoteURL()->Set(__L("mail.pictel.es"));
	diostreamcfg.SetRemotePort(587); //587

	xconsole->Printf(__L("Connectando con:  %s : %d\n"),diostreamcfg.GetRemoteURL()->Get(),diostreamcfg.GetRemotePort());
	diostream = (DIOSTREAMTCPIP*)diofactory->CreateStreamIO(&diostreamcfg);
	if(diostream)
		{
			int c = 0;
			 
			while(!xconsole->KBHit())
				{
					if(diostream->Open())
						{ 
							status = diostream->WaitToConnected(5);
							
							diostream->Close();
							c++;
							xconsole->Printf(__L("Conexion %d estado: %s"), c, status?__L("Ok.       \n"):__L("Error!    \n"));					 							
						}
				}

			diofactory->DeleteStreamIO(diostream);
		}

	xconsole->GetChar();

	diostreamcfg.SetMode(DIOSTREAMMODE_SERVER);				
	diostreamcfg.SetRemotePort(8099);//587

	xconsole->Printf(__L("\n\nAbriendo servidor local  %s : %d\n\n"),diostreamcfg.GetLocalIP()->Get(),diostreamcfg.GetRemotePort());

	diostream = (DIOSTREAMTCPIP*)diofactory->CreateStreamIO(&diostreamcfg);
	if(diostream)
		{
			int c=0;
			while(!xconsole->KBHit())
				{
					if(diostream->Open())
						{ 
							status = diostream->WaitToConnected(1);
													
							xconsole->Printf(__L("\r\nConexion %d estado: %s"), c, status?__L("Connected.       "):__L("Waiting...    "));					 							

							while(diostream->GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED)
								{								
									xconsole->Printf		(__L("\r\n    Sending packet(%d)"),c++);
									diostream->WriteStr	(__L("Hello Jello!\r\n"));
									diostream->WaitToFlushOutXBuffer();
									diostream->Wait(500);
								}

							if (diostream->GetConnectStatus()!=DIOSTREAMSTATUS_GETTINGCONNEXION)
							xconsole->Printf(__L("\r\nConexion %d estado: Disconnected"), c);					 							
							
							diostream->Close();
							
							c=0;
						}
				}

			diofactory->DeleteStreamIO(diostream);
		}


	return true;
}



/*-------------------------------------------------------------------
//	TEST::Test_Zip
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/06/2016 11:55:20
//	
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_Zip()
{
	bool status=true;

 	COMPRESSMANAGER*	manager=new COMPRESSMANAGER();
	COMPRESSBASE*			compressor=manager->Create(COMPRESSBASE_TYPE_GZ);

	XFILE* xfiletxt = xfactory->Create_File();
	if(!xfiletxt) return false;

	XPATH xpath,xpath2,xpathtarget;

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
	xpath.Add(__L("test.txt"));
	
	xpathtarget.Add(__L("test.txt"));

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath2);
	xpath2.Slash_Add();
	xpath2.Add(__L("test.zip"));

								
	XFILEZIP* zipfile = new XFILEZIP();		
	if(!zipfile) return false;

	status = zipfile->Open(xpath2);
	if(status)
		{
			zipfile->AddFile(xpath,xpathtarget);
			zipfile->Close();
		}
	
	delete(zipfile);

	xconsole->Printf(__L("\r\nCreating Zip file: %s"), status?__L("Success.       "):__L("Success  "));					 							

	if(xfiletxt->Open(xpath))
		{
			int size = xfiletxt->GetSize();

			XBUFFER* dec = new XBUFFER(size,false);
			if(!dec) return false;

			XBUFFER* buf = new XBUFFER(size,false);	
			if(!buf) return false;

			xfiletxt->Read(buf->Get(),xfiletxt->GetSize());

			XBUFFER* result = new XBUFFER();	
			status = compressor->Compress(buf->Get(),buf->GetSize(),result);

			if(status)
				{
					XPATH xpathgz;
				
					XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathgz);
					
					xpathgz.Slash_Add();
					xpathgz.Add(__L("test.gz"));

					XFILE* xfilegz = xfactory->Create_File();
					if(!xfilegz)
						{
							xfilegz->Create(xpathgz);
							xfilegz->Write	(*result);
							xfilegz->Close();
							xfactory->Delete_File(xfilegz);
						}

					status=compressor->Decompress(result->Get(),result->GetSize(),dec);
				}

			xconsole->Printf(__L("\r\nCompressing Chunk: %s"), status?__L("Success.       "):__L("Success  "));					 							

			if(!memcmp(dec->Get(),buf->Get(),buf->GetSize()))
						 xconsole->Printf(__L("\r\nBuffer check passed!"));					 							
				else xconsole->Printf(__L("\r\nBuffer check FAILED!"));	

			delete(dec);
			delete(buf);
			delete(result);
		}

	xfactory->Delete_File(xfiletxt);

	delete(compressor);

	delete(manager);

	return true;
}




/*-------------------------------------------------------------------
//	TEST::Test_StringProtocol
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			02/02/2017 12:04:59
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TEST::Test_StringProtocol(bool modeserver)
{	
	DIOSTREAMUARTCONFIG diostreamcfg;
	DIOSTREAMUART*			diostream;
																	
	#ifdef HW_RASPBERRYPI	
	diostreamcfg.GetLocalDeviceName()->Set(__L("/dev/ttyACM0"));
	//diostreamcfg.GetLocalDeviceName()->Set(__L("/dev/ttyUSB0"));						
	//diostreamcfg.GetLocalDeviceName()->Set(__L("/dev/ttyAMA0"));	
	//diostreamcfg.GetLocalDeviceName()->Set(__L("/dev/ttyS0"));											
	#endif							

	diostreamcfg.SetPort(8);		
	diostreamcfg.SetBaudRate(115200);
	diostreamcfg.SetDataBits(DIOSTREAMUARTDATABIT_8);
	diostreamcfg.SetParity(DIOSTREAMUARTPARITY_NONE);
	diostreamcfg.SetStopBits(DIOSTREAMUARTSTOPBITS_ONE);
	diostreamcfg.SetFlowControl(DIOSTREAMUARTFLOWCONTROL_NONE);	

	diostream = (DIOSTREAMUART*)diofactory->CreateStreamIO(&diostreamcfg);
	if(diostream) 
		{
			DIOSTRINGPROTOCOL* stringprotocol =  new DIOSTRINGPROTOCOL(diostream);
			if(stringprotocol)
				{

					if(stringprotocol->Ini())
						{




							stringprotocol->End();
						}
				}	
			
			diofactory->DeleteStreamIO(diostream);
		}

	return true;
}





/*-------------------------------------------------------------------
//	 TEST::Test_XDebug
*/
/**
//
//	
//
//	@author		Abraham J. Velez 
//	@version		
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TEST::Test_XDebug()
{
	XDWORD counter = 0;

	while(!xconsole->KBHit())
		{
			XDEBUG_PRINTCOLOR((counter%2)?3:2, __L(" [%8d] Linea de DEBUG .... "), counter);
			counter++;			
		}

	return true;

}

/*-------------------------------------------------------------------
//	 TEST::Test_String
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	29/01/2018 10:36:51
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TEST::Test_String()
{
		XSTRING str = __L("410.0");

		int amount = str.ConvertToInt();

		/*
		XSTRING s;
		s.Set(__L("El veloz murcielago hindu comia 6 cardillos y 1.3 kiwis."));

		XSTRING animal;
		animal.AdjustSize(100);

		XCHAR* hold[100];
		int cardillos = 0;
		float kiwis = 0.0f;

		XSTRING mask = __L("El veloz %s hindu comia %d cardillos y %f kiwis.");
		int n= UNICODESSCANF(s.Get(),mask.Get(), hold,100,&cardillos, &kiwis);
		
		XSTRING out;
		out.Format(__L("%s %d %f"), animal.Get(), cardillos, kiwis);
		xconsole->Print(out.Get());
		*/

		/*
		XSTRING s;
		s = __L("01010101");
		unsigned long vp = 0;
		s.atob(&vp, 2);

		char test[20] = "esto es 1 test";
		XSTRING text;
		text.AdjustSize(100);
		int number;
		s.Scan(__L("%s es %d test"), text.Get(), &number);
		*/

		/*
		XSTRING s;
		s = __L("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse risus diam, luctus vitae finibus non, imperdiet sit amet tellus. In hac habitasse platea dictumst. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Ut turpis velit, consequat in pellentesque tristique, rhoncus at enim. Curabitur sed justo ex. Nam cursus vitae lacus et semper. Nam venenatis magna at mollis laoreet. Interdum et malesuada fames ac ante ipsum primis in faucibus. Aenean eu feugiat massa, non sagittis nisi. Nullam vel mollis risus. Suspendisse egestas leo risus, a lobortis justo aliquam ut."
				"Sed ac sodales lectus.Praesent id turpis fringilla, luctus mi vitae, mollis quam.Aenean ut neque id dolor pretium elementum.Nam in sollicitudin leo, at consectetur est.Nunc et massa eleifend, faucibus magna eleifend, interdum lacus.Morbi semper laoreet tristique.Suspendisse porta pellentesque leo, id auctor sem lobortis vel.Praesent a risus vestibulum, feugiat neque ac, tempor odio.Suspendisse luctus quam vestibulum placerat feugiat."
				"Aenean in sagittis nunc, dignissim vestibulum diam.Nulla facilisi.Vivamus est magna, iaculis vel urna vel, ultricies finibus tellus.Phasellus et aliquam eros.Mauris vel nunc tellus.Praesent eget risus mi.Mauris pharetra aliquam est, non tristique eros.Phasellus tempor nunc eget eros finibus porta.Integer mollis semper molestie.In hac habitasse platea dictumst."
				"Morbi pharetra laoreet porta.Cras accumsan velit in eros ullamcorper consequat.Integer a sodales odio.Suspendisse volutpat mattis purus sed faucibus.Mauris non ullamcorper sem.Sed accumsan lacus arcu, eget malesuada magna imperdiet quis.Suspendisse sit amet odio libero.Suspendisse accumsan finibus faucibus.Mauris maximus et nisi in tristique."
				"Maecenas sodales ultrices ligula ut ornare.Suspendisse semper nibh diam, vitae molestie leo iaculis eget.Donec finibus pretium dui ac vestibulum.Pellentesque id congue felis, vitae fermentum magna.Interdum et malesuada fames ac ante ipsum primis in faucibus.In condimentum accumsan quam ac euismod.Vivamus malesuada neque id purus pretium, in molestie lacus efficitur.Cras viverra orci sed vulputate pretium.Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.Ut et augue accumsan leo consectetur eleifend.Vestibulum pellentesque at erat a scelerisque.In at sem a lorem congue ultrices.Integer ante lectus, tincidunt eget lorem nec, pulvinar molestie libero.Mauris dictum diam sit amet sapien blandit tempus.");

		xconsole->Print(s.Get());

		xconsole->Print(__L("-------------------------------"));

		XTIMER* timer = xfactory->CreateTimer();

		timer->Reset();

		s.DeleteCharacter(__C('a'));

		xconsole->Printf(__L("Time: %d\n\n"),timer->GetMeasureMicroSeconds());
		xconsole->Print(s.Get());
		*/
		/*
		XSTRING s;
		s = "\r\n\r   \t\tel veloz murcielago\t\t\r\n hindu";
		xconsole->Print(s.Get());

		xconsole->Print(__L("\r\n-------------------------------\n\n"));

		s.DeleteNoCharacters();
		xconsole->Print(s.Get());
		*/

		/*
		XSTRING n1;
		XSTRING n2;
		XSTRING n3;
		n1.AdjustSize(100);
		n2.AdjustSize(100);
		n3.AdjustSize(100);		
		
		int i = 0;
		s = test;
		s.UnFormat(__L("%s %s %d %s"), n1.Get(), n2.Get(), &i, n3.Get());
		*/

		/*
		//--------------- diferencia 1 : tratamiento de los %
		char string[] = "esto es %12  8 de test 1.34 3.8 9.2";
		int i = 0;
		int e = 0;
		sscanf(string, "esto es %%%d %d de test 1.34 3.8 9.2", &i, &e);

		XSTRING s;
		s.Set(string);
		s.UnFormat(__L("esto es %%%d %d de test 1.34 3.8 9.2"), &i,&e);
		*/
		//--------------- diferencia 3 : problemas con el double
		/*
		double t1 = 0, t2=0, t3=0;
		sscanf(string, "esto es %%12  8 de test %lf %lf %lf", &t1, &t2, &t3);
		t1 = 0, t2 = 0, t3 = 0;
		s.Set(string);
		s.UnFormat(__L("esto es %%12  8 de test %lf %lf %lf"), &t1, &t2, &t3);

		float f1 = 0,f2 = 0,f3 = 0;
		sscanf(string, "esto es %%12  8 de test %f %f %f", &f1, &f2, &f3);
		f1 = 0, f2 = 0, f3 = 0;
		s.Set(string);
		s.UnFormat(__L("esto es %%12  8 de test %f %f %f"), &f1, &f2, &f3);
		*/ //pues parece funcionar esto , que raro


		//------------- floats
		/*
		float fe = 0;
		s.Set(__L("1.9"));
		s.UnFormat(__L("%f"), &fe);

		double de = 0;
		s.Set(__L("1.9"));
		s.UnFormat(__L("%f"), &de);
		
		double lde = 0;
		s.Set(__L("1.9"));
		s.UnFormat(__L("%lf"), &lde);

		//--------------- diferencia 2 : ignorar el *
				sscanf(string, "%*s es %%%d %d de test 1.34", &i, &e);

				s.Set(string);
				s.UnFormat(__L("%*s es %%%d %d de test 1.34"), &i, &e); //<<este directamente peta

				*/
		/*
		XSTRING s;
		int i = 0;
		float e = 0;
		s.Set("esto es %12 1 de test 1.34");
		int n=s.Scan(__L("%*s es %%%d %d de test 1.34"), &i, &e);
		*/
		return true;
}


/*-------------------------------------------------------------------
//  TEST::HandleEvent
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/08/2012 17:12:12
//	
//	@return 			void : 
//	@param				xevent : 
*/
/*-----------------------------------------------------------------*/
void TEST::HandleEvent(XEVENT* xevent)
{
	if(!xevent) return;

	switch(xevent->GetEventFamily())
		{	
			case XEVENTTYPE_NONE											: break;


			case XEVENTTYPE_PROTOCOLCONNEXIONS				: { DIOPROTOCOLCONNEXIONSMANAGERXEVENT* event = (DIOPROTOCOLCONNEXIONSMANAGERXEVENT*)xevent;
																										if(!event) break;
																								
																										switch(xevent->GetEventType())
																											{																											
																												case DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_CONNECTEDCONNEXION		: { 

																																																														}
																																																														break;
																									
																												case DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_INITPROTOCOL					: { DIOPROTOCOLCONNEXION*	 protocolconnexion =	protocolconnexionmanager->ProtocolConnexions_Get(0);
																																																															TESTPROTOCOL*					 protocol					 =	NULL;
																																																														
																																																															if(protocolconnexion) protocol=	(TESTPROTOCOL*)protocolconnexion->GetDIOProtocol();																																																																																																																																																																																								
																																																															if(protocol)	
																																																																{																																																																	
																																																																	protocol->SetIsInitialized(true);
																																																																}
																																																														}	
																																																														//XDEBUG_PRINTCOLOR(2,__L("Connexion Manager: Init Protocol"));
																																																														break;

																												case DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION	: { 

																																																														}  
																																																														//XDEBUG_PRINTCOLOR(2,__L("Connexion Manager: Disconnected Connexion. "));
																																																														break;			
																											}																																																															
																									}
																									break;
		
	
			case XEVENTTYPE_WEBCLIENT								:	{	DIOWEBCLIENTXEVENT* event = (DIOWEBCLIENTXEVENT*)xevent;
																									if(!event) return;																				

																									switch(xevent->GetEventType())
																										{						
																											case DIOWEBCLIENTXEVENTTYPE_OPENWEB					:
																											case DIOWEBCLIENTXEVENTTYPE_WRITEHEADER		 	:
																											case DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA		:
																											case DIOWEBCLIENTXEVENTTYPE_READHEADER			:
																											case DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK		:
																											case DIOWEBCLIENTXEVENTTYPE_CLOSEWEB				: break;
																										}																																																								
																								}
																								break;

			case XEVENTTYPE_WEBSERVER								:	{	DIOWEBSERVERXEVENT* event = (DIOWEBSERVERXEVENT*)xevent;
																									if(!event) return;																				

																									switch(xevent->GetEventType())
																										{						
																											case DIOWEBSERVERXEVENTTYPE_REQUEST					:	Test_WEBServerRequest(event);
																																																		break;

																											case DIOWEBSERVERXEVENTTYPE_TAGFOUND		 		: Test_WEBServerTag(event);
																																																		break;
																												
																										}																																																								
																								}
																								break;


			case XEVENTTYPE_PING										:	{	DIOPINGXEVENT* event = (DIOPINGXEVENT*)xevent;
																									if(!event) return;
																												
																									switch(xevent->GetEventType())
																										{			
																											case DIOPINGXEVENTTYPE_DOPING								: break;
																										}																														
																								}
																								break;


			case XEVENTTYPE_SCHEDULER								: { XSCHEDULERXEVENT* event = (XSCHEDULERXEVENT*)xevent;
																									if(!event) return;

																									switch(event->GetTask()->GetID())
																										{
																											case  1:	XDEBUG_PRINTCOLOR(4,__L("TASK Event...."));
																																break;

																											case  2:	break;
																										}																								
																								}	
																								break;

			case XEVENTTYPE_SCRIPT                  : { SCRIPTXEVENT* event = (SCRIPTXEVENT*)xevent;
																								  if(event) return;

																									switch(xevent->GetEventType())
																										{			
																											case SCRIPTXEVENTTYPE_ERROR		:	XDEBUG_PRINTCOLOR(4,__L("Script ERROR [%d]: %s line %d -> \"%s\""), event->GetError(), event->GetErrorText()->Get(), event->GetNLine(), event->GetCurrentToken()->Get());			
																																											break;

																											case SCRIPTXEVENTTYPE_BREAK		: XDEBUG_PRINTCOLOR(4,__L("Script BREAK: line %d -> \"%s\""), event->GetNLine(), event->GetCurrentToken()->Get());			
																																											break;

																										}

																								}
																							 break;

			case XEVENTTYPE_APPLICATIONUPDATE				: { DIOAPPLICATIONUPDATEXEVENT* event = (DIOAPPLICATIONUPDATEXEVENT*)xevent;
																									if(!event) return;
																									
																									XSTRING string;

																									switch(xevent->GetEventType())
																										{			
																											case DIOAPPLICATIONUPDATEXEVENTTYPE_GETVERSIONAVAILABLE					: string.Format(__L("Get new update version %d.%d.%d"), event->GetVersionData()->GetVersion()
																																																																																						, event->GetVersionData()->GetSubVersion()
																																																																																						, event->GetVersionData()->GetErrorControl());
																																																												break;

																																																			

																											case DIOAPPLICATIONUPDATEXEVENTTYPE_DOWNLOADFILE								: 
																											case DIOAPPLICATIONUPDATEXEVENTTYPE_UNZIPFILE										:
																											case DIOAPPLICATIONUPDATEXEVENTTYPE_BACKUPORIGINALFILE					:
																											case DIOAPPLICATIONUPDATEXEVENTTYPE_COPYUPDATEFILE							:
																											case DIOAPPLICATIONUPDATEXEVENTTYPE_DELETEBACKUPORIGINALFILE		: { DIOAPPLICATIONUPDATEFILE*  updatefiler = event->GetActualFileToUpdate();																																																									
																																																													if(updatefiler) 
																																																														{
																																																															XSTRING operation;
																																																															switch(xevent->GetEventType())
																																																																{
																																																																	case DIOAPPLICATIONUPDATEXEVENTTYPE_DOWNLOADFILE								: operation = __L("Download file");								break;
																																																																	case DIOAPPLICATIONUPDATEXEVENTTYPE_UNZIPFILE										: operation = __L("Unzip file");									break;
																																																																	case DIOAPPLICATIONUPDATEXEVENTTYPE_BACKUPORIGINALFILE					: operation = __L("Backup original file");				break;
																																																																	case DIOAPPLICATIONUPDATEXEVENTTYPE_COPYUPDATEFILE							: operation = __L("Copy update file");						break;
																																																																	case DIOAPPLICATIONUPDATEXEVENTTYPE_DELETEBACKUPORIGINALFILE		: operation = __L("Delete backup original file");	break;
																																																																}

																																																															string.Format(__L("%s %s (%dk)."), operation.Get(), updatefiler->GetXPathFile()->Get(), updatefiler->GetSize()/1024);
																																																														}
																																																												}
																																																												break;

																											
																										}

																									if(string.GetSize()) XLOG::GetInstance().AddEntry(event->GetStatusOperation()?XLOGLEVEL_INFO:XLOGLEVEL_ERROR, __L("Update"), false, string.Get());																									
																								}
																								break;


															default					: break;
		}
	
}







/*-------------------------------------------------------------------
//  TEST::ThreadRunFunction
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2013 11:43:22
//	
//	@return 			void : 
//	@param				thread : 
*/
/*-----------------------------------------------------------------*/
void TEST::ThreadRunFunction(void* data)
{
	TEST* test   = (TEST*)data;
	int   actual = test->threadwebactual;
			 
	test->Test_WEBClient();

	//test->threadwebclient[actual]->Run(false);
}
	


