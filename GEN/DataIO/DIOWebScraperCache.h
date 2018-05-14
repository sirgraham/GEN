/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERCACHE.H
*/
/**
// \class
//
//  Web Scraper Cache Class
//
//  @author  Abraham J. Velez
//  @version 05/11/2007 16:20:28
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWEBSCRAPERCACHE_H_
#define _DIOWEBSCRAPERCACHE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "XMap.h"
#include "XString.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XMUTEX;


class DIOWEBSCRAPERCACHERESULT
{
  public:

                                                DIOWEBSCRAPERCACHERESULT              ()                                    {                 }
    virtual                                    ~DIOWEBSCRAPERCACHERESULT              ()                                    {                 }

    virtual bool                                CopyFrom                              (DIOWEBSCRAPERCACHERESULT* result)    { return false;   }
    virtual bool                                CopyTo                                (DIOWEBSCRAPERCACHERESULT* result)    { return false;   }
};



class DIOWEBSCRAPERCACHE
{
  public:
                                                DIOWEBSCRAPERCACHE                    ();
    virtual                                    ~DIOWEBSCRAPERCACHE                    ();


    bool                                        Add                                   (XSTRING& ask, DIOWEBSCRAPERCACHERESULT* result);
    DIOWEBSCRAPERCACHERESULT*                   Get                                   (XSTRING& ask);
    bool                                        DeleteAll                             ();

  private:

    void                                        Clean                                 ()
                                                {
                                                  xmutexlist = NULL;
                                                }

    XMAP<XSTRING*, DIOWEBSCRAPERCACHERESULT*>   cache;
    XMUTEX*                                     xmutexlist;
};




/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

