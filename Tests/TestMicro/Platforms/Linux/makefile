
TARGET   		 = testmicro

ARCHITECTURE = PC
#ARCHITECTURE 	= RASPBERRYPI
DEBUGMODE    	= YES
TOOLCHAIN			= LINARO
SYSROOT				= JESSIE

ADDDIRECTORYGEN = ../GENFrameWork/

FLAGS  			+= -D LINUX
FLAGS  			+= -D UNICODE
FLAGS				+= -D DIO_ACTIVE
FLAGS				+= -D DIOUART_ACTIVE
FLAGS				+= -D DIOUSB_ACTIVE
FLAGS				+= -D DIOI2C_ACTIVE
FLAGS				+= -D DIOSPI_ACTIVE
FLAGS				+= -D DIOPORTDATA_ACTIVE

FLAGSC  		+= 
FLAGSCPP		+= 

#---- INCLUDES --------------------------------------------------------------------------------------------------

CPPDIR = ../../Application 										 								
CDIR	 = 


#---- SOURCES CPP ------------------------------------------------------------------------------------------------

SOURCESCPP  += TestMicro.cpp

SOURCESCPP	+= CompressBase.cpp
SOURCESCPP	+= CompressLZRW1KH.cpp
SOURCESCPP	+= CompressLZW.cpp
SOURCESCPP	+= CompressManager.cpp
SOURCESCPP	+= CompressZIP.cpp
SOURCESCPP  += CompressGZ.cpp

SOURCESCPP  += Cipher.cpp
SOURCESCPP  += CipherAES.cpp
SOURCESCPP  += Hash.cpp
SOURCESCPP  += HashCRC32.cpp
SOURCESCPP  += HashMD5.cpp
SOURCESCPP  += HashSHA1.cpp

SOURCESCPP  += DIOFactory.cpp
SOURCESCPP	+= DIOStream.cpp
SOURCESCPP	+= DIOStreamConfig.cpp
SOURCESCPP	+= DIOStreamDevice.cpp
SOURCESCPP	+= DIOStreamDeviceUSB.cpp
SOURCESCPP	+= DIOStreamEnumDevices.cpp
SOURCESCPP	+= DIOStreamEnumServers.cpp
SOURCESCPP	+= DIOStreamI2C.cpp
SOURCESCPP	+= DIOStreamI2CConfig.cpp
SOURCESCPP	+= DIOStreamSPI.cpp
SOURCESCPP	+= DIOStreamSPIConfig.cpp
SOURCESCPP	+= DIOStreamUART.cpp
SOURCESCPP	+= DIOStreamUARTConfig.cpp
SOURCESCPP	+= DIOStreamUARTLocalEnumDevices.cpp
SOURCESCPP	+= DIOStreamUSB.cpp
SOURCESCPP	+= DIOStreamUSBConfig.cpp
SOURCESCPP	+= DIOStreamUSBLocalEnumDevices.cpp
SOURCESCPP	+= DIOI2CSensorLightTSL2561.cpp
SOURCESCPP	+= DIOI2C3DCompassAccelerometerLSM303DLHC.cpp
SOURCESCPP	+= DIOURL.cpp
SOURCESCPP	+= DIOIP.cpp

SOURCESCPP  += Main.cpp

SOURCESCPP  += DIOLINUXFactory.cpp
SOURCESCPP  += DIOLINUXStreamI2C.cpp
SOURCESCPP  += DIOLINUXStreamSPI.cpp
SOURCESCPP  += DIOLINUXPortData.cpp
SOURCESCPP  += DIOLINUXStreamUART.cpp
SOURCESCPP  += DIOLINUXStreamUARTLocalEnumDevices.cpp
SOURCESCPP  += DIOLINUXStreamUSB.cpp
SOURCESCPP  += DIOLINUXStreamUSBLocalEnumDevices.cpp
SOURCESCPP  += DIOLINUXURL.cpp


SOURCESCPP  += LINUXMain.cpp
SOURCESCPP  += XLINUXSleep.cpp
SOURCESCPP  += XLINUXDebug.cpp
SOURCESCPP  += XLINUXFactory.cpp
SOURCESCPP  += XLINUXRand.cpp
SOURCESCPP  += XLINUXSystem.cpp
SOURCESCPP  += XLINUXThread.cpp
SOURCESCPP  += XLINUXTimer.cpp
SOURCESCPP  += XLINUXDateTime.cpp
SOURCESCPP  += XLINUXFile.cpp
SOURCESCPP  += XLINUXDir.cpp
SOURCESCPP  += XLINUXConsole.cpp

SOURCESCPP  += XFactory.cpp
SOURCESCPP  += XSleep.cpp
SOURCESCPP  += XApplication.cpp
SOURCESCPP  += XBase.cpp
SOURCESCPP  += XBuffer.cpp
SOURCESCPP  += XDebug.cpp
SOURCESCPP  += XFSMachine.cpp
SOURCESCPP  += XPublisher.cpp
SOURCESCPP  += XString.cpp
SOURCESCPP  += XSystem.cpp
SOURCESCPP  += XThread.cpp
SOURCESCPP  += XTimer.cpp
SOURCESCPP  += XDateTime.cpp
SOURCESCPP  += XVariant.cpp
SOURCESCPP  += XConsole.cpp
SOURCESCPP  += XPath.cpp
SOURCESCPP  += XPathsManager.cpp
SOURCESCPP  += XLanguage.cpp
SOURCESCPP  += XLog.cpp
SOURCESCPP  += XDir.cpp
SOURCESCPP  += XFile.cpp
SOURCESCPP  += XFileTXT.cpp
SOURCESCPP  += XFileXML.cpp
SOURCESCPP  += XFileZIP.cpp



#---- SOURCES C --------------------------------------------------------------------------------------------------
       
SOURCESC 		+= adler32.c
SOURCESC		+= compress.c
SOURCESC 		+= crc32.c
SOURCESC 		+= deflate.c
SOURCESC 		+= gzio.c
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
       
       
       
       
       
include ../../../$(ADDDIRECTORYGEN)GEN/Platforms/Linux/makefile        
     
   
