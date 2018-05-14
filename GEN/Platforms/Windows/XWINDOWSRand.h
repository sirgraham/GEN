//------------------------------------------------------------------------------------------
//  XWINDOWSRAND.H
//
/**
// \class
//
//  WINDOWS rand class
//
//  @author  Abraham J. Velez
//  @version 22/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XWINDOWSRAND_H_
#define _XWINDOWSRAND_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>

#include "XRand.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------


class XWINDOWSRAND : public XRAND
{
  public:
                   XWINDOWSRAND     ();
    virtual       ~XWINDOWSRAND     ();

    bool           Ini              ();

    int            MaxElements      (int max);
    int            Max              (int max);

    int            Between          (int min, int max);
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

