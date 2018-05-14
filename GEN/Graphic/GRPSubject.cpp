
/*------------------------------------------------------------------------------------------
//  GRPSUBJECT.CPP
//
//  subject pattern
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 11/02/2016 10:57:20
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPSubject.h"
#include "GRPNamed.h"
#include "XDebug.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPSUBJECT::GRPSUBJECT
*/
/**
//
//  Class Constructor GRPSUBJECT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/02/2016 10:58:19
//
*/
/*-----------------------------------------------------------------*/
GRPSUBJECT::GRPSUBJECT()
{
  Clean();
}

/*-------------------------------------------------------------------
//  GRPSUBJECT::~GRPSUBJECT
*/
/**
//
//   Class Destructor GRPSUBJECT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/02/2016 10:58:27
//
*/
/*-----------------------------------------------------------------*/
GRPSUBJECT::~GRPSUBJECT()
{
  this->Publish(GRPMSG_DESTROYED);

  for (XDWORD e=0;e<Observers.GetSize();e++)
    Observers.FastGet(e)->GetSubjects()->Delete(this);

  Observers.DeleteAll();
  Clean();
}


/*-------------------------------------------------------------------
//  GRPSUBJECT::AddObserver
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/02/2016 10:58:44
//
//  @return       bool :
//
//  @param        observer :
*/
/*-----------------------------------------------------------------*/
bool GRPSUBJECT::AddObserver(GRPOBSERVER* observer)
{
  if (observer==NULL)
    return false;

  if (this->Observers.Add(observer))
  {
    nObservers++;
    observer->GetSubjects()->Add(this);
  }

  return true;
}

/*-------------------------------------------------------------------
//  GRPSUBJECT::RemoveObserver
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/02/2016 10:59:00
//
//  @return       bool :
//
//  @param        observer :
*/
/*-----------------------------------------------------------------*/
bool GRPSUBJECT::RemoveObserver (GRPOBSERVER* observer)
{
  if (this->Observers.Delete(observer))
    nObservers--;
  return true;
}

bool GRPSUBJECT::Hide()
{
    while (this->Observers.GetSize())
        RemoveObserver(this->Observers.Get(0));
    return true;
}

/*-------------------------------------------------------------------
//  GRPSUBJECT::Publish
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/02/2016 10:59:18
//
//  @return       int :
//
//  @param        msg :
*/
/*-----------------------------------------------------------------*/
int GRPSUBJECT::Publish (GRPMSGTYPE msg)
{
  XDWORD n = Observers.GetSize();

  for (XDWORD e=0;e<n;e++)
    if (Observers.FastGet(e))
      Observers.FastGet(e)->Notify(msg,this);

  if (msg == GRPMSG_DESTROYED) //only on destruction
  {
      for (XDWORD e = 0; e<n; e++)
          if (Observers.FastGet(e))
              Observers.FastGet(e)->Unsubscribe(this);
  }

  return 1;
}


/*-------------------------------------------------------------------
//  GRPSUBJECT::Publish
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 12:50:53
//
//  @return       int :
//
//  @param        msg :
//  @param        subject :
*/
/*-----------------------------------------------------------------*/
int GRPSUBJECT::Publish (GRPMSGTYPE msg, GRPSUBJECT* subject)
{
for (XDWORD e=0;e<Observers.GetSize();e++)
if (Observers.FastGet(e))
  Observers.FastGet(e)->Notify(msg,subject);
return 1;
}


