//------------------------------------------------------------------------------------------
//  DIOSTREAMUART.H
//
/**
// \class
//
//  WINDOWS Data IO Stream UART class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMUART_H_
#define _DIOSTREAMUART_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStream.h"
#include "DIOStreamUARTConfig.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XMUTEX;
class XPUBLISHER;
class DIOFACTORY;


class DIOSTREAMUART : public DIOSTREAM
{
  public:
                                  DIOSTREAMUART                     ();
    virtual                      ~DIOSTREAMUART                     ();

    DIOSTREAMCONFIG*              GetConfig                         ();
    virtual bool                  SetConfig                         (DIOSTREAMCONFIG* config);

    virtual DIOSTREAMSTATUS       GetConnectStatus                  ()                                    = 0;

    virtual bool                  Open                              ()                                    = 0;

    virtual bool                  Config                            (XWORD mask = DIOSTREAMUARTMASK_ALL)  = 0;

    virtual XDWORD                ReadDirect                        (XBYTE* buffer, XDWORD size)          = 0;
    virtual XDWORD                WriteDirect                       (XBYTE* buffer, XDWORD size)          = 0;

    virtual bool                  Close                             ()                                    = 0;

    virtual bool                  GetCTS                            ()                                            { return false;  };
    virtual bool                  GetDSR                            ()                                            { return false;  };
    virtual bool                  GetRing                           ()                                            { return false;  };
    virtual bool                  GetRLSD                           ()                                            { return false;  };

    virtual bool                  SetRTS                            (bool on=true)                                { return false;  };
    virtual bool                  SetDTR                            (bool on=true)                                { return false;  };

    virtual bool                  CleanBuffers                      ()                                            { return false;  };


  protected:

    DIOSTREAMUARTCONFIG*          config;
    XTIMER*                       xtimer;
    //XBYTE                         buffer[DIOSTREAMUART_MAXBUFFER];

  private:

    void                          Clean                             ();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
