/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERCACHE.CPP
//
//   Web Scraper Cache Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 23/06/2013 9:54:10
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"

#include "DIOWebScraperCache.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//  DIOWEBSCRAPERCACHE::DIOWEBSCRAPERCACHE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2014 18:53:41
//
//  @return

*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERCACHE::DIOWEBSCRAPERCACHE()
{
  Clean();

  xmutexlist = xfactory->Create_Mutex();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERCACHERESULT::~DIOWEBSCRAPERCACHE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 10:00:52
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERCACHE::~DIOWEBSCRAPERCACHE()
{
  xfactory->Delete_Mutex(xmutexlist);

  Clean();
}


/*-------------------------------------------------------------------
//  DIOWEBSCRAPERCACHE::Add
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 10:01:46
//
//  @return       bool :
//  @param        ask :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERCACHE::Add(XSTRING& ask, DIOWEBSCRAPERCACHERESULT* result)
{
  DIOWEBSCRAPERCACHERESULT* _result = Get(ask);
  if(_result)  return false;

  if(xmutexlist) xmutexlist->Lock();

  bool status = false;

  XSTRING* _ask = new XSTRING();
  if(_ask)
    {
      (*_ask) = ask.Get();
      if(cache.Add(_ask, result))
        {
          status = true;

        } else delete _ask;
    }

  if(xmutexlist) xmutexlist->UnLock();

  return status;
}


/*-------------------------------------------------------------------
//  DIOWEBSCRAPERCACHE::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 10:02:20
//
//  @return       DIOWEBSCRAPERCACHERESULT* :
//  @param        ask :
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERCACHERESULT* DIOWEBSCRAPERCACHE::Get(XSTRING& ask)
{
  if(xmutexlist) xmutexlist->Lock();

  for(XDWORD c=0;c<cache.GetSize();c++)
    {
      XSTRING* _ask = (XSTRING*)cache.GetKey(c);
      if(_ask)
        {
          if(!_ask->Compare(ask.Get()))
            {
              DIOWEBSCRAPERCACHERESULT* result  = cache.GetElement(c);
              if(result)
                {
                  if(xmutexlist) xmutexlist->UnLock();
                  return result;
                }
            }
        }
    }

  if(xmutexlist) xmutexlist->UnLock();

  return NULL;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERCACHE::DeleteAll
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 10:02:56
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERCACHE::DeleteAll()
{
  if(cache.IsEmpty())  return false;

  if(xmutexlist) xmutexlist->Lock();

  cache.DeleteKeyContents();
  cache.DeleteElementContents();
  cache.DeleteAll();

  if(xmutexlist) xmutexlist->UnLock();

  return true;
}

