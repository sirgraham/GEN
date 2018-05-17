//------------------------------------------------------------------------------------------
//  CIPHER3DES.H
//
/**
// \class
//
//  Cipher / Uncipher 3DES Class
//
//  @author  Abraham J. Velez
//  @version 25/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _CIPHER3DES_H_
#define _CIPHER3DES_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "CipherDES.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

typedef struct
{
  int     mode;
  XDWORD  sk[96];

} CIPHER3DES_CONTEXT;


//---- CLASS -------------------------------------------------------------------------------


class CIPHER3DES : public CIPHERDES
{
  public:

                          CIPHER3DES              ();
    virtual              ~CIPHER3DES              ();

    using                 CIPHER::Cipher;
    using                 CIPHER::Uncipher;

    bool                  Cipher                  (XBYTE* input, XDWORD size);
    bool                  Uncipher                (XBYTE* input, XDWORD size);

  private:

    void                  Clean                   ();

    void                  DES3Set2Key             (XDWORD esk[96], XDWORD dsk[96], XBYTE key[CIPHERDES_KEYSIZE*2]);
    int                   DES3Set2KeyCipher       (CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*2]);
    int                   DES3Set2KeyUncipher     (CIPHER3DES_CONTEXT *ctx, XBYTE key[CIPHERDES_KEYSIZE*2]);

    void                  DES3Set3Key             (XDWORD esk[96], XDWORD dsk[96], XBYTE key[24]);
    int                   DES3Set3KeyCipher       (CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3]);
    int                   DES3Set3KeyUncipher     (CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3]);

    int                   DES3Cipher              (CIPHER3DES_CONTEXT* ctx, XBYTE input[8], XBYTE output[8]);

    bool                  DES3Cipher_ECB          (CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output);
    bool                  DES3Cipher_CBC          (CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output);
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

