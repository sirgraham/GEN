//------------------------------------------------------------------------------------------
//	INPLINUXDEVICEKEYBOARDDIRECT.H
//	
/**	
// \class 
//   
//  Input LINUX device keyboard direct class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 11/02/2003
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _INPLINUXDEVICEKEYBOARDDIRECT_H_
#define _INPLINUXDEVICEKEYBOARDDIRECT_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include <termios.h>

#include "XVector.h"

#include "INPButton.h"
#include "INPDevice.h"	

#include "INPLINUXDeviceID.h"
	

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;


class INPLINUXDEVICEKEYBOARDDIRECT : public INPDEVICE
{
	public:
																INPLINUXDEVICEKEYBOARDDIRECT			();
		virtual										 ~INPLINUXDEVICEKEYBOARDDIRECT			();

		virtual	bool								SetScreen													(void* screenpointer);
		
		XVECTOR<INPBUTTON*>*				GetButtons												()										{	return &buttons;						}
		
		bool												Release														();		
		bool												Update														();
		
	private:

		void												Clean															();

		bool												CreateAllButtons									();
		bool												DeleteAllButtons									();

		bool												GetHandleEvent										();
		
		int													eventnumber;
				
		XVECTOR<INPBUTTON*>					buttons;					
		XVECTOR<INPLINUXDEVICEID*>	keyboards;

		struct termios							termiosinitialsettings;
};
	
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif
