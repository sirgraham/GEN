//------------------------------------------------------------------------------------------
//  XFILEHASH.H
//
/**
// \class
//
//  File HASH funtions
//
//  @author  Abraham J. Velez
//  @version 22/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XFILEHASHH_
#define _XFILEHASHH_

//---- INCLUDES ----------------------------------------------------------------------------

#include "Hash.h"
#include "XFile.h"
#include "XFileID.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XHASH32;

class XFILEHASH : public XFILECONTAINER
{
  public:
                      XFILEHASH         (HASH* hash);
    virtual          ~XFILEHASH         ();

    bool              Open              (XPATH& pathname, bool readonly = true, bool checkHASH = true, bool checkversion = true);
    bool              Create            (XPATH& pathname, bool checkhash = true);
    bool              Close             ();

    XWORD             GetID             ();
    XWORD             GetType           ();
    XWORD             GetVersion        ();
    XSTRING*          GetIDString       ();

    bool              Set               (XWORD ID, XWORD type, XWORD version, XCHAR* IDstring = NULL);

    bool              SetID             (XWORD ID);
    bool              SetType           (XWORD type);
    bool              SetVersion        (XWORD version);
    bool              SetIDString       (XCHAR* IDstring);
    bool              SetIDString       (XSTRING& IDstring);

    int               GetDataPosition   ();

    bool              UpdateHash        ();

  private:

    void              Clean             ()
                      {
                        hash          = NULL;

                        ID            = 0;
                        type          = 0;
                        version       = 0;

                        hashisupdate  = false;

                        hashposition  = 0;
                        dataposition  = 0;
                      }

    HASH*             hash;

    XWORD             ID;
    XWORD             type;
    XWORD             version;
    XSTRING           IDstring;

    bool              hashisupdate;

    int               hashposition;
    int               dataposition;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

