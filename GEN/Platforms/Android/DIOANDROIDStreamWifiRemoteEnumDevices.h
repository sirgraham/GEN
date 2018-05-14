//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES.H
//
/**
// \class
//
//  ANDROID Data IO Stream Wifi Remote Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES_H_
#define _DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOWIFI_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"
#include "DIOMAC.h"
#include "DIOStreamWifiRemoteEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOANDROIDWIFIENUMFSMFSMEVENTS
{
  DIOANDROIDWIFIENUMFSMEVENT_NONE             = 0 ,
  DIOANDROIDWIFIENUMFSMEVENT_SEARCH               ,
  DIOANDROIDWIFIENUMFSMEVENT_SEARCHEND            ,

  DIOANDROIDWIFIENUM_LASTEVENT
};


enum DIOANDROIDWIFIENUMFSMSTATES
{
  DIOANDROIDWIFIENUMFSMSTATE_NONE             = 0 ,
  DIOANDROIDWIFIENUMFSMSTATE_SEARCH               ,
  DIOANDROIDWIFIENUMFSMSTATE_SEARCHEND            ,

  DIOANDROIDWIFIENUM_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREAD;
class DIOSTREAMDEVICEWIFI;



class DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES :  public DIOSTREAMWIFIREMOTEENUMDEVICES, public XFSMACHINE
{
  public:
                              DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES             ( );
    virtual                  ~DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES             ();

    bool                      Search                                          ();
    bool                      StopSearch                                      (bool waitend);
    bool                      IsSearching                                     ();

  private:

    void                      Clean                                           ();

    static void               ThreadEnumDevices                               (void* thread);

    XTHREAD*                  threadenumdevices;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif