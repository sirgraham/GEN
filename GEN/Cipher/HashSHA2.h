/*------------------------------------------------------------------------------------------
//  HASHSHA2.H
*/
/**
// \class
//
//  Hash SHA2 Class
//
//  @author  Abraham J. Velez
//  @version 02/03/2013 12:02:40
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _HASHSHA2_H_
#define _HASHSHA2_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "Hash.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum HASHSHA2TYPE
{
  HASHSHA2TYPE_UNKNOWN = 0  ,
  HASHSHA2TYPE_224        ,
  HASHSHA2TYPE_256        ,
  HASHSHA2TYPE_384        ,
  HASHSHA2TYPE_512        ,
};


#define HASHSHA2_224_DIGEST_SIZE ( 224 / 8)
#define HASHSHA2_256_DIGEST_SIZE ( 256 / 8)
#define HASHSHA2_384_DIGEST_SIZE ( 384 / 8)
#define HASHSHA2_512_DIGEST_SIZE ( 512 / 8)

#define HASHSHA2_256_BLOCK_SIZE  ( 512 / 8)
#define HASHSHA2_512_BLOCK_SIZE  (1024 / 8)
#define HASHSHA2_384_BLOCK_SIZE  HASHSHA2_512_BLOCK_SIZE
#define HASHSHA2_224_BLOCK_SIZE  HASHSHA2_256_BLOCK_SIZE


typedef struct
{
  XDWORD        totalsize;
  XDWORD        size;
  XBYTE         block[2*HASHSHA2_256_BLOCK_SIZE];
  XDWORD        h[8];

} HASHSHA2_256_CTX;


typedef struct
{
  XDWORD        totalsize;
  XDWORD        size;
  XBYTE         block[2*HASHSHA2_512_BLOCK_SIZE];
  XQWORD        h[8];

} HASHSHA2_512_CTX;


typedef HASHSHA2_512_CTX HASHSHA2_384_CTX;
typedef HASHSHA2_256_CTX HASHSHA2_224_CTX;


/*---- CLASS -----------------------------------------------------------------------------*/


class HASHSHA2 : public HASH
{
  public:
                      HASHSHA2              (HASHSHA2TYPE type);
    virtual          ~HASHSHA2              ();

    using             HASH::Do;
    bool              Do                    (XBYTE* input, int size);

    int               GetDefaultSize        ();

  private:

    void              Clean                 ();

    HASHSHA2TYPE      typesha2;

    static XDWORD     Sha224_h0[8];
    static XDWORD     Sha256_h0[8];
    static XQWORD     Sha384_h0[8];
    static XQWORD     Sha512_h0[8];
    static XDWORD     Sha256_k[64];
    static XQWORD     Sha512_k[80];

    void              Sha2_256_Transf       (HASHSHA2_256_CTX* ctx, XBYTE* message, XDWORD blocknb);
    void              Sha2_256              (XBYTE* input, XDWORD size, XBYTE* digest);
    void              Sha2_256_Ini          (HASHSHA2_256_CTX* ctx);
    void              Sha2_256_Update       (HASHSHA2_256_CTX* ctx, XBYTE* input, XDWORD size);
    void              Sha2_256_End          (HASHSHA2_256_CTX* ctx, XBYTE* digest);

    void              Sha2_512_Transf       (HASHSHA2_512_CTX* ctx, XBYTE* input, XDWORD blocknb);
    void              Sha2_512              (XBYTE* input, XDWORD size, XBYTE* digest);
    void              Sha2_512_Ini          (HASHSHA2_512_CTX* ctx);
    void              Sha2_512_Update       (HASHSHA2_512_CTX* ctx, XBYTE* input, XDWORD size);
    void              Sha2_512_End          (HASHSHA2_512_CTX* ctx, XBYTE* digest);

    void              Sha2_384              (XBYTE* input, XDWORD size, XBYTE* digest);
    void              Sha2_384_Ini          (HASHSHA2_384_CTX* ctx);
    void              Sha2_384_Update       (HASHSHA2_384_CTX* ctx, XBYTE* input, XDWORD size);
    void              Sha2_384_End          (HASHSHA2_384_CTX* ctx, XBYTE* digest);

    void              Sha2_224              (XBYTE* input, XDWORD size, XBYTE* digest);
    void              Sha2_224_Ini          (HASHSHA2_224_CTX* ctx);
    void              Sha2_224_Update       (HASHSHA2_224_CTX* ctx, XBYTE* input, XDWORD size);
    void              Sha2_224_End          (HASHSHA2_224_CTX* ctx, XBYTE* digest);
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

