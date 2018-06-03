/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XLINUXDebugTrace.cpp
*
* @class       XLINUXDEBUGTRACE
* @brief       
* @ingroup     PLATFORM_LINUX
*
* @author      Abraham J. Velez 
* @date        01/06/2018 9:03:29
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "XBase.h"
#include "XBuffer.h"

#include "XLINUXSystem.h"
#include "XLINUXDateTime.h"

#include "DIOLINUXURL.h"

#include "XLINUXDebugTrace.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XLINUXDEBUGTRACE::XLINUXDEBUGTRACE()
* @brief      Constructor
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:04:03
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XLINUXDEBUGTRACE::XLINUXDEBUGTRACE()
{
  Clean();

  XDEBUGTRACE::instance = this;

  pthread_mutexattr_init(&attr);
  if(!pthread_mutex_init(&mutex, &attr)) mutexhandle = 1;
};



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XLINUXDEBUGTRACE::~XLINUXDEBUGTRACE()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:04:14
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XLINUXDEBUGTRACE::~XLINUXDEBUGTRACE()
{
  if(mutexhandle)
    {
      pthread_mutex_destroy(&mutex);
      mutexhandle = 0;
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
* @fn         void XLINUXDEBUGTRACE::PrintSpecial(XDEBUGTRACETARGET* target, XBYTE level, XCHAR* string)
* @brief      Print Special aim
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:04:36
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XLINUXDEBUGTRACE::PrintSpecial(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  Lock();

  XSTRING  _aim;
  XBYTE    aim[_MAXPATH];
  int      filehdl;
  int      size = 0;

  memset(aim, 0, _MAXPATH);

  _aim.Set(target->GetAim());
  _aim.ConvertToUTF8(aim, size);

  signal(SIGPIPE,SIG_IGN);

  filehdl = open((char*)aim, O_RDWR | O_NONBLOCK);
  if(filehdl == -1)
    {
      if(mkfifo((char*)aim, 0777 /*S_IRUSR | S_IWUSR*/)!=0) return;

      filehdl = open((char*)aim, O_RDWR | O_NONBLOCK);
      if(filehdl == -1)
        {
          UnLock();
          return;
        }
    }


  // Not Limit of Size ...

  XSTRING line;

  /*
  if(level)
    {
      XSTRING codestr;

      codestr.Format(__L("%03d: "), level);
      write(filehdl,codestr.GetOEM(),strlen(codestr.GetOEM()));
    }
  */

  line = string;

  XSTRING_CREATEOEM(line, charOEM)
  write(filehdl, charOEM, line.GetSize());
  XSTRING_DELETEOEM(charOEM)

  write(filehdl,__L("\n\r"),2);
  fsync(filehdl);


  close(filehdl);

  UnLock();
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XLINUXDEBUGTRACE::PrintFile(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print File aim
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:05:04
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XLINUXDEBUGTRACE::PrintFile(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  Lock();

  XSTRING  _aim;
  XBYTE    aim[_MAXPATH];
  XSTRING  _mode;
  XBYTE    mode[32];
  XSTRING  line;
  FILE*    file;
  int      size = 0;

  _aim.Set(target->GetAim());
  _aim.ConvertToUTF8(aim, size);

  _mode.Set(__L("at"));
  _mode.ConvertToUTF8(mode,size);

  file=fopen((char*)aim, (char*)mode);
  if(!file)
    {
      UnLock();
      return;
    }

  // --- Check File size --------------

  XDWORD sizefile = 0;

  fflush(file);

  int position = ftell(file);
  fseek(file,0,SEEK_END);
  sizefile = ftell(file);
  fseek(file,position,SEEK_SET);

  if((sizefile+(int)line.GetSize())>sizelimit)
    {
      fclose(file);

      if(unlink((char*)target)==-1)
        {
          int a=0;
          a++;
        }

      file=fopen((char*)target,(char*)mode);
      if(!file)
        {
          UnLock();
          return;
        }
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

              GenerateTab(level&0x0F,tabstr);

              XSTRING_CREATEOEM(tabstr, charOEM)
              fwrite(charOEM, 1, tabstr.GetSize(), file);
              XSTRING_DELETEOEM(charOEM)
            }
           else
            {
              if((level & XDEBUGTRACE_LEVEL_WITHCODE) == XDEBUGTRACE_LEVEL_WITHCODE)
                {
                  XSTRING codestr;

                  codestr.Format(__L("%02d: "), (level&0x0F));

                  XSTRING_CREATEOEM(codestr, charOEM)
                  fwrite(charOEM, 1, codestr.GetSize(), file);
                  XSTRING_DELETEOEM(charOEM)
                }
            }
        }
    }


  line = string;

  XSTRING_CREATEOEM(line, charOEM)
  fwrite(charOEM, 1, line.GetSize(), file);
  XSTRING_DELETEOEM(charOEM)

  fwrite(__L("\n\r"),1,1,file);

  fclose(file);

  UnLock();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XLINUXDEBUGTRACE::PrintNet(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print Net aim
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:05:26
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XLINUXDEBUGTRACE::PrintNet(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target) return;

  Lock();

  XBUFFER xbufferpacket;

  xdatetime.Read();

  SetDebugToXBuffer(publicIP, localIP, level, sequence, &xdatetime,  string, xbufferpacket);

  int handle = (int)target->GetNETHandle();

  send(handle,(const char*)xbufferpacket.Get(), xbufferpacket.GetSize(), 0);

  UnLock();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXDEBUGTRACE::GetHandleNet(XDEBUGTRACE_TARGET* target)
* @brief      Get Handle NET (LINUX)
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:05:37
*
* @param[in]  target : target of debug
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDEBUGTRACE::GetHandleNet(XDEBUGTRACE_TARGET* target)
{
  struct sockaddr_in  addr;
  int                 handle;

  memset(&addr, 0, sizeof(struct sockaddr));

  handle = socket(AF_INET, SOCK_DGRAM, 0);
  if(handle < 0)  return false;

  if(!target->GetIP()[0]) return false;

  addr.sin_family       = AF_INET;
  addr.sin_addr.s_addr  = inet_addr(target->GetIP());
  addr.sin_port         = htons(target->GetPort());

  connect(handle, (struct sockaddr*)&addr, sizeof(struct sockaddr));

  XQWORD NEThandle = (XQWORD)handle;

  target->SetNETHandle(NEThandle);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXDEBUGTRACE::CloseHandleNet(XDEBUGTRACETARGET* target)
* @brief      Close Handle NET (LINUX)
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:05:47
*
* @param[in]  target : target of debug
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDEBUGTRACE::CloseHandleNet(XDEBUGTRACE_TARGET* target)
{
  int handle = (int)target->GetNETHandle();
  if(!handle) return false;

  close(handle);
  target->SetNETHandle(0);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXDEBUGTRACE::Lock()
* @brief      Lock (Mutex to print)
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:05:58
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDEBUGTRACE::Lock()
{
  if(!mutexhandle)               return false;
  if(pthread_mutex_lock(&mutex)) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXDEBUGTRACE::UnLock()
* @brief      Unlock (Mutex to print)
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       01/06/2018 9:06:10
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXDEBUGTRACE::UnLock()
{
  if(!mutexhandle)                   return false;
  if(pthread_mutex_unlock(&mutex))   return false;

  return true;
}


#endif
