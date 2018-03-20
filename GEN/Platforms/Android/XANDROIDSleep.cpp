//------------------------------------------------------------------------------------------
//	XANDROIDSLEEP.CPP
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

#include "XANDROIDSleep.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XANDROIDSLEEP::XANDROIDSLEEP
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:54:45
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XANDROIDSLEEP::XANDROIDSLEEP()
{

}


//-------------------------------------------------------------------
//  XANDROIDSLEEP::~W32XANDROIDSLEEP
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:01:21
//	
//	@return 			void : 
*/
//-------------------------------------------------------------------
XANDROIDSLEEP::~XANDROIDSLEEP()
{

}



/*-------------------------------------------------------------------
//  XANDROIDSLEEP::Seconds
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
void XANDROIDSLEEP::Seconds(int seconds)
{
	sleep(seconds);
}



 
/*-------------------------------------------------------------------
//  XANDROIDSLEEP::MilliSeconds
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
void XANDROIDSLEEP::MilliSeconds(int milliseconds)
{
	usleep(milliseconds*1000);
}




/*-------------------------------------------------------------------
//  XANDROIDSLEEP::MicroSeconds
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
void XANDROIDSLEEP::MicroSeconds(int microseconds)
{
  usleep(microseconds);
}




/*-------------------------------------------------------------------
//  XANDROIDSLEEP::NanoSeconds
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
void XANDROIDSLEEP::NanoSeconds(int nanoseconds)
{										
	struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = nanoseconds;

  nanosleep(&tim , &tim2);  
}



