//------------------------------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS.CPP
//
//  Data IO Stream Local Enum Servers class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XBuffer.h"

#include "DIOFactory.h"

#include "DIOStreamEnumServers.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::DIOSTREAMENUMSERVERS
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:23:16
//
//  @return       void :
*/
/*-----------------------------------------------------------------*/
DIOSTREAMENUMSERVERS::DIOSTREAMENUMSERVERS() : DIOSTREAMENUMDEVICES()
{
  Clean();
  xtimer  = xfactory->CreateTimer();
}




/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::~DIOSTREAMENUMSERVERS
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:24:57
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMENUMSERVERS::~DIOSTREAMENUMSERVERS()
{
  xfactory->DeleteTimer(xtimer);

  Clean();
}




/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::Search
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/05/2013 9:48:28
//
//  @return       bool :
//  @param        searchtimeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMSERVERS::Search(int searchtimeout)
{
  if(issearching) return false;

  this->searchtimeout = searchtimeout;

  return Search();
}



/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::Search
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:36:43
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMSERVERS::Search()
{
  issearching = false;

  return false;
}




/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::StopSearch
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:36:52
//
//  @return       bool :
//  @param        waitend :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMSERVERS::StopSearch(bool waitend)
{

  return false;
}



/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::IsSearching
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:37:02
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMSERVERS::IsSearching()
{
  return issearching;
}




/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::IsDeviceAvailable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 23:37:16
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMSERVERS::IsDeviceAvailable(XCHAR* resource)
{

  return false;
}



/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::GetXTimer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/05/2013 0:39:33
//
//  @return       XTIMER* :
//  */
/*-----------------------------------------------------------------*/
XTIMER* DIOSTREAMENUMSERVERS::GetXTimer()
{
  return xtimer;
}




/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::SendSignal
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:37:48
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMSERVERS::SendSignal()
{

  return false;
}





/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::GetID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/05/2011 12:06:40
//
//  @return       XDWORD :
//  */
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAMENUMSERVERS::GetID()
{
  return ID;
}




/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::SetID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/05/2011 12:07:44
//
//  @return       bool :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMENUMSERVERS::SetID(XDWORD ID)
{
  this->ID = ID;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMENUMSERVERS::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:37:54
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOSTREAMENUMSERVERS::Clean()
{

  type             = DIOSTREAMENUMTYPE_LOCAL_SERVERS;

  xtimer           = NULL;

  ID               = 0;

  issearching      = false;
  searchtimeout    = 0;
}

