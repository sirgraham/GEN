
//------------------------------------------------------------------------------------------
//  XANDROIDDEBUG.CPP
//
//  ANDROID debug class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifdef XDEBUG

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <android/log.h>

#include "XBase.h"
#include "XBuffer.h"

#include "XANDROIDSystem.h"
#include "XANDROIDDateTime.h"
#include "DIOANDROIDURL.h"

#include "XANDROIDDebug.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XANDROIDDEBUGCTRL::XANDROIDDEBUGCTRL
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:03:05
//
//  @return
*/
//-------------------------------------------------------------------
XANDROIDDEBUGCTRL::XANDROIDDEBUGCTRL()
{
  Clean();

  XDebugCtrl = this;

  pthread_mutexattr_init(&attr);
  if(!pthread_mutex_init(&mutex, &attr)) mutexhandle = 1;
};



//-------------------------------------------------------------------
//  XDEBUGCTRL::~XDEBUGCTRL
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:03:22
//
//  @return
*/
//-------------------------------------------------------------------
XANDROIDDEBUGCTRL::~XANDROIDDEBUGCTRL()
{
  if(mutexhandle)
    {
      pthread_mutex_destroy(&mutex);
      mutexhandle = 0;
    }

  Clean();

  XDebugCtrl =  NULL;
}






/*-------------------------------------------------------------------
//  XANDROIDDEBUGCTRL::PrintSpecial
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2016 13:08:56
//
//  @param        target :
//  @param        level :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
void XANDROIDDEBUGCTRL::PrintSpecial(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  Lock();

  XSTRING line;
  line = string;

  const char test[] = "prueba";

  XSTRING_CREATEOEM(line, charOEM)
  switch(level)
    {
      case  0 : XANDROIDDEBUG_LOG_INFO((const char*)test);      break;
      case  1 : XANDROIDDEBUG_LOG_ERROR((const char*)test);     break;
      case  2 : XANDROIDDEBUG_LOG_WARNING((const char*)test);   break;
      case  3 :
      default : XANDROIDDEBUG_LOG_DEBUG((char*)test);           break;
    }
  XSTRING_DELETEOEM(charOEM)

  UnLock();
}



/*-------------------------------------------------------------------
//  XANDROIDDEBUGCTRL::PrintFile
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2016 13:09:10
//
//  @param        target :
//  @param        level :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
void XANDROIDDEBUGCTRL::PrintFile(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
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

  int sizefile = 0;

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
      if((level & XDEBUGCTRLLEVEL_WITHCOLOR) == XDEBUGCTRLLEVEL_WITHCOLOR)
        {

        }
       else
        {
          if((level & XDEBUGCTRLLEVEL_WITHTAB) == XDEBUGCTRLLEVEL_WITHTAB)
            {
              XSTRING tabstr;

              GenerateTab(level&0x0F,tabstr);

              XSTRING_CREATEOEM(tabstr, charOEM)
              fwrite(charOEM, 1, tabstr.GetSize(), file);
              XSTRING_DELETEOEM(charOEM)
            }
           else
            {
              if((level & XDEBUGCTRLLEVEL_WITHCODE) == XDEBUGCTRLLEVEL_WITHCODE)
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



/*-------------------------------------------------------------------
//  XANDROIDDEBUGCTRL::PrintNet
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2016 13:08:22
//
//  @param        target :
//  @param        level :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
void XANDROIDDEBUGCTRL::PrintNet(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  struct sockaddr_in  addr;
  int                 handle;

  Lock();

  memset(&addr, 0, sizeof(struct sockaddr));

  handle = socket(AF_INET, SOCK_DGRAM, 0);
  if(handle < 0)
    {
      UnLock();
      return;
    }

  if(!target->GetIP()[0])
    {
      UnLock();
      return;
    }

  addr.sin_family       = AF_INET;
  addr.sin_addr.s_addr  = inet_addr(target->GetIP());
  addr.sin_port         = htons(target->GetPort());

  connect(handle, (struct sockaddr*)&addr, sizeof(struct sockaddr));

  XBUFFER            xbufferpacket(&xfactory);
  XANDROIDDATETIME   xdatetime;

  xdatetime.Read();

  SetToDebugPacket(level, &xdatetime, sequence, string, xbufferpacket);

  send(handle,(const char*)xbufferpacket.Get(), xbufferpacket.GetSize(), 0);

  close(handle);

  UnLock();
}




/*-------------------------------------------------------------------
//  XANDROIDDEBUGCTRL::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/07/2012 12:42:37
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XANDROIDDEBUGCTRL::Clean()
{
  mutexhandle  = 0;
}



/*-------------------------------------------------------------------
//  XANDROIDDEBUGCTRL::Lock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 10:48:04
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XANDROIDDEBUGCTRL::Lock()
{
  if(!mutexhandle)               return false;
  if(pthread_mutex_lock(&mutex)) return false;

  return true;
}




/*-------------------------------------------------------------------
//  XANDROIDDEBUGCTRL::UnLock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/10/2012 17:59:20
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XANDROIDDEBUGCTRL::UnLock()
{
  if(!mutexhandle)                   return false;
  if(pthread_mutex_unlock(&mutex))   return false;

  return true;
}



#endif
