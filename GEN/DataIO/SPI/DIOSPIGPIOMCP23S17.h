/*------------------------------------------------------------------------------------------
//  DIOSPIGPIOMCP23S17.H
*/
/**
// \class
//
//  Data IO SPI MCP23S17 (Microchip) (8+8 Digital I/O) class
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 03/05/2014 16:12:18
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOSPIGPIOMCP23S17_H_
#define _DIOSPIGPIOMCP23S17_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>

#include "XBase.h"

#include "DIODevice.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOSPIGPIOMCP23S17_WRITECMD         0x40
#define DIOSPIGPIOMCP23S17_READCMD          0x41

#define DIOSPIGPIOMCP23S17_IODIRA           0x00    // I/O direction A
#define DIOSPIGPIOMCP23S17_IODIRB           0x01    // I/O direction B
#define DIOSPIGPIOMCP23S17_IOCON            0x0A    // I/O config
#define DIOSPIGPIOMCP23S17_GPIOA            0x12    // port A
#define DIOSPIGPIOMCP23S17_GPIOB            0x13    // port B
#define DIOSPIGPIOMCP23S17_GPPUA            0x0C    // port A pullups
#define DIOSPIGPIOMCP23S17_GPPUB            0x0D    // port B pullups


/*---- CLASS -----------------------------------------------------------------------------*/


class XFACTORY;
class DIOFACTORY;
class XPUBLISHER;
class DIOSTREAMSPICONFIG;
class DIOSTREAMSPI;

class DIOSPIGPIOMCP23S17 : public DIODEVICE
{
  public:

                              DIOSPIGPIOMCP23S17            ();
    virtual                  ~DIOSPIGPIOMCP23S17            ();

    bool                      Ini                           (int chipselect, int timeout);
    bool                      Configure                     ();

    bool                      SendCommand                   (XBYTE cmd, XBYTE port, XBYTE value);

    bool                      End                           ();


    DIOSTREAMSPI*             GetDIOStreamSPI               ()                                              { return diostreamSPI;                            }
    void                      SetDIOStreamSPI               (DIOSTREAMSPI* diostreamSPI)
                              {
                                this->diostreamSPI     = diostreamSPI;
                                isdiostreamSPIexternal = true;
                              }

  protected:

    int                       timeout;

    DIOSTREAMSPICONFIG*       diostreamSPIcfg;
    DIOSTREAMSPI*             diostreamSPI;
    bool                      isdiostreamSPIexternal;

  private:

    void                      Clean                       ()
                              {
                                timeout                 = 0;

                                diostreamSPIcfg         = NULL;
                                diostreamSPI            = NULL;
                                isdiostreamSPIexternal  = false;
                              }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

