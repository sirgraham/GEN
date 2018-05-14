/*------------------------------------------------------------------------------------------
//	TESTMICRO.H
*/	
/**	
// \class 
//   
//  Organic I2C Module
//   
//	@author	 Abraham J. Velez
//	@version 13/11/2013 12:20:06
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTMICRO_H_
#define _TESTMICRO_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XApplication.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define TESTMICRO_VERSION						    0
#define TESTMICRO_SUBVERSION					  1
#define TESTMICRO_SUBVERSIONERR				  0
#define TESTMICRO_NAMEAPP			 				  __L("TestMicro")



#ifdef HW_PC
  #define TEST_PINLCD_RESET  					 0
	#define TEST_PINLCD_DC  						 0
  #define TEST_PINSPI_MOSI						 1	
  #define TEST_PINSPI_MISO						 4
  #define TEST_PINSPI_CLK							 2
  #define TEST_PINSPI_CS							 3
#endif


#ifdef HW_RASPBERRYPI		
	#define TEST_PINLCD_RESET  					11
	#define TEST_PINLCD_DC  						22
  #define TEST_PINSPI_CLK							23
  #define TEST_PINSPI_MISO						21
  #define TEST_PINSPI_MOSI						19	
  #define TEST_PINSPI_CS							24	
#endif


#ifdef STM32F072xB
  #define TEST_PINLCD_RESET  					 4
	#define TEST_PINLCD_DC  						22
  #define TEST_PINSPI_CLK							23
  #define TEST_PINSPI_MISO						21
  #define TEST_PINSPI_MOSI						19	
  #define TEST_PINSPI_CS							24	
#endif


#ifdef STM32F303xC
  #define TEST_PINLCD_RESET  					 4
	#define TEST_PINLCD_DC  						22
  #define TEST_PINSPI_CLK							23
  #define TEST_PINSPI_MISO						21
  #define TEST_PINSPI_MOSI						19	
  #define TEST_PINSPI_CS							24	
#endif


#if defined(STM32F411xE) || defined(STM32F407xx)
  #define TEST_PINLCD_RESET  					 4
	#define TEST_PINLCD_DC  						22
  #define TEST_PINSPI_CLK							23
  #define TEST_PINSPI_MISO						21
  #define TEST_PINSPI_MOSI						19	
  #define TEST_PINSPI_CS							24	
#endif

#ifdef STM32F446xx
  #define TEST_PINLCD_RESET  					 4
	#define TEST_PINLCD_DC  						22
  #define TEST_PINSPI_CLK							23
  #define TEST_PINSPI_MISO						21
  #define TEST_PINSPI_MOSI						19	
  #define TEST_PINSPI_CS							24	
#endif


//---- CLASS -------------------------------------------------------------------------------

class TESTMICRO : public XAPPLICATION
{
	public:	
																								TESTMICRO														();													
		virtual																		 ~TESTMICRO														();

		bool																				Ini 																();
		bool																				FirstUpdate													();
		bool																				Update															();
        
		bool																				End																	();

    bool                                        Test_Random                         ();
		bool																				Test_DIOStreamUART									();
    bool                                        Test_DIOStreamUSBReadCommand        (DIOSTREAM*	diostream, XBUFFER& command);
		bool																				Test_DIOStreamUSB										();
		bool																				Test_DIOStreamI2C										();
		bool																				Test_DIOStreamSPI										();
    bool                                        Test_DIOGPIO                        ();
    bool                                        Test_SRAM                           ();
				
	private:

		void																				Clean																()
																								{
                                                  																								                                                 
																								}	
    		
};


	
//---- INLINE FUNCTIONS --------------------------------------------------------------------

extern TESTMICRO* testmicro;

#endif

