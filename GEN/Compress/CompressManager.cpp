//------------------------------------------------------------------------------------------
//  COMPRESSMANAGER.CPP
//
//  Compress Manager
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/05/2004 14:57:48
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------


#include "CompressLZW.h"
#include "CompressLZRW1KH.h"
#include "CompressZIP.h"
#include "CompressGZ.h"

#include "CompressManager.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------

COMPRESSMANAGER* COMPRESSMANAGER::Instance=NULL;

//-------------------------------------------------------------------
//  COMPRESSMANAGER::COMPRESSMANAGER
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 14:59:01
//
//  @return
//  */
//-------------------------------------------------------------------
COMPRESSMANAGER::COMPRESSMANAGER()
{
  Clean();
}


//-------------------------------------------------------------------
//  COMPRESSMANAGER::~COMPRESSMANAGER
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 15:00:06
//
//  @return       void :
//  */
//-------------------------------------------------------------------
COMPRESSMANAGER::~COMPRESSMANAGER()
{
  Clean();
}


//-------------------------------------------------------------------
//  COMPRESSMANAGER::Create
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 15:00:27
//
//  @return       COMPRESS* :
//  @param        type :
*/
//-------------------------------------------------------------------
COMPRESSBASE* COMPRESSMANAGER::Create(COMPRESSBASE_TYPE type)
{
  COMPRESSBASE* compress;

  switch(type)
    {
      case COMPRESSBASE_TYPE_LZW      : compress = new COMPRESS_LZW();        break;
      case COMPRESSBASE_TYPE_LZRW1KH  : compress = new COMPRESS_LZRW1KH();    break;
      case COMPRESSBASE_TYPE_ZIP      : compress = new COMPRESS_ZIP();        break;
      case COMPRESSBASE_TYPE_GZ       : compress = new COMPRESS_GZ();         break;
                            default   : compress = new COMPRESSBASE();        break;
    }

  if(compress) lastcompresstype = type;

  return compress;
}



//-------------------------------------------------------------------
//  COMPRESSMANAGER::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2004 15:00:57
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void COMPRESSMANAGER::Clean()
{
  lastcompresstype = COMPRESSBASE_TYPE_NONE;
}
