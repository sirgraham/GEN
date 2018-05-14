//------------------------------------------------------------------------------------------
//  DIOSTREAMUART.H
//
/**
// \class
//
//  Data IO Stream UART Local Enum Device class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMUARTLOCALENUMDEVICE_H_
#define _DIOSTREAMUARTLOCALENUMDEVICE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;


class DIOSTREAMUARTLOCALENUMDEVICES : public DIOSTREAMENUMDEVICES
{
  public:
                                DIOSTREAMUARTLOCALENUMDEVICES       ();
    virtual                    ~DIOSTREAMUARTLOCALENUMDEVICES       ();

    virtual bool                Search                              ()                                  { return false;         };
    virtual bool                StopSearch                          (bool waitend)                      { return false;         };
    bool                        IsSearching                         ()                                  { return issearching;   };

    virtual bool                IsDeviceAvailable                   (XCHAR* resource)                   { return false;         };

  private:

    void                        Clean                               ();

    bool                        issearching;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
