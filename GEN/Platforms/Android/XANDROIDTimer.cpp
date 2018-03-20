//------------------------------------------------------------------------------------------
//	XANDROIDTIMER.CPP
//	
//	ANDROID time class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
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

#include "XDebug.h"
#include "XANDROIDFile.h"

#include "XANDROIDTimer.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XANDROIDTIMER::XANDROIDTIMER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XANDROIDTIMER::XANDROIDTIMER()
{
	Reset();
}


//-------------------------------------------------------------------
//  XANDROIDTIMER::~W32XANDROIDTIMER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:01:21
//	
//	@return 			void : 
*/
//-------------------------------------------------------------------
XANDROIDTIMER::~XANDROIDTIMER()
{

}



/*-------------------------------------------------------------------
//  XANDROIDTIMER::GetMicroSecondsTickCounter
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 17:13:33
//	
//	@return 			XQWORD : 
//	*/
/*-----------------------------------------------------------------*/
XQWORD XANDROIDTIMER::GetMicroSecondsTickCounter()
{	
	struct timespec t1;

	clock_gettime(CLOCK_MONOTONIC, &t1);

	XQWORD elapsedtime = ((double)(t1.tv_sec) * 1.0e9 + (double)(t1.tv_nsec))/1000; 
	
	return elapsedtime;
}


