/*------------------------------------------------------------------------------------------
//  HASHSHA1.CPP
//
//  Hash SHA1 class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/03/2013 12:07:38
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <string.h>

#include "XBuffer.h"

#include "HashSHA1.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  HASHSHA1::HASHSHA1
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:43:10
//
//  @return
*/
/*-----------------------------------------------------------------*/
HASHSHA1::HASHSHA1() : HASH()
{
  Clean();

  type            = HASHTYPE_SHA1;
  OUInosignstring = __L("1.3.14.3.2.26");

  Reset();
}



/*-------------------------------------------------------------------
//  HASHSHA1::~HASHSHA1
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:09:01
//
//  @return
//  */
/*-----------------------------------------------------------------*/
HASHSHA1::~HASHSHA1()
{
  Clean();
}




/*-------------------------------------------------------------------
//  HASHSHA1::Do
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 16:28:42
//
//  @return       bool :
//  @param        input :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool HASHSHA1::Do(XBYTE* input, int size)
{
  XDWORD msgdigest[5];

  result->Delete();

  Input(input, size);

  Result(msgdigest);

  for(int c=0;c<5;c++)
    {
      result->Add((XDWORD)msgdigest[c]);
    }

  return true;
}



/*-------------------------------------------------------------------
//  HASHSHA1::GetDefaultSize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/04/2013 1:39:57
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int HASHSHA1::GetDefaultSize()
{
  return sizeof(XDWORD)*5;
}





/*-------------------------------------------------------------------
//  HASHSHA1::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:09:56
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void HASHSHA1::Clean()
{

}



/*-------------------------------------------------------------------
//  HASHSHA1::Reset
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 11:22:06
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void HASHSHA1::Reset()
{
  lengthlow     = 0;
  lengthhigh    = 0;
  msgblockindex = 0;

  H[0]          = 0x67452301;
  H[1]          = 0xEFCDAB89;
  H[2]          = 0x98BADCFE;
  H[3]          = 0x10325476;
  H[4]          = 0xC3D2E1F0;

  computed      = false;
  corrupted     = false;
}





/*-------------------------------------------------------------------
//  HASHSHA1::Input
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 16:29:07
//
//  @return       void :
//  @param        input :
//  @param        length :
*/
/*-----------------------------------------------------------------*/
void HASHSHA1::Input(XBYTE* input, XDWORD length)
{
  if(!length) return;

  if(computed || corrupted)
    {
      corrupted = true;
      return;
    }

  while(length-- && !corrupted)
    {
      if(msgblockindex < 64) msgblock[msgblockindex++] = (*input & 0xFF);

      lengthlow += 8;
      lengthlow &= 0xFFFFFFFF;                            // Force it to 32 bits

      if(lengthlow == 0)
        {
            lengthhigh++;
            lengthhigh &= 0xFFFFFFFF;                    // Force it to 32 bits

            if(lengthhigh == 0) corrupted = true;        // Message is too long
        }

       if(msgblockindex == 64) ProcessMessageBlock();

       input++;
    }
}



/*-------------------------------------------------------------------
//  HASHSHA1::ProcessMessageBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 11:28:44
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void HASHSHA1::ProcessMessageBlock()
{
    XDWORD  K[] = { 0x5A827999 ,
                    0x6ED9EBA1 ,
                    0x8F1BBCDC ,
                    0xCA62C1D6
                  };
    int     t;                          // Loop counter
    XDWORD  temp;                       // Temporary word value
    XDWORD  W[80];                      // Word sequence
    XDWORD  A;
    XDWORD  B;
    XDWORD  C;
    XDWORD  D;
    XDWORD  E;                          // Word buffers

    for(t=0;t<16;t++)
      {
        W[t]  = ((unsigned) msgblock[t * 4])     << 24;
        W[t] |= ((unsigned) msgblock[t * 4 + 1]) << 16;
        W[t] |= ((unsigned) msgblock[t * 4 + 2]) << 8;
        W[t] |= ((unsigned) msgblock[t * 4 + 3]);
      }

    for(t=16;t<80;t++)
      {
        W[t] = CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
      }

    A = H[0];
    B = H[1];
    C = H[2];
    D = H[3];
    E = H[4];

    for(t=0;t<20;t++)
      {
        temp  = CircularShift(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        temp &= 0xFFFFFFFF;
        E     = D;
        D     = C;
        C     = CircularShift(30,B);
        B     = A;
        A     = temp;
      }

    for(t=20;t<40;t++)
      {
        temp = CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        temp &= 0xFFFFFFFF;
        E     = D;
        D     = C;
        C     = CircularShift(30,B);
        B     = A;
        A     = temp;
      }

    for(t=40;t<60;t++)
      {
        temp  = CircularShift(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        temp &= 0xFFFFFFFF;
        E     = D;
        D     = C;
        C     = CircularShift(30,B);
        B     = A;
        A     = temp;
      }

    for(t=60;t<80;t++)
      {
        temp  = CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        temp &= 0xFFFFFFFF;
        E     = D;
        D     = C;
        C     = CircularShift(30,B);
        B     = A;
        A     = temp;
      }

    H[0] = (H[0] + A) & 0xFFFFFFFF;
    H[1] = (H[1] + B) & 0xFFFFFFFF;
    H[2] = (H[2] + C) & 0xFFFFFFFF;
    H[3] = (H[3] + D) & 0xFFFFFFFF;
    H[4] = (H[4] + E) & 0xFFFFFFFF;

    msgblockindex = 0;
}




/*-------------------------------------------------------------------
//  HASHSHA1::PadMessage
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 11:39:42
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void HASHSHA1::PadMessage()
{

  // Check to see if the current message block is too small to hold
  // the initial padding bits and length.  If so, we will pad the
  // block, process it, and then continue padding into a second block.

  if(msgblockindex > 55)
    {
      msgblock[msgblockindex++] = 0x80;

      while(msgblockindex < 64)
        {
          msgblock[msgblockindex++] = 0;
        }

      ProcessMessageBlock();

      while(msgblockindex < 56)
        {
          msgblock[msgblockindex++] = 0;
        }
    }
   else
    {
      msgblock[msgblockindex++] = 0x80;

      while(msgblockindex < 56)
        {
          msgblock[msgblockindex++] = 0;
        }

    }


  // Store the message length as the last 8 octets

  msgblock[56] = (lengthhigh >> 24) & 0xFF;
  msgblock[57] = (lengthhigh >> 16) & 0xFF;
  msgblock[58] = (lengthhigh >> 8) & 0xFF;
  msgblock[59] = (lengthhigh) & 0xFF;

  msgblock[60] = (lengthlow >> 24) & 0xFF;
  msgblock[61] = (lengthlow >> 16) & 0xFF;
  msgblock[62] = (lengthlow >> 8) & 0xFF;
  msgblock[63] = (lengthlow) & 0xFF;

  ProcessMessageBlock();
}



/*-------------------------------------------------------------------
//  HASHSHA1::CircularShift
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 11:28:14
//
//  @return       XDWORD :
//  @param        bits :
//  @param        word :
*/
/*-----------------------------------------------------------------*/
XDWORD HASHSHA1::CircularShift(int bits, XDWORD word)
{
    return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32-bits));
}



/*-------------------------------------------------------------------
//  HASHSHA1::Result
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 11:43:50
//
//  @return       bool :
//  @param        msgdigest :
*/
/*-----------------------------------------------------------------*/
bool HASHSHA1::Result(XDWORD* msgdigest)
{
  if(corrupted) return false;

  if(!computed)
    {
      PadMessage();
      computed = true;
    }

  for(int i=0; i<5; i++)
    {
       msgdigest[i] = H[i];
    }

  return true;
}




