//------------------------------------------------------------------------------------------
//  DIOPROTOCOL.CPP
//
//  eXtended class for generic protocols
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/04/2006 11:43:47
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdarg.h>
#include <string.h>

#include "XSleep.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XRand.h"
#include "XFile.h"
#include "XSystem.h"
#include "XFactory.h"
#include "XString.h"
#include "XThreadCollected.h"
#include "XDebug.h"
#include "XVariant.h"

#include "HashCRC32.h"

#include "DIOFactory.h"
#include "DIOStream.h"
#include "DIOProtocolConnexionsManager.h"
#include "DIOProtocolConnexionsInUseDefinitions.h"

#include "DIOProtocolApplicationData.h"

#include "DIOProtocol.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOPROTOCOL_FILE::ReadBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/01/2008 17:48:00
//
//  @return       bool :
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::ReadBlock(XBYTE* buffer)
{
  if(!sourcename[0]) return false;

  if(!xfile->Open(sourcename)) return false;

  bool status = false;

  size = xfile->GetSize();
  if(size)
    {
      xfile->SetPosition(position);

      xfile->Read(buffer,&sizeblock);
      if(sizeblock)
        {
          xfile->GetPosition(position);
          if(size) percent = (XBYTE)(position*100/size);

          status = true;
        }
    }

  xfile->Close();

  return status;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL_FILE::WriteBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/01/2008 17:52:23
//
//  @return       bool :
//  @param        buffer :
//  @param        init :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::WriteBlock(XBYTE* buffer)
{
  if(!xfile)         return false;
  if(!targetname[0]) return false;

  if(position)
    {
      if(!xfile->Open(targetname,false))
        {
          if(!xfile->Create(targetname)) return false;
        }
    }
   else
    {
      if(!xfile->Create(targetname)) return false;
    }

  bool status = false;

  if(sizeblock)
    {
      xfile->SetPosition(XFILE_SEEKEND);

      status = xfile->Write(buffer,sizeblock);
      if(status)
        {
          xfile->GetPosition(position);
          if(size) percent = (XBYTE)(position*100/size);
        }
    }

  xfile->Close();

  return status;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL_FILE::Erase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/01/2008 10:45:17
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL_FILE::Erase()
{
  if(!xfile)         return false;
  if(!targetname[0]) return false;

  xfile->Erase(targetname);

  return true;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::DIOPROTOCOL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/06/2011 10:59:07
//
//  @return       void :


//  @param        publisher :
//  @param        diostream :
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOL::DIOPROTOCOL(DIOSTREAM* diostream)
{
  Clean();

  this->diostream   = diostream;

  answers.SetIsMulti(true);
  asks.SetIsMulti(true);

  RegisterEvent(DIOPROTOCOLXEVENTTYPE_COMMAND);
  RegisterEvent(DIOPROTOCOLXEVENTTYPE_FILEBLOCK);
  RegisterEvent(DIOPROTOCOLXEVENTTYPE_ADDALERT);

  xdatetime         = xfactory->CreateDateTime();
  xtimer            = xfactory->CreateTimer();
  watchdog          = xfactory->CreateTimer();
  xdir              = xfactory->Create_Dir();
  timerfileprogress = xfactory->CreateTimer();


  AddCommand(DIOPROTOCOL_CMDTYPE_SENDISINITSERVERPROTOCOL   , __L("O")                , __L("O")            , RCV_SendIsInitServerProtocol      , __L("SendIsInitServerProtocol")   );
  AddCommand(DIOPROTOCOL_CMDTYPE_GETPROTOCOLVERSION         , __L("")                 , __L("WWW")          , RCV_GetProtocolVersion            , __L("GetProtocolVersion")         );
  AddCommand(DIOPROTOCOL_CMDTYPE_GETAPPLICATIONVERSION      , __L("")                 , __L("WWW")          , RCV_GetApplicationVersion         , __L("GetApplicationVersion")      );
  AddCommand(DIOPROTOCOL_CMDTYPE_PING                       , __L("")                 , __L("O")            , RCV_Ping                          , __L("Ping")                       );
  AddCommand(DIOPROTOCOL_CMDTYPE_GETDATETIME                , __L("")                 , __L("S")            , RCV_GetDateTime                   , __L("GetDateTime")                );
  AddCommand(DIOPROTOCOL_CMDTYPE_SETDATETIME                , __L("S")                , __L("O")            , RCV_SetDateTime                   , __L("SetDateTime")                );
  AddCommand(DIOPROTOCOL_CMDTYPE_INITFILEBLOCK              , __L("SSBD")             , __L("B")            , RCV_InitFileBlock                 , __L("FileInit")                   );
  AddCommand(DIOPROTOCOL_CMDTYPE_GETFILEBLOCK               , __L("")                 , __L("BDDDBAX")      , RCV_GetFileBlock                  , __L("GetFileBlock")               );
  AddCommand(DIOPROTOCOL_CMDTYPE_SENDFILEBLOCK              , __L("BDDDBAX")          , __L("B")            , RCV_SendFileBlock                 , __L("SendFileBlock")              );
  AddCommand(DIOPROTOCOL_CMDTYPE_DELETEFILE                 , __L("SB")               , __L("O")            , RCV_DeleteFile                    , __L("DeleteFile")                 );
  AddCommand(DIOPROTOCOL_CMDTYPE_MAKEDIR                    , __L("S")                , __L("O")            , RCV_MakeDir                       , __L("MakeDir")                    );
  AddCommand(DIOPROTOCOL_CMDTYPE_DELETEDIR                  , __L("SB")               , __L("O")            , RCV_DeleteDir                     , __L("DeleteDir")                  );
  AddCommand(DIOPROTOCOL_CMDTYPE_GETFIRSTDIRELEMENT         , __L("SS")               , __L("SBDD")         , RCV_GetDirElement                 , __L("GetFirstDirElement")         );
  AddCommand(DIOPROTOCOL_CMDTYPE_GETNEXTDIRELEMENT          , __L("")                 , __L("SBDD")         , RCV_GetDirElement                 , __L("GetNextDirElement")          );
  #ifdef DIOALERTS_ACTIVE
  AddCommand(DIOPROTOCOL_CMDTYPE_SENDALERT                  , __L("SSDDDDDBSSS")      , __L("B")            , RCV_SendAlert                     , __L("SendAlert")                  );
  #endif

  fileprogress = new DIOPROTOCOL_FILE();
  if(!fileprogress) return;

  xmutexanswers = xfactory->Create_Mutex();
  if(!xmutexanswers) return;

  xmutexasks  = xfactory->Create_Mutex();
  if(!xmutexasks) return;

  xmutexcommands = xfactory->Create_Mutex();
  if(!xmutexcommands) return;

  xmutexreads = xfactory->Create_Mutex();
  if(!xmutexreads) return;

  xmutexwrites = xfactory->Create_Mutex();
  if(!xmutexwrites) return;

  xmutexfilesprogress = xfactory->Create_Mutex();
  if(!xmutexfilesprogress) return;

  xthreadreceived = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL, __L("DIOPROTOCOL::DIOPROTOCOL received"),ThreadReceived,(void*)this);
  if(xthreadreceived)
    {
      if(!xthreadreceived->Ini()) return;
    }

  xthreadasks = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL, __L("DIOPROTOCOL::DIOPROTOCOL ask"), ThreadAsks, (void*)this);
  if(xthreadasks)
    {
      if(!xthreadasks->Ini()) return;
    }
}


//-------------------------------------------------------------------
//  DIOPROTOCOL::~DIOPROTOCOL
//
/**
//
//  @author       Abraham J. Velez
//  @version      13/04/2006 11:48:23
//
//  @return       virtual :
//  */
//-------------------------------------------------------------------
DIOPROTOCOL::~DIOPROTOCOL()
{
  DeRegisterEvent(DIOPROTOCOLXEVENTTYPE_COMMAND);
  DeRegisterEvent(DIOPROTOCOLXEVENTTYPE_FILEBLOCK);
  DeRegisterEvent(DIOPROTOCOLXEVENTTYPE_ADDALERT);

  doexit = true;

  if(xthreadreceived)
    {
      xthreadreceived->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL, xthreadreceived);
      xthreadreceived = NULL;
    }

  if(xthreadasks)
    {
      xthreadasks->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL, xthreadasks);
      xthreadasks = NULL;
    }

  DeleteAllAnswers();
  DeleteAllAsks();

  DeleteAllCommands();

  xfactory->Delete_Mutex(xmutexanswers);
  xfactory->Delete_Mutex(xmutexasks);
  xfactory->Delete_Mutex(xmutexcommands);
  xfactory->Delete_Mutex(xmutexreads);
  xfactory->Delete_Mutex(xmutexwrites);
  xfactory->Delete_Mutex(xmutexfilesprogress);

  delete fileprogress;

  if(xdir)              xfactory->Delete_Dir(xdir);
  if(watchdog)          xfactory->DeleteTimer(watchdog);
  if(xtimer)            xfactory->DeleteTimer(xtimer);
  if(xdatetime)         xfactory->DeleteDateTime(xdatetime);
  if(timerfileprogress) xfactory->DeleteTimer(timerfileprogress);

  Clean();
}


/*-------------------------------------------------------------------
//  DIOPROTOCOL::Connect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/01/2008 10:01:11
//
//  @return       bool :
//  @param        password :
//  @param        cipher :
//  @param        authenticatekey :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::Connect()
{
  if(!diostream)              return false;
  if(!diostream->GetConfig()) return false;

  if(!diostream->Open())      return false;

  return true;
}



//-------------------------------------------------------------------
//  DIOPROTOCOL::Disconnect
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/04/2006 12:53:44
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool DIOPROTOCOL::Disconnect()
{
  if(!diostream) return false;

  diostream->Disconnect();

  return true;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::Reset
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2008 11:01:45
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::Reset()
{
  if(!Disconnect()) return false;

  return Connect();
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::IsConnected
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/01/2008 19:49:37
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::IsConnected()
{
  if(!diostream)                                                    return false;
  if(diostream->GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED)      return true;

  return false;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::IsConnecting
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/01/2008 10:20:50
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::IsConnecting()
{
  if(!diostream)                                                       return false;
  if(diostream->GetConnectStatus()==DIOSTREAMSTATUS_GETTINGCONNEXION)  return true;

  return false;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::IsDisconnected
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2008 10:52:21
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::IsDisconnected()
{
  if(!diostream)                                                      return false;
  if(diostream->GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED)     return true;

  return false;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::AddCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2012 10:06:48
//
//  @return       bool :
//  @param        type :
//  @param        inmask :
//  @param        outmask :
//  @param        receivedfunc :
//  @param        description :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::AddCommand(XDWORD type,XCHAR* inmask,XCHAR* outmask,DIOPROTOCOL_RECEIVEDFUNC receivedfunc,XCHAR* description)
{
  DIOPROTOCOL_COMMAND* cmd = new DIOPROTOCOL_COMMAND(type,inmask,outmask,receivedfunc,description);
  if(!cmd) return false;

  commands.Add(cmd);

  return true;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::GetCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/01/2008 11:42:40
//
//  @return       DIOPROTOCOL_COMMAND* :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOL_COMMAND* DIOPROTOCOL::GetCommand(XDWORD type)
{
  for(int c=0;c<(int)commands.GetSize();c++)
    {
      DIOPROTOCOL_COMMAND* cmd = (DIOPROTOCOL_COMMAND*)commands.Get(c);
      if(cmd)
        {
          if(cmd->GetType() == type) return cmd;
        }
    }

  return NULL;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::AddAnswer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:01:01
//
//  @return       DIOPROTOCOL_ANSWER* :
//
//  @param        type :
//  @param        ID :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOL_ANSWER* DIOPROTOCOL::AddAnswer(XDWORD type,XDWORD ID, XBUFFER& xbuffer)
{
  if(xmutexanswers) xmutexanswers->Lock();

  DIOPROTOCOL_ANSWER* answer = new DIOPROTOCOL_ANSWER(type, ID, xbuffer);
  if(answer) answers.Add(answer);

  if(xmutexanswers) xmutexanswers->UnLock();

  return answer;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::GetAnswer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:04:35
//
//  @return       DIOPROTOCOL_ANSWER* :
//
//  @param        type :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOL_ANSWER* DIOPROTOCOL::GetAnswer(XDWORD type, XDWORD ID)
{
  if(xmutexanswers) xmutexanswers->Lock();

  DIOPROTOCOL_ANSWER* answer = NULL;

  for(XDWORD c=0; c<answers.GetSize(); c++)
    {
      answer = (DIOPROTOCOL_ANSWER*)answers.Get(c);
      if(answer)
        {
          if((answer->GetType() == type) && (answer->GetID()  == ID))  break; else answer = NULL;
        }
    }

  if(xmutexanswers) xmutexanswers->UnLock();

  return answer;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::DeleteAnswer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:10:53
//
//  @return       bool :
//
//  @param        type :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAnswer(XDWORD type, XDWORD ID)
{
  if(xmutexanswers) xmutexanswers->Lock();

  for(XDWORD c=0; c<answers.GetSize(); c++)
    {
      DIOPROTOCOL_ANSWER* answer = (DIOPROTOCOL_ANSWER*)answers.Get(c);
      if(answer)
        {
          if((answer->GetType() == type) && (answer->GetID() == ID))
            {
              answers.Delete(answer);
              delete answer;

              break;
            }
        }
    }

  if(xmutexanswers) xmutexanswers->UnLock();

  return true;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::DeleteAllAnswers
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:11:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAllAnswers()
{
  if(xmutexanswers) xmutexanswers->Lock();

  bool status = false;

  if(!answers.IsEmpty())
    {
      answers.DeleteContents();
      answers.DeleteAll();

      status = true;
    }

  if(xmutexanswers) xmutexanswers->UnLock();

  return status;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::AddAnswer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:01:01
//
//  @return       DIOPROTOCOL_ANSWER* :
//
//  @param        type :
//  @param        ID :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOL_ANSWER* DIOPROTOCOL::AddAsk(XDWORD type,XDWORD ID, XBUFFER& xbuffer)
{
  if(xmutexasks) xmutexasks->Lock();

  DIOPROTOCOL_ANSWER* ask = new DIOPROTOCOL_ANSWER(type, ID, xbuffer);
  if(ask) asks.Add(ask);

  if(xmutexasks) xmutexasks->UnLock();

  return ask;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::GetAsk
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:31:32
//
//  @return       DIOPROTOCOL_ANSWER* :
//
//  @param        type :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOL_ANSWER* DIOPROTOCOL::GetAsk(XDWORD index)
{
  if(xmutexasks) xmutexasks->Lock();

  DIOPROTOCOL_ANSWER* ask = (DIOPROTOCOL_ANSWER*)asks.Get(index);

  if(xmutexasks) xmutexasks->UnLock();

  return ask;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::DeleteAnswer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:10:53
//
//  @return       bool :
//
//  @param        type :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAsk(XDWORD index)
{
  if(xmutexasks) xmutexasks->Lock();

  DIOPROTOCOL_ANSWER* answer = (DIOPROTOCOL_ANSWER*)asks.Get(index);
  if(answer)
    {
      asks.Delete(answer);
      delete answer;
    }

  if(xmutexasks) xmutexasks->UnLock();

  return true;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::DeleteAllAsks
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:28:11
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAllAsks()
{
  if(xmutexasks) xmutexasks->Lock();

  bool status =  false;

  if(!asks.IsEmpty())
    {
      asks.DeleteContents();
      asks.DeleteAll();

      status = true;
    }

  if(xmutexasks) xmutexasks->UnLock();

  return status;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::ReadCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2016 20:39:15
//
//* @return       int :
//
//  @param        type :
//  @param        ID :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::ReadCommand(XDWORD& type, XDWORD& ID, XBUFFER& xbuffer)
{
  if(!IsConnected()) return 0;

  XDWORD sizecmd = diostream->GetInXBuffer()->GetSize();
  if(sizecmd < DIOPROTOCOL_SIZEOFHEAD) return 0;

  XBYTE head[DIOPROTOCOL_SIZEOFHEAD];

  memset(head ,0 , DIOPROTOCOL_SIZEOFHEAD);
  diostream->GetInXBuffer()->Get(head, DIOPROTOCOL_SIZEOFHEAD);

  HASHCRC32 crc32;
  XDWORD*   headcmd         = (XDWORD*)head;
  XDWORD    IDhead          = 0;
  XDWORD    sizebuffer      = 0;
  XDWORD    bodycrc32value  = 0;
  XDWORD    headcrc32value  = 0;
  XDWORD    crc32value      = 0;

  IDhead          = headcmd[0];
  type            = headcmd[1];
  sizebuffer      = headcmd[2];
  ID              = headcmd[3];
  bodycrc32value  = headcmd[4];
  headcrc32value  = headcmd[5];

  crc32.ResetResult();
  crc32.Do((XBYTE*)headcmd, (DIOPROTOCOL_SIZEOFHEAD - sizeof(XDWORD)));
  crc32value  = crc32.GetResultCRC32();

  if((IDhead != DIOPROTOCOL_MSGIDHEAD) || (headcrc32value != crc32value))
    {
      return 0;
    }

  if(!diostream->WaitToFilledReadingBuffer((DIOPROTOCOL_SIZEOFHEAD + sizebuffer), 5))
    {
      return 0;
    }

  //diostream->GetInXBuffer()->Extract(NULL, 0, DIOPROTOCOL_SIZEOFHEAD);
  diostream->Read(head, DIOPROTOCOL_SIZEOFHEAD);

  if(sizebuffer)
    {
      xbuffer.Delete();
      xbuffer.Resize(sizebuffer);

    //if(diostream->GetInXBuffer()->Extract(xbuffer.Get(), 0, sizebuffer))
      if(diostream->Read(xbuffer.Get(), sizebuffer))
        {
          crc32.ResetResult();
          crc32.Do(xbuffer);
          crc32value  = crc32.GetResultCRC32();

          if(bodycrc32value == crc32value)
            {
              return (DIOPROTOCOL_SIZEOFHEAD + sizebuffer);
            }
           else
            {
              return 0;
            }

        }
       else
        {
          return 0;
        }
    }

  return DIOPROTOCOL_SIZEOFHEAD;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::Received
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/02/2008 19:09:16
//
//  @return       DIOPROTOCOL_RESULT :
//  */
/*-----------------------------------------------------------------*/
DIOPROTOCOL_RESULT DIOPROTOCOL::Received()
{
  if(!IsConnected()) return DIOPROTOCOL_RESULT_NOTCONNEX;

  if(xmutexreads) xmutexreads->Lock();

  int                 size = 0;
  DIOPROTOCOL_RESULT  status = DIOPROTOCOL_RESULT_UNKNOWNCMD;

  XDWORD                type   = DIOPROTOCOL_CMDTYPE_NONE;
  XDWORD                ID     = 0;
  XBUFFER               xbuffer;

  size = ReadCommand(type, ID, xbuffer);
  if(size)
    {
      bool   isanswer = ((type&DIOPROTOCOL_CMDTYPE_ANSWER)==DIOPROTOCOL_CMDTYPE_ANSWER)?true:false;
      XDWORD rtype    = (type&~DIOPROTOCOL_CMDTYPE_ANSWER);

      /*
      DIOPROTOCOL_COMMAND* cmd  = GetCommand(rtype);
      if(cmd)
        {
          XDEBUG_PRINTCOLOR(((isanswer)?1:2), __L("DIOPROTOCOL %s %s : %08X %08X %02d %-35s (%8d)") , GetDIOStream()->GetConfig()->IsServer()?__L("Server  "):__L("Client  ")
                                                                                                    , (isanswer)?__L("Answer  "):__L("Ask     ")
                                                                                                    , this
                                                                                                    , ID
                                                                                                    , rtype
                                                                                                    , cmd->GetDescription()
                                                                                                    , xbuffer.GetSize());
        }
      */
      if(isanswer)
        {
          if(AddAnswer(rtype, ID, xbuffer)) status = DIOPROTOCOL_RESULT_OK;
        }
       else
        {
          //status = ReceivedHandle(rtype, ID, xbuffer);
          if(AddAsk(rtype, ID, xbuffer))    status = DIOPROTOCOL_RESULT_OK;
        }

    }

  if(xmutexreads) xmutexreads->UnLock();

  return status;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::ReceivedHandle
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2016 20:40:53
//
//  @return       DIOPROTOCOL_RESULT :
//
//  @param        type :
//  @param        ID :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOL_RESULT DIOPROTOCOL::ReceivedHandle(XDWORD type, XDWORD ID, XBUFFER& xbuffer)
{
  DIOPROTOCOL_RESULT status     = DIOPROTOCOL_RESULT_UNKNOWNCMD;
  XDWORD             param      = 0;

  if((type & DIOPROTOCOL_CMDTYPE_ANSWER) == DIOPROTOCOL_CMDTYPE_ANSWER) return DIOPROTOCOL_RESULT_UNKNOWNCMD;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(type);
  if(!cmd)  return DIOPROTOCOL_RESULT_UNKNOWNCMD;

  DIOPROTOCOLCONNEXION* connexion =  (DIOPROTOCOLCONNEXION*)GetConnexion();

  if(connexion) connexion->SetInUse(true, DIOPROTOCOLCONNEXIONSINUSEID_ANSWER);

  if(cmd->GetReceivedFunc())
    {
      int ssize = cmd->GetReceivedFunc()(this, cmd, xbuffer, ID, param);

      if(ssize != DIOPROTOCOL_ERRORRECEIVED)
        {
          if(cmd->GetSOUTMask()->GetSize())
            {
              if(ssize)
                {
                  if(SendCommand(type|DIOPROTOCOL_CMDTYPE_ANSWER, ID, xbuffer)) status = DIOPROTOCOL_RESULT_OK;
                }

            } else status = DIOPROTOCOL_RESULT_OK;
        }
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, type, (status==DIOPROTOCOL_RESULT_OK)?true:false, NULL, param);

  if(connexion) connexion->SetInUse(false, DIOPROTOCOLCONNEXIONSINUSEID_ANSWER);

  return status;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::SendCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2008 10:01:17
//
//  @return       bool :
//  @param        type :
//  @param        xbuffer :
//  @param        size :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::SendCommand(XDWORD type,XDWORD& ID, XBUFFER& xbuffer)
{
  if(!IsConnected()) return false;

  HASHCRC32 crc32;
  XDWORD    crc32head = 0;
  XDWORD    crc32body = 0;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(type&~DIOPROTOCOL_CMDTYPE_ANSWER);
  if(!cmd) return false;

  if(xmutexwrites) xmutexwrites->Lock();

  XBYTE   head[DIOPROTOCOL_SIZEOFHEAD];
  XDWORD* ptr_head  = (XDWORD*)head;

  memset(head, 0, DIOPROTOCOL_SIZEOFHEAD);

  if(xbuffer.GetSize())
    {
      crc32.ResetResult();
      crc32.Do(xbuffer);
      crc32body = crc32.GetResultCRC32();
    }

  if(!ID)
    {
      ID = IDmsgcounter;
      ID |= maskID;

      IDmsgcounter++;
      if(IDmsgcounter > DIOPROTOCOL_CMDTYPE_SECUENCIALLIMIT)  IDmsgcounter = 0;
    }

  ptr_head[0] = DIOPROTOCOL_MSGIDHEAD;
  ptr_head[1] = type;
  ptr_head[2] = xbuffer.GetSize();
  ptr_head[3] = ID;
  ptr_head[4] = crc32body;

  crc32.ResetResult();
  crc32.Do(head, DIOPROTOCOL_SIZEOFHEAD-sizeof(XDWORD));
  crc32head = crc32.GetResultCRC32();

  ptr_head[5] = crc32head;

  bool status = false;

  if(xbuffer.GetSize())
    {
      XBUFFER xbuffersend;

      xbuffersend.Add(head, DIOPROTOCOL_SIZEOFHEAD);
      xbuffersend.Add(xbuffer);

      status = (diostream->Write(xbuffersend) == xbuffersend.GetSize())?true:false;
      if(status) diostream->WaitToFlushOutXBuffer(5);
    }
   else
    {
      status = (diostream->Write(head, DIOPROTOCOL_SIZEOFHEAD) == DIOPROTOCOL_SIZEOFHEAD)?true:false;
      if(status) status = diostream->WaitToFlushOutXBuffer(5);
    }

  if(xmutexwrites) xmutexwrites->UnLock();

  return status;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::Command
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/02/2016 18:04:22
//
//  @return       bool :
//
//  @param        type :
//  @param        xbuffer :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::Command(XDWORD type, XBUFFER& xbuffer, int timeout)
{
  if(!IsConnected())  return false;
  if(!watchdog)       return false;

  if(xmutexcommands) xmutexcommands->Lock();

  XDWORD ID     = 0;
  bool   status = false;


  DIOPROTOCOL_COMMAND* cmd = GetCommand(type);
  if(cmd)
    {
      /*
      XDEBUG_PRINTCOLOR(1, __L("DIOPROTOCOL %s Command  : %08X %08X %02d %-35s (%8d)") , GetDIOStream()->GetConfig()->IsServer()?__L("Server  "):__L("Client  ")
                                                                                       , this
                                                                                       , ID
                                                                                       , type
                                                                                       , cmd->GetDescription()
                                                                                       , xbuffer.GetSize());
      */
      if(SendCommand(type, ID, xbuffer))
        {
          /*
          XDEBUG_PRINTCOLOR(1, __L("DIOPROTOCOL %s Send     : %08X %08X %02d %-35s (%8d)") , GetDIOStream()->GetConfig()->IsServer()?__L("Server  "):__L("Client  ")
                                                                                           , this
                                                                                           , ID
                                                                                           , type
                                                                                           , cmd->GetDescription()
                                                                                           , xbuffer.GetSize());
          */
          xbuffer.Delete();

          //--------------------------------------------
          // No Answer
          if(!cmd->GetSOUTMask()->IsEmpty())
            {
              watchdog->Reset();

              while(1)
                {
                  if(!IsConnected()) break;

                  DIOPROTOCOL_ANSWER* answer = GetAnswer(type, ID);
                  if(answer)
                    {
                      xbuffer.Delete();
                      xbuffer.Add(answer->GetBuffer());

                      DeleteAnswer(type, ID);

                      status = true;
                      break;
                    }

                  if((int)watchdog->GetMeasureSeconds() >= timeout)
                    {
                      break;
                    }

                  xsleep->MilliSeconds(10);
                }

            } else status = true;

          if(!status)
            {
              XDEBUG_PRINTCOLOR((status?1:4), __L("DIOPROTOCOL %s Resolved : %08X %08X %02d %-35s (%8d)  %s") , GetDIOStream()->GetConfig()->IsServer()?__L("Server  "):__L("Client  ")
                                                                                                              , this
                                                                                                              , ID
                                                                                                              , type
                                                                                                              , cmd->GetDescription()
                                                                                                              , xbuffer.GetSize()
                                                                                                              , status?__L("Ok"):__L("Error!"));
            }
        }

    }

  if(xmutexcommands) xmutexcommands->UnLock();

  return status;
}






/*-------------------------------------------------------------------
//  DIOPROTOCOL::SendEvent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/02/2012 19:05:59
//
//  @return       bool :
//  @param        type :
//  @param        protocol :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::SendEvent(DIOPROTOCOLXEVENTTYPE type,DIOPROTOCOL* protocol)
{
  DIOPROTOCOLXEVENT xevent(this,type);

  xevent.SetProtocol(protocol);
  PostEvent(&xevent);

  return true;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::SendEventCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2011 19:02:11
//  //  @return       bool :
//  @param        protocol :
//  @param        direction :
//  @param        type :
//  @param        result :
//  @param        param1 :
//  @param        param2 :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::SendEventCommand(DIOPROTOCOL* protocol, DIOPROTOCOL_DIRECTION direction, XDWORD type, bool result, void* param1, XDWORD param2)
{
  DIOPROTOCOLXEVENT xevent(this,DIOPROTOCOLXEVENTTYPE_COMMAND);

  xevent.SetProtocol(protocol);
  xevent.SetDirection(direction);
  xevent.SetType(type);
  xevent.SetResult(result);

  xevent.SetParam1(param1);
  xevent.SetParam2(param2);

  PostEvent(&xevent);

  return true;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::SendEventFileBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2011 18:58:07
//
//  @return       bool :
//  @param        filestatus :
//  @param        fileissend :
//  @param        filesource :
//  @param        filetarget :
//  @param        filepercent :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::SendEventFileBlock(DIOPROTOCOL_FILESTATUS filestatus,bool fileissend,XPATH* filesource,XPATH* filetarget,int filepercent,void* param)
{
  DIOPROTOCOLXEVENT xevent(this,DIOPROTOCOLXEVENTTYPE_FILEBLOCK);

  xevent.SetFileStatus(filestatus);
  xevent.SetFileIsSend(fileissend);
  xevent.SetFileSource(filesource->Get());
  xevent.SetFileTarget(filetarget->Get());
  xevent.SetFilePercent(filepercent);

  xevent.SetParam1(param);

  PostEvent(&xevent);

  return true;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::SetFileProgress
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/02/2012 17:12:46
//
//  @return       bool :
//  @param        direction :
//  @param        sourcename :
//  @param        targetname :
//  @param        CRC32 :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::SetFileProgress(DIOPROTOCOL_DIRECTION direction, XPATH& sourcename, XPATH& targetname, XDWORD CRC32)
{
  if(!xfactory) return false;

  if(sourcename.IsEmpty())  return false;

  fileprogress->Reset();

  fileprogress->SetSourceName(sourcename);

  if(targetname.IsEmpty())
         fileprogress->SetTargetName(sourcename);
    else fileprogress->SetTargetName(targetname);

  fileprogress->SetActive(false);
  fileprogress->SetDirection(direction);
  fileprogress->SetPosition(0);

  if(CRC32)
        fileprogress->SetCRC32(CRC32);
   else fileprogress->SetCRC32(CRC32OfFile(sourcename));

  return true;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_GetProtocolVersion
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/12/2016 10:59:48
//
//  @return       bool :
//
//  @param        version :
//  @param        subversion :
//  @param        subversionerr :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetProtocolVersion(XWORD& version, XWORD& subversion, XWORD& subversionerr)
{
  bool result = false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETPROTOCOLVERSION);
  if(!cmd) return false;

  XBUFFER xbuffer;

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &version, &subversion, &subversionerr);
      result = true;
    }

  XDWORD param = (version<<8)|subversion;

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result, NULL, param);

  return result;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_GetProtocolVersion
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/12/2016 10:57:59
//
//  @return       int :
//
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetProtocolVersion(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(),  protocol->protocolversion,  protocol->protocolsubversion,  protocol->protocolsubversionerr);
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_GetApplicationVersion
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/12/2016 10:57:39
//
//  @return       bool :
//
//  @param        version :
//  @param        subversion :
//  @param        subversionerr :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetApplicationVersion(XWORD& version, XWORD& subversion, XWORD& subversionerr)
{
  bool result = false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETAPPLICATIONVERSION);
  if(!cmd) return false;

  XBUFFER xbuffer;

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &version, &subversion, &subversionerr);
      result = true;
    }

  XDWORD param = (version<<8)|subversion;

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result, NULL, param);

  return result;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_GetApplicationVersion
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/12/2016 10:57:46
//
//  @return       int :
//
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetApplicationVersion(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(),  protocol->applicationversion,  protocol->applicationsubversion,  protocol->applicationsubversionerr);
}





/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_Ping
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2008 10:05:05
//
//  @return       bool :
//  @param        milliseconds :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_Ping(XDWORD& milliseconds, int timeout)
{
  if(!xtimer) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_PING);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XBYTE result = 0;

  xtimer->Reset();

  if(Command(cmd->GetType(), xbuffer, timeout))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  milliseconds = (XDWORD)xtimer->GetMeasureMilliSeconds();

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(),result?true:false,NULL,milliseconds);

  return result?true:false;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_Ping
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2012 17:18:59
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_Ping(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  bool result = true;

  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_SendIsInitServerProtocol
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/06/2016 13:11:09
//
//  @return       bool :
//
//  @param        isinit :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_SendIsInitServerProtocol(bool isinit)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_SENDISINITSERVERPROTOCOL);
  if(!cmd) return false;

  XBUFFER  xbuffer;
  XBYTE    _isinit    = isinit?1:0;
  bool     result     = false;

  xbuffer.AddWithMask(cmd->GetSINMask(), _isinit);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::SendIsInitServerProtocol
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/06/2016 13:11:27
//
//  @return       int :
//
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_SendIsInitServerProtocol(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XBYTE  isinit;
  bool   result = false;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &isinit);

  if(isinit)
    {
      protocol->isserverinitialized = true;
      result                        = true;
    }

  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_GetDateTime
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/02/2012 18:58:07
//
//  @return       bool :
//  @param        datestring :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetDateTime(XSTRING& datestring)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETDATETIME);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XBYTE   result = 0;

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &datestring);

      if(!datestring.IsEmpty()) result = 1;
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(),result?true:false, NULL, 0);

  return result?true:false;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_GetDateTime
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2012 17:54:59
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetDateTime(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XSTRING datestring;

  protocol->GetDateTime()->Read();
  protocol->GetDateTime()->GetDateTimeToString(XDATETIME_FORMAT_STANDARD, datestring);

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), &datestring);
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_SetDateTime
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/02/2012 18:29:15
//
//  @return       bool :
//  @param        datestring :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_SetDateTime(XCHAR* datestring)
{
  if(!datestring) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_SETDATETIME);
  if(!cmd) return false;

  XBUFFER  xbuffer;
  XSTRING  _datestring;
  bool     result     = 0;

  _datestring = datestring;

  xbuffer.AddWithMask(cmd->GetSINMask(), &_datestring);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}





/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_SetDateTime
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2012 18:05:26
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_SetDateTime(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XSTRING datestring;
  bool    result = false;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &datestring);

  if(protocol->GetDateTime()->GetDateTimeFromString(datestring, XDATETIME_FORMAT_STANDARD))
    {
      protocol->GetDateTime()->Write();
      result = true;
    }

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_File
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/03/2009 09:18:41 p.m.
//
//  @return       bool :
//  @param        direction :
//  @param        sourcename :
//  @param        targetname :
//  @param        timeout :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_File(DIOPROTOCOL_DIRECTION direction, XCHAR* sourcename, XCHAR* targetname, int timeout, void* param)
{
  if(!xfactory)  return false;
  if(!xtimer)    return false;

  if(!sourcename) return 0;
  if(!targetname) return 0;

  XPATH _sourcename;
  XPATH _targetname;

  _sourcename = sourcename;
  _targetname = targetname;

  if(_sourcename.IsEmpty()) return false;
  if(_targetname.IsEmpty()) return false;

  if(!SetFileProgress(direction,_sourcename,_targetname,0)) return false;

  if(!CMD_InitFileBlock(fileprogress)) return false;

  bool status  = false;

  SendEventFileBlock(DIOPROTOCOL_FILESTATUS_REQUEST , fileprogress->IsActive()
                                                    , fileprogress->GetSourceName()
                                                    , fileprogress->GetTargetName()
                                                    , fileprogress->GetPercent()
                                                    , param);
  xtimer->Reset();

  while(1)
    {
      if(fileprogress->IsActive())
        {
          switch(fileprogress->GetDirection())
            {
              case DIOPROTOCOL_DIRECTION_UNKNOWN  : fileprogress->SetActive(false);     break;
              case DIOPROTOCOL_DIRECTION_GET      : CMD_GetFileBlock(fileprogress);     break;
              case DIOPROTOCOL_DIRECTION_SEND     : CMD_SendFileBlock(fileprogress);    break;
            }
        }

      SendEventFileBlock(DIOPROTOCOL_FILESTATUS_WAIT , fileprogress->IsActive()
                                                     , fileprogress->GetSourceName()
                                                     , fileprogress->GetTargetName()
                                                     , fileprogress->GetPercent()
                                                     , param);

      xsleep->MilliSeconds(10);

      if(!IsConnected())                              break;
      if((int)xtimer->GetMeasureSeconds()>=timeout)   break;
      if(!fileprogress->IsActive())                   break;
    }

  if(fileprogress->GetPercent()>=100)  status = true;

  DIOPROTOCOL_FILESTATUS fstatus = DIOPROTOCOL_FILESTATUS_END;
  if(!status) fstatus = DIOPROTOCOL_FILESTATUS_ERROR;

  SendEventFileBlock(fstatus , fileprogress->IsActive()
                             , fileprogress->GetSourceName()
                             , fileprogress->GetTargetName()
                             , fileprogress->GetPercent()
                             , param);
  return status;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_DeleteFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/07/2010 18:52:37
//
//  @return       bool :
//  @param        namefile :
//  @param        overwrite :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_DeleteFile(XCHAR* namefile, bool overwrite)
{
  if(!namefile) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_DELETEFILE);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XPATH   _namefile;
  bool    result    = false;

  _namefile = namefile;
  if(_namefile.IsEmpty()) return false;

  xbuffer.AddWithMask(cmd->GetSINMask(), &_namefile, overwrite?1:0);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_DeleteFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2012 20:01:34
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        buffer :
//  @param        size :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_DeleteFile(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH path;
  XBYTE overwrite = 0;
  bool  result    = false;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &path, &overwrite);

  XFILE* xfile = xfactory->Create_File();
  if(xfile)
    {
      result = xfile->Erase(path,overwrite?true:false)?true:false;
      xfactory->Delete_File(xfile);
    }

  xbuffer.Delete();

  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_MakeDir
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2012 9:50:44
//
//  @return       bool :
//  @param        namedir :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_MakeDir(XCHAR* namedir)
{
  if(!namedir) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_MAKEDIR);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XPATH   _namedir;
  bool    result    = false;

  _namedir = namedir;
  if(_namedir.IsEmpty()) return false;

  xbuffer.AddWithMask(cmd->GetSINMask(), &_namedir);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_MakeDir
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2012 9:52:32
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_MakeDir(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH path;
  bool  result = 0;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &path);

  result = protocol->GetXDir()->Make(path.Get())?true:false;

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);

}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_DeleteDir
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2012 9:57:43
//
//  @return       bool :
//  @param        namedir :
//  @param        all :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_DeleteDir(XCHAR* namedir, bool all)
{
  if(!namedir) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_DELETEDIR);
  if(!cmd) return false;

  XBUFFER xbuffer;
  XPATH   _namedir;
  bool    result    = false;

  _namedir = namedir;
  if(_namedir.IsEmpty()) return false;

  xbuffer.AddWithMask(cmd->GetSINMask(), &_namedir, all?1:0);

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_DeleteDir
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2012 9:57:48
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_DeleteDir(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH path;
  XBYTE all   = 0;
  bool  result = 0;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &path, &all);

  result = protocol->GetXDir()->Delete(path.Get(),all?true:false);

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), 0, result);
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_GetDirElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/07/2010 11:12:49
//
//  @return       bool :
//  @param        pathsearch :
//  @param        mask :
//  @param        namefile :
//  @param        type :
//  @param        sizefile :
//  @param        CRC32 :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetDirElement(XCHAR* pathsearch, XCHAR* mask, XPATH& namefile, XBYTE& type, int& sizefile, XDWORD& CRC32)
{
  if(!pathsearch) return false;
  if(!mask)       return false;

  XBUFFER   xbuffer;
  XPATH     _pathsearch;
  XSTRING   _mask;
  bool      result      = false;

  _pathsearch = pathsearch;
  _mask       = mask;

  DIOPROTOCOL_COMMAND* cmd = GetCommand((!_pathsearch.IsEmpty())?DIOPROTOCOL_CMDTYPE_GETFIRSTDIRELEMENT:DIOPROTOCOL_CMDTYPE_GETNEXTDIRELEMENT);
  if(!cmd) return false;

  if(_mask.IsEmpty()) _mask = __L("*");

  xbuffer.AddWithMask(cmd->GetSINMask(), &_pathsearch, &_mask);

  if(Command(cmd->GetType(), xbuffer))
    {
      namefile.Empty();

      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &namefile, &type, &sizefile, &CRC32);

      if(!namefile.IsEmpty()) result = true;
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result?true:false, NULL, 0);

  return result?true:false;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_GetDirElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2012 22:53:58
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetDirElement(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH     pathsearch;
  XSTRING   masksearch;
  XPATH     pathfile;
  XBYTE     typefile     = 0;
  int       sizefile     = 0;
  XDWORD    crc32value  = 0;
  bool      found       = false;

  pathfile.Empty();

  if(cmd->GetType()==DIOPROTOCOL_CMDTYPE_GETFIRSTDIRELEMENT)
    {
      pathsearch.Empty();

      xbuffer.GetWithMask(cmd->GetSINMask(), 0, &pathsearch, &masksearch);

      protocol->GetPathSearch()->Set(pathsearch);
      protocol->GetSearchElement()->DeleteFindFileData();

      found = protocol->GetXDir()->FirstSearch(pathsearch, masksearch, protocol->GetSearchElement());
    }
   else found =  protocol->GetXDir()->NextSearch(protocol->GetSearchElement());

  if(found)
    {
      typefile    = (XBYTE)protocol->GetSearchElement()->GetType();

      if(protocol->GetSearchElement()->GetType() == XDIRELEMENTTYPE_FILE)
        {
          pathfile  = protocol->GetPathSearch()->Get();
          pathfile += protocol->GetSearchElement()->GetNameFile()->Get();

          HASHCRC32 crc32;

          if(crc32.Do(pathfile, sizefile)) crc32value = crc32.GetResultCRC32();
        }

      pathfile  = protocol->GetSearchElement()->GetNameFile()->Get();
    }

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), &pathfile, typefile, sizefile, crc32value);
}





/*-------------------------------------------------------------------
//  DIOPROTOCOL::DeleteAllCommands
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/01/2008 11:36:09
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::DeleteAllCommands()
{
  if(commands.IsEmpty())  return false;

  commands.DeleteContents();
  commands.DeleteAll();

  return true;
}





/*-------------------------------------------------------------------
//  DIOPROTOCOL::CRC32OfFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/07/2009 10:09:23
//
//  @return       XDWORD :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOPROTOCOL::CRC32OfFile(XPATH& xpath)
{
  XDWORD value = 0;

  HASHCRC32* crc32 = new HASHCRC32();
  if(!crc32) return false;

  XFILE* file = xfactory->Create_File();
  if(file)
    {
      if(file->Open(xpath))
        {
          crc32->ResetResult();
          crc32->Do(file, file->GetSize(), 0);
          value  = crc32->GetResultCRC32();

          file->Close();
        }

      xfactory->Delete_File(file);
    }

  delete crc32;

  return value;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_InitFileBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2008 12:06:24
//
//  @return       bool :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_InitFileBlock(DIOPROTOCOL_FILE* fp)
{
  if(!fp) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_INITFILEBLOCK);
  if(!cmd) return false;

  XBUFFER xbuffer;
  bool    result = false;

  xbuffer.AddWithMask(cmd->GetSINMask(), fp->GetSourceName()
                                       , fp->GetTargetName()
                                       , fp->GetDirection()
                                       , fp->GetCRC32());


  if(Command(cmd->GetType(), xbuffer))
    {
      if(xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result))
        {
          if(result) GetFileProgress()->SetActive(true);
        }
    }

  SendEventCommand(this, fp->GetDirection(), cmd->GetType(), result, NULL, 0);

  return result;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_InitFileBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2012 23:42:55
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_InitFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XPATH                  sourcename;
  XPATH                  targetname;
  XBYTE                  _direction   = 0;
  DIOPROTOCOL_DIRECTION  direction    = (DIOPROTOCOL_DIRECTION)_direction;
  XDWORD                 CRC32        = 0;
  bool                   result       = false;

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &sourcename, &targetname, &_direction, &CRC32);

  direction = (DIOPROTOCOL_DIRECTION)_direction;

  if(protocol->SetFileProgress(direction, sourcename, targetname, CRC32))
    {
      protocol->GetFileProgress()->SetActive(true);
      result = true;
    }

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}





/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_GetFileBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/02/2012 13:49:56
//
//  @return       bool :
//  @param        fp :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_GetFileBlock(DIOPROTOCOL_FILE* fp)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_GETFILEBLOCK);
  if(!cmd) return false;

  if(!fp) return false;

  if(!fp->IsActive()) return false;

  XBUFFER xbuffer;
  bool    status = false;

  if(Command(cmd->GetType(), xbuffer))
    {
      XBYTE* filebuffer = new XBYTE[DIOPROTOCOL_MAXFILEBLOCK];
      if(filebuffer)
        {
          XBYTE   islastblock   = 0;
          XDWORD  size          = 0;
          XDWORD  crc32         = 0;
          XDWORD  sizeblock     = DIOPROTOCOL_MAXFILEBLOCK;
          XBYTE   percent       = 0;

          memset(filebuffer, 0, DIOPROTOCOL_MAXFILEBLOCK);

          if(xbuffer.GetWithMask(cmd->GetSOUTMask(), 0  , &islastblock
                                                        , &size
                                                        , &crc32
                                                        , &sizeblock
                                                        , &percent
                                                        , filebuffer
                                                        , &sizeblock))

            {
              fp->SetSize(size);
              fp->SetCRC32(crc32);
              fp->SetSizeBlock(sizeblock);

              if(fp->GetSizeBlock()) status = fp->WriteBlock(filebuffer);

              if(islastblock)
                {
                  XDWORD valuecrc = CRC32OfFile((*fp->GetTargetName()));
                  if(crc32!=valuecrc) fp->Erase();

                  fp->SetActive(false);
                }
            }

          delete[] filebuffer;
        }
    }

  SendEventCommand(this, fp->GetDirection(), cmd->GetType(), status, NULL, 0);

  return status;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_GetFileBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2012 23:52:35
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_GetFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  DIOPROTOCOL_FILE*   fp;
  int                 ssize  = 0;
  bool                status = false;

  XBYTE* filebuffer = new XBYTE[DIOPROTOCOL_MAXFILEBLOCK];
  if(filebuffer)
    {
      memset(filebuffer, 0, DIOPROTOCOL_MAXFILEBLOCK);

      fp = protocol->GetFileProgress();
      if(fp)
        {
          XBYTE islastblock = 0;

          fp->SetSizeBlock(DIOPROTOCOL_MAXFILEBLOCK);

          fp->ReadBlock(filebuffer);

          //if(fp->GetSizeBlock()!=DIOPROTOCOL_MAXFILEBLOCK)  islastblock = 1;
          if(fp->GetSize() == fp->GetPosition()) islastblock = 1;

          if(fp->GetSizeBlock())
            {
              xbuffer.Delete();
              ssize = xbuffer.AddWithMask(cmd->GetSOUTMask(), islastblock
                                                            , fp->GetSize()
                                                            , fp->GetCRC32()
                                                            , fp->GetSizeBlock()
                                                            , fp->GetPercent()
                                                            , filebuffer
                                                            , fp->GetSizeBlock());



              param  = 1;
              status = true;
            }

          if(islastblock) fp->SetActive(false);
        }

      delete[] filebuffer;
    }

  if(!status) ssize = 0;

  return ssize;
}





/*-------------------------------------------------------------------
//  DIOPROTOCOL::CMD_SendFileBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/02/2012 13:49:24
//
//  @return       bool :
//  @param        fp :
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_SendFileBlock(DIOPROTOCOL_FILE* fp)
{
  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_SENDFILEBLOCK);
  if(!cmd) return false;

  if(!fp) return false;

  if(!fp->IsActive()) return false;

  XBUFFER xbuffer;
  bool    status = false;


  XBYTE* filebuffer = new XBYTE[DIOPROTOCOL_MAXFILEBLOCK];
  if(filebuffer)
    {
      XBYTE islastblock = 0;

      fp->SetSizeBlock(DIOPROTOCOL_MAXFILEBLOCK);

      fp->ReadBlock(filebuffer);

      //if(fp->GetSizeBlock()!=DIOPROTOCOL_MAXFILEBLOCK)  islastblock = 1;
      if(fp->GetSize() == fp->GetPosition()) islastblock = 1;

      if(fp->GetSizeBlock())
        {
          xbuffer.Delete();
          XDWORD size = xbuffer.AddWithMask(cmd->GetSINMask(), islastblock
                                                             , fp->GetSize()
                                                             , fp->GetCRC32()
                                                             , fp->GetSizeBlock()
                                                             , fp->GetPercent()
                                                             , filebuffer
                                                             , fp->GetSizeBlock());


          if(Command(cmd->GetType(), xbuffer))
            {
              XBYTE result = 0;

              if(xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result))
                {
                  if(result) status = true;
                }

              if(islastblock) fp->SetActive(false);
            }
        }

      delete[] filebuffer;
    }

  SendEventCommand(this, fp->GetDirection(), cmd->GetType(), status, NULL, 0);

  return status;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::RCV_SendFileBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2012 0:07:19
//
//  @return       int :
//  @param        protocol :
//  @param        cmd :
//  @param        xbuffer :
//  @param        rID :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_SendFileBlock(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XDWORD  filesize    = 0;
  XDWORD  crc32       = 0;
  XDWORD  sizeblock   = DIOPROTOCOL_MAXFILEBLOCK;
  XBYTE percent     = 0;
  int   ssize       = 0;
  bool  status      = false;

  XBYTE* filebuffer = new XBYTE[DIOPROTOCOL_MAXFILEBLOCK];
  if(filebuffer)
    {
      XBYTE islastblock = 0;

      memset(filebuffer,0,DIOPROTOCOL_MAXFILEBLOCK);

      xbuffer.GetWithMask(cmd->GetSINMask(), 0, &islastblock
                                              , &filesize
                                              , &crc32
                                              , &sizeblock
                                              , &percent
                                              , filebuffer
                                              , &sizeblock);

      DIOPROTOCOL_FILE* fp = protocol->GetFileProgress();
      if(fp)
        {
          fp->SetSize(filesize);
          fp->SetCRC32(crc32);
          fp->SetSizeBlock(sizeblock);

          fp->WriteBlock(filebuffer);

          if(fp->GetSizeBlock())
            {
              xbuffer.Delete();
              ssize = xbuffer.AddWithMask(cmd->GetSOUTMask(), fp->GetSizeBlock()?1:0);

              param  = 1;
              status = true;
            }

          if(islastblock)
            {
              XDWORD valuecrc = protocol->CRC32OfFile((*fp->GetTargetName()));
              if(crc32!=valuecrc) fp->Erase();

              fp->SetActive(false);
            }
        }

      delete[] filebuffer;
    }

  if(!status) ssize = 0;

  return ssize;
}



#ifdef DIOALERTS_ACTIVE

/*-------------------------------------------------------------------
//   DIOPROTOCOL::CMD_SendAlert
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version
//
//  @return   bool :
//
//  @param    DIOALERT* :
//
*//*-----------------------------------------------------------------*/
bool DIOPROTOCOL::CMD_SendAlert(DIOALERT* alert)
{
  if(!alert) return false;

  DIOPROTOCOL_COMMAND* cmd = GetCommand(DIOPROTOCOL_CMDTYPE_SENDALERT);
  if(!cmd) return false;

  XBUFFER  xbuffer;
  XSTRING  datetimestr;
  XDWORD   applicationversion;
  XDWORD   applicationsubversion;
  XDWORD   applicationsubversionerr;
  bool     result     = false;

  alert->GetDateTime()->GetDateTimeToString(XDATETIME_FORMAT_STANDARD, datetimestr);
  alert->GetApplicationVersion(applicationversion, applicationsubversion, applicationsubversionerr);

  xbuffer.AddWithMask(cmd->GetSINMask() , &datetimestr
                                        , alert->GetApplicationID()
                                        , applicationversion
                                        , applicationsubversion
                                        , applicationsubversionerr
                                        , alert->GetID()
                                        , alert->GetType()
                                        , (XBYTE)alert->GetLevel()
                                        , alert->GetOrigin()
                                        , alert->GetTitle()
                                        , alert->Get_Message());

  if(Command(cmd->GetType(), xbuffer))
    {
      xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &result);
    }

  SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

  return result;
}




/*-------------------------------------------------------------------
//   DIOPROTOCOL::RCV_SendAlert
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version

//  @return   int :
//
//  @param    DIOPROTOCOL* :
//  @param    DIOPROTOCOL_COMMAND* :
//  @param    XBUFFER& :
//  @param    XDWORD :
//  @param    XDWORD& :
//
*//*-----------------------------------------------------------------*/
int DIOPROTOCOL::RCV_SendAlert(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
  XSTRING  datetimestr;
  XSTRING  applicationID;
  XDWORD   applicationversion       = 0;
  XDWORD   applicationsubversion    = 0;
  XDWORD   applicationsubversionerr = 0;
  XDWORD   ID                       = 0;
  XDWORD   type                     = 0;
  XBYTE    level                    = 0;
  XSTRING  origin;
  XSTRING  title;
  XSTRING  message;
  bool     result                   = false;

  datetimestr.AdjustSize(_MAXSTR);
  applicationID.AdjustSize(_MAXSTR);
  origin.AdjustSize(_MAXSTR);
  title.AdjustSize(_MAXSTR);
  message.AdjustSize(2048);

  xbuffer.GetWithMask(cmd->GetSINMask(), 0, &datetimestr
                                          , &applicationID
                                          , &applicationversion
                                          , &applicationsubversion
                                          , &applicationsubversionerr
                                          , &ID
                                          , &type
                                          , &level
                                          , &origin
                                          , &title
                                          , &message);

  datetimestr.AdjustSize();
  applicationID.AdjustSize();
  origin.AdjustSize();
  title.AdjustSize();
  message.AdjustSize();

  if((!title.IsEmpty())  && (!origin.IsEmpty()) && (!message.IsEmpty()))
    {
      DIOALERT  alert;

      alert.GetDateTime()->GetDateTimeFromString(datetimestr, XDATETIME_FORMAT_STANDARD);
      alert.GetApplicationID()->Set(applicationID);
      alert.SetApplicationVersion(applicationversion, applicationsubversion, applicationsubversionerr);
      alert.SetID(ID);
      alert.SetType(type);
      alert.SetLevel((DIOALERTLEVEL)level);
      alert.GetOrigin()->Set(origin);
      alert.GetTitle()->Set(title);
      alert.Get_Message()->Set(message);

      if(protocol->GetApplicationData())
        {
          DIOPROTOCOLAPPLICATIONDATA* applicationdata = (DIOPROTOCOLAPPLICATIONDATA*)protocol->GetApplicationData();
          if(applicationdata)  result = applicationdata->AddAlert(alert);

          if(result)
            {
              DIOPROTOCOLXEVENT xevent(protocol, DIOPROTOCOLXEVENTTYPE_ADDALERT);

              xevent.SetProtocol(protocol);
              xevent.SetAlert(&alert);

              protocol->PostEvent(&xevent);
            }
        }
    }

  xbuffer.Delete();
  return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


#endif






/*-------------------------------------------------------------------
//  DIOPROTOCOL::ThreadReceived
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2008 20:42:14
//
//  @return       void :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOPROTOCOL::ThreadReceived(void* data)
{
  DIOPROTOCOL* protocol = (DIOPROTOCOL*)data;
  if(!protocol) return;

  if(protocol->doexit) return;

  if(!protocol->xthreadreceived)              return;
  if(!protocol->xthreadreceived->IsRunning()) return;

  if(protocol->GetDIOStream())
    {
      if(protocol->IsConnected())
        {
          protocol->Received();
        }
    }
}




/*-------------------------------------------------------------------
//  DIOPROTOCOL::ThreadAsk
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2016 11:50:43
//
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOPROTOCOL::ThreadAsks(void* data)
{
  DIOPROTOCOL* protocol = (DIOPROTOCOL*)data;
  if(!protocol) return;

  if(protocol->doexit) return;

  if(!protocol->xthreadreceived)              return;
  if(!protocol->xthreadreceived->IsRunning()) return;

  if(protocol->GetDIOStream())
    {
      if(protocol->IsConnected())
        {
          if(!protocol->asks.IsEmpty())
            {
              DIOPROTOCOL_ANSWER* ask = protocol->GetAsk(0);
              if(ask)
                {
                  XDWORD t = ask->GetType();
                  protocol->ReceivedHandle(ask->GetType(), ask->GetID(), ask->GetBuffer());
                  protocol->DeleteAsk(0);
                }
            }

          //if(protocol->xtimer) protocol->xsleep->MilliSeconds(50);
        }
    }
}
