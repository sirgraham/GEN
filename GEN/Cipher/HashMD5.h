/*------------------------------------------------------------------------------------------
//  HASHMD5.H
*/
/**
// \class
//
//  Hash MD5 Class
//
//  @author  Abraham J. Velez
//  @version 02/03/2013 12:02:40
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _HASHMD5_H_
#define _HASHMD5_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "Hash.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define HASHMD5_BLOCKSIZE  64


/*---- CLASS -----------------------------------------------------------------------------*/


class HASHMD5 : public HASH
{
  public:
                      HASHMD5               ();
    virtual          ~HASHMD5               ();

    using             HASH::Do;
    bool              Do                    (XBYTE* input, XDWORD size);

    int               GetDefaultSize        ();

  private:

    bool              finalized;
    XBYTE             buffer[HASHMD5_BLOCKSIZE];
    XDWORD            count[2];
    XDWORD            state[4];
    XBYTE             digest[16];

    void              Clean                 ();

    void              Ini                   ();

    void              Update                (XBYTE* input, XDWORD size);
    void              Transform             (XBYTE block[HASHMD5_BLOCKSIZE]);

    void              End                   ();

    void              Decode                (XDWORD* output, XBYTE* input, XDWORD size);
    void              Encode                (XBYTE* output, XDWORD* input, XDWORD size);

    inline XDWORD     F                     (XDWORD x, XDWORD y, XDWORD z);
    inline XDWORD     G                     (XDWORD x, XDWORD y, XDWORD z);
    inline XDWORD     H                     (XDWORD x, XDWORD y, XDWORD z);
    inline XDWORD     I                     (XDWORD x, XDWORD y, XDWORD z);
    inline XDWORD     RotateLeft            (XDWORD x, int n);
    inline void       FF                    (XDWORD &a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac);
    inline void       GG                    (XDWORD &a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac);
    inline void       HH                    (XDWORD &a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac);
    inline void       II                    (XDWORD &a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac);
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

