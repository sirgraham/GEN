//------------------------------------------------------------------------------------------
//  DIOREMOTEFILECONFIG.H
//
/**
// \class
//
//  DIO Remote Application CFG class
//
//  @author  Abraham J. Velez
//  @version 05/05/2004 20:51:37
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOREMOTEFILECONFIG_H_
#define _DIOREMOTEFILECONFIG_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFileConfig.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOREMOTEFILECONFIG_PREFIXNAMEFILE    __L("remote_")

#define DIOREMOTEFILECONFIG_SECTIONGENERAL    __L("general")
#define DIOREMOTEFILECONFIG_URLREMOTECFG      __L("urlremotecfg")


//---- CLASS -------------------------------------------------------------------------------

class DIOURL;
class DIOWEBCLIENT;

class DIOREMOTEFILECONFIG : public XFILECONFIG
{
  public:
                                        DIOREMOTEFILECONFIG       (XCHAR* namefile = NULL);
    virtual                            ~DIOREMOTEFILECONFIG       ();

    virtual bool                        Ini                       ();
    XSTRING*                            GetURLRemoteCFG           ()                     { return &URLremoteCFG;                                      }

  protected:

    XSTRING                             URLremoteCFG;

  private:

    void                                Clean                     ()
                                        {
                                          webclient     = NULL;
                                        }

    DIOWEBCLIENT*                       webclient;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

