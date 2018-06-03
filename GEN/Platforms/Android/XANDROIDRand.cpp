//------------------------------------------------------------------------------------------
//  XANDROIDTIME.CPP
//
//  Time Functions
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <time.h>

#include "xandroidrand.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XANDROIDRAND::Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/09/2009 07:14:11 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XANDROIDRAND::Ini()
{
  srand((unsigned)time(NULL));

  return true;
}


//-------------------------------------------------------------------
//  XANDROIDRAND::MaxElements
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return       int :
//  @param        max :
//  */
//-------------------------------------------------------------------
int XANDROIDRAND::MaxElements(int max)
{
  return GETRANDOM(0, max-1);;
}



//-------------------------------------------------------------------
//  XANDROIDRAND::Max
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return       int :
//  @param        max :
//  */
//-------------------------------------------------------------------
int XANDROIDRAND::Max(int max)
{
  return GETRANDOM(0, max);
}


//-------------------------------------------------------------------
//  XANDROIDRAND::Between
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return       int :
//  @param        min :
//  @param        max :
//  */
//-------------------------------------------------------------------
int XANDROIDRAND::Between(int min,int max)
{
  return GETRANDOM(min, max);
}



