//------------------------------------------------------------------------------------------
//  XLINUXTIMER.CPP
//
//  LINUX timer class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>

#include "XDebugTrace.h"
#include "XLINUXFile.h"

#include "XLINUXTimer.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XLINUXTIMER::XLINUXTIMER
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return
//  */
//-------------------------------------------------------------------
XLINUXTIMER::XLINUXTIMER()
{
  Reset();
}



/*-------------------------------------------------------------------
//  XLINUXTIMER::~XLINUXTIMER
*/
/**
//
//   Class Destructor XLINUXTIMER
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 6:56:26
//
*/
/*-----------------------------------------------------------------*/
XLINUXTIMER::~XLINUXTIMER()
{

}



/*-------------------------------------------------------------------
//  XLINUXTIMER::GetMicroSecondsTickCounter
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2013 17:13:33
//
//  @return       XQWORD :
//  */
/*-----------------------------------------------------------------*/
XQWORD XLINUXTIMER::GetMicroSecondsTickCounter()
{
  /*
  struct timeval t1;
  XQWORD         elapsedtime;

  gettimeofday(&t1, NULL);

  elapsedtime  = (t1.tv_sec * 1000000.0);    // sec to ms
  elapsedtime += (t1.tv_usec / 1000.0);      // us to ms

  return elapsedtime;
  */


  XQWORD elapsedtime = 0;
  struct timespec t1;

  #ifdef CLOCK_MONOTONIC
  clock_gettime(CLOCK_MONOTONIC, &t1);
  #else
  clock_gettime(CLOCK_REALTIME, &t1);
  #endif

  elapsedtime = ((double)(t1.tv_sec) * 1000000) + (double)(t1.tv_nsec/1000); // to us

  return elapsedtime;
}



