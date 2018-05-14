//------------------------------------------------------------------------------------------
//  XWINDOWSSLEEP.CPP
//
//  WINDOWS sleep class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <math.h>

#include "XWINDOWSSleep.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XWINDOWSSLEEP::XWINDOWSSLEEP
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return
//  */
//-------------------------------------------------------------------
XWINDOWSSLEEP::XWINDOWSSLEEP()
{
  Clean();

  QueryPerformanceFrequency(&frequency);

  PCfrequencymilliseconds = (double)(frequency.QuadPart) / 1000.0;
  PCfrequencymicroseconds = (double)(frequency.QuadPart) / 1000000.0;
}


//-------------------------------------------------------------------
//  XWINDOWSSLEEP::~WINDOWSXWINDOWSSLEEP
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:01:21
//
//  @return       void :
*/
//-------------------------------------------------------------------
XWINDOWSSLEEP::~XWINDOWSSLEEP()
{
  Clean();
}



/*-------------------------------------------------------------------
//  XWINDOWSSLEEP::Seconds
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2013 13:29:17
//
//  @return       void :
//  @param        seconds :
*/
/*-----------------------------------------------------------------*/
void XWINDOWSSLEEP::Seconds(int seconds)
{
  Sleep(seconds*1000);
}



/*-------------------------------------------------------------------
//  XWINDOWSSLEEP::MilliSeconds
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2013 14:10:57
//
//  @return       void :
//  @param        milliseconds :
*/
/*-----------------------------------------------------------------*/
void XWINDOWSSLEEP::MilliSeconds(int milliseconds)
{
  Sleep(milliseconds);
}



/*-------------------------------------------------------------------
//  XWINDOWSSLEEP::MicroSeconds
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2013 13:37:00
//
//  @return       void :
//  @param        microseconds :
*/
/*-----------------------------------------------------------------*/
void XWINDOWSSLEEP::MicroSeconds(int microseconds)
{
  __int64 timeellapsed;
  __int64 timestart;
  __int64 timedelta;

  QueryPerformanceFrequency((LARGE_INTEGER*)(&timedelta));

  __int64 timetowait = (__int64)((XDWORD)timedelta * ((XDWORD)microseconds / 1000000.0f));

  QueryPerformanceCounter ((LARGE_INTEGER*)(&timestart));

  timeellapsed = timestart;

  while(( timeellapsed - timestart ) < timetowait)
   {
     QueryPerformanceCounter( (LARGE_INTEGER*)(&timeellapsed ) );
   };
}



