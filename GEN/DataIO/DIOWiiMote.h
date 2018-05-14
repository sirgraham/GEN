/*------------------------------------------------------------------------------------------
//  DIOWIIMOTE.H
*/
/**
// \class
//
//  DIO Wii Mote  (Wii controler)
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 15/02/2015 16:34:59
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWIIMOTE_H_
#define _DIOWIIMOTE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOWIIMOTE_DEFAULTTIMEOUT  5


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMBLUETOOTHREMOTEENUMDEVICES;
class DIOSTREAMBLUETOOTHCONFIG;
class DIOSTREAMBLUETOOTH;
class DIOSTREAMDEVICEBLUETOOTH;


class DIOWIIMOTE
{
  public:
                                          DIOWIIMOTE              ();
    virtual                              ~DIOWIIMOTE              ();

    bool                                  Ini                     (int timeout = DIOWIIMOTE_DEFAULTTIMEOUT);
    bool                                  End                     ();

  private:

    void                                  Clean                   ()
                                          {
                                            xtimerout       = NULL;
                                            diostreamcfg    = NULL;
                                            diostream       = NULL;
                                            device          = NULL;
                                          }

    XTIMER*                               xtimerout;
    DIOSTREAMBLUETOOTHREMOTEENUMDEVICES*  enumremotedevices;
    DIOSTREAMBLUETOOTHCONFIG*             diostreamcfg;
    DIOSTREAMBLUETOOTH*                   diostream;
    DIOSTREAMDEVICEBLUETOOTH*             device;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

