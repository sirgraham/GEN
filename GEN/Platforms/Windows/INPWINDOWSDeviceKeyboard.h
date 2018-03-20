//------------------------------------------------------------------------------------------
//	INPWINDOWSDEVICEKEYBOARD.H
//	
/**	
// \class 
//   
//  WINDOWS INPUT Device Keyboard class
//   
//	@author	 Abraham J. Velez
//	@version 11/02/2003
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _INPWINDOWSDEVICEKEYBOARD_H_
#define _INPWINDOWSDEVICEKEYBOARD_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>

#include "XBase.h"
#include "XMap.h"

#include "INPButton.h"
#include "INPDevice.h"	

#include "GRPWINDOWSScreen.h"

#include "GRPBufferPixelFormatRGB.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;


class INPWINDOWSDEVICEKEYBOARD : public INPDEVICE
{
	public:
													INPWINDOWSDEVICEKEYBOARD			( GRPWINDOWSSCREEN* grpscreen);
		virtual							 ~INPWINDOWSDEVICEKEYBOARD			();

		virtual	bool					SetScreen											(void* screenpointer);
		XVECTOR<INPBUTTON*>*	GetButtons										();

		bool									Release												();		
		bool									Update												();	

	private:

		void									Clean													();
		
		bool 									CreateAllButtons							();
		bool									DeleteAllButtons							();
		
		XVECTOR<INPBUTTON*>		buttons;	
		GRPWINDOWSSCREEN*			grpscreen;
};
	
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

