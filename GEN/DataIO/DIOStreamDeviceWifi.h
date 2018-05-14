//------------------------------------------------------------------------------------------
//  DIOSTREAMDEVICEWIFI.H
//
/**
// \class
//
//  Data IO Stream Device WIFI class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMDEVICEWIFI_H_
#define _DIOSTREAMDEVICEWIFI_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamDeviceIP.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------


class DIOSTREAMDEVICEWIFI : public DIOSTREAMDEVICEIP
{
  public:

                                  DIOSTREAMDEVICEWIFI     ();
    virtual                      ~DIOSTREAMDEVICEWIFI     ();

    int                           GetTransmisionPower     ()                              { return transmisionpower;                            }
    void                          SetTransmisionPower     (int transmisionpower)          { this->transmisionpower = transmisionpower;          }


    bool                          DebugPrintInfo          (XCONSOLE* xconsole);

  protected:

    int                           transmisionpower;


  private:

    void                          Clean                   ()
                                  {
                                    transmisionpower = 0;
                                  }
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
