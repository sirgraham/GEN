//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES.H
//
/**
// \class
//
//  ANDROID Data IO Stream Bluetooth Remote Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES_H_
#define _DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

#include "XFSMachine.h"
#include "DIOMAC.h"
#include "DIOStreamBluetoothRemoteEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOANDROIDBTENUMFSMFSMEVENTS
{
  DIOANDROIDBTENUMFSMEVENT_NONE             = 0 ,
  DIOANDROIDBTENUMFSMEVENT_SEARCHMAC            ,
  DIOANDROIDBTENUMFSMEVENT_SEARCHSERVICES       ,
  DIOANDROIDBTENUMFSMEVENT_SEARCHNAME           ,
  DIOANDROIDBTENUMFSMEVENT_SEARCHEND            ,

  DIOANDROIDBTENUM_LASTEVENT
};


enum DIOANDROIDBTENUMFSMSTATES
{
  DIOANDROIDBTENUMFSMSTATE_NONE             = 0 ,
  DIOANDROIDBTENUMFSMSTATE_SEARCHMAC            ,
  DIOANDROIDBTENUMFSMSTATE_SEARCHSERVICES       ,
  DIOANDROIDBTENUMFSMSTATE_SEARCHNAME           ,
  DIOANDROIDBTENUMFSMSTATE_SEARCHEND            ,

  DIOANDROIDBTENUM_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTHREAD;
class DIOSTREAMDEVICEBLUETOOTH;


struct search_context
{
  char*       svc;      /* Service */
  uuid_t      group;    /* Browse group */
  int         tree;     /* Display full attribute tree */
  uint32_t    handle;   /* Service record handle */
};


class DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES :  public DIOSTREAMBLUETOOTHREMOTEENUMDEVICES, public XFSMACHINE
{
  public:
                              DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES      ( );
    virtual                  ~DIOANDROIDSTREAMBLUETOOTHREMOTEENUMDEVICES      ();

    bool                      Search                                          ();
    bool                      StopSearch                                      (bool waitend);
    bool                      IsSearching                                     ();

  private:

    void                      Clean                                           ();

    bool                      GetAddrFromLocalDevice                          (int localdeviceindex, char* btaddr);

    int                       IsReadyConnect                                  (int socket);

    void                      SearchServices                                  ();
    bool                      ScanDevices                                     (XVECTOR<DIOSTREAMDEVICE*>* devices);
    bool                      ScanDevicesName                                 (DIOSTREAMDEVICEBLUETOOTH* device);
    bool                      ScanDevicesServices                             (XVECTOR<DIOSTREAMDEVICE*>* devices);
    bool                      ScanDeviceServices                              (DIOSTREAMDEVICEBLUETOOTH* device, struct search_context* context);

    static void               ThreadEnumDevices                               (void* thread);

    XTHREAD*                  threadenumdevices;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif