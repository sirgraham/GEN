/*------------------------------------------------------------------------------------------
//	DIOI2CPWMCONTROLERPCA9685.H
*/	
/**	
// \class 
//   
//  Data IO I2C PCA9685 (16-channel, 12-bit PWM controller) class
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 03/05/2014 16:12:18
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOI2CPWMCONTROLERPCA9685_H_
#define _DIOI2CPWMCONTROLERPCA9685_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <stdio.h>

#include "XBase.h"
#include "XString.h"

#include "DIODeviceI2C.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOI2CPWMCONTROLERPCA9685_MODE1					0x00
#define DIOI2CPWMCONTROLERPCA9685_PRESCALE			0xFE

#define DIOI2CPWMCONTROLERPCA9685_LED0_ON_L			0x06
#define DIOI2CPWMCONTROLERPCA9685_LED0_ON_H			0x07
#define DIOI2CPWMCONTROLERPCA9685_LED0_OFF_L		0x08
#define DIOI2CPWMCONTROLERPCA9685_LED0_OFF_H		0x09

#define DIOI2CPWMCONTROLERPCA9685_ALLLED_ON_L		0xFA
#define DIOI2CPWMCONTROLERPCA9685_ALLLED_ON_H		0xFB
#define DIOI2CPWMCONTROLERPCA9685_ALLLED_OFF_L	0xFC
#define DIOI2CPWMCONTROLERPCA9685_ALLLED_OFF_H	0xFD


/*---- CLASS -----------------------------------------------------------------------------*/


class XFACTORY;
class XPUBLISHER;
class XTIMER;
class DIOFACTORY;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;


class DIOI2CPWMCONTROLERPCA9685 : public DIODEVICEI2C
{
	public:

															DIOI2CPWMCONTROLERPCA9685		();
		virtual									 ~DIOI2CPWMCONTROLERPCA9685		();

		bool											Reset												();
		bool											SetPWMFrequency							(float frequency);
		bool											SetPWM											(XBYTE channel, XWORD on, XWORD off, bool wait = true);
	
		bool											End													();

		bool											ReadCommand									(XBYTE cmd, XBYTE& value);
		bool											SendCommand									(XBYTE cmd, XBYTE value, bool wait = true);

		DIOSTREAMI2C*							GetDIOStream								();

	protected:
	
		DIOSTREAMI2C*							diostream;
			
	private:
		
		bool											IniDevice										();

		void											Clean												()
															{																
																diostream			= NULL;																																																
															}		
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

