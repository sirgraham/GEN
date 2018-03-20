//------------------------------------------------------------------------------------------
//	XLINUXSLEEP.CPP
//	
//	LINUX Sleep class
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
#include "XLINUXFile.h"

#include "XLINUXSleep.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XLINUXSLEEP::XLINUXSLEEP
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XLINUXSLEEP::XLINUXSLEEP()
{
	
}



/*-------------------------------------------------------------------
//	XLINUXSLEEP::~XLINUXSLEEP
*/	
/**	
//	
//	 Class Destructor XLINUXSLEEP
//	
//	@author				Abraham J. Velez
//	@version			03/06/2014 6:56:26
//	
*/
/*-----------------------------------------------------------------*/
XLINUXSLEEP::~XLINUXSLEEP()
{

}




/*-------------------------------------------------------------------
//  XLINUXSLEEP::Seconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 17:10:01
//	
//	@return 			void : 
//	@param				seconds : 
*/
/*-----------------------------------------------------------------*/
void XLINUXSLEEP::Seconds(int seconds)
{
	sleep(seconds);
}



 
/*-------------------------------------------------------------------
//  XLINUXSLEEP::MilliSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 17:10:18
//	
//	@return 			void : 
//	@param				milliseconds : 
*/
/*-----------------------------------------------------------------*/
void XLINUXSLEEP::MilliSeconds(int milliseconds)
{
	usleep(milliseconds*1000);
}




/*-------------------------------------------------------------------
//  XLINUXSLEEP::MicroSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 17:10:54
//	
//	@return 			void : 
//	@param				microseconds : 
*/
/*-----------------------------------------------------------------*/
void XLINUXSLEEP::MicroSeconds(int microseconds)
{
  usleep(microseconds);
}



/*-------------------------------------------------------------------
//  XLINUXSLEEP::SleepNanoSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/12/2013 18:32:39
//	
//	@return 			void : 
//	@param				nanoseconds : 
*/
/*-----------------------------------------------------------------*/
void XLINUXSLEEP::NanoSeconds(int nanoseconds)
{										
	struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = nanoseconds;

  nanosleep(&tim , &tim2);  
}






