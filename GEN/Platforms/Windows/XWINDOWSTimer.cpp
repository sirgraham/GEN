//------------------------------------------------------------------------------------------
//  XWINDOWSTIMER.CPP
//
//  WINDOWS timer class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <math.h>

#include "XString.h"
#include "XDebug.h"
#include "XPath.h"

#include "XWINDOWSTimer.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XWINDOWSTIMER::XWINDOWSTIMER
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return
//  */
//-------------------------------------------------------------------
XWINDOWSTIMER::XWINDOWSTIMER()
{
  Clean();

  QueryPerformanceFrequency(&frequency);

  PCfrequencymilliseconds = (double)(frequency.QuadPart) / 1000.0;
  PCfrequencymicroseconds = (double)(frequency.QuadPart) / 1000000.0;


  Reset();
}


//-------------------------------------------------------------------
//  XWINDOWSTIMER::~WINDOWSXWINDOWSTIMER
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:01:21
//
//  @return       void :
*/
//-------------------------------------------------------------------
XWINDOWSTIMER::~XWINDOWSTIMER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  XWINDOWSTIMER::GetMicroSecondsTickCounter
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2013 13:38:33
//
//  @return       XQWORD :
//  */
/*-----------------------------------------------------------------*/
XQWORD XWINDOWSTIMER::GetMicroSecondsTickCounter()
{
  #ifdef XDEBUG_VIRTUALCLOCKTICK
  if(!xtimerclock) return 0;
  return xtimerclock->GetClockTicks();
  #else

  if(!PCfrequencymilliseconds) return 0;

  LARGE_INTEGER ticks;
  XQWORD        microseconds = 0;

  QueryPerformanceCounter(&ticks);
  if(frequency.QuadPart) microseconds = (XQWORD)(ticks.QuadPart / PCfrequencymicroseconds);

  return microseconds;

  #endif
}



