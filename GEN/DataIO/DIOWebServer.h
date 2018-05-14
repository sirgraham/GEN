//------------------------------------------------------------------------------------------
//  DIOWEBSERVER.H
//
/**
// \class
//
//  DIO WEB Server class
//
//  @author  Abraham J. Velez
//  @version 20/01/2007 21:05:48
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWEBSERVER_H_
#define _DIOWEBSERVER_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"
#include "XBuffer.h"
#include "XString.h"
#include "XEvent.h"
#include "XObserver.h"
#include "XSubject.h"
#include "XPath.h"
#include "XThreadCollected.h"

#include "DIOStream.h"

#include "DIOWebHeader.h"
#include "DIOWebClient.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


#define DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT      10    // Seconds
#define DIOWEBSERVER_DEFAULTTIMEOUTSERVERPAGE     30    // Seconds

#define DIOWEBSERVER_MAXPAGECONNEXIONS            30

#define DIOWEBSERVER_IDSERVER                     __L("GEN Web Server v1.5")
#define DIOWEBSERVER_LOGSECTIONID                 __L("WEB Server")

#define DIOWEBSERVER_TAGLABEL                     __L("GEN")


#define DIOWEBSERVER_DEFAULTPORT                  80
#define DIOWEBSERVER_MAXSENDFILE                  DIOSTREAM_MAXBUFFER/2
#define DIOWEBSERVER_MAXBUFFER                    (16*1024)


enum DIOWEBSERVER_PAGESTATUS
{
  DIOWEBSERVER_PAGESTATUS_NONE                        = 0 ,
  DIOWEBSERVER_PAGESTATUS_WAIT                            ,
  DIOWEBSERVER_PAGESTATUS_SENDING                         ,
  DIOWEBSERVER_PAGESTATUS_SENT
};


enum DIOWEBSERVER_TYPE
{
  DIOWEBSERVER_TYPE_SYMPLE                            = 0 ,
  DIOWEBSERVER_TYPE_FULL
};

enum DIOWEBSERVER_METHOD
{
  DIOWEBSERVER_METHOD_UNKNOWN                         = 0 ,
  DIOWEBSERVER_METHOD_GET                                 ,
  DIOWEBSERVER_METHOD_HEAD                                ,
  DIOWEBSERVER_METHOD_POST                                ,
  DIOWEBSERVER_METHOD_OPTIONS
};


enum DIOWEBSERVER_ALIGN
{
  DIOWEBSERVER_ALIGN_NONE                             = 0 ,
  DIOWEBSERVER_ALIGN_LEFT                                 ,
  DIOWEBSERVER_ALIGN_RIGHT                                ,
  DIOWEBSERVER_ALIGN_CENTER                               ,
};


enum DIOWEBSERVER_TAGSTATUS
{
  DIOWEBSERVER_TAGSTATUS_NOTFOUND                     = 0 ,
  DIOWEBSERVER_TAGSTATUS_TAGFOUNDNOTEND                   ,
  DIOWEBSERVER_TAGSTATUS_TAGFOUNDEND
};

enum DIOWEBSERVER_ENCODINGFLAGS
{
  DIOWEBSERVER_ENCODINGFLAGS_NONE               = 0     ,
  DIOWEBSERVER_ENCODINGFLAGS_GZIP               = 1<<1  ,
  DIOWEBSERVER_ENCODINGFLAGS_DEFLATE            = 1<<2  ,
  DIOWEBSERVER_ENCODINGFLAGS_COMPRESS           = 1<<3  ,
  DIOWEBSERVER_ENCODINGFLAGS_SHAREDDICTIONARY   = 1<<4
};



//---- CLASS -------------------------------------------------------------------------------

class XPATHS;
class XFACTORY;
class XBUFFER;
class XTIMER;
class XPUBLISHER;
class XLOG;
class DIOFACTORY;
class DIOSTREAMTCPIPCONFIG ;
class DIOSTREAMTCPIP;

class DIOWEBSERVER_REQUEST;
class DIOWEBSERVER_CONNEXION;
class DIOWEBSERVER;




class DIOWEBSERVER_HEADER : public DIOWEBHEADER
{
  public:
                                DIOWEBSERVER_HEADER               ();
    virtual                    ~DIOWEBSERVER_HEADER               ();

    int                         GetVersion                        ();
    int                         GetSubVersion                     ();
    int                         GetResult                         ();
    bool                        GetResultString                   (XSTRING& coderesultstring);

    bool                        Create                            (DIOWEBSERVER* webserver, DIOWEBSERVER_CONNEXION* connexion, DIOWEBSERVER_REQUEST* request, XSTRING* resource, int version, int subversion, DIOWEBHEADER_RESULT result, int size, XSTRING* entitytag = NULL, bool isplayed = true, bool password = false);

  private:

    void                        Clean                             ();

    bool                        GetDescriptionResult              (DIOWEBHEADER_RESULT result, XSTRING& description);
};




class DIOWEBSERVER_AUTHENTICATION
{
  public:
                                                DIOWEBSERVER_AUTHENTICATION             ()                                      { Clean();                                        }
    virtual                                    ~DIOWEBSERVER_AUTHENTICATION             ()                                      { Clean();                                        }

    XSTRING*                                    GetGuest                                ()                                      { return &guest;                                  }
    XSTRING*                                    GetLogin                                ()                                      { return &login;                                  }
    XSTRING*                                    GetPassword                             ()                                      { return &password;                               }

    bool                                        Set                                     (XSTRING& login, XSTRING& password)
                                                {
                                                  this->login    = login;
                                                  this->password = password;

                                                  return true;
                                                }

    bool                                        Set                                     (XSTRING& guest, XSTRING& login, XSTRING& password)
                                                {
                                                  this->guest    = guest;
                                                  this->login    = login;
                                                  this->password = password;

                                                  return true;
                                                }


  private:

    void                                        Clean                                   ()                                      {                                                 }

    XSTRING                                     guest;
    XSTRING                                     login;
    XSTRING                                     password;

};



class DIOWEBSERVER_REQUEST
{
  public:
                                                DIOWEBSERVER_REQUEST                    ()
                                                {
                                                  Clean();
                                                  data = new XBUFFER();
                                                }

    virtual                                    ~DIOWEBSERVER_REQUEST                    ()
                                                {
                                                  delete data;
                                                  Clean();
                                                }


    DIOWEBSERVER_TYPE                           GetType                                 ()                                      { return type;                                    }
    DIOWEBSERVER_METHOD                         GetMethod                               ()                                      { return method;                                  }
    XSTRING*                                    GetResource                             ()                                      { return &resource;                               }
    XSTRING*                                    GetGuest                                ()                                      { return &guest;                                  }
    XSTRING*                                    GetUserAgent                            ()                                      { return &useragent;                              }
    int                                         GetSize                                 ()                                      { return size;                                    }
    XSTRING*                                    GetIfNoneMatch                          ()                                      { return &ifnonematch;                            }

    XBUFFER*                                    GetData                                 ()                                      { return data;                                    }

    bool                                        HaveLoginPassword                       ()                                      { return (loginpassword.IsEmpty()?false:true);    }

    void                                        SetType                                 (DIOWEBSERVER_TYPE type)                { this->type = type;                              }
    void                                        SetMethod                               (DIOWEBSERVER_METHOD method)            { this->method = method;                          }
    void                                        SetResource                             (XSTRING& resource)                     { this->resource = resource;                      }
    void                                        SetGuest                                (XSTRING& guest)                        { this->guest = guest;                            }

    void                                        SetUserAgent                            (XSTRING& useragent)                    { this->useragent = useragent;                    }
    void                                        SetSize                                 (int size)                              { this->size = size;                              }
    void                                        SetIfNoneMatch                          (XSTRING& ifnonematch);

    void                                        SetLoginPassword                        (XSTRING& loginpassword)                { this->loginpassword = loginpassword;            }

    bool                                        ConvertFromWeb                          (XSTRING& string);

    bool                                        GetLoginPassword                        (XSTRING& login, XSTRING& password);

    void                                        ProcessAcceptedEncodingFlags            (XSTRING& string);
    void                                        ProcessAcceptedLanguage                 (XSTRING& string)                       { this->acceptedlanguage            = string;     }
    void                                        ProcessAcceptedCharset                  (XSTRING& string)                       { this->acceptedcharset             = string;     }
    void                                        ProcessAccessControlRequestMethod       (XSTRING& string)                       { this->acceptedcontrolrequest      = string;     }
    void                                        ProcessAcceptedAllowHeaders             (XSTRING& string)                       { this->acceptedallowheaders        = string;     }
    void                                        ProcessAccessControlAllowOrigin         (XSTRING& string)                       { this->accesscontrolalloworigin    = string;     }
    void                                        ProcessAccessControlAllowMethods        (XSTRING& string)                       { this->accesscontrolallowmethods   = string;     }
    void                                        ProcessAccessControlAllowHeaders        (XSTRING& string)                       { this->accesscontrolallowheaders   = string;     }
    void                                        ProcessOrigin                           (XSTRING& string)                       { this->origin                      = string;     }


    bool                                        IsAcceptedEncodingFlag                  (DIOWEBSERVER_ENCODINGFLAGS flag)       { return ((acceptedencodingflags&flag) != 0);     }

    bool                                        Reset                                   ();

    XSTRING                                     acceptedcharset;
    XSTRING                                     acceptedcontrolrequest;
    XSTRING                                     acceptedlanguage;
    XSTRING                                     acceptedallowheaders;
    XSTRING                                     accesscontrolalloworigin;
    XSTRING                                     accesscontrolallowmethods;
    XSTRING                                     accesscontrolallowheaders;
    XSTRING                                     origin;

  private:

    void                                        Clean                                   ()
                                                {
                                                  type                  = DIOWEBSERVER_TYPE_SYMPLE;
                                                  method                = DIOWEBSERVER_METHOD_UNKNOWN;
                                                  size                  = -1;
                                                  data                  = NULL;
                                                  acceptedencodingflags = DIOWEBSERVER_ENCODINGFLAGS_NONE;
                                                }

    DIOWEBSERVER_TYPE                           type;
    DIOWEBSERVER_METHOD                         method;
    XSTRING                                     resource;
    XSTRING                                     guest;
    XSTRING                                     useragent;
    XSTRING                                     loginpassword;
    XSTRING                                     ifnonematch;
    int                                         size;
    XBUFFER*                                    data;
    XDWORD                                      acceptedencodingflags;



};




class DIOWEBSERVER_CONNEXION
{
  public:
                                                DIOWEBSERVER_CONNEXION                  ();
    virtual                                    ~DIOWEBSERVER_CONNEXION                  ();

    bool                                        Ini                                     (DIOWEBSERVER* webserver, DIOSTREAMTCPIPCONFIG* diostreamcfg);

    DIOSTREAMTCPIP*                             GetDIOStream                            ()                                      { return diostream;                               }

    bool                                        Activate                                ();

    bool                                        IsActive                                ()                                      { return isactive;                                }

    bool                                        Receiver                                (XBYTE* buffer, int& size,int timeout = DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT);

    bool                                        Send                                    (XBYTE* buffer, int& size, int timeout = DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT);
    bool                                        Send                                    (XBUFFER& xbuffer, int timeout = DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT);
    bool                                        Send                                    (XSTRING& string, int timeout = DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT);
    bool                                        Send                                    (XCHAR* string, int timeout = DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT);
    bool                                        SendResult                              (XSTRING& string, int timeout = DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT);

    bool                                        SendFile                                (XPATH& xpath, int timeout = DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT);
    bool                                        ParseFile                               (XPATH& xpath, XSTRING& resultofparsing,  int timeout);
    bool                                        ProcessFile                             (XPATH& xpath, XSTRING& resultofparsing,  int timeout);

    DIOWEBSERVER_PAGESTATUS                     GetPageStatus                           ()                                      { return pagestatus;                              }
    XTIMER*                                     GetXTimerSendingPage                    ()                                      { return xtimersendingpage;                       }

    DIOWEBCLIENT_HEADER*                          GetHeader                               ()                                      { return &header;                                 }

    bool                                        End                                     ();

    DIOWEBSERVER*                               GetServer                               ()                                      { return this->webserver;                         }

    DIOWEBSERVER_ENCODINGFLAGS                  GetActualEncodingFlag                   ()                                      { return actualencodingflag;                      }
    bool                                        SetActualEncodingFlag                   (DIOWEBSERVER_ENCODINGFLAGS actualencodingflag);
    XCHAR*                                      GetAccessControlAllowHeaders            ();
    XCHAR*                                      GetAccessControlAllowMethods            ();
    XCHAR*                                      GetAcceptLenguage                       ();
    XCHAR*                                      GetOrigin                               ();

  protected:

    DIOWEBSERVER*                               webserver;
    DIOSTREAMTCPIPCONFIG *                      diostreamcfg;

    DIOSTREAMTCPIP*                             diostream;
    XTIMER*                                     xtimer;
    bool                                        isactive;
    DIOWEBCLIENT_HEADER                         header;
    DIOWEBSERVER_REQUEST*                       request;
    DIOWEBSERVER_PAGESTATUS                     pagestatus;
    XTIMER*                                     xtimersendingpage;
    XSTRING                                     lastTAG;
    XTHREADCOLLECTED*                           threadconnexion;
    DIOWEBSERVER_ENCODINGFLAGS                  actualencodingflag;

    XSTRING                                     origin;


  private:

    void                                        Clean                                   ()
                                                {
                                                  webserver           = NULL;
                                                  diostream           = NULL;
                                                  xtimer              = NULL;
                                                  isactive            = false;
                                                  request             = NULL;
                                                  pagestatus          = DIOWEBSERVER_PAGESTATUS_NONE;
                                                  xtimersendingpage   = NULL;
                                                  actualencodingflag  = DIOWEBSERVER_ENCODINGFLAGS_NONE;
                                                }

    bool                                        GetRequest                              (DIOWEBSERVER_REQUEST& request, int timeout = DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT);

    bool                                        DoLogPage                               (bool ini, DIOWEBSERVER_REQUEST& request);

    bool                                        ReceiverLine                            (XSTRING& string,int maxlen);
    bool                                        ExistFile                               (XPATH& xpath);

    bool                                        SendingPage                             ();

    DIOWEBSERVER_TAGSTATUS                      SearchTAG                               (XSTRING* originalline, bool& intag, XSTRING& inioutputline,XSTRING& endoutputline);

    static void                                 ThreadRunFunction                       (void* param);
};



class DIOWEBSERVER : public XSUBJECT, public XOBSERVER
{
  public:
                                                DIOWEBSERVER                            ();
    virtual                                    ~DIOWEBSERVER                            ();

    bool                                        Ini                                     (int port = DIOWEBSERVER_DEFAULTPORT, int timeoutserverpage = DIOWEBSERVER_DEFAULTTIMEOUTSERVERPAGE, XSTRING* addrlocal = NULL);

    int                                         GetPort                                 ()                                                  { return port;                                                      }
    int                                         GetTimeoutServerPage                    ()                                                  { return timeoutserverpage;                                         }

    DIOSTREAMTCPIPCONFIG *                      GetDIOStreamCFG                         ()                                                  { return diostreamcfg;                                              }

    bool                                        IsSendingAnyPage                        ();
    bool                                        UnSubscribeAllPages                     ();

    bool                                        AddAuthentication                       (XSTRING& guest, XSTRING& login, XSTRING& password);
    DIOWEBSERVER_AUTHENTICATION*                GetAuthentication                       (XSTRING& guest);
    bool                                        DelAuthentication                       (XSTRING& guest);
    bool                                        DelAllAuthentication                    ();

    bool                                        IsPlayablePage                          (XCHAR* extension);
    bool                                        IsPlayablePage                          (XSTRING& extension);

    bool                                        AddPlayablePageExtension                (XCHAR* extension);
    bool                                        AddPlayablePageExtension                (XSTRING& extension);

    bool                                        DeleteAllPlayablePageExtensions         ();

    void                                        AddIncludedEncodingFlag                 (DIOWEBSERVER_ENCODINGFLAGS flag)                   { includedencodingflags |= flag;                                    }
    bool                                        IsIncludedEncodingFlag                  (DIOWEBSERVER_ENCODINGFLAGS flag)                   { return ((includedencodingflags&flag) != 0);                       }

    bool                                        End                                     ();


  protected:

    DIOSTREAMTCPIPCONFIG *                      diostreamcfg;
    int                                         port;
    int                                         timeoutserverpage;
    XSTRING                                     addrlocal;
    XVECTOR<DIOWEBSERVER_CONNEXION*>            connexions;
    bool                                        isactive;
    bool                                        doexit;
    XVECTOR<DIOWEBSERVER_AUTHENTICATION*>       authentications;
    XVECTOR<XSTRING*>                           playablepageextensions;
    XTIMER*                                     xtimercontrol;
    XTHREADCOLLECTED*                           threadserver;
    XDWORD                                      includedencodingflags;

    XMUTEX*                                     xmutexconnexions;

  private:

    void                                        Clean                                   ()
                                                {
                                                  diostreamcfg          = NULL;
                                                  port                  = DIOWEBSERVER_DEFAULTPORT;

                                                  timeoutserverpage     = 0;
                                                  isactive              = false;
                                                  doexit                = false;
                                                  xtimercontrol         = NULL;
                                                  threadserver          = NULL;

                                                  includedencodingflags = DIOWEBSERVER_ENCODINGFLAGS_NONE;

                                                  xmutexconnexions      = NULL;
                                                }

    bool                                        WaitToSentAllPages                      ();

    XDWORD                                      Connexions_GetNWaiting                  ();

    int                                         Connexions_GetNConnexionsSendingPage    ();
    bool                                        Connexions_CreateNew                    ();
    bool                                        Connexions_DeleteUsed                   ();
    bool                                        Connexions_DeleteWaiting                ();
    bool                                        Connexions_DeleteAll                    ();

    static void                                 ThreadRunFunction                       (void* param);
    void                                        HandleEvent                             (XEVENT* xevent);
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

