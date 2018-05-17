//------------------------------------------------------------------------------------------
//  COMPRESS_ZIP.H
//
/**
// \class
//
//  Compression/Descompression LZW Class
//
//  @author  Abraham J. Velez
//  @version 28/10/2003
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _COMPRESS_ZIP_H_
#define _COMPRESS_ZIP_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "CompressBase.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XBUFFER;

class COMPRESS_ZIP : public COMPRESSBASE
{
  public:
                            COMPRESS_ZIP          ();
    virtual                ~COMPRESS_ZIP          ();

    bool                    Compress              (XBYTE* source,XDWORD size,XBUFFER* buffer);
    bool                    Decompress            (XBYTE* source,XDWORD size,XBUFFER* buffer);

  private:

    void                    Clean                 ();

    int                     ZCompress             (XBYTE* target,XDWORD* targetsize,XBYTE* source,XDWORD sourcesize);
    int                     ZDecompress           (XBYTE* target,XDWORD* targetsize,XBYTE* source,XDWORD sourcesize);

};

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

