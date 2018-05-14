//------------------------------------------------------------------------------------------
//  DIOWEBSERVERXEVENT.H
//
/**
// \class
//
//  WEB Server Event class
//
//  @author  Abraham J. Velez
//  @version 20/01/2007 21:05:48
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOWEBSERVERXEVENT_H_
#define _DIOWEBSERVERXEVENT_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XEvent.h"
#include "XString.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOWEBSERVERXEVENTTYPE
{
  DIOWEBSERVERXEVENTTYPE_UNKNOWN                 = XEVENTTYPE_WEBSERVER ,
  DIOWEBSERVERXEVENTTYPE_REQUEST                                        ,
  DIOWEBSERVERXEVENTTYPE_TAGFOUND                                       ,
};

//---- CLASS -------------------------------------------------------------------------------

class XSUBJECT;
class DIOWEBSERVER_REQUEST;
class DIOWEBSERVER_CONNEXION;
class DIOWEBSERVER;


class DIOWEBSERVERXEVENT : public XEVENT
{
  public:
                                    DIOWEBSERVERXEVENT                (XSUBJECT* subject, XDWORD type = DIOWEBSERVERXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_WEBSERVER) : XEVENT(subject, type, family)
                                    {
                                      Clean();
                                    }

    virtual                        ~DIOWEBSERVERXEVENT                ()                                                                                        { Clean();                      }

    DIOWEBSERVER*                   GetServer                         ()                                                                                        { return server;                }
    void                            SetServer                         (DIOWEBSERVER* server)                                                                    { this->server = server;        }

    DIOWEBSERVER_CONNEXION*         GetConnexion                      ()                                                                                        { return connexion;             }
    void                            SetConnexion                      (DIOWEBSERVER_CONNEXION* connexion)                                                       { this->connexion = connexion;  }

    DIOWEBSERVER_REQUEST*           GetRequest                        ()                                                                                        { return request;               }
    void                            SetRequest                        (DIOWEBSERVER_REQUEST* request)                                                           { this->request = request;      }

    XSTRING*                        GetDataTAG                        ()                                                                                        { return &dataTAG;              }

    int                             GetTimeOut                        ()                                                                                        { return timeout;               }
    void                            SetTimeOut                        (int timeout)                                                                             { this->timeout = timeout;      }

    XSTRING*                        GetOutput                         ()                                                                                        { return &output;               }

  private:

    void                            Clean                             ()
                                    {
                                      server    = NULL;
                                      connexion = NULL;
                                      request   = NULL;

                                      timeout   = 0;
                                    }

    DIOWEBSERVER*                   server;
    DIOWEBSERVER_CONNEXION*         connexion;
    DIOWEBSERVER_REQUEST*           request;

    XSTRING                         dataTAG;
    XSTRING                         output;
    int                             timeout;
};






//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

