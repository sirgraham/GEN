//------------------------------------------------------------------------------------------
//	XLINUXTHREAD.CPP
//	
//	LINUX thread class 
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 06/03/2006 11:46:26
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <errno.h>

#include "XDebug.h"

#include "XLINUXThread.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	

//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  XLINUXMUTEX::XLINUXMUTEX
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2011 11:43:12
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XLINUXMUTEX::XLINUXMUTEX()
{
	Clean();

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	if(!pthread_mutex_init(&mutex, &attr)) init = true;
}


/*-------------------------------------------------------------------
//  XLINUXMUTEX::~XLINUXMUTEX
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2011 11:43:31
//	
//	@return				virtual : 
//	*/
/*-----------------------------------------------------------------*/
XLINUXMUTEX::~XLINUXMUTEX()
{
	if(init)
		{			
			pthread_mutex_destroy(&mutex);
			init = false;			
		}

	Clean();
}

		 

/*-------------------------------------------------------------------
//  XLINUXMUTEX::Lock
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2011 11:44:27
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XLINUXMUTEX::Lock()
{
	if(!init)											 return false;	

	if(pthread_mutex_lock(&mutex)) return false;

	islock = true;

	return true;
}
		 

/*-------------------------------------------------------------------
//  XLINUXMUTEX::UnLock
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2011 11:44:30
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XLINUXMUTEX::UnLock()
{
	if(!init)												 return false;	
	if(pthread_mutex_unlock(&mutex)) return false;

	islock = false;

	return true;
}


/*-------------------------------------------------------------------
//  XLINUXMUTEX::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2011 11:44:34
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void XLINUXMUTEX::Clean()
{
	init = false;
}





/*-------------------------------------------------------------------
//	XLINUXTHREAD::XLINUXTHREAD
*/	
/**	
//	
//	Class Constructor XLINUXTHREAD
//	
//	@author				Abraham J. Velez
//	@version			21/04/2016 12:20:05
//	
//  @param				groupID : 
//  @param				ID : 
//  @param				function : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
XLINUXTHREAD::XLINUXTHREAD(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* data) : XTHREAD(groupID, ID, function, data)
{
	Clean();
}


//-------------------------------------------------------------------
//  XLINUXTHREAD::~XLINUXTHREAD
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2006 12:03:47
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XLINUXTHREAD::~XLINUXTHREAD()
{
	End();

	Clean();
}


//-------------------------------------------------------------------
//  XLINUXTHREAD::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2006 12:03:47
//	
//	@return				
//	*/
//-------------------------------------------------------------------
bool XLINUXTHREAD::Ini(bool run)
{	
	if(run)
		{
			if(!Run(true)) return false;
		}
	 else 
		{
			statusfunc = XTHREADSTATUS_STOP;
		}
	
	pthread_attr_t attr;

	if(!stacksize) SetStackSize(1024*1024);		

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, stacksize);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	int _status = pthread_create(&thhandle, &attr, Callback, this);
	if(!_status)
		{		
			XSTRING_CREATEOEM(ID, charstr)		
			if(ID.GetSize()>=16) charstr[15] = 0;
			//pthread_setname_np(thhandle, charstr);
			XSTRING_DELETEOEM(charstr)
		}
	 else		
		{
			thhandle = 0;		
			// If pthread_create() fails, no new thread is created and the contents of the location referenced by thread are UNDEFINED.			
		}

	pthread_attr_destroy(&attr);

	return (!_status)?true:false;
	

}




/*-------------------------------------------------------------------
//  XLINUXTHREAD::Wait
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2011 11:16:03
//	
//	@return				bool : 
//	@param				miliseconds : 
*/
/*-----------------------------------------------------------------*/
bool XLINUXTHREAD::Wait(int miliseconds)
{
	usleep(1000*(miliseconds?miliseconds:1));

	return true;
}



//-------------------------------------------------------------------
//  XLINUXTHREAD::End
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2006 12:03:47
//	
//	@return				
//	*/
//-------------------------------------------------------------------
bool XLINUXTHREAD::End()
{
	if(statusfunc == XTHREADSTATUS_END) return false;

	statusfunc = XTHREADSTATUS_EXIT;

	WaitToEnd();

	if(thhandle) 
		{
			pthread_join(thhandle, NULL);	
			thhandle = 0;
		}

	return true;
}



/*-------------------------------------------------------------------
//  XLINUXTHREAD::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/11/2008 09:33:02 p.m.
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
void XLINUXTHREAD::Clean()
{
	thhandle = 0;	
}



//-------------------------------------------------------------------
//  void* XLINUXTHREAD::Callback
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2006 12:14:02
//	
//	@return				static : 
//	@param				thread : 
*/
//-------------------------------------------------------------------
void* XLINUXTHREAD::Callback(void* thread)
{	
	XLINUXTHREAD*		 th   = NULL;
	XTHREADFUNCTION  func = NULL;
	void*						 param;

  th = (XLINUXTHREAD*)thread;
	if(!th)	return NULL;

	func = (XTHREADFUNCTION)th->GetFunction();
	if(!func) return NULL;

	param = th->GetParam();

	th->gotofunction = true;

	if((th->priority!=XTHREADPRIORITY_UNKNOWN) && (th->priority!=XTHREADPRIORITY_NORMAL))
		{
			 struct sched_param param;  // scheduling priority
       int policy = SCHED_FIFO;     // scheduling policy

			switch(th->priority)
				{
					#ifdef SCHED_IDLE
					case XTHREADPRIORITY_LOW			: policy = SCHED_IDLE;
																					param.sched_priority = sched_get_priority_min(policy);
																					break;
					#endif

					case XTHREADPRIORITY_HIGH			: policy = SCHED_FIFO;
																					param.sched_priority = sched_get_priority_max(policy)/2;	 
																					break;

					case XTHREADPRIORITY_REALTIME	: policy = SCHED_FIFO;
																					param.sched_priority = sched_get_priority_max(policy);	 
																					break;
				}
			
      pthread_setschedparam(th->thhandle, policy, &param);
		}
		
	while(1)
		{			
			if(!th) break;	
	
			if(th->statusfunc == XTHREADSTATUS_EXIT)     break;
			if(th->statusfunc == XTHREADSTATUS_END)  break;
			
			if(th->statusfunc == XTHREADSTATUS_RUN)
				{
					if(func) 						
						{	  
							th->isinfunction = true;
							func(param); 
							th->isinfunction = false;

						} else break;
				}
			
			if(!th) break;

			if(th->waityield) usleep(th->waityield*1000);				
		}

			
	if(th) th->statusfunc = XTHREADSTATUS_END;

	pthread_exit(0);
		
	return NULL;
}


