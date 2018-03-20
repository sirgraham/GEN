
/*------------------------------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG.CPP
//	
//  Data IO Sensor Light LDR (Light Dependent Resistor) Analog in port digital
//  
//  Diagram:
//	http://www.raspberrypi-spy.co.uk/2012/08/reading-analogue-sensors-with-one-gpio-pin/
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 22/03/2014 17:31:07
//	Last Modification	:	
//	
//	(C) Copyright EndoraSoft S.L.. All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XFactory.h"
#include "XSleep.h"
#include "XThreadCollected.h"
#include "XTimer.h"

#include "DIOLightSensorLDRAnalog.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG::DIOLIGHTSENSORLDRANALOG
*/	
/**	
//	
//	Class Constructor DIOLIGHTSENSORLDRANALOG
//	
//	@author				Abraham J. Velez
//	@version			22/03/2014 17:33:55
//	
 
//  @param				diogpio : 
//  @param				pindata : 
//  @param				activatecache : 
*/
/*-----------------------------------------------------------------*/
DIOLIGHTSENSORLDRANALOG::DIOLIGHTSENSORLDRANALOG( DIOGPIO* diogpio, int pindata, bool activatecache) : DIODEVICE()
{ 
	Clean();												

	this->diogpio	= diogpio;
	this->pindata			= pindata;

	xtimer = xfactory->CreateTimer();

	if(activatecache)
		{
			xmutexread  = xfactory->Create_Mutex();	

			if(xmutexread)
				{
					threadcache  = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAM, __L("DIOLIGHTSENSORLDRANALOG::DIOLIGHTSENSORLDRANALOG"), ThreadRunFunction, (void*)this);
					if(!threadcache) return;		

					//threadcache->SetPriority(THREADPRIORITY_REALTIME);	
				}
		}
} 



/*-------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG::~DIOLIGHTSENSORLDRANALOG
*/	
/**	
//	
//	 Class Destructor DIOLIGHTSENSORLDRANALOG
//	
//	@author				Abraham J. Velez
//	@version			22/03/2014 17:34:01
//	
*/
/*-----------------------------------------------------------------*/
DIOLIGHTSENSORLDRANALOG::~DIOLIGHTSENSORLDRANALOG()
{ 
	End();

	if(threadcache) 
		{
			threadcache->End();
			DELETEXTHREAD(XTHREADGROUPID_DIOSTREAM, threadcache);			
		}

	if(xmutexread)  xfactory->Delete_Mutex(xmutexread);

	if(xtimer) xfactory->DeleteTimer(xtimer);

	Clean();													
}



/*-------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG::Ini
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/09/2014 19:47:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOLIGHTSENSORLDRANALOG::Ini()
{
	if(!threadcache) return false;

	if(!threadcache->Ini()) return false;

	return DIODEVICE::Ini();
}


/*-------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG::ReadFromCache
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2014 17:49:40
//	
//	@return 			bool : 
//
//  @param				level : 
*/
/*-----------------------------------------------------------------*/
bool DIOLIGHTSENSORLDRANALOG::ReadFromCache(XDWORD& level)
{
	if(!xmutexread) return false;

	if(!IsInitialized()) return false;

	if(!nreads) return false;
	
	xmutexread->Lock(); 
	
	level = levelcache;
	
	xmutexread->UnLock(); 
		
	return true;

}



/*-------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG::ReadDirect
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2014 17:49:47
//	
//	@return 			bool : 
//
//  @param				level : 
*/
/*-----------------------------------------------------------------*/
bool DIOLIGHTSENSORLDRANALOG::ReadDirect(XDWORD& level)
{
	if(!diogpio)			return false;
	if(!xtimer)						return false;
	if(!IsInitialized())	return false;

	level = 0;

	xtimer->Reset();

	// Discharge capacitor
	diogpio->SetMode(pindata, false);
	diogpio->Set(pindata    , false);  
	xsleep->MilliSeconds(100);
													
	diogpio->SetMode(pindata,  true);
	while(!diogpio->Get(pindata))  
		{
			level++;
			if(!(level%100))
				{
					if(xtimer->GetMeasureSeconds()>=1) return false;
				}
		}

	if(!level) return false;

	return true;
}



/*-------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG::End
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/09/2014 19:48:25
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOLIGHTSENSORLDRANALOG::End()
{
	if(!DIODEVICE::End()) return false;

	if(threadcache) threadcache->End();

	return true;
}




/*-------------------------------------------------------------------
//	DIOLIGHTSENSORLDRANALOG::ThreadRunFunction
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2014 17:34:09
//	
//  @param				thread : 
*/
/*-----------------------------------------------------------------*/
void DIOLIGHTSENSORLDRANALOG::ThreadRunFunction(void* param)
{	
	DIOLIGHTSENSORLDRANALOG* sensor = (DIOLIGHTSENSORLDRANALOG*)param;
	if(!sensor) return;

	if(!sensor->IsInitialized()) return;
			
	XDWORD level;
			
	bool status	= sensor->ReadDirect(level);			
	if(status)
		{										
			if(sensor->xmutexread) sensor->xmutexread->Lock(); 

			sensor->levelcache	= level;
			sensor->nreads++;	
					
			if(sensor->xmutexread) sensor->xmutexread->UnLock(); 	
		}	
	
	if(sensor->xtimer) 
		{
			if(sensor->threadcache->IsRunning()) xsleep->Seconds(1);	
		}
}
