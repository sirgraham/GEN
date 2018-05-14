//------------------------------------------------------------------------------------------
//  DIOSTREAMCONFIG.H
//
/**
// \class
//
//  Data IO Stream class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMCONFIG_H_
#define _DIOSTREAMCONFIG_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XSubject.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOSTREAMTYPE
{
  DIOSTREAMTYPE_UNKNOWN         = 0 ,
  DIOSTREAMTYPE_UART                ,
  DIOSTREAMTYPE_USB                 ,
  DIOSTREAMTYPE_ICMP                ,
  DIOSTREAMTYPE_UDP                 ,
  DIOSTREAMTYPE_TCPIP               ,
  DIOSTREAMTYPE_BLUETOOTH           ,
  DIOSTREAMTYPE_SPI                 ,
  DIOSTREAMTYPE_I2C                 ,
};


enum DIOSTREAMMODE
{
  DIOSTREAMMODE_NONE           = 0  ,
  DIOSTREAMMODE_SERVER         = 1  ,
  DIOSTREAMMODE_HOST           = 1  ,
  DIOSTREAMMODE_MASTER         = 1  ,
  DIOSTREAMMODE_SEMISERVER     = 2  ,
  DIOSTREAMMODE_SEMIHOST       = 2  ,
  DIOSTREAMMODE_SEMIMASTER     = 2  ,
  DIOSTREAMMODE_CLIENT         = 3  ,
  DIOSTREAMMODE_SLAVE          = 3
};






//---- CLASS -------------------------------------------------------------------------------


class XFACTORY;


class DIOSTREAMCONFIG : public XSUBJECT
{
  public:

                              DIOSTREAMCONFIG                 ();
    virtual                  ~DIOSTREAMCONFIG                 ();

    DIOSTREAMTYPE             GetType                         ()                                  { return type;                                            }
    void                      SetType                         (DIOSTREAMTYPE type)                { this->type = type;                                      }

    DIOSTREAMMODE             GetMode                         ()                                  { return mode;                                            }
    void                      SetMode                         (DIOSTREAMMODE mode)                { this->mode = mode;                                      }
    bool                      IsServer                        ()
                              {
                                if(mode == DIOSTREAMMODE_SERVER) return true;
                                if(mode == DIOSTREAMMODE_HOST)   return true;

                                return false;
                              }

    XDWORD                    GetSizeBufferSO                 ()                                  { return sizebufferSO;                                    }
    void                      SetSizeBufferSO                 (XDWORD sizebufferSO)               { this->sizebufferSO = sizebufferSO;                      }

    XTHREADPRIORITY           GetThreadPriority               ()                                  { return threadpriority;                                  }
    void                      SetThreadPriority               (XTHREADPRIORITY threadpriority)    { this->threadpriority =  threadpriority;                 }

    XDWORD                    GetThreadWaitYield              ()                                  { return threadwaityield;                                 }
    void                      SetThreadWaitYield              (XDWORD threadwaityield)            { this->threadwaityield =  threadwaityield;               }

  protected:

    DIOSTREAMTYPE             type;
    DIOSTREAMMODE             mode;

    XDWORD                    sizebufferSO;
    XTHREADPRIORITY           threadpriority;
    XDWORD                    threadwaityield;

  private:

    void                      Clean                           ()
                              {
                                type                    = DIOSTREAMTYPE_UNKNOWN;
                                mode                    = DIOSTREAMMODE_NONE;

                                sizebufferSO            = 0;
                                threadpriority          = XTHREADPRIORITY_NORMAL;
                                threadwaityield         = XTHREAD_DEFAULTWAITYIELD;
                              }


};



//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
