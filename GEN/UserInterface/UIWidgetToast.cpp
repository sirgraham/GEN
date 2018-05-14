
/*------------------------------------------------------------------------------------------
//  UIWIDGETTOAST.CPP
//
//  widget toast
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 11/07/2016 14:13:16
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetToast.h"
#include "UIManager.h"

#include "XFactory.h"
#include "XTimer.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETTOAST::UIWIDGETTOAST
*/
/**
//
//  Class Constructor UIWIDGETTOAST
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/07/2016 14:16:20
//
//  @param        name :
//  @param        uimanagerarg :
*/
/*-----------------------------------------------------------------*/
UIWIDGETTOAST::UIWIDGETTOAST(XCHAR* name, UIMANAGER * uimanager) : UIWIDGETFORM(name, uimanager)
{
  Clean();

  xtimer = xfactory->CreateTimer();
}



/*-------------------------------------------------------------------
//  UIWIDGETTOAST::UIWIDGETTOAST
*/
/**
//
//  Class Constructor UIWIDGETTOAST
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/02/2017 10:14:06
//
//  @param        rhs :
*/
/*-----------------------------------------------------------------*/
UIWIDGETTOAST::UIWIDGETTOAST(UIWIDGETTOAST* rhs) : UIWIDGETFORM(rhs)
{
  Clean();

  xtimer          = xfactory->CreateTimer();
  milliseconds    = rhs->milliseconds;
  updated         = rhs->updated;
}



/*-------------------------------------------------------------------
//  UIWIDGETTOAST::~UIWIDGETTOAST
*/
/**
//
//   Class Destructor UIWIDGETTOAST
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/07/2016 14:16:25
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETTOAST::~UIWIDGETTOAST()
{
  xfactory->DeleteTimer(xtimer);

  Clean();
}




/*-------------------------------------------------------------------
//  UIWIDGETTOAST::SetTime
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/07/2016 14:16:31
//
//  @param        milliseconds :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETTOAST::SetTime(XDWORD milliseconds)
{
  this->milliseconds = milliseconds;

}




/*-------------------------------------------------------------------
//  UIWIDGETTOAST::GetTime
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/07/2016 14:16:39
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD UIWIDGETTOAST::GetTime()
{
  return this->milliseconds;
}




/*-------------------------------------------------------------------
//  UIWIDGETTOAST::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/07/2016 14:16:46
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTOAST::InternalUpdate(XDWORD widgetlevel)
{
  if(milliseconds != 0)
    {
      if(!updated)
        {
          updated = true;
          xtimer->Reset();
        }

      if(xtimer->GetMeasureMilliSeconds() > milliseconds)
        {
          uimanager->AddToDeleteQueue(this);
        }
    }

  return true;
}

