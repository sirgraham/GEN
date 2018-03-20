//------------------------------------------------------------------------------------------
//	XWINDOWSSYSTEM.CPP
//	
//	WINDOWS system class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 03/03/2004 12:28:40
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------

	
//---- INCLUDES ----------------------------------------------------------------------------

#include <cstdio>
#include <windows.h>
#ifndef BUILDER
#include <versionhelpers.h>
#endif
#include <tlhelp32.h>
#include <process.h>
#include <errno.h>

#include "XFactory.h"
#include "XFile.h"
#include "XDebug.h"

#include "XWINDOWSSystem.h"

#include "XMemory.h"



//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XWINDOWSSYSTEM::XWINDOWSSYSTEM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 12:28:59
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XWINDOWSSYSTEM::XWINDOWSSYSTEM() : XSYSTEM()
{

}



//-------------------------------------------------------------------
//  XWINDOWSSYSTEM::~XWINDOWSSYSTEM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 12:29:20
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XWINDOWSSYSTEM::~XWINDOWSSYSTEM()
{

}


/*-------------------------------------------------------------------
//	XWINDOWSSYSTEM::GetTypeHardware
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			21/04/2014 20:33:03
//	
//	@return 			XSYSTEM_HARDWARE : 
//
//  @param				revision : 
*/
/*-----------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XWINDOWSSYSTEM::GetTypeHardware(int* revision)
{
	if(revision) (*revision) = -1;

	return XSYSTEM_HARDWARETYPE_PC;
}



//-------------------------------------------------------------------
//  XWINDOWSSYSTEM::GetSO
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 12:29:40
//	
//	@return				XWINDOWSSYSTEM_SO : 
//	*/
//-------------------------------------------------------------------
XSYSTEM_SO XWINDOWSSYSTEM::GetSO()
{
	/*
	OSVERSIONINFO osvi;
	
	osvi.dwOSVersionInfoSize =sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	switch(osvi.dwPlatformId)
		{
			case VER_PLATFORM_WIN32s				: return XSYSTEM_SO_WINDOWS;
			case VER_PLATFORM_WIN32_WINDOWS : return XSYSTEM_SO_WINDOWS98;			
			case VER_PLATFORM_WIN32_NT			: return XSYSTEM_SO_WINDOWSNT;
		}
	*/

  #ifndef BUILDER
	OSVERSIONINFOEXW osvi;
	DWORDLONG        const dwlConditionMask = VerSetConditionMask(0, VER_PLATFORMID, VER_GREATER_EQUAL);

	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwPlatformId				 = _WIN32_WINNT_WIN10;

	VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);


	switch(osvi.dwPlatformId)
		{
			case _WIN32_WINNT_NT4           : return XSYSTEM_SO_WINDOWSNT4;
			case _WIN32_WINNT_WIN2K         : return XSYSTEM_SO_WINDOWS2000;
			case _WIN32_WINNT_WINXP         : return XSYSTEM_SO_WINDOWSXP;
			case _WIN32_WINNT_WS03          :	return XSYSTEM_SO_WINDOWSSERVER2003;
			case _WIN32_WINNT_VISTA         : return XSYSTEM_SO_WINDOWSVISTA;
			case _WIN32_WINNT_WIN7          : return XSYSTEM_SO_WINDOWS7;
			case _WIN32_WINNT_WIN8          : return XSYSTEM_SO_WINDOWS8;
			case _WIN32_WINNT_WINBLUE       : return XSYSTEM_SO_WINDOWS81;
			case _WIN32_WINNT_WIN10		      : return XSYSTEM_SO_WINDOWS10;
		}
  #endif
	
  return XSYSTEM_SO_WINDOWS;
}





//-------------------------------------------------------------------
//  XWINDOWSSYSTEM::GetLanguage
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 12:29:55
//	
//	@return				XLANGUAGE_CODE : 
//	*/
//-------------------------------------------------------------------
XLANGUAGE_CODE XWINDOWSSYSTEM::GetLanguage()
{
	switch(PRIMARYLANGID(GetUserDefaultLangID()))
		{
			case LANG_ENGLISH			: return XLANGUAGE_ENGLISH;
			case LANG_SPANISH			: return XLANGUAGE_SPANISH;			
			case LANG_BASQUE			: return XLANGUAGE_BASQUE;	 
			case LANG_CATALAN			: return XLANGUAGE_CATALAN;	 
			case LANG_GALICIAN		: return XLANGUAGE_GALICIAN;	  
			case LANG_FRENCH			: return XLANGUAGE_FRENCH;
			case LANG_GERMAN			: return XLANGUAGE_GERMAN;
			case LANG_ITALIAN			: return XLANGUAGE_ITALIAN;
			case LANG_PORTUGUESE	: return XLANGUAGE_PORTUGUESE;
			case LANG_RUSSIAN			: return XLANGUAGE_RUSSIAN;	 
			case LANG_CZECH				:	return XLANGUAGE_CZECH;
			case LANG_DANISH			: return XLANGUAGE_DANISH;
			case LANG_FINNISH			: return XLANGUAGE_FINNISH;
			case LANG_NORWEGIAN		: return XLANGUAGE_NORWEGIAN;
			case LANG_SLOVAK			: return XLANGUAGE_SLOVAK;
			case LANG_TURKISH			: return XLANGUAGE_TURKISH;
		}
  
	return XLANGUAGE_UNKNOWN;
}



/*-------------------------------------------------------------------
//  XWINDOWSSYSTEM::GetMemoryInfo
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      18/09/2012 8:58:22
//  
//  @return       bool : 
//  @param        total : (in Kb)
//  @param        free :  (in Kb)
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
{
  MEMORYSTATUS mem;

	GlobalMemoryStatus(&mem);

  total = (XDWORD)(mem.dwTotalPhys/1024);
  free  = (XDWORD)(mem.dwAvailPhys/1024);

  return true;
}



/*-------------------------------------------------------------------
//	XWINDOWSSYSTEM::MakeCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/11/2014 20:32:22
//	
//	@return 			bool : 
//
//  @param				command : 
//  @param				waitexit : 
//  @param				NULL : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSSYSTEM::MakeCommand(XCHAR* command, int* returncode)
{
	int status;

	status = _wsystem(command);	
	if(status	== -1) return false;
	if(status)
		{			
			switch(errno)
				{ 
					case E2BIG		:
					case ENOENT		:
					case ENOEXEC	:
					case ENOMEM		: return false;
				}
		}

	if(returncode) (*returncode) = status;
   
	return true;
}



/*-------------------------------------------------------------------
//	XWINDOWSSYSTEM::ExecuteApplication
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/08/2015 15:58:02
//	
//	@return 			bool : 
//
//  @param				command : 
//  @param				params : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSSYSTEM::ExecuteApplication(XCHAR* command, XCHAR* params, bool screen)
{ 	
	bool exist = false;

	XFILE* xfile = xfactory->Create_File();
	if(xfile)
		{
			exist = xfile->Open(command);
			xfile->Close();
		}

	xfactory->Delete_File(xfile);
	
	if(!exist) 
		return false;
	
	 _wspawnl(_P_NOWAIT , command, command, params, NULL);

	return true;												
}	




/*-------------------------------------------------------------------
//	 XWINDOWSSYSTEM::IsApplicationRunning
*/
/**
//
//	
//
//	@author		Abraham J. Velez 
//	@version		
//
//	@return		bool : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
bool XWINDOWSSYSTEM::IsApplicationRunning(XCHAR* command, XDWORD* ID)
{	
	PROCESSENTRY32 entry;	 
	HANDLE				 snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	bool					 exists = false;
    
	entry.dwSize = sizeof(PROCESSENTRY32);

  if(Process32First(snapshot, &entry))
		{
      while(Process32Next(snapshot, &entry))
				{
					XSTRING nameapp  = entry.szExeFile;
					XSTRING _command = command;

					if(_command.Find(nameapp, true) != XSTRING_NOTFOUND)
			      {								
							if(ID) (*ID) = entry.th32ProcessID;
							exists = true;
						}
				}
		}

  CloseHandle(snapshot);
 
	return exists;
}




/*-------------------------------------------------------------------
//  XWINDOWSSYSTEM::ShutDown
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/12/2013 13:47:24
//	
//	@return 			bool : 
//	@param				type : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
{	
	if(type == XSYSTEM_SHUTDOWNTYPE_UNKNOWN) return false;

	HANDLE						hToken; 
	TOKEN_PRIVILEGES	tkp; 
 
	// Get a token for this process.  
  if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))  return false; 
 
  // Get the LUID for the shutdown privilege.  
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
 
  tkp.PrivilegeCount						= 1;  // one privilege to set    
  tkp.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED; 
 
  // Get the shutdown privilege for this process.  
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);  
  if(GetLastError() != ERROR_SUCCESS) return false; 

	switch(type)
		{
			case XSYSTEM_SHUTDOWNTYPE_REBOOT	 :	if(!ExitWindowsEx(EWX_REBOOT	 | EWX_FORCE,	SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false; 
																						break;

			case XSYSTEM_SHUTDOWNTYPE_LOGOFF	 :	if(!ExitWindowsEx(EWX_LOGOFF	 | EWX_FORCE,	SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false; 
																						break;

			case XSYSTEM_SHUTDOWNTYPE_POWEROFF :	if(!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE,	SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false; 
																						break;
		}
	
	return true;
	
}

