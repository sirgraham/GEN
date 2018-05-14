/*------------------------------------------------------------------------------------------
//  DIOCHECKINTERNETCONNEXION.CPP
//
//  Data IO Check Internet Connexion
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/11/2007 16:26:37
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XDateTime.h"
#include "XBuffer.h"
#include "XDebug.h"
#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOPing.h"

#include "DIOCheckConnexions.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::DIOCHECKCONNEXIONS
*/
/**
//
//  Class Constructor DIOCHECKCONNEXIONS
//
//  @author       Abraham J. Velez
//  @version      31/05/2015 17:03:55
//


//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOCHECKCONNEXIONS::DIOCHECKCONNEXIONS()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::~DIOCHECKCONNEXIONS
*/
/**
//
//   Class Destructor DIOCHECKCONNEXIONS
//
//  @author       Abraham J. Velez
//  @version      31/05/2015 17:04:02
//
*/
/*-----------------------------------------------------------------*/
DIOCHECKCONNEXIONS::~DIOCHECKCONNEXIONS()
{
  End();

  Clean();
}




/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/05/2015 14:16:09
//
//  @return       bool :
//
//  @param        timerconnexionscheck :
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Ini(int timeconnexionchecks, bool validsomeisconnected, bool dispersionmode)
{
  Setup(timeconnexionchecks, validsomeisconnected, dispersionmode);

  xtimerfortimeconnexionchecks = xfactory->CreateTimer();
  if(!xtimerfortimeconnexionchecks) return false;

  //ping  = new DIOPING;
  ping = diofactory->CreatePing();
  if(!ping) return false;

  xmutexconnexions = xfactory->Create_Mutex();
  if(!xmutexconnexions) return false;

  threadcheckconnexions = CREATEXTHREAD(XTHREADGROUPID_DIOCHECKCONNEXIONS, __L("DIOCHECKCONNEXIONS::Ini"), ThreadCheckConnexions, (void*)this);
  if(!threadcheckconnexions)  return false;

  SetIsCheckTimeConnexions(true);

  return threadcheckconnexions->Ini();
}




/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::Connexion_Add
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/05/2015 16:45:49
//
//  @return       bool :
//
//  @param        url :
//  @param        onlycheckoneconnexion :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexion_Add(XCHAR* url, XDWORD& ID)
{
  DIOCHECKCONNEXION* connexion = new DIOCHECKCONNEXION();
  if(!connexion) return false;

  if(connexion->Set(url))
    {
      ID = connexion->GetID();

      if(ID)
        {
          if(xmutexconnexions) xmutexconnexions->Lock();

          connexions.Add(connexion);

          if(xmutexconnexions) xmutexconnexions->UnLock();

          return true;
        }
    }

  delete connexion;

  return false;
}



/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::Connexion_DeleteByID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/05/2015 23:40:16
//
//  @return       bool :
//
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexion_DeleteByID(XDWORD ID)
{
  DIOCHECKCONNEXION* connexion = Connexion_GetByID(ID);
  if(connexion)
    {
      if(xmutexconnexions) xmutexconnexions->Lock();

      connexions.Delete(connexion);
      delete connexion;

      if(xmutexconnexions) xmutexconnexions->UnLock();

      return true;
    }

  return false;
}




/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::Connexion_GetByID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/05/2015 15:59:45
//
//  @return       DIOCHECKCONNEXION* :
//
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
DIOCHECKCONNEXION* DIOCHECKCONNEXIONS::Connexion_GetByID(XDWORD ID)
{
  if(xmutexconnexions) xmutexconnexions->Lock();

  for(XDWORD c=0; c<connexions.GetSize(); c++)
    {
      DIOCHECKCONNEXION* cc = connexions.Get(c);
      if(cc)
        {
          if(cc->GetID() == ID)
            {
              if(xmutexconnexions) xmutexconnexions->UnLock();
              return cc;
            }
        }
    }

  if(xmutexconnexions) xmutexconnexions->UnLock();

  return NULL;
}




/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::Connexion_IsActive
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/05/2015 16:01:03
//
//  @return       bool :
//
//  @param        ID :
//  @param        isactive :
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexion_IsActive(XDWORD ID, bool& isactive)
{
  isactive = false;

  if(xmutexconnexions) xmutexconnexions->Lock();

  for(XDWORD c=0; c<connexions.GetSize(); c++)
    {
      DIOCHECKCONNEXION* cc = connexions.Get(c);
      if(cc)
        {
          if(cc->GetID() == ID)
            {
              isactive = cc->IsConnected();
              if(xmutexconnexions) xmutexconnexions->UnLock();

              return true;
            }
        }
    }

  if(xmutexconnexions) xmutexconnexions->UnLock();

  return false;
}



/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::Connexions_IsAllChecked
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2015 16:58:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexions_IsAllChecked()
{
  bool ischeckallconnexions = true;

  if(xmutexconnexions) xmutexconnexions->Lock();

  for(XDWORD c=0; c<connexions.GetSize(); c++)
    {
      DIOCHECKCONNEXION* cc = connexions.Get(c);
      if(cc)
        {
          if(!cc->GetNChecks()) ischeckallconnexions = false;
        }
    }

  if(xmutexconnexions) xmutexconnexions->UnLock();

  return ischeckallconnexions;
}




/*-------------------------------------------------------------------
//   DIOCHECKCONNEXIONS::Connexions_IsSomeChecked
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version
//
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexions_IsSomeChecked()
{
  bool ischecksomeconnexions = false;

  if(xmutexconnexions) xmutexconnexions->Lock();

  for(XDWORD c=0; c<connexions.GetSize(); c++)
    {
      DIOCHECKCONNEXION* cc = connexions.Get(c);
      if(cc)
        {
          if(cc->GetNChecks())
            {
              ischecksomeconnexions = true;
              break;
            }
        }
    }

  if(xmutexconnexions) xmutexconnexions->UnLock();

  return ischecksomeconnexions;
}





/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::Connexions_WaitToAllChecked
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2015 16:59:27
//
//  @return       bool :
//
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexions_WaitToAllChecked(int timeout)
{
  if(!connexions.GetSize()) return false;

  XTIMER* xtimer = xfactory->CreateTimer();
  if(!xtimer) return false;

  bool status = true;

  xtimer->Reset();

  SetIsCheckTimeConnexions(false);

  while(1)
    {
      if(timeout != XTIMER_INFINITE)
        {
          if(xtimer->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              status = false;
              break;
            }
        }

      if(Connexions_IsAllChecked()) break;

      xsleep->MilliSeconds(100);
    }

  xfactory->DeleteTimer(xtimer);

  SetIsCheckTimeConnexions(true);

  return status;
}





/*-------------------------------------------------------------------
//   DIOCHECKCONNEXIONS::Connexions_WaitToSomeIsChecked
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version
//
//  @return   bool :
//
//  @param    int :
//
*//*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexions_WaitToSomeIsChecked(int timeout)
{
  if(!connexions.GetSize()) return false;

  XTIMER* xtimer = xfactory->CreateTimer();
  if(!xtimer) return false;

  bool status = true;

  xtimer->Reset();

  SetIsCheckTimeConnexions(false);

  while(1)
    {
      if(timeout != XTIMER_INFINITE)
        {
          if(xtimer->GetMeasureSeconds() >= (XDWORD)timeout)
            {
              status = false;
              break;
            }
        }

      if(Connexions_IsSomeChecked()) break;

      xsleep->MilliSeconds(100);
    }

  xfactory->DeleteTimer(xtimer);

  SetIsCheckTimeConnexions(true);

  return status;
}



/*-------------------------------------------------------------------
//   DIOCHECKCONNEXIONS::Connexions_SomeIsConnected
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version

//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexions_SomeIsConnected()
{
  bool someisactive = false;

  if(xmutexconnexions) xmutexconnexions->Lock();

  for(XDWORD c=0; c<connexions.GetSize(); c++)
    {
      DIOCHECKCONNEXION* cc = connexions.Get(c);
      if(cc)
        {
          someisactive = cc->IsConnected();
          if(someisactive) break;
        }
    }

  if(xmutexconnexions) xmutexconnexions->UnLock();

  return someisactive;
}





/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::DeleteAllConnexions
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/05/2015 12:10:30
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::Connexions_DeleteAll()
{
  if(xmutexconnexions) xmutexconnexions->Lock();

  connexions.DeleteContents();
  connexions.DeleteAll();

  if(xmutexconnexions) xmutexconnexions->UnLock();

  return true;
}




/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/05/2015 13:39:54
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKCONNEXIONS::End()
{
  if(threadcheckconnexions)
    {
      threadcheckconnexions->End();

      DELETEXTHREAD(XTHREADGROUPID_DIOCHECKCONNEXIONS, threadcheckconnexions);
      threadcheckconnexions = NULL;
    }

  Connexions_DeleteAll();

  if(xmutexconnexions)
    {
      xfactory->Delete_Mutex(xmutexconnexions);
      xmutexconnexions = NULL;
    }

  if(ping)
    {
      diofactory->DeletePing(ping);
      ping = NULL;
    }

  if(xtimerfortimeconnexionchecks)
    {
      xfactory->DeleteTimer(xtimerfortimeconnexionchecks);
      xtimerfortimeconnexionchecks = NULL;
    }


  return true;
}





/*-------------------------------------------------------------------
//  DIOCHECKCONNEXIONS::ThreadCheckConnexions
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/05/2015 13:30:12
//
//  @param        param :
*/
/*-----------------------------------------------------------------*/
void DIOCHECKCONNEXIONS::ThreadCheckConnexions(void* param)
{
  DIOCHECKCONNEXIONS* checkconnexions = (DIOCHECKCONNEXIONS*)param;
  if(!checkconnexions) return;

  if(!checkconnexions->ping)                                  return;
  if(!checkconnexions->xmutexconnexions)                      return;
  if(!checkconnexions->xtimerfortimeconnexionchecks)    return;

  static int index = 0;

  if(checkconnexions->dispersionmode)
    {
      checkconnexions->xmutexconnexions->Lock();

      int nconnexions = checkconnexions->connexions.GetSize();
      if(nconnexions)
        {
          DIOCHECKCONNEXION* checkconnexion = checkconnexions->connexions.Get(index);
          if(!checkconnexion)
            {
              index = 0;
              checkconnexion = checkconnexions->connexions.Get(index);
            }

          if(checkconnexion)
            {
              checkconnexions->ping->Set(checkconnexion->GetURL()->Get());
              checkconnexion->SetIsConnected(checkconnexions->ping->Do(DIOCHECKCONNEXIONS_DEFAULTNCHECKSFOREVERYCONNEXION, DIOCHECKCONNEXIONS_DEFAULTTIMERCONNEXIONCHECK, checkconnexions->validsomeisconnected));
              checkconnexion->IncNChecks();

              index++;
            }
        }

      checkconnexions->xmutexconnexions->UnLock();

      if(nconnexions && checkconnexions->IsCheckTimeConnexions())
        {
          int timer = checkconnexions->timeconnexionchecks/nconnexions;

          for(int c=0; c<timer; c++)
            {
              for(int d=0; d<10; d++)
                {
                  if(!checkconnexions->threadcheckconnexions->IsRunning()) break;
                  xsleep->MilliSeconds(100);
                }
            }
        }
    }
   else
    {
      checkconnexions->xmutexconnexions->Lock();

      int nconnexions = checkconnexions->connexions.GetSize();
      if(nconnexions)
        {
          for(int c=0; c<nconnexions; c++)
            {
              DIOCHECKCONNEXION* checkconnexion = checkconnexions->connexions.Get(index);
              if(checkconnexion)
                {
                  checkconnexions->ping->Set(checkconnexion->GetURL()->Get());
                  checkconnexion->SetIsConnected(checkconnexions->ping->Do(DIOCHECKCONNEXIONS_DEFAULTNCHECKSFOREVERYCONNEXION, DIOCHECKCONNEXIONS_DEFAULTTIMERCONNEXIONCHECK, checkconnexions->validsomeisconnected));
                  checkconnexion->IncNChecks();

                  index++;
                  if(index >= nconnexions) index = 0;

                  if((checkconnexions->validsomeisconnected) && (checkconnexion->IsConnected())) break;
                  if(!checkconnexions->threadcheckconnexions->IsRunning()) break;
                }
            }
        }

      checkconnexions->xmutexconnexions->UnLock();

      if(nconnexions && checkconnexions->IsCheckTimeConnexions())
        {
          for(int c=0; c<checkconnexions->timeconnexionchecks; c++)
            {
              for(int d=0; d<10; d++)
                {
                  if(!checkconnexions->threadcheckconnexions->IsRunning()) break;
                  xsleep->MilliSeconds(100);
                }
            }
        }
   }
}
