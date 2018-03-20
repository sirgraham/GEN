//------------------------------------------------------------------------------------------
//	XTIMER.H
//	
/**	
// \class 
//   
//  Timer Functions
//   
//	@author	 Abraham J. Velez
//	@version 04/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XTIMER_H_
#define _XTIMER_H_
	
						
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"
#include "XString.h"
#include "XPath.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


#define XTIMER_INFINITE										-1

	
//---- CLASS -------------------------------------------------------------------------------

class XDATETIME;

#ifdef XDEBUG_VIRTUALCLOCKTICK

class XFACTORY;
class XMUTEX;
class XTHREADCOLLECTED;


class XTIMERCLOCK
{
	public:

												 	  XTIMERCLOCK												();
		virtual							 	 ~XTIMERCLOCK 											();

		XQWORD									GetClockTicks											();

	private:

		void										Clean															(); 	

		static void							ThreadClockFunction								(void* data);
		
		XQWORD									clockticks;

		XMUTEX*									xmutexclock;
		XTHREAD*								threadclock;		
};
#endif


class XTIMER
{
	public:

												 	  XTIMER														();
		virtual							 	 ~XTIMER														();
					
		void    								AddMilliSeconds										(XQWORD milliseconds);
		void										AddSeconds												(XQWORD seconds);
										
		void										SetMilliSeconds										(XQWORD millisecods);

		void										Reset															();	
		
		XDWORD									GetMeasureHours										();
		XDWORD									GetMeasureMinutes									();
		XDWORD									GetMeasureSeconds									();																															
		XQWORD									GetMeasureMilliSeconds						();		
		XQWORD									GetMeasureMicroSeconds						();

		bool										GetMeasureToDate									(XDATETIME* xdatetime);

		bool										GetMeasureString									(XSTRING& measure, bool large = false);
	
		virtual XQWORD					GetMicroSecondsTickCounter				()																			{	return 0;					}

 protected:
	
		XQWORD									last;
		XQWORD									more;

 private:

		void										Clean															(); 		
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#ifdef XDEBUG_VIRTUALCLOCKTICK
extern XTIMERCLOCK*  xtimerclock;
#endif

#endif
