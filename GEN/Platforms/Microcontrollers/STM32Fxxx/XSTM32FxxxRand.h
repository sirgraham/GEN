//------------------------------------------------------------------------------------------
//  XSTM32FXXXRAND.H
//
/**
// \class
//
//  STM32Fxxx rand class
//
//  @author  Abraham J. Velez
//  @version 22/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSTM32FXXXRAND_H_
#define _XSTM32FXXXRAND_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>

#include "XRand.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XSTM32FXXXRAND : public XRAND
{
  public:
                  XSTM32FXXXRAND    ();
    virtual      ~XSTM32FXXXRAND    ();

    bool          Ini               ();
    int           MaxElements       (int max);
    int           Max               (int max);
    int           Between           (int min, int max);
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

