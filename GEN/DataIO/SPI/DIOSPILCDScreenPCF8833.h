/*------------------------------------------------------------------------------------------
//	DIOSPILCDSCREENPCF8833.H
*/	
/**	
// \class 
//   
//  Data IO SPI Screen LCD Philips PCF8833
//   
//	@author	 Abraham J. Velez
//	@version 07/10/2012 16:41:37
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOSPILCDSCREENPCF8833_H_
#define _DIOSPILCDSCREENPCF8833_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
		
#include <stdio.h>

#include "DIODeviceSPI.h"
#include "DIOGPIO.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


// -------------------------------------------------------------------------------
// Philips PCF8833 LCD controller command codes

#define DIOSPILCDSCREENPCF8833_NOP													0x00    // nop
#define DIOSPILCDSCREENPCF8833_SWRESET											0x01    // software reset
#define DIOSPILCDSCREENPCF8833_BSTROFF											0x02    // booster voltage OFF
#define DIOSPILCDSCREENPCF8833_BSTRON												0x03    // booster voltage ON
#define DIOSPILCDSCREENPCF8833_RDDIDIF											0x04    // read display identification
#define DIOSPILCDSCREENPCF8833_RDDST												0x09    // read display status
#define DIOSPILCDSCREENPCF8833_SLEEPIN											0x10    // sleep in
#define DIOSPILCDSCREENPCF8833_SLEEPOUT											0x11    // sleep out
#define DIOSPILCDSCREENPCF8833_PTLON												0x12    // partial display mode
#define DIOSPILCDSCREENPCF8833_NORON												0x13    // display normal mode
#define DIOSPILCDSCREENPCF8833_INVOFF												0x20    // inversion OFF
#define DIOSPILCDSCREENPCF8833_INVON												0x21    // inversion ON
#define DIOSPILCDSCREENPCF8833_DALO													0x22    // all pixel OFF
#define DIOSPILCDSCREENPCF8833_DAL													0x23    // all pixel ON
#define DIOSPILCDSCREENPCF8833_SETCON												0x25    // write contrast
#define DIOSPILCDSCREENPCF8833_DISPOFF											0x28    // display OFF
#define DIOSPILCDSCREENPCF8833_DISPON												0x29    // display ON
#define DIOSPILCDSCREENPCF8833_CASET												0x2A    // column address set
#define DIOSPILCDSCREENPCF8833_PASET												0x2B    // page address set
#define DIOSPILCDSCREENPCF8833_RAMWR												0x2C    // memory write
#define DIOSPILCDSCREENPCF8833_RGBSET												0x2D    // colour set
#define DIOSPILCDSCREENPCF8833_PTLAR												0x30    // partial area
#define DIOSPILCDSCREENPCF8833_VSCRDEF											0x33    // vertical scrolling definition
#define DIOSPILCDSCREENPCF8833_TEOFF												0x34    // test mode
#define DIOSPILCDSCREENPCF8833_TEON													0x35    // test mode
#define DIOSPILCDSCREENPCF8833_MADCTL												0x36    // memory access control
#define DIOSPILCDSCREENPCF8833_SEP													0x37    // vertical scrolling start address
#define DIOSPILCDSCREENPCF8833_IDMOFF												0x38    // idle mode OFF
#define DIOSPILCDSCREENPCF8833_IDMON												0x39    // idle mode ON
#define DIOSPILCDSCREENPCF8833_COLMOD												0x3A    // interface pixel format
#define DIOSPILCDSCREENPCF8833_SETVOP												0xB0    // set Vop
#define DIOSPILCDSCREENPCF8833_BRS													0xB4    // bottom row swap
#define DIOSPILCDSCREENPCF8833_TRS													0xB6    // top row swap
#define DIOSPILCDSCREENPCF8833_DISCTR												0xB9    // display control
#define DIOSPILCDSCREENPCF8833_DOR													0xBA    // data order
#define DIOSPILCDSCREENPCF8833_TCDFE												0xBD    // enable/disable DF temperature compensation
#define DIOSPILCDSCREENPCF8833_TCVOPE												0xBF    // enable/disable Vop temp comp
#define DIOSPILCDSCREENPCF8833_EC														0xC0    // internal or external oscillator
#define DIOSPILCDSCREENPCF8833_SETMUL												0xC2    // set multiplication factor
#define DIOSPILCDSCREENPCF8833_TCVOPAB											0xC3    // set TCVOP slopes A and B
#define DIOSPILCDSCREENPCF8833_TCVOPCD											0xC4    // set TCVOP slopes c and d
#define DIOSPILCDSCREENPCF8833_TCDF													0xC5    // set divider frequency
#define DIOSPILCDSCREENPCF8833_DF8COLOR											0xC6    // set divider frequency 8-color mode
#define DIOSPILCDSCREENPCF8833_SETBS												0xC7    // set bias system
#define DIOSPILCDSCREENPCF8833_RDTEMP												0xC8    // temperature read back
#define DIOSPILCDSCREENPCF8833_RDID1										    0xDA    // read ID1
#define DIOSPILCDSCREENPCF8833_RDID2												0xDB    // read ID2
#define DIOSPILCDSCREENPCF8833_RDID3												0xDC    // read ID3

#define DIOSPILCDSCREENPCF8833_ADDCOMMAND(command)         databuffer->AddNBits((XWORD)(command & ~0x100))		
#define DIOSPILCDSCREENPCF8833_ADDDATA(data)               databuffer->AddNBits((XWORD)(data    |  0x100))			

// 12-bit color definitions

#define DIOSPILCDSCREENPCF8833_WHITE												0xFFF0
#define DIOSPILCDSCREENPCF8833_BLACK												0x0000
#define DIOSPILCDSCREENPCF8833_RED													0xF000
#define DIOSPILCDSCREENPCF8833_GREEN												0x0F00
#define DIOSPILCDSCREENPCF8833_BLUE													0x00F0
#define DIOSPILCDSCREENPCF8833_CYAN													0x0FF0
#define DIOSPILCDSCREENPCF8833_MAGENTA											0xF0F0
#define DIOSPILCDSCREENPCF8833_YELLOW												0xFF00
#define DIOSPILCDSCREENPCF8833_BROWN												0xB220
#define DIOSPILCDSCREENPCF8833_ORANGE												0xFA00
#define DIOSPILCDSCREENPCF8833_PINK													0xF6A0



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XSYSTEM;
class XPUBLISHER;
class XBUFFERBITS;
class DIOFACTORY;
class DIOSTREAMSPI;

class DIOSPILCDSCREENPCF8833 : public DIODEVICESPI
{
	public:
														DIOSPILCDSCREENPCF8833					(XSYSTEM* xsystem);																										
		virtual						 		 ~DIOSPILCDSCREENPCF8833					();
				
		bool										IniDevice												();

		bool										Clear														(XWORD color = DIOSPILCDSCREENPCF8833_BLACK);
		bool										Update													(XBYTE* buffer);	
		bool										PutPixel												(XWORD x, XWORD y, XWORD color);

		bool										SetInversion										(bool on);
		bool										SetWindow												(XWORD x0, XWORD y0, XWORD x1, XWORD y1);

		bool										End															();
																		
		DIOGPIO*								GetDIOGPIO											()																							{ return diogpio;																	}
		void										SetDIOGPIO											(DIOGPIO* diogpio)															{ this->diogpio		 = diogpio;											}
		
		DIOGPIO_PINSGROUP				GetResetPinsGroup     					()																							{ return resetpinsgroup;													}
		void										SetResetPinsGroup   						(DIOGPIO_PINSGROUP resetpinsgroup = DIOGPIO_PINSGROUP_A)					
														{ 
															this->resetpinsgroup = resetpinsgroup;                      
														}

		int											GetResetPin         						()																							{ return resetpin;																}
		void										SetResetPin        							(int resetpin = DIOGPIO_PINNOTUSE)							{ this->resetpin = resetpin;                      }

		int											GetWidth		        						()																							{ return width;																		}
		int											GetHeight			      						()																							{ return height;							                    }
		XDWORD									GetNPixels											()																							{ return (width*height);													}	
		XDWORD									GetSizeBuffer										()																							{ return sizebuffervideo;													}	
														
	private:

		void										Clean														()
														{															
															xsystem									= NULL;
															
															diogpio									= NULL;
															diostream								= NULL;
															isdirectconnexion				= false;														
													
															databuffer							= NULL;																		
															resetpinsgroup					= DIOGPIO_PINSGROUP_A;
															resetpin								= DIOGPIO_PINNOTUSE;

															width                   = 0;
															height                  = 0;															
															sizebuffervideo					= 0;
														}
		

		bool										LCD_Reset												();		
		bool										LCD_Init												();
					
		bool										Buffer_AdjustWithNOP						();		
		bool										Buffer_Resize										(XDWORD newsize);		
		bool										Buffer_Send											();		
		bool										Buffer_Delete										();
																										    
    XSYSTEM*								xsystem;
	
		DIOGPIO*								diogpio;
		DIOSTREAMSPI*						diostream;
		bool										isdirectconnexion;

		XBUFFER*								databuffer;
		DIOGPIO_PINSGROUP				resetpinsgroup;
		int											resetpin;  		

		int											width;
		int											height;
		int                     sizebuffervideo;
};
	

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

