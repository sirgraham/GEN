//------------------------------------------------------------------------------------------
//  CIPHER.H
//
/**
// \class
//
//  Cipher Interface
//
//  @author  Abraham J. Velez
//  @version 22/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _CIPHERH_
#define _CIPHERH_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XBase.h"
#include "XBuffer.h"
#include "XString.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define CIPHERMAXKEYS 2

enum CIPHERTYPE
{
  CIPHERTYPE_XOR      = 0 ,
  CIPHERTYPE_DES          ,
  CIPHERTYPE_3DES         ,
  CIPHERTYPE_BLOWFISH     ,
  CIPHERTYPE_AES          ,
};



enum CIPHERKEYTYPE
{
  CIPHERKEYTYPE_UNKNOWN         = 0  ,
  CIPHERKEYTYPE_SYMMETRICAL          ,
  CIPHERKEYTYPE_PUBLIC               ,
  CIPHERKEYTYPE_PRIVATE              ,

};


enum CIPHERCHAININGMODE
{
  CIPHERCHAININGMODE_UNKNOWN    = 0 ,
  CIPHERCHAININGMODE_ECB            ,
  CIPHERCHAININGMODE_CBC            ,
  CIPHERCHAININGMODE_CFB            ,
  CIPHERCHAININGMODE_CTR            ,
};


//---- CLASS -------------------------------------------------------------------------------


class CIPHERKEY
{
  public:
                          CIPHERKEY                 ()                                        { Clean();                                          }
    virtual              ~CIPHERKEY                 ()                                        { Clean();                                          }

    CIPHERKEYTYPE         GetType                   ()                                        { return type;                                      }
    void                  SetType                   ()                                        { this->type = type;                                }

    virtual int           GetSizeInBytes            ()                                        { return 0;                                         }
    int                   GetSizeInBits             ()                                        { return (GetSizeInBytes()*8);                      }

    bool                  CopyFrom                  (CIPHERKEY* key)
                          {
                            if(!key) return false;
                            type = key->GetType();

                            return true;
                          }

  protected:

    CIPHERKEYTYPE         type;

private:

    void                  Clean                     ()
                          {
                            type = CIPHERKEYTYPE_UNKNOWN;
                          }
};




class CIPHERKEYSYMMETRICAL : public CIPHERKEY
{
  public:
                          CIPHERKEYSYMMETRICAL      ();
    virtual              ~CIPHERKEYSYMMETRICAL      ();

    XBYTE*                Get                       (int& size);
    XBUFFER*              Get                       ();

    bool                  Set                       (XBYTE* key, XDWORD size);
    bool                  Set                       (XBUFFER& key);

    int                   GetSizeInBytes            ()                                        { return xbufferkey->GetSize();                     }

    bool                  CopyFrom                  (CIPHERKEYSYMMETRICAL* key)
                          {
                            if(!key) return false;

                            if(!CIPHERKEY::CopyFrom((CIPHERKEY*)key)) return false;

                            xbufferkey->Delete();
                            xbufferkey->Add(key->Get()->Get(), key->Get()->GetSize());

                            return true;
                          }

  private:

    void                  Clean                     ()
                          {
                            xbufferkey = NULL;
                          }

    XBUFFER*              xbufferkey;
};





class CIPHER
{
  public:
                          CIPHER                    ();
    virtual              ~CIPHER                    ();

    CIPHERTYPE            GetType                   ()                                        { return type;                                      }
    void                  SetType                   ()                                        { this->type = type;                                }

    CIPHERCHAININGMODE    GetChainingMode           ()                                        { return chainingmode;                              }
    void                  SetChainingMode           (CIPHERCHAININGMODE chainingmode)         { this->chainingmode = chainingmode;                }

    XBUFFER_PADDINGTYPE   GetPaddingType            ()                                        { return paddingtype;                               }
    void                  SetPaddingType            (XBUFFER_PADDINGTYPE paddingtype)         { this->paddingtype = paddingtype;                  }

    int                   GetPaddingAdjustSize      ()                                        { return paddingadjustsize;                         }
    void                  SetPaddingAdjustSize      (int paddingadjustsize)                   { this->paddingadjustsize = paddingadjustsize;      }

    CIPHERKEY*            GetKey                    (CIPHERKEYTYPE type = CIPHERKEYTYPE_SYMMETRICAL);
    virtual bool          SetKey                    (CIPHERKEY* key, bool integritycheck = false);

    XBYTE*                GetInitVector             ();
    bool                  SetInitVector             (XBYTE* inivector, XDWORD size);
    bool                  SetInitVector             (XBUFFER& inivector);

    virtual bool          Cipher                    (XBYTE* input, XDWORD size);
    bool                  Cipher                    (XBUFFER& input);

    virtual bool          Uncipher                  (XBYTE* input,XDWORD size);
    bool                  Uncipher                  (XBUFFER& input);

    virtual XBYTE*        GetResult                 (int& resultsize);
    XBUFFER*              GetResult                 ();
    bool                  GetResultString           (XSTRING& stringhex);

  protected:

    CIPHERTYPE            type;
    CIPHERCHAININGMODE    chainingmode;
    XBUFFER_PADDINGTYPE   paddingtype;
    int                   paddingadjustsize;

    CIPHERKEY*            keys[CIPHERMAXKEYS];
    XBUFFER*              inivector;
    XBUFFER*              result;

  private:

    void                  Clean                     ();

    CIPHERKEY*            GetKey                    (int index);
    bool                  SetKey                    (int index, CIPHERKEY* key);
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

