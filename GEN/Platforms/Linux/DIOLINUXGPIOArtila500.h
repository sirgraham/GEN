/*------------------------------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500.H
*/
/**
// \class
//
//  LINUX DIO port data Artila 500 class
//
//  @author  Abraham J. Velez
//  @version 25/07/2009 07:13:30 p.m.
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOLINUXGPIOARTILA500_H_
#define _DIOLINUXGPIOARTILA500_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



// HW_ARTILA500
//--------------------------------------------
// ARTILA 500
//
/* /dev/gpio, GPIO IOCTL Commands */
#define GPIO_IOCTL_COUNT                0
#define GPIO_IOCTL_DIPSW                1
#define GPIO_IOCTL_GET                  2
#define GPIO_IOCTL_SET                  3
#define GPIO_IOCTL_CLEAR                4
#define GPIO_IOCTL_OUTPUT               5
#define GPIO_IOCTL_INPUT                6
#define GPIO_IOCTL_INTERRUPT            7


/*---- CLASS -----------------------------------------------------------------------------*/

class DIOLINUXGPIOARTILA500 : public DIOGPIO
{
  public:
                          DIOLINUXGPIOARTILA500   ();
    virtual              ~DIOLINUXGPIOARTILA500   ();

    bool                  Ini                         (XPATH* xpath = NULL);

    bool                  SetMode                     (XBYTE pin, bool isinput, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
    bool                  Get                         (XBYTE pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
    bool                  Set                         (XBYTE pin, bool active, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);

    bool                  End                         ();

  private:

    void                  Clean                       ()
                          {
                            ARTILA500_fd        = -1;
                            ARTILA500_countIO   = 0;
                          }

    bool                  ARTILA500_Ini               ();
    bool                  ARTILA500_End               ();

    int                   ARTILA500_fd;
    int                   ARTILA500_countIO;
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

