//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP.H
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

#ifndef _DIOWINDOWSSTREAMICMP_H_
#define _DIOWINDOWSSTREAMICMP_H_

#if defined(DIO_ACTIVE) && defined(DIOICMP_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"
#include "DIOStreamICMP.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOWINDOWSICMPFSMEVENTS
{
  DIOWINDOWSICMPFSMEVENT_NONE                 = 0 ,

  DIOWINDOWSICMPFSMEVENT_GETTINGCONNEXION         ,
  DIOWINDOWSICMPFSMEVENT_CONNECTED                ,
  DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD            ,
  DIOWINDOWSICMPFSMEVENT_SENDINGDATA              ,
  DIOWINDOWSICMPFSMEVENT_DISCONNECTING            ,

  DIOWINDOWSICMP_LASTEVENT
};


enum DIOWINDOWSICMPFSMSTATES
{
  DIOWINDOWSICMPFSMSTATE_NONE                 = 0 ,

  DIOWINDOWSICMPFSMSTATE_GETTINGCONNEXION         ,
  DIOWINDOWSICMPFSMSTATE_CONNECTED                ,
  DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD            ,
  DIOWINDOWSICMPFSMSTATE_SENDINGDATA              ,
  DIOWINDOWSICMPFSMSTATE_DISCONNECTING            ,

  DIOWINDOWSICMP_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class DIOFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;


class DIOWINDOWSSTREAMICMP : public DIOSTREAMICMP , public XFSMACHINE
{
  public:
                              DIOWINDOWSSTREAMICMP                    ();
    virtual                  ~DIOWINDOWSSTREAMICMP                    ();

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
    //XBYTE                     buffer[DIOSTREAM_MAXBUFFER];
    XSTRING                   remoteaddress;

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif