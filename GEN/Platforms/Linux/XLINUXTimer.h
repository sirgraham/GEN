//------------------------------------------------------------------------------------------
//  XLINUXTIMER.H
//
/**
// \class
//
//  LINUX timer class
//
//  @author  Abraham J. Velez
//  @version 04/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XLINUXTIMER_H_
#define _XLINUXTIMER_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XTimer.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XLINUXTIMER : public XTIMER
{
  public:

                        XLINUXTIMER                     ();
    virtual            ~XLINUXTIMER                     ();

    XQWORD              GetMicroSecondsTickCounter      ();
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

