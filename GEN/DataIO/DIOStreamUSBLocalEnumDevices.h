//------------------------------------------------------------------------------------------
//  DIOSTREAMUSBLOCALENUMDEVICES.H
//
/**
// \class
//
//  Data IO Stream USB Local Enum Device class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMUSBLOCALENUMDEVICES_H_
#define _DIOSTREAMUSBLOCALENUMDEVICES_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamDeviceUSB.h"
#include "DIOStreamEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSTREAMUSBLOCALENUMDEVICES_PORTKEY  __L("Port_")
#define DIOSTREAMUSBLOCALENUMDEVICES_PORTHUB  __L("Hub_")


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOSTREAMDEVICEUSB;
class DIOSTREAMUSBCONFIG;

class DIOSTREAMUSBLOCALENUMDEVICES : public DIOSTREAMENUMDEVICES
{
  public:
                                DIOSTREAMUSBLOCALENUMDEVICES        ();
    virtual                    ~DIOSTREAMUSBLOCALENUMDEVICES        ();

    virtual bool                Search                              ()                                  { return false;                                             };
    virtual bool                StopSearch                          (bool waitend)                      { return false;                                             };
    bool                        IsSearching                         ()                                  { return issearching;                                       };

    virtual bool                IsDeviceAvailable                   (XCHAR* resource)                   { return false;                                             };

    DIOSTREAMDEVICEUSB*         FindDevice                          (XCHAR* location);
    DIOSTREAMDEVICEUSB*         FindDevice                          (XSTRING& location)                 { return FindDevice(location.Get());                        };

    DIOSTREAMDEVICEUSB*         FindDevice                          (XCHAR* location, DIOSTREAMDEVICEUSBCLASS classdev);
    DIOSTREAMDEVICEUSB*         FindDevice                          (XSTRING& location, DIOSTREAMDEVICEUSBCLASS classdev)
                                {
                                  return FindDevice(location.Get(), classdev);
                                };

    DIOSTREAMDEVICEUSB*         FindDevice                          (XDWORD vendorID, XDWORD productID);
    DIOSTREAMDEVICEUSB*         FindDevice                          (XDWORD vendorID, XDWORD productID, DIOSTREAMDEVICEUSBCLASS classdev);
    DIOSTREAMDEVICEUSB*         FindDevice                          (DIOSTREAMUSBCONFIG& config);

  private:

    void                        Clean                               ();

    bool                        issearching;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
