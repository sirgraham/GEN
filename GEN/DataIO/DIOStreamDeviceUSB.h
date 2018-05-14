//------------------------------------------------------------------------------------------
//  DIOSTREAMDEVICEUSB.H
//
/**
// \class
//
//  Data IO Stream Device Bluetooth class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMDEVICEUSB_H_
#define _DIOSTREAMDEVICEUSB_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamDevice.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOSTREAMDEVICEUSBCLASS
{
  DIOSTREAMDEVICEUSBCLASS_DEVICE_USECLASSINFORMATIONINTERFACEDESCRIPTORS  = 0x00 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_AUDIO                                 = 0x01 ,
  DIOSTREAMDEVICEUSBCLASS_BOTH_COMMUNICATIONS_CDC_CONTROL                 = 0x02 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_HID                                   = 0x03 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_PHYSICAL                              = 0x05 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_IMAGE                                 = 0x06 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_PRINTER                               = 0x07 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_MASSSTORAGE                           = 0x08 ,
  DIOSTREAMDEVICEUSBCLASS_DEVICE_HUB                                      = 0x09 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_CDC_DATA                              = 0x0A ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_SMARTCARD                             = 0x0B ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_CONTENTSECURITY                       = 0x0D ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_VIDEO                                 = 0x0E ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_PERSONAL_HEALTHCARE                   = 0x0F ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_AUDIO_VIDEO_DEVICES                   = 0x10 ,
  DIOSTREAMDEVICEUSBCLASS_DEVICE_BILLBOARD_DEVICE                         = 0x11 ,
  DIOSTREAMDEVICEUSBCLASS_BOTH_DIAGNOSTIC_DEVICE                          = 0xDC ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_WIRELESS_CONTROLLER                   = 0xE0 ,
  DIOSTREAMDEVICEUSBCLASS_BOTH_MISCELLANEOUS                              = 0xEF ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_APPLICATION_SPECIFIC                  = 0xFE ,
  DIOSTREAMDEVICEUSBCLASS_BOTH_VENDOR_SPECIFIC                            = 0xFF ,
};



class DIOSTREAMDEVICEUSB : public DIOSTREAMDEVICE
{
  public:

                                                    DIOSTREAMDEVICEUSB                      ();
    virtual                                        ~DIOSTREAMDEVICEUSB                      ();

    bool                                            IsHUB                                   ()                                    { return isHUB;                      }
    void                                            SetIsHUB                                (bool isHUB)                          { this->isHUB = isHUB;               }

    XSTRING*                                        GetLocation                             ()                                    { return &location;                  }

    XSTRING*                                        GetParentID                             ()                                    { return &parentID;                  }

    XDWORD                                          GetVendorID                             ()                                    { return vendorID;                   }
    void                                            SetVendorID                             (XDWORD vendorID)                     { this->vendorID = vendorID;         }

    XDWORD                                          GetProductID                            ()                                    { return productID;                  }
    void                                            SetProductID                            (XDWORD productID)                    { this->productID=productID;         }

    DIOSTREAMDEVICEUSBCLASS                         GetClass                                ()                                    { return classdev;                   }
    void                                            SetClass                                (DIOSTREAMDEVICEUSBCLASS classdev)    { this->classdev = classdev;         }

    XSTRING*                                        GetSerialString                         ()                                    { return &serialstring;              }

    bool                                            DebugPrintInfo                          (XCONSOLE* xconsole);

  protected:

    bool                                            isHUB;
    XSTRING                                         location;
    XSTRING                                         parentID;
    XDWORD                                          vendorID;
    XDWORD                                          productID;
    DIOSTREAMDEVICEUSBCLASS                         classdev;
    XSTRING                                         serialstring;

  private:

    void                                            Clean                                   ()
                                                    {
                                                      isHUB       = false;
                                                      vendorID    = 0;
                                                      productID   = 0;
                                                      classdev    = DIOSTREAMDEVICEUSBCLASS_DEVICE_USECLASSINFORMATIONINTERFACEDESCRIPTORS;
                                                    }
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
