/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERMACMANUFACTURER.H
*/
/**
// \class
//
//  Get Manufactured from MAC (Web Service)
//
//  @author  Abraham J. Velez
//  @version 05/11/2007 16:20:28
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWEBSCRAPERMACMANUFACTURER_H_
#define _DIOWEBSCRAPERMACMANUFACTURER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOMAC.h"
#include "DIOURL.h"
#include "DIOWebScraper.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOWEBSCRAPERMACMANUFACTURER_NAMEFILE     __L("ws_publicip.xml")
#define DIOWEBSCRAPERMACMANUFACTURER_NAMESERVICE  __L("MACMANUFACTURED")


/*---- CLASS -----------------------------------------------------------------------------*/


class DIOMACMANUFACTUREDRESULT :  public DIOWEBSCRAPERCACHERESULT
{
  public:
                              DIOMACMANUFACTUREDRESULT          ();
    virtual                  ~DIOMACMANUFACTUREDRESULT          ();

    XSTRING*                  Get                               ();

  private:

    void                      Clean                             ();

    XSTRING                   manufactured;
};



class DIOWEBSCRAPERMACMANUFACTURER : public DIOWEBSCRAPER
{
  public:
                              DIOWEBSCRAPERMACMANUFACTURER      (  );
    virtual                  ~DIOWEBSCRAPERMACMANUFACTURER      ();

    bool                      ChangeURL                         (XCHAR* maskurl, DIOURL& url);

    bool                      Get                               (DIOMAC& MAC, XSTRING& manufactured, int timeoutforurl  = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                      Clean                             ();

    DIOMAC                    MAC;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

