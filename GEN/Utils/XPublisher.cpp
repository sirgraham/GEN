//------------------------------------------------------------------------------------------
//	XPUBLISHER.CPP
//	
//	Basic eXtended Publisher Class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 09/01/2004 17:31:44
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XFactory.h"
#include "XObserver.h"
#include "XDebug.h"

#include "XPublisher.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------

XPUBLISHER*		XPUBLISHER::instance = NULL;

//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  XPUBLISHER::XPUBLISHER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2004 9:57:11
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XPUBLISHER::XPUBLISHER()
{
	Clean();

	subjectsmap.SetIsMulti(true);

	if(xfactory) xmutexevents = xfactory->Create_Mutex();
}


//-------------------------------------------------------------------
//  XPUBLISHER::~XPUBLISHER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2004 9:57:34
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XPUBLISHER::~XPUBLISHER()
{		
	DeRegisterAllEvents();

	if(xfactory)
		{
			if(xmutexevents) xfactory->Delete_Mutex(xmutexevents);
		}
	
	Clean();
}




//-------------------------------------------------------------------
//  XPUBLISHER::RegisterEvent
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 16:23:42
//	
//	@return				bool : 
//	@param				type : 
//  @param				evencityer : 
*/
//-------------------------------------------------------------------
bool XPUBLISHER::RegisterEvent(XDWORD type, XSUBJECT* subject)
{
	bool status = false;

	if(xmutexevents) xmutexevents->Lock();

	bool found = false;

	for(XDWORD c=0; c<subjectsmap.GetSize(); c++)
		{
			XDWORD    _type    = subjectsmap.GetKey(c);
			XSUBJECT* _subject = subjectsmap.GetElement(c);

			if((_type == type) && (_subject == subject)) 
				{
					found = true;
					break;
				}
		}
	
	if(!found) status = subjectsmap.Add(type, subject);
		
	if(xmutexevents) xmutexevents->UnLock();

	return status;
}




/*-------------------------------------------------------------------
//	XPUBLISHER::DeRegisterEvent
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2016 20:24:51
//	
//	@return 			bool : 
//
//  @param				type : 
//  @param				subject : 
*/
/*-----------------------------------------------------------------*/
bool XPUBLISHER::DeRegisterEvent(XDWORD type, XSUBJECT* subject)
{
	bool status = false;
	
	if(xmutexevents) xmutexevents->Lock();

	for(XDWORD c=0; c<subjectsmap.GetSize(); c++)
		{
			XDWORD    _type    = subjectsmap.GetKey(c);
			XSUBJECT* _subject = subjectsmap.GetElement(c);

			if((_type == type) && (_subject == subject))
				{

					status = subjectsmap.Delete(type, subject);
					if(status)
						{
							XDWORD d=0;
							while(d<eventsvector.GetSize())
								{	
									XPUBLISHERENTRY* publisherentry = eventsvector.Get(d);
									if(publisherentry)
										{
											if((publisherentry->type == type) && (publisherentry->subject == subject))
												{													 
													eventsvector.Delete(publisherentry);
													delete publisherentry;

												} else d++;

										} else d++;									
								}
						}
				}
		}
	
	if(xmutexevents) xmutexevents->UnLock();

	return status;
}




/*-------------------------------------------------------------------
//	XPUBLISHER::DeRegisterAllEvents
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2016 20:27:35
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XPUBLISHER::DeRegisterAllEvents()
{
	if(xmutexevents) xmutexevents->Lock();
	
	subjectsmap.DeleteAll();
		
	eventsvector.DeleteContents();
	eventsvector.DeleteAll();

	if(xmutexevents) xmutexevents->UnLock();

	return true;
}
		



/*-------------------------------------------------------------------
//	XPUBLISHER::SubscribeEvent
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2016 20:32:08
//	
//	@return 			bool : 
//
//  @param				type : 
//  @param				subject : 
//  @param				observer : 
*/
/*-----------------------------------------------------------------*/
bool XPUBLISHER::SubscribeEvent(XDWORD type, XSUBJECT* subject, XOBSERVER* observer)
{
	if(!subject)	return false;			
	if(!observer) return false;
	
	if(xmutexevents) xmutexevents->Lock();

	bool status = false;
	bool found  = false;

	for(XDWORD c=0; c<subjectsmap.GetSize(); c++)
		{
			XDWORD    _type    = subjectsmap.GetKey(c);
			XSUBJECT* _subject = subjectsmap.GetElement(c);

			if((_type == type) && (_subject == subject)) 
				{
					found = true;
					break;
				}
		}
 
	if(found)
		{
			found = false;

			for(XDWORD c=0; c<eventsvector.GetSize(); c++)
				{
					XPUBLISHERENTRY* publisherentry = eventsvector.Get(c);
					if(publisherentry)
						{
							if((publisherentry->type == type) && (publisherentry->subject == subject) && (publisherentry->observer == observer))
								{
									found = true;
								} 
						}
				}
		 
			if(!found)
				{
					XPUBLISHERENTRY* publisherentry = new XPUBLISHERENTRY();
					if(publisherentry)
						{
							publisherentry->type		 = type;
							publisherentry->subject  = subject;
							publisherentry->observer = observer;

							status = eventsvector.Add(publisherentry);					
						}
				}
		}

	if(xmutexevents) xmutexevents->UnLock();

	return status;
}




/*-------------------------------------------------------------------
//	XPUBLISHER::UnSubscribeEvent
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2016 21:22:08
//	
//	@return 			bool : 
//
//  @param				type : 
//  @param				subject : 
//  @param				observer : 
*/
/*-----------------------------------------------------------------*/
bool XPUBLISHER::UnSubscribeEvent(XDWORD type, XSUBJECT* subject, XOBSERVER* observer)
{
	bool status = false;

	if(xmutexevents) xmutexevents->Lock();
	
	for(XDWORD c=0; c<eventsvector.GetSize(); c++)
		{
			XPUBLISHERENTRY* publisherentry = eventsvector.Get(c);
			if(publisherentry)
				{
					if((publisherentry->type == type) && (publisherentry->subject == subject) && (publisherentry->observer == observer))
					{
							eventsvector.Delete(publisherentry);
							delete publisherentry;
							
							status = true;
							break;					
					}
				}
		}

	if(xmutexevents) xmutexevents->UnLock();

	return status;
}






/*-------------------------------------------------------------------
//	XPUBLISHER::PostEvent
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/03/2016 21:30:16
//	
//	@return 			bool : 
//
//  @param				event : 
//  @param				subject : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool XPUBLISHER::PostEvent(XEVENT* event, XSUBJECT* subject)
{
	if(!on) 
		{ 
			return false;
		}
	
	inevent = true;
	
	XVECTOR<XOBSERVER*>  observers;

	if(xmutexevents) xmutexevents->Lock();

	for(XDWORD c=0; c<eventsvector.GetSize(); c++)
		{
			XPUBLISHERENTRY* publisherentry = eventsvector.Get(c);
			if(publisherentry)
				{
					if((publisherentry->type == event->GetEventType()) && (publisherentry->subject == subject))
						{					
							observers.Add(publisherentry->observer);														
						} 
				}
		}	

	if(xmutexevents) xmutexevents->UnLock();

	if(!observers.GetSize()) return false;

	for(XDWORD c=0; c<observers.GetSize(); c++)
		{
			XOBSERVER* observer = observers.Get(c);
			if(observer) observer->HandleEvent(event);
		}

	observers.DeleteAll();

	inevent = false;

	return true;
}





