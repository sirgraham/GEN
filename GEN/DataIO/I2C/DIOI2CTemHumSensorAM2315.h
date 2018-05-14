/*------------------------------------------------------------------------------------------
//  DIOI2CTEMHUMSENSORAM2315.H
*/
/**
// \class
//
//  Data IO I2C Sensor AOSONG AM2315  class (Temperature and humidity module)
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 03/05/2014 16:12:18
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOI2CTEMHUMSENSORAM2315_H_
#define _DIOI2CTEMHUMSENSORAM2315_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>

#include "XBase.h"

#include "DIODeviceI2C.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class XFACTORY;
class XPUBLISHER;
class XTIMER;
class DIOFACTORY;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;


class DIOI2CTEMHUMSENSORAM2315: public DIODEVICEI2C
{
  public:

                              DIOI2CTEMHUMSENSORAM2315      ();
    virtual                  ~DIOI2CTEMHUMSENSORAM2315      ();

    bool                      Read                          (float& temperature, float& humidity);

    bool                      End                           ();

  protected:

    DIOSTREAMI2C*             diostream;

  private:

    bool                      IniDevice                     ();

    void                      Clean                         ()
                              {
                                diostream     = NULL;
                              }

    bool                      CalculeCRC                    (XBYTE* buffer, XWORD& CRC);
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

