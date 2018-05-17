//------------------------------------------------------------------------------------------
//  CIPHERBLOWFISH.H
//
/**
// \class
//
//  Cipher / Uncipher Blowfish Class
//
//  @author  Abraham J. Velez
//  @version 25/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _CIPHERBLOWFISH_H_
#define _CIPHERBLOWFISH_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "Cipher.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define CIPHERBLOWFISH_ENCRYPT     1
#define CIPHERBLOWFISH_DECRYPT     0
#define CIPHERBLOWFISH_MAX_KEY     448
#define CIPHERBLOWFISH_MIN_KEY     32
#define CIPHERBLOWFISH_ROUNDS      16         /* when increasing this value, make sure to extend the initialisation vectors */
#define CIPHERBLOWFISH_BLOCKSIZE   8          /* Blowfish uses 64 bit blocks */


typedef struct  { XDWORD  P[CIPHERBLOWFISH_ROUNDS + 2];    /*!<  Blowfish round keys    */
                  XDWORD  S[4][256];                 /*!<  key dependent S-boxes  */

                } CIPHERBLOWFISH_CONTEXT;

//---- CLASS -------------------------------------------------------------------------------


class CIPHERBLOWFISH : public CIPHER
{
  public:

                          CIPHERBLOWFISH          ();
    virtual              ~CIPHERBLOWFISH          ();

    using                 CIPHER::Cipher;
    using                 CIPHER::Uncipher;

    bool                  Cipher                  (XBYTE* input, XDWORD size);
    bool                  Uncipher                (XBYTE* input, XDWORD size);

  private:

    void                  Clean                   ();

    static XDWORD         P[CIPHERBLOWFISH_ROUNDS + 2];
    static XDWORD         S[4][256];

    XDWORD                F                       (CIPHERBLOWFISH_CONTEXT* ctx, XDWORD x);

    void                  BlowfishCipher          (CIPHERBLOWFISH_CONTEXT* ctx, XDWORD* xl, XDWORD* xr);
    void                  BlowfishUncipher        (CIPHERBLOWFISH_CONTEXT* ctx, XDWORD* xl, XDWORD* xr) ;

    bool                  BlowfishSetKey          (CIPHERBLOWFISH_CONTEXT* ctx, XBYTE* key, XDWORD keysize);

    bool                  BlowfishCipher_ECB      (CIPHERBLOWFISH_CONTEXT* ctx, int mode, XBYTE input[CIPHERBLOWFISH_BLOCKSIZE], XBYTE output[CIPHERBLOWFISH_BLOCKSIZE]);
    bool                  BlowfishCipher_CBC      (CIPHERBLOWFISH_CONTEXT* ctx, int mode, XDWORD size, XBYTE iv[CIPHERBLOWFISH_BLOCKSIZE], XBYTE* input, XBYTE* output);
    bool                  BlowfishCipher_CFB64    (CIPHERBLOWFISH_CONTEXT* ctx, int mode, XDWORD size, int* iv_off, XBYTE iv[CIPHERBLOWFISH_BLOCKSIZE], XBYTE* input, XBYTE* output);
    bool                  BlowfishCipher_CTR      (CIPHERBLOWFISH_CONTEXT* ctx, XDWORD size, int* nc_off, XBYTE nonce_counter[CIPHERBLOWFISH_BLOCKSIZE], XBYTE stream_block[CIPHERBLOWFISH_BLOCKSIZE], XBYTE* input, XBYTE* output);
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif





