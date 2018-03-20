/*------------------------------------------------------------------------------------------
//	DIOWINDOWSGPIO.H
*/	
/**	
// \class 
//   
//  WINDOWS DIO port class
//   
//	@author	 Abraham J. Velez
//	@version 25/07/2009 07:13:30 p.m.
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOWINDOWSGPIO_H_
#define _DIOWINDOWSGPIO_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "windows.h"
#include "stdio.h"

#include "DIOGPIO.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#ifdef HW_PC

//--------------------------------------------
// PC
//
#define PC_PORT									                         0x378

typedef void	(__stdcall* DIOWGPIOOUT)               (short, short);
typedef short	(__stdcall* DIOWGPIOINP)               (short);
typedef BOOL	(__stdcall* DIOWGPIOISDRIVEROPEN)      (void);
typedef BOOL	(__stdcall* DIOWGPIOISDRIVER64BITS)    (void);

#endif

	
/*---- CLASS -----------------------------------------------------------------------------*/
	
class DIOWINDOWSGPIO : public DIOGPIO
{
	public:
										            DIOWINDOWSGPIO								();
		virtual	  			           ~DIOWINDOWSGPIO								();

    bool						            Ini													  (XPATH* xpath);

		bool						            SetMode											  (XBYTE pin, bool isinput, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		bool						            Get													  (XBYTE pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		bool						            Set													  (XBYTE pin, bool active, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		
		bool						            End													  ();

	private:

		void						            Clean												  ();	


    #ifdef HW_PC
		//--------------------------------------------
		// PC
		//		
		bool						            PC_Get												(XDWORD nport, XBYTE& data);
		bool						            PC_Set												(XDWORD nport, XBYTE data);

    HINSTANCE                   handleDLL;
    DIOWGPIOOUT									OutPort;
    DIOWGPIOINP									InpPort;
    DIOWGPIOISDRIVEROPEN				IsDriverOpen;
    DIOWGPIOISDRIVER64BITS			IsDriver64Bit;

		XBYTE                       dataport;
    
    #endif

};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

