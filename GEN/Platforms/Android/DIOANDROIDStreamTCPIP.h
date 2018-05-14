//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMTCPIP.H
//
/**
// \class
//
//  ANDROID Data IO Stream TCP/IP class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMTCPIP_H_
#define _DIOANDROIDSTREAMTCPIP_H_


//---- INCLUDES ----------------------------------------------------------------------------



#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamTCPIP.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOANDROIDTCPIPFSMEVENTS
{
  DIOANDROIDTCPIPFSMEVENT_NONE                = 0 ,

  DIOANDROIDTCPIPFSMEVENT_GETTINGCONNEXION        ,
  DIOANDROIDTCPIPFSMEVENT_CONNECTED             ,
  DIOANDROIDTCPIPFSMEVENT_WAITINGTOREAD         ,
  DIOANDROIDTCPIPFSMEVENT_SENDINGDATA           ,
  DIOANDROIDTCPIPFSMEVENT_DISCONNECTING         ,

  DIOANDROIDTCPIP_LASTEVENT
};


enum DIOANDROIDTCPIPFSMSTATES
{
  DIOANDROIDTCPIPFSMSTATE_NONE                = 0 ,

  DIOANDROIDTCPIPFSMSTATE_GETTINGCONNEXION        ,
  DIOANDROIDTCPIPFSMSTATE_CONNECTED               ,
  DIOANDROIDTCPIPFSMSTATE_WAITINGTOREAD           ,
  DIOANDROIDTCPIPFSMSTATE_SENDINGDATA             ,
  DIOANDROIDTCPIPFSMSTATE_DISCONNECTING           ,

  DIOANDROIDTCPIP_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;


class DIOANDROIDSTREAMTCPIP : public DIOSTREAMTCPIP , public XFSMACHINE
{
  public:
                              DIOANDROIDSTREAMTCPIP                     ();
    virtual                  ~DIOANDROIDSTREAMTCPIP                     ();

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

                                memset(buffer, 0, DIOSTREAM_MAXBUFFER);
                              }

    bool                      GetHandleServer                         ();
    bool                      GetHandleClient                         ();


    static void               ThreadConnexion                         (void* data);

    XTHREADCOLLECTED*         threadconnexion;

    int                       handlesocket;
    XBYTE                     buffer[DIOSTREAM_MAXBUFFER];
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

