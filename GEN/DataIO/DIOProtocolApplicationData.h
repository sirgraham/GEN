/*------------------------------------------------------------------------------------------
//  DIOPROTOCOLAPPLICATIONDATA.H
*/
/**
// \class
//
//  DIO Protocol Aplication Data Class
//
//  @author  Abraham J. Velez
//  @version 02/03/2012 10:25:01
*/
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOPROTOCOLAPPLICATIONDATA_H_
#define _DIOPROTOCOLAPPLICATIONDATA_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

#ifdef DIOALERTS_ACTIVE
#include "DIOAlerts.h"
#endif

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class DIOPROTOCOLAPPLICATIONDATA
{
  public:
                                        DIOPROTOCOLAPPLICATIONDATA      ()
                                        {
                                          Clean();

                                          xmutexalert = xfactory->Create_Mutex();

                                        }

    virtual                            ~DIOPROTOCOLAPPLICATIONDATA      ()
                                        {
                                          if(xmutexalert) xfactory->Delete_Mutex(xmutexalert);

                                          Clean();
                                        }

    XWORD                               protocolversion;
    XWORD                               protocolsubversion;
    XWORD                               protocolsubversionerr;

    XWORD                               applicationversion;
    XWORD                               applicationsubversion;
    XWORD                               applicationsubversionerr;

    #ifdef DIOALERTS_ACTIVE
    bool                                AddAlert                            (DIOALERT& alert)
                                        {
                                          bool status = false;

                                          if(xmutexalert) xmutexalert->Lock();

                                          DIOALERT* _alert = new DIOALERT();
                                          if(_alert)
                                            {
                                              _alert->CopyFrom(&alert);
                                              status = alerts.Add(_alert);
                                              if(!status) delete _alert;
                                            }

                                          if(xmutexalert) xmutexalert->UnLock();

                                          return status;

                                        }

    bool                                ExtractAlert                        (int index, DIOALERT& alert)
                                        {
                                          bool status = false;

                                          if(xmutexalert) xmutexalert->Lock();

                                          DIOALERT* _alert = alerts.Get(index);
                                          if(_alert)
                                            {
                                              status = alert.CopyFrom(_alert);
                                              if(status) alerts.Delete(_alert);
                                            }

                                          if(xmutexalert) xmutexalert->UnLock();

                                          return status;
                                        }

    bool                                DeleteAllAlerts                     ()
                                        {
                                          bool status = false;

                                          if(xmutexalert) xmutexalert->Lock();

                                          if(!alerts.IsEmpty())
                                            {
                                              alerts.DeleteContents();
                                              alerts.DeleteAll();

                                              status = true;
                                            }

                                          if(xmutexalert) xmutexalert->UnLock();

                                          return status;
                                        }
    #endif

  private:

    void                                Clean                               ()
                                        {
                                          protocolversion                 = 0;
                                          protocolsubversion              = 0;
                                          protocolsubversionerr           = 0;

                                          applicationversion              = 0;
                                          applicationsubversion           = 0;
                                          applicationsubversionerr        = 0;

                                          xmutexalert                     = NULL;

                                        }


    XMUTEX*                             xmutexalert;

    #ifdef DIOALERTS_ACTIVE
    XVECTOR<DIOALERT*>                  alerts;
    #endif
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

