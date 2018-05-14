//------------------------------------------------------------------------------------------
//  XANDROIDSYSTEM.H
//
/**
// \class
//
//  System Resources Class
//
//  @author  Abraham J. Velez
//  @version 03/03/2004 12:15:55
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XANDROIDSYSTEM_H_
#define _XANDROIDSYSTEM_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XSystem.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XANDROIDSYSTEM : public XSYSTEM
{
  public:
                            XANDROIDSYSTEM              ();
    virtual                ~XANDROIDSYSTEM              ();

    XSYSTEM_HARDWARETYPE    GetTypeHardware             (int* revision = NULL);
    XSYSTEM_SO              GetSO                       ();
    XLANGUAGE_CODE          GetLanguage                 ();
    int                     GetTotalMemory              ();
    int                     GetFreeMemory               ();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

