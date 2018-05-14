//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMUSBLOCALENUMDEVICES.H
//
/**
// \class
//
//  ANDROID Data IO Stream USB Local Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMUSBLOCALENUMDEVICES_H_
#define _DIOANDROIDSTREAMUSBLOCALENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

#include "DIOStreamUSBLocalEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------


class DIOANDROIDSTREAMUSBLOCALENUMDEVICES :  public DIOSTREAMUSBLOCALENUMDEVICES
{
  public:
                          DIOANDROIDSTREAMUSBLOCALENUMDEVICES           ( );
    virtual              ~DIOANDROIDSTREAMUSBLOCALENUMDEVICES           ();

    bool                  Search                                      ();
    bool                  IsDeviceAvailable                           (XCHAR* resource);

  private:

    bool                  SearchAndDeleteEqualDevices                 ();

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif





