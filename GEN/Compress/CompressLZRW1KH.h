//------------------------------------------------------------------------------------------
//  COMPRESSLZRW1KH.H
//
/**
// \class
//
//  Compress LZRW1/KH class
//
//  @author  Abraham J. Velez
//  @version 03/05/2004 10:25:39
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _COMPRESSLZRW1KH_H_
#define _COMPRESSLZRW1KH_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "CompressBase.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define CMPFLAG_COPIED                    0x80
#define CMPFLAG_COMPRESS                  0x40

#define CMPTARGET(operation,index,data)  target?(target[index]=(XBYTE)data):CompressResult((bool)operation,(XBYTE)data,(void*)param1)

//---- CLASS -------------------------------------------------------------------------------

class XBUFFER;

class COMPRESS_LZRW1KH : public COMPRESSBASE
{
  public:

                            COMPRESS_LZRW1KH      ();
    virtual                ~COMPRESS_LZRW1KH      ();

    bool                    Compress              (XBYTE* origin,XDWORD size,XBUFFER* buffer);
    bool                    Decompress            (XBYTE* origin,XDWORD size,XBUFFER* buffer);

    bool                    Compress              (XBYTE* origin,XDWORD size,void* param1=NULL);
    bool                    Decompress            (XBYTE* origin,XDWORD size,void* param1=NULL);

  private:

    XBYTE                   GetMatch              (XBYTE* source, XDWORD x, XDWORD sourcesize, XDWORD* hash, XDWORD* size, XDWORD* pos);
    XDWORD                  CompressionBuffer     (XBYTE* source, XBYTE* target, XDWORD sourcesize);
    XDWORD                  DecompressionBuffer   (XBYTE *source,XBYTE *target,XDWORD sourcesize);

    XDWORD                  hash[4096];
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

