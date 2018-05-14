
/*------------------------------------------------------------------------------------------
//  SNDEVENT.CPP
//
//  sound system events
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 09/11/2015 17:14:48
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDEvent.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  SNDEVENT::SNDEVENT
*/
/**
//
//  Class Constructor SNDEVENT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/11/2015 17:49:46
//
//  @param        subject :
//  @param        type :
//  @param        XEVENT(subject :
//  @param        type :
//  @param        family :
*/
/*-----------------------------------------------------------------*/
SNDEVENT::SNDEVENT(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
{
  Clean();
}





/*-------------------------------------------------------------------
//  SNDEVENT::~SNDEVENT
*/
/**
//
//   Class Destructor SNDEVENT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/11/2015 17:49:51
//
*/
/*-----------------------------------------------------------------*/
SNDEVENT::~SNDEVENT()
{
  Clean();
}
