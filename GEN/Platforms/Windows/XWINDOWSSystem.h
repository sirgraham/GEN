//------------------------------------------------------------------------------------------
//	XWINDOWSSYSTEM.H
//	
/**	
// \class 
//   
//  WINDOWS system class
//   
//	@author	 Abraham J. Velez
//	@version 03/03/2004 12:15:55
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XWINDOWSSYSTEM_H_
#define _XWINDOWSSYSTEM_H_
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSystem.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
#define MAX_KEY_LENGTH								 255
#define MAX_VALUE_NAME								 16383

#define XWINDOWSSYSTEM_MAXNETADDRIP    16

//---- CLASS -------------------------------------------------------------------------------

class XWINDOWSSYSTEM : public XSYSTEM
{
	public:
														XWINDOWSSYSTEM										();
		virtual								 ~XWINDOWSSYSTEM										();

		XSYSTEM_HARDWARETYPE		GetTypeHardware										(int* revision = NULL);		
    XSYSTEM_SO							GetSO															();		
    XLANGUAGE_CODE					GetLanguage												();
	  bool										GetMemoryInfo											(XDWORD& total,XDWORD& free);

		bool										MakeCommand												(XCHAR* command, int* returncode = NULL);
		
		bool										ExecuteApplication								(XCHAR* command, XCHAR* params, bool screen);

		bool										IsApplicationRunning							(XCHAR* command, XDWORD* ID = NULL);
														
		bool										ShutDown													(XSYSTEM_SHUTDOWNTYPE type);
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

