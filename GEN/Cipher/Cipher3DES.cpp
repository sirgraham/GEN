//------------------------------------------------------------------------------------------
//  CIPHER3DES.CPP
//
//  Cipher / Uncipher DES
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 25/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XFactory.h"
#include "XBuffer.h"

#include "Cipher3DES.h"

#include "XMemory.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




//-------------------------------------------------------------------
//  CIPHER3DES::CIPHER3DES
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2006 19:42:40
//
//  @return
//  */
//-------------------------------------------------------------------
CIPHER3DES::CIPHER3DES() : CIPHERDES()
{
  type             = CIPHERTYPE_3DES;
  paddingadjustsize = 16;
}




//-------------------------------------------------------------------
//  CIPHER3DES::~CIPHER3DES
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2006 19:42:37
//
//  @return
//  */
//-------------------------------------------------------------------
CIPHER3DES::~CIPHER3DES()
{

}




//-------------------------------------------------------------------
//  CIPHER3DES::Cipher
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/04/2002 10:22:59
//
//  @return       bool :
//  @param        input :
//  @param        size :
*/
//-------------------------------------------------------------------
bool CIPHER3DES::Cipher(XBYTE* input, XDWORD size)
{
  if(!size) return false;

  XBUFFER             inputpadding;
  CIPHER3DES_CONTEXT ctx;
  XBYTE               iv[8];
  int                 sizecipher = size;

  memset(iv,0,8);
  if(inivector && inivector->GetSize()>=8) memcpy(iv, inivector->Get() ,8);

  inputpadding.Add(input, size);
  inputpadding.Padding_Add(paddingadjustsize, paddingtype);

  result->Delete();
  result->Add(input, size);
  result->Padding_Add(paddingadjustsize, paddingtype);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey();
  if(!key)               return false;
  if(!key->Get()->Get()) return false;

  if(key->Get()->GetSize() <= 16)
        DES3Set2KeyCipher(&ctx, (XBYTE *)key->Get()->Get());
   else DES3Set3KeyCipher(&ctx, (XBYTE *)key->Get()->Get());

  bool status;

  if(this->GetChainingMode() == CIPHERCHAININGMODE_ECB)
         status = DES3Cipher_ECB(&ctx, CIPHERDES_ENCRYPT, result->GetSize(), iv, inputpadding.Get(),  result->Get());
    else status = DES3Cipher_CBC(&ctx, CIPHERDES_ENCRYPT, result->GetSize(), iv, inputpadding.Get(),  result->Get());

  return status;
}




/*-------------------------------------------------------------------
//  CIPHER3DES::Uncipher
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:44:01
//
//  @return       bool :
//  @param        input :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool CIPHER3DES::Uncipher(XBYTE* input, XDWORD size)
{
  if(!size) return false;

  CIPHER3DES_CONTEXT ctx;
  XBYTE               iv[8];
  int                 sizecipher = size;

  memset(iv,0,8);
  if(inivector && inivector->GetSize()>=8) memcpy(iv, inivector->Get() ,8);

  result->Delete();
  result->Add(input, size);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey();
  if(!key)               return false;
  if(!key->Get()->Get()) return false;

  if(key->Get()->GetSize() <= 16)
        DES3Set2KeyUncipher(&ctx, (XBYTE *)key->Get()->Get());
   else DES3Set3KeyUncipher(&ctx, (XBYTE *)key->Get()->Get());

  bool status;

  if(this->GetChainingMode() == CIPHERCHAININGMODE_ECB)
         status = DES3Cipher_ECB(&ctx,  CIPHERDES_DECRYPT, size, iv, input,  result->Get());
    else status = DES3Cipher_CBC(&ctx,  CIPHERDES_DECRYPT, size, iv, input,  result->Get());

  result->Padding_Delete();

  return status;
}




/*-------------------------------------------------------------------
//  CIPHER3DES::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/03/2013 23:51:12
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void CIPHER3DES::Clean()
{

}




/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Set2Key
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:16:44
//
//  @return       void :
//  @param        esk[96] :
//  @param        dsk[96] :
//  @param        key[CIPHERDES_KEYSIZE*2] :
*/
/*-----------------------------------------------------------------*/
void CIPHER3DES::DES3Set2Key(XDWORD esk[96], XDWORD dsk[96], XBYTE key[CIPHERDES_KEYSIZE*2])
{
  DESSetKey(esk, key);

  DESSetKey(dsk + 32, key + 8);

  for(int c=0; c<32; c+=2 )
    {
       dsk[c     ] = esk[30 - c];
       dsk[c +  1] = esk[31 - c];

       esk[c + 32] = dsk[62 - c];
       esk[c + 33] = dsk[63 - c];

       esk[c + 64] = esk[c    ];
       esk[c + 65] = esk[c + 1];

       dsk[c + 64] = dsk[c    ];
       dsk[c + 65] = dsk[c + 1];
    }
}



/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Set2KeyCipher
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:16:50
//
//  @return       int :
//  @param        ctx :
//  @param        key[CIPHERDES_KEYSIZE*2] :
*/
/*-----------------------------------------------------------------*/
int CIPHER3DES::DES3Set2KeyCipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*2])
{
  XDWORD sk[96];

  DES3Set2Key(ctx->sk, sk, key);

  memset(sk,  0, sizeof(sk));

  return 0;
}



/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Set2KeyUncipher
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:16:55
//
//  @return       int :
//  @param        *ctx :
//  @param        key[CIPHERDES_KEYSIZE*2] :
*/
/*-----------------------------------------------------------------*/
int CIPHER3DES::DES3Set2KeyUncipher(CIPHER3DES_CONTEXT *ctx, XBYTE key[CIPHERDES_KEYSIZE*2])
{
  XDWORD sk[96];

  DES3Set2Key(sk, ctx->sk, key);

  memset(sk,  0, sizeof(sk));

  return 0;
}




/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Set3Key
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:17:01
//
//  @return       void :
//  @param        esk[96] :
//  @param        dsk[96] :
//  @param        key[24] :
*/
/*-----------------------------------------------------------------*/
void CIPHER3DES::DES3Set3Key(XDWORD esk[96], XDWORD dsk[96], XBYTE key[24])
{
  DESSetKey(esk, key);
  DESSetKey(dsk + 32, key +  8);
  DESSetKey(esk + 64, key + 16);

  for(int c=0; c<32; c+=2 )
    {
      dsk[c     ] = esk[94 - c];
      dsk[c +  1] = esk[95 - c];

      esk[c + 32] = dsk[62 - c];
      esk[c + 33] = dsk[63 - c];

      dsk[c + 64] = esk[30 - c];
      dsk[c + 65] = esk[31 - c];
    }
}



/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Set3KeyCipher
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:17:07
//
//  @return       int :
//  @param        ctx :
//  @param        key[CIPHERDES_KEYSIZE*3] :
*/
/*-----------------------------------------------------------------*/
int CIPHER3DES::DES3Set3KeyCipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3])
{
  XDWORD sk[96];

  DES3Set3Key(ctx->sk, sk, key);

  memset(sk, 0, sizeof(sk));

  return 0;
}




/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Set3KeyUncipher
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:17:13
//
//  @return       int :
//  @param        ctx :
//  @param        key[CIPHERDES_KEYSIZE*3] :
*/
/*-----------------------------------------------------------------*/
int CIPHER3DES::DES3Set3KeyUncipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3])
{
  XDWORD sk[96];

  DES3Set3Key(sk, ctx->sk, key);

  memset(sk, 0, sizeof(sk));

  return 0;
}





/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Cipher
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:17:47
//
//  @return       int :
//  @param        ctx :
//  @param        input[8] :
//  @param        output[8] :
*/
/*-----------------------------------------------------------------*/
int CIPHER3DES::DES3Cipher(CIPHER3DES_CONTEXT* ctx, XBYTE input[8], XBYTE output[8])
{
  XDWORD  X;
  XDWORD  Y;
  XDWORD  T;
  XDWORD* SK;

  SK = ctx->sk;

  GET_XDWORD_BE( X, input, 0 );
  GET_XDWORD_BE( Y, input, 4 );

  DES_IP( X, Y );

  for(int c=0; c<8; c++)
    {
      DES_ROUND(Y, X);
      DES_ROUND(X, Y);
    }

  for(int c=0; c<8; c++)
    {
      DES_ROUND(X, Y);
      DES_ROUND(Y, X);
    }

  for(int c=0; c<8; c++ )
    {
      DES_ROUND(Y, X);
      DES_ROUND(X, Y);
    }

  DES_FP( Y, X );

  PUT_XDWORD_BE( Y, output, 0 );
  PUT_XDWORD_BE( X, output, 4 );

  return 0;
}



/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Cipher_ECB
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:17:57
//
//  @return       int :
//  @param        ctx :
//  @param        mode :
//  @param        length :
//  @param        iv[8] :
//  @param        input :
//  @param        output :
*/
/*-----------------------------------------------------------------*/
bool CIPHER3DES::DES3Cipher_ECB(CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
{
  XBYTE temp[8];

  if(length % 8) return false;

  if(mode == CIPHERDES_ENCRYPT)
    {
      while(length > 0)
        {
          DES3Cipher( ctx, output, output);

          memcpy( iv, output, 8 );

          input  += 8;
          output += 8;
          length -= 8;
        }
    }
   else /* CIPHERDES_DECRYPT */
    {
      while(length > 0)
        {
          memcpy(temp, input, 8);

          DES3Cipher( ctx, input, output );

          memcpy( iv, temp, 8 );

          input  += 8;
          output += 8;
          length -= 8;
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  CIPHER3DES::DES3Cipher_CBC
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/03/2013 23:17:57
//
//  @return       int :
//  @param        ctx :
//  @param        mode :
//  @param        length :
//  @param        iv[8] :
//  @param        input :
//  @param        output :
*/
/*-----------------------------------------------------------------*/
bool CIPHER3DES::DES3Cipher_CBC(CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
{
  XBYTE temp[8];

  if(length % 8) return false;

  if(mode == CIPHERDES_ENCRYPT)
    {
      while(length > 0)
        {
          for(int c=0; c<8; c++)
            {
              output[c] = (XBYTE)(input[c] ^ iv[c]);
            }

          DES3Cipher( ctx, output, output);

          memcpy( iv, output, 8 );

          input  += 8;
          output += 8;
          length -= 8;
        }
    }
   else /* CIPHERDES_DECRYPT */
    {
      while(length > 0)
        {
          memcpy(temp, input, 8);

          DES3Cipher( ctx, input, output );

          for(int c=0; c<8; c++ )
            {
              output[c] = (XBYTE)( output[c] ^ iv[c]);
            }

          memcpy( iv, temp, 8 );

          input  += 8;
          output += 8;
          length -= 8;
        }
    }

  return true;
}


