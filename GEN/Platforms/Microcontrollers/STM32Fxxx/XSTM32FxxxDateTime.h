//------------------------------------------------------------------------------------------
//  XSTM32FXXXDATETIME.H
//
/**
// \class
//
//  STM32Fxxx date time class
//
//  @author  Abraham J. Velez
//  @version 04/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSTM32FXXXDATETIME_H_
#define _XSTM32FXXXDATETIME_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XDateTime.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------



class XSTM32FXXXDATETIME : public XDATETIME
{
  public:
                        XSTM32FXXXDATETIME              ();
    virtual            ~XSTM32FXXXDATETIME              ();

    bool                Read                            ();
    bool                Write                           ();

    int                 GetDifferenceGMT                ();

    bool                IsDayLigthSavingTime            (int* bias = NULL);
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

