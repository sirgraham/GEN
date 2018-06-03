//------------------------------------------------------------------------------------------
//  DIOCLICLIENT.CPP
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
#include "XFSMachine.h"

#include "DIOStream.h"
#include "DIOCLIClient.h"

#include "XMemory.h"


//---- GENERAL VARIABLE -------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::DIOCLICLIENTCOMMAND
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:27:15
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOCLICLIENTCOMMAND::DIOCLICLIENTCOMMAND()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::~DIOCLICLIENTCOMMAND
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:27:19
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOCLICLIENTCOMMAND::~DIOCLICLIENTCOMMAND()
{
  DeleteAllAnswers();
  DeleteAllParams();

  Clean();
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::GetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 12:06:16
//
//  @return       DIOCLICLIENT_TYPECOMMAND :
//  */
/*-----------------------------------------------------------------*/
int DIOCLICLIENTCOMMAND::GetType()
{
  return type;
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::GetCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:43:56
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* DIOCLICLIENTCOMMAND::GetCommand()
{
  return command.Get();
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:48:06
//
//  @return       bool :
//  @param        type :
//  @param        command :
//  @param        answer :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENTCOMMAND::Set(int type, XCHAR* command)
{
  this->type    = type;
  this->command = command;

  return true;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::AddParam
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 22:57:54
//
//  @return       bool :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENTCOMMAND::AddParam(XSTRING* param)
{
  params.Add(param);

  return true;
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::GetParam
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:43:56
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* DIOCLICLIENTCOMMAND::GetParam(int index)
{
  XSTRING* param = (XSTRING*)params.Get(index);
  if(!param) return NULL;

  return param->Get();
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::GetNParams
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 23:35:39
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOCLICLIENTCOMMAND::GetNParams()
{
  return params.GetSize();
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::DeleteAllParams
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 23:00:44
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOCLICLIENTCOMMAND::DeleteAllParams()
{
  if(params.IsEmpty()) return false;

  params.DeleteContents();

  params.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::AddAnswer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 22:57:54
//
//  @return       bool :
//  @param        answer :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENTCOMMAND::AddAnswer(XCHAR* answer)
{
  XSTRING* _answer = new XSTRING(answer);

  if(_answer)  answers.Add(_answer);

  return true;
}




/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::AddAnswer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/10/2011 13:09:17
//
//  @return       bool :
//  @param        answer :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENTCOMMAND::AddAnswer(XSTRING* answer)
{
  if(!answer) return false;

  answers.Add(answer);

  return true;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::GetNAnswers
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 23:35:39
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOCLICLIENTCOMMAND::GetNAnswers()
{
  return answers.GetSize();
}




/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::GetAnswer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/10/2011 13:20:44
//
//  @return       XSTRING* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XSTRING* DIOCLICLIENTCOMMAND::GetAnswer(int index)
{
  XSTRING* answer = (XSTRING*)answers.Get(index);
  if(!answer) return NULL;

  return answer;
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::DeleteAllAnswers
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 23:00:44
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOCLICLIENTCOMMAND::DeleteAllAnswers()
{
  if(answers.IsEmpty()) return false;

  answers.DeleteContents();
  answers.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  DIOCLICLIENTCOMMAND::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:27:23
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOCLICLIENTCOMMAND::Clean()
{

}



//-------------------------------------------------------------------
//  DIOCLICLIENT::DIOCLICLIENT
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
DIOCLICLIENT::DIOCLICLIENT(DIOSTREAM* diostream) : XFSMACHINE(0)
{
  Clean();

  this->diostream = diostream;

  AddState( DIOCLICLIENT_XFSMSTATE_NONE         ,
            DIOCLICLIENT_XFSMEVENT_INICMD       , DIOCLICLIENT_XFSMSTATE_INICMD       ,
            DIOCLICLIENT_XFSMEVENT_SENDCMD      , DIOCLICLIENT_XFSMSTATE_SENDCMD      ,
            DIOCLICLIENT_XFSMEVENT_ENDCMD       , DIOCLICLIENT_XFSMSTATE_ENDCMD       ,
            EVENTDEFEND);

  AddState( DIOCLICLIENT_XFSMSTATE_INICMD       ,
            DIOCLICLIENT_XFSMEVENT_NONE         , DIOCLICLIENT_XFSMSTATE_NONE         ,
            DIOCLICLIENT_XFSMEVENT_SENDCMD      , DIOCLICLIENT_XFSMSTATE_SENDCMD      ,
            DIOCLICLIENT_XFSMEVENT_ENDCMD       , DIOCLICLIENT_XFSMSTATE_ENDCMD       ,
            EVENTDEFEND);

  AddState( DIOCLICLIENT_XFSMSTATE_SENDCMD      ,
            DIOCLICLIENT_XFSMEVENT_NONE         , DIOCLICLIENT_XFSMSTATE_NONE         ,
            DIOCLICLIENT_XFSMEVENT_RECEIVEDCMD  , DIOCLICLIENT_XFSMSTATE_RECEIVEDCMD  ,
            DIOCLICLIENT_XFSMEVENT_ENDCMD       , DIOCLICLIENT_XFSMSTATE_ENDCMD       ,
            EVENTDEFEND);

  AddState( DIOCLICLIENT_XFSMSTATE_RECEIVEDCMD  ,
            DIOCLICLIENT_XFSMEVENT_NONE         , DIOCLICLIENT_XFSMSTATE_NONE         ,
            DIOCLICLIENT_XFSMEVENT_SENDCMD      , DIOCLICLIENT_XFSMSTATE_SENDCMD      ,
            DIOCLICLIENT_XFSMEVENT_ENDCMD       , DIOCLICLIENT_XFSMSTATE_ENDCMD       ,
            EVENTDEFEND);

  AddState( DIOCLICLIENT_XFSMSTATE_ENDCMD       ,
            DIOCLICLIENT_XFSMEVENT_NONE         , DIOCLICLIENT_XFSMSTATE_NONE         ,
            DIOCLICLIENT_XFSMEVENT_INICMD       , DIOCLICLIENT_XFSMSTATE_INICMD       ,
            EVENTDEFEND);

  timer = xfactory->CreateTimer();
}



//-------------------------------------------------------------------
//  DIOCLICLIENT::~DIOCLICLIENT
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 21:53:15
//
//  @return
//  */
//-------------------------------------------------------------------
DIOCLICLIENT::~DIOCLICLIENT()
{
  DeleteAllCommand();

  xfactory->DeleteTimer(timer);

  End();

  Clean();
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::Ini
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
bool DIOCLICLIENT::Ini(int timeout)
{
  if(!diostream)          return false;
  if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!diostream->Open()) return false;

  this->timeout = timeout;

  if(!diostream->WaitToConnected(timeout)) return false;


  // -----------------------------------------
  // Read Header of CLI

  char* line = new char[_MAXSTR];
  if(line)
    {
      memset(line,0,_MAXSTR);

      for(int c=0;c<DIOCLICLIENT_HEADLINEMAX;c++)
        {
          while(!diostream->ReadStr(line))
            {

            }

          headline[c] = line;
        }

      delete [] line;
    }

  // -----------------------------------------

  return true;
}




//-------------------------------------------------------------------
//  DIOCLICLIENT::End
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 15:59:02
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOCLICLIENT::End()
{
  if(diostream)
    {
      diostream->Disconnect();
      diostream->Close();
    }
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::AddCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:34:35
//
//  @return       bool :
//  @param        command :
//  @param        answer :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENT::AddCommand(int type, XCHAR* command)
{
  DIOCLICLIENTCOMMAND* clicommand = GetCommand(type);
  if(clicommand) return false;

  clicommand = new DIOCLICLIENTCOMMAND();
  if(!clicommand) return false;

  clicommand->Set(type,command);

  clicommands.Add(clicommand);

  return true;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::GetCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 11:57:19
//
//  @return       DIOCLICLIENTCOMMAND* :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENTCOMMAND* DIOCLICLIENT::GetCommand(int type)
{
  DIOCLICLIENTCOMMAND* clicommand;

  for(int c=0;c<(int)clicommands.GetSize();c++)
    {
       clicommand = (DIOCLICLIENTCOMMAND*)clicommands.Get(c);
       if(clicommand)
         {
           if(clicommand->GetType() == type) return clicommand;
         }
    }

  return NULL;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::DeleteAllCommand
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
bool DIOCLICLIENT::DeleteAllCommand()
{
  if(clicommands.IsEmpty()) return false;

  clicommands.DeleteContents();
  clicommands.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::GetAnswer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2011 11:56:23
//
//  @return       bool :
//  @param        type :
//  @param        indexanswer :
//  @param        answerstring :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENT::GetAnswer(int type,int indexanswer,XSTRING& answerstring)
{
  DIOCLICLIENTCOMMAND* clicommand = GetCommand(type);
  if(clicommand)
    {
      XSTRING* answer = clicommand->GetAnswer(indexanswer);
      if(answer)
        {
          answerstring = (*answer);
          return true;
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::ReadAnswerLF
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 23:53:40
//
//  @return       bool :
//  @param        answer :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENT::ReadAnswerLF(XSTRING& answer,int timeout)
{
  char str[4];
  int  br;
  bool status = true;

  answer.Empty();

  if(timer) timer->Reset();

  do{ memset(str,0,4);

      br = diostream->Read((XBYTE*)str,1);
      if(br)
        {
          if((str[0]=='\n')||(str[0]=='\r'))
            {
              memset(str,0,4);
              while(!diostream->Read((XBYTE*)str,1))
                {
                  XSLEEP::GetInstance().MilliSeconds(10);
                }

              break;
            }
           else answer += str;
        }

      if(timer)
        {
          if((int)timer->GetMeasureSeconds()>=timeout)
            {
              status = false;
              break;

            } else XSLEEP::GetInstance().MilliSeconds(10);
        }

    } while(1);

  return status;
}




/*-------------------------------------------------------------------
//  DIOCLICLIENT::ReadAnswer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2011 22:38:40
//
//  @return       bool :
//  @param        answer :
//  @param        size :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENT::ReadAnswer(XSTRING& answer,XDWORD size,int timeout)
{
  char str[4];
  int  br;
  bool status = true;

  answer.Empty();

  if(timer) timer->Reset();

  do{ memset(str,0,4);

      br = diostream->Read((XBYTE*)str,1);
      if(br)
        {
          answer += str;
          if((int)answer.GetSize()>=size) break;
        }

      if(timer)
        {
          if((int)timer->GetMeasureSeconds()>=timeout)
            {
              status = false;
              break;

            } else XSLEEP::GetInstance().MilliSeconds(10);
        }

    } while(1);

  return status;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::SendCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/03/2011 22:50:20
//
//  @return       bool :
//  @param        type :
//  @param        nparams :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENT::SendCommand(int type, int nparams,...)
{
  DIOCLICLIENTCOMMAND* clicommand = GetCommand(type);
  if(!clicommand) return false;

  clicommand->DeleteAllParams();

  va_list arg;
  va_start(arg,nparams);

  for(int c=0;c<nparams;c++)
    {
      XCHAR* param = (XCHAR*)va_arg(arg, XCHAR*);
      if(param)
        {
          XSTRING* string = new XSTRING();
          if(string)
            {
              (*string) = param;
              clicommand->AddParam(string);
            }
        }
    }

  va_end(arg);

  actualtypecommand = type;
  SetEvent(DIOCLICLIENT_XFSMEVENT_INICMD);

  DIOCLICLIENT_ERROR error;
  ProcessCommand(error);

  return true;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::IsSendCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/03/2011 0:05:15
//
//  @return       bool :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENT::IsSendCommand(DIOCLICLIENT_ERROR& error)
{
  DIOCLICLIENTCOMMAND* clicommand = GetCommand(actualtypecommand);
  if(!clicommand) return false;

  if(GetCurrentState()==DIOCLICLIENT_XFSMSTATE_NONE) return false;

  return ProcessCommand(error);
}




/*-------------------------------------------------------------------
//  DIOCLICLIENT::SendCommandWithoutAnswer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/10/2011 16:36:26
//
//  @return       DIOCLICLIENT_ERROR :
//  @param        type :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENT::SendCommandWithoutAnswer(int type,XCHAR* param)
{
  DIOCLICLIENT_ERROR error = DIOCLICLIENT_ERROR_NONE;

  if(!SendCommand(type,1,param)) return DIOCLICLIENT_ERROR_INVALIDCOMMAND;

  ProcessWait(error);

  DIOCLICLIENTCOMMAND* clicommand = GetCommand(type);
  if(clicommand) clicommand->DeleteAllAnswers();

  return error;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::SendCommandWithAnswer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/10/2011 16:14:12
//
//  @return       DIOCLICLIENT_ERROR :
//  @param        type :
//  @param        answer :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENT::SendCommandWithAnswer(int type,XSTRING& answer,XCHAR* param)
{
  DIOCLICLIENT_ERROR error = DIOCLICLIENT_ERROR_NONE;

  if(!SendCommand(type,1,param)) return DIOCLICLIENT_ERROR_INVALIDCOMMAND;

  ProcessWait(error);

  DIOCLICLIENTCOMMAND* clicommand = GetCommand(type);
  if(clicommand)
    {
      if(error==DIOCLICLIENT_ERROR_NONE)
        {
          if(clicommand->GetNAnswers())
            {
              XSTRING* string = clicommand->GetAnswer(0);
              if(string) answer = (*string);

            } else error = DIOCLICLIENT_ERROR_NOTANSWER;
        }

      clicommand->DeleteAllAnswers();
    }

  return error;
}




/*-------------------------------------------------------------------
//  DIOCLICLIENT::SendCommandWithAnswers
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/10/2011 16:28:21
//
//  @return       DIOCLICLIENT_ERROR :
//  @param        type :
//  @param        answers :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENT::SendCommandWithAnswers(int type,XVECTOR<XSTRING*>* answers,XCHAR* param)
{
  DIOCLICLIENT_ERROR error = DIOCLICLIENT_ERROR_NONE;

  if(!SendCommand(type,1,param)) return DIOCLICLIENT_ERROR_INVALIDCOMMAND;

  ProcessWait(error);

  DIOCLICLIENTCOMMAND* clicommand = GetCommand(type);
  if(clicommand && answers)
    {
      if(error==DIOCLICLIENT_ERROR_NONE)
        {
          if(clicommand->GetNAnswers())
            {
              for(int c=0;c<clicommand->GetNAnswers();c++)
                {
                  XSTRING* string = clicommand->GetAnswer(c);
                  if(string)
                    {
                      XSTRING* answer = new XSTRING();
                      if(answer) answers->Add(answer);
                    }
                }

            } else error = DIOCLICLIENT_ERROR_NOTANSWER;
        }

      clicommand->DeleteAllAnswers();
    }

  return error;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::ProcessWait
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2011 15:59:53
//
//  @return       void :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
void DIOCLICLIENT::ProcessWait(DIOCLICLIENT_ERROR& error)
{
  while(IsSendCommand(error))
    {
      if(timer) XSLEEP::GetInstance().MilliSeconds(10);
    }
}



/*-------------------------------------------------------------------
//  DIOCLICLIENT::ProcessCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/03/2011 0:04:40
//
//  @return       bool :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOCLICLIENT::ProcessCommand(DIOCLICLIENT_ERROR& error)
{
  static int  paramindex  =  0;
  static int  answerindex =  0;

  if(diostream->GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  DIOCLICLIENTCOMMAND* clicommand = GetCommand(actualtypecommand);
  if(!clicommand) return false;

  if(GetEvent()==DIOCLICLIENT_XFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(GetCurrentState())
        {
          case DIOCLICLIENT_XFSMSTATE_NONE              : break;

          case DIOCLICLIENT_XFSMSTATE_INICMD            : break;

          case DIOCLICLIENT_XFSMSTATE_SENDCMD         : if(!paramindex)
                                                          {
                                                            XSTRING command;

                                                            command = clicommand->GetCommand();

                                                            if(clicommand->GetNParams())
                                                              {
                                                                command += __L(" ");
                                                                command += clicommand->GetParam(paramindex);
                                                              }

                                                            //------------------------------------------
                                                            // Read Prompt

                                                            XBYTE str[2] = { 0 , 0 };

                                                            while(!diostream->Read(str,1))
                                                              {
                                                                if(timer) XSLEEP::GetInstance().MilliSeconds(10);
                                                              }

                                                            if(str[0]!='>')
                                                              {
                                                                error = DIOCLICLIENT_ERROR_WRITECMD;
                                                                SetEvent(DIOCLICLIENT_XFSMEVENT_ENDCMD);
                                                                break;
                                                              }

                                                            //-----------------------------------------

                                                            command += __L("\n");;

                                                            XSTRING_CREATEOEM(command, charOEM)
                                                            if(!diostream->WriteStr(charOEM))
                                                              {
                                                                error = DIOCLICLIENT_ERROR_WRITECMD;
                                                                SetEvent(DIOCLICLIENT_XFSMEVENT_ENDCMD);
                                                              }
                                                            XSTRING_DELETEOEM(charOEM)
                                                           }
                                                          else
                                                           {
                                                             XSTRING param;

                                                             param = clicommand->GetParam(paramindex);

                                                             XSTRING_CREATEOEM(param, charOEM)
                                                             if(!diostream->WriteStr(charOEM))
                                                               {
                                                                 error = DIOCLICLIENT_ERROR_WRITECMD;
                                                                 SetEvent(DIOCLICLIENT_XFSMEVENT_ENDCMD);
                                                               }
                                                             XSTRING_DELETEOEM(charOEM)
                                                           }

                                                          paramindex++;
                                                          SetEvent(DIOCLICLIENT_XFSMEVENT_RECEIVEDCMD);
                                                          break;

          case DIOCLICLIENT_XFSMSTATE_RECEIVEDCMD     : {
                                                          //------------------------------------------
                                                          // Read \n extra

                                                          if(!answerindex)
                                                            {
                                                              XBYTE str[2] = { 0 , 0 };

                                                              while(!diostream->Read(str,1))
                                                                {
                                                                  if(timer) XSLEEP::GetInstance().MilliSeconds(10);
                                                                }

                                                              if(str[0]!='\n')
                                                                {
                                                                  error = DIOCLICLIENT_ERROR_WRITECMD;
                                                                  SetEvent(DIOCLICLIENT_XFSMEVENT_ENDCMD);
                                                                  break;
                                                                }
                                                            }

                                                          //-----------------------------------------

                                                          bool exit = false;

                                                          while(1)
                                                            {
                                                              XSTRING* answer = new XSTRING();
                                                              if(answer)
                                                                {
                                                                  if(!ReadAnswerLF((*answer), timeout))
                                                                    {
                                                                      error = DIOCLICLIENT_ERROR_TIMEOUT;
                                                                      exit  = true;
                                                                    }
                                                                   else
                                                                    {
                                                                      if(!answer->Compare(__L("OK")   ,true)) exit = true;
                                                                      if(!answer->Compare(__L("ERROR"),true))
                                                                        {
                                                                          XSTRING* answererror = clicommand->GetAnswer(0);
                                                                          if(answererror) answererror->UnFormat(__L("Error code %02d: "),&error);

                                                                          exit = true;
                                                                        }

                                                                      if(exit)
                                                                        {
                                                                          delete answer;
                                                                          SetEvent(DIOCLICLIENT_XFSMEVENT_ENDCMD);
                                                                          break;
                                                                        }
                                                                       else
                                                                        {
                                                                          clicommand->AddAnswer(answer);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        break;

          case DIOCLICLIENT_XFSMSTATE_ENDCMD          : break;

        }
    }
   else //  Nuevo evento
    {
      if(GetEvent()<DIOCLICLIENT_LASTEVENT)
        {
          CheckTransition();

          switch(GetCurrentState())
            {
              case DIOCLICLIENT_XFSMSTATE_NONE          : break;

              case DIOCLICLIENT_XFSMSTATE_INICMD        : paramindex  = 0;
                                                          answerindex = 0;
                                                          SetEvent(DIOCLICLIENT_XFSMEVENT_SENDCMD);
                                                          break;

              case DIOCLICLIENT_XFSMSTATE_SENDCMD     : break;

              case DIOCLICLIENT_XFSMSTATE_RECEIVEDCMD : break;

              case DIOCLICLIENT_XFSMSTATE_ENDCMD      : return false;
            }
        }
    }

  return true;
}





//-------------------------------------------------------------------
//  DIOCLICLIENT::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 21:50:13
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOCLICLIENT::Clean()
{
  diostream         = NULL;

  timeout           = DIOCLICLIENT_TIMEOUT;

  timer             = NULL;

  actualtypecommand = 0;
}





