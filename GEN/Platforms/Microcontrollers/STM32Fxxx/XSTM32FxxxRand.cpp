//------------------------------------------------------------------------------------------
//  XSTM32FXXXRAND.CPP
//
//  STM32Fxxx rand class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <time.h>

#include "XSTM32FxxxRand.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  XSTM32FXXXRAND::XSTM32FXXXRAND
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return
//  */
//-------------------------------------------------------------------
XSTM32FXXXRAND::XSTM32FXXXRAND()
{
  Ini();
}


//-------------------------------------------------------------------
//  XSTM32FXXXRAND::~XSTM32FXXXRAND
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return
//  */
//-------------------------------------------------------------------
XSTM32FXXXRAND::~XSTM32FXXXRAND()
{

}


/*-------------------------------------------------------------------
//  XSTM32FXXXRAND::Ini
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
bool XSTM32FXXXRAND::Ini()
{
  srand((unsigned)time(NULL));

  return true;
}


//-------------------------------------------------------------------
//  XSTM32FXXXRAND::MaxElements
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
int XSTM32FXXXRAND::MaxElements(int max)
{
  return GetRandom(0,max-1);;
}


//-------------------------------------------------------------------
//  XSTM32FXXXRAND::Max
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
int XSTM32FXXXRAND::Max(int max)
{
  return GetRandom(0,max);
}


//-------------------------------------------------------------------
//  XSTM32FXXXRAND::Between
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
int XSTM32FXXXRAND::Between(int min,int max)
{
  return GetRandom(min,max);
}



