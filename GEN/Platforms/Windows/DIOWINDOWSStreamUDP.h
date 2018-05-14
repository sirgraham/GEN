//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMUDP.H
//
/**
// \class
//
//  WINDOWS Data IO Stream TCP/IP class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMUDP_H_
#define _DIOWINDOWSSTREAMUDP_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"
#include "DIOStreamUDP.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOWINDOWSUDPFSMEVENTS
{
  DIOWINDOWSUDPFSMEVENT_NONE              = 0 ,

  DIOWINDOWSUDPFSMEVENT_GETTINGCONNEXION      ,
  DIOWINDOWSUDPFSMEVENT_CONNECTED             ,
  DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD         ,
  DIOWINDOWSUDPFSMEVENT_SENDINGDATA           ,
  DIOWINDOWSUDPFSMEVENT_DISCONNECTING         ,

  DIOWINDOWSUDP_LASTEVENT
};


enum DIOWINDOWSUDPFSMSTATES
{
  DIOWINDOWSUDPFSMSTATE_NONE              = 0 ,

  DIOWINDOWSUDPFSMSTATE_GETTINGCONNEXION      ,
  DIOWINDOWSUDPFSMSTATE_CONNECTED             ,
  DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD         ,
  DIOWINDOWSUDPFSMSTATE_SENDINGDATA           ,
  DIOWINDOWSUDPFSMSTATE_DISCONNECTING         ,

  DIOWINDOWSUDP_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;
class DIOFACTORY;


class DIOWINDOWSSTREAMUDP : public DIOSTREAMUDP , public XFSMACHINE
{
  public:
                              DIOWINDOWSSTREAMUDP                     ();
    virtual                  ~DIOWINDOWSSTREAMUDP                     ();

    bool                      Open                                    ();
    bool                      Disconnect                              ();
    bool                      Close                                   ();

  protected:

    int                       IsReadyConnect                          (SOCKET socket);

  private:

    void                      Clean                                   ();
    static void               ThreadConnexion                         (void* data);

    XTHREADCOLLECTED*         threadconnexion;
    SOCKET                    handle;
    XSTRING                   remoteaddress;

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

