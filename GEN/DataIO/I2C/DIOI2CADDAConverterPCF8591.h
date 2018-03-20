/*------------------------------------------------------------------------------------------
//	DIOI2CADDACONVERTERPCF8591.H
*/	
/**	
// \class 
//   
//  Data IO I2C PCF8591 (AD/DA converter) class
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 03/05/2014 16:12:18
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOI2CADDACONVERTERPCF8591_H_
#define _DIOI2CADDACONVERTERPCF8591_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <stdio.h>

#include "XBase.h"

#include "DIODeviceI2C.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class XFACTORY;
class XPUBLISHER;
class XTIMER;
class DIOFACTORY;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;


class DIOI2CADDACONVERTERPCF8591 : public DIODEVICEI2C
{
	public:

															DIOI2CADDACONVERTERPCF8591	();
		virtual									 ~DIOI2CADDACONVERTERPCF8591	();

		bool											ReadADChannels              (XBYTE values[4]);
		bool											WriteDAChannel              (XBYTE value);

		bool											End													();

	protected:
			
		DIOSTREAMI2C*							diostream;
			
	private:

		bool											IniDevice										();

		void											Clean												()
															{																
																diostream				= NULL;																																																
															}		
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

