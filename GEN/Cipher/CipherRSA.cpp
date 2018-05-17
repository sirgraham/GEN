//------------------------------------------------------------------------------------------
//  CIPHERRSA.CPP
//
//  Cipher / Uncipher RSA
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 25/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "XFactory.h"
#include "XRand.h"
#include "XBER.h"

#include "Hash.h"

#include "CipherRSA.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

CIPHERRSA* CIPHERRSA::instance = NULL;

//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
// CIPHERKEYRSAPUBLIC::CIPHERKEYRSAPUBLIC
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:48:04
//

*/
/*-----------------------------------------------------------------*/
CIPHERKEYRSAPUBLIC::CIPHERKEYRSAPUBLIC() : CIPHERKEY()
{
  Clean();

  type = CIPHERKEYTYPE_PUBLIC;

  modulus.Ini();
  exponent.Ini();
}


/*-------------------------------------------------------------------
// CIPHERKEYRSAPUBLIC::~CIPHERKEYRSAPUBLIC
*/
/**
//  Destructor de la Clase CIPHERKEYRSAPUBLIC
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:48:27
//
*/
/*-----------------------------------------------------------------*/
 CIPHERKEYRSAPUBLIC::~CIPHERKEYRSAPUBLIC()
{
  modulus.End();
  exponent.End();

  Clean();
}



/*-------------------------------------------------------------------
// CIPHERKEYRSAPUBLIC::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:49:44
//
//  @return       bool :
//  @param        modulus :
//  @param        exponent :
*/
/*-----------------------------------------------------------------*/
bool CIPHERKEYRSAPUBLIC::Get(XMPINTEGER& modulus, XMPINTEGER& exponent)
{
  modulus.CopyFrom(&this->modulus);
  exponent.CopyFrom(&this->exponent);

  return true;
}




/*-------------------------------------------------------------------
//  CIPHERKEYRSAPUBLIC::Set
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2014 10:00:01
//
//  @return       bool :
//
//  @param        modulus :
//  @param        exponent :
*/
/*-----------------------------------------------------------------*/
bool CIPHERKEYRSAPUBLIC::Set(XMPINTEGER& modulus, XMPINTEGER& exponent)
{
  this->modulus.CopyFrom(&modulus);
  this->exponent.CopyFrom(&exponent);

  return true;
}






/*-------------------------------------------------------------------
//  CIPHERKEYRSAPUBLIC::Check
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 9:52:43
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool CIPHERKEYRSAPUBLIC::Check()
{
  if(!modulus.GetLimbs()              || !exponent.GetLimbs())                      return false;
  if((modulus.GetLimbs()[0] & 1) == 0 || (exponent.GetLimbs()[0] & 1 ) == 0)        return false;
  if((modulus.GetMSB() < 128)         || (modulus.GetMSB() >  XMPINTEGER_MAXBITS )) return false;
  if((exponent.GetMSB() < 2)          || (exponent.GetMSB() >  64 ))                return false;

  return true;
}





/*-------------------------------------------------------------------
//  CIPHERKEYRSAPRIVATE::CIPHERKEYRSAPRIVATE
*/
/**
//
//  Class Constructor CIPHERKEYRSAPRIVATE
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 9:14:42
//
*/
/*-----------------------------------------------------------------*/
CIPHERKEYRSAPRIVATE::CIPHERKEYRSAPRIVATE() : CIPHERKEY()
{
  Clean();

  type = CIPHERKEYTYPE_PRIVATE;

  prime1factor.Ini();
  prime2factor.Ini();
  exponent.Ini();
}



/*-------------------------------------------------------------------
//  CIPHERKEYRSAPRIVATE::~CIPHERKEYRSAPRIVATE
*/
/**
//
//  Class Destructor CIPHERKEYRSAPRIVATE
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 9:33:45
//
*/
/*-----------------------------------------------------------------*/
 CIPHERKEYRSAPRIVATE::~CIPHERKEYRSAPRIVATE()
{
  prime1factor.End();
  prime2factor.End();
  exponent.End();

  Clean();
}



/*-------------------------------------------------------------------
//  CIPHERKEYRSAPRIVATE::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 9:33:12
//
//  @return       bool :
//
//  @param        prime1factor :
//  @param        prime2factor :
//  @param        exponent :
*/
/*-----------------------------------------------------------------*/
bool CIPHERKEYRSAPRIVATE::Get(XMPINTEGER& prime1factor, XMPINTEGER& prime2factor, XMPINTEGER& exponent)
{
  prime1factor.CopyFrom(&this->prime1factor);
  prime2factor.CopyFrom(&this->prime2factor);
  exponent.CopyFrom(&this->exponent);

  return true;
}



/*-------------------------------------------------------------------
//  CIPHERKEYRSAPRIVATE::Set
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 9:33:00
//
//  @return       bool :
//
//  @param        prime1factor :
//  @param        prime2factor :
//  @param        exponent :
*/
/*-----------------------------------------------------------------*/
bool CIPHERKEYRSAPRIVATE::Set(XMPINTEGER& prime1factor, XMPINTEGER& prime2factor, XMPINTEGER& exponent)
{
  this->prime1factor.CopyFrom(&prime1factor);
  this->prime2factor.CopyFrom(&prime2factor);
  this->exponent.CopyFrom(&exponent);

  return true;
}



/*-------------------------------------------------------------------
//  CIPHERKEYRSAPRIVATE::Check
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 10:46:52
//
//  @return       bool :
//
//  @param        publickey :
*/
/*-----------------------------------------------------------------*/
bool CIPHERKEYRSAPRIVATE::Check(CIPHERKEYRSAPUBLIC& publickey)
{
  XMPINTEGER PQ;
  XMPINTEGER DE;
  XMPINTEGER P1;
  XMPINTEGER Q1;
  XMPINTEGER H;
  XMPINTEGER I;
  XMPINTEGER G;
  XMPINTEGER G2;
  XMPINTEGER L1;
  XMPINTEGER L2;
  XMPINTEGER publicmodulus;
  XMPINTEGER publicexponent;

  if(!publickey.Check())  return false;

  if((!prime1factor.GetLimbs()) || (!prime2factor.GetLimbs()) || (!exponent.GetLimbs()))  return false;

  publickey.Get(publicmodulus, publicexponent);

  PQ.Ini();
  DE.Ini();
  P1.Ini();
  Q1.Ini();
  H.Ini();
  I.Ini();
  G.Ini();
  G2.Ini();
  L1.Ini();
  L2.Ini();

  if(!PQ.Multiplication(&prime1factor, &prime2factor))  return false;
  if(!DE.Multiplication(&exponent, &publicexponent))    return false;
  if(!P1.SubtractionSigned(&prime1factor, 1))           return false;
  if(!Q1.SubtractionSigned(&prime2factor, 1))           return false;
  if(!H.Multiplication(&P1, &Q1))                       return false;
  if(!G.GreatestCommonDivisor(&G, &publicexponent, &H)) return false;
  if(!G2.GreatestCommonDivisor(&G2, &P1, &Q1))          return false;
  if(!L1.Division(&L1, &L2, &H, &G2 ))                  return false;
  if(!I.Module(&I, &DE, &L1))                           return false;

  // Check for a valid PKCS1v2 private key
  if((PQ.CompareSignedValues(publicmodulus) != 0) ||
     (L2.CompareSignedValues(0)             != 0) ||
     (I.CompareSignedValues(1)              != 0) ||
     (G.CompareSignedValues(1)              != 0)) return false;

  PQ.End();
  DE.End();
  P1.End();
  Q1.End();
  H.End();
  I.End();
  G.End();
  G2.End();
  L1.End();
  L2.End();

  return true;
}








//-------------------------------------------------------------------
//  CIPHERRSA::CIPHERRSA
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2006 19:42:40
//
//  @return
//  */
//-------------------------------------------------------------------
CIPHERRSA::CIPHERRSA() : CIPHER()
{
  Clean();

  xrand = xfactory->CreateRand();

  CIPHERRSA::instance = this;
}



//-------------------------------------------------------------------
//  CIPHERRSA::~CIPHERRSA
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2006 19:42:37
//
//  @return
//  */
//-------------------------------------------------------------------
CIPHERRSA::~CIPHERRSA()
{
  xfactory->DeleteRand(xrand);

  Clean();
}



/*-------------------------------------------------------------------
//  CIPHERRSA::SetKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2014 13:49:26
//
//  @return       bool :
//
//  @param        key :
//  @param        integritycheck :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::SetKey(CIPHERKEY* key, bool integritycheck)
{
  if(!key) return false;

  switch(key->GetType())
    {
      case CIPHERKEYTYPE_SYMMETRICAL  : return false;

      case CIPHERKEYTYPE_PUBLIC       : { CIPHERKEYRSAPUBLIC* publickey = (CIPHERKEYRSAPUBLIC*)key;

                                          publickey->Get(context.N, context.E); // Assign key public to context

                                          if(integritycheck)
                                            {
                                              if(!CheckContextPublicKey()) return false;
                                            }
                                        }
                                        break;

      case CIPHERKEYTYPE_PRIVATE      : { CIPHERKEYRSAPRIVATE* privatekey = (CIPHERKEYRSAPRIVATE*)key;
                                          XMPINTEGER           P1;
                                          XMPINTEGER           Q1;

                                          privatekey->Get(context.P, context.Q, context.D); // Assign key public to context

                                          if(!P1.SubtractionSigned(&context.P, 1))                  return false;
                                          if(!Q1.SubtractionSigned(&context.Q, 1))                  return false;
                                          if(!context.DP.Module(&context.DP, &context.D, &P1))      return false;
                                          if(!context.DQ.Module(&context.DQ, &context.D, &Q1))      return false;
                                          if(!context.QP.ModularInverse(&context.Q, &context.P))    return false;

                                          if(integritycheck)
                                            {
                                              if(!CheckContextPrivateKey()) return false;
                                            }

                                        }
                                        break;
    }

  return CIPHER::SetKey(key);
}




/*-------------------------------------------------------------------
//  CIPHERRSA::Cipher
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 16:00:26
//
//  @return       bool :
//
//  @param        input :
//  @param        size :
//  @param        keytouse :
//  @param        pkcs1version :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::Cipher(XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
{
  if(!size) return false;

  int keysize   = GetKeySizeInBytes(keytouse);
  if(!keysize) return false;

  XBUFFER _input;
  XBUFFER output;
  XBUFFER inputparcial;
  int     sizeblock = (keysize-3);
  int     rest      = (size%sizeblock);
  int     nblocks   = (size/sizeblock);
  bool    status    = false;

  _input.Add(input, size);

  result->Delete();

  for(int c=0; c<(nblocks+(rest?1:0)); c++)
    {
      int dosizeblock = sizeblock;

      inputparcial.Delete();
      output.Delete();

      if(c==nblocks) dosizeblock = rest;

      inputparcial.Add(&_input.Get()[sizeblock*c], dosizeblock);

      switch(pkcs1version)
        {
          case CIPHERRSAPKCS1VERSIONV15   : status = Cipher_PKCS1_V15(inputparcial.Get(), dosizeblock,  output, keytouse, pkcs1version, GenerateRandom, NULL);
                                            result->Add(output);
                                            break;

          case CIPHERRSAPKCS1VERSIONV21   : break;
        }
    }


  return status;
}




/*-------------------------------------------------------------------
//  CIPHERRSA::Cipher
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 16:36:47
//
//  @return       bool :
//
//  @param        input :
//  @param        keytouse :
//  @param        pkcs1version :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::Cipher(XBUFFER& input, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
{
  return Cipher(input.Get(), input.GetSize(), keytouse, pkcs1version);
}



/*-------------------------------------------------------------------
//  CIPHERRSA::Uncipher
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 16:00:32
//
//  @return       bool :
//
//  @param        input :
//  @param        size :
//  @param        keytouse :
//  @param        pkcs1version :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::Uncipher(XBYTE* input,XDWORD size, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
{
  if(!size) return false;

  int keysize = GetKeySizeInBytes(keytouse);
  if(!keysize) return false;

  XBUFFER _input;
  XBUFFER output;
  XBUFFER inputparcial;
  int     sizeblock = keysize;
  int     rest      = (size%sizeblock);
  int     nblocks   = (size/sizeblock);
  bool    status    = false;

  _input.Add(input, size);

  result->Delete();

  for(int c=0; c<(nblocks+(rest?1:0)); c++)
    {
      int dosizeblock = sizeblock;

      inputparcial.Delete();
      output.Delete();

      if(c==nblocks) dosizeblock = rest;

      inputparcial.Add(&_input.Get()[sizeblock*c], dosizeblock);

      switch(pkcs1version)
        {
          case CIPHERRSAPKCS1VERSIONV15   : status = Uncipher_PKCS1_V15(inputparcial.Get(), dosizeblock,  output, keytouse, pkcs1version, GenerateRandom, NULL);
                                            result->Add(output);
                                            break;

          case CIPHERRSAPKCS1VERSIONV21   : break;
        }
    }


  return status;
}




/*-------------------------------------------------------------------
//  CIPHERRSA::Uncipher
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 16:06:03
//
//  @return       bool :
//
//  @param        input :
//  @param        keytouse :
//  @param        pkcs1version :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::Uncipher(XBUFFER& input, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
{
  return Uncipher(input.Get(), input.GetSize(), keytouse, pkcs1version);
}




/*-------------------------------------------------------------------
//  CIPHERRSA::Signed
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2014 10:20:19
//
//  @return       bool :
//
//  @param        input :
//  @param        size :
//  @param        keytouse :
//  @param        hash :
//  @param        pkcs1version :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::Sign(XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version)
{
  if(!hash) return false;

  XBER      beroui;
  XBER      bernull;
  XBER      berhash;
  XBER      berseq1;
  XBER      berseq2;
  XSTRING*  OUINoSignstring = hash->GetOUINoSign();
  XBUFFER   sign;

  if(!OUINoSignstring) return false;

  if(!OUINoSignstring->GetSize()) return false;

  beroui.SetOID(OUINoSignstring->Get());
  bernull.SetNULL();

  hash->Do((XBYTE*)input, size);

  berhash.SetOCTETSTRING((*hash->GetResult()));

  berseq1.Sequence_AddTo(beroui);
  berseq1.Sequence_AddTo(bernull);

  berseq1.SetType(berseq1.GetType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

  berseq2.Sequence_AddTo(berseq1);
  berseq2.Sequence_AddTo(berhash);
  berseq2.SetType(berseq2.GetType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

  berseq2.GetDump(sign);

  return Cipher(sign, CIPHERKEYTYPE_PRIVATE, pkcs1version);
}



/*-------------------------------------------------------------------
//  CIPHERRSA::Sign
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2014 10:20:13
//
//  @return       bool :
//
//  @param        input :
//  @param        keytouse :
//  @param        hash :
//  @param        pkcs1version :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::Sign(XBUFFER& input, CIPHERKEYTYPE keytouse, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version)
{
  return Sign(input.Get(), input.GetSize(), keytouse, hash, pkcs1version);
}




/*-------------------------------------------------------------------
//  CIPHERRSA::GenerateKeys
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 10:57:22
//
//  @return       bool :
//
//  @param        nbits :
//  @param        exponent :
//  @param        publickey :
//  @param        privatekey :
//  @param        funcrandom :
//  @param        paramrandom :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::GenerateKeys(XDWORD nbits, int exponent, CIPHERKEYRSAPUBLIC& publickey, CIPHERKEYRSAPRIVATE& privatekey, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  XMPINTEGER P1;
  XMPINTEGER Q1;
  XMPINTEGER H;
  XMPINTEGER G;
  bool       status;

  if(nbits<128 || exponent<3 )  return false;

  P1.Ini();
  Q1.Ini();
  H.Ini();
  G.Ini();

  // find primes P and Q with Q < P so that: GCD( E, (P-1)*(Q-1) ) == 1
  status = context.E.LeftSet(exponent);
  if(status)
    {
      do{ if(!context.P.GeneratePrime((nbits+1)>>1, 0, funcrandom?funcrandom:(XMPINTEGER_FUNCRANDOM)GenerateRandom, NULL)) return false;
          if(!context.Q.GeneratePrime((nbits+1)>>1, 0, funcrandom?funcrandom:(XMPINTEGER_FUNCRANDOM)GenerateRandom, NULL)) return false;

          if(context.P.CompareSignedValues(context.Q) < 0)  context.P.Swap(&context.Q);

          if(context.P.CompareSignedValues(context.Q) == 0) continue;

          if(!context.N.Multiplication(&context.P, &context.Q)) return false;

          if(context.N.GetMSB()!=nbits) continue;

          if(!P1.SubtractionSigned(&context.P, 1)) return false;
          if(!Q1.SubtractionSigned(&context.Q, 1)) return false;

          if(!H.Multiplication(&P1, &Q1)) return false;

          if(!G.GreatestCommonDivisor(&G, &context.E, &H)) return false;

        } while(G.CompareSignedValues(1) != 0);


      // D  = E^-1 mod ((P-1)*(Q-1))
      // DP = D mod (P - 1)
      // DQ = D mod (Q - 1)
      // QP = Q^-1 mod P

      if(!context.D.ModularInverse(&context.E, &H))           return false;
      if(!context.DP.Module(&context.DP, &context.D, &P1))    return false;
      if(!context.DP.Module(&context.DQ, &context.D, &Q1))    return false;
      if(!context.QP.ModularInverse(&context.Q, &context.P))  return false;

      context.len = (context.N.GetMSB() + 7) >> 3;
    }

  P1.End();
  Q1.End();
  H.End();
  G.End();

  if(!status)
    {
      context.End();
    }
   else
    {
      publickey.Set(context.N, context.E);
      privatekey.Set(context.P, context.Q, context.D);
    }

  return status;
}




/*-------------------------------------------------------------------
//  CIPHERRSA::CheckContextPublicKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 12:13:33
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::CheckContextPublicKey()
{
  if(!context.N.GetLimbs()      || !context.E.GetLimbs())                       return false;
  if((context.N.GetLimbs()[0] & 1) == 0 || (context.E.GetLimbs()[0] & 1 ) == 0) return false;
  if((context.N.GetMSB() < 128) || (context.N.GetMSB() >  XMPINTEGER_MAXBITS )) return false;
  if((context.E.GetMSB() < 2)   || (context.E.GetMSB() >  64 ))                 return false;

  return true;
}




/*-------------------------------------------------------------------
//  CIPHERRSA::CheckContextPrivateKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2014 12:13:50
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::CheckContextPrivateKey()
{
  XMPINTEGER PQ;
  XMPINTEGER DE;
  XMPINTEGER P1;
  XMPINTEGER Q1;
  XMPINTEGER H;
  XMPINTEGER I;
  XMPINTEGER G;
  XMPINTEGER G2;
  XMPINTEGER L1;
  XMPINTEGER L2;
  XMPINTEGER DP;
  XMPINTEGER DQ;
  XMPINTEGER QP;

  if(!CheckContextPublicKey()) return false;

  if((!context.P.GetLimbs()) || (!context.Q.GetLimbs()) || (!context.D.GetLimbs()))  return false;

  PQ.Ini();
  DE.Ini();
  P1.Ini();
  Q1.Ini();
  H.Ini();
  I.Ini();
  G.Ini();
  G2.Ini();
  L1.Ini();
  L2.Ini();
  DP.Ini();
  DQ.Ini();
  QP.Ini();

  if(!PQ.Multiplication(&context.P, &context.Q))    return false;
  if(!DE.Multiplication(&context.D, &context.E))    return false;
  if(!P1.SubtractionSigned(&context.P, 1))          return false;
  if(!Q1.SubtractionSigned(&context.Q, 1))          return false;
  if(!H.Multiplication(&P1, &Q1))                   return false;
  if(!G.GreatestCommonDivisor(&G, &context.E, &H))  return false;
  if(!G2.GreatestCommonDivisor(&G2, &P1, &Q1))      return false;
  if(!L1.Division(&L1, &L2, &H, &G2 ))              return false;
  if(!I.Module(&I, &DE, &L1))                       return false;
  if(!DP.Module( &DP, &context.D, &P1))             return false;
  if(!DQ.Module(&DQ, &context.D, &Q1))              return false;
  if(!QP.ModularInverse(&context.Q, &context.P))    return false;

  // Check for a valid PKCS1v2 private key

  if((PQ.CompareSignedValues(context.N ) != 0) || (DP.CompareSignedValues(context.DP) != 0) ||
     (DQ.CompareSignedValues(context.DQ) != 0) || (QP.CompareSignedValues(context.QP) != 0) ||
     (L2.CompareSignedValues(0)          != 0) || (I.CompareSignedValues(1)           != 0) ||
     (G.CompareSignedValues(1)           != 0)) return false;

  PQ.End();
  DE.End();
  P1.End();
  Q1.End();
  H.End();
  I.End();
  G.End();
  G2.End();
  L1.End();
  L2.End();
  DP.End();
  DQ.End();
  QP.End();

  return true;
}





/*-------------------------------------------------------------------
//  CIPHERRSA::Cipher_PKCS1_V15
*/
/**
//
//
//
//  @author
//  @version      17/03/2014 13:55:30
//
//  @return       bool :
//
//  @param        buffer :
//  @param        size :
//  @param        output :
//  @param        keytypetouse :
//  @param        pkcs1version :
//  @param        funcrandom :
//  @param        paramrandom :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::Cipher_PKCS1_V15(XBYTE* buffer, XDWORD size, XBUFFER& output, CIPHERKEYTYPE keytypetouse, CIPHERRSAPKCS1VERSION pkcs1version,  XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  XBUFFER input;
  int     nb_pad;
  bool    status;

  if(!funcrandom) return false;

  int keysize = GetKeySizeInBytes(keytypetouse);
  if(!keysize) return false;

  //if(keysize < (size + 11)) return false;

  nb_pad = keysize - 3 - size;

  input.Add((XBYTE)0);

  if(keytypetouse == CIPHERKEYTYPE_PUBLIC)
    {
      XBYTE data = 0;

      input.Add((XBYTE)CIPHERRSA_CIPHERDATA);

       while(nb_pad > 0)
        {
          if(funcrandom(&data, 1, paramrandom))
            {
              input.Add((XBYTE)data);
              nb_pad--;
            }
        }
    }
   else
    {
      input.Add((XBYTE)CIPHERRSA_SINGDATA);

       while(nb_pad-- > 0)
         {
           input.Add((XBYTE)0xFF);
         }
    }


  input.Add((XBYTE)0);
  input.Add(buffer,size);

  status = false;

  switch(keytypetouse)
    {
      case CIPHERKEYTYPE_PUBLIC :  status = DoRSAPublicOperation(input, output);                              break;
      case CIPHERKEYTYPE_PRIVATE:  status = DoRSAPrivateOperation(input, output, funcrandom, paramrandom);    break;
    }

  return status;
}




/*-------------------------------------------------------------------
//  CIPHERRSA::Uncipher_PKCS1_V15
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2014 12:04:57
//
//  @return       bool :
//
//  @param        buffer :
//  @param        size :
//  @param        output :
//  @param        keytypetouse :
//  @param        pkcs1version :
//  @param        funcrandom :
//  @param        paramrandom :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::Uncipher_PKCS1_V15(XBYTE* buffer, XDWORD size, XBUFFER& output, CIPHERKEYTYPE keytypetouse, CIPHERRSAPKCS1VERSION pkcs1version,  XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  XBUFFER   input;
  XBUFFER   _output;
  XBYTE     pad_chk;
  int       pad_count = 0;
  XBYTE     pad_done = 0;
  bool      status;

  if(!funcrandom) return false;

  int keysize = GetKeySizeInBytes(keytypetouse);
  if(!keysize) return false;

  if((keysize < 16) || (keysize > XMPINTEGER_MAXSIZE)) return false;

  input.Add(buffer, size);

  status = false;

  switch(keytypetouse)
    {
      case CIPHERKEYTYPE_PUBLIC :  status = DoRSAPublicOperation(input , _output);                            break;
      case CIPHERKEYTYPE_PRIVATE:  status = DoRSAPrivateOperation(input, _output, funcrandom, paramrandom);   break;
    }

  if(!status) return false;

  int index = 0;

  pad_chk  = 0;
  pad_chk |= _output.GetByte(index++);

  pad_chk |= (_output.GetByte(index++)) ^  ((keytypetouse == CIPHERKEYTYPE_PRIVATE)? CIPHERRSA_CIPHERDATA : CIPHERRSA_SINGDATA);

  for(XDWORD i=0; i<size-3; i++)
    {
      pad_done  |= (_output.GetByte(index+i) == 0);
      pad_count += (pad_done == 0);
    }

  index += pad_count;
  pad_chk |= _output.GetByte(index++); // Must be zero


  if(pad_chk) return false;

  output.Add(&_output.Get()[index], (size-index));

  return true;
}





/*-------------------------------------------------------------------
//  CIPHERRSA::DoRSAPublicOperation
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/03/2014 13:22:39
//
//  @return       bool :
//
//  @param        input :
//  @param        output :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::DoRSAPublicOperation(XBUFFER& input, XBUFFER& output)
{
  int keysize = GetKeySizeInBytes(CIPHERKEYTYPE_PUBLIC);
  if(!keysize) return false;

  XMPINTEGER  T;

  T.Ini();

  if(!T.SetFromXBuffer(input)) return false;

  if(T.CompareSignedValues(context.N)>=0) return false;

  if(!T.SlidingWindowExponentiation(&T, &context.E, &context.N, &context.RN)) return false;

  if(!T.GetToXBuffer(output, keysize)) return false;

  return true;
}



/*-------------------------------------------------------------------
//  CIPHERRSA::DoRSAPrivateOperation
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2014 9:58:14
//
//  @return       bool :
//
//  @param        input :
//  @param        output :
//  @param        funcrandom :
//  @param        paramrandom :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::DoRSAPrivateOperation(XBUFFER& input, XBUFFER& output, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  XMPINTEGER  T;
  XMPINTEGER  T1;
  XMPINTEGER  T2;

  #if !defined(CIPHERRSA_RSANOCRT)
  XMPINTEGER* Vi;
  XMPINTEGER* Vf;

  Vi = &context.Vi;
  Vf = &context.Vf;
  #endif

  T.Ini();
  T1.Ini();
  T2.Ini();

  if(!T.SetFromXBuffer(input)) return false;

  if(T.CompareSignedValues(context.N)>=0) return false;

  #if defined(CIPHERRSA_RSANOCRT)
    if(!T.SlidingWindowExponentiation(&T, &context.D, &context.N, &context.RN)) return false;
  #else
    if(funcrandom != NULL)
      {
        // Blinding   T = T * Vi mod N
        if(!PrepareBlinding(Vi, Vf, funcrandom, paramrandom)) return false;
        if(!T.Multiplication(&T, Vi ))                        return false;
        if(!T.Module( &T, &T, &context.N))                    return false;
      }

    // faster decryption using the CRT
    // T1 = input ^ dP mod P
    // T2 = input ^ dQ mod Q

    if(!T1.SlidingWindowExponentiation(&T, &context.DP, &context.P, &context.RP)) return false;
    if(!T2.SlidingWindowExponentiation(&T, &context.DQ, &context.Q, &context.RQ)) return false;

    // T = (T1 - T2) * (Q^-1 mod P) mod P
    if(!T.SubtractionSigned(&T1, &T2))      return false;
    if(!T1.Multiplication(&T, &context.QP)) return false;
    if(!T.Module(&T, &T1, &context.P))      return false;

    // T = T2 + T * Q
    if(!T1.Multiplication(&T, &context.Q )) return false;
    if(!T.AdditionSigned(&T2, &T1))         return false;

    if(funcrandom!= NULL)
      {
        // Unblind T = T * Vf mod N
        if(!T.Multiplication(&T, Vf))   return false;
        if(!T.Module(&T, &T, &context.N)) return false;
      }
  #endif

  int keysize = GetKeySizeInBytes(CIPHERKEYTYPE_PRIVATE);
  if(!keysize) return false;

  if(!T.GetToXBuffer(output, keysize)) return false;

  T.End();
  T1.End();
  T2.End();

  return true;
}



/*-------------------------------------------------------------------
//  CIPHERRSA::PrepareBlinding
*/
/**
//
//
//
//  @author
//  @version      18/03/2014 8:57:48
//
//  @return       bool :
//
//  @param        Vi :
//  @param        Vf :
//  @param        funcrandom :
//  @param        paramrandom :
*/
/*-----------------------------------------------------------------*/
#if !defined(CIPHERRSA_RSANOCRT)
bool CIPHERRSA::PrepareBlinding(XMPINTEGER* Vi, XMPINTEGER* Vf, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  int count = 0;

  if(context.Vf.GetLimbs()!= NULL)
    {
      // We already have blinding values, just update them by squaring
      if(!context.Vi.Multiplication(&context.Vi, &context.Vi))      return false;
      if(!context.Vi.Module(&context.Vi, &context.Vi, &context.N )) return false;
      if(!context.Vf.Multiplication(&context.Vf, &context.Vf))      return false;
      if(!context.Vf.Module(&context.Vf, &context.Vf, &context.N )) return false;
    }
   else
    {
      int keysize = GetKeySizeInBytes(CIPHERKEYTYPE_PRIVATE);
      if(!keysize) return false;

      // Unblinding value: Vf = random number, invertible mod N
      do{ if(count++ > 10) return false;

          if(!context.Vf.FillRandom(keysize-1, funcrandom, paramrandom))             return false;
          if(!context.Vi.GreatestCommonDivisor(&context.Vi, &context.Vf, &context.N )) return false;

        } while(context.Vi.CompareSignedValues(1)!=0);

      // Blinding value: Vi =  Vf^(-e) mod N
      if(!context.Vi.ModularInverse( &context.Vf, &context.N))                                       return false;
      if(!context.Vi.SlidingWindowExponentiation(&context.Vi, &context.E, &context.N, &context.RN )) return false;
    }

  if(Vi != &context.Vi)
    {
      if(!Vi->CopyFrom(&context.Vi)) return false;
      if(!Vf->CopyFrom(&context.Vf)) return false;
    }

  return true;
}
#endif




/*-------------------------------------------------------------------
//  CIPHERRSA::GetKeySizeInBytes
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2014 16:27:58
//
//  @return       int :
//
//  @param        keytouse :
*/
/*-----------------------------------------------------------------*/
int CIPHERRSA::GetKeySizeInBytes(CIPHERKEYTYPE keytouse)
{
  int keysize = 0;

  CIPHERKEY* key =  GetKey(CIPHERKEYTYPE_PRIVATE);
  if(!key) return 0;

  keysize = key->GetSizeInBytes();

  return keysize;
}




/*-------------------------------------------------------------------
//  CIPHERRSA::GenerateRandom
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/03/2014 18:03:07
//
//  @return       int :
//  @param        buffer :
//  @param        size :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
bool CIPHERRSA::GenerateRandom(XBYTE* buffer, XDWORD size, void* param)
{
  if(!GetInstance()) return false;

  XRAND* xrand = instance->GetXRand();
  if(!xrand) return false;

  if(param != NULL) param = NULL;

  for(XDWORD c=0; c<size; c++)
    {
      buffer[c] = xrand->Max(255);
      if(!buffer[c]) buffer[c]++;
    }

  return true;
}
