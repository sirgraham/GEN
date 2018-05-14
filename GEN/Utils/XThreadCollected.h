//------------------------------------------------------------------------------------------
//  XTHREADCOLLECTED.H
//
/**
// \class
//
//  Thread Collection for Utils
//
//  @author  Abraham J. Velez
//  @version 06/03/2006 11:41:39
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifdef XTHREADCOLLECTION_ACTIVE

#ifndef _XTHREADCOLLECTION_H_
#define _XTHREADCOLLECTION_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XString.h"
#include "XVector.h"
#include "XTimer.h"
#include "XThread.h"
#include "XDebug.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define CREATEXTHREAD             XTHREADSCOLLECTEDMANAGER::GetInstance().CreateThread
#define DELETEXTHREAD             XTHREADSCOLLECTEDMANAGER::GetInstance().DeleteThread
#define ACTIVATEXTHREADGROUP      XTHREADSCOLLECTEDMANAGER::GetInstance().ActivateGroup
#define DEACTIVATEXTHREADGROUP    XTHREADSCOLLECTEDMANAGER::GetInstance().DeactivateGroup

//---- CLASS -------------------------------------------------------------------------------


class XTHREADCOLLECTED
{
  public:
                                            XTHREADCOLLECTED                ()
                                            {
                                              Clean();

                                              xtimerout = xfactory->CreateTimer();
                                            }

                                            XTHREADCOLLECTED                (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
                                            {
                                              Clean();

                                              directxthread = xfactory->CreateThread(groupID, ID, function, param);
                                              if(directxthread)
                                                {
                                                  this->ID       = ID;
                                                  this->function = function;
                                                  this->param    = param;
                                                }
                                            }

    virtual                                ~XTHREADCOLLECTED                ()
                                            {
                                              if(xtimerout)     xfactory->DeleteTimer(xtimerout);
                                              if(directxthread) xfactory->DeleteThread(groupID, directxthread);

                                              Clean();
                                            }

    bool                                    Ini                             (bool run = true)
                                            {
                                              if(directxthread) return directxthread->Ini(run);

                                              if(run)
                                                {
                                                  if(!Run(true)) return false;
                                                }
                                               else
                                                {
                                                  status = XTHREADSTATUS_STOP;
                                                }

                                              return true;
                                            }

    void                                    Wait                            (int milliseconds = XTHREAD_DEFAULTWAITYIELD)
                                            {
                                              if(directxthread)
                                                {
                                                  directxthread->Wait(milliseconds);
                                                  return;
                                                }

                                              xsleep->MilliSeconds(milliseconds);
                                            }

    bool                                    End                             ()
                                            {
                                              if(directxthread) return directxthread->End();

                                              if(status == XTHREADSTATUS_END) return false;
                                              status = XTHREADSTATUS_EXIT;

                                              WaitToEnd();

                                              return true;
                                            }

    bool                                    IsRunning                       ()
                                            {
                                              if(directxthread) return directxthread->IsRunning();

                                              if(status == XTHREADSTATUS_RUN) return true;
                                              return false;
                                            }

    bool                                    Run                             (bool activate)
                                            {
                                              if(directxthread) return directxthread->Run(activate);

                                              if(activate)
                                                {
                                                  if(IsRunning()) return false;
                                                  status = XTHREADSTATUS_RUN;
                                                }
                                               else
                                                {
                                                  if(!IsRunning()) return false;
                                                  status = XTHREADSTATUS_STOP;
                                                }

                                              return true;
                                            }

    bool                                    Exit                            ()
                                            {
                                              if(directxthread) return directxthread->Exit();

                                              status = XTHREADSTATUS_EXIT;
                                              return true;
                                            }

    XTHREADGROUPID                          GetGroupID        ()                                    { return groupID;                                                         }
    void                                    SetGroupID        (XTHREADGROUPID groupID)              { this->groupID = groupID;                                                }

    XSTRING*                                GetID                           ()
                                            {
                                              if(directxthread)
                                                {
                                                  return directxthread->GetID();
                                                }
                                               else
                                                {
                                                  return &ID;
                                                }
                                            }

    XTHREADFUNCTION                         GetFunction                     ()
                                            {
                                              if(directxthread)
                                                {
                                                  return directxthread->GetFunction();
                                                }
                                               else
                                                {
                                                  return function;
                                                }
                                            }

    void                                    SetFunction                     (XTHREADFUNCTION function)
                                            {
                                              this->function = function;
                                            }

    void*                                   GetParam                        ()
                                            {
                                              if(directxthread) return directxthread->GetParam();
                                              return param;
                                            }

    void                                    SetParam                        (void* param)
                                            {
                                              this->param = param;
                                            }

     XDWORD                                 GetWaitYield                    ()
                                            {
                                              if(directxthread) return directxthread->GetWaitYield();
                                              return waityield;
                                            }

     void                                   SetWaitYield                    (XDWORD waityield = XTHREAD_DEFAULTWAITYIELD)
                                            {
                                              if(directxthread) return directxthread->SetWaitYield(priority);
                                              this->waityield = waityield;
                                            }

    XTHREADPRIORITY                         GetPriority                     ()
                                            {
                                              if(directxthread) return directxthread->GetPriority();
                                              return priority;
                                            }

    void                                    SetPriority                     (XTHREADPRIORITY priority)
                                            {
                                              if(directxthread) return directxthread->SetPriority(priority);
                                              this->priority = priority;
                                            }

    XDWORD                                  GetStackSize                    ()                      { return stacksize;                                                       }
    void                                    SetStackSize                    (XDWORD stacksize)      { this->stacksize = stacksize;                                            }


    bool                                    IsInFunction                    ()
                                            {
                                              if(directxthread) return directxthread->IsInFunction();
                                              return isinfunction;
                                            }

    void                                    SetIsInFunction                 (bool isinfunction)     { this->isinfunction = isinfunction;                                      }

    XTHREADSTATUS                           GetStatus                       ()
                                            {
                                              if(directxthread) return directxthread->GetStatusFunc();
                                              //if(status == XTHREADSTATUS_EXIT) return XTHREADSTATUS_UNKNOWN; // #Imanol
                                              return status;
                                            }

    void                                    SetStatus                       (XTHREADSTATUS status)  { this->status = status;                                                  }

  private:

    void                                    Clean                           ()
                                            {
                                              groupID         = XTHREADGROUPID_UNKNOWN;
                                              ID.Empty();
                                              function        = NULL;
                                              param           = NULL;

                                              waityield       = XTHREAD_DEFAULTWAITYIELD;

                                              priority        = XTHREADPRIORITY_UNKNOWN;
                                              stacksize       = 0;

                                              isinfunction    = false;
                                              status          = XTHREADSTATUS_NONE;

                                              xtimerout       = NULL;

                                              directxthread   = NULL;
                                            }

    bool                                    WaitToEnd()
                                            {
                                              while(GetStatus() != XTHREADSTATUS_END)
                                               {
                                                 Wait();
                                               }

                                              return true;
                                            }

    XTHREADGROUPID                          groupID;
    XSTRING                                 ID;
    XTHREADFUNCTION                         function;
    void*                                   param;

    XDWORD                                  waityield;

    XTHREADPRIORITY                         priority;
    XDWORD                                  stacksize;

    bool                                    isinfunction;
    XTHREADSTATUS                           status;

    XTIMER*                                 xtimerout;
    XTHREAD*                                directxthread;
};




class XTHREADSCOLLECTED
{
  public:
                                            XTHREADSCOLLECTED               (XTHREADGROUPID groupID)
                                            {
                                              Clean();

                                              this->groupID = groupID;

                                              xthreadsvectormutex = xfactory->Create_Mutex();

                                              XSTRING stringID;

                                              GetGroupThreadIDString(stringID);

                                              if(xthreadsvectormutex)
                                                {
                                                  xthread = xfactory->CreateThread(groupID, stringID.Get(), ThreadRunFunction, (void*)this);
                                                  if(xthread) xthread->Ini();
                                                }
                                            }

                                            XTHREADSCOLLECTED               ()
                                            {
                                              Clean();
                                            }

    virtual                                ~XTHREADSCOLLECTED               ()
                                            {
                                              ExitAll();

                                              if(xthread)
                                                {
                                                  xthread->End();
                                                  xfactory->DeleteThread(groupID, xthread);
                                                }

                                              DeleteAll();

                                              if(xthreadsvectormutex) xfactory->Delete_Mutex(xthreadsvectormutex);

                                              Clean();
                                            }

    XTHREADGROUPID                          GetGroupID                      ()                                { return groupID;                                                           }
    void                                    SetGroupID                      (XTHREADGROUPID groupID)
                                            {
                                              this->groupID = groupID;
                                            }

    XTHREADCOLLECTED*                       Create                          (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
                                            {
                                              XTHREADCOLLECTED* xthreadcollected = NULL;

                                              if(xthread)
                                                {
                                                  if(!xthreadsvectormutex) return NULL;

                                                  xthreadsvectormutex->Lock();

                                                  xthreadcollected = new XTHREADCOLLECTED();
                                                  if(xthreadcollected)
                                                    {
                                                      xthreadcollected->SetGroupID(groupID);
                                                      xthreadcollected->GetID()->Set(ID);
                                                      xthreadcollected->SetFunction(function);
                                                      xthreadcollected->SetParam(param);

                                                      xthreadsvector.Add(xthreadcollected);
                                                    }

                                                  /*
                                                  #ifdef XDEBUG
                                                  XSTRING groupID;
                                                  GetGroupThreadIDString(groupID);
                                                  XDEBUG_PRINTCOLOR(XDEBUG_COLORINFO, __L("Create THREAD COLLECTED (%02d) grupo %s: %s"), xthreadsvector.GetSize(), groupID.Get(), ID);
                                                  #endif
                                                  */

                                                  xthreadsvectormutex->UnLock();
                                                }
                                               else
                                                {
                                                  xthreadcollected = new XTHREADCOLLECTED(groupID, ID, function, param);
                                                  if(xthreadcollected)
                                                    {

                                                    }
                                                }

                                              return xthreadcollected;
                                            }

    bool                                    Delete                          (XTHREADGROUPID groupID, XTHREADCOLLECTED* xthreadcollected)
                                            {
                                              bool status = false;

                                              if(xthread)
                                                {
                                                  if(!xthreadsvectormutex) return false;

                                                  xthreadsvectormutex->Lock();

                                                  XDWORD index = 0;

                                                  do{ XTHREADCOLLECTED* _xthreadcollected = xthreadsvector.Get(index);
                                                      if(xthreadcollected == _xthreadcollected)
                                                        {
                                                          /*
                                                          #ifdef XDEBUG
                                                          XSTRING groupID;
                                                          GetGroupThreadIDString(groupID);
                                                          XDEBUG_PRINTCOLOR(XDEBUG_COLORINFO, __L("Delete THREAD COLLECTED (%02d) grupo %s: %s"), xthreadsvector.GetSize(), groupID.Get(), xthreadcollected->GetID()->Get());
                                                          #endif
                                                          */

                                                          xthreadcollected->Exit();
                                                          xthreadsvector.Delete(xthreadcollected);
                                                          delete xthreadcollected;

                                                          status = true;

                                                          break;
                                                        }

                                                      index++;

                                                    } while(index < xthreadsvector.GetSize());

                                                  xthreadsvectormutex->UnLock();
                                                }
                                               else
                                                {
                                                  delete xthreadcollected;
                                                }

                                              return status;
                                            }

    bool                                    ExitAll                       ()
                                            {
                                              if(!xthreadsvectormutex) return false;

                                              xthreadsvectormutex->Lock();

                                              for(XDWORD c=0; c<xthreadsvector.GetSize(); c++)
                                                {
                                                  XTHREADCOLLECTED* xthreadcollected  = xthreadsvector.Get(c);
                                                  if(xthreadcollected) xthreadcollected->Exit();
                                                }

                                              xthreadsvectormutex->UnLock();

                                              return true;
                                            }

    bool                                    DeleteAll                       ()
                                            {
                                              if(!xthreadsvectormutex) return false;

                                              xthreadsvectormutex->Lock();

                                              xthreadsvector.DeleteContents();
                                              xthreadsvector.DeleteAll();

                                              xthreadsvectormutex->UnLock();

                                              return true;
                                            }

  private:

    void                                    Clean                           ()
                                            {
                                              groupID             = XTHREADGROUPID_UNKNOWN;
                                              xthreadsvectormutex = NULL;
                                              xthread             = NULL;
                                            }

    static void                             ThreadRunFunction               (void* param)
                                            {
                                              XTHREADSCOLLECTED* xthreadscollected = (XTHREADSCOLLECTED*)param;
                                              if(!xthreadscollected) return;

                                              if(!xthreadscollected->xthreadsvectormutex) return;

                                              xthreadscollected->xthreadsvectormutex->Lock();

                                              for(XDWORD c=0; c<xthreadscollected->xthreadsvector.GetSize(); c++)
                                                {
                                                  XTHREADCOLLECTED* xthreadcollected  = xthreadscollected->xthreadsvector.Get(c);
                                                  if(xthreadcollected)
                                                    {
                                                      switch(xthreadcollected->GetStatus())
                                                        {
                                                          case XTHREADSTATUS_RUN    : xthreadcollected->SetIsInFunction(true);
                                                                                      xthreadcollected->GetFunction()(xthreadcollected->GetParam());
                                                                                      xthreadcollected->SetIsInFunction(false);

                                                                                      xthreadscollected->xthread->Wait();
                                                                                      break;

                                                          case XTHREADSTATUS_EXIT   : xthreadcollected->SetStatus(XTHREADSTATUS_END);
                                                                                      break;
                                                        }
                                                    }
                                                }

                                              xthreadscollected->xthreadsvectormutex->UnLock();
                                            }

    bool                                    GetGroupThreadIDString          (XSTRING& stringID)
                                            {
                                              stringID = __L("TGRP_");

                                              switch(groupID)
                                                {
                                                  case XTHREADGROUPID_UNKNOWN                     : stringID.Add(__L("UNKNOWN"));                       break;
                                                  case XTHREADGROUPID_SCHEDULER                   : stringID.Add(__L("SCHEDULE"));                      break;
                                                  case XTHREADGROUPID_SCRIPT                      : stringID.Add(__L("SCRIPT"));                        break;
                                                  case XTHREADGROUPID_DIOSTREAM                   : stringID.Add(__L("DIOSTREAM"));                     break;
                                                  case XTHREADGROUPID_DIOSTREAMUART               : stringID.Add(__L("DIOSTREAMUART"));                 break;
                                                  case XTHREADGROUPID_DIOSTREAMUDP                : stringID.Add(__L("DIOSTREAMUDP"));                  break;
                                                  case XTHREADGROUPID_DIOSTREAMTCPIP              : stringID.Add(__L("DIOSTREAMTCPIP"));                break;
                                                  case XTHREADGROUPID_DIOSTREAMWIFI               : stringID.Add(__L("DIOSTREAMWIFI"));                 break;
                                                  case XTHREADGROUPID_DIOSTREAMBLUETOOTH          : stringID.Add(__L("DIOSTREAMBLUETOOTH"));            break;
                                                  case XTHREADGROUPID_DIOSTREAMUSB                : stringID.Add(__L("DIOSTREAMUSB"));                  break;
                                                  case XTHREADGROUPID_DIOSTREAMICMP               : stringID.Add(__L("DIOSTREAMICMP"));                 break;
                                                  case XTHREADGROUPID_DIOSTREAMSPI                : stringID.Add(__L("DIOSTREAMSPI"));                  break;
                                                  case XTHREADGROUPID_DIOSTREAMCIPHER             : stringID.Add(__L("DIOSTREAMCIPHER"));               break;
                                                  case XTHREADGROUPID_DIOCHECKCONNEXIONS          : stringID.Add(__L("DIOCHECKCONNEXIONS"));            break;
                                                  case XTHREADGROUPID_DIOPROTOCOL                 : stringID.Add(__L("DIOPROTOCOL"));                   break;
                                                  case XTHREADGROUPID_DIOPROTOCOLCONNEXIONMANAGER : stringID.Add(__L("DIOPROTOCOLCONNEXIONMANAGER"));   break;
                                                  case XTHREADGROUPID_DIOWEBSERVER                : stringID.Add(__L("DIOWEBSERVER"));                  break;
                                                  case XTHREADGROUPID_DIOATCMS                    : stringID.Add(__L("DIOATCMS"));                      break;
                                                  case XTHREADGROUPID_DIOALERTS                   : stringID.Add(__L("DIOALERTS"));                     break;
                                                  case XTHREADGROUPID_DIOPCAP                     : stringID.Add(__L("DIOPCAP"));                       break;
                                                  case XTHREADGROUPID_VIDEO                       : stringID.Add(__L("VIDEO"));                         break;
                                                  case XTHREADGROUPID_APPOWNER                    :
                                                                          default                 : { XSTRING IDlocal;
                                                                                                      IDlocal.Format(__L("APPOWNER[%d]"), groupID);

                                                                                                      stringID.Add(IDlocal);
                                                                                                    }
                                                                                                    break;
                                                }

                                              return true;
                                            }


    XTHREADGROUPID                          groupID;
    XMUTEX*                                 xthreadsvectormutex;
    XVECTOR<XTHREADCOLLECTED*>              xthreadsvector;
    XTHREAD*                                xthread;
};




class XTHREADSCOLLECTEDMANAGER
{
  public:

    static XTHREADSCOLLECTEDMANAGER&        GetInstance                     ()
                                            {
                                              if(!instance) instance = new XTHREADSCOLLECTEDMANAGER();

                                              return (*instance);
                                            }

    static bool                             DelInstance                     ()
                                            {
                                              if(instance)
                                                {
                                                  delete instance;
                                                  instance = NULL;

                                                  return true;
                                                }

                                              return false;
                                            }

    bool                                    ActivateGroup                   (XTHREADGROUPID groupID)
                                            {
                                              groupthreadsactivate.Add(groupID);
                                              return true;
                                            }


    bool                                    DeactivateGroup                 (XTHREADGROUPID groupID)
                                            {
                                              groupthreadsactivate.Delete(groupID);
                                              return true;
                                            }

    bool                                    IsActivateGroup                 (XTHREADGROUPID groupID)
                                            {
                                              for(XDWORD c=0; c<groupthreadsactivate.GetSize(); c++)
                                                {
                                                  XTHREADGROUPID _groupID = groupthreadsactivate.Get(c);
                                                  if(groupID == _groupID) return true;
                                                }

                                              return false;
                                            }

    XTHREADSCOLLECTED*                      GetThreadsCollectedByGroupID   (XTHREADGROUPID groupID)
                                            {
                                              XTHREADSCOLLECTED* xthreadscollected = NULL;

                                              for(XDWORD c=0; c<groupthreadsvector.GetSize(); c++)
                                                {
                                                  XTHREADSCOLLECTED* _xthreadscollected = groupthreadsvector.Get(c);
                                                  if(_xthreadscollected)
                                                    {
                                                      if(_xthreadscollected->GetGroupID() == groupID)
                                                        {
                                                          xthreadscollected = _xthreadscollected;
                                                          break;
                                                        }
                                                    }
                                                }

                                              return xthreadscollected;
                                            }


    XTHREADCOLLECTED*                       CreateThread                    (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
                                            {
                                              XTHREADCOLLECTED*  xthreadcollected = NULL;
                                              bool               isactivategroup  = IsActivateGroup(groupID);

                                              if(isactivategroup)
                                                {
                                                  if(!groupthreadsmutex) return NULL;

                                                  groupthreadsmutex->Lock();

                                                  XTHREADSCOLLECTED* xthreadscollected = GetThreadsCollectedByGroupID(groupID);
                                                  if(!xthreadscollected)
                                                    {
                                                      xthreadscollected = new XTHREADSCOLLECTED(groupID);
                                                      if(xthreadscollected) groupthreadsvector.Add(xthreadscollected);
                                                    }

                                                  if(xthreadscollected) xthreadcollected = xthreadscollected->Create(groupID, ID, function, param);

                                                  groupthreadsmutex->UnLock();
                                                }
                                               else
                                                {
                                                  XTHREADSCOLLECTED* xthreadscollected = new XTHREADSCOLLECTED();
                                                  if(xthreadscollected)
                                                    {
                                                      xthreadcollected = xthreadscollected->Create(groupID, ID, function, param);
                                                      delete xthreadscollected;
                                                    }
                                                }

                                              return xthreadcollected;
                                            }

    bool                                    DeleteThread                    (XTHREADGROUPID groupID, XTHREADCOLLECTED* xthreadcollected)
                                            {
                                              bool isactivategroup = IsActivateGroup(groupID);
                                              bool status          = false;

                                              if(isactivategroup)
                                                {
                                                  if(!groupthreadsmutex) return false;

                                                  groupthreadsmutex->Lock();

                                                  XTHREADSCOLLECTED* xthreadscollected = GetThreadsCollectedByGroupID(isactivategroup?groupID:XTHREADGROUPID_UNGROUP);
                                                  if(xthreadscollected)
                                                    {
                                                      status = xthreadscollected->Delete(groupID, xthreadcollected);
                                                    }

                                                  groupthreadsmutex->UnLock();
                                                }
                                               else
                                                {
                                                  XTHREADSCOLLECTED* xthreadscollected = new XTHREADSCOLLECTED();
                                                  if(xthreadscollected)
                                                    {
                                                      status = xthreadscollected->Delete(groupID, xthreadcollected);
                                                      delete xthreadscollected;
                                                    }
                                                }

                                              return status;
                                            }

    bool                                    DeleteAll                       ()
                                            {
                                              if(!groupthreadsmutex) return false;

                                              groupthreadsmutex->Lock();

                                              groupthreadsvector.DeleteContents();
                                              groupthreadsvector.DeleteAll();

                                              groupthreadsmutex->UnLock();

                                              return true;
                                            }


  private:
                                            XTHREADSCOLLECTEDMANAGER        ()
                                            {
                                              Clean();

                                              groupthreadsmutex = xfactory->Create_Mutex();

                                            }

    virtual                                ~XTHREADSCOLLECTEDMANAGER        ()
                                            {
                                              DeleteAll();

                                              groupthreadsactivate.DeleteAll();

                                              if(groupthreadsmutex) xfactory->Delete_Mutex(groupthreadsmutex);

                                              Clean();
                                            }

    void                                    Clean                           ()
                                            {
                                              nogroupthread     = NULL;
                                              groupthreadsmutex = NULL;
                                            }


    static XTHREADSCOLLECTEDMANAGER*        instance;

    XTHREAD*                                nogroupthread;
    XVECTOR<XTHREADGROUPID>                 groupthreadsactivate;
    XMUTEX*                                 groupthreadsmutex;
    XVECTOR<XTHREADSCOLLECTED*>             groupthreadsvector;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#else

#define CREATEXTHREAD               xfactory->CreateThread
#define DELETEXTHREAD               xfactory->DeleteThread
#define XTHREADCOLLECTED            XTHREAD
#define ACTIVATEXTHREADGROUP        //
#define DEACTIVATEXTHREADGROUP      //

#endif