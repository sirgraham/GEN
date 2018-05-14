
/*------------------------------------------------------------------------------------------
//  GRPOBSERVER.CPP
//
//  Observer pattern
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 11/02/2016 10:53:32
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObserver.h"
#include "GRPSubject.h"
#include "XVector.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPOBSERVER::~GRPOBSERVER
*/
/**
//
//  destructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return    :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPOBSERVER::~GRPOBSERVER()
{
    this->Unsubscribe();
    this->subjects.DeleteAll();
}

/*-------------------------------------------------------------------
//  GRPOBSERVER::GetSubjects
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/02/2016 10:54:56
//
//  @return       XVECTOR<GRPSUBJECT*>* :
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPSUBJECT*>* GRPOBSERVER::GetSubjects()
{
  return &this->subjects;
}

/*-------------------------------------------------------------------
//  GRPOBSERVER::Subscribe
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/05/2016 16:06:43
//
//  @param        subject :
*/
/*-----------------------------------------------------------------*/
bool  GRPOBSERVER::Subscribe(GRPSUBJECT* subject)
{
  if (subject)
  return subject->AddObserver(this);
  return false;
}

/*-------------------------------------------------------------------
//  GRPOBSERVER::Unsubscribe
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/02/2016 10:55:01
//
*/
/*-----------------------------------------------------------------*/
void GRPOBSERVER::Unsubscribe()
{
  while (subjects.GetSize())
  {
      int index = 0;
      do {
              index = subjects.Get(0)->GetObservers()->Find(this);
              if (index != -1)
                  subjects.Get(0)->GetObservers()->DeleteIndex(index);
              else
                  break;
      } while (index != -1);
      subjects.DeleteIndex(0);
  }

  this->subjects.DeleteAll();
}

void GRPOBSERVER::Unsubscribe(GRPSUBJECT* subject)
{
    for (int e=(int)subjects.GetSize()-1; e>=0;e--)
    {
        if (subjects.Get(e) == subject)
        {
            if (subjects.Get(e)->GetObservers()->Find(this) != NOTFOUND)
                subjects.Get(e)->GetObservers()->Delete(this);
            subjects.DeleteIndex(e);
        }
    }
}