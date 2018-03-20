//------------------------------------------------------------------------------------------
//	XSYSTEM.H
//	
/**	
// \class 
//   
//  System Resources Class
//   
//	@author	 Abraham J. Velez
//	@version 03/03/2004 12:15:55
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XSYSTEM_H_
#define _XSYSTEM_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XLanguage.h"

	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
enum XSYSTEM_HARDWARETYPE
{
	XSYSTEM_HARDWARETYPE_UNKNOWN					 = 0	,		
	
	XSYSTEM_HARDWARETYPE_PC											,
	XSYSTEM_HARDWARETYPE_LEX3V700A							,	
	
	XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX					,	
	XSYSTEM_HARDWARETYPE_RASPBERRYPI            ,
	XSYSTEM_HARDWARETYPE_ODROIDC1		            ,
  XSYSTEM_HARDWARETYPE_ARTILA500							,
	XSYSTEM_HARDWARETYPE_ARMGENERIC             , 

	XSYSTEM_HARDWARETYPE_MICRO_STM32FXXX				,	

	XSYSTEM_HARDWARETYPE_END
};


enum XSYSTEM_SO
{
	XSYSTEM_SO_UNKNOWN								= 0x0000	,
	
	XSYSTEM_SO_WINDOWS								= 0x0100	,
	XSYSTEM_SO_WINDOWS95						 						,
	XSYSTEM_SO_WINDOWS98												,
	XSYSTEM_SO_WINDOWSNT4												,
	XSYSTEM_SO_WINDOWS2000											,
	XSYSTEM_SO_WINDOWSXP												,
	XSYSTEM_SO_WINDOWSSERVER2003								,
	XSYSTEM_SO_WINDOWSVISTA											,
	XSYSTEM_SO_WINDOWS7													,
	XSYSTEM_SO_WINDOWS8													,
	XSYSTEM_SO_WINDOWS81												,
	XSYSTEM_SO_WINDOWS10												,

	XSYSTEM_SO_LINUX									= 0x0200	,

	XSYSTEM_SO_LINUX_EMBEDDED					= 0x0400	,	

	XSYSTEM_SO_ANDROID								= 0x0800	,
};


enum XSYSTEM_SHUTDOWNTYPE
{
	XSYSTEM_SHUTDOWNTYPE_UNKNOWN			= 0				,
	XSYSTEM_SHUTDOWNTYPE_REBOOT									,
	XSYSTEM_SHUTDOWNTYPE_LOGOFF									,
	XSYSTEM_SHUTDOWNTYPE_POWEROFF								,
};


//---- CLASS -------------------------------------------------------------------------------


class XSYSTEM
{
	public:

																		XSYSTEM												();
		virtual												 ~XSYSTEM												();

		virtual XSYSTEM_HARDWARETYPE		GetTypeHardware								(int* revision = NULL)													{	return XSYSTEM_HARDWARETYPE_UNKNOWN;							}
    bool														HardwareUseLittleEndian				();	

		virtual	XSYSTEM_SO							GetSO													()																							{ return XSYSTEM_SO_UNKNOWN;												}			
		bool														CheckGetSO										(XSYSTEM_SO SOtoCheck);

		virtual	XLANGUAGE_CODE					GetLanguage										()																							{ return XLANGUAGE_UNKNOWN;											 		}
		
		virtual	bool    								GetMemoryInfo									(XDWORD& total,XDWORD& free)							
																		{  
																			total = 0;
																			free  = 0;
																			return false;												
																		}

		int															GetMemoryFreePercentAvailable		();

		
		virtual bool										MakeCommand										(XCHAR* command, int* returncode = NULL)				{ return false;																			}			
		bool														MakeCommand										(XSTRING& command, int* returncode = NULL)			{ return MakeCommand(command.Get(), returncode);		}	


		virtual bool										ExecuteApplication						(XCHAR* command, XCHAR* params = NULL, bool special = false)					
                                    { 
                                      return false;												
                                    }	

		virtual bool										IsApplicationRunning					(XCHAR* command, XDWORD* ID = NULL)							{	return false;																			}
		
		virtual bool										ShutDown											(XSYSTEM_SHUTDOWNTYPE type)											{ return false;																			}

	protected:

		void														Clean													()																							{																										}
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

