//------------------------------------------------------------------------------------------
//	XWINDOWSDEBUGCTRL.CPP
//	
//	WINDOWS Debug class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 05/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------

#ifdef XDEBUG	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>					
#include <iphlpapi.h>

#include <stdio.h>
#include <string.h>
#include <io.h>

#include "XBuffer.h"
#include "XString.h"
#include "XWINDOWSSystem.h"
#include "XWINDOWSDateTime.h"

#include "DIOWINDOWSURL.h"

#include "XWINDOWSDebug.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XWINDOWSDEBUGCTRL::XWINDOWSDEBUGCTRL
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:03:05
//	
//	@return 			
*/
//-------------------------------------------------------------------
XWINDOWSDEBUGCTRL::XWINDOWSDEBUGCTRL() 
{		
	Clean();

	XDebugCtrl = this;	
  
  mutexhandle = (XDWORD)CreateMutex( NULL, FALSE, NULL);
  if(!mutexhandle) return;	
};				



//-------------------------------------------------------------------
//  XDEBUGCTRL::~XDEBUGCTRL
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:03:22
//	
//	@return 			
*/
//-------------------------------------------------------------------
XWINDOWSDEBUGCTRL::~XWINDOWSDEBUGCTRL()
{						
  if(mutexhandle)
    {
      CloseHandle((HANDLE)mutexhandle);
      mutexhandle = NULL;
    }

	for(int c=0; c<XDEBUG_MAXNTARGETS; c++)
		{
			if(targets[c].GetType() == XDEBUGCTRLTYPE_NET) 
				{
				  CloseHandleNet(&targets[c]);
				}
		}

	Clean();

	XDebugCtrl =  NULL;
}



/*-------------------------------------------------------------------
//	XWINDOWSDEBUGCTRL::PrintSpecial
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			09/05/2016 11:28:50
//	
//  @param				target : 
//  @param				level : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void XWINDOWSDEBUGCTRL::PrintSpecial(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{	
	if(!target)							 return;
	if(!target->GetAim()[0]) return;

  Lock();

	HWND					 hwnd;
	COPYDATASTRUCT cs;	

	hwnd = FindWindow(NULL, target->GetAim());
	
	if(hwnd==NULL) 
		{
			#ifdef _MSC_VER	

			if(level) 
				{
					if((level & XDEBUGCTRLLEVEL_WITHCOLOR) == XDEBUGCTRLLEVEL_WITHCOLOR)
						{
					
						}
					 else
						{
							if((level & XDEBUGCTRLLEVEL_WITHTAB) == XDEBUGCTRLLEVEL_WITHTAB)
								{
									XSTRING tabstr;

									GenerateTab(level&0x0F,tabstr);		
									OutputDebugString(tabstr.Get()); 										
								}
							 else
								{ 
									if((level & XDEBUGCTRLLEVEL_WITHCODE) == XDEBUGCTRLLEVEL_WITHCODE)
										{
											XSTRING codestr;

											codestr.Format(__L("%02d: "), (level&0x0F));
											OutputDebugString(codestr.Get()); 									
										}
								}
						}			
				}
			     		
			OutputDebugString(string); 
			OutputDebugString(__L("\n")); 	
			#endif		
		}
	 else
	  {		

			XDWORD             publicIP = 0;
			XDWORD             localIP	= 0;
			XBUFFER						 xbufferpacket;
			XWINDOWSDATETIME   xdatetime;  				
	
			xdatetime.Read();

			SetDebugToXBuffer(publicIP, localIP, level, sequence, &xdatetime,  string, xbufferpacket);

			cs.dwData  = 0;
			cs.lpData  = (PVOID)xbufferpacket.Get();
			cs.cbData  = (XDWORD)xbufferpacket.GetSize();

			SendMessage(hwnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cs);	
		}

	target->AddNSend();

  UnLock();
}



/*-------------------------------------------------------------------
//	XWINDOWSDEBUGCTRL::PrintFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			09/05/2016 11:28:44
//	
//  @param				target : 
//  @param				level : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void XWINDOWSDEBUGCTRL::PrintFile(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{
	if(!target)							 return;
	if(!target->GetAim()[0]) return;

	XSTRING line;
  FILE*		file;

  Lock();

  int error = 0;
  
  #ifdef BUILDER
  file = _wfopen(target->GetAim(), __L("at"));
  if(!file)
    {
      UnLock();
      return;
    }
  #else
  error = _wfopen_s(&file, target->GetAim(), __L("at"));
  #endif
  
  if(error)
    { 
      UnLock();
      return;
    }

	// --- Check File size --------------

	int	sizefile;
	
	fflush(file);

	int position = ftell(file);
	fseek(file,0,SEEK_END);	
	sizefile = ftell(file);
	fseek(file,position,SEEK_SET);
	
	if(sizefile+(XDWORD)line.GetSize()>sizelimit)
		{
			fclose(file);	

			DeleteFile(target->GetAim());

			#ifdef BUILDER
      file = _wfopen(target->GetAim(),__L("at"));
      if(!file)
        {
          UnLock();
          return;
        }
      #else
      error = _wfopen_s(&file, target->GetAim(), __L("at"));
      #endif

			if(error)	
        {
          UnLock();
          return;
        };
		}

	// --- Check File size --------------
  
	if(level) 
		{
			if((level & XDEBUGCTRLLEVEL_WITHCOLOR) == XDEBUGCTRLLEVEL_WITHCOLOR)
				{
					
				}
			 else
			  {
					if((level & XDEBUGCTRLLEVEL_WITHTAB) == XDEBUGCTRLLEVEL_WITHTAB)
						{
							XSTRING tabstr;
							
							GenerateTab(level&0x0F, tabstr);					
							
							XSTRING_CREATEOEM(tabstr, charstr)
							fwrite(charstr, 1, tabstr.GetSize(), file);
							XSTRING_DELETEOEM(charstr)
						}
					 else
					  { 
							if((level & XDEBUGCTRLLEVEL_WITHCODE) == XDEBUGCTRLLEVEL_WITHCODE)
								{
									XSTRING codestr;
									
									codestr.Format(__L("%02d: "), (level&0x0F));

									XSTRING_CREATEOEM(codestr, charstr)
									fwrite(charstr, 1, codestr.GetSize(), file);
									XSTRING_DELETEOEM(charstr)
								}
						}
				}			
		}
	
	line = string;

	if(file)
		{ 
			XSTRING_CREATEOEM(line, charstr)	
			fwrite(charstr, 1, line.GetSize(), file);
			XSTRING_DELETEOEM(charstr)
	
			fwrite(__L("\n\r"),1,1,file);
	
			fclose(file);	
		}

	target->AddNSend();

  UnLock();
}




/*-------------------------------------------------------------------
//	XWINDOWSDEBUGCTRL::PrintNet
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			09/05/2016 11:29:44
//	
//  @param				target : 
//  @param				level : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void XWINDOWSDEBUGCTRL::PrintNet(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{	
	if(!target)	 return;
	
	Lock();
	
	XBUFFER  xbufferpacket;
	
	xdatetime.Read();
		
	SetDebugToXBuffer(publicIP, localIP, level, sequence, &xdatetime,  string, xbufferpacket);

	SOCKET handle = (SOCKET)target->GetNETHandle();

	send(handle,(const char*)xbufferpacket.Get(), xbufferpacket.GetSize(), 0);

	/*
	int result = WSAGetLastError();
	if(result)
		{
			int a=0;
			a++;
		}
	*/
		
	UnLock();
}




/*-------------------------------------------------------------------
//	XWINDOWSDEBUGCTRL::GetHandleNet
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/12/2016 14:18:19
//	
//	@return 			bool : 
//
//  @param				target : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSDEBUGCTRL::GetHandleNet(XDEBUGCTRLTARGET* target)																					
{
	SOCKADDR_IN  addr;
	SOCKET			 handle;
													
	memset(&addr,0,sizeof(SOCKADDR_IN));
	
	handle = socket(AF_INET, SOCK_DGRAM, 0);
	if(handle == INVALID_SOCKET)  return false;
							
	if(!target->GetIP()[0]) return false; 
	
	addr.sin_family				= AF_INET;
  #ifndef BUILDER
	inet_pton(addr.sin_family, target->GetIP(), &addr.sin_addr.s_addr);
  #else
  addr.sin_addr.s_addr	= inet_addr(target->GetIP());
  #endif
	addr.sin_port					= htons(target->GetPort());				
																																																				
	connect(handle,(LPSOCKADDR)&addr,sizeof(SOCKADDR_IN));	

	XQWORD NEThandle = (XQWORD)handle;

	target->SetNETHandle(NEThandle);

	return true;                              
}



/*-------------------------------------------------------------------
//	XWINDOWSDEBUGCTRL::CloseHandleNet
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/12/2016 14:39:11
//	
//	@return 			bool : 
//
//  @param				target : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSDEBUGCTRL::CloseHandleNet(XDEBUGCTRLTARGET* target)
{
	SOCKET handle = (SOCKET)target->GetNETHandle(); 	
	if(!handle) return false;
		
	closesocket(handle);
	target->SetNETHandle(0); 
	
	return true;
}





/*-------------------------------------------------------------------
//  XWINDOWSDEBUGCTRL::Lock
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      08/10/2012 17:54:16
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool XWINDOWSDEBUGCTRL::Lock()
{
	if(!mutexhandle) return false;
	if(WaitForSingleObject((HANDLE)mutexhandle, INFINITE) == WAIT_FAILED) return false;  

  return true;
}




/*-------------------------------------------------------------------
//  XWINDOWSDEBUGCTRL::UnLock
*/ 
/**
//  
//  ResolveURL(const char* url, char* IP)
//  @author       Abraham J. Velez
//  @version      08/10/2012 17:54:22
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool XWINDOWSDEBUGCTRL::UnLock()
{	
 	if(!mutexhandle)							         return false;
	if(!ReleaseMutex((HANDLE)mutexhandle)) return false;

  return true;
}



#endif
