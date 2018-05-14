//  XLINUXSYSTEM.H
//
/**
// \class
//
//  LINUX system class
//
//  @author  Abraham J. Velez
//  @version 03/03/2004 12:15:55
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XLINUXSYSTEM_H_
#define _XLINUXSYSTEM_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XSystem.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

//class BTIDDEVICE_LOCALCFG;


class XLINUXSYSTEM : public XSYSTEM
{
  public:
                            XLINUXSYSTEM                ();
    virtual                ~XLINUXSYSTEM                ();

    XSYSTEM_HARDWARETYPE    GetTypeHardware             (int* revision = NULL);
    XSYSTEM_SO              GetSO                       ();
    XLANGUAGE_CODE          GetLanguage                 ();
    bool                    GetMemoryInfo               (XDWORD& total,XDWORD& free);

    bool                    MakeCommand                 (XCHAR* command, int* returncode = NULL);

    bool                    ExecuteApplication          (XCHAR* command, XCHAR* params = NULL, bool special = false);

    bool                    IsApplicationRunning        (XCHAR* command, XDWORD* ID = NULL);

    bool                    ShutDown                    (XSYSTEM_SHUTDOWNTYPE type);
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

