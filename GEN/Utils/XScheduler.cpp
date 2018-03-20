/*------------------------------------------------------------------------------------------
//	XSCHEDULER.CPP
//	
//	Scheduler for cyclic tasks
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 27/12/2012 8:48:19
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XFactory.h"
#include "XSleep.h"
#include "XThread.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XDebug.h"

#include "XScheduler.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  XSCHEDULERTASK::XSCHEDULERTASK
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/07/2013 23:13:55
//	
//	@return 			

//  @param				xscheduler : 
*/
/*-----------------------------------------------------------------*/
XSCHEDULERTASK::XSCHEDULERTASK(XSCHEDULER* xscheduler)
{
	Clean();

	this->xscheduler	= xscheduler;

	xtimer	= xfactory->CreateTimer();	
}




/*-------------------------------------------------------------------
//  XSCHEDULERTASK::~XSCHEDULERTASK
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/12/2012 18:24:03
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
XSCHEDULERTASK::~XSCHEDULERTASK()
{
	if(xtimer)
		{
			xfactory->DeleteTimer(xtimer);
			xtimer = NULL;			
		}
	
	Clean();
}




/*-------------------------------------------------------------------
//  XSCHEDULERTASK::SetNCycles
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2012 12:18:56
//	
//	@return 			bool : 
//	@param				ncycles : 
*/
/*-----------------------------------------------------------------*/
bool XSCHEDULERTASK::SetNCycles(int ncyclestodo, XDATETIME* xdatetimecadence)
{
	this->ncyclestodo = ncyclestodo;
	if(this->ncyclestodo<0) this->ncyclestodo = XSCHEDULER_CYCLEFOREVER;

	this->ncyclesactual = this->ncyclestodo;

	if(xdatetimecadence) this->xdatetimecadence.CopyFrom(xdatetimecadence);

	return true;
}




/*-------------------------------------------------------------------
//	XSCHEDULERTASK::SetNCycles
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/10/2014 9:39:10
//	
//	@return 			bool : 
//
//  @param				ncycles : 
//  @param				cadenceinseconds : 
*/
/*-----------------------------------------------------------------*/
bool XSCHEDULERTASK::SetNCycles(int ncyclestodo, XQWORD cadenceinseconds)
{	
	XDATETIME	xdatetimecadence;
	XTIMER		xtimercadence;
	
	xdatetimecadence.SetToZero();
	
	xtimercadence.Reset();
	xtimercadence.AddSeconds(cadenceinseconds);
	
	xtimercadence.GetMeasureToDate(&xdatetimecadence);
	
	return SetNCycles(ncyclestodo, &xdatetimecadence);
}

	
/*-------------------------------------------------------------------
//  XSCHEDULERTASK::SetTimeLimits
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/07/2013 18:52:11
//	
//	@return 			bool : 
//	@param				xtimelimitstart : 
//  @param				xtimelimitend : 
*/
/*-----------------------------------------------------------------*/
bool XSCHEDULERTASK::SetTimeLimits(XDATETIME* xdatetimelimitstart, XDATETIME* xdatetimelimitend)
{
	if(xdatetimelimitstart) this->xdatetimelimitstart.CopyFrom(xdatetimelimitstart);
	if(xdatetimelimitend)		this->xdatetimelimitend.CopyFrom(xdatetimelimitend);

	return false;
}





/*-------------------------------------------------------------------
//  XSCHEDULERTASK::CheckCondition
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/07/2013 8:41:47
//	
//	@return 			bool : 
//	@param				xtimeactual : 
//  @param				xtimeractual : 
*/
/*-----------------------------------------------------------------*/
bool XSCHEDULERTASK::CheckCondition(XDATETIME* xdatetimeactual, XTIMER* xtimeractual)
{
	if(!xdatetimeactual) return false;

	XQWORD timeactualsec	 = xdatetimeactual->GetSeconsFromDate();

	if(xdatetimelimitstart.IsValidDate())
		{
			if(timeactualsec <  xdatetimelimitstart.GetSeconsFromDate()) 
				{
					isinvalidtimelimit = false; 

					xtimeractual->Reset();
					return false;
				}	
			 else isinvalidtimelimit = true; 
		}

	if(xdatetimelimitend.IsValidDate())
		{
			if(timeactualsec >  xdatetimelimitend.GetSeconsFromDate()) 
				{
					isinvalidtimelimit = false; 
					return false;

				} else isinvalidtimelimit = true; 
		}

	if(conditiondayweekmask	!= XSCHEDULER_DAYWEEK_NONE)
		{
			if(!(GetDayOfWeekMask(xdatetimeactual) & conditiondayweekmask))
				{
					xtimeractual->Reset();
					return false;
				}
		}
	
	if(!ncyclesactual)  return false;

	bool checkcadence = true;
	if(isstartimmediatelycycles)
		{
			if(!ncyclesmade)  checkcadence = false;
		}

	if(checkcadence)
		{
			if(xdatetimecadence.IsValidDate())
				{
					if(xtimeractual->GetMeasureSeconds() < xdatetimecadence.GetSeconsFromDate()) return false;			
				}
		}
	
	if(ncyclesactual>0)  ncyclesactual--;

	xtimeractual->Reset();
	
	ncyclesmade++;

	return true;	
}





/*-------------------------------------------------------------------
//  XSCHEDULERTASK::GetDayOfWeekMask
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/07/2013 10:28:24
//	
//	@return 			XBYTE : 
//	@param				xtimeactual : 
*/
/*-----------------------------------------------------------------*/
XBYTE XSCHEDULERTASK::GetDayOfWeekMask(XDATETIME* xtimeactual)
{
	if(!xtimeactual) return XSCHEDULER_DAYWEEK_NONE;

	XBYTE mask = XSCHEDULER_DAYWEEK_NONE;

	switch(xtimeactual->GetDayOfWeek())
		{
			case XDATETIME_DAYWEEK_SUNDAY			: mask = XSCHEDULER_DAYWEEK_ISSUNDAY;			break;
			case XDATETIME_DAYWEEK_MONDAY			: mask = XSCHEDULER_DAYWEEK_ISMONDAY;			break;
			case XDATETIME_DAYWEEK_TUESDAY 		: mask = XSCHEDULER_DAYWEEK_ISTUESDAY;		break;
			case XDATETIME_DAYWEEK_WEDNESDAY	: mask = XSCHEDULER_DAYWEEK_ISWEDNESDAY;	break;
			case XDATETIME_DAYWEEK_THURSDAY		: mask = XSCHEDULER_DAYWEEK_ISTHURSDAY;		break;
			case XDATETIME_DAYWEEK_FRIDAY			: mask = XSCHEDULER_DAYWEEK_ISFRIDAY;			break;
			case XDATETIME_DAYWEEK_SATURDAY		: mask = XSCHEDULER_DAYWEEK_ISSATURDAY;		break;
		}

	return mask;
}



/*-------------------------------------------------------------------
//	XSCHEDULER::XSCHEDULER
*/	
/**	
//	
//	Class Constructor XSCHEDULER
//	
//	@author				Abraham J. Velez
//	@version			09/05/2014 16:26:38
//	
 
 
*/
/*-----------------------------------------------------------------*/
XSCHEDULER::XSCHEDULER()
{
	Clean();
	
	RegisterEvent(XEVENTTYPE_SCHEDULER);

	xtimerwait	= xfactory->CreateTimer();	
}
		
	

/*-------------------------------------------------------------------
//  XSCHEDULER::~XSCHEDULER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/12/2012 19:35:49
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
XSCHEDULER::~XSCHEDULER()
{
	DeRegisterEvent(XEVENTTYPE_SCHEDULER);

	End();

	if(xtimerwait) xfactory->DeleteTimer(xtimerwait);

	Clean();
}





/*-------------------------------------------------------------------
//  XSCHEDULER::Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2012 22:53:07
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XSCHEDULER::Ini()
{
	if(!xfactory) return false;

	xmutexscheduler = xfactory->Create_Mutex();
	if(!xmutexscheduler) return false;	
	
	xdatetimeactual = xfactory->CreateDateTime();
	if(!xdatetimeactual) return false;
	
	xthreadscheduler = CREATEXTHREAD(XTHREADGROUPID_SCHEDULER, __L("XSCHEDULER::Ini"),ThreadScheduler,(void*)this);
	if(!xthreadscheduler) return false;
				
	if(!xthreadscheduler->Ini()) return false;

	isactive = true;
	
	return true;
}




/*-------------------------------------------------------------------
//  XSCHEDULER::Task_Add
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/12/2012 18:19:47
//	
//	@return 			bool : 
//	@param				task : 
*/
/*-----------------------------------------------------------------*/
bool XSCHEDULER::Task_Add(XSCHEDULERTASK* task)
{
	if(!task)	return false;
	
	if(xmutexscheduler) xmutexscheduler->Lock();
	
	if(task->GetXTimer()) task->GetXTimer()->Reset();
  	
	tasks.Add(task);

	if(xmutexscheduler) xmutexscheduler->UnLock();

	return true;
}



/*-------------------------------------------------------------------
//  XSCHEDULER::Task_Get
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/12/2012 16:35:02
//	
//	@return 			XSCHEDULERTASK* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
XSCHEDULERTASK* XSCHEDULER::Task_Get(int index)
{	
	if(tasks.IsEmpty()) return NULL;

	return (XSCHEDULERTASK*)tasks.Get(index);
}



/*-------------------------------------------------------------------
//  XSCHEDULER::Task_GetForID
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/12/2013 11:44:55
//	
//	@return 			XSCHEDULERTASK* : 
//	@param				ID : 
*/
/*-----------------------------------------------------------------*/
XSCHEDULERTASK* XSCHEDULER::Task_GetForID(XDWORD ID)
{	
	if(!xmutexscheduler) return NULL;
	if(tasks.IsEmpty())  return NULL;

	xmutexscheduler->Lock();

	for(XDWORD c=0;c<tasks.GetSize();c++)
		{
			XSCHEDULERTASK* task = tasks.Get(c);
			if(task)
				{
					if(task->GetID()==ID) 
						{
							xmutexscheduler->UnLock();
							return task;
						}	
				}
		}

	xmutexscheduler->UnLock();

	return NULL;
}



/*-------------------------------------------------------------------
//  XSCHEDULER::Task_Del
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/12/2012 18:20:11
//	
//	@return 			bool : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
bool XSCHEDULER::Task_Del(int index)
{
	if(!xmutexscheduler) return false;
	if(tasks.IsEmpty())  return false;
	
	bool status = false;

	xmutexscheduler->Lock();
	
	XSCHEDULERTASK* task = Task_Get(index);	
	if(task) 
		{	
			status = tasks.Delete(task);
			delete task;
		}

	xmutexscheduler->UnLock();
		
	return status;
}




/*-------------------------------------------------------------------
//  XSCHEDULER::Task_DelForID
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/12/2013 11:51:04
//	
//	@return 			XSCHEDULERTASK* : 
//	@param				ID : 
*/
/*-----------------------------------------------------------------*/
bool XSCHEDULER::Task_DelForID(XDWORD ID)
{	
	if(!xmutexscheduler) return false;
	if(tasks.IsEmpty())  return false;

	xmutexscheduler->Lock();

	bool status = false;

	for(XDWORD c=0;c<tasks.GetSize();c++)
		{
			XSCHEDULERTASK* task = tasks.Get(c);
			if(task)
				{
					if(task->GetID()==ID) 
						{							
						  status = tasks.Delete(task); 
							delete task;
							break;
						}
				}
		}

	xmutexscheduler->UnLock();

	return status;
}





/*-------------------------------------------------------------------
//  XSCHEDULER::Task_DelAll
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/12/2012 18:20:29
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XSCHEDULER::Task_DelAll()
{
	if(tasks.IsEmpty())  return false;

	if(xmutexscheduler) xmutexscheduler->Lock();
	
	tasks.DeleteContents();	
  tasks.DeleteAll();

	if(xmutexscheduler) xmutexscheduler->UnLock();

	return true;
}



/*-------------------------------------------------------------------
//  XSCHEDULER::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2012 22:56:17
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XSCHEDULER::End()
{
	isactive = false;
	
	if(xthreadscheduler)     
		{					
			xthreadscheduler->End();
			DELETEXTHREAD(XTHREADGROUPID_SCHEDULER, xthreadscheduler);
		
			xthreadscheduler = NULL;
		}

	Task_DelAll();

	if(xmutexscheduler) 
		{
			xfactory->Delete_Mutex(xmutexscheduler);
			xmutexscheduler = NULL;
		}

	if(xdatetimeactual) 
		{
			xfactory->DeleteDateTime(xdatetimeactual);
			xdatetimeactual = NULL;
		}

	return true;
}






/*-------------------------------------------------------------------
//  XSCHEDULER::ThreadScheduler
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/12/2012 20:12:44
//	
//	@return 			void : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
void XSCHEDULER::ThreadScheduler(void* data)
{	
	XSCHEDULER* xscheduler = (XSCHEDULER*)data;
	if(!xscheduler)											 return;
	if(!xscheduler->xmutexscheduler)		 return;
	if(!xscheduler->xdatetimeactual)		 return;			
	
	xsleep->MilliSeconds(10);
													
	if(!xscheduler->isactive)  return;

	xscheduler->xmutexscheduler->Lock();
	
	XSCHEDULERTASK* task; 

	task = xscheduler->Task_Get(xscheduler->indextask);
	if(!task) 
		{
			xscheduler->xmutexscheduler->UnLock();	
			xscheduler->indextask = 0;
			return;
		}

	if(xscheduler->xdatetimeactual) xscheduler->xdatetimeactual->Read();

	if(task->IsActive())
		{				
			if(task->CheckCondition(xscheduler->xdatetimeactual, task->GetXTimer()))
				{	
					xscheduler->indextask++;
					xscheduler->xmutexscheduler->UnLock();	

					XSCHEDULERXEVENT xevent(xscheduler, XEVENTTYPE_SCHEDULER);
					
					xevent.SetScheduler(xscheduler);
					xevent.SetTask(task);
					xevent.SetDateTime(xscheduler->xdatetimeactual);
							
					xscheduler->PostEvent(&xevent);					

					return;
				}
		}

	xscheduler->indextask++;
	xscheduler->xmutexscheduler->UnLock();		
}