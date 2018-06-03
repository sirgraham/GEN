//------------------------------------------------------------------------------------------
//  XANDROIDRAND.H
//
/**
// \class
//
//  Windows 32 Random Functions
//
//  @author  Abraham J. Velez
//  @version 22/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XANDROIDRAND_H_
#define _XANDROIDRAND_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>

#include "XRand.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------

class XANDROIDRAND : public XRAND
{
  public:

    bool          Ini               ();
    int           MaxElements       (int max);
    int           Max               (int max);
    int           Between           (int min, int max);
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

