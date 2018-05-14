//------------------------------------------------------------------------------------------
//  XWINDOWSTIMER.H
//
/**
// \class
//
//  WINDOWS timer class
//
//  @author  Abraham J. Velez
//  @version 04/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XWINDOWSTIMER_H_
#define _XWINDOWSTIMER_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>
#include <time.h>

#include "XBase.h"
#include "XTimer.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XWINDOWSTIMER : public XTIMER
{
  public:

                                XWINDOWSTIMER                     ();
    virtual                    ~XWINDOWSTIMER                     ();

    XQWORD                      GetMicroSecondsTickCounter        ();

    void                        Clean                             ()
                                {
                                  PCfrequencymilliseconds = 0.0;
                                  PCfrequencymicroseconds = 0.0;
                                }

  private:

    LARGE_INTEGER               frequency;
    double                      PCfrequencymilliseconds;
    double                      PCfrequencymicroseconds;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

