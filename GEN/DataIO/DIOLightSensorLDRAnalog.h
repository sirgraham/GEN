/*------------------------------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG.H
*/	
/**	
// \class 
//   
//  Data IO Sensor Light LDR (Light Dependent Resistor) Analog in port digital
//  
//  Diagram:
//	http://www.raspberrypi-spy.co.uk/2012/08/reading-analogue-sensors-with-one-gpio-pin/
//
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 22/03/2014 17:11:08
//	Last Modification	:	
*/	
/*	(C) Copyright EndoraSoft S.L.. All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOLIGHTSENSORLDRANALOG_H_
#define _DIOLIGHTSENSORLDRANALOG_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

#include "DIODevice.h"
#include "DIOGPIO.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XTIMER;
class XTHREADCOLLECTED;
class XMUTEX;
class DIOGPIO;

class DIOLIGHTSENSORLDRANALOG :  public DIODEVICE
{
	public:
													DIOLIGHTSENSORLDRANALOG				( DIOGPIO* diogpio, int pindata, bool activatecache);
		virtual							 ~DIOLIGHTSENSORLDRANALOG				();

		bool									Ini                           ();

		int										GetPinData										()																{ return pindata;										}
		
		bool									ReadFromCache									(XDWORD& level);
		bool									ReadDirect										(XDWORD& level);

		bool									End		                        ();

	private:

		void									Clean													()
													{
														diogpio				= NULL;
														pindata						= DIOGPIO_PINNOTUSE;

														xtimer						= NULL;
														threadcache				= NULL;
														xmutexread				= NULL;

														nreads						= 0;
														levelcache				= 0;
														
													}

		static void						ThreadRunFunction						(void* param);	

		DIOGPIO*							diogpio;
		int										pindata;

		XTIMER*								xtimer;
		XTHREADCOLLECTED*			threadcache;
		XMUTEX*								xmutexread;

		XDWORD								nreads;	
		XDWORD								levelcache;				
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

