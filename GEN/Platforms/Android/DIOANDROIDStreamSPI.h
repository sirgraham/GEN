/*------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMSPI.H
*/
/**
// \class
//
//  Data IO Stream SPI class
//
//  @author  Abraham J. Velez
//  @version 20/09/2012 14:31:53
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOANDROIDSTREAMSPI_H_
#define _DIOANDROIDSTREAMSPI_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBuffer.h"
#include "XANDROIDThread.h"

#include "DIOStreamSPI.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



/*---- CLASS -----------------------------------------------------------------------------*/

class XBUFFER;
class XFACTORY;


class DIOANDROIDSTREAMSPI : public DIOSTREAMSPI
{
  public:
                                  DIOANDROIDSTREAMSPI                 ( );
    virtual                      ~DIOANDROIDSTREAMSPI                 ();

    bool                          Open                                ();
    bool                          Close                               ();
    bool                          TransferBuffer                      (XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size);

    virtual void                  Sleep                               (int count);

  private:

    void                          Clean                               ();

    int                           handle;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

