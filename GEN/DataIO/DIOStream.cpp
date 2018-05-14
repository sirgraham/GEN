//------------------------------------------------------------------------------------------
//  DIOSTREAM.CPP
//
//  Data IO Stream class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XTimer.h"

#include "DIOStreamXEvent.h"

#include "DIOStream.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAM::DIOSTREAM
*/
/**
//
//  Class Constructor DIOSTREAM
//
//  @author       Abraham J. Velez
//  @version      20/11/2014 16:11:30
//



*/
/*-----------------------------------------------------------------*/
DIOSTREAM::DIOSTREAM()
{
  Clean();

  inbuffer  = new XBUFFER();
  outbuffer = new XBUFFER();

  xmutextimerout    = xfactory->Create_Mutex();

  xtimerconnexion   = xfactory->CreateTimer();
  xtimernotactivity = xfactory->CreateTimer();
  xtimerout         = xfactory->CreateTimer();


  RegisterEvent(DIOSTREAMXEVENTTYPE_GETTINGCONNEXION);
  RegisterEvent(DIOSTREAMXEVENTTYPE_CONNECTED);
  RegisterEvent(DIOSTREAMXEVENTTYPE_DISCONNECTED);
}



//-------------------------------------------------------------------
//  DIOSTREAM::~DIOSTREAM
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:10:20
//
//  @return
//  */
//-------------------------------------------------------------------
DIOSTREAM::~DIOSTREAM()
{
  if(inbuffer)
    {
      delete inbuffer;
      inbuffer = NULL;
    }

  if(outbuffer)
    {
      delete outbuffer;
      outbuffer = NULL;
    }

  if(xmutextimerout) xmutextimerout->Lock();

  if(xtimerout)
    {
      xfactory->DeleteTimer(xtimerout);
      xtimerout = NULL;
    }

  if(xmutextimerout) xmutextimerout->UnLock();

  if(xtimerconnexion)
    {
      xfactory->DeleteTimer(xtimerconnexion);
      xtimerconnexion = NULL;
    }

  if(xtimernotactivity)
    {
      xfactory->DeleteTimer(xtimernotactivity);
      xtimernotactivity = NULL;
    }

  if(xmutextimerout)
    {
      xfactory->Delete_Mutex(xmutextimerout);
      xmutextimerout = NULL;
    }

  DeRegisterEvent(DIOSTREAMXEVENTTYPE_GETTINGCONNEXION);
  DeRegisterEvent(DIOSTREAMXEVENTTYPE_CONNECTED);
  DeRegisterEvent(DIOSTREAMXEVENTTYPE_DISCONNECTED);

  Clean();
}




/*-------------------------------------------------------------------
//  DIOSTREAM::IsConnected
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/06/2013 8:06:54
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAM::IsConnected()
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED) return true;

  return false;
}



/*-------------------------------------------------------------------
//  DIOSTREAM::IsDisconnected
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/06/2013 8:14:40
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAM::IsDisconnected()
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return true;

  return false;
}




/*-------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH::WaitToConnected
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/06/2013 8:07:10
//
//  @return       bool :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::WaitToConnected(int timeout)
{
  if(!xtimerconnexion) return false;

  bool status = false;

  xtimerconnexion->Reset();

  while(1)
    {
      if(IsConnected())
        {
          status = true;
          break;
        }

      if(GetConnectStatus() == DIOSTREAMSTATUS_DISCONNECTED)
        {
          break;
        }

      if((int)xtimerconnexion->GetMeasureSeconds() >= timeout)
        {
          break;
        }

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::WaitToDisconnected
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/06/2013 8:13:52
//
//  @return       bool :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::WaitToDisconnected(int timeout)
{
  if(!xtimerconnexion) return false;

  bool status = false;

  xtimerconnexion->Reset();

  while(1)
    {
      if(!IsDisconnected())
        {
          status = true;
          break;
        }

      if((int)xtimerconnexion->GetMeasureSeconds() >= timeout) break;

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::Read
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2012 13:29:32
//
//  @return       XDWORD :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAM::Read(XBYTE* buffer, XDWORD size)
{
  if(!inbuffer) return false;

  XDWORD esize = inbuffer->Extract(buffer,0,size);

  if(esize)
    {
      nbytesread+=size;
      if(xtimernotactivity) xtimernotactivity->Reset();
    }

  return esize;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::Write
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2012 13:29:23
//
//  @return       XDWORD :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAM::Write(XBYTE* buffer, XDWORD size)
{
  if(!outbuffer)                    return 0;
  if(!outbuffer->Add(buffer,size))  return 0;

  nbyteswrite+=size;
  if(xtimernotactivity) xtimernotactivity->Reset();

  return size;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/05/2014 12:29:46
//
//  @return       XDWORD :
//
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAM::Read(XBUFFER& xbuffer)
{
  if(inbuffer->IsEmpty()) return 0;

  XDWORD br = Read(xbuffer.Get(), xbuffer.GetSize());
  if(br)
    {
      if(br!=xbuffer.GetSize()) xbuffer.Resize(br);
    }
   else
    {
      xbuffer.Delete();
    }

  return xbuffer.GetSize();
}



/*-------------------------------------------------------------------
//  DIOSTREAM::Write
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2012 13:06:16
//
//  @return       XDWORD :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAM::Write(XBUFFER& xbuffer)
{
  return Write(xbuffer.Get(),xbuffer.GetSize());
}




/*-------------------------------------------------------------------
//  DIOSTREAM::WaitToFillingReadingBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2014 12:22:39
//
//  @return       bool :
//
//  @param        filledto :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::WaitToFilledReadingBuffer(int filledto, int timeout)
{
  if(!xtimerout)  return false;

  bool status = true;

  xtimerout->Reset();

  if(xmutextimerout) xmutextimerout->Lock();

  while(1)
    {
      if(filledto == DIOSTREAM_SOMETHINGTOREAD)
        {
          if(GetInXBuffer()->GetSize()) break;
        }
       else
        {
          if((int)GetInXBuffer()->GetSize() >= filledto) break;
        }

      if(timeout!=XTIMER_INFINITE)
        {
          if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              status = false;
              break;
            }
        }

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  if(xmutextimerout) xmutextimerout->UnLock();

  return status;
}





/*-------------------------------------------------------------------
//  DIOSTREAM::ReadStr
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/03/2013 15:35:19
//
//  @return       bool :
//  @param        str :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::ReadStr(char* str)
{
  if(!str) return false;

  XSTRING string;
  bool    status = ReadStr(string);

  XSTRING_CREATEOEM(string, charstr)
  memcpy(str, charstr, string.GetSize());
  XSTRING_DELETEOEM(charstr)

  return status;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::ReadStr
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/03/2013 15:35:26
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::ReadStr(XSTRING& string)
{
  bool foundreturn = false;
  int  stringsize  = 0;

  inbuffer->SetBlocked(true);

  for(int c=0;c<(int)inbuffer->GetSize();c++)
    {
      if((inbuffer->GetByte(c) =='\n') || (inbuffer->GetByte(c) == '\r'))
        {
          stringsize++;

          if(c+1<(int)inbuffer->GetSize())
            {
              if((inbuffer->GetByte(c+1) == '\n') || (inbuffer->GetByte(c+1) =='\r')) stringsize++;
            }

          foundreturn = true;

          break;

        } else stringsize++;
    }

  inbuffer->SetBlocked(false);

  if(!foundreturn) return false;

  XBYTE* buffer = new XBYTE[stringsize+1];
  if(!buffer) return false;

  memset(buffer, 0, stringsize+1);

  int br = Read((XBYTE*)buffer, stringsize);

  if(br != stringsize) return false;

  int c=(int)strlen((const char*)buffer);
  while(c>=0)
    {
      if((buffer[c]=='\n')||(buffer[c]=='\r')) buffer[c]=0;
      c--;
    }

  string.Empty();
  string = (char*)buffer;

  delete [] buffer;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::ReadStr
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 12:10:02
//
//  @return       bool :
//
//  @param        string :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::ReadStr(XSTRING& string, int timeout)
{
  bool status = true;

  if(!xtimerout) return false;

  xtimerout->Reset();

  if(xmutextimerout) xmutextimerout->Lock();

  while(!ReadStr(string))
    {
      if((int)xtimerout->GetMeasureSeconds() >= timeout)
        {
          status = false;
          break;
        }
    }

  if(xmutextimerout) xmutextimerout->UnLock();

  return status;
}



//-------------------------------------------------------------------
//  DIOSTREAM::WriteStr
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        str :
*/
//-------------------------------------------------------------------
bool DIOSTREAM::WriteStr(const char* str)
{
  if(!str) return false;

  if(!Write((XBYTE*)str,(XDWORD)strlen(str))) return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAM::WriteStr
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/03/2013 15:36:34
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::WriteStr(XCHAR* string)
{
  XSTRING str(string);

  XSTRING_CREATEOEM(str, charstr)
  bool status = WriteStr(charstr);
  XSTRING_DELETEOEM(charstr)

  return status;
}



/*-------------------------------------------------------------------
//  DIOSTREAM::WriteStr
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/03/2013 15:36:34
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::WriteStr(XSTRING& string)
{
  XSTRING_CREATEOEM(string, charstr)
  bool status = WriteStr(charstr);
  XSTRING_DELETEOEM(charstr)

  return status;
}





/*-------------------------------------------------------------------
//  DIOSTREAM::ResetXBuffers
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/07/2012 10:49:51
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAM::ResetXBuffers()
{
  if((!inbuffer) && (!outbuffer)) return false;

  inbuffer->Delete();
  outbuffer->Delete();

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::ResetInXBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2014 20:41:38
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::ResetInXBuffer()
{
  if(!inbuffer) return false;

  inbuffer->Delete();

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::ResetOutXBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2014 20:41:43
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::ResetOutXBuffer()
{
  if(!outbuffer) return false;

  outbuffer->Delete();

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::IsFlushOutXBuffer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2013 10:36:56
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAM::IsFlushOutXBuffer()
{
  if(!outbuffer) return false;

  if(outbuffer->GetSize()) return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAM::IsFlushXBuffers
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/02/2013 6:51:53
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAM::IsFlushXBuffers()
{
  if((!inbuffer) && (!outbuffer)) return false;

  if(inbuffer->GetSize())  return false;
  if(outbuffer->GetSize()) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::WaitToFlushOutXBuffer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2013 10:46:46
//
//  @return       bool :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::WaitToFlushOutXBuffer(int timeout)
{
  if(!xtimerout) return false;

  bool status = true;

  xtimerout->Reset();

  if(xmutextimerout) xmutextimerout->Lock();

  while(!IsFlushOutXBuffer())
    {
      if(timeout!=XTIMER_INFINITE)
        {
          if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              ResetOutXBuffer();

              status = false;
              break;
            }
        }

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  if(xmutextimerout) xmutextimerout->UnLock();

  return status;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::WaitToFlushXBuffers
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2013 10:47:03
//
//  @return       bool :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAM::WaitToFlushXBuffers(int timeout)
{
  if(!xtimerout) return false;

  bool status = true;

  xtimerout->Reset();

  if(xmutextimerout) xmutextimerout->Lock();

  while(!IsFlushXBuffers())
    {
      if(timeout!=XTIMER_INFINITE)
        {
          if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              ResetXBuffers();

              status = false;
              break;
            }
        }

      Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
    }

  if(xmutextimerout) xmutextimerout->UnLock();

  return status;
}




/*-------------------------------------------------------------------
//  DIOSTREAM::ResetConnexionStatistics
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 21:17:14
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAM::ResetConnexionStatistics()
{
  nbytesread  = 0;
  nbyteswrite = 0;

  if(xtimerconnexion)   xtimerconnexion->Reset();
  if(xtimernotactivity) xtimernotactivity->Reset();

  return true;
}


