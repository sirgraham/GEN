//------------------------------------------------------------------------------------------
//  DIOCLICLIENT.H
//
/**
// \class
//
//  Data IO Mod CLI class
//
//  @author  Abraham J. Velez
//  @version 13/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOCLICLIENT_H_
#define _DIOCLICLIENT_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOCLICLIENT_ERROR
{
  DIOCLICLIENT_ERROR_NONE                   = 0 ,
  DIOCLICLIENT_ERROR_INTERNAL                   ,
  DIOCLICLIENT_ERROR_INVALIDCOMMAND             ,
  DIOCLICLIENT_ERROR_INVALIDPARAM               ,
  DIOCLICLIENT_ERROR_INSUFFICIENTPARAMS         ,
  DIOCLICLIENT_ERROR_ERROREXECCOMMAND           ,
  DIOCLICLIENT_ERROR_TIMEOUT                    ,
  DIOCLICLIENT_ERROR_WRITECMD                   ,
  DIOCLICLIENT_ERROR_NOTANSWER                  ,
};


enum DIOCLICLIENT_XFSMEVENTS
{
  DIOCLICLIENT_XFSMEVENT_NONE               = 0 ,
  DIOCLICLIENT_XFSMEVENT_INICMD                 ,
  DIOCLICLIENT_XFSMEVENT_SENDCMD                ,
  DIOCLICLIENT_XFSMEVENT_RECEIVEDCMD            ,
  DIOCLICLIENT_XFSMEVENT_ENDCMD                 ,

  DIOCLICLIENT_LASTEVENT
};


enum DIOCLICLIENT_XFSMSTATES
{
  DIOCLICLIENT_XFSMSTATE_NONE               = 0 ,
  DIOCLICLIENT_XFSMSTATE_INICMD                 ,
  DIOCLICLIENT_XFSMSTATE_SENDCMD                ,
  DIOCLICLIENT_XFSMSTATE_RECEIVEDCMD            ,
  DIOCLICLIENT_XFSMSTATE_ENDCMD                 ,

  DIOCLICLIENT_LASTSTATE
};

#define DIOCLICLIENT_HEADLINEMAX    3
#define DIOCLICLIENT_TIMEOUT        5  //Seconds

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class DIOSTREAM;


class DIOCLICLIENTCOMMAND
{
  public:
                                    DIOCLICLIENTCOMMAND           ();
    virtual                        ~DIOCLICLIENTCOMMAND           ();

    int                             GetType                       ();
    XCHAR*                          GetCommand                    ();

    bool                            Set                           (int type, XCHAR* command);

    bool                            AddParam                      (XSTRING* param);
    XCHAR*                          GetParam                      (int index);
    int                             GetNParams                    ();
    bool                            DeleteAllParams               ();

    bool                            AddAnswer                     (XCHAR* answer);
    bool                            AddAnswer                     (XSTRING* answer);
    int                             GetNAnswers                   ();
    XSTRING*                        GetAnswer                     (int index);
    bool                            DeleteAllAnswers              ();


  private:

    void                            Clean                         ();

    int                             type;
    XSTRING                         command;
    XVECTOR<XSTRING*>               params;
    XVECTOR<XSTRING*>               answers;

};



class DIOCLICLIENT  : public XFSMACHINE
{
  public:
                                    DIOCLICLIENT                  (DIOSTREAM* diostream);
    virtual                        ~DIOCLICLIENT                  ();

    bool                            Ini                           (int timeout = DIOCLICLIENT_TIMEOUT);
    void                            End                           ();

  protected:

    bool                            AddCommand                    (int type, XCHAR* command);
    DIOCLICLIENTCOMMAND*            GetCommand                    (int type);
    bool                            DeleteAllCommand              ();

    bool                            GetAnswer                     (int type,int indexanswer,XSTRING& answerstring);
    bool                            ReadAnswerLF                  (XSTRING& answer,int timeout);
    bool                            ReadAnswer                    (XSTRING& answer,XDWORD size,int timeout);

    bool                            SendCommand                   (int type, int nparams = 0, ...);
    bool                            IsSendCommand                 (DIOCLICLIENT_ERROR& error);

    DIOCLICLIENT_ERROR              SendCommandWithoutAnswer      (int type,XCHAR* param);
    DIOCLICLIENT_ERROR              SendCommandWithAnswer         (int type,XSTRING& answer,XCHAR* param = NULL);
    DIOCLICLIENT_ERROR              SendCommandWithAnswers        (int type,XVECTOR<XSTRING*>* answers,XCHAR* param = NULL);

    void                            ProcessWait                   (DIOCLICLIENT_ERROR& error);
    bool                            ProcessCommand                (DIOCLICLIENT_ERROR& error);

  private:

    void                            Clean                         ();

    DIOSTREAM*                      diostream;
    XTIMER*                         timer;

    int                             timeout;

    XVECTOR<DIOCLICLIENTCOMMAND*>   clicommands;

    int                             actualtypecommand;

    XSTRING                         headline[DIOCLICLIENT_HEADLINEMAX];
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

