/*------------------------------------------------------------------------------------------
//  DIOLINUXURL.H
*/
/**
// \class
//
//  Linux Data I/O URL class
//
//  @author  Abraham J. Velez
//  @version 13/02/2012 10:37:04
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOLINUXURL_H_
#define _DIOLINUXURL_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOURL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


class DIOLINUXURL : public DIOURL
{
  public:
                              DIOLINUXURL                     ();
    virtual                  ~DIOLINUXURL                     ();

                              DIOLINUXURL                     (int size);
                              DIOLINUXURL                     (const char* url);
                              DIOLINUXURL                     (const XCHAR* url);
                              DIOLINUXURL                     (const XCHAR* url,int size);
                              DIOLINUXURL                     (DIOURL& url);
                              DIOLINUXURL                     (XSTRING& url);
                              DIOLINUXURL                     (XWORD* url);

    virtual bool              ResolveURL                      (XSTRING& IPstring);

  private:

    void                      Clean                           ();

};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif










