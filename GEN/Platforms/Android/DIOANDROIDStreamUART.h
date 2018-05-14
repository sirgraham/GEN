//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMUART.H
//
/**
// \class
//
//  ANDROID Data IO Stream UART class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMUART_H_
#define _DIOANDROIDSTREAMUART_H_

#if defined(DIO_ACTIVE) && defined(DIOUART_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XFSMachine.h"

#include "DIOStreamUART.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOANDROIDUARTFSMEVENTS
{
  DIOANDROIDUARTFSMEVENT_NONE              = 0  ,
  DIOANDROIDUARTFSMEVENT_CONNECTED              ,
  DIOANDROIDUARTFSMEVENT_WAITINGTOREAD          ,
  DIOANDROIDUARTFSMEVENT_DISCONNECTING          ,

  DIOANDROIDUART_LASTEVENT

};


enum DIOANDROIDUARTFSMSTATES
{
  DIOANDROIDUARTFSMSTATE_NONE              = 0  ,

  DIOANDROIDUARTFSMSTATE_CONNECTED              ,
  DIOANDROIDUARTFSMSTATE_WAITINGTOREAD          ,
  DIOANDROIDUARTFSMSTATE_DISCONNECTING          ,

  DIOANDROIDUART_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;
class DIOFACTORY;
class DIOSTREAMCONFIG;


class DIOANDROIDSTREAMUART : public DIOSTREAMUART , public XFSMACHINE
{
  public:
                          DIOANDROIDSTREAMUART            ( );
    virtual              ~DIOANDROIDSTREAMUART            ();

    DIOSTREAMSTATUS       GetConnectStatus              ();

    bool                  Open                          ();

    bool                  Config                        (XWORD mask = DIOSTREAMUARTMASK_ALL);

    XDWORD                ReadDirect                    (XBYTE* buffer, XDWORD size);
    XDWORD                WriteDirect                   (XBYTE* buffer, XDWORD size);

    bool                  Disconnect                    ()                                { return false; };
    bool                  Close                         ();

    bool                  GetCTS                        ();
    bool                  GetDSR                        ();
    bool                  GetRing                       ();
    bool                  GetRLSD                       ();

    bool                  SetRTS                        (bool on = true);
    bool                  SetDTR                        (bool on = true);

    bool                  CleanBuffers                  ();

  protected:

    void                  Clean                         ();

    static void           ThreadConnexion               (void* param);

    XTHREADCOLLECTED*     threadconnexion;

    int                   fd;

    int                   readtimeout;
    int                   writetimeout;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif





