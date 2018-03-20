/*------------------------------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685.CPP
//	
//	Data IO I2C PCA9685 (16-channel, 12-bit PWM controller) class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 03/05/2014 16:21:57
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>  

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"
	
#include "DIOI2CPWMControlerPCA9685.h"


#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::DIOI2CPWMCONTROLERPCA9685
*/	
/**	
//	
//	Class Constructor DIOI2CPWMCONTROLERPCA9685
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:37:54
//	
*/
/*-----------------------------------------------------------------*/
DIOI2CPWMCONTROLERPCA9685::DIOI2CPWMCONTROLERPCA9685() : DIODEVICEI2C()
{
	Clean();

	 
	;
}




/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::~DIOI2CPWMCONTROLERPCA9685
*/	
/**	
//	
//	 Class Destructor DIOI2CPWMCONTROLERPCA9685
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:25:15
//	
*/
/*-----------------------------------------------------------------*/
DIOI2CPWMCONTROLERPCA9685::~DIOI2CPWMCONTROLERPCA9685()
{
	End();

	Clean();
}




/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::Reset
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/06/2014 17:56:26
//	
*/
/*-----------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::Reset() 
{
	return SendCommand(DIOI2CPWMCONTROLERPCA9685_MODE1, 0x00);
}



/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::SetPWMFrequency
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/06/2014 19:03:20
//	
//	@return 			bool : 
//
//  @param				frequency : 
*/
/*-----------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::SetPWMFrequency(float frequency) 
{   
	if(!IsInitialized()) return false;

  float prescaleval;
 
	prescaleval	 = 25000000;				//  25MHz
	prescaleval	/= 4096;						//  12 bits		
  prescaleval	/= frequency;
  prescaleval	-= 1;
  
	XBYTE prescale		 = (XBYTE)floor(prescaleval + 0.5f);
	XBYTE oldmode;	
	bool  status;

	//XDEBUG_PRINTCOLOR(1, __L("Estimated pre-scale: %f"), prescaleval);  
  //XDEBUG_PRINTCOLOR(1, __L("Final pre-scale    : %d"), prescale);  
 	
  status = ReadCommand(DIOI2CPWMCONTROLERPCA9685_MODE1, oldmode);
	if(!status) return false;
	
  XBYTE newmode = (oldmode&0x7F) | 0x10; // sleep

	status = SendCommand(DIOI2CPWMCONTROLERPCA9685_MODE1			, newmode);				// go to sleep
	if(!status) return false;

  status = SendCommand(DIOI2CPWMCONTROLERPCA9685_PRESCALE	, prescale);			// set the prescaler
	if(!status) return false;

  status = SendCommand(DIOI2CPWMCONTROLERPCA9685_MODE1			, oldmode);
	if(!status) return false;
  
	xtimerout->SetMilliSeconds(50);
	
  return SendCommand(DIOI2CPWMCONTROLERPCA9685_MODE1, (oldmode | 0x80));		
}




/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::SetPWM
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			17/02/2016 16:42:12
//	
//	@return 			bool : 
//
//  @param				channel : 
//  @param				on : 
//  @param				off : 
//  @param				wait : 
*/
/*-----------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::SetPWM(XBYTE channel, XWORD on, XWORD off, bool wait) 
{ 		
	if(!IsInitialized())  return false;

	//XDEBUG_PRINTCOLOR(XDEBUG_COLORGREEN, __L("PC9685 set channel(%d), on(%d), off(%d), wait(%s)"), channel, on, off, wait?__L("true"):__L("false"));

  if(!SendCommand(DIOI2CPWMCONTROLERPCA9685_LED0_ON_L  + (4*channel), (on  &  0xFF), wait))	return false;
  if(!SendCommand(DIOI2CPWMCONTROLERPCA9685_LED0_ON_H  + (4*channel), (on  >> 0x08), wait))	return false;			 		
  if(!SendCommand(DIOI2CPWMCONTROLERPCA9685_LED0_OFF_L + (4*channel), (off &  0xFF), wait))	return false;	
  if(!SendCommand(DIOI2CPWMCONTROLERPCA9685_LED0_OFF_H + (4*channel), (off >> 0x08), wait))	return false;
	
	return true;			
}




/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::End
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
bool DIOI2CPWMCONTROLERPCA9685::End()
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

	isinitialized = false;

	return true;
}



/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::IniDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			09/09/2016 14:14:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::IniDevice()
{			
	diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO( diostreamcfg);
	if(!diostream)  return false;
		
	if(!diostream->Open())  return false;
	
	if(!diostream->WaitToConnected(timeout)) return false;
	
	if(!Reset()) return false;

	return DIODEVICE::Ini();
}





/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::ReadCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/06/2014 18:58:47
//	
//	@return 			bool : 
//
//  @param				cmd : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::ReadCommand(XBYTE cmd, XBYTE& value)
{	
	if(!diostream)			 return false;
	
	bool  status;

	status = diostream->Write(&cmd, 1)?true:false;

	if(status) status = diostream->WaitToFlushOutXBuffer(timeout);				
	if(!status) return false;
			
	status = diostream->WaitToFilledReadingBuffer(1, timeout);
	if(status) status = diostream->Read(&value, 1)?true:false;
	
	return status;
}




/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::SendCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			17/02/2016 16:43:00
//	
//	@return 			bool : 
//
//  @param				cmd : 
//  @param				value : 
//  @param				wait : 
*/
/*-----------------------------------------------------------------*/
bool DIOI2CPWMCONTROLERPCA9685::SendCommand(XBYTE cmd, XBYTE value, bool wait)
{
	if(!diostream)	return false;
		
	XBUFFER xbuffer;
					xbuffer.Add((XBYTE)cmd);
					xbuffer.Add((XBYTE)value);
	
	XDWORD size_written=diostream->Write(xbuffer);
	bool status = size_written>0 ? true : false; 
	
	/*
	XBYTE buffer[2] = { cmd, value };
	bool status = diostream->Write(buffer,2)?true:false;
	*/

	if(wait)
		{
			if(status) 
				status = diostream->WaitToFlushOutXBuffer(timeout);
		}

	return status;	
}

/*-------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685::GetDIOStream
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/03/2016 16:09:12
//	
//	@return 			DIOSTREAMI2C* : 
//
*/
/*-----------------------------------------------------------------*/
DIOSTREAMI2C* DIOI2CPWMCONTROLERPCA9685::GetDIOStream()
{
	return this->diostream;
}