//------------------------------------------------------------------------------------------
//  COMPRESS_GZ.CPP
//
//  Compression/Descompression LZW Class
//
//  Author            : Diego Martínez Ruiz de Gaona
//  Date Of Creation  : 28/10/2016
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include <zlib.h>

#include "XBuffer.h"

#include "CompressGZ.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  COMPRESS_ZIP::COMPRESS_ZIP
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/08/2001 9:32:45
//
//  @return
//  */
//-------------------------------------------------------------------
COMPRESS_GZ::COMPRESS_GZ() : COMPRESSBASE()
{
  Clean();

  SetType(COMPRESSBASE_TYPE_GZ);
}



//-------------------------------------------------------------------
//  COMPRESS_ZIP::~COMPRESS_ZIP
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/08/2001 9:33:11
//
//  @return
//  @param
*/
//-------------------------------------------------------------------
COMPRESS_GZ::~COMPRESS_GZ()
{
  Clean();
}



//-------------------------------------------------------------------
//  COMPRESS_GZ::Compress
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2004 18:05:17
//
//  @return       bool :
//  @param        source :
//  @param        size :
//  @param        xbuffer :
*/
//-------------------------------------------------------------------
bool COMPRESS_GZ::Compress(XBYTE* source,XDWORD size,XBUFFER* xbuffer)
{

  if(!source)  return false;
  if(!size)    return false;
  if(!xbuffer) return false;

  xbuffer->Delete();
  xbuffer->Resize(size);

  XDWORD csize = size;

  ZCompress(xbuffer->Get(), &csize, source, size);

  xbuffer->Resize(csize);

  return true;
}


//-------------------------------------------------------------------
//  COMPRESS_GZ::Decompress
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2004 18:05:21
//
//  @return       bool :
//  @param        source :
//  @param        size :
//  @param        xbuffer :
*/
//-------------------------------------------------------------------
bool COMPRESS_GZ::Decompress(XBYTE* source,XDWORD size,XBUFFER* xbuffer)
{

  if(!source)  return false;
  if(!size)    return false;
  if(!xbuffer) return false;

  //xbuffer->Delete();
  //xbuffer->Resize(xbuffer->size);

  XDWORD csize = xbuffer->GetSize();

  ZDecompress(xbuffer->Get(), &csize, source, size);

  //xbuffer->Resize(csize);

  return true;
}


//-------------------------------------------------------------------
//  COMPRESS_GZ::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/10/2003 16:40:47
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void COMPRESS_GZ::Clean()
{
  param1 = NULL;
}



/*-------------------------------------------------------------------
//  COMPRESS_GZ::ZCompress
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2009 06:41:35 p.m.
//
//  @return       int :
//  @param        target :
//  @param        targetsize :
//  @param        source :
//  @param        sourcesize :
*/
/*-----------------------------------------------------------------*/
int COMPRESS_GZ::ZCompress(XBYTE* target,XDWORD* targetsize,XBYTE* source,XDWORD sourcesize)
{
  int      level=-1;

  z_stream  stream;
            stream.zalloc = Z_NULL;
            stream.zfree  = Z_NULL;
            stream.opaque = Z_NULL;

if (deflateInit2(&stream, level, Z_DEFLATED, windowBits | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    return Z_BUF_ERROR;

            stream.next_in  = (Bytef*)source;
            stream.avail_in = (uInt)sourcesize;
            int destlen     = (uInt)*targetsize;

    do
    {
      int have;
      stream.avail_out = destlen;
      stream.next_out = target;
      if (deflate(&stream, Z_FINISH) == Z_STREAM_ERROR)
          return Z_STREAM_ERROR;

      have = destlen - stream.avail_out;
    }
    while (stream.avail_out == 0);

      *targetsize = stream.total_out;

    if (deflateEnd(&stream) != Z_OK)
      return Z_BUF_ERROR;


return  Z_OK;
}



/*-------------------------------------------------------------------
//  COMPRESS_GZ::ZDecompress
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2009 06:41:43 p.m.
//
//  @return       int :
//  @param        target :
//  @param        targetsize :
//  @param        source :
//  @param        sourcesize :
*/
/*-----------------------------------------------------------------*/
int COMPRESS_GZ::ZDecompress(XBYTE* target,XDWORD* targetsize,XBYTE* source,XDWORD sourcesize)
{
  z_stream stream;
  int err;

  int ret;
  unsigned have;

  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  stream.avail_in = 0;
  stream.next_in = Z_NULL;

  if (inflateInit2(&stream, 16 + MAX_WBITS) != Z_OK)
    return Z_DATA_ERROR;

  stream.avail_in = sourcesize;

   if ((uLong)stream.avail_in != (uLong)sourcesize) return Z_BUF_ERROR;


  stream.next_in =  (unsigned char*)source;
  do {
    stream.avail_out = *targetsize;
    stream.next_out = (XBYTE*)target;
    ret = inflate(&stream, Z_NO_FLUSH);

    switch (ret)
    {
    case Z_ERRNO        :
    case Z_STREAM_ERROR :
    case Z_BUF_ERROR    :
    case Z_VERSION_ERROR:
    case Z_NEED_DICT:
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
      inflateEnd(&stream);
      return false;
    }
    have = CHUNK - stream.avail_out;

  } while (stream.avail_out == 0);

  if (inflateEnd(&stream) != Z_OK) {
    return Z_BUF_ERROR;
  }

  *targetsize = stream.total_out;

  err = inflateEnd(&stream);
  if(err!=Z_OK)
    return err;

    return Z_OK;
}

