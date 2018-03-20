/*------------------------------------------------------------------------------------------
//	DIOI2CTEMHUMSENSORAM2315.CPP
//	
//	Data IO I2C Sensor AOSONG AM2315  class (Temperature and humidity module)
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 03/05/2014 16:21:57
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"
	
#include "DIOI2CTemHumSensorAM2315.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	DIOI2CTEMHUMSENSORAM2315::DIOI2CTEMHUMSENSORAM2315
*/	
/**	
//	
//	Class Constructor DIOI2CTEMHUMSENSORAM2315
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:37:54
//	 
*/
/*-----------------------------------------------------------------*/
DIOI2CTEMHUMSENSORAM2315::DIOI2CTEMHUMSENSORAM2315() : DIODEVICEI2C()
{
	Clean();
}




/*-------------------------------------------------------------------
//	DIOI2CTEMHUMSENSORAM2315::~DIOI2CTEMHUMSENSORAM2315
*/	
/**	
//	
//	 Class Destructor DIOI2CTEMHUMSENSORAM2315
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:25:15
//	
*/
/*-----------------------------------------------------------------*/
DIOI2CTEMHUMSENSORAM2315::~DIOI2CTEMHUMSENSORAM2315()
{
	End();
	
	Clean();
}



/*-------------------------------------------------------------------
//	DIOI2CTEMHUMSENSORAM2315::Read
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/05/2014 22:46:28
//	
//	@return 			bool : 
//
//  @param				temperature : 
//  @param				humidity : 
*/
/*-----------------------------------------------------------------*/
bool DIOI2CTEMHUMSENSORAM2315::Read(float& temperature, float& humidity)
{
	if(!diostream)				return false;
	if(!IsInitialized())	return false;

	bool	status		= false;
	XBYTE cmd[3]		= { 0x03, 0x00, 0x04 };
	XBYTE answer[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	status = diostream->Write(cmd, 3)?true:false;
	if(status) status = diostream->WaitToFlushOutXBuffer(timeout);	
	
	if(!status) return false;	

	status = diostream->WaitToFilledReadingBuffer(8, timeout);
	if(status) status = diostream->Read(answer, 8)?true:false;

	/*
	xtimerout->Reset();
	while(1)
		{
			
			if(!status) 
				{
					if(xtimerout->GetMeasureSeconds()>(XDWORD)timeout) break;					

				} else break;

			xsleep->MilliSeconds(50);
		}
	*/

	if(status)
		{
			//XDEBUG_PRINTCOLOR(2, __L("%02X %02X %02X %02X %02X %02X %02X %02X"), answer[0], answer[1], answer[2], answer[3], answer[4], answer[5], answer[6], answer[7]);

			XWORD  CRC			 = (answer[7]<<8) + answer[6];
			XWORD	 CRCbuffer = 0;
			double rhtfactor = 0.1;

			if((answer[1] != cmd[2]) || (answer[0] != cmd[0])) return false;

			CalculeCRC(answer, CRCbuffer);

			if(CRC != CRCbuffer) return false;
				
			humidity		= (float)(rhtfactor * (double)(answer[3] + (answer[2] << 8)));
			temperature = (float)(rhtfactor * (double)(answer[5] + (answer[4] << 8)));					
		}

	return status;
}





/*-------------------------------------------------------------------
//	DIOI2CTEMHUMSENSORAM2315::End
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:29:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CTEMHUMSENSORAM2315::End()
{	
	if(!DIODEVICE::End()) return false;

	if(diostream)			
		{
			diostream->Close();
			diofactory->DeleteStreamIO(diostream);	
			diostream = NULL;
		}

	if(diostreamcfg)	
		{
			delete diostreamcfg;			
			diostreamcfg = NULL;
		}

	if(xtimerout) 
		{
			xfactory->DeleteTimer(xtimerout);
			xtimerout = NULL;
		}

	return true;
}



/*-------------------------------------------------------------------
//	DIOI2CTEMHUMSENSORAM2315::IniDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			09/09/2016 13:18:23
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CTEMHUMSENSORAM2315::IniDevice()
{
	diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO( diostreamcfg);
	if(!diostream) return false;
							
	if(diostream->Open())  
		{
			if(diostream->WaitToConnected(timeout)) 
				{	
					for(int c=0; c<3; c++)
						{
							float temperature;
							float humidity;

							Read(temperature, humidity);
							xsleep->MilliSeconds(300);
						}

					return DIODEVICE::Ini();
				}
		}

	return false;
}




/*-------------------------------------------------------------------
//	DIOI2CTEMHUMSENSORAM2315::CalculeCRC
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/05/2014 23:19:45
//	
//	@return 			bool : 
//
//  @param				buffer : 
//  @param				CRC : 
*/
/*-----------------------------------------------------------------*/
bool DIOI2CTEMHUMSENSORAM2315::CalculeCRC(XBYTE* buffer, XWORD& CRC)
{
	CRC = 0xFFFF;

	for(int l=0; l<6; l++)
		{
			CRC ^= buffer[l];
			
			for(int i=0; i<8; i++)
				{
					if(CRC & 0x01)
						{
							CRC >>= 1;
							CRC ^=  0xA001;
						}
					 else
						{
							CRC >>= 1;
						}
				}
		}

	return true;
}