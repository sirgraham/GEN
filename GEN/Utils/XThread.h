//------------------------------------------------------------------------------------------
//  XTHREAD.H
//
/**
// \class
//
//  Thread for Utils
//
//  @author  Abraham J. Velez
//  @version 06/03/2006 11:41:39
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XTHREAD_H_
#define _XTHREAD_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XString.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XTHREADSTATUS
{
  //XTHREADSTATUS_UNKNOWN                     = 0 ,
  XTHREADSTATUS_NONE                            ,
  XTHREADSTATUS_EXIT                            ,
  XTHREADSTATUS_STOP                            ,
  XTHREADSTATUS_RUN                             ,
  XTHREADSTATUS_END
};


enum XTHREADPRIORITY
{
  XTHREADPRIORITY_UNKNOWN                   = 0 ,
  XTHREADPRIORITY_LOW                           ,
  XTHREADPRIORITY_NORMAL                        ,
  XTHREADPRIORITY_HIGH                          ,
  XTHREADPRIORITY_REALTIME                      ,
};


enum XTHREADGROUPID
{
  XTHREADGROUPID_UNKNOWN                    = 0 ,
  XTHREADGROUPID_UNGROUP                        ,
  XTHREADGROUPID_XTIMERCLOCK                    ,
  XTHREADGROUPID_SCHEDULER                      ,
  XTHREADGROUPID_SCRIPT                         ,
  XTHREADGROUPID_DIOSTREAM                      ,
  XTHREADGROUPID_DIOSTREAMUART                  ,
  XTHREADGROUPID_DIOSTREAMUDP                   ,
  XTHREADGROUPID_DIOSTREAMTCPIP                 ,
  XTHREADGROUPID_DIOSTREAMWIFI                  ,
  XTHREADGROUPID_DIOSTREAMBLUETOOTH             ,
  XTHREADGROUPID_DIOSTREAMUSB                   ,
  XTHREADGROUPID_DIOSTREAMICMP                  ,
  XTHREADGROUPID_DIOSTREAMI2C                   ,
  XTHREADGROUPID_DIOSTREAMSPI                   ,
  XTHREADGROUPID_DIOSTREAMCIPHER                ,
  XTHREADGROUPID_DIOSSHREVERSE                  ,
  XTHREADGROUPID_DIOCHECKCONNEXIONS             ,
  XTHREADGROUPID_DIOPROTOCOL                    ,
  XTHREADGROUPID_DIOPROTOCOLCONNEXIONMANAGER    ,
  XTHREADGROUPID_DIOWEBSERVER                   ,
  XTHREADGROUPID_DIOATCMS                       ,
  XTHREADGROUPID_DIOALERTS                      ,
  XTHREADGROUPID_DIOPCAP                        ,
  XTHREADGROUPID_VIDEO                          ,
  XTHREADGROUPID_GRPLOADER                      ,

  XTHREADGROUPID_APPOWNER
};


#define XTHREAD_TIMEOUT                     60   // seconds
#define XTHREAD_DEFAULTWAITYIELD            10   // milliseconds

#define XTHREAD_DEFAULTTIMEOUTEND           10   // seconds

typedef void (*XTHREADFUNCTION)(void* data);

//---- CLASS -------------------------------------------------------------------------------


class XMUTEX
{
  public:
                                  XMUTEX            ()                              { Clean();                            }
      virtual                    ~XMUTEX            ()                              { Clean();                            }

     bool                         IsLock            ()                              { return islock;                      }

     virtual bool                 Lock              ()                              { return false;                       }
     virtual bool                 UnLock            ()                              { return false;                       }

  protected:

     bool                         islock;

  private:

    void                          Clean             ()                              { islock = false;                     }

};



class XTHREAD
{
  public:
                                  XTHREAD           (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function = NULL, void* data = NULL);
     virtual                     ~XTHREAD           ();

     virtual bool                 Ini               (bool run = true)               { return false;                        }
     virtual bool                 Wait              (int miliseconds =  XTHREAD_DEFAULTWAITYIELD)
                                  {
                                    return false;
                                  }
     virtual bool                 End               ()                              { return false;                        }

     bool                         IsRunning         ()
                                  {
                                    if(statusfunc == XTHREADSTATUS_RUN) return true;
                                    return false;
                                  }

     bool                         Run               (bool activate)
                                  {
                                    if(activate)
                                      {
                                        if(IsRunning()) return false;
                                        statusfunc = XTHREADSTATUS_RUN;
                                      }
                                     else
                                      {
                                        if(!IsRunning()) return false;
                                        statusfunc = XTHREADSTATUS_STOP;
                                      }

                                    return true;
                                  }

     virtual bool                 WaitToEnd         (XDWORD timeout = XTHREAD_DEFAULTTIMEOUTEND);

     XDWORD                       GetWaitYield      ()                              { return waityield;                   }
     void                         SetWaitYield      (XDWORD waityield = XTHREAD_DEFAULTWAITYIELD)
                                  {
                                    this->waityield = waityield;
                                  }


     bool                         Exit              ()
                                  {
                                    statusfunc = XTHREADSTATUS_EXIT;
                                    return true;
                                  }

     XTHREADGROUPID               GetGroupID        ()                              { return groupID;                     }
     void                         SetGroupID        (XTHREADGROUPID groupid)        { this->groupID = groupID;            }

     XSTRING*                     GetID             ()                              { return &ID;                         }
     XTHREADFUNCTION              GetFunction       ()                              { return function;                    }
     void*                        GetParam          ()                              { return param;                       }

     XTHREADPRIORITY              GetPriority       ()                              { return priority;                    }
     void                         SetPriority       (XTHREADPRIORITY priority)      { this->priority = priority;          }

     XDWORD                       GetStackSize      ()                              { return stacksize;                   }
     void                         SetStackSize      (XDWORD stacksize)              { this->stacksize = stacksize;        }

     bool                         IsInFunction      ()                              { return isinfunction;                }
     XTHREADSTATUS                GetStatusFunc     ()                              { return statusfunc;                  }

  protected:

     XTHREADGROUPID               groupID;
     XSTRING                      ID;
     XTHREADFUNCTION              function;
     void*                        param;

     XDWORD                       waityield;

     XTHREADPRIORITY              priority;
     XDWORD                       stacksize;

     bool                         gotofunction;
     bool                         isinfunction;
     XTHREADSTATUS                statusfunc;

  private:

    void                          Clean             ()
                                  {
                                    groupID         = XTHREADGROUPID_UNKNOWN;
                                    ID.Empty();
                                    function        = NULL;
                                    param           = NULL;

                                    waityield       = XTHREAD_DEFAULTWAITYIELD;

                                    priority        = XTHREADPRIORITY_UNKNOWN;
                                    stacksize       = 0;

                                    gotofunction    = false;
                                    isinfunction    = false;
                                    statusfunc      = XTHREADSTATUS_NONE;
                                  }
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

