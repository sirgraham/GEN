/*------------------------------------------------------------------------------------------
//  DIOANDROIDURL.H
*/
/**
// \class
//
//  Data I/O URL class
//
//  @author  Abraham J. Velez
//  @version 13/02/2012 10:37:04
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOANDROIDURL_H_
#define _DIOANDROIDURL_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOURL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


class DIOANDROIDURL : public DIOURL
{
  public:
                              DIOANDROIDURL                     ();
    virtual                  ~DIOANDROIDURL                     ();

                              DIOANDROIDURL                     (int size);
                              DIOANDROIDURL                     (const char* url);
                              DIOANDROIDURL                     (const XCHAR* url);
                              DIOANDROIDURL                     (const XCHAR* url,int size);
                              DIOANDROIDURL                     (DIOURL& url);
                              DIOANDROIDURL                     (XSTRING& url);
                              DIOANDROIDURL                     (XWORD* url);

    virtual bool              ResolveURL                      (XSTRING& IPstring);

  private:

    void                      Clean                           ();

};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif










