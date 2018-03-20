//------------------------------------------------------------------------------------------
//	DIOSTM32FXXXSTREAMSPI.H
//	
/**	
// \class 
//   
//  STM32Fxxx Data IO Stream SPI class
//   
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTM32FXXXSTREAMSPI_H_
#define _DIOSTM32FXXXSTREAMSPI_H_
	
#if defined(DIO_ACTIVE) && defined(DIOSPI_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XFSMachine.h"

#include "XSTM32Fxxx_HAL.h"

#include "DIOStreamSPI.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSTM32FXXXSTREAMSPI_MAXHANDLES   2
#define DIOSTM32FXXXSTREAMSPI_MAXPORTS     2

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class XTHREADCOLLECTED;


class DIOSTM32FXXXSTREAMSPIPORT : public DIOSTREAMSPI
{
	public:
                                            DIOSTM32FXXXSTREAMSPIPORT	  			  ()														{ Clean();																}
		virtual							                   ~DIOSTM32FXXXSTREAMSPIPORT 				  ()														{ Clean();																}	
	
		
		XDWORD													        GetCounterRef 											()														{ return counterref;											}
		void														        SetCounterRef												(XDWORD counterref)						{ this->counterref = counterref;				  }
	
		SPI_HandleTypeDef*							        GetHandleSPI												()														{ return hspi;														}
		void														        SetHandleSPI											  (SPI_HandleTypeDef* hspi)			{ this->hspi = hspi;											}

	private:

		void									                  Clean					      								()
                                            { 
                                              counterref        = 0;
                                              hspi              = NULL;                                      
                                            } 

		XDWORD													        counterref;
		SPI_HandleTypeDef*							        hspi;		
};




class DIOSTM32FXXXSTREAMSPI : public DIOSTREAMSPI
{
	public:
																						DIOSTM32FXXXSTREAMSPI				        ();
		virtual																 ~DIOSTM32FXXXSTREAMSPI				        ();
		
		DIOSTREAMSTATUS													GetConnectStatus							      ();		
      
		bool																		Open													      ();	
    
		bool																		WaitToFilledReadingBuffer			      (int filledto = DIOSTREAM_SOMETHINGTOREAD, int timeout = XTIMER_INFINITE);			
		
		XDWORD																	ReadDirect										      (XBYTE* buffer, XDWORD size);
		XDWORD																	WriteDirect										      (XBYTE* buffer, XDWORD size);
    
    XDWORD                                  Write                               (XBYTE* buffer, XDWORD size);

		bool																		Disconnect										      ()				  							    {	return false;												    }
		bool																		Close													      ();		

		bool																		CleanBuffers									      (); 											
   
    SPI_HandleTypeDef*											hspi;
    XWORD																		localaddress;
    XWORD																		remoteaddress;
    XBYTE																		bufferdata[DIOSTREAM_MAXBUFFER];	
    XWORD																		sizebufferdata;
    
    static DIOSTM32FXXXSTREAMSPIPORT*       ports[DIOSTM32FXXXSTREAMSPI_MAXPORTS];
    static DIOSTM32FXXXSTREAMSPI*						handles[DIOSTM32FXXXSTREAMSPI_MAXHANDLES]; 
    
    int																			indexhandle; 
    int																			indexport;
   
	protected:

    void																		Clean													      ()
																						{ 
																							hspi              = NULL;
																							localaddress      = 0;
																							remoteaddress     = 0;                                 
																							threadconnexion   = NULL;
																							indexhandle       = -1;
																							indexport         = -1;
																							sizebufferdata		= 0; 
																						} 
		
		static void															ThreadConnexion								      (void* param); 
	
		XTHREADCOLLECTED*												threadconnexion;				    
    
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif





