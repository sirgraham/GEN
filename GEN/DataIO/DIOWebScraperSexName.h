/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERSEXNAME.H
*/
/**
// \class
//
//  Sex Name (Web Service)
//
//  @author  Abraham J. Velez
//  @version 05/11/2007 16:20:28
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWEBSCRAPERSEXNAME_H_
#define _DIOWEBSCRAPERSEXNAME_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOURL.h"
#include "DIOWebScraper.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DIOWEBSCRAPERSEXNAMETYPE
{
  DIOWEBSCRAPERSEXNAMETYPE_NOTOBTAINED      = 0 ,
  DIOWEBSCRAPERSEXNAMETYPE_NONAME               ,
  DIOWEBSCRAPERSEXNAMETYPE_MALE                 ,
  DIOWEBSCRAPERSEXNAMETYPE_FEMALE               ,
};

#define DIOWEBSCRAPERSEXNAME_NAMEFILE       __L("ws_sexname.xml")
#define DIOWEBSCRAPERSEXNAME_NAMESERVICE    __L("SEXNAME")

/*---- CLASS -----------------------------------------------------------------------------*/



class DIOSEXNAMERESULT :  public DIOWEBSCRAPERCACHERESULT
{
  public:
                                DIOSEXNAMERESULT                  ();
    virtual                    ~DIOSEXNAMERESULT                  ();

    DIOWEBSCRAPERSEXNAMETYPE    Get                               ();
    bool                        Set                               (DIOWEBSCRAPERSEXNAMETYPE sex);

    XSTRING*                    GetNameClear                      ();

  private:

    void                        Clean                             ();

    DIOWEBSCRAPERSEXNAMETYPE    sex;
    XSTRING                     nameclear;
};




class DIOWEBSCRAPERSEXNAME : public DIOWEBSCRAPER
{
  public:
                                DIOWEBSCRAPERSEXNAME              (  );
    virtual                    ~DIOWEBSCRAPERSEXNAME              ();

    bool                        ChangeURL                         (XCHAR* maskurl, DIOURL& url);

    bool                        Get                               (XCHAR* name  , DIOWEBSCRAPERSEXNAMETYPE& sexnametype, XSTRING* nameclear = NULL, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
    bool                        Get                               (XSTRING& name, DIOWEBSCRAPERSEXNAMETYPE& sexnametype, XSTRING* nameclear = NULL, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL,  XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                        Clean                             ();

    XSTRING                     name;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

