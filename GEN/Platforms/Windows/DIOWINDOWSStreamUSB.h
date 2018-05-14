//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB.H
//
/**
// \class
//
//  WINDOWS Data IO Stream USB class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMUSB_H_
#define _DIOWINDOWSSTREAMUSB_H_

#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>

#include "XFSMachine.h"
#include "DIOStreamUSB.h"
#include "DIOStreamUSBConfig.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOWINDOWSUSBFSMEVENTS
{
  DIOWINDOWSUSBFSMEVENT_NONE                = 0 ,
  DIOWINDOWSUSBFSMEVENT_CONNECTED             ,
  DIOWINDOWSUSBFSMEVENT_WAITINGTOREAD         ,
  DIOWINDOWSUSBFSMEVENT_SENDINGDATA           ,
  DIOWINDOWSUSBFSMEVENT_DISCONNECTING         ,

  DIOWINDOWSUSB_LASTEVENT
};


enum DIOWINDOWSUSBFSMSTATES
{
  DIOWINDOWSUSBFSMSTATE_NONE                = 0 ,
  DIOWINDOWSUSBFSMSTATE_CONNECTED             ,
  DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD         ,
  DIOWINDOWSUSBFSMSTATE_SENDINGDATA           ,
  DIOWINDOWSUSBFSMSTATE_DISCONNECTING         ,

  DIOWINDOWSUSB_LASTSTATE
};



//---- CLASS -------------------------------------------------------------------------------

class XTHREADCOLLECTED;
class CIPHER;



class DIOWINDOWSSTREAMUSB : public DIOSTREAMUSB , public XFSMACHINE
{
  public:
                          DIOWINDOWSSTREAMUSB                           ();
    virtual              ~DIOWINDOWSSTREAMUSB                           ();

    DIOSTREAMSTATUS       GetConnectStatus                              ();

    bool                  Open                                          ();

    bool                  Disconnect                                    ()                        { return false; };
    bool                  Close                                         ();

    bool                  CleanBuffers                                  ();

  protected:

    void                  Clean                                         ();

    static void           ThreadConnexion                               (void* data);

    XDWORD                ReadBuffer                                    (XBYTE* buffer,XDWORD size);
    XDWORD                WriteBuffer                                   (XBYTE* buffer,XDWORD size);

    XTHREADCOLLECTED*     threadconnexion;

    HANDLE                handle;
    XDWORD                mask;
    HANDLE                hevent;
    OVERLAPPED            ovi;

    int                   readtimeout;
    int                   writetimeout;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif




