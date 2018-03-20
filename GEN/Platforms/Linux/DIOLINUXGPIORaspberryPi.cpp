/*------------------------------------------------------------------------------------------
//	DIOLINUXGPIORASPBERRYPI.CPP
//	
//	LINUX DIO port data Raspberry pi class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 25/07/2009 07:14:17 p.m.
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>	
#include <stdint.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "XDebug.h"
#include "XFileTXT.h"

#include "DIOLINUXGPIORaspberryPi.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

#ifdef HW_RASPBERRYPI   	
static volatile uint32_t* RPI_gpio;
#endif

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::DIOLINUXGPIORASPBERRYPI
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:18:20 p.m.
//	
//	@return				
*/
/*-----------------------------------------------------------------*/
DIOLINUXGPIORASPBERRYPI::DIOLINUXGPIORASPBERRYPI() : DIOGPIO()
{
	Clean();
}


/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::~DIOLINUXGPIORASPBERRYPI
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:18:44 p.m.
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOLINUXGPIORASPBERRYPI::~DIOLINUXGPIORASPBERRYPI()
{
	Clean();
}



/*-------------------------------------------------------------------
//	DIOLINUXGPIORASPBERRYPI::Ini
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/10/2015 14:25:06
//	
//	@return 			bool : 
//
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::Ini(XPATH* xpath)
{	  
	if(!RPI_RevisionBoard(RPI_model, RPI_megabytes, RPI_revision)) return false;

	switch(RPI_model)
		{
			case RASPBERRYPI_MODEL_UNKNOWN				:
			case RASPBERRYPI_MODEL_A							:
			case RASPBERRYPI_MODEL_B							:
			case RASPBERRYPI_MODEL_A_PLUS					:
			case RASPBERRYPI_MODEL_B_PLUS					:
			case RASPBERRYPI_MODEL_COMPUTERMODULE	:	
			case RASPBERRYPI_MODEL_ZERO						: RPI_gpio_base = (RPI_BCM2708_PERI_BASE + 0x200000);
																							break;

			case RASPBERRYPI_MODEL_B_2						:		
			case RASPBERRYPI_MODEL_B_3						: RPI_gpio_base = (RPI_BCM2708_PERI_BASE_2 + 0x200000);
																							break;		
		}

	/* 1*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 1] = DIOGPIO_PINNOTUSE;					/* Not available (3v3 Power) */        /* 2*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 2] = DIOGPIO_PINNOTUSE;						/* Not available (5v  Power) */
	/* 3*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 3] =  2;													/* I2C (SDA)                 */        /* 4*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 4] = DIOGPIO_PINNOTUSE;						/* Not available (5v  Power) */
	/* 5*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 5] =  3;													/* I2C (SDL)                 */        /* 6*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 6] = DIOGPIO_PINNOTUSE;						/* Not available (Ground)    */
	/* 7*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 7] =  4;													/* GPCLK0                    */        /* 8*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 8] = 14;													/* UART TXD                  */
	/* 9*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 9] = DIOGPIO_PINNOTUSE;					/* Not available (Ground)    */        /*10*/ GPIOvalue[DIOGPIO_PINSGROUP_A][10] = 15;													/* UART RXD                  */
	/*11*/ GPIOvalue[DIOGPIO_PINSGROUP_A][11] = 17;													/* Generic I/O               */        /*12*/ GPIOvalue[DIOGPIO_PINSGROUP_A][12] = 18;													/* PCM_CLK                   */  
	/*13*/ GPIOvalue[DIOGPIO_PINSGROUP_A][13] = 27;													/* PCM_DOUT                  */        /*14*/ GPIOvalue[DIOGPIO_PINSGROUP_A][14] = DIOGPIO_PINNOTUSE;						/* Not available (Ground)    */
	/*15*/ GPIOvalue[DIOGPIO_PINSGROUP_A][15] = 22;													/* Generic I/O               */        /*16*/ GPIOvalue[DIOGPIO_PINSGROUP_A][16] = 23;													/* Generic I/O               */
	/*17*/ GPIOvalue[DIOGPIO_PINSGROUP_A][17] = DIOGPIO_PINNOTUSE;					/* Not available (3v3 Power) */        /*18*/ GPIOvalue[DIOGPIO_PINSGROUP_A][18] = 24;													/* Generic I/O               */
	/*19*/ GPIOvalue[DIOGPIO_PINSGROUP_A][19] = 10;													/* SPI MOSI                  */        /*20*/ GPIOvalue[DIOGPIO_PINSGROUP_A][20] = DIOGPIO_PINNOTUSE;						/* Not available (Ground)    */ 
	/*21*/ GPIOvalue[DIOGPIO_PINSGROUP_A][21] =  9;													/* SPI MISO                  */        /*22*/ GPIOvalue[DIOGPIO_PINSGROUP_A][22] = 25;													/* Generic I/O               */ 
  /*23*/ GPIOvalue[DIOGPIO_PINSGROUP_A][23] = 11;													/* SPI SCLK                  */        /*24*/ GPIOvalue[DIOGPIO_PINSGROUP_A][24] =  8;													/* SPI CE0                   */ 
  /*25*/ GPIOvalue[DIOGPIO_PINSGROUP_A][25] = DIOGPIO_PINNOTUSE;					/* Not available             */        /*26*/ GPIOvalue[DIOGPIO_PINSGROUP_A][26] =  7;													/* SPI CE1                   */ 
  
	// --------------Extended PI2 -------------------------------------------------------

	if((RPI_model == RASPBERRYPI_MODEL_A_PLUS)					||  (RPI_model == RASPBERRYPI_MODEL_B_PLUS)	||	
		 (RPI_model == RASPBERRYPI_MODEL_COMPUTERMODULE)  ||  (RPI_model == RASPBERRYPI_MODEL_B_2)    ||
		 (RPI_model == RASPBERRYPI_MODEL_ZERO)						||		
		 (RPI_model == RASPBERRYPI_MODEL_B_3))							
		{
			/*27*/ GPIOvalue[DIOGPIO_PINSGROUP_A][27] = DIOGPIO_PINNOTUSE;			/* I2C ID EEProm SC		       */        /*28*/ GPIOvalue[DIOGPIO_PINSGROUP_A][28] = DIOGPIO_PINNOTUSE;						/* I2C ID EEProm SD          */
			/*29*/ GPIOvalue[DIOGPIO_PINSGROUP_A][29] =  5;											/* Generic I/O	             */        /*30*/ GPIOvalue[DIOGPIO_PINSGROUP_A][30] = DIOGPIO_PINNOTUSE;						/* Not available  (Ground)   */ 
			/*31*/ GPIOvalue[DIOGPIO_PINSGROUP_A][31] =  6;											/* Generic I/O               */        /*32*/ GPIOvalue[DIOGPIO_PINSGROUP_A][32] = 12;													/* Generic I/O               */ 
			/*33*/ GPIOvalue[DIOGPIO_PINSGROUP_A][33] = 13;											/* Generic I/O               */        /*34*/ GPIOvalue[DIOGPIO_PINSGROUP_A][34] = DIOGPIO_PINNOTUSE;						/* Not available (Ground)		 */ 
			/*35*/ GPIOvalue[DIOGPIO_PINSGROUP_A][35] = 19;											/* Generic I/O			         */        /*36*/ GPIOvalue[DIOGPIO_PINSGROUP_A][36] = 16;													/* Generic I/O               */ 
			/*37*/ GPIOvalue[DIOGPIO_PINSGROUP_A][37] = 26;											/* Generic I/O		           */        /*38*/ GPIOvalue[DIOGPIO_PINSGROUP_A][38] = 20;													/* Generic I/O               */
			/*39*/ GPIOvalue[DIOGPIO_PINSGROUP_A][39] = DIOGPIO_PINNOTUSE;			/* Not available (Ground)		 */        /*40*/ GPIOvalue[DIOGPIO_PINSGROUP_A][40] = 21;													/* Generic I/O	             */ 
		}
	
	return RPI_Ini();																  
}




/*-------------------------------------------------------------------
//	DIOLINUXGPIORASPBERRYPI::SetMode
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 16:22:26
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				isinput : 
//  @param				group : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::SetMode(XBYTE pin,bool isinput, DIOGPIO_PINSGROUP group)
{
  return RPI_GPIOMode(GPIOvalue[DIOGPIO_PINSGROUP_A][pin], isinput);  
}



/*-------------------------------------------------------------------
//	DIOLINUXGPIORASPBERRYPI::Get
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 16:22:47
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				group : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::Get(XBYTE pin, DIOGPIO_PINSGROUP group)
{
  return RPI_GPIORead(GPIOvalue[DIOGPIO_PINSGROUP_A][pin]);
}



/*-------------------------------------------------------------------
//	DIOLINUXGPIORASPBERRYPI::Set
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 16:23:17
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				active : 
//  @param				group : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::Set(XBYTE pin, bool active, DIOGPIO_PINSGROUP group)
{
	return RPI_GPIOWrite(GPIOvalue[DIOGPIO_PINSGROUP_A][pin],active);																									
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 08:24:53 p.m.
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::End()
{	
	return RPI_End();
}




/*-------------------------------------------------------------------
//	DIOLINUXGPIORASPBERRYPI::RPI_RevisionBoard
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2016 20:28:54
//	
//	@return 			bool : 
//
//  @param				model : 
//  @param				megabytes : 
//  @param				revision : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::RPI_RevisionBoard(RASPBERRYPI_MODEL& model, int& megabytes, float& revision)
{
	XPATH			xpath;
	XFILETXT* xfiletxt;
	bool			status = false;

	xpath = __L("/proc/cpuinfo");

	//xpath = __L("D:\\GENFrameWork\\Test\\resources\\cpuinfo");

	xfiletxt = new XFILETXT();
	if(!xfiletxt) return false;

	if(xfiletxt->Open(xpath))
		{
			xfiletxt->ReadAllFile();

			for(int c=0;c< xfiletxt->GetNLines(); c++)
				{
					XSTRING* line = xfiletxt->GetLine(c);
					if(line)
						{
							if(line->Find(__L("Revision"), false) != XSTRING_NOTFOUND)
								{							
									int position = line->Find(__L(":"), true);
									if(position != XSTRING_NOTFOUND)
										{
											line->DeleteCharacters(0, position+1);
											line->DeleteCharacter(0x20);
											
											model = RASPBERRYPI_MODEL_UNKNOWN;

											if(!line->Compare(__L("0002")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 1.0f; }
											if(!line->Compare(__L("0003")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 1.0f; }
											if(!line->Compare(__L("0004")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0005")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0006")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0007")		, true))  { model = RASPBERRYPI_MODEL_A;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0008")		, true))  { model = RASPBERRYPI_MODEL_A;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("0009")		, true))  { model = RASPBERRYPI_MODEL_A;								megabytes = 256;			revision = 2.0f; }
											if(!line->Compare(__L("000d")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 512;			revision = 2.0f; }
											if(!line->Compare(__L("000e")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 512;			revision = 2.0f; }
											if(!line->Compare(__L("000f")		, true))  { model = RASPBERRYPI_MODEL_B;								megabytes = 512;			revision = 2.0f; }
											if(!line->Compare(__L("0010")		, true))  { model = RASPBERRYPI_MODEL_B_PLUS;						megabytes = 512;			revision = 1.0f; }
											if(!line->Compare(__L("0011")		, true))  { model = RASPBERRYPI_MODEL_COMPUTERMODULE; 	megabytes = 512;			revision = 1.0f; }
											if(!line->Compare(__L("0012")		, true))  { model = RASPBERRYPI_MODEL_A_PLUS;						megabytes = 256;			revision = 1.0f; }
											if(!line->Compare(__L("0013")		, true))  { model = RASPBERRYPI_MODEL_B_PLUS;						megabytes = 512;			revision = 1.2f; }
											if(!line->Compare(__L("a01041")	, true))  { model = RASPBERRYPI_MODEL_B_2;							megabytes = 1024;			revision = 1.1f; }
											if(!line->Compare(__L("a21041")	, true))  { model = RASPBERRYPI_MODEL_B_2;							megabytes = 1024;		  revision = 1.1f; }
											if(!line->Compare(__L("900092")	, true))  { model = RASPBERRYPI_MODEL_ZERO;							megabytes = 512;			revision = 1.2f; }																						
											if(!line->Compare(__L("900093") , true))  { model = RASPBERRYPI_MODEL_ZERO;							megabytes = 512;			revision = 1.2f; }																						
											if(!line->Compare(__L("a02082") , true))  { model = RASPBERRYPI_MODEL_B_3;	            megabytes = 1024;		  revision = 1.2f; } 
											if(!line->Compare(__L("a22082") , true))  { model = RASPBERRYPI_MODEL_B_3;	            megabytes = 1024;		  revision = 1.2f; } 	

											if(model != RASPBERRYPI_MODEL_UNKNOWN) 
												{
													status = true;
													break;
												}
										}
								}
						}
				}	

			xfiletxt->Close();
		}

	delete xfiletxt;

	return status;	
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::RPI_Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/06/2012 23:31:27
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::RPI_Ini()
{		
	int fd;
 
	if((fd = open ("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) return false;
	
	RPI_gpio = (uint32_t *)mmap(0, RPI_BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, RPI_gpio_base) ;
  if ((int32_t)RPI_gpio == -1) return false;

	return true;	
} 



/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::RPI_End
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      27/09/2012 14:46:54
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::RPI_End()
{  
	if ((int32_t)RPI_gpio == -1) return false;

  //fixme - set all gpios back to input
  //munmap((caddr_t)RPI_gpio, RPI_BLOCK_SIZE);

	return true;
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::RPI_IsGPIOValid
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/06/2012 21:01:51
//	
//	@return 			bool : 
//	@param				pin : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::RPI_IsGPIOValid(int GPIO)
{
	if(GPIO<2 || GPIO>27) return false;
	
	return true;	
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::RPI_GPIOMode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/06/2012 19:27:03
//	
//	@return 			bool : 
//	@param				pin : 
//  @param				input : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::RPI_GPIOMode(int GPIO, bool isinput)
{ 
	uint8_t gpiotoGPFsel[] = {	0,0,0,0,0,0,0,0,0,0,
															1,1,1,1,1,1,1,1,1,1,
															2,2,2,2,2,2,2,2,2,2,
															3,3,3,3,3,3,3,3,3,3,
															4,4,4,4,4,4,4,4,4,4,
															5,5,5,5,5,5,5,5,5,5,
													  };

	uint8_t gpiotoshift[]		=	{ 0,3,6,9,12,15,18,21,24,27,
															0,3,6,9,12,15,18,21,24,27,
															0,3,6,9,12,15,18,21,24,27,
															0,3,6,9,12,15,18,21,24,27,
															0,3,6,9,12,15,18,21,24,27,
														};
  int			gpiopin;
	int			fsel;
	int			shift;
	
	if((int32_t)RPI_gpio == -1) return false;
	if(!RPI_IsGPIOValid(GPIO))  return false;
  
  fsel    = gpiotoGPFsel[GPIO] ;
  shift   = gpiotoshift[GPIO] ;

  if(isinput) *(RPI_gpio+ fsel) = (*(RPI_gpio+ fsel) & ~(7 << shift)) ;								// Sets bits to zero = input
			  else  *(RPI_gpio+ fsel) = (*(RPI_gpio+ fsel) & ~(7 << shift)) | (1 << shift);


	//XDEBUG_PRINTCOLOR(1, __L("RPI Data Port Mode: GPIO %d -> %s "), GPIO, isinput?__L("input"):__L("output"));

	return true;  
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::RPI_GPIORead 
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/06/2012 23:09:26
//	
//	@return 			bool : 
//	@param				pin : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::RPI_GPIORead(int GPIO)
{
	uint8_t gpiotoGPLEV [] = {	13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
															14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
													 };
	
	if((int32_t)RPI_gpio == -1) return false;
  if(!RPI_IsGPIOValid(GPIO)) return false;

  if((*(RPI_gpio+ gpiotoGPLEV[GPIO]) & (1 << (GPIO & 31))) != 0) 
		{
			//XDEBUG_PRINTCOLOR(1, __L("RPI Data Port Read: GPIO %d"), GPIO);	
			return true;
		}
  
  return false;
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIORASPBERRYPI::RPI_GPIOWrite
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/06/2012 23:15:02
//	
//	@return 			bool : 
//	@param				pin : 
//  @param				isactive : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIORASPBERRYPI::RPI_GPIOWrite(int GPIO, bool isactive)
{
	uint8_t gpiotoGPSET [] = {  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
														  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
													 };


	uint8_t gpiotoGPCLR [] = { 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
														 11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
													 };


  if((int32_t)RPI_gpio == -1) return false;
	if(!RPI_IsGPIOValid(GPIO)) return false;

  if(isactive)   *(RPI_gpio+ gpiotoGPSET[GPIO]) = 1 << (GPIO & 31);
	 			  else	 *(RPI_gpio+ gpiotoGPCLR[GPIO]) = 1 << (GPIO & 31);

	//XDEBUG_PRINTCOLOR(1, __L("RPI Data Port Write: GPIO %d ->%s"), GPIO, isactive?__L("on"):__L("off"));	

	return true;
}


