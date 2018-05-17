//------------------------------------------------------------------------------------------
//  XSERIALIZE.CPP
//
//  eXtended Serialize
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 24/11/2004 13:06:06
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XSerialize.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XSERIALIZE::XSERIALIZE
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 13:06:47
//
//  @return
//  @param        xfactory :
//  */
//-------------------------------------------------------------------
XSERIALIZE::XSERIALIZE()
{

}


//-------------------------------------------------------------------
//  XSERIALIZE::~XSERIALIZE
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 13:07:06
//
//  @return
//  */
//-------------------------------------------------------------------
XSERIALIZE::~XSERIALIZE()
{

}



//-------------------------------------------------------------------
//  SerializeData
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 13:07:40
//
//  @return       bool :
//  @param        ptr :
//  @param        nptr :
//  @param        buffer :
//  @param        load :
*/
//-------------------------------------------------------------------
bool XSERIALIZE::SerializeData(XSERIALIZEPTR* ptr, int nptr,XBUFFER* buffer,bool load)
{
  if(!ptr)    return false;
  if(!nptr)   return false;
  if(!buffer) return false;

  int c;

  if(load)
    {
      for(c=0;c<nptr;c++)
        {
          buffer->Get(ptr[c].buffer,ptr[c].size);
        }
    }
   else
    {
      XDWORD size = 0;

      for(c=0;c<nptr;c++)
        {
          size += ptr[c].size;
        }

      XBUFFER* newbuffer = new XBUFFER((XDWORD)size, true);
      if((!newbuffer)||(!newbuffer->Get()))
        {
          delete newbuffer;
          return false;
        }

      for(c=0;c<nptr;c++)
        {
          newbuffer->Set(ptr[c].buffer,ptr[c].size);
        }

      buffer->Add(newbuffer);

      delete newbuffer;
    }

  return true;
}

