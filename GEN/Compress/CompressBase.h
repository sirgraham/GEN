//------------------------------------------------------------------------------------------
//  COMPRESSBASE.H
//
/**
// \class
//
//  Compress Class Base
//
//  @author  Abraham J. Velez
//  @version 28/10/2003
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _COMPRESSBASE_H_
#define _COMPRESSBASE_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum COMPRESSBASE_TYPE
{
  COMPRESSBASE_TYPE_NONE      = 0 ,
  COMPRESSBASE_TYPE_LZW           ,
  COMPRESSBASE_TYPE_LZRW1KH       ,
  COMPRESSBASE_TYPE_ZIP           ,
  COMPRESSBASE_TYPE_GZ
};

//---- CLASS -------------------------------------------------------------------------------

class XBUFFER;

class COMPRESSBASE
{
  public:
                          COMPRESSBASE          ();
                          COMPRESSBASE          (COMPRESSBASE_TYPE type);
    virtual              ~COMPRESSBASE          ();

    COMPRESSBASE_TYPE     GetType               ();
    COMPRESSBASE_TYPE     SetType               (COMPRESSBASE_TYPE type = COMPRESSBASE_TYPE_NONE);

    virtual bool          Compress              (XBYTE* origin,XDWORD size,XBUFFER* buffer);
    virtual bool          Decompress            (XBYTE* origin,XDWORD size,XBUFFER* buffer);

    virtual bool          Compress              (XBYTE* origin,XDWORD size,void* param1 = NULL);
    virtual bool          Decompress            (XBYTE* origin,XDWORD size,void* param1 = NULL);

    virtual bool          CompressResult        (bool compress,XBYTE data,void* param1 = NULL);

  protected:

    void*                 param1;

  private:

    COMPRESSBASE_TYPE     type;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

