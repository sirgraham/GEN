//------------------------------------------------------------------------------------------
//  DIOFACTORY.H
//
/**
// \class
//
//  Data Input Output Platform Factory
//
//  @author  Abraham J. Velez
//  @version 15/07/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOFACTORY_H_
#define _DIOFACTORY_H_


#ifdef DIO_ACTIVE

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "DIOStreamEnumDevices.h"
#include "DIOStream.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


#if (defined(DIOUART_ACTIVE) || defined(DIOUSB_ACTIVE) || defined(DIOICMP_ACTIVE) || defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE)  || defined(DIOBLUETOOTH_ACTIVE) || defined(DIOSPI_ACTIVE) || defined(DIOI2C_ACTIVE))
#define ANYTYPEOFDIOSTREAMIO
#endif

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPATHS;
class XLOG;
class DIOURL;
class DIOSTREAM;
class DIOPING;
class DIOPCAP;
class DIOWEBSERVER;
class DIOGPIO;
class CIPHER;

class DIOFACTORY
{
  public:

                                      DIOFACTORY                  ()                                                                                                                                                { };
    virtual                          ~DIOFACTORY                  ()                                                                                                                                                { };

    virtual DIOURL*                   CreateURL                   ()                                                                                                                                                { return NULL;  };
    virtual bool                      DeleteURL                   (DIOURL* url)                                                                                                                                     { return false; };

    #ifdef ANYTYPEOFDIOSTREAMIO
    virtual DIOSTREAMENUMDEVICES*     CreateStreamEnumDevices     (DIOSTREAMENUMTYPE type)                                                                                                                          { return NULL;  };
    virtual bool                      DeleteStreamEnumDevices     (DIOSTREAMENUMDEVICES* enumdevices)                                                                                                               { return false; };

    virtual DIOSTREAM*                CreateStreamIO              (DIOSTREAMCONFIG* config)                                                                                                                         { return NULL;  };
    virtual bool                      DeleteStreamIO              (DIOSTREAM* streamio)                                                                                                                             { return false; };
    #endif

    #ifdef DIOPING_ACTIVE
    virtual DIOPING*                  CreatePing                  ()                                                                                                                                                { return NULL;  };
    virtual bool                      DeletePing                  (DIOPING* ping)                                                                                                                                   { return false; };
    #endif

    #ifdef DIOPCAP_ACTIVE
    virtual DIOPCAP*                  CreatePCap                  ()                                                                                                                                                { return NULL;  };
    virtual bool                      DeletePCap                  (DIOPCAP* pcap)                                                                                                                                   { return false; };
    #endif

    #ifdef DIOGPIO_ACTIVE
    virtual DIOGPIO*                  CreateGPIO                  ()                                                                                                                                                { return NULL;  };
    virtual bool                      DeleteGPIO                  (DIOGPIO* gpio)                                                                                                                           { return false; };
    #endif
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

extern DIOFACTORY* diofactory;

#endif

#endif