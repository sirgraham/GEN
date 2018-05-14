/*------------------------------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301.H
*/
/**
// \class
//
//  AOSONG AM2301 Sensor class (Temperature and humidity module)
//
//  @author  Abraham J. Velez
//  @version 25/11/2013 22:50:29
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOTEMHUMSENSORAM2301_H_
#define _DIOTEMHUMSENSORAM2301_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

#include "DIODevice.h"
#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOTEMHUMSENSORAM2301_INPUT   true
#define DIOTEMHUMSENSORAM2301_OUTPUT  false

#define DIOTEMHUMSENSORAM2301_HIGH    true
#define DIOTEMHUMSENSORAM2301_LOW     false


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XTIMER;
class XTHREADCOLLECTED;
class XMUTEX;
class DIOGPIO;


class DIOTEMHUMSENSORAM2301 : public DIODEVICE
{
  public:
                                  DIOTEMHUMSENSORAM2301       (DIOGPIO* diogpio, int pindata, bool activatecache);
    virtual                      ~DIOTEMHUMSENSORAM2301       ();

    bool                          Ini                         (int cadenceread = 30);

    int                           GetPinData                  ()                                { return pindata;                   }

    bool                          ReadFromCache               (float& temperature, float& humidity);
    bool                          ReadDirect                  (float& temperature, float& humidity);

    bool                          End                         ();

  private:

    void                          Clean                       ()
                                  {
                                    diogpio       = NULL;
                                    pindata           = DIOGPIO_PINNOTUSE;

                                    cadenceread       = 0;

                                    xtimer            = NULL;
                                    threadcache       = NULL;
                                    xmutexread        = NULL;

                                    nreads            = 0;
                                    temperaturecache  = 0;
                                    humiditycache     = 0;
                                  }

    bool                          WaitTo                      (bool tohigh, int timeout, int* timeelapsed = NULL);

    static void                   ThreadRunFunction           (void* param);

    DIOGPIO*                      diogpio;
    int                           pindata;

    int                           cadenceread;

    XTIMER*                       xtimer;
    XTHREADCOLLECTED*             threadcache;
    XMUTEX*                       xmutexread;

    XDWORD                        nreads;
    float                         temperaturecache;
    float                         humiditycache;
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

