//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES.H
//
/**
// \class
//
//  WINDOWS Data IO Stream Bluetooth Remote Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES_H_
#define _DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <Windows.h>


#include "XFSMachine.h"
#include "DIOStream.h"

#include "DIOStreamBluetoothRemoteEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOWINDOWSBTENUMFSMFSMEVENTS
{
  DIOWINDOWSBTENUMFSMEVENT_NONE             = 0 ,
  DIOWINDOWSBTENUMFSMEVENT_SEARCHMAC            ,
  DIOWINDOWSBTENUMFSMEVENT_SEARCHSERVICES       ,
  DIOWINDOWSBTENUMFSMEVENT_SEARCHNAME           ,
  DIOWINDOWSBTENUMFSMEVENT_SEARCHEND            ,

  DIOWINDOWSBTENUM_LASTEVENT
};


enum DIOWINDOWSBTENUMFSMSTATES
{
  DIOWINDOWSBTENUMFSMSTATE_NONE             = 0 ,
  DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC            ,
  DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES       ,
  DIOWINDOWSBTENUMFSMSTATE_SEARCHNAME           ,
  DIOWINDOWSBTENUMFSMSTATE_SEARCHEND            ,

  DIOWINDOWSBTENUM_LASTSTATE
};


enum DIOWINDOWSBTENUMPROTOCOLUUID
{
  DIOWINDOWSBTENUMPROTOCOLUUID_NONE         = 0X0000 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_SDP          = 0X0001 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_UDP          = 0X0002 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_RFCOMM       = 0X0003 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_TCP          = 0X0004 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_TCS_BIN      = 0X0005 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_TCS_AT       = 0X0006 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_OBEX         = 0X0008 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_IP           = 0X0009 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_FTP          = 0X000A ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HTTP         = 0X000C ,
  DIOWINDOWSBTENUMPROTOCOLUUID_WSP          = 0X000E ,
  DIOWINDOWSBTENUMPROTOCOLUUID_BNEP         = 0X000F ,
  DIOWINDOWSBTENUMPROTOCOLUUID_UPNP         = 0X0010 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HIDP         = 0X0011 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HCRP_CTRL    = 0X0012 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HCRP_DATA    = 0X0014 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HCRP_NOTE    = 0X0016 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_AVCTP        = 0X0017 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_AVDTP        = 0X0019 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_CMTP         = 0X001B ,
  DIOWINDOWSBTENUMPROTOCOLUUID_UDI          = 0X001D ,
  DIOWINDOWSBTENUMPROTOCOLUUID_MCAP_CTRL    = 0X001E ,
  DIOWINDOWSBTENUMPROTOCOLUUID_MCAP_DATA    = 0X001F ,
  DIOWINDOWSBTENUMPROTOCOLUUID_L2CAP        = 0X0100
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;


class DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES :  public DIOSTREAMBLUETOOTHREMOTEENUMDEVICES, public XFSMACHINE
{
  public:
                              DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES      ();
    virtual                  ~DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES      ();

    bool                      Search                                          ();
    bool                      StopSearch                                      (bool waitend);
    bool                      IsSearching                                     ();

  private:

    void                      Clean                                           ();

    static void               ThreadEnumDevices                               (void* thread);
    void                      SearchServices                                  ();


    XTHREADCOLLECTED*         threadenumdevices;

    WSAQUERYSET               wsaq;
    HANDLE                    hlookup;
    XBYTE                     buffer[DIOSTREAM_MAXBUFFER];

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

BOOL __stdcall    SDP_ServiceCallback     (ULONG attribID, LPBYTE valuestream, ULONG cbstreamsize, LPVOID vparam);


#endif

#endif