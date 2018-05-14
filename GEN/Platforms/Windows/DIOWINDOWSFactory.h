//------------------------------------------------------------------------------------------
//  DIOWINDOWSFACTORY.H
//
/**
// \class
//
//  WINDOWS Utils Platform Factory class
//
//  @author  Abraham J. Velez
//  @version 15/07/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSFACTORY_H_
#define _DIOWINDOWSFACTORY_H_

#ifdef DIO_ACTIVE

//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOFactory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------


class DIOWINDOWSFACTORY : public DIOFACTORY
{
  public:

    DIOURL*                   CreateURL                   ();
    bool                      DeleteURL                   (DIOURL* url);

    #ifdef ANYTYPEOFDIOSTREAMIO
    DIOSTREAMENUMDEVICES*     CreateStreamEnumDevices     (DIOSTREAMENUMTYPE type);
    bool                      DeleteStreamEnumDevices     (DIOSTREAMENUMDEVICES* enumdevices);

    DIOSTREAM*                CreateStreamIO              (DIOSTREAMCONFIG* config);
    bool                      DeleteStreamIO              (DIOSTREAM* streamio);
    #endif

    #ifdef DIOPING_ACTIVE
    DIOPING*                  CreatePing                  ();
    bool                      DeletePing                  (DIOPING* ping);
    #endif

    #ifdef DIOPCAP_ACTIVE
    virtual DIOPCAP*          CreatePCap                  ();
    virtual bool              DeletePCap                  (DIOPCAP* pcap);
    #endif

    #ifdef DIOGPIO_ACTIVE
    DIOGPIO*                  CreateGPIO                  ();
    bool                      DeleteGPIO                  (DIOGPIO* gpio);
    #endif
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif

