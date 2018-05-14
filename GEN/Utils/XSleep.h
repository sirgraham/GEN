//------------------------------------------------------------------------------------------
//  XSLEEP.H
//
/**
// \class
//
//  Sleep Functions
//
//  @author  Abraham J. Velez
//  @version 04/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSLEEP_H_
#define _XSLEEP_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XSLEEP
{
  public:

                            XSLEEP                            ();
    virtual                ~XSLEEP                            ();

    virtual void            Seconds                           (int seconds)                           {                   }
    virtual void            MilliSeconds                      (int milliseconds)                      {                   }
    virtual void            MicroSeconds                      (int microseconds)                      {                   }
    virtual void            NanoSeconds                       (int nanoseconds)                       {                   }

 private:

    void                    Clean                             ()
                            {

                            }
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

extern XSLEEP* xsleep;

#endif
