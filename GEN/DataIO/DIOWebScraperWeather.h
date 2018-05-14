/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERWEATHER.H
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

#ifndef _DIOWEBSCRAPERWEATHER_H_
#define _DIOWEBSCRAPERWEATHER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOURL.h"
#include "DIOWebScraper.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOWEBSCRAPERWEATHER_NAMEFILE       __L("ws_weather.xml")
#define DIOWEBSCRAPERWEATHER_NAMESERVICE    __L("WEATHER")

/*---- CLASS -----------------------------------------------------------------------------*/



class DIOWEBSCRAPERWEATHER : public DIOWEBSCRAPER
{
  public:
                            DIOWEBSCRAPERWEATHER          (  );
    virtual                ~DIOWEBSCRAPERWEATHER          ();

    bool                    ChangeURL                     (XCHAR* maskurl, DIOURL& url);

    bool                    Get                           (XCHAR* location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                           (XSTRING& location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                    Clean                         ()
                            {
                              iscelsius = false;
                            }

    XSTRING                 location;
    bool                    iscelsius;

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

