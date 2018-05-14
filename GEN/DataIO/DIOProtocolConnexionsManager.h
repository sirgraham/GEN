/*------------------------------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER.H
*/
/**
// \class
//
//  Data I/O Protocol Connexions Manager
//
//  @author  Abraham J. Velez
//  @version 30/01/2012 12:05:03
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOPROTOCOLCONNEXIONSMANAGER_H_
#define _DIOPROTOCOLCONNEXIONSMANAGER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "XEvent.h"
#include "XObserver.h"
#include "XSubject.h"

#include "DIOURL.h"
#include "DIOProtocol.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE
{
  DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_UNKNOWN                      = XEVENTTYPE_PROTOCOLCONNEXIONS ,
  DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_CONNECTEDCONNEXION                                           ,
  DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_INITPROTOCOL                                                 ,
  DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION                                        ,
};


#define DIOPROTOCOLCONNEXIONS_DEFAULTIMETRYCONNEXIONS         8    // Seconds
#define DIOPROTOCOLCONNEXIONS_DEFAULTIMECHECKCONNEXIONS      60    // Seconds
#define DIOPROTOCOLCONNEXIONS_UNLIMITEDNCONNEXIONS           -1



/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XTIMER;
class XPUBLISHER;
class XTHREADCOLLECTED;
class CIPHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class DIOSTREAMTCPIPCONFIG;
class DIOSTREAM;
class DIOPROTOCOL;
class DIOPROTOCOLCONNEXION;
class DIOPROTOCOLCONNEXIONSMANAGER;
class DIOSTREAMENUMSERVERS;

class DIOPROTOCOLCONNEXIONSMANAGERXEVENT : public XEVENT
{
  public:
                                        DIOPROTOCOLCONNEXIONSMANAGERXEVENT            (XSUBJECT* subject, XDWORD type = DIOPROTOCOLXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_PROTOCOLCONNEXIONS)   : XEVENT(subject, type, family)
                                        {
                                          Clean();
                                        }

    virtual                            ~DIOPROTOCOLCONNEXIONSMANAGERXEVENT            ()                                                                  { Clean();                                                              }


    DIOSTREAMENUMSERVERS*               GetDIOStreamEnumServers                       ()                                                                  { return diostreamenumservers;                                          }
    void                                SetDIOStreamEnumServers                       (DIOSTREAMENUMSERVERS* diostreamenumservers)                        { this->diostreamenumservers = diostreamenumservers;                    }

    DIOSTREAMCONFIG*                    GetDIOStreamConfig                            ()                                                                  { return diostreamcfg;                                                  }
    void                                SetDIOStreamConfig                            (DIOSTREAMCONFIG* diostreamcfg)                                     { this->diostreamcfg = diostreamcfg;                                    }

    DIOPROTOCOL*                        GetDIOProtocol                                ()                                                                  { return dioprotocol;                                                   }
    void                                SetDIOProtocol                                (DIOPROTOCOL* dioprotocol)                                          { this->dioprotocol = dioprotocol;                                      }

    DIOPROTOCOLCONNEXION*               GetProtocolConnexion                          ()                                                                  { return protocolconnexion;                                             }
    void                                SetProtocolConnexion                          (DIOPROTOCOLCONNEXION* protocolconnexion)                           { this->protocolconnexion = protocolconnexion;                          }

    DIOPROTOCOLCONNEXIONSMANAGER*       GetProtocolConnexionsManager                  ()                                                                  { return protocolconnexionsmanager;                                     }
    void                                SetProtocolConnexionsManager                  (DIOPROTOCOLCONNEXIONSMANAGER* protocolconnexionsmanager)           { this->protocolconnexionsmanager = protocolconnexionsmanager;          }

    bool                                IsInitialized                                 ()                                                                  { return isinitialized;                                                 }
    void                                SetIsInitialized                              (bool isinitialized)                                                { this->isinitialized = isinitialized;                                  }

  private:

    void                                Clean                                         ()
                                        {
                                          diostreamenumservers      = NULL;
                                          diostreamcfg              = NULL;
                                          dioprotocol               = NULL;
                                          protocolconnexion         = NULL;
                                          protocolconnexionsmanager = NULL;
                                          isinitialized             = false;
                                        }

    DIOSTREAMENUMSERVERS*               diostreamenumservers;
    DIOSTREAMCONFIG*                    diostreamcfg;
    DIOPROTOCOL*                        dioprotocol;
    DIOPROTOCOLCONNEXION*               protocolconnexion;
    DIOPROTOCOLCONNEXIONSMANAGER*       protocolconnexionsmanager;
    bool                                isinitialized;
};



class DIOPROTOCOLCONNEXION
{
  public:
                                        DIOPROTOCOLCONNEXION                          (DIOSTREAMCONFIG* diostreamcfg);
                                       ~DIOPROTOCOLCONNEXION                          ();

    DIOSTREAMCONFIG*                    GetDIOStreamConfig                            ()                                                                  { return diostreamcfg;                                  }
    DIOPROTOCOL*                        GetDIOProtocol                                ()                                                                  { return dioprotocol;                                   }
    void                                SetDIOProtocol                                (DIOPROTOCOL* protocol)                                             { this->dioprotocol = protocol;                         }

    bool                                Connect                                       ();
    bool                                Disconected                                   ();

    bool                                SetInUse                                      (bool inuse, XDWORD ID)
                                        {
                                          if(!xmutexinuseID)  return false;

                                          xmutexinuseID->Lock();

                                          if(inuse)
                                                inuseID.Add(ID);
                                           else inuseID.Delete(ID);

                                          xmutexinuseID->UnLock();

                                          return true;
                                        }

    bool                                IsInUse                                       ()
                                        {
                                          bool inuse = false;

                                          if(!xmutexinuseID)

                                          xmutexinuseID->Lock();
                                          inuse = (inuseID.GetSize()?true:false);
                                          xmutexinuseID->UnLock();

                                          return inuse;
                                        }


    bool                                IsSendEventConnected                          ()                                                                  { return issendeventconnected;                          }
    void                                SetIsSendEventConnected                       (bool issendeventconnected)                                         { this->issendeventconnected = issendeventconnected;    }

  protected:

    DIOSTREAMCONFIG*                    diostreamcfg;

    DIOPROTOCOL*                        dioprotocol;

    bool                                issendeventconnected;

    XMUTEX*                             xmutexinuseID;
    XVECTOR<XDWORD>                     inuseID;


  private:

    void                                Clean                                         ()
                                        {
                                          diostreamcfg            = NULL;

                                          dioprotocol             = NULL;

                                          issendeventconnected    = false;

                                          xmutexinuseID           = NULL;
                                        }
};



class DIOPROTOCOLCONNEXIONSMANAGER : public XSUBJECT
{
  public:
                                        DIOPROTOCOLCONNEXIONSMANAGER                  ();
                                       ~DIOPROTOCOLCONNEXIONSMANAGER                  ();

    bool                                Ini                                           (bool isserver, DIOSTREAMCONFIG* diostreamcfg, DIOSTREAMENUMSERVERS* diostreamenumservers = NULL);
    bool                                End                                           ();

    virtual DIOPROTOCOL*                CreateProtocol                                ()                      = 0;
    virtual bool                        DeleteProtocol                                (DIOPROTOCOL* protocol) = 0;

    int                                 TargetURL_GetNTargets                         ()                                                                  { return targetURLs.GetSize();                                }
    bool                                TargetURL_Add                                 (XCHAR* URL);
    bool                                TargetURL_Add                                 (XSTRING& URL)                                                      { return TargetURL_Add(URL.Get());                            }
    bool                                TargetURL_Add                                 (DIOURL& URL)                                                       { return TargetURL_Add(URL.Get());                            }
    XSTRING*                            TargetURL_Get                                 (int index);
    bool                                TargetURL_Delete                              (int index);
    bool                                TargetURL_DeleteAll                           ();

    int                                 ProtocolConnexions_GetNLimit                  ()                                                                  { return protocolconnexionsnlimit;                            }
    void                                ProtocolConnexions_SetNLimit                  (int protocolconnexionsnlimit)                                      { this->protocolconnexionsnlimit = protocolconnexionsnlimit;  }
    int                                 ProtocolConnexions_GetNAvailable              ();
    DIOPROTOCOLCONNEXION*               ProtocolConnexions_Get                        (int index);
    DIOPROTOCOLCONNEXION*               ProtocolConnexions_GetFirstConnected          ();
    DIOPROTOCOLCONNEXION*               ProtocolConnexions_GetFirstOperative          ();
    virtual DIOPROTOCOLCONNEXION*       ProtocolConnexions_GetByDIOStream             (DIOSTREAM* diostream);
    DIOPROTOCOL*                        ProtocolConnexions_GetProtocol                (int index = 0);
    int                                 ProtocolConnexions_GetNConnected              ();
    int                                 ProtocolConnexions_GetNFreeToConnect          ();
    bool                                ProtocolConnexions_SendEventConnected         ();
    bool                                ProtocolConnexions_DeleteDisconnected         ();

    bool                                ProtocolConnexions_DeleteAllWaitConnections   ();
    bool                                ProtocolConnexions_Disconnect                 (int index);
    bool                                ProtocolConnexions_DisconnectAll              ();
    bool                                ProtocolConnexions_Delete                     (int index);
    bool                                ProtocolConnexions_DeleteAll                  ();

    bool                                WaitToAnyConnexionIsConnected                 (int timeout);
    bool                                WaitToAllConnexionsCanBeDeleted               (int timeout = 0);

    bool                                SendEvent                                     (DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE type, DIOPROTOCOLCONNEXION* protocolconnexion);

    void                                GetApplicationVersion           (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
                                        {
                                          version         = this->applicationversion;
                                          subversion      = this->applicationsubversion;
                                          subversionerr   = this->applicationsubversionerr;
                                        }

     void                               SetApplicationVersion           (XDWORD version, XDWORD subversion, XDWORD subversionerr)
                                        {
                                          this->applicationversion        = version;
                                          this->applicationsubversion     = subversion;
                                          this->applicationsubversionerr  = subversionerr;
                                        }

  protected:


    DIOSTREAMENUMSERVERS*               diostreamenumservers;
    DIOSTREAMCONFIG*                    diostreamcfg;

    XWORD                               applicationversion;
    XWORD                               applicationsubversion;
    XWORD                               applicationsubversionerr;

    bool                                isserver;
    int                                 protocolconnexionsnlimit;
    XVECTOR<DIOPROTOCOLCONNEXION*>      protocolconnexions;


  private:

    void                                Clean                                         ()
                                        {
                                          diostreamenumservers          = NULL;
                                          diostreamcfg                  = NULL;

                                          applicationversion            = 0;
                                          applicationsubversion         = 0;
                                          applicationsubversionerr      = 0;

                                          isserver                      = false;
                                          protocolconnexionsnlimit      = 0;

                                          xthreadconnexions             = NULL;
                                          xtimerconnexions              = NULL;
                                          xtimerclienttry               = NULL;
                                          xtimerout                     = NULL;

                                          xmutexprocotolconnexions      = NULL;
                                        }


    void                                ManageProtocolConnexionsServer                ();
    void                                ManageProtocolConnexionsClient                ();

    int                                 GetNConnexionsToConnect                       ();
    bool                                DeleteConnexionsDisconnected                  ();

    static void                         ThreadProtocolConnexions                      (void* param);

    XTHREADCOLLECTED*                   xthreadconnexions;
    XTIMER*                             xtimerconnexions;
    XTIMER*                             xtimerclienttry;
    XTIMER*                             xtimerout;

    XMUTEX*                             xmutexprocotolconnexions;

    XVECTOR<DIOURL*>                    targetURLs;
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

