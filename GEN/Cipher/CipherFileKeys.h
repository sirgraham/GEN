/*------------------------------------------------------------------------------------------
//  CIPHERFILEKEYS.H
*/
/**
// \class
//
//  Cipher File Keys (Own keys cipher file for GEN)
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 19/03/2014 13:09:53
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _CIPHERFILEKEYS_H_
#define _CIPHERFILEKEYS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "XDateTime.h"

#include "Cipher.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define CIPHERFILEKEYS_EXT                            __L(".key")

#define CIPHERFILEKEYS_NODENAME_ROOT                  __L("CIPHERKEYSFILE")
#define CIPHERFILEKEYS_NODENAME_CREATOR               __L("CREATOR")
#define CIPHERFILEKEYS_ATTRNAME_CREATOR_CREATENAME    __L("NAME")
#define CIPHERFILEKEYS_ATTRNAME_CREATOR_CREATEDATE    __L("CREATED")
#define CIPHERFILEKEYS_ATTRNAME_CREATOR_LASTMODDATE   __L("LASTMODIFIED")

#define CIPHERFILEKEYS_NODENAME_CIPHERKEY             __L("CIPHERKEY")
#define CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_TYPEKEY     __L("TYPE")
#define CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_TYPECIPHER  __L("TYPECIPHER")
#define CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_SIZEBITS    __L("SIZEBITS")
#define CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_KEY         __L("KEY")
#define CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_MODULUS     __L("MODULUS")
#define CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_EXPONENT    __L("EXPONENT")
#define CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_FACTOR1     __L("1STPRIMEFACTOR")
#define CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_FACTOR2     __L("2NDPRIMEFACTOR")


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class CIPHERKEY;
class XFILEXML;

class CIPHERFILEKEYS
{
  public:
                           CIPHERFILEKEYS                 (XPATH& xpath);
    virtual               ~CIPHERFILEKEYS                 ();

    XSTRING*               GetCreatorName                 ()                                      { return &creatorname;                    }
    XDATETIME*             GetCreatorDateTime             ()                                      { return &creatordatetime;                }
    XDATETIME*             GetCreatorLastModifiedDateTime ()                                      { return &creatorlastmodifieddatetime;    }

    bool                   HaveKey                        (CIPHERKEYTYPE type);
    CIPHERKEY*             GetKey                         (CIPHERKEYTYPE type);
    bool                   AddKey                         (CIPHERKEY& key);
    bool                   DeleteKey                      (CIPHERKEYTYPE type);
    bool                   DeleteAllKeys                  ();

    bool                   UpdateFile                     ();

    bool                   ExportToPEMFile                (CIPHERKEY* key, XSTRING& publicPEM);
    bool                   ExportToPEMFile                (CIPHERKEY* key, XPATH& xpath);

    XPATH                  GetXPath                       ()                                      { return xpath;                           }
    XFILEXML*              GetXFileXML                    ()                                      { return xfilexml;                        }


  private:

    bool                   ReadAllFile                    ();

    void                   Clean                          ()
                           {
                              xfilexml  = NULL;
                           }

    XPATH                  xpath;
    XFILEXML*              xfilexml;

    XSTRING                creatorname;
    XDATETIME              creatordatetime;
    XDATETIME              creatorlastmodifieddatetime;
    XVECTOR<CIPHERKEY*>    keys;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

