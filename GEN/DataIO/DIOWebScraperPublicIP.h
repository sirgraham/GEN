/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERPUBLICIP.H
*/
/**
// \class
//
//  Public IP (Web Service)
//
//  @author  Abraham J. Velez
//  @version 05/11/2007 16:20:28
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWEBSCRAPERPUBLICIP_H_
#define _DIOWEBSCRAPERPUBLICIP_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOIP.h"
#include "DIOWebScraper.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOWEBSCRAPERPUBLICIP_NAMEFILE      __L("ws_publicip.xml")
#define DIOWEBSCRAPERPUBLICIP_NAMESERVICE   __L("PUBLICIP")


/*---- CLASS -----------------------------------------------------------------------------*/


class DIOPUBLICIP :  public DIOWEBSCRAPERCACHERESULT
{
  public:
                              DIOPUBLICIP                       ();
    virtual                  ~DIOPUBLICIP                       ();

    DIOIP*                    Get                               ();

  private:

    void                      Clean                             ();

    DIOIP                     IP;
};




class DIOWEBSCRAPERPUBLICIP : public DIOWEBSCRAPER
{
  public:
                              DIOWEBSCRAPERPUBLICIP             (  );
    virtual                  ~DIOWEBSCRAPERPUBLICIP             ();

    bool                      Get                               (DIOIP& IP, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                      Clean                             ();

    DIOIP                     IP;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

