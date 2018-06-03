//------------------------------------------------------------------------------------------
//  DIOOBEXPUSH.CPP
//
//  Send Method
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 14/09/2004 12:01:13
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XFile.h"
#include "XPath.h"
#include "XBuffer.h"
#include "XTimer.h"
#include "XThreadCollected.h"
#include "XDebugTrace.h"

#include "HashCRC32.h"

#include "DIOStreamBluetooth.h"

#include "DIOOBEXPush.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::DIOOBEXPUSHFILE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/12/2010 19:47:22
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOOBEXPUSHFILE::DIOOBEXPUSHFILE()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::~DIOOBEXPUSHFILE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/12/2010 19:47:30
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOOBEXPUSHFILE::~DIOOBEXPUSHFILE()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::GetID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/05/2011 13:29:49
//
//  @return       XDWORD :
//  */
/*-----------------------------------------------------------------*/
XDWORD DIOOBEXPUSHFILE::GetID()
{
  return ID;
}


/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::GetXPath
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/05/2011 13:40:30
//
//  @return       XPATH* :
//  */
/*-----------------------------------------------------------------*/
XPATH* DIOOBEXPUSHFILE::GetXPath()
{
  return &xpath;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::GetNameSend
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/05/2011 13:40:42
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOOBEXPUSHFILE::GetNameSend()
{
  return &namesend;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::IsSend
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2011 0:22:45
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::IsSend()
{
  return sendis;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE:GetSendPercent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2011 11:13:51
//
//  @return       XBYTE :
//  */
/*-----------------------------------------------------------------*/
XBYTE DIOOBEXPUSHFILE::GetSendPercent()
{
  return sendpercent;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::GetSendTimeElapsed
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2011 10:07:37
//
//  @return       XQWORD :
//  */
/*-----------------------------------------------------------------*/
XQWORD DIOOBEXPUSHFILE::GetSendTimeElapsed()
{
  return sendtimeelapsed;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::GetError
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/05/2011 13:31:25
//
//  @return       DIOOBEXPUSHERROR :
//  */
/*-----------------------------------------------------------------*/
DIOOBEXPUSHERROR DIOOBEXPUSHFILE::GetError()
{
  return error;
}


/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::SetID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/05/2011 13:31:50
//
//  @return       bool :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetID(XDWORD ID)
{
  this->ID = ID;

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::SetIsSend
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2011 0:23:47
//
//  @return       bool :
//  @param        issend :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetIsSend(bool issend)
{
  this->sendis = issend;

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::SetSendPercent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2011 11:14:59
//
//  @return       bool :
//  @param        percent :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetSendPercent(XBYTE percent)
{
  sendpercent = percent;

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::SetSendTimeElapsed
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2011 10:08:58
//
//  @return       bool :
//  @param        sendtimeelapsed :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetSendTimeElapsed(XQWORD sendtimeelapsed)
{
  this->sendtimeelapsed = sendtimeelapsed;

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::SetError
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/05/2011 13:32:10
//
//  @return       bool :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSHFILE::SetError(DIOOBEXPUSHERROR error)
{
  this->error = error;

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSHFILE::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/12/2010 19:48:14
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOOBEXPUSHFILE::Clean()
{
  ID               = 0;
  sendis           = false;
  sendpercent      = 0;
  sendtimeelapsed  = 0;
  error            = DIOOBEXPUSHERROR_NONE;
}






//-------------------------------------------------------------------
//  DIOOBEXPUSH::DIOOBEXPUSH
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/10/2004 15:03:37
//
//  @return

//  @param        diostream :
*/
//-------------------------------------------------------------------
DIOOBEXPUSH::DIOOBEXPUSH(DIOSTREAM* diostream, bool inlittleendian) : XFSMACHINE(0)
{
  Clean();

  this->diostream       = diostream;
  this->inlittleendian  = inlittleendian;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_NONE            ,
                DIOOBEXPUSH_XFSMEVENT_CONNECT         , DIOOBEXPUSH_XFSMSTATE_CONNECT       ,
                DIOOBEXPUSH_XFSMEVENT_INIPUSH         , DIOOBEXPUSH_XFSMSTATE_INIPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_CONNECT         ,
                DIOOBEXPUSH_XFSMEVENT_INIPUSH         , DIOOBEXPUSH_XFSMSTATE_INIPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_INIPUSH         ,
                DIOOBEXPUSH_XFSMEVENT_SENDHEADPUSH    , DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH  ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH    ,
                DIOOBEXPUSH_XFSMEVENT_RECVHEADPUSH    , DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH  ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH    ,
                DIOOBEXPUSH_XFSMEVENT_SENDBLOCKPUSH   , DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH   ,
                DIOOBEXPUSH_XFSMEVENT_RECVBLOCKPUSH   , DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH   ,
                DIOOBEXPUSH_XFSMEVENT_SENDBLOCKPUSH   , DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH ,
                DIOOBEXPUSH_XFSMEVENT_ENDPUSH         , DIOOBEXPUSH_XFSMSTATE_ENDPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_ENDPUSH         ,
                DIOOBEXPUSH_XFSMEVENT_INIPUSH         , DIOOBEXPUSH_XFSMSTATE_INIPUSH       ,
                DIOOBEXPUSH_XFSMEVENT_DISCONNECT      , DIOOBEXPUSH_XFSMSTATE_DISCONNECT    ,
                EVENTDEFEND)) return;

  if(!AddState( DIOOBEXPUSH_XFSMSTATE_DISCONNECT      ,
                DIOOBEXPUSH_XFSMEVENT_NONE            , DIOOBEXPUSH_XFSMSTATE_NONE          ,
                EVENTDEFEND)) return;
}


//-------------------------------------------------------------------
//  DIOOBEXPUSH::~DIOOBEXPUSH
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 11:58:01
//
//  @return       void :
//  */
//-------------------------------------------------------------------
DIOOBEXPUSH::~DIOOBEXPUSH()
{
  End();

  Clean();
}



//-------------------------------------------------------------------
//  DIOOBEXPUSH::Ini
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 12:02:44
//
//  @return       bool :
//  @param        device :
*/
//-------------------------------------------------------------------
bool DIOOBEXPUSH::Ini(int timeout)
{
  if(!diostream)               return false;
  if(!diostream->GetConfig()) return false;

  sendbuffer = new XBUFFER();
  xfile      = xfactory->Create_File();
  xtimer     = xfactory->CreateTimer();

  issending  = false;

  if(xfile &&  sendbuffer && xtimer)
    {
      if(diostream->Open())
        {
          if(diostream->WaitToConnected(timeout))
            {
              xthreadobex = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAM, __L("DIOOBEXPUSH::Ini"),ThreadSend,(void*)this);
              if(xthreadobex)
                {
                  xthreadobex->Ini(false);
                  return true;
                }
            }

          diostream->Close();
        }
    }

  delete sendbuffer;

  xfactory->Delete_File(xfile);
  xfactory->DeleteTimer(xtimer);

  sendbuffer = NULL;
  xfile      = NULL;
  xtimer     = NULL;

  return false;
}



//-------------------------------------------------------------------
//  DIOOBEXPUSH::IsSending
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 12:02:09
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOOBEXPUSH::IsSending()
{
  if(sendfilelist.IsEmpty()) return false;

  if(!issending) xthreadobex->Run(false);

  return issending;
}


/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SetIsSending
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/06/2011 17:46:45
//
//  @return       bool :
//  @param        issending :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSH::SetIsSending(bool issending)
{
  this->issending = issending;

  return true;
}




/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SendFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2011 23:59:29
//
//  @return       bool :
//  @param        xpath :
//  @param        namesend :
//  @param        ID :
//  @param        timeoutask :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSH::SendFile(XPATH& xpath,XSTRING& namesend,XDWORD ID,int timeoutask)
{
  if(!xfactory) return false;

  SendFileList_DeleteAll();
  if(!SendFileList_AddFile(xpath,namesend,ID)) return false;

  return SendFiles(timeoutask);
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SendFiles
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2011 23:59:42
//
//  @return       bool :
//  @param        asktimeout :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSH::SendFiles(int asktimeout)
{
  if(!xfactory)             return false;
  if(!sendfilelist.GetSize()) return false;

  xtimer->Reset();

  this->asktimeout =  asktimeout;

  asktimeelapsed   = 0;
  issending        = true;
  indexfilelist    = 0;

  SetEvent(sendisstart?DIOOBEXPUSH_XFSMSTATE_INIPUSH:DIOOBEXPUSH_XFSMSTATE_CONNECT);
  xthreadobex->Run(true);

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::GetXTimer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/06/2011 16:23:54
//
//  @return       XTIMER* :
//  */
/*-----------------------------------------------------------------*/
XTIMER* DIOOBEXPUSH::GetXTimer()
{
  return xtimer;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::GetAskTimeElapsed
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2011 10:04:34
//
//  @return       XQWORD :
//  */
/*-----------------------------------------------------------------*/
XQWORD DIOOBEXPUSH::GetAskTimeElapsed()
{
  return asktimeelapsed;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SendFileList_AddFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/05/2011 13:38:59
//
//  @return       bool :
//  @param        xpath :
//  @param        namesend :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSH::SendFileList_AddFile(XPATH& xpath,XSTRING& namesend,XDWORD ID)
{
  DIOOBEXPUSHFILE* obexpushfile = new DIOOBEXPUSHFILE();
  if(!obexpushfile) return false;

  obexpushfile->GetXPath()->Set(xpath);
  obexpushfile->GetNameSend()->Set(namesend);

  XDWORD _ID = GenerateFileID(xpath);
  if(!_ID)
    {
      delete obexpushfile;
      return false;
    }

  if(!ID)
     {
       obexpushfile->SetID(_ID);
     }
    else
     {
        if(ID == _ID)
          {
            obexpushfile->SetID(ID);
          }
         else
          {
            delete obexpushfile;
            return false;
          }
     }

  obexpushfile->SetError(DIOOBEXPUSHERROR_NONE);

  sendfilelist.Add(obexpushfile);

  return true;
}


/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SendFileList_GetFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/12/2010 9:48:13
//
//  @return       DIOOBEXPUSHFILE* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
DIOOBEXPUSHFILE* DIOOBEXPUSH::SendFileList_GetFile(int index)
{
  if(sendfilelist.IsEmpty())                    return NULL;
  if((indexfilelist>=(int)sendfilelist.GetSize())) return NULL;

  return (DIOOBEXPUSHFILE*)sendfilelist.Get(index);
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SendFileList_GetActualFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/12/2010 10:15:58
//
//  @return       DIOOBEXPUSHFILE* :
//  */
/*-----------------------------------------------------------------*/
DIOOBEXPUSHFILE* DIOOBEXPUSH::SendFileList_GetActualFile()
{
  int index = SendFileList_GetIndex();
  if(index==-1) return NULL;

  return (DIOOBEXPUSHFILE*)SendFileList_GetFile(index);
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SendFileList_DeleteAll
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/12/2010 10:29:41
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSH::SendFileList_DeleteAll()
{
  if(sendfilelist.IsEmpty()) return false;

  sendfilelist.DeleteContents();

  sendfilelist.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SendFileList_GetIndex
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/12/2010 10:10:16
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOOBEXPUSH::SendFileList_GetIndex()
{
  if(sendfilelist.IsEmpty())                    return -1;
  if((indexfilelist>=(int)sendfilelist.GetSize())) return -1;

  return indexfilelist;
}


/*-------------------------------------------------------------------
//  DIOOBEXPUSH::SendFileList_GetSize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/12/2010 11:18:00
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOOBEXPUSH::SendFileList_GetSize()
{
  return sendfilelist.GetSize();
}



//-------------------------------------------------------------------
//  DIOOBEXPUSH::End
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 12:04:09
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool DIOOBEXPUSH::End()
{
  if(xthreadobex)
    {
      xthreadobex->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAM, xthreadobex);
      xthreadobex = NULL;
    }

  if(diostream)
    {
      diostream->Disconnect();
      diostream->Close();
    }

  if(xtimer)
    {
      xfactory->DeleteTimer(xtimer);
      xtimer = NULL;
    }

  if(xfile)
    {
      xfile->Close();
      xfactory->Delete_File(xfile);
      xfile = NULL;
    }

  if(sendbuffer)
    {
      delete sendbuffer;
      sendbuffer = NULL;
    }

  SendFileList_DeleteAll();

  SetCurrentState(DIOOBEXPUSH_XFSMSTATE_NONE);

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::GetDIOStream
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/12/2009 08:22:22 a.m.
//
//  @return       DIOSTREAM* :
//  */
/*-----------------------------------------------------------------*/
DIOSTREAM* DIOOBEXPUSH::GetDIOStream()
{
  return diostream;
}



//-------------------------------------------------------------------
//  DIOOBEXPUSH::GetFileName
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/04/2006 23:12:04
//
//  @return       char* :
//  */
//-------------------------------------------------------------------
XPATH* DIOOBEXPUSH::GetFileNameSend()
{
  return &xpath;
}



//-------------------------------------------------------------------
//  DIOOBEXPUSH::GenerateFileID
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 12:44:11
//
//  @return       bool :
//  @param        param :
*/
//-------------------------------------------------------------------
XDWORD DIOOBEXPUSH::GenerateFileID(XPATH& xpath)
{
  if(!xfactory) return 0;
  if(xpath.IsEmpty()) return 0;

  HASHCRC32 hash;

  hash.Do(xpath);

  return hash.GetResultCRC32();
}



//-------------------------------------------------------------------
//  DIOOBEXPUSH::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 12:04:13
//
//  @return
//  */
//-------------------------------------------------------------------
void DIOOBEXPUSH::Clean()
{
  diostream           = NULL;
  inlittleendian      = false;

  xtimer              = NULL;
  asktimeout          = DIOOBEXPUSH_MAXTIMEOUTASK;
  asktimeelapsed      = 0;

  indexfilelist       = -1;

  xfile               = NULL;

  sendbuffer          = NULL;

  issending           = false;

  sendblocksize       = 0;
  sendisstart         = false;

  filesize            = 0;
  readfilesize        = 0;
  readfilesizeblock   = 0;

  haveerror           = false;

  xthreadobex         = NULL;
}



//-------------------------------------------------------------------
//  DIOOBEXPUSH::CheckSending
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 12:12:24
//
//  @return       bool :
//  @param        error :
*/
//-------------------------------------------------------------------
bool DIOOBEXPUSH::CheckSending()
{
  DIOOBEXPUSHFILE*    obexpushfile = (DIOOBEXPUSHFILE*)sendfilelist.Get(indexfilelist);
  DIOSTREAMBLUETOOTH* sio          = (DIOSTREAMBLUETOOTH*)diostream;
  XWORD               wsize        = 0;
  XDWORD              dsize        = 0;
  bool                islastblock  = false;

  if((!sio)||(!obexpushfile))
    {
      if(obexpushfile) obexpushfile->SetError(DIOOBEXPUSHERROR_NOTCONNEXION);
      return false;
    }

  if(sio->GetConnectStatus()==DIOSTREAMSTATUS_GETTINGCONNEXION)
    {
      if(xtimer->GetMeasureSeconds()>=(XDWORD)asktimeout)
        {
          asktimeelapsed = xtimer->GetMeasureMilliSeconds();
          for(int c=0;c<(int)sendfilelist.GetSize();c++)
            {
              obexpushfile = (DIOOBEXPUSHFILE*)sendfilelist.Get(c);
              if(obexpushfile) obexpushfile->SetError(DIOOBEXPUSHERROR_NOTCONNEXION);
            }

          return false;
        }

      return true;
    }

  if(sio->GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED)
    {
      haveerror=true;
      SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
    }

  if(GetEvent()==DIOOBEXPUSH_XFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(GetCurrentState())
        {
          case DIOOBEXPUSH_XFSMSTATE_NONE               : break;

          case DIOOBEXPUSH_XFSMSTATE_CONNECT            : { XBUFFER answer;

                                                            if(!ReadAnswer(answer)) break;

                                                            //XDEBUGTRACE_PRINTDATABLOCK(4|XDEBUGTRACELEVEL_WITHCOLOR, answer);

                                                            if(answer.Get()[0]==0xA0)
                                                              {
                                                                sendblocksize  = (XWORD)answer.Get()[5];
                                                                sendblocksize <<=8;
                                                                sendblocksize |= (XWORD)answer.Get()[6];

                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_INIPUSH);
                                                              }
                                                             else
                                                              {
                                                                haveerror = true;
                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                              }
                                                          }
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_INIPUSH            : SetEvent(DIOOBEXPUSH_XFSMEVENT_SENDHEADPUSH);
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH       : SetEvent(DIOOBEXPUSH_XFSMEVENT_RECVHEADPUSH);
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH      : sendbuffer->Delete();

                                                          islastblock = ((filesize - readfilesize) >= sendblocksize)? false:true;

                                                          sendbuffer->Add((!islastblock)?(XBYTE)0x02:(XBYTE)0x82);

                                                          wsize = 0;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          if(islastblock)
                                                                   readfilesizeblock = (filesize - readfilesize);
                                                              else readfilesizeblock = sendblocksize -16;

                                                          sendbuffer->Add((!islastblock)?(XBYTE)0x48:(XBYTE)0x49);

                                                          wsize+=3;

                                                          wsize += (int)readfilesizeblock;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          { XBUFFER datafile((XDWORD)readfilesizeblock, true);

                                                            xfile->Read(datafile.Get(),(XDWORD*)&readfilesizeblock);
                                                            if(!readfilesizeblock)
                                                              {
                                                                haveerror = true;
                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                                break;
                                                              }

                                                            sendbuffer->Add(datafile);
                                                          }

                                                          if(inlittleendian)
                                                                  wsize = SwapWORD((XWORD)sendbuffer->GetSize());
                                                            else  wsize = (XWORD)sendbuffer->GetSize();
                                                          sendbuffer->Set((XBYTE*)&wsize,2,1);

                                                          //XDEBUGTRACE_PRINTDATABLOCK(2|XDEBUGTRACELEVEL_WITHCOLOR, (*sendbuffer));

                                                          sio->Write(sendbuffer->Get(),sendbuffer->GetSize());
                                                          sio->WaitToFlushOutXBuffer(5);

                                                          if(filesize) obexpushfile->SetSendPercent((XBYTE)((readfilesize*100)/filesize));

                                                          SetEvent(DIOOBEXPUSH_XFSMEVENT_RECVBLOCKPUSH);
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH       :
          case DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH      : { XBUFFER answer;

                                                            if(!ReadAnswer(answer)) break;

                                                            //XDEBUGTRACE_PRINTDATABLOCK(4|XDEBUGTRACELEVEL_WITHCOLOR, answer);

                                                            switch(answer.Get()[0])
                                                              {
                                                                case 0x90 : readfilesize+=readfilesizeblock;
                                                                            SetEvent(DIOOBEXPUSH_XFSMEVENT_SENDBLOCKPUSH);
                                                                            break;

                                                                case 0xA0 : haveerror = false;
                                                                            SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                                            break;

                                                                  default : if(GetEvent()==DIOOBEXPUSH_XFSMEVENT_NONE)
                                                                              {
                                                                                haveerror = true;
                                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                                              }
                                                                            break;
                                                              }

                                                          }
                                                          break;

          case DIOOBEXPUSH_XFSMSTATE_ENDPUSH            : return false;
        }
    }
   else //  Nuevo evento
    {
      if(GetEvent()<DIOOBEXPUSH_LASTEVENT)
        {
          CheckTransition();

          switch(GetCurrentState())
            {
              case DIOOBEXPUSH_XFSMSTATE_NONE           : break;

              case DIOOBEXPUSH_XFSMSTATE_CONNECT        : sendisstart = false;

                                                          asktimeelapsed = xtimer->GetMeasureMilliSeconds();

                                                          sendbuffer->Delete();

                                                          sendbuffer->Add((XBYTE)0x80);

                                                          wsize = 0x0007;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          sendbuffer->Add((XBYTE)0x10);
                                                          sendbuffer->Add((XBYTE)0x00);

                                                          wsize = DIOOBEXPUSH_MAXBUFFER;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          //XDEBUGTRACE_PRINTDATABLOCK(2|XDEBUGTRACELEVEL_WITHCOLOR, (*sendbuffer));

                                                          sio->Write(sendbuffer->Get(),sendbuffer->GetSize());
                                                          sio->WaitToFlushOutXBuffer(5);
                                                          break;

              case DIOOBEXPUSH_XFSMSTATE_INIPUSH        : { xtimer->Reset();

                                                            obexpushfile->SetIsSend(false);
                                                            obexpushfile->SetSendPercent(0);

                                                            xpath = (*obexpushfile->GetXPath());

                                                            if((!xfile)||(!xfile->Open(xpath,true))||(!sendbuffer))
                                                              {
                                                                haveerror = true;
                                                                SetEvent(DIOOBEXPUSH_XFSMEVENT_ENDPUSH);
                                                                break;
                                                              }

                                                            readfilesizeblock = 0;

                                                            sendisstart     = true;
                                                            filesize        = xfile->GetSize();
                                                            readfilesize    = 0;
                                                          }
                                                          break;

              case DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH   : sendbuffer->Delete();

                                                          sendbuffer->Add((XBYTE)0x02);

                                                          wsize = 0;
                                                          sendbuffer->Add((XWORD)wsize);

                                                          sendbuffer->Add((XBYTE)0x01);

                                                          // Unicode 16 + 1 byte of head;
                                                          wsize++;
                                                          wsize += (XWORD)((obexpushfile->GetNameSend()->GetSize()+1)*2);
                                                          sendbuffer->Add((XWORD)wsize);

                                                          { XSTRING_CREATENORMALIZE((*obexpushfile->GetNameSend()), buffnormalize, true)
                                                            sendbuffer->Add((XBYTE*)buffnormalize, wsize);
                                                            XSTRING_DELETENORMALIZE(buffnormalize)
                                                          }

                                                          sendbuffer->Add((XBYTE)0xC3);
                                                          sendbuffer->Add((XDWORD)filesize);

                                                          if(inlittleendian)
                                                                  wsize = SwapWORD((XWORD)sendbuffer->GetSize());
                                                            else  wsize = (XWORD)sendbuffer->GetSize();
                                                          sendbuffer->Set((XBYTE*)&wsize,2,1);

                                                          //XDEBUGTRACE_PRINTDATABLOCK(2|XDEBUGTRACELEVEL_WITHCOLOR, (*sendbuffer));

                                                          sio->Write(sendbuffer->Get(),sendbuffer->GetSize());
                                                          sio->WaitToFlushOutXBuffer(5);
                                                          break;

              case DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH   : break;

              case DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH  : break;

              case DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH  : break;

              case DIOOBEXPUSH_XFSMSTATE_ENDPUSH        : if(xfile) xfile->Close();

                                                          if(!haveerror)
                                                            {
                                                              obexpushfile->SetIsSend(true);
                                                              obexpushfile->SetSendPercent(100);
                                                              obexpushfile->SetSendTimeElapsed(xtimer->GetMeasureMilliSeconds());
                                                              obexpushfile->SetError(DIOOBEXPUSHERROR_NONE);

                                                              if((indexfilelist+1<(int)sendfilelist.GetSize()))
                                                                {
                                                                  indexfilelist++;
                                                                  SetEvent(DIOOBEXPUSH_XFSMEVENT_INIPUSH);
                                                                }
                                                            }
                                                           else
                                                            {
                                                              for(int c=0;c<(int)sendfilelist.GetSize();c++)
                                                                {
                                                                  obexpushfile = (DIOOBEXPUSHFILE*)sendfilelist.Get(c);
                                                                  if(obexpushfile)
                                                                    {
                                                                      obexpushfile->SetIsSend(false);

                                                                      if(sendisstart)
                                                                        {
                                                                          obexpushfile->SetError(DIOOBEXPUSHERROR_SENDING);
                                                                        }
                                                                       else
                                                                        {
                                                                          if(sio->IsRefusedConnexion())
                                                                            {
                                                                              obexpushfile->SetError(DIOOBEXPUSHERROR_REFUSEDCONNEXION);
                                                                            }
                                                                           else
                                                                            {
                                                                              obexpushfile->SetError(DIOOBEXPUSHERROR_NOTCONNEXION);
                                                                            }
                                                                        }
                                                                    }
                                                                }

                                                              return false;
                                                            }
                                                          break;
            }
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOOBEXPUSH::ReadAnswer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/06/2013 7:13:41
//
//  @return       bool :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool DIOOBEXPUSH::ReadAnswer(XBUFFER& xbuffer)
{
  if(!diostream) return false;

  int  sizeread;

  xbuffer.Resize(DIOOBEXPUSH_MAXANSWER);

  sizeread = diostream->Read((XBYTE*)xbuffer.Get(), xbuffer.GetSize());
  xbuffer.Resize(sizeread);

  return sizeread?true:false;
}




/*-------------------------------------------------------------------
//  DIOOBEXPUSH_ThreadSend
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/06/2011 16:08:48
//
//  @return       void :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOOBEXPUSH::ThreadSend(void* data)
{
  DIOOBEXPUSH* obexpush = (DIOOBEXPUSH*)data;
  if(!obexpush) return;

  obexpush->SetIsSending(obexpush->CheckSending());
}