//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMTCPIP.H
//
/**
// \class
//
//  LINUX Data IO Stream TCP/IP class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMTCPIP_H_
#define _DIOLINUXSTREAMTCPIP_H_


//---- INCLUDES ----------------------------------------------------------------------------



#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamTCPIP.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOLINUXTCPIPFSMEVENTS
{
  DIOLINUXTCPIPFSMEVENT_NONE                = 0 ,

  DIOLINUXTCPIPFSMEVENT_GETTINGCONNEXION        ,
  DIOLINUXTCPIPFSMEVENT_CONNECTED               ,
  DIOLINUXTCPIPFSMEVENT_WAITINGTOREAD           ,
  DIOLINUXTCPIPFSMEVENT_SENDINGDATA             ,
  DIOLINUXTCPIPFSMEVENT_DISCONNECTING           ,

  DIOLINUXTCPIP_LASTEVENT
};


enum DIOLINUXTCPIPFSMSTATES
{
  DIOLINUXTCPIPFSMSTATE_NONE                = 0 ,

  DIOLINUXTCPIPFSMSTATE_GETTINGCONNEXION        ,
  DIOLINUXTCPIPFSMSTATE_CONNECTED               ,
  DIOLINUXTCPIPFSMSTATE_WAITINGTOREAD           ,
  DIOLINUXTCPIPFSMSTATE_SENDINGDATA             ,
  DIOLINUXTCPIPFSMSTATE_DISCONNECTING           ,

  DIOLINUXTCPIP_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;


class DIOLINUXSTREAMTCPIP : public DIOSTREAMTCPIP , public XFSMACHINE
{
  public:
                              DIOLINUXSTREAMTCPIP                     ();
    virtual                  ~DIOLINUXSTREAMTCPIP                     ();

    bool                      Open                                    ();
    bool                      Disconnect                              ();
    bool                      Close                                   ();

  protected:

    int                       Accept                                  (int socket, void* addr, void* addrlen, XDWORD usec);
    int                       IsReadyConnect                          (int socket);
    bool                      SetPropertysHandle                      (int socket);

  private:

    void                      Clean                                   ()
                              {
                                threadconnexion   = NULL;
                                handlesocket      = -1;
                              }

    bool                      GetHandleServer                         ();
    bool                      GetHandleClient                         ();


    static void               ThreadConnexion                         (void* data);

    XTHREADCOLLECTED*         threadconnexion;

    int                       handlesocket;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

