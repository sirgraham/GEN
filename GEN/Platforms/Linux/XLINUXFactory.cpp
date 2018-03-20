//------------------------------------------------------------------------------------------
//	XLINUXFACTORY.CPP
//	
//	LINUX utils platform factory class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 08/08/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------


#include "XLINUXDateTime.h"
#include "XLINUXTimer.h"
#include "XLINUXRand.h"
#include "XLINUXFile.h"
#include "XLINUXDir.h"
#include "XLINUXSystem.h"
#include "XLINUXConsole.h"
#include "XLINUXThread.h"

#include "XFactoryMacros.h"

#include "XLINUXFactory.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------

CREATEFUNC(XLINUXFACTORY, XTIMER		 , XLINUXTIMER      , CreateTimer)
DELETEFUNC(XLINUXFACTORY, XTIMER		 , XLINUXTIMER			, DeleteTimer)

CREATEFUNC(XLINUXFACTORY, XDATETIME	 , XLINUXDATETIME		, CreateDateTime)
DELETEFUNC(XLINUXFACTORY, XDATETIME	 , XLINUXDATETIME		, DeleteDateTime)

CREATEFUNC(XLINUXFACTORY, XRAND			 , XLINUXRAND				, CreateRand)
DELETEFUNC(XLINUXFACTORY, XRAND			 , XLINUXRAND				, DeleteRand)

CREATEFUNC(XLINUXFACTORY, XFILE			 , XLINUXFILE				, Create_File)
DELETEFUNC(XLINUXFACTORY, XFILE			 , XLINUXFILE				, Delete_File)

XDIR* XLINUXFACTORY::Create_Dir()
{
	XLINUXDIR* _class =  new XLINUXDIR();
	return (XDIR*)_class;
}
DELETEFUNC(XLINUXFACTORY, XDIR			 , XLINUXDIR				, Delete_Dir)

XSYSTEM* XLINUXFACTORY::CreateSystem()
{
	XLINUXSYSTEM* _class =  new XLINUXSYSTEM();
	return (XSYSTEM*)_class;
}
DELETEFUNC(XLINUXFACTORY, XSYSTEM		 , XLINUXSYSTEM			, DeleteSystem)


XCONSOLE* XLINUXFACTORY::CreateConsole()
{
	XLINUXCONSOLE* _class =  new XLINUXCONSOLE();
	return (XCONSOLE*)_class;
}
DELETEFUNC(XLINUXFACTORY, XCONSOLE	 , XLINUXCONSOLE		, DeleteConsole)


CREATEFUNC(XLINUXFACTORY, XMUTEX		 , XLINUXMUTEX	   	, Create_Mutex)
DELETEFUNC(XLINUXFACTORY, XMUTEX		 , XLINUXMUTEX	   	, Delete_Mutex)



XTHREAD* XLINUXFACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function,void* data)
{
	XLINUXTHREAD* _class =  new XLINUXTHREAD(groupID, ID,function,data);
	return (XTHREAD*)_class;
}


bool XLINUXFACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
{ 
	if(!xthread) return false;

	XLINUXTHREAD* _xthread = (XLINUXTHREAD*)xthread;	
	delete _xthread;
	
	return true;
};		


