//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMBLUETOOTH.H
//
/**
// \class
//
//  LINUX Data IO Stream Bluetooth class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMBLUETOOTH_H_
#define _DIOLINUXSTREAMBLUETOOTH_H_

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

enum DIOLINUXBTFSMEVENTS
{
  DIOLINUXBTFSMEVENT_NONE             = 0 ,
  DIOLINUXBTFSMEVENT_SEARCHMAC            ,
  DIOLINUXBTFSMEVENT_SEARCHSERVICES       ,
  DIOLINUXBTFSMEVENT_SEARCHEND            ,

  DIOLINUXBTFSMEVENT_GETTINGCONNEXION     ,
  DIOLINUXBTFSMEVENT_CONNECTED            ,
  DIOLINUXBTFSMEVENT_WAITINGTOREAD        ,
  DIOLINUXBTFSMEVENT_SENDINGDATA          ,
  DIOLINUXBTFSMEVENT_DISCONNECTING        ,

  DIOLINUXBT_LASTEVENT
};


enum DIOLINUXBTFSMSTATES
{
  DIOLINUXBTFSMSTATE_NONE             = 0 ,
  DIOLINUXBTFSMSTATE_SEARCHMAC            ,
  DIOLINUXBTFSMSTATE_SEARCHSERVICES       ,
  DIOLINUXBTFSMSTATE_SEARCHEND            ,

  DIOLINUXBTFSMSTATE_GETTINGCONNEXION     ,
  DIOLINUXBTFSMSTATE_CONNECTED            ,
  DIOLINUXBTFSMSTATE_WAITINGTOREAD        ,
  DIOLINUXBTFSMSTATE_SENDINGDATA          ,
  DIOLINUXBTFSMSTATE_DISCONNECTING        ,

  DIOLINUXBT_LASTSTATE
};


#define DIOLINUXSTREAMBLUETOOTH_SCANBLOCKING

#define DIOLINUXSTREAMBLUETOOTH_DBUSAGENTPATH     "/org/bluez"


//---- CLASS -------------------------------------------------------------------------------

class XTHREADCOLLECTED;

class DIOLINUXSTREAMBLUETOOTH : public DIOSTREAMBLUETOOTH , public XFSMACHINE
{
  public:
                              DIOLINUXSTREAMBLUETOOTH             ();
    virtual                  ~DIOLINUXSTREAMBLUETOOTH             ();

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

    XTHREADCOLLECTED*         threadconnexion;

    int                       handlesocket;
    void*                     sdpserversession;
    int                       handleserver;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif


