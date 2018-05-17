//------------------------------------------------------------------------------------------
//  COMPRESS_GZ.H
//
/**
// \class
//
//  Compression/Descompression LZW Class
//
//  @author  Diego Martinez
//  @version 22/06/2016
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _COMPRESS_GZ_H_
#define _COMPRESS_GZ_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "CompressBase.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XBUFFER;

class COMPRESS_GZ : public COMPRESSBASE
{
  public:

    static const int CHUNK          =16384;
    static const int windowBits   =15;
    static const int GZIP_ENCODING  =16;

                            COMPRESS_GZ           ();
    virtual                ~COMPRESS_GZ           ();

    bool                    Compress              (XBYTE* source,XDWORD size,XBUFFER* buffer);
    bool                    Decompress            (XBYTE* source,XDWORD size,XBUFFER* buffer);

  private:

    void                    Clean                 ();

    int                     ZCompress             (XBYTE* target,XDWORD* targetsize,XBYTE* source,XDWORD sourcesize);
    int                     ZDecompress           (XBYTE* target,XDWORD* targetsize,XBYTE* source,XDWORD sourcesize);

};

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif
