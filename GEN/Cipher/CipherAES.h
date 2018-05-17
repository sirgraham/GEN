//------------------------------------------------------------------------------------------
//  CIPHERAES.H
//
/**
// \class
//
//  Cipher / Uncipher AES Class
//
//  @author  Abraham J. Velez
//  @version 25/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _CIPHERAES_H_
#define _CIPHERAES_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "Cipher.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define CIPHERAES_ENCRYPT     1
#define CIPHERAES_DECRYPT     0


typedef struct  { int       nr;                     /*!<  number of rounds  */
                  XDWORD*   rk;                     /*!<  AES round keys    */
                  XDWORD    buf[68];                /*!<  unaligned data    */

                } CIPHERAES_CONTEXT;


//---- CLASS -------------------------------------------------------------------------------


class CIPHERAES : public CIPHER
{
  public:

                          CIPHERAES               ();
    virtual              ~CIPHERAES               ();

    using                 CIPHER::Cipher;
    using                 CIPHER::Uncipher;

    bool                  Cipher                  (XBYTE* input,  XDWORD size);
    bool                  Uncipher                (XBYTE* input,  XDWORD size);

  private:

    void                  Clean                   ();

    void                  AESGenTables            (void);

    bool                  AESSetKeyCipher         (CIPHERAES_CONTEXT* ctx, XBYTE* key, XDWORD keysize);
    bool                  AESSetKeyUncipher       (CIPHERAES_CONTEXT* ctx, XBYTE* key, XDWORD keysize);

    bool                  AESCipher_ECB_Block     (CIPHERAES_CONTEXT* ctx, int mode, XBYTE input[16], XBYTE output[16]);

    bool                  AESCipher_ECB           (CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, XBYTE* input, XBYTE* output);
    bool                  AESCipher_CBC           (CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, XBYTE iv[16], XBYTE* input, XBYTE* output);
    bool                  AESCipher_CFB128        (CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, int* iv_off, XBYTE iv[16], XBYTE* input, XBYTE* output);
    bool                  AESCipher_CTR           (CIPHERAES_CONTEXT* ctx, XDWORD size, int*  nc_off, XBYTE nonce_counter[16], XBYTE stream_block[16], XBYTE* input, XBYTE* output);

    static XBYTE          FSb[256];
    static XDWORD         FT0[256];
    static XDWORD         FT1[256];
    static XDWORD         FT2[256];
    static XDWORD         FT3[256];

    static XBYTE          RSb[256];
    static XDWORD         RT0[256];
    static XDWORD         RT1[256];
    static XDWORD         RT2[256];
    static XDWORD         RT3[256];

    static XDWORD         RCON[10];

    static int            initdone;

};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

