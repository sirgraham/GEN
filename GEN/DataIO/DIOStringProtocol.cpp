//------------------------------------------------------------------------------------------
//  DIOSTRINGPROTOCOL.CPP
//
//  Data IO Mod CLI class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/05/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XFSMachine.h"
#include "XDebugTrace.h"

#include "DIOStream.h"

#include "DIOStringProtocol.h"

#include "XMemory.h"


//---- GENERAL VARIABLE -------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::DIOSTRINGPROTOCOL
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/04/2002 13:34:54
//
//  @return       void :

//  @param        diostream :
*/
//-------------------------------------------------------------------
DIOSTRINGPROTOCOL::DIOSTRINGPROTOCOL(DIOSTREAM* diostream)
{
  Clean();

  this->diostream = diostream;
}



//-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::~DIOSTRINGPROTOCOL
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 21:53:15
//
//  @return
//  */
//-------------------------------------------------------------------
DIOSTRINGPROTOCOL::~DIOSTRINGPROTOCOL()
{
  DeleteAllCommand();

  DeleteAllAnswers();

  End();

  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/06/2013 19:29:56
//
//  @return       bool :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::Ini(int timeout)
{
  xtimerout = xfactory->CreateTimer();
  if(!xtimerout) return false;

  xmutexanswers = xfactory->Create_Mutex();
  if(!xmutexanswers) return false;

  if(!diostream)         return false;
  if(!diostream->Open()) return false;

  this->timeout = timeout;

  if(!diostream->WaitToConnected(timeout)) return false;

  return true;
}




//-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::End
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 15:59:02
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOSTRINGPROTOCOL::End()
{
  if(diostream)
    {
      diostream->Disconnect();
      diostream->Close();
    }

  if(xmutexanswers)
    {
      xfactory->Delete_Mutex(xmutexanswers);
      xmutexanswers = NULL;
    }

  if(xtimerout)
    {
      xfactory->DeleteTimer(xtimerout);
      xtimerout = NULL;
    }
}



/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::SendCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/03/2017 14:31:30
//
//  @return       bool :
//
//  @param        command :
//  @param        answer :
//  @param        timeoutanswer :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::SendCommand(XCHAR* command, XSTRING* answer, int timeoutanswer, ...)
{
  va_list arg;

  va_start(arg, timeoutanswer);

  bool status = SendCommand(command, answer, timeoutanswer, arg);

  va_end(arg);

  return status;
}



/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::SendCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2017 11:05:27
//
//  @return       bool :
//
//  @param        command :
//  @param        answer :
//  @param        timeoutanswer :
//  @param        arg :
*/
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::SendCommand(XCHAR* command, XSTRING* answer, int timeoutanswer, va_list& arg)
{
  if(answer) answer->Empty();

  DIOSTRINGPROTOCOLCOMMAND* protocolcommand = GetCommand(command);
  if(!protocolcommand) return false;

  XSTRING  tosend;

  if(!answer) tosend += __L("#");
  tosend += protocolcommand->GetCommand();

  int nparams = protocolcommand->GetNParams();

  for(int c=0;c<nparams;c++)
    {
      XCHAR* param = (XCHAR*)va_arg(arg, XCHAR*);
      if(param)
        {
          tosend += __L(" ");
          tosend += param;
          if(c!=nparams) tosend += __L(" ");
        }
    }

  tosend += __L("\n\r");

  XSTRING_CREATEOEM(tosend, charstr);
  diostream->Write((XBYTE*)charstr, tosend.GetSize());
  XSTRING_DELETEOEM(charstr);


  bool status = true;

  if(answer)
    {
      if(xtimerout) xtimerout->Reset();

      while(1)
        {
          if(!xtimerout)
            {
              status = false;
              break;
            }

          if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeoutanswer)
            {
              status = false;
              break;
            }

          XSTRING* firstanswer = GetFirstAnswer(command);
          if(firstanswer)
            {
              answer->Add(firstanswer->Get());
              DeleteFirstAnswer(command);
              break;
            }

          //XSLEEP::GetInstance().MilliSeconds(10);
        }
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::ReceivedCommandManager
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/02/2017 11:11:08
//
*/
/*-----------------------------------------------------------------*/
void DIOSTRINGPROTOCOL::ReceivedCommandManager()
{
  if(!diostream) return;

  XDWORD size  = diostream->GetInXBuffer()->GetSize();
  if(!size) return;

  for(XDWORD c=0; c<size; c++)
    {
      char character = 0;

      if(diostream->Read((XBYTE*)&character, 1))
        {
          if((character == '\n') || (character == '\r'))
            {
              if(diostream->GetInXBuffer()->GetSize())
                {
                  character = diostream->GetInXBuffer()->GetByte(0);
                  if((character == '\n') || (character == '\r'))
                    {
                      if(diostream->Read((XBYTE*)&character, 1))
                        {

                        }
                    }
                }

              if(!laststringreceived.IsEmpty())
                {
                  XSTRING           command;
                  XSTRING           answer;
                  XSTRING           markanswer;
                  bool              makeanswer = true;


                  if(laststringreceived.Get()[0] == __C('#'))
                    {
                      laststringreceived.DeleteCharacters(0, 1);
                      makeanswer = false;
                    }

                  markanswer.Format(__L("[%s"), DIOSTRINGPROTOCOL_ANSWER);

                  int indexanswer = laststringreceived.Find(markanswer.Get(), true);
                  if((indexanswer == XSTRING_NOTFOUND) || (indexanswer > 0))
                    {
                      XVECTOR<XSTRING*> params;

                      ExtractParamsFromCommand(laststringreceived, command, params);

                      bool status = ReceivedCommand(command, params, answer);
                      if(!status)
                        {
                          answer = DIOSTRINGPROTOCOL_ERROR;
                        }
                       else
                        {
                          if(answer.IsEmpty()) answer = DIOSTRINGPROTOCOL_OK;
                        }

                      if(makeanswer)
                        {
                          XSTRING result;

                          result.Format(__L("[%s%s]:%s\n\r"), DIOSTRINGPROTOCOL_ANSWER, command.Get(), answer.Get());

                          //XDEBUGTRACE_PRINTCOLOR(3, __L("DIOStringProtocol Send Answer: %s"), result.Get());

                          XSTRING_CREATEOEM(result, charstr);
                          diostream->Write((XBYTE*)charstr, result.GetSize());
                          XSTRING_DELETEOEM(charstr);
                        }

                      params.DeleteContents();
                      params.DeleteAll();
                   }
                  else
                   {
                     int startcommand;
                     int startanswer;

                     startcommand = laststringreceived.Find(markanswer.Get(), false);
                     if(startcommand  != XSTRING_NOTFOUND)
                       {
                         startcommand += markanswer.GetSize();

                         startanswer= laststringreceived.Find(__L("]:"), false, startcommand);
                         if(startanswer  != XSTRING_NOTFOUND)
                           {
                             laststringreceived.Copy(startcommand, startanswer, command);

                             laststringreceived.DeleteCharacters(0, startanswer+2);

                             //XDEBUGTRACE_PRINTCOLOR(3, __L("DIOStringProtocol Received Answer: %s %s"), command.Get(), laststringreceived.Get());

                             if(AddAnswer(command.Get(), laststringreceived))
                               {
                                 ReceivedAnswer(laststringreceived);
                               }
                           }
                       }

                   }

                  laststringreceived.Empty();
               }
            }
           else
            {
              if((character >= 0x20) && (character < 0x80)) laststringreceived.Add(character);
            }
        }
    }
}



/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::AddAnswer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2017 8:44:11
//
//  @return       bool :
//
//  @param        command :
//  @param        answer :
*/
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::AddAnswer(XCHAR*command, XSTRING& answer)
{
  DIOSTRINGPROTOCOLANSWER* protocolanswer = new DIOSTRINGPROTOCOLANSWER();
  if(!protocolanswer) return false;

  if(xmutexanswers) xmutexanswers->Lock();

  protocolanswer->GetCommand()->Add(command);
  protocolanswer->GetAnswer()->Add(answer.Get());

  answers.Add(protocolanswer);

  if(xmutexanswers) xmutexanswers->UnLock();

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::GetFirstAnswer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2017 9:57:17
//
//  @return       XSTRING* :
//
*/
/*-----------------------------------------------------------------*/
XSTRING* DIOSTRINGPROTOCOL::GetFirstAnswer(XCHAR* command)
{
  if(answers.IsEmpty()) return NULL;

  XSTRING* answer = NULL;

  if(xmutexanswers) xmutexanswers->Lock();

  for(XDWORD c=0; c<answers.GetSize(); c++)
    {
      DIOSTRINGPROTOCOLANSWER* protocolanswer = answers.Get(c);
      if(protocolanswer)
        {
          if(!protocolanswer->GetCommand()->Compare(command))
            {
               answer = protocolanswer->GetAnswer();
               break;
            }
        }
    }

  if(xmutexanswers) xmutexanswers->UnLock();

  return answer;
}



/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::DeleteFirstAnswer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2017 9:58:04
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::DeleteFirstAnswer(XCHAR* command)
{
  if(answers.IsEmpty()) return false;

  bool status = false;

  if(xmutexanswers) xmutexanswers->Lock();

  for(XDWORD c=0; c<answers.GetSize(); c++)
    {
      DIOSTRINGPROTOCOLANSWER* protocolanswer = answers.Get(c);
      if(protocolanswer)
        {
          if(!protocolanswer->GetCommand()->Compare(command))
            {
              delete protocolanswer;
              answers.DeleteIndex(c);
              status = true;

              break;
            }
        }
    }

  if(xmutexanswers) xmutexanswers->UnLock();

  return status;
}





/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::DeleteAllAnswers
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2017 9:58:10
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::DeleteAllAnswers()
{
  if(answers.IsEmpty()) return false;

  if(xmutexanswers) xmutexanswers->Lock();

  answers.DeleteContents();
  answers.DeleteAll();

  if(xmutexanswers) xmutexanswers->UnLock();

  return true;
}





/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::AddCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/02/2017 16:14:51
//
//  @return       bool :
//
//  @param        command :
//  @param        nparams :
*/
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::AddCommand(XCHAR* command, int nparams)
{
  XSTRING                   commandstring;
  DIOSTRINGPROTOCOLCOMMAND* protocolcommand;

  commandstring = command;
  commandstring.ToLowerCase();

  protocolcommand = GetCommand(commandstring.Get());
  if(protocolcommand) return false;

  protocolcommand = new DIOSTRINGPROTOCOLCOMMAND();
  if(!protocolcommand) return false;

  protocolcommand->Set(commandstring.Get(), nparams);

  return commands.Add(protocolcommand);
}



/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::GetCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:57:19
//
//  @return       DIOSTRINGPROTOCOLCOMMAND* :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
DIOSTRINGPROTOCOLCOMMAND* DIOSTRINGPROTOCOL::GetCommand(XCHAR* command)
{
  XSTRING                   commandstring;
  DIOSTRINGPROTOCOLCOMMAND* protocolcommand;

  commandstring = command;
  commandstring.ToLowerCase();

  for(int c=0;c<(int)commands.GetSize();c++)
    {
       protocolcommand = (DIOSTRINGPROTOCOLCOMMAND*)commands.Get(c);
       if(protocolcommand)
         {
           XSTRING protocolcommandstring;

           protocolcommandstring = protocolcommand->GetCommand();

           if(!protocolcommandstring.Compare(commandstring.Get())) return protocolcommand;
         }
    }

  return NULL;
}



/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::DeleteAllCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:35:30
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::DeleteAllCommand()
{
  if(commands.IsEmpty()) return false;

  commands.DeleteContents();
  commands.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTRINGPROTOCOL::ExtractParamsFromCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/02/2017 13:12:30
//
//  @return       ool :
//
//  @param        stringreceived :
//  @param        command :
//  @param        params :
*/
/*-----------------------------------------------------------------*/
bool DIOSTRINGPROTOCOL::ExtractParamsFromCommand(XSTRING& stringreceived, XSTRING& command, XVECTOR<XSTRING*>& params)
{
  params.DeleteContents();
  params.DeleteAll();

  int firstspace = stringreceived.Find(__L(" "), true);
  if(firstspace == XSTRING_NOTFOUND)
    {
      command = stringreceived.Get();
      return true;
    }
   else
    {
      stringreceived.Copy(0, firstspace, command);
      command.ToLowerCase();
    }

  int index = firstspace+1;

  while(index < (int)stringreceived.GetSize())
    {
      int indexspace = stringreceived.Find(__L(" "), true, index);
      if(indexspace != XSTRING_NOTFOUND)
        {
          XSTRING* param = new XSTRING();
          if(param)
            {
              stringreceived.Copy(index, indexspace, (*param));
              if(!param->IsEmpty())
                {
                  param->ToLowerCase();
                  params.Add(param);
                }
            }

          index = indexspace+1;
        }
       else
        {
          if(index < (int)stringreceived.GetSize())
            {
              XSTRING* param = new XSTRING();
              if(param)
                {
                  stringreceived.Copy(index, stringreceived.GetSize(), (*param));
                  if(!param->IsEmpty())
                    {
                      param->ToLowerCase();
                      params.Add(param);
                    }
                }
            }

           break;
        }
    }

  return true;
}
