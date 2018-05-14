//------------------------------------------------------------------------------------------
//  DIOSTREAMIPLOCALENUMDEVICES.H
//
/**
// \class
//
//  Data IO Stream IP Local Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMIPLOCALENUMDEVICES_H_
#define _DIOSTREAMIPLOCALENUMDEVICES_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;

class DIOSTREAMIPLOCALENUMDEVICES :  public DIOSTREAMENUMDEVICES
{
  public:
                            DIOSTREAMIPLOCALENUMDEVICES       ();
    virtual                ~DIOSTREAMIPLOCALENUMDEVICES       ();

    virtual bool            Search                            ()                                  { return false;         };
    virtual bool            StopSearch                        (bool waitend)                      { return false;         };
    bool                    IsSearching                       ()                                  { return issearching;   };

    virtual bool            IsDeviceAvailable                 (XCHAR* resource)                   { return false;         };

    DIOSTREAMDEVICE*        GetFirstActiveDevice              ();

  private:

    void                    Clean                             ();

    bool                    issearching;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
