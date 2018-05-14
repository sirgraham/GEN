//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMUDP.H
//
/**
// \class
//
//  LINUX Data IO Stream UDP class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMUDP_H_
#define _DIOLINUXSTREAMUDP_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XFSMachine.h"

#include "DIOStreamUDP.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOLINUXUDPFSMEVENTS
{
  DIOLINUXUDPFSMEVENT_NONE              = 0 ,

  DIOLINUXUDPFSMEVENT_GETTINGCONNEXION      ,
  DIOLINUXUDPFSMEVENT_CONNECTED             ,
  DIOLINUXUDPFSMEVENT_WAITINGTOREAD         ,
  DIOLINUXUDPFSMEVENT_SENDINGDATA           ,
  DIOLINUXUDPFSMEVENT_DISCONNECTING         ,

  DIOLINUXUDP_LASTEVENT

};


enum DIOLINUXUDPFSMSTATES
{
  DIOLINUXUDPFSMSTATE_NONE              = 0 ,

  DIOLINUXUDPFSMSTATE_GETTINGCONNEXION      ,
  DIOLINUXUDPFSMSTATE_CONNECTED             ,
  DIOLINUXUDPFSMSTATE_WAITINGTOREAD         ,
  DIOLINUXUDPFSMSTATE_SENDINGDATA           ,
  DIOLINUXUDPFSMSTATE_DISCONNECTING         ,

  DIOLINUXUDP_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XTHREADCOLLECTED;

class DIOLINUXSTREAMUDP : public DIOSTREAMUDP , public XFSMACHINE
{
  public:
                              DIOLINUXSTREAMUDP                     ();
    virtual                  ~DIOLINUXSTREAMUDP                     ();

    bool                      Open                                  ();
    bool                      Disconnect                            ();
    bool                      Close                                 ();

  protected:

    int                       IsReadyConnect                        (int sock);

  private:

    void                      Clean                                 ();
    static void               ThreadRunFunction                     (void* thread);

    XTHREADCOLLECTED*         threadconnexion;
    int                       handle;
    XSTRING                   remoteaddress;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif




