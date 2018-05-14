/*------------------------------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1331.H
*/
/**
// \class
//
//  Data IO SPI OLED Screen SSD1305
//
//  @author  Abraham J. Velez
//  @version 07/10/2012 16:41:37
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOSPIOLEDSCREENSSD1331_H_
#define _DIOSPIOLEDSCREENSSD1331_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "DIODeviceSPI.h"
#include "DIOGPIO.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSPIOLEDSCREENSSD1331_CMD_FILLRECTDISABLE              0x00
#define DIOSPIOLEDSCREENSSD1331_CMD_FILLRECTENABLE               0x01

#define DIOSPIOLEDSCREENSSD1331_CMD_HSCROLLDISABLE               0x00
#define DIOSPIOLEDSCREENSSD1331_CMD_HSCROLLENABLE                0x01

#define DIOSPIOLEDSCREENSSD1331_CMD_VSCROLLDISABLE               0x00
#define DIOSPIOLEDSCREENSSD1331_CMD_VSCROLLENABLE                0x01

#define DIOSPIOLEDSCREENSSD1331_CMD_LOCKMODEDISABLE              0x12
#define DIOSPIOLEDSCREENSSD1331_CMD_LOCKMODEENABLE               0x16

#define DIOSPIOLEDSCREENSSD1331_CMD_SETCOLUMNADDRESS             0x15
#define DIOSPIOLEDSCREENSSD1331_CMD_SETROWADDRESS                0x75

#define DIOSPIOLEDSCREENSSD1331_CMD_DRAWLINE                     0x21
#define DIOSPIOLEDSCREENSSD1331_CMD_DRAWRECT                     0x22
#define DIOSPIOLEDSCREENSSD1331_CMD_CLEARWINDOW                  0x25
#define DIOSPIOLEDSCREENSSD1331_CMD_FILLRECT                     0x26

#define DIOSPIOLEDSCREENSSD1331_CMD_CONTINUOUSSCROLLINGSETUP     0x27
#define DIOSPIOLEDSCREENSSD1331_CMD_DEACTIVESCROLLING            0x2E
#define DIOSPIOLEDSCREENSSD1331_CMD_ACTIVESCROLLING              0x2F

#define DIOSPIOLEDSCREENSSD1331_CMD_SETCONTRASTA                 0x81
#define DIOSPIOLEDSCREENSSD1331_CMD_SETCONTRASTB                 0x82
#define DIOSPIOLEDSCREENSSD1331_CMD_SETCONTRASTC                 0x83

#define DIOSPIOLEDSCREENSSD1331_CMD_MASTERCURRENTCONTROL         0x87

#define DIOSPIOLEDSCREENSSD1331_CMD_SETPRECHARGESPEEDA           0x8A
#define DIOSPIOLEDSCREENSSD1331_CMD_SETPRECHARGESPEEDB           0x8B
#define DIOSPIOLEDSCREENSSD1331_CMD_SETPRECHARGESPEEDC           0x8C

#define DIOSPIOLEDSCREENSSD1331_CMD_SETREMAP                     0xA0
#define DIOSPIOLEDSCREENSSD1331_CMD_SETDISPLAYSTARTLINE          0xA1
#define DIOSPIOLEDSCREENSSD1331_CMD_SETDISPLAYOFFSET             0xA2
#define DIOSPIOLEDSCREENSSD1331_CMD_NORMALDISPLAY                0xA4
#define DIOSPIOLEDSCREENSSD1331_CMD_ENTIREDISPLAY_ON             0xA5
#define DIOSPIOLEDSCREENSSD1331_CMD_ENTIREDISPLAY_OFF            0xA6
#define DIOSPIOLEDSCREENSSD1331_CMD_INVERSEDISPLAY               0xA7
#define DIOSPIOLEDSCREENSSD1331_CMD_SETMULTIPLEXRATIO            0xA8
#define DIOSPIOLEDSCREENSSD1331_CMD_DISPLAYONDIM                 0xAC
#define DIOSPIOLEDSCREENSSD1331_CMD_SETMASTERCONFIGURE           0xAD
#define DIOSPIOLEDSCREENSSD1331_CMD_DISPLAYOFF                   0xAE
#define DIOSPIOLEDSCREENSSD1331_CMD_DISPLAYON                    0xAF
#define DIOSPIOLEDSCREENSSD1331_CMD_POWERSAVEMODE                0xB0
#define DIOSPIOLEDSCREENSSD1331_CMD_PHASE12PERIOD                0xB1
#define DIOSPIOLEDSCREENSSD1331_CMD_CLOCKDIVIDER                 0xB3
#define DIOSPIOLEDSCREENSSD1331_CMD_SETPRECHARGEVOLTAGE          0xBB
#define DIOSPIOLEDSCREENSSD1331_CMD_SETVVOLTAGE                  0xBE

#define DIOSPIOLEDSCREENSSD1331_CMD_LOCKMODE                     0xFD



#define DIOSPIOLEDSCREENSSD1331_BLACK                     0x0000      //   0,   0,   0
#define DIOSPIOLEDSCREENSSD1331_LIGHTGREY                 0xC618      // 192, 192, 192
#define DIOSPIOLEDSCREENSSD1331_DARKGREY                  0x7BEF      // 128, 128, 128
#define DIOSPIOLEDSCREENSSD1331_RED                       0xF800      // 255,   0,   0
#define DIOSPIOLEDSCREENSSD1331_GREEN                     0x07E0      //   0, 255,   0
#define DIOSPIOLEDSCREENSSD1331_CYAN                      0x07FF      //   0, 255, 255
#define DIOSPIOLEDSCREENSSD1331_BLUE                      0x001F      //   0,   0, 255
#define DIOSPIOLEDSCREENSSD1331_MAGENTA                   0xF81F      // 255,   0, 255
#define DIOSPIOLEDSCREENSSD1331_YELLOW                    0xFFE0      // 255, 255,   0
#define DIOSPIOLEDSCREENSSD1331_WHITE                     0xFFFF      // 255, 255, 255

#define DIOSPIOLEDSCREENSSD1331_DEFAULTTIMEOUT            3

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XSYSTEM;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMSPI;

class DIOSPIOLEDSCREENSSD1331 : public DIODEVICESPI
{
  public:
                            DIOSPIOLEDSCREENSSD1331         ();
    virtual                ~DIOSPIOLEDSCREENSSD1331         ();

    bool                    IniDevice                       ();
    bool                    Clear                           (XWORD color = DIOSPIOLEDSCREENSSD1331_BLACK);
    bool                    Update                          (XBYTE* buffer);
    bool                    PutPixel                        (XWORD x, XWORD y, XWORD color);
    bool                    End                             ();

    DIOGPIO*                GetDIOGPIO                      ()                                                                { return diogpio;                             }
    void                    SetDIOGPIO                      (DIOGPIO* diogpio)                                                { this->diogpio    = diogpio;                 }

    DIOGPIO_PINSGROUP       GetResetPinsGroup               ()                                                                { return resetpinsgroup;                      }
    void                    SetResetPinsGroup               (DIOGPIO_PINSGROUP resetpinsgroup = DIOGPIO_PINSGROUP_A)          { this->resetpinsgroup = resetpinsgroup;      }
    int                     GetResetPin                     ()                                                                { return resetpin;                            }
    void                    SetResetPin                     (int resetpin = DIOGPIO_PINNOTUSE)                                { this->resetpin = resetpin;                  }

    DIOGPIO_PINSGROUP       GetDCPinsGroup                  ()                                                                { return DCpinsgroup;                         }
    void                    SetDCPinsGroup                  (DIOGPIO_PINSGROUP DCpinsgroup = DIOGPIO_PINSGROUP_A)             { this->DCpinsgroup = DCpinsgroup;            }
    int                     GetDCPin                        ()                                                                { return DCpin;                               }
    void                    SetDCPin                        (int DCpin = DIOGPIO_PINNOTUSE)                                   { this->DCpin = DCpin;                        }

    int                     GetWidth                        ()                                                                { return width;                               }
    int                     GetHeight                       ()                                                                { return height;                              }
    XDWORD                  GetNPixels                      ()                                                                { return (width*height);                      }
    XDWORD                  GetSizeBuffer                   ()                                                                { return (GetNPixels()*sizeof(XWORD));        }

  private:

    void                    Clean                           ()
                            {
                              diogpio                 = NULL;
                              diostream               = NULL;
                              isdirectconnexion       = false;

                              resetpinsgroup          = DIOGPIO_PINSGROUP_A;
                              resetpin                = DIOGPIO_PINNOTUSE;

                              DCpinsgroup             = DIOGPIO_PINSGROUP_A;
                              DCpin                   = DIOGPIO_PINNOTUSE;

                              width                   = 0;
                              height                  = 0;
                              sizebuffervideo         = 0;
                            }

    bool                    LCD_Reset                       ();
    bool                    LCD_Init                        ();
    bool                    LCD_Active                      (bool on);
    bool                    LCD_EnableFillMode              (bool on);
    bool                    LCD_EnableLockMode              (bool on);
    bool                    LCD_EnableScrollMode            (bool on);
    bool                    LCD_SetScrollMode               (XWORD horizontal, XWORD vertical);
    bool                    LCD_Clear                       ();
    bool                    LCD_MaxWindow                   ();
    bool                    LCD_FillRect                    (XBYTE x0, XBYTE y0, XBYTE x1, XBYTE y1, XWORD color, XWORD background = DIOSPIOLEDSCREENSSD1331_BLACK);

    bool                    WriteCommand                    (XBYTE command, int timeout = DIOSPIOLEDSCREENSSD1331_DEFAULTTIMEOUT);
    bool                    WriteCommand                    (XBYTE* command, XDWORD size, int timeout = DIOSPIOLEDSCREENSSD1331_DEFAULTTIMEOUT);

    bool                    WriteData                       (XBYTE data, int timeout = DIOSPIOLEDSCREENSSD1331_DEFAULTTIMEOUT);
    bool                    WriteData                       (XBYTE* data, XDWORD size, int timeout = DIOSPIOLEDSCREENSSD1331_DEFAULTTIMEOUT);

    XSYSTEM*                xsystem;

    DIOGPIO*                diogpio;
    DIOSTREAMSPI*           diostream;
    bool                    isdirectconnexion;

    DIOGPIO_PINSGROUP       resetpinsgroup;
    int                     resetpin;

    DIOGPIO_PINSGROUP       DCpinsgroup;
    int                     DCpin;


    int                     width;
    int                     height;
    int                     sizebuffervideo;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

