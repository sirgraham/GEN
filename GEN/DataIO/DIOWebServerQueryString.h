/*------------------------------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING.H
*/
/**
// \class
//
//  DIO Web Server Query String
//
//  @author  Abraham J. Velez
//  @version 02/06/2013 11:35:32
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWEBSERVERQUERYSTRING_H_
#define _DIOWEBSERVERQUERYSTRING_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"
#include "XMap.h"

#include "DIOURL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;

class DIOWEBSERVERQUERYSTRING
{
  public:
                                    DIOWEBSERVERQUERYSTRING           ();
    virtual                        ~DIOWEBSERVERQUERYSTRING           ();

    int                             GetNParams                        ();
    XMAP<XSTRING*, XSTRING*>*       GetParams                         ();

    bool                            AddParam                          (XSTRING& ID, XSTRING& value);
    bool                            AddParam                          (XCHAR* ID  , XSTRING& value);
    bool                            AddParam                          (XSTRING& ID, XCHAR* value);
    bool                            AddParam                          (XCHAR* ID  , XCHAR* value);

    XSTRING*                        GetParam                          (XSTRING& ID);
    XSTRING*                        GetParam                          (XCHAR* ID);

    XSTRING*                        GetParamID                        (int index);
    XSTRING*                        GetParamValue                     (int index);

    bool                            DelParam                          (XSTRING& ID);
    bool                            DelParam                          (XCHAR* ID);

    bool                            DelAllParam                       ();

    int                             GetParamsFromURL                  (DIOURL& url);
    int                             GetParamsFromURL                  (XSTRING& url);
    int                             GetParamsFromURL                  (XCHAR* url);

    bool                            CreateURLFromParams               (XSTRING& urlpart);

  protected:

    XMAP<XSTRING*, XSTRING*>        params;

  private:

    void                            Clean                             ();

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

