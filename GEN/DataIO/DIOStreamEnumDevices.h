//------------------------------------------------------------------------------------------
//  DIOSTREAMENUMDEVICES.H
//
/**
// \class
//
//  Data IO Stream Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMENUMDEVICES_H_
#define _DIOSTREAMENUMDEVICES_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"
#include "XString.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOSTREAMENUMTYPE
{
  DIOSTREAMENUMTYPE_UNKNOWN               = 0 ,
  DIOSTREAMENUMTYPE_UART_LOCAL                ,
  DIOSTREAMENUMTYPE_USB_LOCAL                 ,
  DIOSTREAMENUMTYPE_IP_LOCAL                  ,
  DIOSTREAMENUMTYPE_LOCAL_SERVERS             ,
  DIOSTREAMENUMTYPE_TCPIP_LOCAL_SERVERS       ,
  DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL           ,
  DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE          ,
  DIOSTREAMENUMTYPE_WIFI_REMOTE               ,
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class XTIMER;
class DIOSTREAMDEVICE;


class DIOSTREAMENUMDEVICES
{
  public:
                                    DIOSTREAMENUMDEVICES            ();
    virtual                        ~DIOSTREAMENUMDEVICES            ();

    DIOSTREAMENUMTYPE               GetType                         ();
    bool                            SetType                         (DIOSTREAMENUMTYPE type);

    virtual bool                    Search                          ()                                  { return false;    };
    virtual bool                    StopSearch                      (bool waitend)                      { return false;    };
    virtual bool                    IsSearching                     ()                                  { return false;    };

    bool                            AddDevice                       (DIOSTREAMDEVICE* device);
    XVECTOR<DIOSTREAMDEVICE*>*      GetDevices                      ()                                  { return &devices; };
    bool                            DelAllDevices                   ();

    virtual bool                    IsDeviceAvailable               (XCHAR* string)                     { return false;    };
    bool                            IsDeviceAvailable               (XSTRING& string)                   { return IsDeviceAvailable(string.Get());   };
    bool                            AreDevicesAvailable             ()                                  { return devices.GetSize()?true:false;      };

    DIOSTREAMDEVICE*                GetDeviceByName                 (XCHAR* name);
    DIOSTREAMDEVICE*                GetDeviceByName                 (XSTRING& name)                     { return GetDeviceByName(name.Get());       };

    DIOSTREAMDEVICE*                GetDeviceByIndex                (int index);

  protected:

    XTIMER*                         xtimerout;

    DIOSTREAMENUMTYPE               type;

    XVECTOR<DIOSTREAMDEVICE*>       devices;

  private:

    void                            Clean                           ()
                                    {
                                      xtimerout       = NULL;
                                    }
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
