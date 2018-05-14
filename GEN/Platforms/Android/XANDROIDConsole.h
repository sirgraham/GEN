//------------------------------------------------------------------------------------------
//  XANDROIDCONSOLE.H
//
/**
// \class
//
//  ANDROID Console class
//
//  @author  Abraham J. Velez
//  @version 03/03/2004 12:15:55
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XANDROIDCONSOLE_H_
#define _XANDROIDCONSOLE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XConsole.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------



class XANDROIDCONSOLE : public XCONSOLE
{
  public:
                        XANDROIDCONSOLE       ();
    virtual            ~XANDROIDCONSOLE       ();

    bool                Maximize              ();
    bool                Minimize              ();
    bool                Hide                  ();
    bool                IsHide                ();
    bool                Print                 (XCHAR* string);
    bool                Clear                 ();

    bool                KBHit                 (void);
    int                 GetChar               ();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

