/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERUSERAGENTID.CPP
//
//  User Agent ID (Web Service)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/11/2007 16:26:37
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"
#include "DIOWebScraperUserAgentID.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOUSERAGENTIDRESULT:: DIOUSERAGENTIDRESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:47:38
//
//  @return
//  */
/*-----------------------------------------------------------------*/
 DIOUSERAGENTIDRESULT:: DIOUSERAGENTIDRESULT()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOUSERAGENTIDRESULT::~ DIOUSERAGENTIDRESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:47:57
//
//  @return
//  */
/*-----------------------------------------------------------------*/
 DIOUSERAGENTIDRESULT::~ DIOUSERAGENTIDRESULT()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOUSERAGENTIDRESULT::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:48:19
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOUSERAGENTIDRESULT::GetBrowser()
{
  return &browser;
}



/*-------------------------------------------------------------------
//  DIOUSERAGENTIDRESULT::GetSO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2013 20:18:38
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOUSERAGENTIDRESULT::GetSO()
{
  return &systemoperative;
}




/*-------------------------------------------------------------------
//  DIOUSERAGENTIDRESULT::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:48:36
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOUSERAGENTIDRESULT::Clean()
{

}





/*-------------------------------------------------------------------
//  DIOWEBSCRAPERUSERAGENTID::DIOWEBSCRAPERUSERAGENTID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2014 16:20:40
//
//  @return       void :



//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERUSERAGENTID::DIOWEBSCRAPERUSERAGENTID(  ) : DIOWEBSCRAPER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERUSERAGENTID::~DIOWEBSCRAPERUSERAGENTID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/11/2007 16:28:53
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERUSERAGENTID::~DIOWEBSCRAPERUSERAGENTID()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERUSERAGENTID::ChangeURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/04/2013 22:29:13
//
//  @return       bool :
//  @param        maskurl :
//  @param        url :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERUSERAGENTID::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  DIOURL urluseragent;

  urluseragent = useragent.Get();
  urluseragent.EncodeUnsafeChars();

  url.Format(maskurl, urluseragent.Get());

  //url.EncodeUnsafeChars();

  return true;
}


/*-------------------------------------------------------------------
//  DIOWEBSCRAPERUSERAGENTID::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/05/2014 16:50:17
//
//  @return       bool :
//
//  @param        useragent :
//  @param        browser :
//  @param        systemoperative :
//  @param        timeout :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERUSERAGENTID::Get(XCHAR* useragent, XSTRING& browser, XSTRING& systemoperative, int timeout, XSTRING* localIP, bool usecache)
{
  bool status = false;
  if(xmutexdo) xmutexdo->Lock();

  this->useragent = useragent;
  if(this->useragent.IsEmpty())
    {
      if(xmutexdo) xmutexdo->UnLock();
      return false;
    }

  DIOUSERAGENTIDRESULT* useragentresult;
  if(usecache)
    {
      useragentresult= (DIOUSERAGENTIDRESULT*)cache->Get(this->useragent);
      if(useragentresult)
        {
          browser         = (*useragentresult->GetBrowser());
          systemoperative = (*useragentresult->GetSO());

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  if(Load(DIOWEBSCRAPERUSERAGENTID_NAMEFILE))
    {
      if(Do(DIOWEBSCRAPERUSERAGENTID_NAMESERVICE, timeout, localIP))
        {
          browser         = GetValue(__L("BROWSER"));
          systemoperative = GetValue(__L("SO"));

          if(usecache)
            {
              useragentresult = new DIOUSERAGENTIDRESULT();
              if(useragentresult)
                {
                  useragentresult->GetBrowser()->Set(browser);
                  useragentresult->GetSO()->Set(systemoperative);

                  cache->Add(this->useragent, useragentresult);
                }
            }

          status = true;
        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}


/*-------------------------------------------------------------------
//  DIOWEBSCRAPERUSERAGENTID::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/05/2014 16:50:09
//
//  @return       bool :
//
//  @param        useragent :
//  @param        browser :
//  @param        systemoperative :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERUSERAGENTID::Get(XSTRING& useragent, XSTRING& browser, XSTRING& systemoperative, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(useragent.Get(), browser, systemoperative, timeoutforurl, localIP, usecache);
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERUSERAGENTID::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/11/2007 16:28:12
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWEBSCRAPERUSERAGENTID::Clean()
{

}

