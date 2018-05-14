/*------------------------------------------------------------------------------------------
//  HASH.H
*/
/**
// \class
//
//  Hash Generic Class
//
//  @author  Abraham J. Velez
//  @version 02/03/2013 12:02:40
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _HASHH_
#define _HASHH_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XDateTime.h"
#include "XPath.h"
#include "XString.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define HASHMAXFILESIZEBUFFER (1024*8)
#define HASHALLFILESIZE       -1

enum HASHTYPE
{
  HASHTYPE_NONE     = 0 ,
  HASHTYPE_CKS16        ,
  HASHTYPE_CRC16        ,
  HASHTYPE_CRC32        ,
  HASHTYPE_MD2          ,
  HASHTYPE_MD4          ,
  HASHTYPE_MD5          ,
  HASHTYPE_SHA1         ,
  HASHTYPE_SHA224       ,
  HASHTYPE_SHA256       ,
  HASHTYPE_SHA384       ,
  HASHTYPE_SHA512       ,
  HASHTYPE_RIPEMD160    ,
};

/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XFILE;

class HASH
{
  public:
                                  HASH                          ();
    virtual                      ~HASH                          ();

    HASHTYPE                      GetType                       ()                    { return type;                  }
    XSTRING*                      GetOUINoSign                  ()                    { return &OUInosignstring;      };

    virtual bool                  Do                            (XBYTE* input, int size);
    bool                          Do                            (XBUFFER& input);

    bool                          Do                            (XPATH& xpath, int size = HASHALLFILESIZE, int pos = 0);
    bool                          Do                            (XFILE* xfile, int size = HASHALLFILESIZE, int pos = 0);

    virtual bool                  ResetResult                   ();

    virtual int                   GetDefaultSize                ();

    virtual XBUFFER*              GetResult                     ();
    virtual XBYTE*                GetResult                     (int& resultsize);

    bool                          GetResultString               (XSTRING& stringhex);

  protected:

    HASHTYPE                      type;
    XSTRING                       OUInosignstring;
    XBUFFER*                      result;

  private:

    void                          Clean                         ();
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

