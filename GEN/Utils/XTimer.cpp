//------------------------------------------------------------------------------------------
//	XTIMER.CPP
//	
//	Timer Functions
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 09/04/2007 19:46:31
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "XBase.h"
#include "XFactory.h"
#include "XDateTime.h"

#include "XTimer.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	

#ifdef XDEBUG_VIRTUALCLOCKTICK
XTIMERCLOCK*  xtimerclock = NULL;
#endif

//---- CLASS MEMBERS -----------------------------------------------------------------------


#ifdef XDEBUG_VIRTUALCLOCKTICK

/*-------------------------------------------------------------------
//  XTIMERCLOCK::XTIMERCLOCK
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2013 23:02:48
//	
//	@return 			

*/
/*-----------------------------------------------------------------*/
XTIMERCLOCK::XTIMERCLOCK()
{
	Clean();
	
	xmutexclock = xfactory->CreateMutex();
	if(xmutexclock)
		{
			threadclock = CREATEXTHREAD(XTHREADGROUPID_XTIMERCLOCK, __L("XTIMERCLOCK::XTIMERCLOCK"),ThreadClockFunction,(void*)this);
			if(threadclock) threadclock->Ini();					
		}
}




/*-------------------------------------------------------------------
//  XTIMERCLOCK::~XTIMERCLOCK
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2013 23:03:33
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
XTIMERCLOCK::~XTIMERCLOCK()
{
	if(threadclock)
		{
			threadclock->End();
			DELETEXTHREAD(XTHREADGROUPID_XTIMERCLOCK, threadclock);
		}
					
	if(xmutexclock) xfactory->DeleteMutex(xmutexclock);			
	
	Clean();
}



/*-------------------------------------------------------------------
//  XTIMERCLOCK::GetClockTicks
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2013 23:25:24
//	
//	@return 			XQWORD : 
//	*/
/*-----------------------------------------------------------------*/
XQWORD XTIMERCLOCK::GetClockTicks()
{
	if(!xmutexclock) return 0;

	xmutexclock->Lock();

	XQWORD clktks = clockticks;

	xmutexclock->UnLock();
	
	return clktks;
}



/*-------------------------------------------------------------------
//  XTIMERCLOCK::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2013 23:04:44
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void XTIMERCLOCK::Clean()
{
	clockticks	= 0;

	xmutexclock = NULL;
	threadclock = NULL;
}
	


/*-------------------------------------------------------------------
//  XTIMERCLOCK::ThreadClockFunction
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2013 23:14:17
//	
//	@return 			void : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
void XTIMERCLOCK::ThreadClockFunction(void* data)
{
	XTIMERCLOCK* timerclock = (XTIMERCLOCK*)data;
	if(!data) return;

	if(timerclock->xmutexclock) 
		{
			timerclock->xmutexclock->Lock();

			timerclock->clockticks+=17;	

			timerclock->xmutexclock->UnLock();
		}
}


#endif



/*-------------------------------------------------------------------
//  XTIMER::XTIMER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2008 9:09:48
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
XTIMER::XTIMER()		
{ 
	Clean();
}
	


/*-------------------------------------------------------------------
//  XTIMER::~XTIMER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2008 9:09:58
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
XTIMER::~XTIMER()		
{ 
	Clean();
}




/*-------------------------------------------------------------------
//  XTIMER::Add
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2013 19:43:03
//	
//	@return 			void : 
//	@param				moremillisec : 
*/
/*-----------------------------------------------------------------*/
void XTIMER::AddMilliSeconds(XQWORD milliseconds)
{
  this->more += (milliseconds*1000);
}




/*-------------------------------------------------------------------
//  XTIMER::AddSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/05/2010 10:15:05
//	
//	@return				void : 
//	@param				seconds : 
*/
/*-----------------------------------------------------------------*/
void XTIMER::AddSeconds(XQWORD seconds)
{	
	AddMilliSeconds(seconds * 1000);
}




/*-------------------------------------------------------------------
//  XTIMER::SetMilliSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/06/2010 19:26:02
//	
//	@return				void : 
//	@param				elapsed : 
*/
/*-----------------------------------------------------------------*/
void XTIMER::SetMilliSeconds(XQWORD milliseconds)
{
	Reset();

	AddMilliSeconds(milliseconds);
}




//-------------------------------------------------------------------
//  XTIMER::Reset
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2002 14:55:21
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XTIMER::Reset()
{
  more			= 0;
	last			= GetMicroSecondsTickCounter();
}



/*-------------------------------------------------------------------
//  XTIMER::GetMeasureHours
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2013 19:43:32
//	
//	@return 			XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD XTIMER::GetMeasureHours()
{
	XQWORD hours = GetMeasureMilliSeconds();

	hours/=1000;
	hours/=60;
	hours/=60;

	return (XDWORD)hours;
}




/*-------------------------------------------------------------------
//  XTIMER::GetMeasureMinutes
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2013 19:43:42
//	
//	@return 			XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD XTIMER::GetMeasureMinutes()
{
	XQWORD minutes = GetMeasureMilliSeconds();

	minutes/=1000;
	minutes/=60;

	return (XDWORD)minutes;
}




/*-------------------------------------------------------------------
//  XTIMER::GetMeasureSec
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2013 19:43:55
//	
//	@return 			XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD XTIMER::GetMeasureSeconds()
{
	XQWORD seconds = GetMeasureMilliSeconds();

	seconds/=1000L;

	return (XDWORD)seconds;
}



/*-------------------------------------------------------------------
//  XTIMER::GetMeasureMilliSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 10:36:19
//	
//	@return 			XQWORD : 
//	*/
/*-----------------------------------------------------------------*/
XQWORD XTIMER::GetMeasureMilliSeconds()
{
	XQWORD milliseconds = GetMeasureMicroSeconds();

	milliseconds/=1000L;

	return (XDWORD)milliseconds;
}



/*-------------------------------------------------------------------
//  XTIMER::GetMeasureMicroSeconds
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/11/2013 13:20:15
//	
//	@return 			XQWORD : 
//	*/
/*-----------------------------------------------------------------*/
XQWORD XTIMER::GetMeasureMicroSeconds()
{
	XQWORD	elapsed;		
	XQWORD	current = GetMicroSecondsTickCounter();
	
	elapsed = (XQWORD)((current-last) + more);
	
	return elapsed;
}



/*-------------------------------------------------------------------
//	XTIMER::GetMeasureToDate
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/10/2014 9:48:33
//	
//	@return 			bool : 
//
//  @param				xdatetime : 
*/
/*-----------------------------------------------------------------*/
bool XTIMER::GetMeasureToDate(XDATETIME* xdatetime)
{
	if(!xdatetime) return false;
	
	XQWORD allseconds = GetMeasureSeconds();
		
	xdatetime->SetDateFromSeconds(0);
	xdatetime->AddSeconds(allseconds);

	/*
	xdatetime->SetYear(XDATETIME_SECONDSYEARS(allseconds));
	xdatetime->SetMonth(XDATETIME_SECONDSMONTHS(allseconds));
	xdatetime->SetDay(XDATETIME_SECONDSDAYS(allseconds));
	
	xdatetime->SetHours(XDATETIME_SECONDSHOURS(allseconds));
	xdatetime->SetMinutes(XDATETIME_SECONDSMINUTES(allseconds));
	xdatetime->SetSeconds(XDATETIME_SECONDSSECONDS(allseconds));
	*/
	
	return true;
}




/*-------------------------------------------------------------------
//  XTIMER::GetMeasureString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2013 23:50:35
//	
//	@return 			bool : 
//	@param				measure : 
//  @param				large : 
*/
/*-----------------------------------------------------------------*/
bool XTIMER::GetMeasureString(XSTRING& measure,bool large)
{
	XQWORD allseconds = GetMeasureSeconds();

	XDWORD years   = XDATETIME_SECONDSYEARS(allseconds);
	XDWORD months  = XDATETIME_SECONDSMONTHS(allseconds);
	XDWORD days    = XDATETIME_SECONDSDAYS(allseconds);
	
	XDWORD hours   = XDATETIME_SECONDSHOURS(allseconds);
	XDWORD minutes = XDATETIME_SECONDSMINUTES(allseconds);
	XDWORD seconds = XDATETIME_SECONDSSECONDS(allseconds);

	measure.Empty();

	XSTRING string;

	for(int c=0;c<6;c++)
		{
			string.Empty();

			switch(c)
				{
					case 0: if(!years) break;				
									if(years>1) string.Format(__L("%d a\xf1os"), years); else string.Format(__L("un a\xf1o")); 
									break;				

					case 1: if(!months) break;
									if(months>1) string.Format(__L("%d meses"), months);	else string.Format(__L("un mes")); 
									break;

					case 2: if(!days) break;
									if(days>1) string.Format(__L("%d dias"), days);	else string.Format(__L("un dia")); 
									break;

					case 3: if(large)
										{
											if(!hours) break;
											if(hours>1) string.Format(__L("%d horas"), hours); else string.Format(__L("una hora")); 
										}
									 else
										{
											string.Format(__L("%02d"), hours);
										}
									break;

					case 4: if(large)
										{
											if(!minutes) break;
											if(minutes>1) string.Format(__L("%d minutos"), minutes); else string.Format(__L("un minuto")); 
										}
									 else
										{
											string.Format(__L("%02d"), minutes);
										}
									break;

					case 5: if(large)
										{	
											if(!seconds) break;
											if(seconds>1) string.Format(__L("%d segundos"), seconds); else string.Format(__L("un segundo")); 
										}
									 else
									  {
											string.Format(__L("%02d"), seconds);
										}
									break;
			
				}

			if((large)||(c<=3))
				{
					if(!string.IsEmpty()) 
						{
							if(!measure.IsEmpty()) measure += ", ";
							measure += string;
						}
				}
			 else
			  {
					if(!string.IsEmpty()) 
						{
							if(!measure.IsEmpty()) measure += ":";
							measure += string;
						}
				}
		}

	if(measure.IsEmpty()) 
		{
			measure.Format(__L("0 segundos"));	
			return false;
		}

	return true;
}





/*-------------------------------------------------------------------
//  XTIMER::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2010 10:13:06
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void XTIMER::Clean()
{
	last = 0L;
	more = 0L;
}



