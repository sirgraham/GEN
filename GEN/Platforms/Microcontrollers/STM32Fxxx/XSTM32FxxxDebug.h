//------------------------------------------------------------------------------------------
//  XSTM32FXXXDEBUG.H
//
/**
// \class
//
//  STM32Fxxx debug class
//
//  @author  Abraham J. Velez
//  @version 05/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSTM32FXXXDEBUG_H_
#define _XSTM32FXXXDEBUG_H_

#ifdef XDEBUG

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XString.h"
#include "XSTM32Fxxx_HAL.h"

#include "DIOURL.h"

#include "XSTM32FXXXFactory.h"

#include "XDebug.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XSTM32FXXXDEBUGCTRL : public XDEBUGCTRL
{
  public:
                          XSTM32FXXXDEBUGCTRL   ();
    virtual              ~XSTM32FXXXDEBUGCTRL   ();

    void                  PrintSpecial          (XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
    void                  PrintFile             (XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);

    bool                  IniUARTSpecial        ();

  private:

    void                  Clean                 ();

    UART_HandleTypeDef    huart;
    XBUFFER               xbuffercache;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif
