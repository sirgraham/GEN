/*------------------------------------------------------------------------------------------
//  DIOSTREAMSPI.H
*/
/**
// \class
//
//  Data IO Stream SPI Config class
//
//  @author  Abraham J. Velez
//  @version 20/09/2012 14:31:53
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOSTREAMSPICONFIG_H_
#define _DIOSTREAMSPICONFIG_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOStream.h"
#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOSTREAMSPI_CPHA           0x01
#define DIOSTREAMSPI_CPOL           0x02

#define DIOSTREAMSPI_MODE_0         (0|0)
#define DIOSTREAMSPI_MODE_1         (0|DIOSTREAMSPI_CPHA)
#define DIOSTREAMSPI_MODE_2         (DIOSTREAMSPI_CPOL|0)
#define DIOSTREAMSPI_MODE_3         (DIOSTREAMSPI_CPOL|DIOSTREAMSPI_CPHA)

#define DIOSTREAMSPI_CS_HIGH        0x04
#define DIOSTREAMSPI_LSB_FIRST      0x08
#define DIOSTREAMSPI_3WIRE          0x10
#define DIOSTREAMSPI_LOOP           0x20
#define DIOSTREAMSPI_NO_CS          0x40
#define DIOSTREAMSPI_READY          0x80

/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;


class DIOSTREAMSPICONFIG : public DIOSTREAMCONFIG
{
  public:
                                  DIOSTREAMSPICONFIG                ();
    virtual                      ~DIOSTREAMSPICONFIG                ();

    int                           GetPort                           ()                                { return port;                                }
    void                          SetPort                           (int port)                        { this->port = port;                          }

    XSTRING*                      GetLocalDeviceName                ()                                { return &localdevicename;                    }
    bool                          SetLocalDeviceNameByPort          ();

    int                           GetChipSelect                     ()                                { return chipselect;                          }
    void                          SetChipSelect                     (int chipselect)                  { this->chipselect = chipselect;              }


    bool                          GetPinsGroups                     (DIOGPIO_PINSGROUP& pinsgroup_CLK, DIOGPIO_PINSGROUP& pinsgroup_MISO, DIOGPIO_PINSGROUP& pinsgroup_MOSI, DIOGPIO_PINSGROUP& pinsgroup_CS)
                                  {
                                    pinsgroup_CLK  = this->pinsgroup_CLK;
                                    pinsgroup_MISO = this->pinsgroup_MISO;
                                    pinsgroup_MOSI = this->pinsgroup_MOSI;
                                    pinsgroup_CS   = this->pinsgroup_CS;

                                    return true;
                                  }


    bool                          GetPins                           (int& pin_CLK, int& pin_MISO, int& pin_MOSI, int& pin_CS)
                                  {
                                    pin_CLK  = this->pin_CLK;
                                    pin_MISO = this->pin_MISO;
                                    pin_MOSI = this->pin_MOSI;
                                    pin_CS   = this->pin_CS;

                                    return true;
                                  }

    bool                          IsDirectAccess                    ()                                { return isdirectaccess;                      }

    XBYTE                         GetSPIMode                        ()                                { return SPImode;                             }
    XBYTE                         GetNBitsWord                      ()                                { return nbitsword;                           }
    XDWORD                        GetSpeed                          ()                                { return speed;                               }
    XWORD                         GetDelay                          ()                                { return delay;                               }
    bool                          IsOnlyWrite                       ()                                { return isonlywrite;                         }


    bool                          SetPinsGroups                     (DIOGPIO_PINSGROUP& pinsgroup_CLK, DIOGPIO_PINSGROUP& pinsgroup_MISO, DIOGPIO_PINSGROUP& pinsgroup_MOSI, DIOGPIO_PINSGROUP& pinsgroup_CS)
                                  {
                                    this->pinsgroup_CLK   = pinsgroup_CLK;
                                    this->pinsgroup_MISO  = pinsgroup_MISO;
                                    this->pinsgroup_MOSI  = pinsgroup_MOSI;
                                    this->pinsgroup_CS    = pinsgroup_CS;

                                    isdirectaccess  = true;

                                    return true;
                                  }

    bool                          SetPins                           (int pin_CLK,int pin_MISO,int pin_MOSI,int pin_CS)
                                  {
                                    this->pin_CLK   = pin_CLK;
                                    this->pin_MISO  = pin_MISO;
                                    this->pin_MOSI  = pin_MOSI;
                                    this->pin_CS    = pin_CS;

                                    isdirectaccess  = true;

                                    return true;
                                  }

    void                          SetSPIMode                        (XBYTE modeSPI)                  { this->SPImode = SPImode;                     }
    void                          SetNBitsWord                      (XBYTE nbitsword)                { this->nbitsword = nbitsword;                 }
    void                          SetSpeed                          (XDWORD speed)                   { this->speed = speed;                         }
    void                          SetDelay                          (XWORD delay)                    { this->delay = delay;                         }
    void                          SetIsOnlyWrite                    (bool isonlywrite)               { this->isonlywrite = isonlywrite;             }

  protected:

    int                           port;
    XSTRING                       localdevicename;
    int                           chipselect;

    bool                          isdirectaccess;

    DIOGPIO_PINSGROUP             pinsgroup_CLK;
    int                           pin_CLK;
    DIOGPIO_PINSGROUP             pinsgroup_MISO;
    int                           pin_MISO;
    DIOGPIO_PINSGROUP             pinsgroup_MOSI;
    int                           pin_MOSI;
    DIOGPIO_PINSGROUP             pinsgroup_CS;
    int                           pin_CS;

    XBYTE                         SPImode;
    XBYTE                         nbitsword;
    XDWORD                        speed;
    XWORD                         delay;
    bool                          isonlywrite;

  private:

    void                          Clean                             ()
                                  {
                                    type              = DIOSTREAMTYPE_SPI;

                                    port              = 0;
                                    localdevicename.Empty();
                                    chipselect        = 0;

                                    isdirectaccess    = false;

                                    pinsgroup_CLK     = DIOGPIO_PINSGROUP_A;
                                    pin_CLK           = DIOGPIO_PINNOTUSE;
                                    pinsgroup_MISO    = DIOGPIO_PINSGROUP_A;
                                    pin_MISO          = DIOGPIO_PINNOTUSE;
                                    pinsgroup_MOSI    = DIOGPIO_PINSGROUP_A;
                                    pin_MOSI          = DIOGPIO_PINNOTUSE;
                                    pinsgroup_CS      = DIOGPIO_PINSGROUP_A;
                                    pin_CS            = DIOGPIO_PINNOTUSE;

                                    SPImode           = 0;
                                    nbitsword         = 0;
                                    speed             = 0;
                                    delay             = 0;
                                    isonlywrite       = false;
                                  }
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

