/*------------------------------------------------------------------------------------------
//  DIOI2COLEDSCREENSSD1306.H
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

#ifndef _DIOI2COLEDSCREENSSD1306_H_
#define _DIOI2COLEDSCREENSSD1306_H_


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


class DIOI2COLEDSCREENSSD1306 : public DIODEVICEI2C
{
  public:

                              DIOI2COLEDSCREENSSD1306       ();
    virtual                  ~DIOI2COLEDSCREENSSD1306       ();

    bool                      End                           ();

  protected:

    DIOSTREAMI2C*             diostream;

  private:

    bool                      IniDevice                     ();

    void                      Clean                         ()
                              {
                                diostream     = NULL;
                              }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

