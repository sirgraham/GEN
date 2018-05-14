//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMUSBLOCALENUMDEVICES.H
//
/**
// \class
//
//  LINUX Data IO Stream USB Local Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMUSBLOCALENUMDEVICES_H_
#define _DIOLINUXSTREAMUSBLOCALENUMDEVICES_H_

#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

#include "DIOStreamUSBLocalEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------


class DIOLINUXSTREAMUSBLOCALENUMDEVICES :  public DIOSTREAMUSBLOCALENUMDEVICES
{
  public:
                          DIOLINUXSTREAMUSBLOCALENUMDEVICES           ();
    virtual              ~DIOLINUXSTREAMUSBLOCALENUMDEVICES           ();

    bool                  Search                                      ();
    bool                  IsDeviceAvailable                           (XCHAR* resource);

  private:

    bool                  SearchAndDeleteEqualDevices                 ();

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif





