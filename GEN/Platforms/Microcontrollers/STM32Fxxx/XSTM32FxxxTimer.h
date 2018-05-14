//------------------------------------------------------------------------------------------
//  XSTM32FXXXTIMER.H
//
/**
// \class
//
//  STM32Fxxx timer class
//
//  @author  Abraham J. Velez
//  @version 04/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSTM32FXXXTIMER_H_
#define _XSTM32FXXXTIMER_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XTimer.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XSTM32FXXXTIMER : public XTIMER
{
  public:

                        XSTM32FXXXTIMER                 ();
    virtual            ~XSTM32FXXXTIMER                 ();

    XQWORD              GetMicroSecondsTickCounter      ();
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

