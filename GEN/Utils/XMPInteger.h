/*------------------------------------------------------------------------------------------
//  XMPINTEGER.H
*/
/**
// \class
//
//  eXtended Multi-Precision Integer class
//
//  @author  Abraham J. Velez
//  @version 25/02/2014 14:20:43
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XMPINTEGER_H_
#define _XMPINTEGER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"
#include "XBuffer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

typedef XDWORD XLIMB;
typedef XQWORD XUDBL;

typedef bool (*XMPINTEGER_FUNCRANDOM)   (XBYTE* buffer, XDWORD size, void* param);


#define XMPINTEGER_MAXLIMBS           10000
#define XMPINTEGER_WINDOWSIZE         6

#define XMPINTEGER_MAXSIZE            512                                 // Maximum number of bytes for usable MPIs.
#define XMPINTEGER_MAXBITS            (8*XMPINTEGER_MAXSIZE)

#define XMPINTEGER_CHARSINLIMB        sizeof(XLIMB)                       // chars in limb
#define XMPINTEGER_BITSINLIMB         (XMPINTEGER_CHARSINLIMB << 3)       // bits  in limb
#define XMPINTEGER_HALFLIMBSIZE       (XMPINTEGER_CHARSINLIMB << 2)       // half limb size

#define XMPINTEGER_BITSTOLIMBS(i)     (((i) + XMPINTEGER_BITSINLIMB  - 1) / XMPINTEGER_BITSINLIMB)
#define XMPINTEGER_CHARSTOLIMBS(i)    (((i) + XMPINTEGER_CHARSINLIMB - 1) / XMPINTEGER_CHARSINLIMB)


#define XMPINTEGER_MULADDC_INIT       \
                                      XLIMB s0, s1, b0, b1;                                               \
                                      XLIMB r0, r1, rx, ry;                                               \
                                      b0 = ( b << XMPINTEGER_HALFLIMBSIZE ) >> XMPINTEGER_HALFLIMBSIZE;   \
                                      b1 = ( b >> XMPINTEGER_HALFLIMBSIZE );


#define XMPINTEGER_MULADDC_CORE       \
                                      s0 = ( *s << XMPINTEGER_HALFLIMBSIZE ) >> XMPINTEGER_HALFLIMBSIZE;  \
                                      s1 = ( *s >> XMPINTEGER_HALFLIMBSIZE ); s++;                        \
                                      rx = s0 * b1; r0 = s0 * b0;                                         \
                                      ry = s1 * b0; r1 = s1 * b1;                                         \
                                      r1 += ( rx >> XMPINTEGER_HALFLIMBSIZE );                            \
                                      r1 += ( ry >> XMPINTEGER_HALFLIMBSIZE );                            \
                                      rx <<= XMPINTEGER_HALFLIMBSIZE; ry <<= XMPINTEGER_HALFLIMBSIZE;     \
                                      r0 += rx; r1 += (r0 < rx);                                          \
                                      r0 += ry; r1 += (r0 < ry);                                          \
                                      r0 +=  c; r1 += (r0 <  c);                                          \
                                      r0 += *d; r1 += (r0 < *d);                                          \
                                      c = r1; *(d++) = r0;

#define XMPINTEGER_MULADDC_STOP       \






/*---- CLASS -----------------------------------------------------------------------------*/


class XMPINTEGER
{
  public:
                                XMPINTEGER                    ()                        { Clean();                  }
    virtual                    ~XMPINTEGER                    ()
                                {
                                  End();
                                  Clean();
                                }


    int                         GetSign                       ()                        { return sign;              }
    XDWORD                      GetNLimbs                     ()                        { return nlimbs;            }
    XLIMB*                      GetLimbs                      ()                        { return limbs;             }

    void                        SetSign                       (int sign)                { this->sign   = sign;      }
    void                        SetNLimbs                     (XDWORD nlimbs)           { this->nlimbs = nlimbs;    }
    void                        SetLimbs                      (XLIMB* limbs)            { this->limbs = limbs;      }


    bool                        Ini                           ();

    bool                        Grow                          (XDWORD nblimbs);
    bool                        Shrink                        (XDWORD nblimbs);
    bool                        CopyFrom                      (XMPINTEGER* xmpinteger);
    void                        Swap                          (XMPINTEGER* xmpinteger);

    bool                        LeftSet                       (int z);

    int                         GetBit                        (XDWORD position);
    bool                        SetBit                        (XDWORD position, XBYTE value);

    int                         GetLSB                        ();
    int                         GetMSB                        ();

    XDWORD                      GetSize                       ();

    int                         ImportFromBinary              (XBYTE* buffer, XDWORD size);
    int                         ExportToBinary                (XBYTE* buffer, XDWORD size);

    bool                        LeftShift                     (XDWORD count);
    bool                        RightShift                    (XDWORD count);

    int                         CompareABSValues              (XMPINTEGER& xmpinteger);
    int                         CompareSignedValues           (XMPINTEGER& xmpinteger);
    int                         CompareSignedValues           (int integer);

    bool                        AdditionABS                   (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);
    bool                        SubtractionABS                (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);

    bool                        AdditionSigned                (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);
    bool                        SubtractionSigned             (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);

    bool                        AdditionSigned                (XMPINTEGER* xmpinteger, int integer);
    bool                        SubtractionSigned             (XMPINTEGER* xmpinteger, int integer);

    bool                        Multiplication                (XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2);
    bool                        Multiplication                (XMPINTEGER* xmpinteger, int integer);

    bool                        Division                      (XMPINTEGER* xmpintegerQ, XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, XMPINTEGER* xmpintegerB);
    bool                        Division                      (XMPINTEGER* xmpintegerQ, XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, int integerB);

    bool                        Module                        (XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, XMPINTEGER* xmpintegerB);
    bool                        Module                        (XLIMB* r, XMPINTEGER* xmpintegerA, int integerb);

    void                        MontgomeryIni                 (XLIMB *mm, XMPINTEGER* N);
    void                        MontgomeryMultiplication      (XMPINTEGER* A, XMPINTEGER* B, XMPINTEGER* N, XLIMB mm, XMPINTEGER* T);
    void                        MontgomeryReduction           (XMPINTEGER* A, XMPINTEGER* N, XLIMB mm, XMPINTEGER* T);
    bool                        SlidingWindowExponentiation   (XMPINTEGER* A, XMPINTEGER* E, XMPINTEGER* N, XMPINTEGER* _RR);

    bool                        GreatestCommonDivisor         (XMPINTEGER* G, XMPINTEGER* A, XMPINTEGER* B);

    int                         FillRandom                    (XDWORD size, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandon);

    int                         ModularInverse                (XMPINTEGER* A, XMPINTEGER* N);

    int                         CheckSmallFactors             ();
    bool                        MillerRabin                   (XMPINTEGER* X, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);

    int                         IsPrime                       (XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);
    bool                        GeneratePrime                 (int nbits, int dhflag, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);

    bool                        SetFromString                 (int radix, XCHAR* string);
    bool                        SetFromString                 (int radix, XSTRING& string);
    bool                        GetToString                   (int radix, XSTRING& string);

    bool                        SetFromXBuffer                (XBUFFER& xbuffer);
    bool                        GetToXBuffer                  (XBUFFER& xbuffer, XDWORD size);

    bool                        End                           ();

  private:

    void                        Clean                         ()
                                {
                                  sign    = 0;
                                  nlimbs  = 0;
                                  limbs   = NULL;
                                }

    void                        SubtractionHelper             (XDWORD n, XLIMB *s, XLIMB *d);
    void                        MultiplicationHelper          (XDWORD i, XLIMB* s, XLIMB* d, XLIMB b);

    bool                        GetDigit                      (XDWORD* data, int radix, XCHAR character);
    bool                        SetToStringHelper             (int radix, XSTRING& string);

    int                         sign;           // Integer sign
    XDWORD                      nlimbs;         // Number of limbs;
    XLIMB*                      limbs;          // Pointer to limbs
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif






