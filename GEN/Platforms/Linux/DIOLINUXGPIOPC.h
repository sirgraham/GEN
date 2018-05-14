/*------------------------------------------------------------------------------------------
//  DIOLINUXGPIO.H
*/
/**
// \class
//
//  LINUX DIO port data PC class
//
//  @author  Abraham J. Velez
//  @version 25/07/2009 07:13:30 p.m.
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOLINUXGPIOPC_H_
#define _DIOLINUXGPIOPC_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

// HW_PC
#define PCPARALLEL_PORT                 0x378

/*---- CLASS -----------------------------------------------------------------------------*/

class DIOLINUXGPIOPC : public DIOGPIO
{
  public:
                          DIOLINUXGPIOPC          ();
    virtual              ~DIOLINUXGPIOPC          ();

    bool                  Ini                         (XPATH* xpath = NULL);

    bool                  SetMode                     (XBYTE pin, bool isinput, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
    bool                  Get                         (XBYTE pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
    bool                  Set                         (XBYTE pin, bool active, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);

    bool                  End                         ();

  private:

    void                  Clean                       ()
                          {

                          }

    bool                  PC_Get                      (XDWORD nport,XBYTE& data);
    bool                  PC_Set                      (XDWORD nport,XBYTE data);
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

