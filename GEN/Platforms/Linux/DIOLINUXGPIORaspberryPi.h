/*------------------------------------------------------------------------------------------
//	DIOLINUXGPIORASPBERRYPI.H
*/	
/**	
// \class 
//   
//  LINUX DIO port data class
//   
//	@author	 Abraham J. Velez
//	@version 25/07/2009 07:13:30 p.m.
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOLINUXGPIORASPBERRYPI_H_
#define _DIOLINUXGPIORASPBERRYPI_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOGPIO.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



// HW_RASPBERRYPI
//--------------------------------------------
// RaspBerry Pi
//

enum RASPBERRYPI_MODEL
{
	RASPBERRYPI_MODEL_UNKNOWN						=   0 ,	
	RASPBERRYPI_MODEL_A												,
	RASPBERRYPI_MODEL_B												,
	RASPBERRYPI_MODEL_A_PLUS									,
	RASPBERRYPI_MODEL_B_PLUS									,
	RASPBERRYPI_MODEL_COMPUTERMODULE					,
	RASPBERRYPI_MODEL_B_2											,
	RASPBERRYPI_MODEL_ZERO										,
	RASPBERRYPI_MODEL_B_3											,
};


#define RPI_BCM2708_PERI_BASE						0x20000000		 //Rapsberry Pi A+, B+    
#define RPI_BCM2708_PERI_BASE_2					0x3F000000     //Rapsberry Pi 2    0x20000000
#define	RPI_PAGE_SIZE										(4*1024)
#define	RPI_BLOCK_SIZE									(4*1024)


/*---- CLASS -----------------------------------------------------------------------------*/
	
class DIOLINUXGPIORASPBERRYPI : public DIOGPIO
{
	public:
													DIOLINUXGPIORASPBERRYPI	();
		virtual	  				 	 ~DIOLINUXGPIORASPBERRYPI	();

		bool									Ini													(XPATH* xpath = NULL);

		bool									SetMode											(XBYTE pin, bool isinput, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		bool									Get													(XBYTE pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		bool									Set													(XBYTE pin, bool active, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		
		bool									End													();

	private:

		void									Clean												()
													{														
														RPI_model						= RASPBERRYPI_MODEL_UNKNOWN;
														RPI_megabytes				= 0;
														RPI_revision				= 0.0f;
														RPI_gpio_base				= 0;														
													}
    
		bool									RPI_RevisionBoard						(RASPBERRYPI_MODEL& model, int& megabytes, float& revision);
		bool									RPI_Ini											();
		bool									RPI_End											();
		bool									RPI_IsGPIOValid							(int GPIO);
		bool									RPI_GPIOMode								(int GPIO, bool isinput);
		bool									RPI_GPIORead								(int GPIO);
		bool									RPI_GPIOWrite								(int GPIO, bool isactive);

		RASPBERRYPI_MODEL			RPI_model;
		int										RPI_megabytes;
		float									RPI_revision;
		XDWORD								RPI_gpio_base;    
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

