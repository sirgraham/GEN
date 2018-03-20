
/*------------------------------------------------------------------------------------------
//	INPANDROIDFACTORY.CPP
//	
//	android input factory
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 06/09/2016 11:54:23
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "INPANDROIDFactory.h"
#include "INPANDROIDDeviceKeyboard.h"
#include "INPANDROIDDeviceMouse.h"
#include "GRPScreen.h"
#include "XDebug.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	INPANDROIDFACTORY::CreateDevice
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/09/2016 11:55:08
//	
//	@return 			INPDEVICE* : 
//
//  @param				device : 
//  @param				screenpointer : 
*/
/*-----------------------------------------------------------------*/
INPDEVICE* INPANDROIDFACTORY::CreateDevice(XCHAR* device,void* screenpointer)
{
	if (XSTRING::Compare(device,__L("keyboard"))==0)
	{
		INPANDROIDDEVICEKEYBOARD* keyboard=new INPANDROIDDEVICEKEYBOARD();
		return keyboard;
	}

	if (XSTRING::Compare(device,__L("mouse"))==0 || XSTRING::Compare(device,__L("touchscreen"))==0)
	{
		INPANDROIDDEVICEMOUSE* mouse=new INPANDROIDDEVICEMOUSE((GRPANDROIDSCREEN*)screenpointer);
		return mouse;
	}
		
	XDEBUG_PRINTCOLOR(4,__L("INPANDROIDFACTORY::CreateDevice : unrecognized input type"));
	return NULL;
}
