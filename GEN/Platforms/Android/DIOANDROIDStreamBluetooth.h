//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTH.H
//
/**
// \class
//
//  ANDROID Data IO Stream Bluetooth class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMBLUETOOTH_H_
#define _DIOANDROIDSTREAMBLUETOOTH_H_

#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <syslog.h>
#include <signal.h>
#include <netinet/in.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/l2cap.h>

#include "XBuffer.h"
#include "XFSMachine.h"

#include "DIOStreamBluetooth.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOANDROIDBTFSMEVENTS
{
  DIOANDROIDBTFSMEVENT_NONE             = 0 ,
  DIOANDROIDBTFSMEVENT_SEARCHMAC            ,
  DIOANDROIDBTFSMEVENT_SEARCHSERVICES       ,
  DIOANDROIDBTFSMEVENT_SEARCHEND            ,

  DIOANDROIDBTFSMEVENT_GETTINGCONNEXION     ,
  DIOANDROIDBTFSMEVENT_CONNECTED            ,
  DIOANDROIDBTFSMEVENT_WAITINGTOREAD        ,
  DIOANDROIDBTFSMEVENT_SENDINGDATA          ,
  DIOANDROIDBTFSMEVENT_DISCONNECTING        ,

  DIOANDROIDBT_LASTEVENT
};


enum DIOANDROIDBTFSMSTATES
{
  DIOANDROIDBTFSMSTATE_NONE             = 0 ,
  DIOANDROIDBTFSMSTATE_SEARCHMAC            ,
  DIOANDROIDBTFSMSTATE_SEARCHSERVICES       ,
  DIOANDROIDBTFSMSTATE_SEARCHEND            ,

  DIOANDROIDBTFSMSTATE_GETTINGCONNEXION     ,
  DIOANDROIDBTFSMSTATE_CONNECTED            ,
  DIOANDROIDBTFSMSTATE_WAITINGTOREAD        ,
  DIOANDROIDBTFSMSTATE_SENDINGDATA          ,
  DIOANDROIDBTFSMSTATE_DISCONNECTING        ,

  DIOANDROIDBT_LASTSTATE
};


#define DIOANDROIDSTREAMBLUETOOTH_SCANBLOCKING

#define DIOANDROIDSTREAMBLUETOOTH_DBUSAGENTPATH     "/org/bluez"


//---- CLASS -------------------------------------------------------------------------------

class XTHREAD;

class DIOANDROIDSTREAMBLUETOOTH : public DIOSTREAMBLUETOOTH , public XFSMACHINE
{
  public:
                              DIOANDROIDSTREAMBLUETOOTH             ( );
    virtual                  ~DIOANDROIDSTREAMBLUETOOTH             ();

    bool                      Open                                ();
    bool                      Disconnect                          ();
    bool                      Close                               ();

    int                       IsReadyConnect                      (int socket);

  protected:

    sdp_session_t*            SDP_RegisterService                 (char* service_name,char* service_dsc,char* service_prov,int rfcomm_channel);

  private:

    void                      Clean                               ();

    bool                      ManagementOfPIN                     (bool active, XSTRING &locMACstring, XSTRING &remMACstring, XSTRING& PIN);

    static void               ThreadRunFunction                   (void* param);

    XTHREAD*                  threadconnexion;

    int                       handlesocket;
    void*                     sdpserversession;
    int                       handleserver;

    XBYTE                     buffer[DIOSTREAM_MAXBUFFER];
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif


