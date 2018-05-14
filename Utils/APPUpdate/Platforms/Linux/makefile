
TARGET   		 = appupdate

ARCHITECTURE = PC
#ARCHITECTURE = RASPBERRYPI
DEBUGMODE    = YES
TOOLCHAIN    = LINARO
SYSROOT			 = JESSIE

ADDDIRECTORYGEN = ../../GENFrameWork/

FLAGS  			+= -D UNICODE
FLAGS  			+= -D LINUX
FLAGS				+= -D DIO_ACTIVE
FLAGS				+= -D DIOICMP_ACTIVE
FLAGS				+= -D DIOTCPIP_ACTIVE
FLAGS				+= -D DIOUDP_ACTIVE
FLAGS				+= -D DIONTP_ACTIVE
FLAGS				+= -D DIOPING_ACTIVE
FLAGS				+= -D DIOALERTS_ACTIVE


FLAGSC  		+= 
FLAGSCPP		+= 

#---- INCLUDES --------------------------------------------------------------------------------------------------

CPPDIR = ../../Application 										 								
CDIR	 = 


#---- SOURCES CPP ------------------------------------------------------------------------------------------------

SOURCESCPP   = APPUpdate.cpp

SOURCESCPP  += XBase.cpp
SOURCESCPP  += XFactory.cpp
SOURCESCPP  += XDateTime.cpp
SOURCESCPP  += XTimer.cpp
SOURCESCPP  += XApplication.cpp
SOURCESCPP  += XFileConfig.cpp
SOURCESCPP  += XPathsManager.cpp
SOURCESCPP  += XPath.cpp
SOURCESCPP  += XDebug.cpp
SOURCESCPP  += XMemory.cpp
SOURCESCPP  += XFSMachine.cpp
SOURCESCPP  += XBuffer.cpp
SOURCESCPP  += XFile.cpp
SOURCESCPP  += XDir.cpp
SOURCESCPP  += XLanguage.cpp
SOURCESCPP  += XThread.cpp
SOURCESCPP  += XScheduler.cpp
SOURCESCPP  += XFileINI.cpp
SOURCESCPP  += XFileCSV.cpp
SOURCESCPP  += XFileTXT.cpp
SOURCESCPP  += XFileXML.cpp
SOURCESCPP  += XFileZIP.cpp
SOURCESCPP  += XFileJSON.cpp
SOURCESCPP  += XFileDBF.cpp
SOURCESCPP  += XSystem.cpp
SOURCESCPP  += XConsole.cpp
SOURCESCPP  += XPublisher.cpp
SOURCESCPP  += XLog.cpp
SOURCESCPP  += XString.cpp
SOURCESCPP  += XSerialize.cpp
SOURCESCPP  += XSleep.cpp
SOURCESCPP  += XVariant.cpp

SOURCESCPP  += Main.cpp

SOURCESCPP  += DIOFactory.cpp
SOURCESCPP  += DIOMAC.cpp
SOURCESCPP  += DIOURL.cpp
SOURCESCPP  += DIOIP.cpp
SOURCESCPP  += DIOCheckInternetConnexion.cpp
SOURCESCPP  += DIOCheckConnexions.cpp
SOURCESCPP  += DIOStreamDevice.cpp
SOURCESCPP  += DIOStreamDeviceIP.cpp
SOURCESCPP  += DIOStreamConfig.cpp
SOURCESCPP  += DIOStream.cpp
SOURCESCPP  += DIOStreamICMPConfig.cpp
SOURCESCPP  += DIOStreamICMP.cpp
SOURCESCPP  += DIOStreamUDPConfig.cpp
SOURCESCPP  += DIOStreamUDP.cpp
SOURCESCPP  += DIOStreamUDPAcknowledge.cpp
SOURCESCPP  += DIOStreamTCPIPConfig.cpp
SOURCESCPP  += DIOStreamTCPIP.cpp
SOURCESCPP  += DIOStreamEnumDevices.cpp
SOURCESCPP  += DIOStreamEnumServers.cpp
SOURCESCPP  += DIOStreamIPLocalEnumDevices.cpp
SOURCESCPP  += DIOPing.cpp
SOURCESCPP  += DIOWebHeader.cpp
SOURCESCPP  += DIOWebServer.cpp
SOURCESCPP  += DIOWebServerQueryString.cpp
SOURCESCPP  += DIOWebPageHTMLCreator.cpp
SOURCESCPP  += DIOWebClient.cpp
SOURCESCPP  += DIOWebScraper.cpp
SOURCESCPP  += DIOWebScraperCache.cpp
SOURCESCPP  += DIOWebScraperGeolocationIP.cpp
SOURCESCPP  += DIOWebScraperPublicIP.cpp
SOURCESCPP  += DIOWebScraperUserAgentID.cpp
SOURCESCPP  += DIOWebScraperWeather.cpp
SOURCESCPP  += DIONTP.cpp
SOURCESCPP  += DIOEmailAddress.cpp
SOURCESCPP  += DIOSMTP.cpp
SOURCESCPP  += DIOAlerts.cpp
SOURCESCPP  += DIODynDNS.cpp
SOURCESCPP  += DIOATCMDS.cpp
SOURCESCPP  += DIOATCMDGSM.cpp

SOURCESCPP  += Cipher.cpp
SOURCESCPP  += Hash.cpp
SOURCESCPP  += HashCRC32.cpp
SOURCESCPP  += HashMD5.cpp

SOURCESCPP	+= CompressManager.cpp
SOURCESCPP	+= CompressBase.cpp
SOURCESCPP	+= CompressLZRW1KH.cpp
SOURCESCPP	+= CompressLZW.cpp
SOURCESCPP	+= CompressZIP.cpp
SOURCESCPP	+= CompressGZ.cpp

SOURCESCPP  += LINUXMain.cpp
SOURCESCPP  += DIOLINUXFactory.cpp
SOURCESCPP  += DIOLINUXURL.cpp
SOURCESCPP  += DIOLINUXStreamICMP.cpp
SOURCESCPP  += DIOLINUXStreamUDP.cpp
SOURCESCPP  += DIOLINUXStreamTCPIP.cpp
SOURCESCPP  += DIOLINUXStreamIPLocalEnumDevices.cpp
SOURCESCPP  += DIOLINUXPing.cpp
SOURCESCPP  += XLINUXFactory.cpp
SOURCESCPP  += XLINUXDebug.cpp
SOURCESCPP  += XLINUXFile.cpp
SOURCESCPP  += XLINUXDir.cpp
SOURCESCPP  += XLINUXRand.cpp
SOURCESCPP  += XLINUXSystem.cpp
SOURCESCPP  += XLINUXConsole.cpp
SOURCESCPP  += XLINUXDateTime.cpp
SOURCESCPP  += XLINUXSleep.cpp
SOURCESCPP  += XLINUXTimer.cpp
SOURCESCPP  += XLINUXThread.cpp

#---- SOURCES C --------------------------------------------------------------------------------------------------

SOURCESC 		+= adler32.c
SOURCESC		+= compress.c
SOURCESC 		+= crc32.c
SOURCESC 		+= deflate.c
SOURCESC 		+= infback.c
SOURCESC 		+= inffast.c
SOURCESC 		+= inflate.c
SOURCESC 		+= inftrees.c
SOURCESC 		+= ioapi.c
SOURCESC 		+= trees.c
SOURCESC 		+= uncompr.c
SOURCESC 		+= unzip.c
SOURCESC 		+= zip.c
SOURCESC 		+= zutil.c

     
#---- LIBS -------------------------------------------------------------------------------------------------------

          
include ../../../../GEN/Platforms/Linux/makefile      
     
   