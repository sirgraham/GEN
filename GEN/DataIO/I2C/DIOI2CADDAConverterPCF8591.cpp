/*------------------------------------------------------------------------------------------
//	DIOI2CADDACONVERTERPCF8591.CPP
//	
//	Data IO I2C PCF8591 (AD/DA converter) class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 03/05/2014 16:21:57
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"
	
#include "DIOI2CADDAConverterPCF8591.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	DIOI2CADDACONVERTERPCF8591::DIOI2CADDACONVERTERPCF8591
*/	
/**	
//	
//	Class Constructor DIOI2CADDACONVERTERPCF8591
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:37:54
//	
*/
/*-----------------------------------------------------------------*/
DIOI2CADDACONVERTERPCF8591::DIOI2CADDACONVERTERPCF8591() : DIODEVICEI2C()
{
	Clean();
}




/*-------------------------------------------------------------------
//	DIOI2CADDACONVERTERPCF8591::~DIOI2CADDACONVERTERPCF8591
*/	
/**	
//	
//	 Class Destructor DIOI2CADDACONVERTERPCF8591
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:25:15
//	
*/
/*-----------------------------------------------------------------*/
DIOI2CADDACONVERTERPCF8591::~DIOI2CADDACONVERTERPCF8591()
{
	End();

	Clean();
}




/*-------------------------------------------------------------------
//	DIOI2CADDACONVERTERPCF8591::ReadADChannels
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			25/05/2014 19:29:03
//	
//	@return 			bool : 
//
//  @param				channel : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool DIOI2CADDACONVERTERPCF8591::ReadADChannels(XBYTE values[4])
{
	XBYTE   control = 0x04;
	bool		status	= false;

	if(!IsInitialized()) return false;
	
	diostream->ResetXBuffers(); 
	
	if(diostream->Write(&control, 1)) status = diostream->WaitToFlushOutXBuffer(timeout);		
	
	if(status)
		{
			status = diostream->WaitToFilledReadingBuffer(5, timeout);
			if(status) 
				{	
					XBYTE dataread[5] = { 0, 0, 0, 0, 0 };

					if(diostream->Read(dataread, 5) == 5)
						{
							values[0] = dataread[2];
							values[1] = dataread[3];
							values[2] = dataread[4];
							values[3] = dataread[1];							

							status = true;

						} else status = false;
				}
		}

	return status;
}



/*-------------------------------------------------------------------
//	DIOI2CADDACONVERTERPCF8591::WriteDAChannel
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			25/05/2014 19:29:09
//	
//	@return 			bool : 
//
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool DIOI2CADDACONVERTERPCF8591::WriteDAChannel(XBYTE value)
{
	if(!IsInitialized()) return false;

	XBYTE   data[2] = { 0, 0 };
	bool		status	= false;

	data[0] = 0x40;
	data[1] = value;
	
	diostream->ResetXBuffers();

	if(diostream->Write(data, 2)) status = diostream->WaitToFlushOutXBuffer(timeout);			

	return status;
}



/*-------------------------------------------------------------------
//	DIOI2CADDACONVERTERPCF8591::End
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
bool DIOI2CADDACONVERTERPCF8591::End()
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
//	DIOI2CADDACONVERTERPCF8591::IniDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			09/09/2016 14:12:08
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CADDACONVERTERPCF8591::IniDevice()
{	
	diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO( diostreamcfg);
	if(!diostream)  return false;
	
	if(!diostream->Open())  return false;

	if(!diostream->WaitToConnected(timeout)) return false;

	return DIODEVICE::Ini();	
}
