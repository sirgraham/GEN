/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XWINDOWSDebugTrace.cpp
*
* @class       XWINDOWSDEBUGTRACE
* @brief       Windows debug trace class
* @ingroup     PLATFORM_WINDOWS
*
* @author      Abraham J. Velez 
* @date        31/05/2018 20:46:30
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

#ifdef XDEBUG_TRACE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

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

#include "XWINDOWSDebugTrace.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWINDOWSDEBUGTRACE::XWINDOWSDEBUGTRACE()
* @brief      Constructor
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:47:25
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XWINDOWSDEBUGTRACE::XWINDOWSDEBUGTRACE()
{
  Clean();

  XDEBUGTRACE::instance = this;

  mutexhandle = (XDWORD)CreateMutex( NULL, FALSE, NULL);
  if(!mutexhandle) return;
};



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWINDOWSDEBUGTRACE::~XWINDOWSDEBUGTRACE()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:47:31
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XWINDOWSDEBUGTRACE::~XWINDOWSDEBUGTRACE()
{
  if(mutexhandle)
    {
      CloseHandle((HANDLE)mutexhandle);
      mutexhandle = NULL;
    }

  for(int c=0; c<XDEBUGTRACE_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XDEBUGTRACE_TYPE_NET)
        {
          CloseHandleNet(&targets[c]);
        }
    }

  Clean();

  XDEBUGTRACE::instance = NULL;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSDEBUGTRACE::PrintSpecial(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print Special aim
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:48:01
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSDEBUGTRACE::PrintSpecial(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  Lock();

  HWND           hwnd;
  COPYDATASTRUCT cs;

  hwnd = FindWindow(NULL, target->GetAim());

  if(hwnd==NULL)
    {
      #ifdef _MSC_VER

      if(level)
        {
          if((level & XDEBUGTRACE_LEVEL_WITHCOLOR) == XDEBUGTRACE_LEVEL_WITHCOLOR)
            {

            }
           else
            {
              if((level & XDEBUGTRACE_LEVEL_WITHTAB) == XDEBUGTRACE_LEVEL_WITHTAB)
                {
                  XSTRING tabstr;

                  GenerateTab(level&0x0F,tabstr);
                  OutputDebugString(tabstr.Get());
                }
               else
                {
                  if((level & XDEBUGTRACE_LEVEL_WITHCODE) == XDEBUGTRACE_LEVEL_WITHCODE)
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
      XDWORD             localIP  = 0;
      XBUFFER            xbufferpacket;
      XWINDOWSDATETIME   xdatetime;

      xdatetime.Read();

      SetDebugToXBuffer(publicIP, localIP, level, sequence, &xdatetime,  string, xbufferpacket);

      cs.dwData  = 0;
      cs.lpData  = (PVOID)xbufferpacket.Get();
      cs.cbData  = (XDWORD)xbufferpacket.GetSize();

      SendMessage(hwnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cs);
    }

  target->AddNSendings();
  
  UnLock();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSDEBUGTRACE::PrintFile(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print File aim
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:50:00
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSDEBUGTRACE::PrintFile(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  XSTRING line;
  FILE*   file;

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

  XDWORD sizefile;

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
      if((level & XDEBUGTRACE_LEVEL_WITHCOLOR) == XDEBUGTRACE_LEVEL_WITHCOLOR)
        {

        }
       else
        {
          if((level & XDEBUGTRACE_LEVEL_WITHTAB) == XDEBUGTRACE_LEVEL_WITHTAB)
            {
              XSTRING tabstr;

              GenerateTab(level&0x0F, tabstr);

              XSTRING_CREATEOEM(tabstr, charstr)
              fwrite(charstr, 1, tabstr.GetSize(), file);
              XSTRING_DELETEOEM(charstr)
            }
           else
            {
              if((level & XDEBUGTRACE_LEVEL_WITHCODE) == XDEBUGTRACE_LEVEL_WITHCODE)
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

  target->AddNSendings();

  UnLock();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSDEBUGTRACE::PrintNet(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print Net aim
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:51:12
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSDEBUGTRACE::PrintNet(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)  return;

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

  target->AddNSendings();

  UnLock();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSDEBUGTRACE::GetHandleNet(XDEBUGTRACE_TARGET* target)
* @brief      Get Handle NET (WINDOWS)
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:52:47
*
* @param[in]  target : target of debug
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDEBUGTRACE::GetHandleNet(XDEBUGTRACE_TARGET* target)
{
  SOCKADDR_IN  addr;
  SOCKET       handle;

  memset(&addr,0,sizeof(SOCKADDR_IN));

  handle = socket(AF_INET, SOCK_DGRAM, 0);
  if(handle == INVALID_SOCKET)  return false;

  if(!target->GetIP()[0]) return false;

  addr.sin_family       = AF_INET;
  #ifndef BUILDER
  inet_pton(addr.sin_family, target->GetIP(), &addr.sin_addr.s_addr);
  #else
  addr.sin_addr.s_addr  = inet_addr(target->GetIP());
  #endif
  addr.sin_port         = htons(target->GetPort());

  connect(handle,(LPSOCKADDR)&addr,sizeof(SOCKADDR_IN));

  XQWORD NEThandle = (XQWORD)handle;

  target->SetNETHandle(NEThandle);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSDEBUGTRACE::CloseHandleNet(XDEBUGTRACE_TARGET* target)
* @brief      Close Handle NET (WINDOWS)
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:53:28
*
* @param[in]  target : target of debug
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDEBUGTRACE::CloseHandleNet(XDEBUGTRACE_TARGET* target)
{
  SOCKET handle = (SOCKET)target->GetNETHandle();
  if(!handle) return false;

  closesocket(handle);
  target->SetNETHandle(0);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSDEBUGTRACE::Lock()
* @brief      Lock (Mutex to print)
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:54:47
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDEBUGTRACE::Lock()
{
  if(!mutexhandle) return false;
  if(WaitForSingleObject((HANDLE)mutexhandle, INFINITE) == WAIT_FAILED) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSDEBUGTRACE::UnLock()
* @brief      Unlock (Mutex to print)
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:54:35
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSDEBUGTRACE::UnLock()
{
  if(!mutexhandle)                       return false;
  if(!ReleaseMutex((HANDLE)mutexhandle)) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSDEBUGTRACE::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 20:54:07
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSDEBUGTRACE::Clean()
{
  mutexhandle  = NULL;
}



#endif
