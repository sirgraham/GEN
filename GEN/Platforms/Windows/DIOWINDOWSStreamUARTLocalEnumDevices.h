//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMUARTLOCALENUMDEVICES.H
//	
/**	
// \class 
//   
//  WINDOWS Data IO Stream UART Local Enum Devices class
//   
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOWINDOWSSTREAMUARTLOCALENUMDEVICES_H_
#define _DIOWINDOWSSTREAMUARTLOCALENUMDEVICES_H_
	
#if defined(DIO_ACTIVE) && defined(DIOUART_ACTIVE)	

//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

#include "DIOStreamUARTLocalEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------


class DIOWINDOWSSTREAMUARTLOCALENUMDEVICES :  public DIOSTREAMUARTLOCALENUMDEVICES
{
	public:
													DIOWINDOWSSTREAMUARTLOCALENUMDEVICES					();
		virtual							 ~DIOWINDOWSSTREAMUARTLOCALENUMDEVICES					();

		bool									Search																				();		
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif



