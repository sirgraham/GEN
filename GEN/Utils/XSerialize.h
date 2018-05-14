//------------------------------------------------------------------------------------------
//  XSERIALIZE.H
//
/**
// \class
//
//  eXtended Serialize Data
//
//  @author  Abraham J. Velez
//  @version 24/11/2004 13:00:15
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSERIALIZE_H_
#define _XSERIALIZE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


#define XSERIALIZE_ELEMENT(element,size)    { (XBYTE*)&element  , size            }
#define XSERIALIZE_SIZEELEMENT(element)     { (XBYTE*)&element  , sizeof(element) }

#define XSERIALIZE_SIZEPTR(ptr)             sizeof(ptr)/sizeof(XSERIALIZEPTR)

//---- CLASS -------------------------------------------------------------------------------

class XSERIALIZEPTR
{
  public:

    XBYTE*                   buffer;
    int                      size;
};


class XSERIALIZE
{
  public:
                             XSERIALIZE       ();
    virtual                 ~XSERIALIZE       ();

    bool                     SerializeData    (XSERIALIZEPTR* ptr, int nptr,XBUFFER* buffer,bool load = false);

  private:

};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

