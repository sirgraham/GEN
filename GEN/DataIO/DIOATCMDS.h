//------------------------------------------------------------------------------------------
//  DIOATCMDS.H
//
/**
// \class
//
//  ATCommand Standard Class
//
//  @author  Abraham J. Velez
//  @version 13/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOATCMDS_H_
#define _DIOATCMDS_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum
{
  DIOATCMD_TYPE_UNKNOWN                 = 0   ,
  DIOATCMD_TYPE_STANDARD                      ,
  DIOATCMD_TYPE_BASIC                         ,
  DIOATCMD_TYPE_CHARACTER                     ,
  DIOATCMD_TYPE_RESET                         ,
  DIOATCMD_TYPE_ECHO                          ,
  DIOATCMD_TYPE_VERBOSE                       ,
  DIOATCMD_TYPE_BAUDRATE                      ,

  DIOATCMD_TYPE_GSM                     = 100 ,
  DIOATCMD_TYPE_PEI                     = 200 ,

  DIOATCMD_TYPE_LAST
};


enum DIOATCMD_CODECHARACTER
{
  DIOATCMD_CODECHARACTER_ESCAPE         = 2   ,
  DIOATCMD_CODECHARACTER_ENDLINE              ,
  DIOATCMD_CODECHARACTER_ANSWER               ,
  DIOATCMD_CODECHARACTER_DEL                  ,

};


enum DIOATCMD_ERROR
{
  DIOATCMD_ERROR_NONE                   = 0   ,
  DIOATCMD_ERROR_UNKNOWN                        ,
  DIOATCMD_ERROR_UNKNOWNCMD                   ,
  DIOATCMD_ERROR_TIMEOUT                      ,
  DIOATCMD_ERROR_INVALIDCMD                   ,
  DIOATCMD_ERROR_WRITECMD                     ,
};


enum DIOATCMD_TYPEADDITIONALSIZE
{
  DIOATCMD_TYPEADDITIONALSIZE_NONE      = 0   ,
  DIOATCMD_TYPEADDITIONALSIZE_BYTES           ,
  DIOATCMD_TYPEADDITIONALSIZE_LINES           ,
};


#define DIOATCMDS_TIMEOUT             15   //Seconds
#define DIOATCMDS_ANSWERNOTFOUND     -1
#define DIOATCMDS_COMMANDNOTFOUND    -1

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTHREADCOLLECTED;
class DIOSTREAM;


class DIOATCMD
{
  public:
                              DIOATCMD                          ();
    virtual                  ~DIOATCMD                          ();

    XDWORD                    GetType                           ();
    XCHAR*                    GetCommand                        ();
    XSTRING*                  GetCommandString                  ();
    bool                      IsUnSolicited                     ();

    bool                      Set                               (XDWORD type, XCHAR* command,bool isunsolicited = false);

  private:

    void                      Clean                             ();

    XDWORD                    type;
    XSTRING                   command;
    bool                      isunsolicited;
};




class DIOATCMDS  : public XFSMACHINE
{
  public:
                              DIOATCMDS                         (DIOSTREAM* diostream);
    virtual                  ~DIOATCMDS                         ();

    DIOSTREAM*                GetDIOStream                      ();
    XTIMER*                   GetXTimer                         ();

    bool                      Ini                               (int timeout = DIOATCMDS_TIMEOUT, bool echomode = true, bool verbosemode = true, int minimumbaurate = 115200);

    DIOATCMD_ERROR            Basic                             (int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR            GetCharacter                      (DIOATCMD_CODECHARACTER code,XBYTE& character,int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR            Reset                             (int timeout = DIOATCMDS_TIMEOUT);

    DIOATCMD_ERROR            SetEchoMode                       (bool echomode, int timeout = DIOATCMDS_TIMEOUT);

    DIOATCMD_ERROR            GetVerboseMode                    (bool& verbosemode , int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR            SetVerboseMode                    (bool verbosemode  , int timeout = DIOATCMDS_TIMEOUT);

    DIOATCMD_ERROR            GetBaudRate                       (int& baudrate, int timeout  = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR            SetBaudRate                       (int  baudrate, int timeout  = DIOATCMDS_TIMEOUT);

    void                      End                               ();

    virtual bool              AdditionalInitialization          (int timeout = DIOATCMDS_TIMEOUT);

    virtual bool              UnsolicitedCommandAddtionalSize   (XDWORD type, XSTRING& param, DIOATCMD_TYPEADDITIONALSIZE& typesize, int& size);
    virtual bool              UnsolicitedCommand                (XDWORD type, XSTRING& param, XBUFFER& additionalparam);

    int                       CalculeBaudRateStreamUART         (int timeout = DIOATCMDS_TIMEOUT);

  protected:

    bool                      AddCommand                        (XDWORD type, XCHAR* command,bool isunsolicited = false);
    DIOATCMD*                 GetCommand                        (XDWORD type);
    XCHAR*                    GetCommandString                  (XDWORD type);
    bool                      DeleteAllCommand                  ();

    DIOATCMD_ERROR            SendCommand                       (XDWORD type, XCHAR* param = NULL);
    DIOATCMD_ERROR            SendCommand                       (XDWORD type, XSTRING& param);

    DIOATCMD_ERROR            SendParam                         (XCHAR* param);
    DIOATCMD_ERROR            SendParam                         (XSTRING& param);
    DIOATCMD_ERROR            SendParam                         (XBUFFER& xbuffer);

    bool                      AddSendCommand                    (XSTRING* sendcommand);
    bool                      AddSendCommand                    (XBUFFER& xbuffer);
    XSTRING*                  GetSendCommand                    (int index);
    int                       SearchSendCommand                 (XSTRING* sendcommand);
    bool                      DeleteSendCommand                 (int index);
    bool                      DeleteAllSendCommand              ();

    bool                      ReadAnswer                        (XSTRING& answer, int timeout = DIOATCMDS_TIMEOUT);
    bool                      ReadAnswerWithOutEndChar          (XCHAR* answer, int timeout = DIOATCMDS_TIMEOUT);
    bool                      ReadAnswerWithOutEndChar          (XSTRING& answer, int timeout = DIOATCMDS_TIMEOUT);

    XSTRING*                  GetAnswer                         (int index);
    XSTRING*                  GetLastAnswer                     ();
    bool                      DeleteAnswer                      (int index);
    bool                      DeleteLastAnswer                  ();
    bool                      DeleteAllAnswers                  ();
    XVECTOR<XSTRING*>*        GetAnswers                        ();

    bool                      IsAnswerEmpty                     (XSTRING& answer);
    bool                      DeleteAnswerControlChar           (XSTRING* origin, XSTRING& answer);

    int                       SearchInAnswers                   (XCHAR* string, int timeout = 0);
    int                       SearchInAnswers                   (XDWORD typecommand, int timeout = 0);

    bool                      WaitToWrite                       ();

    DIOATCMD_ERROR            WaitToProccessAnswer              (int timeout = DIOATCMDS_TIMEOUT, int* index = NULL);
    DIOATCMD_ERROR            WaitToOneAnswerMore               (int timeout = DIOATCMDS_TIMEOUT);

    DIOATCMD_ERROR            MakeSimpleCommand                 (XDWORD type,XCHAR* param = NULL,int timeout = DIOATCMDS_TIMEOUT);
    DIOATCMD_ERROR            MakeCommandWithSimpleAnswer       (XDWORD type,XSTRING& answer,XCHAR* param = NULL,int timeout = DIOATCMDS_TIMEOUT);

    DIOSTREAM*                diostream;
    XTIMER*                   xtimer;

    bool                      echomode;
    bool                      verbosemode;

    XBYTE                     escapechar;
    XBYTE                     endlinechar;
    XBYTE                     answerchar;
    XBYTE                     delchar;

  private:

    void                      Clean                             ();

    static void               ThreadConnexion                   (void* data);

    bool                      CheckUnsolicitedAnswer            (XSTRING& answer);

    XMUTEX*                   mutexreadwriteproccess;
    XTHREADCOLLECTED*         threadconnexion;

    XVECTOR<DIOATCMD*>        ATcmds;
    XVECTOR<XSTRING*>         sendcommands;
    XVECTOR<XSTRING*>         answers;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

