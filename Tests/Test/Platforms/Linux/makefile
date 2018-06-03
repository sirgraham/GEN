
TARGET   		 	= test

ARCHITECTURE 	= PC
#ARCHITECTURE = RASPBERRYPI
DEBUGMODE    	= YES
TOOLCHAIN     = RASPBIAN
#TOOLCHAIN		= LINARO
SYSROOT				= JESSIE

ADDDIRECTORYGEN = ../../GENFrameWork/

FLAGS  			+= -D LINUX
FLAGS  			+= -D UNICODE
#FLAGS  		+= -D XMEM
#FLAGS			+= -D SND_ACTIVE
FLAGS				+= -D DIO_ACTIVE
FLAGS				+= -D DIOUART_ACTIVE
#FLAGS			+= -D DIOUSB_ACTIVE
 FLAGS			+= -D DIOICMP_ACTIVE
FLAGS				+= -D DIOUDP_ACTIVE
FLAGS				+= -D DIOTCPIP_ACTIVE
FLAGS		    += -D DIOTCPIPLOCALENUMSERVERS_ACTIVE
#FLAGS			+= -D DIOBLUETOOTH_ACTIVE
#FLAGS			+= -D DIOWIFI_ACTIVE
#FLAGS			+= -D DIOSPI_ACTIVE
#FLAGS			+= -D DIOI2C_ACTIVE
#FLAGS			+= -D DIONTP_ACTIVE
 FLAGS			+= -D DIOPING_ACTIVE
#FLAGS			+= -D DIOGPIO_ACTIVE
#FLAGS		  += -D DIOALERTS_ACTIVE
#FLAGS		 	+= -D DBSQL_ACTIVE
#FLAGS		 	+= -D DBSQLMYSQL_ACTIVE	
#FLAGS		  += -D DBSQLPOSTGRESQL_ACTIVE
#FLAGS		  += -D DBSQLSQLITE_ACTIVE			
#FLAGS    	+= -D DIOPCAP_ACTIVE

FLAGSC  		+= 
FLAGSCPP	    += 

#---- INCLUDES --------------------------------------------------------------------------------------------------

CPPDIR = ../../Application 										 								
CDIR	 = 


#---- SOURCES CPP ------------------------------------------------------------------------------------------------

SOURCESCPP  += Test.cpp
SOURCESCPP  += TestProtocolConnexionsManager.cpp
SOURCESCPP  += TestProtocol.cpp

SOURCESCPP	+= Cipher.cpp
SOURCESCPP	+= CipherFileKeys.cpp
SOURCESCPP	+= CipherDES.cpp
SOURCESCPP	+= Cipher3DES.cpp
SOURCESCPP	+= CipherBlowfish.cpp
SOURCESCPP	+= CipherAES.cpp
SOURCESCPP	+= CipherRSA.cpp
SOURCESCPP	+= Hash.cpp
SOURCESCPP	+= HashCOMP128v1.cpp
SOURCESCPP	+= HashCRC32.cpp
SOURCESCPP	+= HashMD5.cpp
SOURCESCPP	+= HashSHA1.cpp
SOURCESCPP	+= HashSHA2.cpp
SOURCESCPP	+= HashWhirlpool.cpp

SOURCESCPP	+= CompressBase.cpp
SOURCESCPP	+= CompressLZRW1KH.cpp
SOURCESCPP	+= CompressLZW.cpp
SOURCESCPP	+= CompressManager.cpp
SOURCESCPP	+= CompressGZ.cpp
SOURCESCPP	+= CompressZIP.cpp

SOURCESCPP  += DIOFactory.cpp
SOURCESCPP	+= DIOAlerts.cpp
SOURCESCPP	+= DIOATCMDGSM.cpp
SOURCESCPP	+= DIOATCMDS.cpp
SOURCESCPP  += DIOCheckConnexions.cpp
SOURCESCPP  += DIOCheckInternetConnexion.cpp
SOURCESCPP	+= DIOCLIClient.cpp
SOURCESCPP	+= DIOCLIClientBTModuleCSR.cpp
SOURCESCPP	+= DIOIEC60870_5.cpp
SOURCESCPP	+= DIOTemHumSensorAM2301.cpp
SOURCESCPP	+= DIOLightSensorLDRAnalog.cpp
SOURCESCPP	+= DIOIP.cpp
SOURCESCPP	+= DIOMAC.cpp
SOURCESCPP	+= DIOMACManufactured.cpp
SOURCESCPP	+= DIOModBus.cpp
SOURCESCPP	+= DIONTP.cpp
SOURCESCPP	+= DIODynDNS.cpp
SOURCESCPP	+= DIOOBEXPush.cpp
SOURCESCPP	+= DIOPCap.cpp
SOURCESCPP	+= DIOPCapEX.cpp
SOURCESCPP	+= DIOPing.cpp
SOURCESCPP	+= DIOProtocol.cpp
SOURCESCPP	+= DIOProtocolConnexionsManager.cpp
SOURCESCPP	+= DIOSNMP.cpp
SOURCESCPP	+= DIOStream.cpp
SOURCESCPP	+= DIOStreamCipher.cpp
SOURCESCPP	+= DIOStreamBluetooth.cpp
SOURCESCPP	+= DIOStreamBluetoothConfig.cpp
SOURCESCPP	+= DIOStreamBluetoothLocalEnumDevices.cpp
SOURCESCPP	+= DIOStreamBluetoothRemoteEnumDevices.cpp
SOURCESCPP	+= DIOStreamWifiRemoteEnumDevices.cpp
SOURCESCPP	+= DIOStreamConfig.cpp
SOURCESCPP	+= DIOStreamDevice.cpp
SOURCESCPP	+= DIOStreamDeviceUSB.cpp
SOURCESCPP	+= DIOStreamDeviceBluetooth.cpp
SOURCESCPP	+= DIOStreamDeviceWifi.cpp
SOURCESCPP	+= DIOStreamDeviceIP.cpp
SOURCESCPP	+= DIOStreamEnumDevices.cpp
SOURCESCPP	+= DIOStreamEnumServers.cpp
SOURCESCPP	+= DIOStreamIPLocalEnumDevices.cpp
SOURCESCPP	+= DIOStreamSPI.cpp
SOURCESCPP	+= DIOStreamSPIConfig.cpp
SOURCESCPP	+= DIOStreamI2C.cpp
SOURCESCPP	+= DIOStreamI2CConfig.cpp
SOURCESCPP	+= DIOStreamICMP.cpp
SOURCESCPP	+= DIOStreamICMPConfig.cpp
SOURCESCPP	+= DIOStreamUDP.cpp
SOURCESCPP	+= DIOStreamUDPConfig.cpp
SOURCESCPP	+= DIOStreamTCPIP.cpp
SOURCESCPP	+= DIOStreamTCPIPConfig.cpp
SOURCESCPP	+= DIOStreamUART.cpp
SOURCESCPP	+= DIOStreamUARTConfig.cpp
SOURCESCPP	+= DIOStreamUARTLocalEnumDevices.cpp
SOURCESCPP	+= DIOStreamUSB.cpp
SOURCESCPP	+= DIOStreamUSBConfig.cpp
SOURCESCPP	+= DIOStreamUSBLocalEnumDevices.cpp
SOURCESCPP	+= DIOStreamUDPLocalEnumServers.cpp
SOURCESCPP	+= DIOStreamUDPAcknowledge.cpp
SOURCESCPP  += DIOStringProtocol.cpp
SOURCESCPP	+= DIOPiFace.cpp
SOURCESCPP	+= DIOSPIGPIOMCP23S17.cpp
SOURCESCPP	+= DIOSPILCDScreenPCF8833.cpp
SOURCESCPP	+= DIOSPILCDScreenILI9341.cpp
SOURCESCPP	+= DIOSPITouchScreenSTMPE610.cpp
SOURCESCPP	+= DIOSPIOLEDScreenSSD1331.cpp
SOURCESCPP	+= DIOI2CEEprom24XXX.cpp
SOURCESCPP	+= DIOI2CADDAConverterPCF8591.cpp
SOURCESCPP	+= DIOI2CTemHumSensorAM2315.cpp
SOURCESCPP	+= DIOI2CGPIOMCP2317.cpp
SOURCESCPP	+= DIOI2CGPIOPCF8574.cpp
SOURCESCPP	+= DIOI2CPWMControlerPCA9685.cpp
SOURCESCPP	+= DIOI2C6AxisTrackingLSM303DLHC.cpp
SOURCESCPP	+= DIOI2COLEDScreenSSD1306.cpp
SOURCESCPP	+= DIOI2CLightSensorTSL2561.cpp
SOURCESCPP	+= DIOURL.cpp
SOURCESCPP	+= DIOWebClient.cpp
SOURCESCPP	+= DIOWebHeader.cpp
SOURCESCPP	+= DIOWebScraper.cpp
SOURCESCPP	+= DIOWebScraperCache.cpp
SOURCESCPP	+= DIOWebScraperGeolocationIP.cpp
SOURCESCPP	+= DIOWebScraperMACManufacturer.cpp
SOURCESCPP	+= DIOWebScraperPublicIP.cpp
SOURCESCPP	+= DIOWebScraperSexName.cpp
SOURCESCPP	+= DIOWebScraperTranslation.cpp
SOURCESCPP	+= DIOWebScraperUserAgentID.cpp
SOURCESCPP	+= DIOWebScraperWeather.cpp
SOURCESCPP	+= DIOWebServer.cpp
SOURCESCPP	+= DIOWebServerQueryString.cpp
SOURCESCPP	+= DIOEmailAddress.cpp
SOURCESCPP	+= DIOSMTP.cpp
SOURCESCPP  += DIOBusPirate.cpp
SOURCESCPP  += DIOWiiMote.cpp
SOURCESCPP  += DIOApplicationUpdate.cpp
SOURCESCPP  += DIOSSHReverse.cpp

SOURCESCPP	+= INPButton.cpp
SOURCESCPP	+= INPCursor.cpp
SOURCESCPP	+= INPDevice.cpp
SOURCESCPP	+= INPManager.cpp
SOURCESCPP	+= INPPath.cpp
SOURCESCPP	+= INPFactory.cpp

SOURCESCPP  += Main.cpp

SOURCESCPP  += DIOLINUXFactory.cpp
SOURCESCPP  += DIOLINUXPCap.cpp
SOURCESCPP  += DIOLINUXPing.cpp
ifneq (,$(findstring DIOGPIO_ACTIVE,$(FLAGS)))
	ifeq "$(ARCHITECTURE)" "PC"  
		SOURCESCPP  += DIOLINUXGPIOPC.cpp
	endif
	ifeq "$(ARCHITECTURE)" "GUMSTIXCONNEX"  
		SOURCESCPP  += DIOLINUXGPIOGumstixConnex.cpp	
	endif
	ifeq "$(ARCHITECTURE)" "LEX3V700A"  
		SOURCESCPP  += DIOLINUXGPIOLex3V700A.cpp	
	endif
	ifeq "$(ARCHITECTURE)" "ARTILA500"  
		SOURCESCPP  += DIOLINUXGPIOArtila500.cpp	
	endif
	ifeq "$(ARCHITECTURE)" "RASPBERRYPI"  
		SOURCESCPP  += DIOLINUXGPIORaspberryPi.cpp	
	endif
endif
SOURCESCPP  += DIOLINUXStreamBluetooth.cpp
SOURCESCPP  += DIOLINUXStreamBluetoothLocalEnumDevices.cpp
SOURCESCPP  += DIOLINUXStreamBluetoothRemoteEnumDevices.cpp
SOURCESCPP  += DIOLINUXStreamWifiRemoteEnumDevices.cpp
SOURCESCPP  += DIOLINUXStreamIPLocalEnumDevices.cpp
SOURCESCPP  += DIOLINUXStreamSPI.cpp
SOURCESCPP  += DIOLINUXStreamI2C.cpp
SOURCESCPP  += DIOLINUXStreamUART.cpp
SOURCESCPP  += DIOLINUXStreamUARTLocalEnumDevices.cpp
SOURCESCPP  += DIOLINUXStreamUSB.cpp
SOURCESCPP  += DIOLINUXStreamUSBLocalEnumDevices.cpp
SOURCESCPP  += DIOLINUXStreamICMP.cpp
SOURCESCPP  += DIOLINUXStreamUDP.cpp
SOURCESCPP  += DIOLINUXStreamTCPIP.cpp
SOURCESCPP  += DIOLINUXURL.cpp
SOURCESCPP  += INPLINUXFactory.cpp
SOURCESCPP  += INPLINUXDeviceKeyboardDirect.cpp
SOURCESCPP  += INPLINUXDeviceMouseDirect.cpp
SOURCESCPP  += LINUXMain.cpp
SOURCESCPP  += XLINUXConsole.cpp
SOURCESCPP  += XLINUXSleep.cpp
SOURCESCPP  += XLINUXDebugTrace.cpp
SOURCESCPP  += XLINUXDir.cpp
SOURCESCPP  += XLINUXFactory.cpp
SOURCESCPP  += XLINUXFile.cpp
SOURCESCPP  += XLINUXRand.cpp
SOURCESCPP  += XLINUXSystem.cpp
SOURCESCPP  += XLINUXThread.cpp
SOURCESCPP  += XLINUXTimer.cpp
SOURCESCPP  += XLINUXDateTime.cpp
SOURCESCPP  += XLINUXSharedMemory.cpp

SOURCESCPP  += Script.cpp
SOURCESCPP  += ScriptLib.cpp
SOURCESCPP  += ScriptLibBase.cpp
SOURCESCPP  += ScriptG.cpp
#SOURCESCPP  += ScriptLua.cpp


ifneq (,$(findstring SND_ACTIVE,$(FLAGS)))
	SOURCESCPP += SNDFile.cpp
	SOURCESCPP += SNDFileFFMPEG.cpp
	SOURCESCPP += SNDOpenALBuffer.cpp
	SOURCESCPP += SNDOpenALSource.cpp
	SOURCESCPP += SNDOpenALElement.cpp
	SOURCESCPP += SNDOpenALStreamElement.cpp
	SOURCESCPP += SNDOpenAL.cpp
	SOURCESCPP += SNDEvent.cpp
	SOURCESCPP += SNDElement.cpp	
	SOURCESCPP += SNDLINUXFactory.cpp
	SOURCESCPP += SNDFactory.cpp
	SOURCESCPP += SNDSource.cpp
	SOURCESCPP += SNDStreamInstance.cpp
	SOURCESCPP += SNDLINUXResampler.cpp
	SOURCESCPP += SNDCodecFFMPEG.cpp
	SOURCESCPP += MEDIAFrameReaderLibAV.cpp
	SOURCESCPP += MEDIAFile.cpp                
endif


SOURCESCPP  += DBSQLFactory.cpp
SOURCESCPP  += DBSQLDatabase.cpp
SOURCESCPP  += DBSQLConnection.cpp
SOURCESCPP  += DBSQLQuery.cpp
SOURCESCPP  += DBSQLVariant.cpp
SOURCESCPP  += SQLMYSQLConnection.cpp
SOURCESCPP  += SQLMYSQLDatabase.cpp
SOURCESCPP  += SQLMYSQLQuery.cpp
SOURCESCPP  += SQLMYSQLResult.cpp
SOURCESCPP  += SQLMYSQLVariant.cpp
SOURCESCPP  += SQLPostgreSQLConnection.cpp
SOURCESCPP  += SQLPostgreSQLDatabase.cpp
SOURCESCPP  += SQLPostgreSQLQuery.cpp
SOURCESCPP  += SQLPostgreSQLResult.cpp
SOURCESCPP  += SQLSQLITEConnection.cpp
SOURCESCPP  += SQLSQLITEDatabase.cpp
SOURCESCPP  += SQLSQLITEQuery.cpp
SOURCESCPP  += SQLSQLITEResult.cpp


SOURCESCPP  += XFactory.cpp
SOURCESCPP  += XSleep.cpp
SOURCESCPP  += XApplication.cpp
SOURCESCPP  += XFileConfig.cpp
SOURCESCPP  += XBase.cpp
SOURCESCPP  += XBER.cpp
SOURCESCPP  += XBuffer.cpp
SOURCESCPP  += XConsole.cpp
SOURCESCPP  += XDebugTrace.cpp
SOURCESCPP  += XDir.cpp
SOURCESCPP  += XErrorMsg.cpp
SOURCESCPP  += XFile.cpp
SOURCESCPP  += XFileCSV.cpp
SOURCESCPP  += XFileDBF.cpp
SOURCESCPP  += XFileHash.cpp
SOURCESCPP  += XFileHEX.cpp
SOURCESCPP  += XFileINI.cpp
SOURCESCPP  += XFileJSON.cpp
SOURCESCPP  += XFileSentences.cpp
SOURCESCPP  += XFileTXT.cpp
SOURCESCPP  += XFileXDB.cpp
SOURCESCPP  += XFileXML.cpp
SOURCESCPP  += XFileZIP.cpp
SOURCESCPP  += XFSMachine.cpp
SOURCESCPP  += XLanguage.cpp
SOURCESCPP  += XLog.cpp
SOURCESCPP  += XMemory.cpp
SOURCESCPP  += XPath.cpp
SOURCESCPP  += XPathsManager.cpp
SOURCESCPP  += XPhone.cpp
SOURCESCPP  += XPublisher.cpp
SOURCESCPP  += XScheduler.cpp
SOURCESCPP  += XSerialize.cpp
SOURCESCPP  += XString.cpp
SOURCESCPP  += XSystem.cpp
SOURCESCPP  += XThread.cpp
SOURCESCPP  += XLicense.cpp
SOURCESCPP  += XTimer.cpp
SOURCESCPP  += XDateTime.cpp
SOURCESCPP  += XUUID.cpp
SOURCESCPP  += XMPInteger.cpp
SOURCESCPP  += XVariant.cpp

#---- SOURCES C --------------------------------------------------------------------------------------------------

SOURCESC 	+= adler32.c
SOURCESC	+= compress.c
SOURCESC 	+= crc32.c
SOURCESC 	+= deflate.c
SOURCESC 	+= infback.c
SOURCESC 	+= inffast.c
SOURCESC 	+= inflate.c
SOURCESC 	+= inftrees.c
SOURCESC 	+= ioapi.c
SOURCESC 	+= trees.c
SOURCESC 	+= uncompr.c
SOURCESC 	+= unzip.c
SOURCESC 	+= zip.c
SOURCESC 	+= zutil.c

ifneq (,$(findstring DBSQL_ACTIVE,$(FLAGS)))			
	ifneq (,$(findstring DBSQLSQLITE_ACTIVE,$(FLAGS)))				
		SOURCESC	  += sqlite3.c
	endif
endif

ifneq (,$(findstring SND_ACTIVE,$(FLAGS)))         
	SOURCESC += stb_vorbis.c         
endif         
         
             
include ../../../../GEN/Platforms/Linux/makefile      
     
   
