/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP.H
*/
/**
// \class
//
//  Geolocation IP (Web Service)
//
//  @author  Abraham J. Velez
//  @version 05/11/2007 16:20:28
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWEBSCRAPERGEOLOCATIONIP_H_
#define _DIOWEBSCRAPERGEOLOCATIONIP_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XMap.h"

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIOWebScraper.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOWEBSCRAPERGEOLOCATIONIP_NAMEFILE       __L("ws_geolocationip.xml")
#define DIOWEBSCRAPERGEOLOCATIONIP_NAMESERVICE    __L("GEOLOCATIONIP")

/*---- CLASS -----------------------------------------------------------------------------*/


class DIOGEOLOCATIONIP : public DIOWEBSCRAPERCACHERESULT
{
  public:

                            DIOGEOLOCATIONIP                  ();
    virtual                ~DIOGEOLOCATIONIP                  ();

    XCHAR*                  GetCountry                        ();
    XCHAR*                  GetState                          ();
    XCHAR*                  GetCity                           ();
    XCHAR*                  GetISP                            ();
    XCHAR*                  GetOrganization                   ();
    float                   GetLatitude                       ();
    float                   GetLongitude                      ();

    bool                    IsEmpty                           ();

    virtual bool            CopyFrom                          (DIOWEBSCRAPERCACHERESULT* result);
    virtual bool            CopyTo                            (DIOWEBSCRAPERCACHERESULT* result);

    bool                    Set                               (XSTRING& country, XSTRING& state, XSTRING& city, XSTRING& ISP, XSTRING& organization);
    bool                    Set                               (XCHAR* country, XCHAR* state = NULL, XCHAR* city = NULL, XCHAR* ISP = NULL, XCHAR* organization = NULL);
    bool                    Set                               (float latitude, float longitude);

  private:

    void                    Clean                             ();

    XSTRING                 country;
    XSTRING                 state;
    XSTRING                 city;
    XSTRING                 ISP;
    XSTRING                 organization;
    float                   latitude;
    float                   longitude;
};




class DIOWEBSCRAPERGEOLOCATIONIP : public DIOWEBSCRAPER
{
  public:
                            DIOWEBSCRAPERGEOLOCATIONIP        (    DIOWEBCLIENT* webclient = NULL);
    virtual                ~DIOWEBSCRAPERGEOLOCATIONIP        ();


    bool                    ChangeURL                         (XCHAR* maskurl, DIOURL& url);

    bool                    Get                               (XCHAR* IP  , DIOGEOLOCATIONIP& geolocationIP, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XSTRING& IP, DIOGEOLOCATIONIP& geolocationIP, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (DIOIP& IP  , DIOGEOLOCATIONIP& geolocationIP, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                    Clean                             ();

    DIOIP                   IP;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

