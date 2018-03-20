/*------------------------------------------------------------------------------------------
//	DIOLINUXGPIOGUMSTIXCONNEX.H
*/	
/**	
// \class 
//   
//  LINUX DIO port data GUMSTIX CONNEX class
//   
//	@author	 Abraham J. Velez
//	@version 25/07/2009 07:13:30 p.m.
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOLINUXGPIOGUMSTIXCONNEX_H_
#define _DIOLINUXGPIOGUMSTIXCONNEX_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOGPIO.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


// HW_GUMSTIXCONNEX
//--------------------------------------------
// GumStix
//
#define GSTX_MAP_SIZE										4096
#define GSTX_MAP_MASK										(GSTX_MAP_SIZE - 1 )

#define GSTX_GPLR0											0x40E00000
#define GSTX_GPDR0											0x40E0000C
#define GSTX_GPSR0											0x40E00018
#define GSTX_GPCR0											0x40E00024
#define GSTX_GAFR0_L										0x40E00054

#define GSTX_IN													250
#define GSTX_OUT												251
#define GSTX_GPIO												0

#define GSTX_SET												252
#define GSTX_CLEAR											253


/*---- CLASS -----------------------------------------------------------------------------*/
	
class DIOLINUXGPIOGUMSTIXCONNEX : public DIOGPIO
{
	public:
													DIOLINUXGPIOGUMSTIXCONNEX			();
		virtual	  				 	 ~DIOLINUXGPIOGUMSTIXCONNEX			();

		bool									Ini																(XPATH* xpath = NULL);

		bool									SetMode														(XBYTE pin, bool isinput, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		bool									Get																(XBYTE pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		bool									Set																(XBYTE pin, bool active, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		
		bool									End																();

	private:

		void									Clean															()
													{														
														GSTX_fd							= -1;
														GSTX_map						= NULL;
														GSTX_regaddr				= NULL;														
													}
   
		bool									GSTX_Ini													();
		bool									GSTX_End													();
		void									GSTX_PutMem												(XDWORD addr, XDWORD val);
		int										GSTX_GetMem												(XDWORD addr);
		void									GSTX_GPIO_Set											(XDWORD gpio);
		void									GSTX_GPIO_Clear										(XDWORD gpio);
		XDWORD								GSTX_GPIO_Status									(XDWORD gpio);
		void									GSTX_GPIO_Direction								(XDWORD gpio, XDWORD dir);
		void									GSTX_GPIO_Function								(XDWORD gpio, XDWORD fun);
		XDWORD								GSTX_GP_IO												(XDWORD dir, XDWORD set, XDWORD reg);

		int										GSTX_fd;
		void*									GSTX_map;
		void*									GSTX_regaddr;
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

