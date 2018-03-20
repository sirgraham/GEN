//------------------------------------------------------------------------------------------
//	DIOWINDOWSSTREAMIPLOCALENUMDEVICES.H
//
/**
// \class
//
//  WINDOWS Data IO Stream IP Local Enum Devices class
//
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMIPLOCALENUMDEVICES_H_
#define _DIOWINDOWSSTREAMIPLOCALENUMDEVICES_H_


//---- INCLUDES ----------------------------------------------------------------------------

/*
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

#include "XFSMachine.h"
#include "XWINDOWSThread.h"


#include "DIOStreamTCPIP.h"
*/

#include "DIOStreamIPLocalEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;


class DIOWINDOWSSTREAMIPLOCALENUMDEVICES :  public DIOSTREAMIPLOCALENUMDEVICES
{
	public:
															DIOWINDOWSSTREAMIPLOCALENUMDEVICES			();
		virtual									 ~DIOWINDOWSSTREAMIPLOCALENUMDEVICES			();

		bool											Search																	();											   
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

