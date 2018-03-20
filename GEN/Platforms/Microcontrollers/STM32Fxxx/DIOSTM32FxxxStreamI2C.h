//------------------------------------------------------------------------------------------
//	DIOSTM32FXXXSTREAMI2C.H
//	
/**	
// \class 
//   
//  STM32Fxxx Data IO Stream I2C class
//   
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTM32FXXXSTREAMI2C_H_
#define _DIOSTM32FXXXSTREAMI2C_H_
	
#if defined(DIO_ACTIVE) && defined(DIOI2C_ACTIVE)
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XFSMachine.h"

#include "XSTM32Fxxx_HAL.h"

#include "DIOStreamI2C.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSTM32FXXXSTREAMI2C_MAXHANDLES   8
#define DIOSTM32FXXXSTREAMI2C_MAXPORTS     2

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class XTHREADCOLLECTED;


class DIOSTM32FXXXSTREAMI2CPORT 
{
	public:
                                            DIOSTM32FXXXSTREAMI2CPORT	  			  ()														{ Clean();																}
		virtual							                   ~DIOSTM32FXXXSTREAMI2CPORT 				  ()														{ Clean();																}	
	
		
		XDWORD													        GetCounterRef 											()														{ return counterref;											}
		void														        SetCounterRef												(XDWORD counterref)						{ this->counterref = counterref;				  }
	
		I2C_HandleTypeDef*							        GetHandleI2C												()														{ return hi2c;														}
		void														        SetHandleI2C											  (I2C_HandleTypeDef* hi2c)			{ this->hi2c = hi2c;											}

	private:

		void									                  Clean					      								()
                                            { 
                                              counterref        = 0;
                                              hi2c              = NULL;                                      
                                            } 

		XDWORD													        counterref;
		I2C_HandleTypeDef*							        hi2c;		
};




class DIOSTM32FXXXSTREAMI2C : public DIOSTREAMI2C
{
	public:
																						DIOSTM32FXXXSTREAMI2C				        ();
		virtual																 ~DIOSTM32FXXXSTREAMI2C				        ();
		
		DIOSTREAMSTATUS													GetConnectStatus							      ();		
      
		virtual bool										  			Open													      ();	
    
		bool																		WaitToFilledReadingBuffer			      (int filledto = DIOSTREAM_SOMETHINGTOREAD, int timeout = XTIMER_INFINITE);			
		
		XDWORD																	ReadDirect										      (XBYTE* buffer, XDWORD size);
		XDWORD																	WriteDirect										      (XBYTE* buffer, XDWORD size);
    
    XDWORD                                  Write                               (XBYTE* buffer, XDWORD size);

		bool																		Disconnect										      ()				  							    {	return false;												    }
		bool																		Close													      ();		

		bool																		CleanBuffers									      (); 											
   
    I2C_HandleTypeDef*											hi2c;
    XWORD																		localaddress;
    XWORD																		remoteaddress;
    XBYTE																		bufferdata[DIOSTREAM_MAXBUFFER];	
    XWORD																		sizebufferdata;
    XBUFFER                                 readcache[DIOSTM32FXXXSTREAMI2C_MAXPORTS];
    
    static DIOSTM32FXXXSTREAMI2CPORT*       ports[DIOSTM32FXXXSTREAMI2C_MAXPORTS];
    static DIOSTM32FXXXSTREAMI2C*						handles[DIOSTM32FXXXSTREAMI2C_MAXHANDLES]; 
    
    int																			indexhandle; 
    int																			indexport;
   
	protected:

    void																		Clean													      ()
																						{ 
																							hi2c              = NULL;
																							localaddress      = 0;
																							remoteaddress     = 0;                                 
																							threadconnexion   = NULL;
																							indexhandle       = -1;
																							indexport         = -1;
																							sizebufferdata		= 0; 
																						} 
		
		static void															ThreadConnexion								      (void* param); 
	
		XTHREADCOLLECTED*												threadconnexion;	
    XBYTE                                   writebuffer[DIOSTREAM_MAXBUFFER];
    
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif





