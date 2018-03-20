//------------------------------------------------------------------------------------------
//	XSTM32FXXXFACTORY.CPP
//	
//	STM32Fxxx utils platform factory class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 08/08/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XThreadCollected.h"

#include "XSTM32FxxxDateTime.h"
#include "XSTM32FxxxTimer.h"
#include "XSTM32FxxxRand.h"
#include "XSTM32FxxxSystem.h"
#include "XSTM32FxxxThread.h"

#include "XFactoryMacros.h"

#include "XSTM32FxxxFactory.h"

#include "XMemory.h"

	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------

CREATEFUNC(XSTM32FXXXFACTORY	, XTIMER		 , XSTM32FXXXTIMER      , CreateTimer)
DELETEFUNC(XSTM32FXXXFACTORY	, XTIMER		 , XSTM32FXXXTIMER			, DeleteTimer)

CREATEFUNC(XSTM32FXXXFACTORY	, XDATETIME	 , XSTM32FXXXDATETIME		, CreateDateTime)
DELETEFUNC(XSTM32FXXXFACTORY	, XDATETIME	 , XSTM32FXXXDATETIME		, DeleteDateTime)

CREATEFUNC(XSTM32FXXXFACTORY	, XRAND			 , XSTM32FXXXRAND				, CreateRand)
DELETEFUNC(XSTM32FXXXFACTORY	, XRAND			 , XSTM32FXXXRAND				, DeleteRand)

CREATEFUNC(XSTM32FXXXFACTORY	, XSYSTEM		 , XSTM32FXXXSYSTEM			, CreateSystem)
DELETEFUNC(XSTM32FXXXFACTORY	, XSYSTEM		 , XSTM32FXXXSYSTEM			, DeleteSystem)

CREATEFUNC(XSTM32FXXXFACTORY	, XMUTEX		 , XSTM32FXXXMUTEX	   	, Create_Mutex)
DELETEFUNC(XSTM32FXXXFACTORY	, XMUTEX		 , XSTM32FXXXMUTEX	   	, Delete_Mutex)


XTHREAD* XSTM32FXXXFACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function,void* data)
{
	XSTM32FXXXTHREAD* _class =  new XSTM32FXXXTHREAD(groupID, ID,function,data);
	return (XTHREAD*)_class;
}


bool XSTM32FXXXFACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
{ 
	if(!xthread) return false;

	XSTM32FXXXTHREAD* _xthread = (XSTM32FXXXTHREAD*)xthread;	
	delete _xthread;
	
	return true;
};		


