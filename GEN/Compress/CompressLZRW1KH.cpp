//------------------------------------------------------------------------------------------
//  COMPRESSLZRW1KH.CPP
//
//  Compress LZRW1KH class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/05/2004 10:28:42
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"

#include "CompressLZRW1KH.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::COMPRESS_LZRW1KH
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:06:42
//
//  @return
//  */
//-------------------------------------------------------------------
COMPRESS_LZRW1KH::COMPRESS_LZRW1KH()
{
  SetType(COMPRESSBASE_TYPE_LZRW1KH);
}


//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::~COMPRESS_LZRW1KH
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:07:00
//
//  @return
//  */
//-------------------------------------------------------------------
COMPRESS_LZRW1KH::~COMPRESS_LZRW1KH()
{

}


//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::Compress
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:07:29
//
//  @return       bool :
//  @param        origin :
//  @param        size :
//  @param        buffer :
*/
//-------------------------------------------------------------------
bool COMPRESS_LZRW1KH::Compress(XBYTE* origin,int size,XBUFFER* buffer)
{
  param1  =  NULL;

  buffer->SetSize(CompressionBuffer(origin, buffer->Get(),(XDWORD)size));

  return true;
}


//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::Decompress
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:07:47
//
//  @return       bool :
//  @param        origin :
//  @param        size :
//  @param        buffer :
*/
//-------------------------------------------------------------------
bool COMPRESS_LZRW1KH::Decompress(XBYTE* origin,int size,XBUFFER* buffer)
{
  param1  =  NULL;

  buffer->SetSize(DecompressionBuffer(origin, buffer->Get(), (XDWORD)size));

  return true;
}


//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::Compress
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:08:02
//
//  @return       bool :
//  @param        origin :
//  @param        size :
//  @param        param1 :
*/
//-------------------------------------------------------------------
bool COMPRESS_LZRW1KH::Compress(XBYTE* origin,int size,void* param1)
{
  this->param1  =  param1;

  CompressionBuffer(origin, NULL, (XDWORD)size);

  return true;
}


//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::Decompress
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:08:26
//
//  @return       bool :
//  @param        origin :
//  @param        size :
//  @param        param1 :
*/
//-------------------------------------------------------------------
bool COMPRESS_LZRW1KH::Decompress(XBYTE* origin,int size,void* param1)
{
  this->param1  =  param1;

  DecompressionBuffer(origin, NULL, (XDWORD)size);

  return true;
}



//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::GetMatch
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:04:35
//
//  @return       XBYTE :
//  @param        source :
//  @param        x :
//  @param        sourcesize :
//  @param        hash :
//  @param        size :
//  @param        pos :
*/
//-------------------------------------------------------------------
XBYTE COMPRESS_LZRW1KH::GetMatch(XBYTE* source, XDWORD x, XDWORD sourcesize, XDWORD* hash, XDWORD* size, XDWORD* pos)
{
  XWORD hashvalue = (XWORD)((40543L*((((source[x] << 4) ^ source[x+1]) << 4) ^ source[x+2]) >> 4) & 0xfff);

  (*pos) = hash[hashvalue];

  hash[hashvalue] = x;

  if(((*pos)!=-1)&&((x-*pos) < 4096))
    {
      for ((*size) = 0; (((*size) < 18)&& (source[x+(*size)] == source[(*pos)+(*size)]) && ((x+*size) < sourcesize)); (*size)++)
        {

        }

      return ((*size) >= 3);
    }

  return 0;
}



//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::CompressionBuffer
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:04:59
//
//  @return       XWORD :
//  @param        source :
//  @param        target :
//  @param        sourcesize :
*/
//-------------------------------------------------------------------
XDWORD COMPRESS_LZRW1KH::CompressionBuffer(XBYTE* source, XBYTE* target, XDWORD sourcesize)
{
  XDWORD  key;
  XDWORD  size;
  XDWORD  pos;
  XBYTE   bit      = 0;
  XWORD   command  = 0;
  XDWORD  x        = 0;
  XDWORD  y        = 3;
  XDWORD  z        = 1;

  for(key = 0; key < 4096; key++)
    {
      hash[key] = -1;
    }

  CMPTARGET(true,0,CMPFLAG_COMPRESS);

  for(;(x<sourcesize) && (y<=sourcesize);)
    {
      if(bit>15)
        {

          CMPTARGET(true,z++ ,(XBYTE)((command >> 8) & 0x00ff));
          CMPTARGET(true,z   ,(XBYTE)(command & 0x00ff));

          z = y;
          bit = 0;
          y += 2;
        }

      for(size=1; (source[x] == source[x+size]) && (size < 0x0fff) && (x+size < sourcesize); size++)
        {

        }

      if(size >= 16)
        {
          CMPTARGET(true,y++,0);
          CMPTARGET(true,y++,(XBYTE)(((size - 16) >> 8) & 0x00ff));
          CMPTARGET(true,y++,(XBYTE)((size - 16) & 0x00ff));
          CMPTARGET(true,y++,source[x]);

          x=(x+size);
          command = (XWORD)((command << 1) + 1);
        }
       else
        {
          if(GetMatch(source,x,sourcesize,hash,&size,(XDWORD*)&pos))
            {
              key = (((x-pos) << 4) + (size - 3));

              CMPTARGET(true,y++,(XBYTE)((key >> 8) & 0x00ff));
              CMPTARGET(true,y++,(XBYTE)(key & 0x00ff));

              x = (x+size);
              command = (XWORD)((command << 1) + 1);
            }
           else
            {

              CMPTARGET(true,y++,source[x++]);

              command = (XWORD)((command << 1));
            }
        }

      bit++;
   }

  command <<= (16-bit);

  CMPTARGET(true,z++,(XBYTE)((command >> 8) & 0x00ff));
  CMPTARGET(true,z   ,(XBYTE)(command & 0x00ff));

  if(y > sourcesize)
    {
      for(y=0; y<sourcesize; CMPTARGET(true,y+1,source[y++]) ) // #Imanol order of execution not guaranteed, y could have weird values
        {

        }

      CMPTARGET(true,0,CMPFLAG_COPIED);

      return (sourcesize+1);
    }

  return y;
}



//-------------------------------------------------------------------
//  COMPRESS_LZRW1KH::DecompressionBuffer
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 11:04:52
//
//  @return       XWORD :
//  @param        *source :
//  @param        *target :
//  @param        sourcesize :
*/
//-------------------------------------------------------------------*
XDWORD COMPRESS_LZRW1KH::DecompressionBuffer(XBYTE *source,XBYTE *target,XDWORD sourcesize)
{
  XDWORD    x       = 3;
  XDWORD    y       = 0;
  XDWORD    pos;
  XDWORD    size;
  XDWORD     k;
  XDWORD    command = ((source[1] << 8) + source[2]);
  XBYTE     bit     = 16;

  if(source[0]==CMPFLAG_COPIED)
    {
      for(y=1;y<sourcesize;CMPTARGET(false,y-1,source[y++]) )
        {

        }

      return (sourcesize-1);
    }

  for(;x<sourcesize;)
    {
      if(bit == 0)
        {
          command  = ((source[x++] << 8));
          command  = (command + source[x++]);
          bit = 16;
        }

      if(command&0x8000)
        {
          pos = (source[x++] << 4);
          pos = (pos + (source[x] >> 4));

          if(pos)
            {
              size = ((source[x++] & 0x0f)+3);

              for(k=0;k<size;k++)
                {
                  CMPTARGET(false,y+k,target[y-pos+k]);
                }

              y = (y+size);
            }
           else
            {
              size = (source[x++] << 8);
              size = (size + source[x++] + 16);

              for (k=0;k<size;CMPTARGET(false,y+k++,source[x]))
                {

                }

              x++;
              y= (y+size);
            }

        }
       else
        {
          CMPTARGET(false,y++,source[x++]);
        }

       command <<= 1;
       bit--;
    }

 return y;
}
