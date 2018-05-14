//------------------------------------------------------------------------------------------
//  COMPRESS_ZIP.CPP
//
//  Compression/Descompression LZW Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 28/10/2003
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include <zlib.h>

#include "XBuffer.h"

#include "CompressZIP.h"

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
COMPRESS_ZIP::COMPRESS_ZIP() : COMPRESSBASE()
{
  Clean();

  SetType(COMPRESSBASE_TYPE_ZIP);
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
COMPRESS_ZIP::~COMPRESS_ZIP()
{
  Clean();
}



//-------------------------------------------------------------------
//  COMPRESS_ZIP::Compress
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
bool COMPRESS_ZIP::Compress(XBYTE* source,int size,XBUFFER* xbuffer)
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
//  COMPRESS_ZIP::Decompress
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
bool COMPRESS_ZIP::Decompress(XBYTE* source,int size,XBUFFER* xbuffer)
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
//  COMPRESS_ZIP::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/10/2003 16:40:47
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void COMPRESS_ZIP::Clean()
{
  param1 = NULL;
}



/*-------------------------------------------------------------------
//  COMPRESS_ZIP::ZCompress
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
int COMPRESS_ZIP::ZCompress(XBYTE* target,XDWORD* targetsize,XBYTE* source,XDWORD sourcesize)
{
  z_stream stream;
  int      err;
  int      nExtraChunks;
  uInt     destlen;

  stream.next_in  = (Bytef*)source;
  stream.avail_in = (uInt)sourcesize;
  destlen         = (uInt)*targetsize;

  if ((uLong)destlen != (uLong)*targetsize) return Z_BUF_ERROR;

  stream.zalloc = (alloc_func)0;
  stream.zfree = (free_func)0;
  stream.opaque = (voidpf)0;

  err = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
  if (err != Z_OK) return err;

  nExtraChunks = 0;

  do{ stream.next_out = target;
      stream.avail_out = destlen;

      err = deflate(&stream, Z_FINISH);
      if(err==Z_STREAM_END) break;

      if (err != Z_OK)
        {
          deflateEnd(&stream);
          return err;
        }

      nExtraChunks += 1;

    }while(stream.avail_out == 0);

 *targetsize = stream.total_out;

 err = deflateEnd(&stream);
 if(err!= Z_OK) return err;

 return nExtraChunks ? Z_BUF_ERROR : Z_OK;
}



/*-------------------------------------------------------------------
//  COMPRESS_ZIP::ZDecompress
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
int COMPRESS_ZIP::ZDecompress(XBYTE* target,XDWORD* targetsize,XBYTE* source,XDWORD sourcesize)
{
  z_stream stream;
  int err;

  int nExtraChunks;
  uInt destlen;

  stream.next_in = (Bytef*)source;
  stream.avail_in = (uInt)sourcesize;

  /* Check for source > 64K on 16-bit machine: */
  if ((uLong)stream.avail_in != (uLong)sourcesize) return Z_BUF_ERROR;

  destlen = (uInt)*targetsize;
  if ((uLong)destlen != (uLong)*targetsize) return Z_BUF_ERROR;

  stream.zalloc = (alloc_func)0;
  stream.zfree = (free_func)0;

  err = inflateInit(&stream);
  if (err != Z_OK) return err;

  nExtraChunks = 0;

  do{ stream.next_out = target;
      stream.avail_out = destlen;
      err = inflate(&stream, Z_FINISH);
      if(err==Z_STREAM_END) break;

      if(err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0)) err = Z_DATA_ERROR;

      if(err!=Z_BUF_ERROR)
        {
          inflateEnd(&stream);
          return err;
        }

      nExtraChunks += 1;

    } while(stream.avail_out == 0);

  *targetsize = stream.total_out;

  err = inflateEnd(&stream);
  if(err!=Z_OK) return err;

  return nExtraChunks ? Z_BUF_ERROR : Z_OK;
}

