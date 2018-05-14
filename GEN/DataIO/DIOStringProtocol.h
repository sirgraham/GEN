//------------------------------------------------------------------------------------------
//  DIOSTRINGPROTOCOL.H
//
/**
// \class
//
//  Data IO CLI SERVER class
//
//  @author  Abraham J. Velez
//  @version 13/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTRINGPROTOCOL_H_
#define _DIOSTRINGPROTOCOL_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"
#include "XTimer.h"
#include "XFSMachine.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOSTRINGPROTOCOL_ERROR
{
  DIOSTRINGPROTOCOL_ERROR_NONE                    = 0 ,
  DIOSTRINGPROTOCOL_ERROR_INTERNAL                    ,
  DIOSTRINGPROTOCOL_ERROR_INVALIDCOMMAND              ,
  DIOSTRINGPROTOCOL_ERROR_INVALIDPARAM                ,
  DIOSTRINGPROTOCOL_ERROR_INSUFFICIENTPARAMS          ,
  DIOSTRINGPROTOCOL_ERROR_ERROREXECCOMMAND            ,
  DIOSTRINGPROTOCOL_ERROR_TIMEOUT                     ,
  DIOSTRINGPROTOCOL_ERROR_WRITECMD                    ,
  DIOSTRINGPROTOCOL_ERROR_NOTANSWER                   ,
};


#define DIOSTRINGPROTOCOL_OK            __L("ok")
#define DIOSTRINGPROTOCOL_ERROR         __L("error")
#define DIOSTRINGPROTOCOL_ANSWER        __L("answer ")

#define DIOSTRINGPROTOCOL_TIMEOUT       5  //Seconds

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class DIOSTREAM;


class DIOSTRINGPROTOCOLCOMMAND
{
  public:
                                          DIOSTRINGPROTOCOLCOMMAND      ()
                                          {
                                            Clean();
                                          }

    virtual                              ~DIOSTRINGPROTOCOLCOMMAND      ()
                                          {
                                            Clean();
                                          }

    XCHAR*                                GetCommand                    ()                                  { return command.Get();                               }

    int                                   GetNParams                    ()                                  { return nparams;                                     }

    bool                                  Set                           (XCHAR* command, int nparams)
                                          {

                                            this->command = command;
                                            this->nparams = nparams;

                                            return true;
                                          }

  private:

    void                                  Clean                         ()
                                          {
                                            command.Empty();
                                            nparams = 0;
                                          }

    XSTRING                               command;
    int                                   nparams;
};



class DIOSTRINGPROTOCOLANSWER
{
  public:
                                          DIOSTRINGPROTOCOLANSWER       ()
                                          {
                                            Clean();
                                          }

    virtual                              ~DIOSTRINGPROTOCOLANSWER       ()
                                          {
                                            Clean();
                                          }


    XSTRING*                              GetCommand                    ()                                  { return &command;                                }
    XSTRING*                              GetAnswer                     ()                                  { return &answer;                                 }

     void                                 Clean                         ()
                                          {
                                            command.Empty();
                                            answer.Empty();
                                          }

    XSTRING                               command;
    XSTRING                               answer;
};



class DIOSTRINGPROTOCOL
{
  public:
                                          DIOSTRINGPROTOCOL             (DIOSTREAM* diostream);
    virtual                              ~DIOSTRINGPROTOCOL             ();

    virtual bool                          Ini                           (int timeout = DIOSTRINGPROTOCOL_TIMEOUT);
    virtual void                          End                           ();

    virtual bool                          SendCommand                   (XCHAR* command, XSTRING* answer, int timeoutanswer, ...);
    bool                                  SendCommand                   (XCHAR* command, XSTRING* answer, int timeoutanswer, va_list& arg);

    virtual bool                          ReceivedCommand               (XSTRING& command, XVECTOR<XSTRING*>& params, XSTRING& answer)
                                          {
                                            return false;
                                          }

    virtual void                          ReceivedAnswer                (XSTRING& answer)
                                          {

                                          }

    void                                  ReceivedCommandManager        ();


    bool                                  AddAnswer                     (XCHAR* command, XSTRING& answer);
    XSTRING*                              GetFirstAnswer                (XCHAR* command);
    bool                                  DeleteFirstAnswer             (XCHAR* command);
    bool                                  DeleteAllAnswers              ();


  protected:

    bool                                  AddCommand                    (XCHAR* command, int nparams);
    DIOSTRINGPROTOCOLCOMMAND*             GetCommand                    (XCHAR* command);
    bool                                  DeleteAllCommand              ();

    bool                                  ExtractParamsFromCommand      (XSTRING& stringreceived, XSTRING& command, XVECTOR<XSTRING*>& params);

  private:

    void                                  Clean                         ()
                                          {
                                            diostream             = NULL;

                                            xtimerout             = NULL;
                                            timeout               = 0;

                                            xmutexanswers         = NULL;
                                          }

    DIOSTREAM*                            diostream;

    XTIMER*                               xtimerout;
    int                                   timeout;

    XVECTOR<DIOSTRINGPROTOCOLCOMMAND*>    commands;

    XMUTEX*                               xmutexanswers;
    XVECTOR<DIOSTRINGPROTOCOLANSWER*>     answers;

    XSTRING                               laststringreceived;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

