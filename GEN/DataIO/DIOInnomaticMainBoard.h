/*------------------------------------------------------------------------------------------
//	DIOINNOMATICMAINBOARD.H
*/	
/**	
// \class 
//   
//  Board Control (GPIO, I2C memory, Rele, etc...)
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 17/02/2016 15:59:42
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOINNOMATICMAINBOARD_H_
#define _DIOINNOMATICMAINBOARD_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XLog.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamI2C.h"
#include "DIOI2CGPIOPCF8574.h"
#include "DIOI2CEEprom24XXX.h"
#include "DIOI2CPWMControlerPCA9685.h"
#include "DIOGPIO.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define DIOINNOMATICMAINBOARD_I2CDEVICENAME						__L("/dev/i2c-1")

#define DIOINNOMATICMAINBOARD_I2CDIGIGPIO_ID_IN				0x3C
#define DIOINNOMATICMAINBOARD_I2CDIGIGPIO_ID_OUT			0x3D
#define DIOINNOMATICMAINBOARD_I2CDIGIGPIO_ID_RELAYS		0x39
#define DIOINNOMATICMAINBOARD_I2CEEPROM								0x50
#define DIOINNOMATICMAINBOARD_I2CPWM									0x40


enum DIOINNOMATICMAINBOARD_HEADER
{
	DIOINNOMATICMAINBOARD_HEADER_UNKNOW								= 0 ,
	DIOINNOMATICMAINBOARD_HEADER_P1												,
	DIOINNOMATICMAINBOARD_HEADER_P24										  ,
	DIOINNOMATICMAINBOARD_HEADER_SLOT										  ,
};

#define DIOINNOMATICMAINBOARD_P1_PIN1									37     // GPIO 26 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_P1_PIN2									35     // GPIO 19 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_P1_PIN3									33     // GPIO 13 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_P1_PIN4									32     // GPIO 12 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_P1_PIN5									22     // GPIO 25 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_P1_PIN6									18     // GPIO 24 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_P1_PIN7									       // 5v
#define DIOINNOMATICMAINBOARD_P1_PIN8									       // GND

#define DIOINNOMATICMAINBOARD_P24_PIN1									     // 3v3     
#define DIOINNOMATICMAINBOARD_P24_PIN2								15     // GPIO 22 Raspberry Pi.			Extern button Button A
#define DIOINNOMATICMAINBOARD_P24_PIN3								12     // GPIO 18 Raspberry Pi.			Extern button Button B
#define DIOINNOMATICMAINBOARD_P24_PIN4								13     // GPIO 27 Raspberry Pi.			Extern button Led    A
#define DIOINNOMATICMAINBOARD_P24_PIN5								11     // GPIO 11 Raspberry Pi.			Extern button Led    B 
#define DIOINNOMATICMAINBOARD_P24_PIN6										   // GND

#define DIOINNOMATICMAINBOARD_SLOT_PIN9              	38     // GPIO 20 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_SLOT_PIN10             	29     // GPIO  5 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_SLOT_PIN11             	31     // GPIO  6 Raspberry Pi.
#define DIOINNOMATICMAINBOARD_SLOT_PIN21             	40     // GPIO 21 Raspberry Pi.


#define DIOINNOMATICMAINBOARD_DEFAULTTIMEOUT					10

#define DIOINNOMATICMAINBOARD_LOGSECTIONID						__L("Inno MainBoard")

enum DIOINNOMATICMAINBOARD_I2CDIGIGPIO
{
	DIOINNOMATICMAINBOARD_I2CDIGIGPIO_IN				= 0 ,
	DIOINNOMATICMAINBOARD_I2CDIGIGPIO_OUT					  ,
	DIOINNOMATICMAINBOARD_I2CDIGIGPIO_RELAYS				,
};


/*---- CLASS -----------------------------------------------------------------------------*/


class DIOINNOMATICMAINBOARD
{
	public:

																DIOINNOMATICMAINBOARD						()															{ Clean();										 }
		virtual										 ~DIOINNOMATICMAINBOARD						()															{ Clean();										 }

		bool												Ini															();


		bool												SetModeHeaderPin								(DIOINNOMATICMAINBOARD_HEADER header, int pin, bool isinput)
																{
																	if(!gpio)	return false;

																	switch(header)
																		{
																			case DIOINNOMATICMAINBOARD_HEADER_P1				: switch(pin)
																																											{
																																												case 1	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P1_PIN1			, isinput))	return false; break;	
																																												case 2	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P1_PIN2			, isinput))	return false; break;	
																																												case 3	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P1_PIN3			, isinput))	return false; break;	
																																												case 4	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P1_PIN4			, isinput))	return false; break;																									
																																												case 5	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P1_PIN5			, isinput))	return false; break;																											
																																												case 6	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P1_PIN6			, isinput))	return false; break;																																														
																																												default : return false;
																																											}
																																										break;

																			case DIOINNOMATICMAINBOARD_HEADER_P24				: switch(pin)
																																											{
																																												case 2	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P24_PIN2		, isinput))	return false; break;	
																																												case 3	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P24_PIN3		, isinput))	return false; break;	
																																												case 4	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P24_PIN4		, isinput))	return false; break;	
																																												case 5	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_P24_PIN5		, isinput))	return false; break;																									
																																												default : return false;
																																											}
																																										break;

																			case DIOINNOMATICMAINBOARD_HEADER_SLOT			: switch(pin)
																																											{
																																												case  9	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_SLOT_PIN9		, isinput))	return false; break;	
																																												case 10	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_SLOT_PIN10	,	isinput))	return false; break;	
																																												case 11 : if(!gpio->SetMode(DIOINNOMATICMAINBOARD_SLOT_PIN11	, isinput))	return false; break;	
																																												case 21	: if(!gpio->SetMode(DIOINNOMATICMAINBOARD_SLOT_PIN21	, isinput))	return false; break;																									
																																												default : return false;
																																											}
																																										break;

																																			default			: return false;
																		}

																	

																	return true;
																}

			
		bool                        GetStatusHeaderPin							(DIOINNOMATICMAINBOARD_HEADER header, int pin)
																{
																	if(!gpio)	return false;

																	switch(header)
																		{
																			case DIOINNOMATICMAINBOARD_HEADER_P1				:	switch(pin)
																																											{
																																												case 1	: return gpio->Get(DIOINNOMATICMAINBOARD_P1_PIN1);
																																												case 2	: return gpio->Get(DIOINNOMATICMAINBOARD_P1_PIN2);
																																												case 3	: return gpio->Get(DIOINNOMATICMAINBOARD_P1_PIN3);																								
																																												case 4	: return gpio->Get(DIOINNOMATICMAINBOARD_P1_PIN4);																			
																																												case 5	: return gpio->Get(DIOINNOMATICMAINBOARD_P1_PIN5);																			
																																												case 6	: return gpio->Get(DIOINNOMATICMAINBOARD_P1_PIN6);																			
																																												default : return false;
																																											}
																																										break;

																			case DIOINNOMATICMAINBOARD_HEADER_P24				: switch(pin)
																																											{
																																												case 2	: return gpio->Get(DIOINNOMATICMAINBOARD_P24_PIN2);
																																												case 3	: return gpio->Get(DIOINNOMATICMAINBOARD_P24_PIN3);
																																												case 4	: return gpio->Get(DIOINNOMATICMAINBOARD_P24_PIN4);																								
																																												case 5	: return gpio->Get(DIOINNOMATICMAINBOARD_P24_PIN5);																			
																																												default : return false;
																																											}
																																										break;

																			case DIOINNOMATICMAINBOARD_HEADER_SLOT			: switch(pin)
																																											{
																																												case  9	: return gpio->Get(DIOINNOMATICMAINBOARD_SLOT_PIN9);
																																												case 10	: return gpio->Get(DIOINNOMATICMAINBOARD_SLOT_PIN10);
																																												case 11 : return gpio->Get(DIOINNOMATICMAINBOARD_SLOT_PIN11);
																																												case 21	: return gpio->Get(DIOINNOMATICMAINBOARD_SLOT_PIN21);
																																												default : return false;
																																											}
																																										break;

																																			default			: return false;
																		}

																	return true;		
																}


		bool                        SetStatusHeaderPin							(DIOINNOMATICMAINBOARD_HEADER header, int pin, bool active)
																{
																	if(!gpio)	return false;

																	switch(header)
																		{
																			case DIOINNOMATICMAINBOARD_HEADER_P1				:	switch(pin)
																																											{
																																												case 1	: return gpio->Set(DIOINNOMATICMAINBOARD_P1_PIN1, active);
																																												case 2	: return gpio->Set(DIOINNOMATICMAINBOARD_P1_PIN2, active);
																																												case 3	: return gpio->Set(DIOINNOMATICMAINBOARD_P1_PIN3, active);																								
																																												case 4	: return gpio->Set(DIOINNOMATICMAINBOARD_P1_PIN4, active);																			
																																												case 5	: return gpio->Set(DIOINNOMATICMAINBOARD_P1_PIN5, active);																								
																																												case 6	: return gpio->Set(DIOINNOMATICMAINBOARD_P1_PIN6, active);																			
																																												default : return false;
																																											}
																																										break;

																			case DIOINNOMATICMAINBOARD_HEADER_P24				: switch(pin)
																																											{
																																												case 2	: return gpio->Set(DIOINNOMATICMAINBOARD_P24_PIN2, active);
																																												case 3	: return gpio->Set(DIOINNOMATICMAINBOARD_P24_PIN3, active);
																																												case 4	: return gpio->Set(DIOINNOMATICMAINBOARD_P24_PIN4, active);																								
																																												case 5	: return gpio->Set(DIOINNOMATICMAINBOARD_P24_PIN5, active);																			
																																												default : return false;
																																											}
																																										break;

																			case DIOINNOMATICMAINBOARD_HEADER_SLOT			: switch(pin)
																																											{
																																												case  9	: return gpio->Set(DIOINNOMATICMAINBOARD_SLOT_PIN9 , active);
																																												case 10	: return gpio->Set(DIOINNOMATICMAINBOARD_SLOT_PIN10, active);
																																												case 11 : return gpio->Set(DIOINNOMATICMAINBOARD_SLOT_PIN11, active);
																																												case 21	: return gpio->Set(DIOINNOMATICMAINBOARD_SLOT_PIN21, active);
																																												default : return false;
																																											}
																																										break;		

																																			default			: return false;	
																	}

																	return true;		
																}



		bool												SetP4HeaderForExternalButton		(bool on = true)
																{																		
																	if(!SetModeHeaderPin(DIOINNOMATICMAINBOARD_HEADER_P24, 2, on))		return false;
																	if(!SetModeHeaderPin(DIOINNOMATICMAINBOARD_HEADER_P24, 3, on))		return false;																																				
																	if(!SetModeHeaderPin(DIOINNOMATICMAINBOARD_HEADER_P24, 4, false)) return false;
																	if(!SetModeHeaderPin(DIOINNOMATICMAINBOARD_HEADER_P24, 5, false)) return false;	

																	SetLedA(false);
																	SetLedB(false);
	
																	issetp24forexternalbutton = on;
																	
																	return true;
																}


		bool												IsButtonAPressed								()
																{																	
																	if(!issetp24forexternalbutton) return false;
																	return GetStatusHeaderPin(DIOINNOMATICMAINBOARD_HEADER_P24, 2);
																}

		bool												IsButtonBPressed								()
																{
																	if(!issetp24forexternalbutton) return false;
																	return GetStatusHeaderPin(DIOINNOMATICMAINBOARD_HEADER_P24, 3);
																}
	
		bool												SetLedA													(bool on)
																{
																	if(!issetp24forexternalbutton) return false;
																	return SetStatusHeaderPin(DIOINNOMATICMAINBOARD_HEADER_P24, 4, on);
																}

		bool												SetLedB													(bool on)
																{
																	if(!issetp24forexternalbutton) return false;
																	return SetStatusHeaderPin(DIOINNOMATICMAINBOARD_HEADER_P24, 5, on);
																}

		bool												GetDigitalInput									(XBYTE&data);		
		bool												SetDigitalOutput								(XBYTE bit, bool on);

		bool												SwitchReleK1										(bool on);		
		bool												SwitchReleK2										(bool on);		
		bool												SwitchReleK3										(bool on);		

		bool												EEProm													(bool write, XBUFFER& xbuffer)
																{
																	if(!eeprom)										return false;
																	if(!eeprom->IsInitialized())	return false;

																	bool status;

																	if(!write)		
																				status =  eeprom->Read(0, 16, xbuffer);			
																	 else status =  eeprom->Write(0, xbuffer);			

																	return status;
																}

		DIOGPIO*										GetGPIO													()															{ return gpio;								 }
		
		bool												SetPWM													(XBYTE channel, XWORD on, XWORD off, bool wait = true)
																{
																	if(!pca9685) return false;
																	return pca9685->SetPWM(channel, on, off, wait); 
																}	
		

		bool												ResetMicroControler							()
																{																								
																	if(!SetModeHeaderPin(DIOINNOMATICMAINBOARD_HEADER_SLOT,  9, false))	return false;   // Reset Micro																	
																	if(!SetStatusHeaderPin(DIOINNOMATICMAINBOARD_HEADER_SLOT, 9, true)) return false;

																		bool pinstatus = false;

																		if(SetStatusHeaderPin(DIOINNOMATICMAINBOARD_HEADER_SLOT, 9,  pinstatus))
																			{
																				xsleep->MilliSeconds(10);
			
																				pinstatus = !pinstatus;

																				return SetStatusHeaderPin(DIOINNOMATICMAINBOARD_HEADER_SLOT, 9,  pinstatus);
																			}

																		return false;
																}

		bool                        End															();


	protected:

		DIOI2CGPIOPCF8574*					pcf8574[3]; 
		DIOI2CEEPROM24XXX*					eeprom;
		DIOGPIO*										gpio;
		DIOI2CPWMCONTROLERPCA9685*	pca9685;
	
	private:

		void                        Clean														() 
																{																	
																	pcf8574[0]								 = NULL; 
																	pcf8574[1]								 = NULL; 
																	pcf8574[2]								 = NULL;

																	eeprom										 = NULL;

																	gpio										 	 = NULL;

																	pca9685										 = NULL;	

																	issetp24forexternalbutton  = false;  	
																	
																	outputdigitaldata					 = 0;
																}
																		
	
		bool												issetp24forexternalbutton;		
		XBYTE                       outputdigitaldata;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

