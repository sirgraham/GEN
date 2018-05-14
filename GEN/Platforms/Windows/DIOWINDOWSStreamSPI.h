/*------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMSPI.H
*/
/**
// \class
//
//  Windows Data IO Stream SPI class
//
//  @author  Abraham J. Velez
//  @version 20/09/2012 14:31:53
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWINDOWSSTREAMSPI_H_
#define _DIOWINDOWSSTREAMSPI_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOStreamSPI.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class DIOFACTORY;
class XPUBLISHER;

class DIOWINDOWSSTREAMSPI : public DIOSTREAMSPI
{
  public:
                                  DIOWINDOWSSTREAMSPI                 ();
    virtual                      ~DIOWINDOWSSTREAMSPI                 ();

    virtual void                  Sleep                               (int count);

  private:

    void                          Clean                               ();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

