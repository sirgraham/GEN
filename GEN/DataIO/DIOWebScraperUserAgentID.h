/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERUSERAGENTID.H
*/
/**
// \class
//
//  User Agent ID (Web Service)
//
//  @author  Abraham J. Velez
//  @version 05/11/2007 16:20:28
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWEBSCRAPERUSERAGENTID_H_
#define _DIOWEBSCRAPERUSERAGENTID_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOURL.h"
#include "DIOWebScraper.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOWEBSCRAPERUSERAGENTID_NAMEFILE       __L("ws_useragentid.xml")
#define DIOWEBSCRAPERUSERAGENTID_NAMESERVICE    __L("USERAGENTID")

/*---- CLASS -----------------------------------------------------------------------------*/


class DIOUSERAGENTIDRESULT :  public DIOWEBSCRAPERCACHERESULT
{
  public:
                              DIOUSERAGENTIDRESULT            ();
    virtual                  ~DIOUSERAGENTIDRESULT            ();

    XSTRING*                  GetBrowser                      ();
    XSTRING*                  GetSO                           ();

  private:

    void                      Clean                           ();

    XSTRING                   browser;
    XSTRING                   systemoperative;
};



class DIOWEBSCRAPERUSERAGENTID : public DIOWEBSCRAPER
{
  public:
                            DIOWEBSCRAPERUSERAGENTID          (  );
    virtual                ~DIOWEBSCRAPERUSERAGENTID          ();

    bool                    ChangeURL                         (XCHAR* maskurl, DIOURL& url);

    bool                    Get                               (XCHAR* useragent  , XSTRING& browser, XSTRING& systemoperative, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XSTRING& useragent, XSTRING& browser, XSTRING& systemoperative, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                    Clean                             ();

    XSTRING                 useragent;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

