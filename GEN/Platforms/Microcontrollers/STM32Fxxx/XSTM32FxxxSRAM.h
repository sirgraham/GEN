//------------------------------------------------------------------------------------------
//	XSTM32FXXXSRAM.H
//	
/**	
// \class 
//   
//  STM32Fxxx SRAM class
//   
//	@author	 Abraham J. Velez
//	@version 04/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XSTM32FXXXSRAM_H_
#define _XSTM32FXXXSRAM_H_
	
						
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"

#include "XSTM32Fxxx_HAL.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------
                            
//#define SRAM1_BASE            0x20000000U /*!< SRAM1(128 KB) base address in the alias region                             */
//#define BKPSRAM_BASE          0x40024000U /*!< Backup SRAM(4 KB) base address in the alias region                         */
//#define SRAM1_BB_BASE         0x22000000U /*!< SRAM1(128 KB) base address in the bit-band region                          */
//#define BKPSRAM_BB_BASE       0x42480000U /*!< Backup SRAM(4 KB) base address in the bit-band region                      */



#define XSTM32FXXXSRAM_BASE      BKPSRAM_BASE
#define XSTM32FXXXSRAM_MAXSIZE   4096

//---- CLASS -------------------------------------------------------------------------------


class XSTM32FXXXSRAM 
{
	public:

																	XSTM32FXXXSRAM		();
		virtual											 ~XSTM32FXXXSRAM	 	();
    
    bool													Ini               ();       
    bool													End               (); 
    
    bool													Read              (XDWORD offset, XBYTE& data);
    bool													Read              (XDWORD offset, XWORD& data);
    bool													Read              (XDWORD offset, XDWORD& data);
    bool													Read              (XDWORD offset, XBYTE* data, XDWORD size);
    
   
    bool													Write             (XDWORD offset, XBYTE data);
    bool													Write             (XDWORD offset, XWORD data);
    bool													Write             (XDWORD offset, XDWORD data);
    bool													Write             (XDWORD offset, XBYTE* data, XDWORD size);
    
    bool													EraseAll          ();             
         
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------
										
#endif

