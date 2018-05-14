//------------------------------------------------------------------------------------------
//  COMPRESSBASE.CPP
//
//  Compression/Descompression Class
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
#include <string.h>

#include "XBuffer.h"

#include "CompressBase.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  COMPRESSBASE::COMPRESSBASE
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/11/2003 16:28:44
//
//  @return
//  */
//-------------------------------------------------------------------
COMPRESSBASE::COMPRESSBASE()
{
  SetType();
}


//-------------------------------------------------------------------
//  COMPRESSBASE::COMPRESSBASE
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/11/2003 16:28:55
//
//  @return
//  @param        type :
*/
//-------------------------------------------------------------------
COMPRESSBASE::COMPRESSBASE(COMPRESSBASE_TYPE type)
{
  SetType(type);
}



//-------------------------------------------------------------------
//  COMPRESSBASE::~COMPRESSBASE
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/11/2003 16:29:12
//
//  @return
//  */
//-------------------------------------------------------------------
COMPRESSBASE::~COMPRESSBASE()
{
  SetType();
}



//-------------------------------------------------------------------
//  COMPRESSBASE::GetType
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/11/2003 16:28:34
//
//  @return       COMPRESSBASE_TYPE :
//  */
//-------------------------------------------------------------------
COMPRESSBASE_TYPE COMPRESSBASE::GetType()
{
  return type;
}



//-------------------------------------------------------------------
//  COMPRESSBASE::SetType
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/11/2003 16:28:27
//
//  @return
//  @param        type :
*/
//-------------------------------------------------------------------
COMPRESSBASE_TYPE COMPRESSBASE::SetType(COMPRESSBASE_TYPE type)
{
  this->type = type;

  return type;
}


//-------------------------------------------------------------------
//  COMPRESSBASE::Compress
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2004 18:00:06
//
//  @return       bool :
//  @param        origin :
//  @param        size :
//  @param        buffer :
*/
//-------------------------------------------------------------------
bool COMPRESSBASE::Compress(XBYTE* origin,int size,XBUFFER* buffer)
{
  if(!origin)   return false;
  if(!size)     return false;
  if(!buffer)   return false;

  memcpy(buffer->Get(),origin,size);
  buffer->SetSize(size);

  return true;
}



//-------------------------------------------------------------------
//  COMPRESSBASE::Decompress
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2004 18:00:11
//
//  @return       bool :
//  @param        origin :
//  @param        size :
//  @param        buffer :
*/
//-------------------------------------------------------------------
bool COMPRESSBASE::Decompress(XBYTE* origin,int size,XBUFFER* buffer)
{
  if(!origin)   return false;
  if(!size)     return false;
  if(!buffer)   return false;

  memcpy(buffer->Get(),origin,size);
  buffer->SetSize(size);

  return true;
}


//-------------------------------------------------------------------
//  COMPRESSBASE::Compress
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/11/2003 16:42:18
//
//  @return       bool :
//  @param        origin :
//  @param        size :
//  @param        param1 :
*/
//-------------------------------------------------------------------
bool COMPRESSBASE::Compress(XBYTE* origin,int size,void* param1)
{
  if(!origin) return false;
  if(!size)   return false;

  for(int c=0;c<size;c++)
    {
      CompressResult(true,origin[c],param1);
    }

  return false;
}



//-------------------------------------------------------------------
//  COMPRESSBASE::Decompress
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/11/2003 16:43:39
//
//  @return       bool :
//  @param        origin :
//  @param        size :
//  @param        param1 :
*/
//-------------------------------------------------------------------
bool COMPRESSBASE::Decompress(XBYTE* origin,int size,void* param1)
{
  if(!origin) return false;
  if(!size)   return false;

  for(int c=0;c<size;c++)
    {
      CompressResult(false,origin[c],param1);
    }

  return false;
}



//-------------------------------------------------------------------
//  COMPRESSBASE::CompressResult
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/11/2003 16:44:04
//
//  @return       bool :
//  @param        compress :
//  @param        data :
//  @param        param1 :
*/
//-------------------------------------------------------------------
bool COMPRESSBASE::CompressResult(bool compress,XBYTE data,void* param1)
{
  if(compress)
    {

    }
   else
    {

    }

  return false;
}




