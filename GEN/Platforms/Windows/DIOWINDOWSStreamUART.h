//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMUART.H
//
/**
// \class
//
//  WINDOWS Data IO Stream UART class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMUART_H_
#define _DIOWINDOWSSTREAMUART_H_


#if defined(DIO_ACTIVE) && defined(DIOUART_ACTIVE)


//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>

#include "XFSMachine.h"
#include "DIOStreamUART.h"
#include "DIOStreamUARTConfig.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOWINDOWSUARTFSMEVENTS
{
  DIOWINDOWSUARTFSMEVENT_NONE               = 0 ,
  DIOWINDOWSUARTFSMEVENT_CONNECTED              ,
  DIOWINDOWSUARTFSMEVENT_WAITINGTOREAD          ,
  DIOWINDOWSUARTFSMEVENT_DISCONNECTING          ,

  DIOWINDOWSUART_LASTEVENT
};


enum DIOWINDOWSUARTFSMSTATES
{
  DIOWINDOWSUARTFSMSTATE_NONE               = 0 ,
  DIOWINDOWSUARTFSMSTATE_CONNECTED              ,
  DIOWINDOWSUARTFSMSTATE_WAITINGTOREAD          ,
  DIOWINDOWSUARTFSMSTATE_DISCONNECTING          ,

  DIOWINDOWSUART_LASTSTATE
};



//---- CLASS -------------------------------------------------------------------------------

class XTHREADCOLLECTED;



class DIOWINDOWSSTREAMUART : public DIOSTREAMUART , public XFSMACHINE
{
  public:
                          DIOWINDOWSSTREAMUART                          ();
    virtual              ~DIOWINDOWSSTREAMUART                          ();

    DIOSTREAMSTATUS       GetConnectStatus                              ();

    bool                  Open                                          ();

    bool                  Config                                        (XWORD mask = DIOSTREAMUARTMASK_ALL);

    XDWORD                ReadDirect                                    (XBYTE* buffer,XDWORD size);
    XDWORD                WriteDirect                                   (XBYTE* buffer,XDWORD size);

    bool                  Disconnect                                    ()                        { return false; };
    bool                  Close                                         ();

    bool                  GetCTS                                        ();
    bool                  GetDSR                                        ();
    bool                  GetRing                                       ();
    bool                  GetRLSD                                       ();

    bool                  SetRTS                                        (bool on=true);
    bool                  SetDTR                                        (bool on=true);

    bool                  CleanBuffers                                  ();

  protected:

    void                  Clean                                         ();

    bool                  SetMask                                       (XDWORD mask);
    bool                  SetTimeouts                                   ();

    static void           ThreadConnexion                               (void* data);

    XTHREADCOLLECTED*     threadconnexion;

    HANDLE                hcom;
    XDWORD                mask;
    HANDLE                hevent;
    OVERLAPPED            ovi;

    int                   readtimeout;
    int                   writetimeout;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif



