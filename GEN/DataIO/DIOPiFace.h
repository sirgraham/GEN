/*------------------------------------------------------------------------------------------
//  DIOPIFACE.H
*/
/**
// \class
//
//  Data IO PiFace Shield (Raspberry Pi)
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 03/05/2014 16:12:18
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOPIFACE_H_
#define _DIOPIFACE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>

#include "DIOSPIGPIOMCP23S17.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOPIFACE_OUTPUTPORT      DIOSPIGPIOMCP23S17_GPIOA
#define DIOPIFACE_INPUTPORT       DIOSPIGPIOMCP23S17_GPIOB

/*---- CLASS -----------------------------------------------------------------------------*/

class DIOPIFACE : public DIOSPIGPIOMCP23S17
{
  public:

                              DIOPIFACE             ();
    virtual                  ~DIOPIFACE             ();

    bool                      Configure             ();

    bool                      ReadInputs            (XBYTE& value);
    bool                      WriteOutputs          (XBYTE value);

    bool                      End                   ();


};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

