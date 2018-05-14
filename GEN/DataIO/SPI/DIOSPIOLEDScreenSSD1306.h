/*------------------------------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306.H
*/
/**
// \class
//
//  Data IO SPI OLED Screen SSD1306
//
//  @author  Abraham J. Velez
//  @version 07/10/2012 16:41:37
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOSPIOLEDSCREENSSD1306_H_
#define _DIOSPIOLEDSCREENSSD1306_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "DIODeviceSPI.h"
#include "DIOGPIO.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------




//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XSYSTEM;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMSPI;

class DIOSPIOLEDSCREENSSD1306 : public DIODEVICESPI
{
  public:
                            DIOSPIOLEDSCREENSSD1306         (XSYSTEM* xsystem);
    virtual                ~DIOSPIOLEDSCREENSSD1306         ();

    bool                    IniDevice                       ();
    bool                    Clear                           (XWORD color = DIOSPIOLEDSCREENSSD1306_BLACK);
    bool                    Update                          (XBYTE* buffer);
    bool                    End                             ();

    DIOGPIO*                GetDIOGPIO                      ()                                              { return diogpio;                                 }
    void                    SetDIOGPIO                      (DIOGPIO* diogpio)                              { this->diogpio    = diogpio;                     }

    DIOGPIO_PINSGROUP       GetResetPinsGroup               ()                                              { return resetpinsgroup;                          }
    void                    SetResetPinsGroup               (DIOGPIO_PINSGROUP resetpinsgroup = DIOGPIO_PINSGROUP_A)
                            {
                              this->resetpinsgroup = resetpinsgroup;
                            }

    int                     GetResetPin                     ()                                              { return resetpin;                                }
    void                    SetResetPin                     (int resetpin = DIOGPIO_PINNOTUSE)              { this->resetpin = resetpin;                      }

    int                     GetWidth                        ()                                              { return width;                                   }
    int                     GetHeight                       ()                                              { return height;                                  }
    XDWORD                  GetNPixels                      ()                                              { return (width*height);                          }
    XDWORD                  GetSizeBuffer                   ()                                              { return (GetNPixels()*sizeof(XWORD));            }

  private:

    void                    Clean                           ()
                            {
                              xsystem                 = NULL;

                              diogpio             = NULL;
                              diostream               = NULL;
                              isdirectconnexion       = false;

                              databuffer              = NULL;
                              resetpinsgroup          = DIOGPIO_PINSGROUP_A;
                              resetpin                = DIOGPIO_PINNOTUSE;

                              width                   = 0;
                              height                  = 0;
                              sizebuffervideo         = 0;
                            }


    bool                    LCD_Init                        ();
    bool                    LCD_Reset                       ();

    bool                    Buffer_AdjustWithNOP            ();
    bool                    Buffer_Send                     ();
    bool                    Buffer_Delete                   ();

    XSYSTEM*                xsystem;

    DIOGPIO*                diogpio;
    DIOSTREAMSPI*           diostream;
    bool                    isdirectconnexion;

    XBUFFER*                databuffer;
    DIOGPIO_PINSGROUP       resetpinsgroup;
    int                     resetpin;

    int                     width;
    int                     height;
    int                     sizebuffervideo;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

