/*------------------------------------------------------------------------------------------
//	DIOI2CGPIOMCP2317.H
*/	
/**	
// \class 
//   
//  Data IO I2C MCP23S17 (Microchip) (8+8 Digital I/O) class
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 03/05/2014 16:12:18
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOI2CGPIOMCP2317_H_
#define _DIOI2CGPIOMCP2317_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <stdio.h>

#include "XBase.h"

#include "DIODeviceI2C.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DIOI2CGPIOMCP2317PORT
{
	DIOI2CGPIOMCP2317PORT_A			=  0 ,	  //	PORT A
	DIOI2CGPIOMCP2317PORT_B					 ,	  //	PORT B
};


#define DIOI2CGPIOMCP2317_IODIR						0x00		// I/O direction 
#define DIOI2CGPIOMCP2317_GPIO						0x12		// Register for inputs	
#define DIOI2CGPIOMCP2317_OLAT						0x14		// Register for outputs


/*---- CLASS -----------------------------------------------------------------------------*/


class XFACTORY;
class XPUBLISHER;
class XTIMER;
class DIOFACTORY;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;


class DIOI2CGPIOMCP2317 :  public DIODEVICEI2C
{
	public:

															DIOI2CGPIOMCP2317						();
		virtual									 ~DIOI2CGPIOMCP2317						();

		bool											Configure										();				
		
		bool											ReadInputs									(DIOI2CGPIOMCP2317PORT port, XBYTE& value);
		bool											WriteOutputs								(DIOI2CGPIOMCP2317PORT port, XBYTE value);

		bool											SendCommand									(XBYTE cmd, XBYTE value);

		bool											End													();

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

