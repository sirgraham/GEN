/*------------------------------------------------------------------------------------------
//  DIOI2CGPIOPCF8574.H
*/
/**
// \class
//
//  Data IO I2C PCF8574 (GPIO 8 Pins) class
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 03/05/2014 16:12:18
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOI2CGPIOPCF8574_H_
#define _DIOI2CGPIOPCF8574_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>

#include "XBase.h"
#include "XString.h"

#include "DIODeviceI2C.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class XFACTORY;
class XPUBLISHER;
class XTIMER;
class DIOFACTORY;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;


class DIOI2CGPIOPCF8574 : public DIODEVICEI2C
{
  public:

                              DIOI2CGPIOPCF8574           ();
    virtual                  ~DIOI2CGPIOPCF8574           ();

    bool                      Read                        (XBYTE& value);
    bool                      Write                       (XBYTE value);

    bool                      End                         ();

  protected:

    DIOSTREAMI2C*             diostream;

  private:

    bool                      IniDevice                   ();

    void                      Clean                       ()
                              {
                                diostream     = NULL;
                              }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

