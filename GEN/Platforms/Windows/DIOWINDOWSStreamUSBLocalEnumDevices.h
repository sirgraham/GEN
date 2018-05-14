//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSBLOCALENUMDEVICES.H
//
/**
// \class
//
//  WINDOWS Data IO Stream USB Local Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWINDOWSSTREAMUSBLOCALENUMDEVICES_H_
#define _DIOWINDOWSSTREAMUSBLOCALENUMDEVICES_H_


#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

#include "DIOStreamDeviceUSB.h"
#include "DIOStreamUSBLocalEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------



class DIOWINDOWSSTREAMUSBLOCALENUMDEVICES :  public DIOSTREAMUSBLOCALENUMDEVICES
{
  public:
                                                    DIOWINDOWSSTREAMUSBLOCALENUMDEVICES         ();
    virtual                                        ~DIOWINDOWSSTREAMUSBLOCALENUMDEVICES         ();

    bool                                            Search                                      ();

  private:

    void                                            Clean                                       ()
                                                    {

                                                    }


    bool                                            SearchGUID                                  (const GUID* interfaceclassguid);

    bool                                            GetInfoFromRegistry                         (DIOSTREAMDEVICEUSB* device);

    bool                                            SearchAndDeleteEqualDevices                 ();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif




