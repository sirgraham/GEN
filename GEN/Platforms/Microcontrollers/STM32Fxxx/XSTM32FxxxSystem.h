//	XSTM32FXXXSYSTEM.H
//	
/**	
// \class 
//   
//  STM32Fxxx system class
//   
//	@author	 Abraham J. Velez
//	@version 03/03/2004 12:15:55
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XSTM32FXXXSYSTEM_H_
#define _XSTM32FXXXSYSTEM_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSystem.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	

//---- CLASS -------------------------------------------------------------------------------


class XSTM32FXXXSYSTEM : public XSYSTEM
{
	public:
														XSTM32FXXXSYSTEM						();
		virtual								 ~XSTM32FXXXSYSTEM						();

		XSYSTEM_HARDWARETYPE		GetTypeHardware							(int* revision = NULL);		
    XSYSTEM_SO							GetSO												();		
    XLANGUAGE_CODE					GetLanguage									();		
    bool    								GetMemoryInfo							  (XDWORD& total,XDWORD& free);	
		
		bool										MakeCommand									(XCHAR* command, int* returncode = NULL);

		bool										ExecuteApplication					(XCHAR* command, XCHAR* params, bool specialexecution = false);

		bool										ShutDown										(XSYSTEM_SHUTDOWNTYPE type);		
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

