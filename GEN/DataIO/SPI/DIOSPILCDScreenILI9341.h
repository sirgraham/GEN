/*------------------------------------------------------------------------------------------
//	DIOSPILCDSCREENILI9341.H
*/	
/**	
// \class 
//   
//  Data IO SPI LCD Screen Ilitek ILI9341
//   
//	@author	 Abraham J. Velez
//	@version 07/10/2012 16:41:37
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOSPILCDSCREENILI9341_H_
#define _DIOSPILCDSCREENILI9341_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include <stdio.h>
#include <stdarg.h>

#include "DIODevice.h"
#include "DIOGPIO.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSPILCDSCREENILI9341_DCHIGH										diogpio->Set(pinDC, true); 																					
#define DIOSPILCDSCREENILI9341_DCLOW										diogpio->Set(pinDC, false);
																								
#define DIOSPILCDSCREENILI9341_RED											0xf800
#define DIOSPILCDSCREENILI9341_GREEN										0x001f
#define DIOSPILCDSCREENILI9341_BLUE											0x07e0
#define DIOSPILCDSCREENILI9341_BLACK										0x0000
#define DIOSPILCDSCREENILI9341_WHITE										0xffff


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XSYSTEM;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMSPI;

class DIOSPILCDSCREENILI9341 : public DIODEVICE
{
	public:
														DIOSPILCDSCREENILI9341					(XSYSTEM* xsystem);																										
		virtual						 		 ~DIOSPILCDSCREENILI9341					();
				
		bool										Ini															(int chipselect, int timeout);
		bool										Clear														(XWORD color = DIOSPILCDSCREENILI9341_BLACK);
		bool										Update													(XBYTE* buffer);		
		bool										End															();
		
		DIOGPIO*								GetDIOGPIO											()																							{ return diogpio;																	}
		void										SetDIOGPIO											(DIOGPIO* diogpio)															{ this->diogpio		 = diogpio;											}
																
		DIOSTREAMSPI*						GetDIOStreamSPI									()																							{ return diostreamSPI;														}
		void										SetDIOStreamSPI									(DIOSTREAMSPI* diostreamSPI)																							
														{ 
															this->diostreamSPI		 = diostreamSPI;															
															isdiostreamSPIexternal = true;
														}


		int											GetPinReset         						()																							{ return pinreset;																}	
		void										SetPinReset	        						(int pinReset = DIOGPIO_PINNOTUSE)							{	this->pinreset = pinreset;											}
		int											GetPinDC		         						()																							{ return pinDC;																		}	
		void										SetPinDC		        						(int pinDC		= DIOGPIO_PINNOTUSE)							{	this->pinDC		 = pinDC;													}

		int											GetWidth		        						()																							{ return width;																		}
		int											GetHeight			      						()																							{ return height;							                    }
		XDWORD									GetNPixels											()																							{ return (width*height);													}	
		XDWORD									GetSizeBuffer										()																							{ return ((GetNPixels()*16)/8);										}	
																
	private:

		void										Clean														()
														{																													
															xsystem									= NULL;
																												
															timeout									= 0;	
															
															diogpio							= NULL;
															diostreamSPIcfg					= NULL;
															diostreamSPI						= NULL;
															isdiostreamSPIexternal	= false;														

															xtimer									= NULL;
															databuffer							= NULL;																		

															pinreset								= DIOGPIO_PINNOTUSE;	
															pinDC										= DIOGPIO_PINNOTUSE;	

															width                   = 0;
															height                  = 0;															
														}
		

		bool										LCD_Init												();
		bool										LCD_Reset												();
		bool										LCD_RotateRGB										(int rotate, XBYTE bgr);
		bool										LCD_SetWindow										(int xs, int ys, int xe, int ye);

		bool										SendCommand											(XBYTE command);
		bool										SendData		                    ();

		bool										WriteRegister										(XBYTE reg, int ndata, ...);

		bool										DeleteBuffer										();
																											
    
    XSYSTEM*								xsystem;
		
		int											timeout;

		DIOGPIO*								diogpio;
		DIOSTREAMSPICONFIG*			diostreamSPIcfg;
		DIOSTREAMSPI*						diostreamSPI;
		bool										isdiostreamSPIexternal;
		
		XTIMER*									xtimer;
		XBUFFER*								databuffer;
		
		int											pinreset;  
		int											pinDC;  

		int											width;
		int											height;	
};
	

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

