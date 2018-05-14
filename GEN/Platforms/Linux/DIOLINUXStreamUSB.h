//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMUSB.H
//
/**
// \class
//
//  LINUX Data IO Stream USB class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMUSB_H_
#define _DIOLINUXSTREAMUSB_H_


#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XFSMachine.h"
#include "XLINUXThread.h"

#include "DIOStreamUSB.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOLINUXUSBFSMEVENTS
{
  DIOLINUXUSBFSMEVENT_NONE               = 0  ,
  DIOLINUXUSBFSMEVENT_CONNECTED             ,
  DIOLINUXUSBFSMEVENT_WAITINGTOREAD         ,
  DIOLINUXUSBFSMEVENT_SENDINGDATA           ,
  DIOLINUXUSBFSMEVENT_DISCONNECTING         ,

  DIOLINUXUSB_LASTEVENT

};


enum DIOLINUXUSBFSMSTATES
{
  DIOLINUXUSBFSMSTATE_NONE               = 0  ,

  DIOLINUXUSBFSMSTATE_CONNECTED             ,
  DIOLINUXUSBFSMSTATE_WAITINGTOREAD         ,
  DIOLINUXUSBFSMSTATE_SENDINGDATA           ,
  DIOLINUXUSBFSMSTATE_DISCONNECTING         ,

  DIOLINUXUSB_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;
class DIOFACTORY;
class DIOSTREAMCONFIG;


class DIOLINUXSTREAMUSB : public DIOSTREAMUSB , public XFSMACHINE
{
  public:
                          DIOLINUXSTREAMUSB             ();
    virtual              ~DIOLINUXSTREAMUSB             ();

    DIOSTREAMSTATUS       GetConnectStatus              ();

    bool                  Open                          ();

    bool                  Disconnect                    ()                                { return false; };
    bool                  Close                         ();

    bool                  CleanBuffers                  ();

  protected:

    void                  Clean                         ();

    static void           ThreadConnexion               (void* data);

    XDWORD                ReadBuffer                    (XBYTE* buffer,XDWORD size);
    XDWORD                WriteBuffer                   (XBYTE* buffer,XDWORD size);

    XTHREADCOLLECTED*     threadconnexion;

    int                   fd;

    int                   readtimeout;
    int                   writetimeout;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif





