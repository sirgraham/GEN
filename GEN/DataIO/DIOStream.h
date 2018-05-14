//------------------------------------------------------------------------------------------
//  DIOSTREAM.H
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

#ifndef _DIOSTREAM_H_
#define _DIOSTREAM_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XSleep.h"
#include "XUUID.h"
#include "XString.h"
#include "XSubject.h"
#include "XTimer.h"

#include "DIOStreamConfig.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOSTREAMSTATUS
{
  DIOSTREAMSTATUS_DISCONNECTED          = 0 ,
  DIOSTREAMSTATUS_CONNECTED                 ,
  DIOSTREAMSTATUS_GETTINGCONNEXION          ,
};

enum DIOSTREAMERROR
{
  DIOSTREAMERROR_NONE                   = 0 ,
  DIOSTREAMERROR_UNKNOWN                    ,
  DIOSTREAMERROR_URLNOTVALID                ,
  DIOSTREAMERROR_ADDRINUSE                  ,
};



#ifndef MICROCONTROLLER
#define DIOSTREAM_MAXBUFFER             1024*10  // 10k
#else
#define DIOSTREAM_MAXBUFFER             64
#endif


#define DIOSTREAM_SOMETHINGTOREAD       -1

#define DIOSTREAM_TIMEINWAITFUNCTIONS   10       // milliseconds

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XBUFFER;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class CIPHER;


class DIOSTREAM : public XSUBJECT
{
  public:
                                DIOSTREAM                       ();
    virtual                    ~DIOSTREAM                       ();

    virtual DIOSTREAMCONFIG*    GetConfig                       ()                            = 0;
    virtual bool                SetConfig                       (DIOSTREAMCONFIG* config)     = 0;

    virtual DIOSTREAMSTATUS     GetConnectStatus                ()                                { return status;                                                              }

    virtual bool                Open                            ()                            = 0;

    bool                        IsConnected                     ();
    bool                        IsDisconnected                  ();

    bool                        WaitToConnected                 (int timeout);
    bool                        WaitToDisconnected              (int timeout);

    virtual XDWORD              Read                            (XBYTE* buffer, XDWORD size);
    virtual XDWORD              Write                           (XBYTE* buffer, XDWORD size);

    XDWORD                      Read                            (XBUFFER& xbuffer);
    XDWORD                      Write                           (XBUFFER& xbuffer);

    virtual bool                WaitToFilledReadingBuffer       (int filledto = DIOSTREAM_SOMETHINGTOREAD, int timeout = XTIMER_INFINITE);

    bool                        ReadStr                         (char* str);
    bool                        ReadStr                         (XSTRING& string);
    bool                        ReadStr                         (XSTRING& string, int timeout);

    bool                        WriteStr                        (const char* str);
    bool                        WriteStr                        (XCHAR* string);
    bool                        WriteStr                        (XSTRING& string);

    bool                        IsBlockRead                     ()                                { return isblockread;                                                         }
    void                        SetIsBlockRead                  (bool isblockread)                { this->isblockread = isblockread;                                            }

    bool                        IsBlockWrite                    ()                                { return isblockwrite;                                                        }
    void                        SetIsBlockWrite                 (bool isblockwrite)               { this->isblockwrite = isblockwrite;                                          }

    virtual bool                Disconnect                      ()                                { return false;                                                               }

    virtual bool                Close                           ()                            = 0;

    DIOSTREAMTYPE               GetType                         ()                                { return type;                                                                }
    void                        SetType                         (DIOSTREAMTYPE type)              { this->type = type;                                                          }

    XBUFFER*                    GetInXBuffer                    ()                                { return inbuffer;                                                            }
    XBUFFER*                    GetOutXBuffer                   ()                                { return outbuffer;                                                           }

    bool                        ResetXBuffers                   ();
    bool                        ResetInXBuffer                  ();
    bool                        ResetOutXBuffer                 ();

    bool                        IsFlushOutXBuffer               ();
    bool                        IsFlushXBuffers                 ();

    bool                        WaitToFlushOutXBuffer           (int timeout = XTIMER_INFINITE);
    bool                        WaitToFlushXBuffers             (int timeout = XTIMER_INFINITE);

    virtual XTIMER*             GetXTimerConnexion              ()                                { return xtimerconnexion;                                                     }
    virtual XTIMER*             GetXTimerNotActivity            ()                                { return xtimernotactivity;                                                   }

    virtual XDWORD              GetNBytesRead                   ()                                { return nbytesread;                                                          }
    virtual XDWORD              GetNBytesWrite                  ()                                { return nbyteswrite;                                                         }

    bool                        ResetConnexionStatistics        ();

    DIOSTREAMERROR              GetLastDIOError                 ()                                { DIOSTREAMERROR ret = error; this->error = DIOSTREAMERROR_NONE; return ret;  }
    void                        SetLastDIOError                 (DIOSTREAMERROR error)            { this->error = error;                                                        }

    void                        Wait                            (int milliseconds)                { xsleep->MilliSeconds(milliseconds);                                         }

    XBUFFER*                    inbuffer;
    XBUFFER*                    outbuffer;

  protected:

    DIOSTREAMTYPE               type;

    DIOSTREAMSTATUS             status;
    DIOSTREAMERROR              error;

    XTIMER*                     xtimerconnexion;
    XTIMER*                     xtimernotactivity;
    XTIMER*                     xtimerout;

    XMUTEX*                     xmutextimerout;

    bool                        isblockread;
    bool                        isblockwrite;

    XDWORD                      nbytesread;
    XDWORD                      nbyteswrite;

  private:

    void                        Clean                           ()
                                {
                                  type                = DIOSTREAMTYPE_UNKNOWN;

                                  status              = DIOSTREAMSTATUS_GETTINGCONNEXION;
                                  error               = DIOSTREAMERROR_NONE;

                                  xtimerconnexion     = NULL;
                                  xtimernotactivity   = NULL;
                                  xtimerout           = NULL;

                                  xmutextimerout      = NULL;

                                  inbuffer            = NULL;
                                  outbuffer           = NULL;

                                  isblockread         = false;
                                  isblockwrite        = false;

                                  nbytesread          = 0;
                                  nbyteswrite         = 0;
                                }

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif