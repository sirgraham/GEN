/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XWINDOWSSystem.cpp
*
* @class       XWINDOWSSYSTEM
* @brief       Windows System class
* @ingroup     PLATFORM_WINDOWS
*
* @author      Abraham J. Velez 
* @date        03/06/2018 13:03:21
*
* @copyright   Copyright(c) 2005 - 2018 GEN Group.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
*---------------------------------------------------------------------------------------------------------------------*/

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

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
#include "XDebugTrace.h"

#include "XWINDOWSSystem.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWINDOWSSYSTEM::XWINDOWSSYSTEM() : XSYSTEM()
* @brief      Constructor
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:05:25
*
* @param[in]  ) : 
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM::XWINDOWSSYSTEM() : XSYSTEM()

{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWINDOWSSYSTEM::~XWINDOWSSYSTEM()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:05:38
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSYSTEM::~XWINDOWSSYSTEM()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM_HARDWARETYPE XWINDOWSSYSTEM::GetTypeHardware(int* revision)
* @brief      Get Type Hardware
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:05:54
*
* @param[in]  revision : Type hardware
*
* @return     XSYSTEM_HARDWARETYPE : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XWINDOWSSYSTEM::GetTypeHardware(int* revision)
{
  if(revision) (*revision) = -1;

  return XSYSTEM_HARDWARETYPE_PC;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM_SO XWINDOWSSYSTEM::GetTypeSO()
* @brief      Get Type SO
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:06:17
*
* @return     XSYSTEM_SO : type of SO (enum XSYSTEM_SO)
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_SO XWINDOWSSYSTEM::GetTypeSO()
{
  #ifdef BUILDER

  OSVERSIONINFO osvi;

  osvi.dwOSVersionInfoSize =sizeof(OSVERSIONINFO);

  GetVersionEx(&osvi);

  switch(osvi.dwPlatformId)
    {
      case VER_PLATFORM_WIN32s        : return XSYSTEM_SO_WINDOWS;
      case VER_PLATFORM_WIN32_WINDOWS : return XSYSTEM_SO_WINDOWS98;
      case VER_PLATFORM_WIN32_NT      : return XSYSTEM_SO_WINDOWSNT;
    }
  
  #else
  
  OSVERSIONINFOEXW osvi;
  DWORDLONG        const dwlConditionMask = VerSetConditionMask(0, VER_PLATFORMID, VER_GREATER_EQUAL);

  osvi.dwOSVersionInfoSize = sizeof(osvi);
  osvi.dwPlatformId        = _WIN32_WINNT_WIN10;

  VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);


  switch(osvi.dwPlatformId)
    {
      case _WIN32_WINNT_NT4           : return XSYSTEM_SO_WINDOWSNT4;
      case _WIN32_WINNT_WIN2K         : return XSYSTEM_SO_WINDOWS2000;
      case _WIN32_WINNT_WINXP         : return XSYSTEM_SO_WINDOWSXP;
      case _WIN32_WINNT_WS03          : return XSYSTEM_SO_WINDOWSSERVER2003;
      case _WIN32_WINNT_VISTA         : return XSYSTEM_SO_WINDOWSVISTA;
      case _WIN32_WINNT_WIN7          : return XSYSTEM_SO_WINDOWS7;
      case _WIN32_WINNT_WIN8          : return XSYSTEM_SO_WINDOWS8;
      case _WIN32_WINNT_WINBLUE       : return XSYSTEM_SO_WINDOWS81;
      case _WIN32_WINNT_WIN10         : return XSYSTEM_SO_WINDOWS10;
    }

  #endif

  return XSYSTEM_SO_WINDOWS;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XLANGUAGE_CODE XWINDOWSSYSTEM::GetLanguage()
* @brief      Get Language Code
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:07:31
*
* @return     XLANGUAGE_CODE : language code (enum XLANGUAGE_CODE)
*
*---------------------------------------------------------------------------------------------------------------------*/
XLANGUAGE_CODE XWINDOWSSYSTEM::GetLanguage()
{
  switch(PRIMARYLANGID(GetUserDefaultLangID()))
    {
      case LANG_ENGLISH     : return XLANGUAGE_ENGLISH;
      case LANG_SPANISH     : return XLANGUAGE_SPANISH;
      case LANG_BASQUE      : return XLANGUAGE_BASQUE;
      case LANG_CATALAN     : return XLANGUAGE_CATALAN;
      case LANG_GALICIAN    : return XLANGUAGE_GALICIAN;
      case LANG_FRENCH      : return XLANGUAGE_FRENCH;
      case LANG_GERMAN      : return XLANGUAGE_GERMAN;
      case LANG_ITALIAN     : return XLANGUAGE_ITALIAN;
      case LANG_PORTUGUESE  : return XLANGUAGE_PORTUGUESE;
      case LANG_RUSSIAN     : return XLANGUAGE_RUSSIAN;
      case LANG_CZECH       : return XLANGUAGE_CZECH;
      case LANG_DANISH      : return XLANGUAGE_DANISH;
      case LANG_FINNISH     : return XLANGUAGE_FINNISH;
      case LANG_NORWEGIAN   : return XLANGUAGE_NORWEGIAN;
      case LANG_SLOVAK      : return XLANGUAGE_SLOVAK;
      case LANG_TURKISH     : return XLANGUAGE_TURKISH;
    }

  return XLANGUAGE_UNKNOWN;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
* @brief      Get Memory Info
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:08:10
*
* @param[out]  total : total memory in bytes.
* @param[out]  free : free memory in bytes.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
{
  MEMORYSTATUS mem;

  GlobalMemoryStatus(&mem);

  total = (XDWORD)(mem.dwTotalPhys/1024);
  free  = (XDWORD)(mem.dwAvailPhys/1024);

  return true;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSSYSTEM::MakeCommand(XCHAR* command, int* returncode)
* @brief      Make Command
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:09:08
*
* @param[in]  command : command to make
* @param[out] returncode : return code of command
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::MakeCommand(XCHAR* command, int* returncode)
{
  int status;

  status = _wsystem(command);
  if(status == -1) return false;
  if(status)
    {
      switch(errno)
        {
          case E2BIG    :
          case ENOENT   :
          case ENOEXEC  :
          case ENOMEM   : return false;
        }
    }

  if(returncode) (*returncode) = status;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSSYSTEM::ExecuteApplication(XCHAR* applicationname, XCHAR* params)
* @brief      Execute Application
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:10:10
*
* @param[in]  applicationpath :  path + name to application to exec
* @param[in]  params : params to exec application
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::ExecuteApplication(XCHAR* applicationpath, XCHAR* params)
{
  bool exist = false;

  XFILE* xfile = xfactory->Create_File();
  if(xfile)
    {
      exist = xfile->Open(applicationpath);
      xfile->Close();
    }

  xfactory->Delete_File(xfile);

  if(!exist)
    return false;

   _wspawnl(_P_NOWAIT, applicationpath, applicationpath, params, NULL);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSSYSTEM::IsApplicationRunning(XCHAR* applicationname, XDWORD* ID)
* @brief      Is Application Running
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:12:24
*
* @param[in]  applicationname : application name
* @param[out] ID : ID of the application by SO
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::IsApplicationRunning(XCHAR* applicationname, XDWORD* ID)
{
  PROCESSENTRY32 entry;
  HANDLE         snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  bool           exists = false;

  entry.dwSize = sizeof(PROCESSENTRY32);

  if(Process32First(snapshot, &entry))
    {
      while(Process32Next(snapshot, &entry))
        {
          XSTRING nameapp  = entry.szExeFile;
          XSTRING _applicationname = applicationname;

          if(_applicationname.Find(nameapp, true) != XSTRING_NOTFOUND)
            {
              if(ID) (*ID) = entry.th32ProcessID;
              exists = true;
            }
        }
    }

  CloseHandle(snapshot);

  return exists;
}





/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
* @brief      ShutDown of the SO
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:13:15
*
* @param[in]  type : type of shutdown
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
{
  if(type == XSYSTEM_SHUTDOWNTYPE_UNKNOWN) return false;

  HANDLE            hToken;
  TOKEN_PRIVILEGES  tkp;

  // Get a token for this process.
  if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))  return false;

  // Get the LUID for the shutdown privilege.
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

  tkp.PrivilegeCount            = 1;  // one privilege to set
  tkp.Privileges[0].Attributes  = SE_PRIVILEGE_ENABLED;

  // Get the shutdown privilege for this process.
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
  if(GetLastError() != ERROR_SUCCESS) return false;

  switch(type)
    {
      case XSYSTEM_SHUTDOWNTYPE_REBOOT   :  if(!ExitWindowsEx(EWX_REBOOT   | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                            break;

      case XSYSTEM_SHUTDOWNTYPE_LOGOFF   :  if(!ExitWindowsEx(EWX_LOGOFF   | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                            break;

      case XSYSTEM_SHUTDOWNTYPE_POWEROFF :  if(!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                            break;
    }

  return true;

}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSSYSTEM::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:14:33
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSSYSTEM::Clean()
{

}