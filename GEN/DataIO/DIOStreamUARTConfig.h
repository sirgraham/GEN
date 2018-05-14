//------------------------------------------------------------------------------------------
//  DIOSTREAMUARTCONFIG.H
//
/**
// \class
//
//  Data IO Stream UART Config class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMUARTCONFIG_H_
#define _DIOSTREAMUARTCONFIG_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

#include "DIOStreamConfig.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOSTREAMUARTDATABITS
{
  DIOSTREAMUARTDATABIT_5           = 5  ,
  DIOSTREAMUARTDATABIT_6                ,
  DIOSTREAMUARTDATABIT_7                ,
  DIOSTREAMUARTDATABIT_8                ,
  DIOSTREAMUARTDATABIT_9
};

enum DIOSTREAMUARTPARITY
{
  DIOSTREAMUARTPARITY_NONE          = 0 ,
  DIOSTREAMUARTPARITY_ODD               ,
  DIOSTREAMUARTPARITY_EVEN              ,
  DIOSTREAMUARTPARITY_MARK              ,
  DIOSTREAMUARTPARITY_SPACE
};

enum DIOSTREAMUARTSTOPBITS
{
  DIOSTREAMUARTSTOPBITS_ONE         = 0 ,
  DIOSTREAMUARTSTOPBITS_ONEANDAHALF     ,
  DIOSTREAMUARTSTOPBITS_TWO             ,
};

enum DIOSTREAMUARTFLOWCONTROL
{
  DIOSTREAMUARTFLOWCONTROL_NONE     = 0 ,
  DIOSTREAMUARTFLOWCONTROL_HARD         ,
  DIOSTREAMUARTFLOWCONTROL_SOFT         ,
};


#define DIOSTREAMUARTMASK_BAUDRATE            0x0001
#define DIOSTREAMUARTMASK_DATABITS            0x0002
#define DIOSTREAMUARTMASK_PARITY              0x0004
#define DIOSTREAMUARTMASK_STOPBITS            0x0008
#define DIOSTREAMUARTMASK_FLOWCONTROL         0x0010
#define DIOSTREAMUARTMASK_ALL                 (DIOSTREAMUARTMASK_BAUDRATE | DIOSTREAMUARTMASK_DATABITS | DIOSTREAMUARTMASK_PARITY |DIOSTREAMUARTMASK_STOPBITS | DIOSTREAMUARTMASK_FLOWCONTROL)

#define DIOSTREAMUART_FLOWCONTROL_STR_NONE    __L("NONE")
#define DIOSTREAMUART_FLOWCONTROL_STR_SOFT    __L("XON/XOFF")
#define DIOSTREAMUART_FLOWCONTROL_STR_HARD    __L("RTS/CTS")



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;

class DIOSTREAMUARTCONFIG : public DIOSTREAMCONFIG
{
  public:
                                  DIOSTREAMUARTCONFIG               ();
    virtual                      ~DIOSTREAMUARTCONFIG               ();

    int                           GetPort                           ()                                        { return port;                            }
    void                          SetPort                           (int port)                                { this->port = port;                      }

    XSTRING*                      GetLocalDeviceName                ()                                        { return &devicename;                     }

    int                           GetBaudRate                       ()                                        { return baudrate;                        }
    void                          SetBaudRate                       (int baudrate)                            { this->baudrate = baudrate;              }

    DIOSTREAMUARTDATABITS         GetDataBits                       ()                                        { return databits;                        }
    void                          SetDataBits                       (DIOSTREAMUARTDATABITS databits)          { this->databits = databits;              }

    DIOSTREAMUARTPARITY           GetParity                         ()                                        { return parity;                          }
    void                          SetParity                         (DIOSTREAMUARTPARITY parity)              { this->parity = parity;                  }

    DIOSTREAMUARTSTOPBITS         GetStopBits                       ()                                        { return stopbits;                        }
    void                          SetStopBits                       (DIOSTREAMUARTSTOPBITS stopbits)          { this->stopbits = stopbits;              }

    DIOSTREAMUARTFLOWCONTROL      GetFlowControl                    ()                                        { return flowcontrol;                     }
    void                          SetFlowControl                    (DIOSTREAMUARTFLOWCONTROL flowcontrol)    { this->flowcontrol = flowcontrol;        }

    bool                          GetToString                       (XSTRING* string);
    bool                          GetToString                       (XSTRING& string)                 { return GetToString(&string);                            }

    bool                          SetFromString                     (XCHAR* string);
    bool                          SetFromString                     (XSTRING* string)
                                  {
                                    if(!string) return false;
                                    return SetFromString(string->Get());
                                  }
    bool                          SetFromString                     (XSTRING& string)                 { return SetFromString(&string);                          }

  protected:

    int                           port;
    XSTRING                       devicename;
    int                           baudrate;
    DIOSTREAMUARTDATABITS         databits;
    DIOSTREAMUARTPARITY           parity;
    DIOSTREAMUARTSTOPBITS         stopbits;
    DIOSTREAMUARTFLOWCONTROL      flowcontrol;

  private:

    void                          Clean                             ()
                                  {
                                    type         = DIOSTREAMTYPE_UART;

                                    port         = 0;
                                    baudrate     = 0;
                                    databits     = DIOSTREAMUARTDATABIT_8;
                                    parity       = DIOSTREAMUARTPARITY_NONE;
                                    stopbits     = DIOSTREAMUARTSTOPBITS_ONE;
                                    flowcontrol  = DIOSTREAMUARTFLOWCONTROL_NONE;
                                  }

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
