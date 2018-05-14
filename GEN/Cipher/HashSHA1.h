/*------------------------------------------------------------------------------------------
//  HASHSHA1.H
*/
/**
// \class
//
//  Hash SHA1 Class
//
//  @author  Abraham J. Velez
//  @version 02/03/2013 12:02:40
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _HASHSHA1_H_
#define _HASHSHA1_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "Hash.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



/*---- CLASS -----------------------------------------------------------------------------*/


class HASHSHA1 : public HASH
{
  public:
                      HASHSHA1              ();
    virtual          ~HASHSHA1              ();

    using             HASH::Do;
    bool              Do                    (XBYTE* input, int size);

    int               GetDefaultSize        ();

  private:

     XDWORD           H[5];                     // Message digest buffers

     XDWORD           lengthlow;                // Message length in bits
     XDWORD           lengthhigh;               // Message length in bits

     XBYTE            msgblock[64];             // 512-bit message blocks
     int              msgblockindex;            // Index into message block array

     bool             computed;                 // Is the digest computed?
     bool             corrupted;                // Is the message digest corrupe

    void              Clean                 ();

    void              Reset                 ();

    void              Input                 (XBYTE* input, XDWORD length);

    void              ProcessMessageBlock   ();
    void              PadMessage            ();
    XDWORD            CircularShift         (int bits, XDWORD word);

    bool              Result                (XDWORD* msgdigest);
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

