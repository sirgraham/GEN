/*------------------------------------------------------------------------------------------
//	WINDOWSMAIN.H
*/	
/**	
// \class 
//   
//  sdfds
//   
//	@author	 Abraham J. Velez
//	@version 04/12/2013 11:25:00
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _WINDOWSMAIN_H_
#define _WINDOWSMAIN_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <windows.h>

#include "WINDOWSStackWalker.h"

#ifdef GRP_ACTIVE
#include "GRPWindowsScreen.h"
#endif

#include "XApplication.h"
#include "XSystem.h"

#include "XWINDOWSDebug.h"

#include "Main.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

	
/*---- CLASS -----------------------------------------------------------------------------*/

class XPATHS;
class XSTRING;
class XFACTORY;
class XWINDOWSFACTORY;
class XWINDOWSSYSTEM;
class XAPPLICATION;
class GRPWINDOWSFACTORY;
class SNDWINDOWSFACTORY;
class INPMANAGER;
class INPWINDOWSDEVICEKEYBOARD;
class INPWINDOWSDEVICEMOUSE;
class INPWINDOWSHID;
class DIOFACTORY;
class DIOWINDOWSFACTORY;

	
class WINDOWSMAIN : public MAIN
{
	public:
																		WINDOWSMAIN						();
		virtual												 ~WINDOWSMAIN						();

		bool														Ini										(XSTRING* xpath = NULL);
				
		bool														Update								();

		bool														End										();
		    
		XAPPLICATION*										GetXApplication				();	

		bool														IsAppClose						()												{	return isappclose;								}
		void														SetIsAppClose					(bool isappclose)					{	this->isappclose = isappclose;		}

		XPATH*													GetXPathExec					()												{ return &xpathexec;								}
		
		bool														GetParams							(int nparams, char* param[]);
		bool														GetParams							(XSTRING& commandline);
		
	private:

		void														Clean									();

		XWINDOWSSYSTEM*									xsystem;
		XPATH														xpathexec;
		XAPPLICATION*	 	 								xapplication;		
		XAPPLICATIONEXITTYPE						exittype;
		XSYSTEM_SHUTDOWNTYPE						shutdowntype;
		
		bool														isappclose;
};
	

class WINDOWSMAINSTACKWALKER : public StackWalker
{
	public:

		virtual void										OnSymInit							(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
		virtual void										OnLoadModule					(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion)   {    }
		virtual void										OnCallstackEntry			(CallstackEntryType eType, CallstackEntry &entry);
		virtual void										OnDbgHelpErr					(LPCSTR szFuncName, DWORD gle, DWORD64 addr)																																									{    }	
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	

extern int					Exception_Filter							(XDWORD code, struct _EXCEPTION_POINTERS *ep);
extern bool					Exception_Printf							(bool iserror, XCHAR* title, XCHAR* mask, ...);
extern BOOL					Exception_ConsoleHandler			(DWORD fdwctrltype);
extern bool					Exception_ShutDown						(XSYSTEM_SHUTDOWNTYPE type);


#endif


