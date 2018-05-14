//------------------------------------------------------------------------------------------
//  DIOWEBCLIENT.H
//
/**
// \class
//
//  DIO Web Client class
//
//  @author  Abraham J. Velez
//  @version 20/01/2007 21:05:48
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWEBCLIENT_H_
#define _DIOWEBCLIENT_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XPath.h"
#include "XSubject.h"

#include "DIOURL.h"
#include "DIOWebHeader.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOWEBCLIENT_TIMEOUT            8
#define DIOWEBCLIENT_MAXBUFFER          (256*1024)
#define DIOWEBCLIENT_DEFAULTPORT        80

#define DIOWEBCLIENT_DEFAULTUSERAGENT   __L("User-Agent: Mozilla 5.0\r\n")

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XFILE;
class DIOFACTORY;
class DIOSTREAMTCPIPCONFIG;
class DIOSTREAMTCPIP;


class DIOWEBCLIENT_HEADER : public DIOWEBHEADER
{
  public:
                                DIOWEBCLIENT_HEADER               ();
    virtual                    ~DIOWEBCLIENT_HEADER               ();

    bool                        GetMethod                         (XSTRING& method);
    bool                        GetResource                       (XSTRING& resource);
    int                         GetVersion                        ();
    int                         GetSubVersion                     ();
    int                         GetResultServer                   ();

  private:

    void                        Clean                             ();
};



class DIOWEBCLIENT : public XSUBJECT
{
  public:
                                  DIOWEBCLIENT                      ();
    virtual                      ~DIOWEBCLIENT                      ();

    int                           GetPort                           ()                        { return port;                                      }
    void                          SetPort                           (int port)                { this->port = port;                                }

    DIOURL*                       GetProxyURL                       ()                        { return &proxyurl;                                 }
    int                           GetProxyPort                      ()                        { return proxyport;                                 }
    void                          SetProxyPort                      (int port)                { proxyport = port;                                 }

    DIOSTREAMTCPIPCONFIG*         GetStreamCFG                      ()                        { return diostreamcfg;                              }

    DIOWEBCLIENT_HEADER*            GetHeader                         ()                        { return &header;                                   }

    bool                          Get                               (DIOURL& url, XBUFFER& webpage    , XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Get                               (XCHAR*  url, XBUFFER& webpage    , XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Get                               (DIOURL& url, XPATH& pathwebpage  , XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Get                               (XCHAR*  url, XPATH& pathwebpage  , XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);

    bool                          Post                              (DIOURL& url, XBUFFER& webpage    , XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Post                              (XCHAR*  url, XBUFFER& webpage    , XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Post                              (DIOURL& url, XPATH& pathwebpage  , XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Post                              (XCHAR*  url, XPATH& pathwebpage  , XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);

    bool                          Post                              (DIOURL& url, XBUFFER& webpage    , XBUFFER* boundary = NULL, XFILE* xfile = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Post                              (XCHAR*  url, XBUFFER& webpage    , XBUFFER* boundary = NULL, XFILE* xfile = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Post                              (DIOURL& url, XPATH& pathwebpage  , XBUFFER* boundary = NULL, XFILE* xfile = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          Post                              (XCHAR*  url, XPATH& pathwebpage  , XBUFFER* boundary = NULL, XFILE* xfile = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);

  private:

    void                          Clean                             ()
                                  {
                                    timerout          = NULL;

                                    diostreamcfg      = NULL;
                                    diostream         = NULL;

                                    proxyport         = 0;

                                    port              = DIOWEBCLIENT_DEFAULTPORT;
                                  }


    bool                          MakeOperationBuffer               (DIOURL& url, XBUFFER& webpage, XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          MakeOperationFile                 (DIOURL& url, XFILE& webpage, XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                          MakeOperationFile                 (DIOURL& url, XBUFFER& webpage, XBUFFER* boundary = NULL, XFILE* file = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);

    XTIMER*                       timerout;

    DIOSTREAMTCPIPCONFIG*         diostreamcfg;
    DIOSTREAMTCPIP*               diostream;

    int                           port;

    DIOURL                        proxyurl;
    int                           proxyport;

    DIOWEBCLIENT_HEADER           header;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

