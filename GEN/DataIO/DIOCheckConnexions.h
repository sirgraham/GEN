/*------------------------------------------------------------------------------------------
//	DIOCHECKCONNEXIONS.H
*/	
/**	
// \class 
//   
//  Data IO Check Connexions class
//   
//	@author	 Abraham J. Velez
//	@version 05/11/2007 16:20:28
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOCHECKCONNEXIONS_H_
#define _DIOCHECKCONNEXIONS_H_
		
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XDateTime.h"
#include "XRand.h"
#include "XTimer.h"
#include "XVector.h"
#include "XString.h"
#include "XThreadCollected.h"

#include "HashCRC32.h"	

#include "DIOFactory.h"
#include "DIOURL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOCHECKCONNEXIONS_DEFAULTTIMECONNEXIONCHECKS			 10   // seconds 

#define DIOCHECKCONNEXIONS_DEFAULTNCHECKSFOREVERYCONNEXION				  5  
#define DIOCHECKCONNEXIONS_DEFAULTTIMERCONNEXIONCHECK			100   // milliseconds

#define DIOCHECKCONNEXIONS_MAXNCONNEXIONCUTS											250

#define DIOCHECKCONNEXIONS_INFINITECUT														 -1

/*---- CLASS -----------------------------------------------------------------------------*/
	
class XFACTORY;
class XPUBLISHER;
class XRAND;
class XBUFFER;
class DIOFACTORY;
class DIOPING;


class DIOCHECKCONNEXIONCUT
{
	public:	
																						DIOCHECKCONNEXIONCUT									()																
																						{ 
																							Clean();																				
																							
																							startdatetime = xfactory->CreateDateTime();
																						}	

		virtual																 ~DIOCHECKCONNEXIONCUT									()																
																						{ 
																							if(startdatetime) xfactory->DeleteDateTime(startdatetime);
																							
																							Clean();																				
																						}
		
		XDATETIME*															GetStartDateTime											()																{ return startdatetime;														}

		int																			GetMeasureNSeconds										()																{ return nseconds;																}	
		void																		SetMeasureNSeconds										(XDWORD nseconds)									{ this->nseconds = nseconds;											}

		bool																		GetMeasureTimeString									(XSTRING& measuretime, bool large = false)
																						{
																							XTIMER xtimer;

																							xtimer.Reset();
																							xtimer.AddSeconds(GetMeasureNSeconds());

																							xtimer.GetMeasureString(measuretime, large);

																							return true;	
																						}
		

		bool																		CopyFrom															(DIOCHECKCONNEXIONCUT* connexioncut)
																						{
																							if(!connexioncut)		return false;

																							startdatetime->CopyTo(connexioncut->GetStartDateTime());
																							nseconds		= connexioncut->GetMeasureNSeconds();	
		
																							return true;
																						}

		bool																		CopyTo																(DIOCHECKCONNEXIONCUT* connexioncut)
																						{
																							if(!connexioncut)		return false;
																						
																							connexioncut->GetStartDateTime()->CopyFrom(startdatetime);	
																							connexioncut->SetMeasureNSeconds(nseconds);	

																							return true;
																						}

	private:

		void																		Clean																	()
																						{				
																							startdatetime		= NULL;		
																							nseconds				= 0;
																						}
				
		XDATETIME*															startdatetime;
		int																			nseconds;
};






class DIOCHECKCONNEXION
{
	public:	
																						DIOCHECKCONNEXION											()
																						{
																							Clean();
																																														
																							if(diofactory) url = diofactory->CreateURL();		
																									
																						}

		virtual																 ~DIOCHECKCONNEXION											()
																						{																							
																							if(url)  diofactory->DeleteURL(url);
																							
																							Clean();
																						}
		
		XDWORD																	GetID	                                ()																{ return ID;																			}	
		DIOURL*																	GetURL                                ()																{ return url;																			}

		bool																		IsConnected														()																{ return isconnected;                             }
		void																		SetIsConnected												(bool isconnected)								{ this->isconnected = isconnected;                }

		XDWORD																	GetNChecks														()																{ return nchecks;																	}
		void																		SetNChecks														(XDWORD nchecks)									{ this->nchecks = nchecks;												}
		XDWORD																	IncNChecks														()																{ return ++nchecks;																}

		bool																		Set																		(XCHAR* url)
																						{
																							if(!this->url)  return 0;

																							(*this->url) = url;
																							
																							this->url->Slash_Delete();
																							
																							ID = CreateID();

																							return ID?true:false;
																						}		
	private:

		void																		Clean																	()
																						{																																																	
																							ID										= 0;
																							url										= NULL;	
																							isconnected						= false;
																							nchecks								= 0;																							
																						}

		XDWORD																	CreateID															()
																						{
																							HASHCRC32 hashcrc32;																							
																							XDWORD    _ID = 0;

																							XSTRING_CREATEOEM((*url), charstr)	
																							if(hashcrc32.Do((XBYTE*)charstr, url->GetSize())) _ID += hashcrc32.GetResultCRC32();																								
																							XSTRING_DELETEOEM(charstr)

																							return _ID;
																						}	

		XDWORD																	ID;
		DIOURL*																	url;
		bool																		isconnected;
		XDWORD																	nchecks;
};





class DIOCHECKCONNEXIONS
{
	public:	
																						DIOCHECKCONNEXIONS										();
		virtual																 ~DIOCHECKCONNEXIONS										();

		bool																		Ini                                   (int timerconnexionscheck = DIOCHECKCONNEXIONS_DEFAULTTIMECONNEXIONCHECKS, bool validsomeisconnected = false,  bool dispersionmode = false);

		bool																		Run                                   (bool activate = true)
																						{
																							if(!threadcheckconnexions) return false;
																							return threadcheckconnexions->Run(activate);
																						}	
		
		int																			GetTimeConnexionChecks								()																			{ return timeconnexionchecks;							}

		bool																		IsCheckTimeConnexions									()																			{ return ischecktimeconnexions;						}	
		void																		SetIsCheckTimeConnexions							(bool ischecktimeconnexions)
																						{ 
																							this->ischecktimeconnexions = ischecktimeconnexions;						
																						}	

		
		void																		Setup																	(int timeconnexionchecks = DIOCHECKCONNEXIONS_DEFAULTTIMECONNEXIONCHECKS, bool validsomeisconnected = false, bool dispersionmode = false)					
																						{ 
																							this->timeconnexionchecks		= timeconnexionchecks; 	
																							this->validsomeisconnected	= validsomeisconnected;
																							this->dispersionmode				= dispersionmode;
																						}
							
		bool																		Connexion_Add													(XCHAR* url, XDWORD& ID);
		bool																		Connexion_Add													(XSTRING& url, XDWORD& ID)				
																						{ 
																							return Connexion_Add(url.Get(), ID);							
																						}

		bool																		Connexion_Add													(DIOURL& url, XDWORD& ID)					
																						{ 
																							return Connexion_Add(url.Get(), ID);							
																						}		

		DIOCHECKCONNEXION*											Connexion_GetByID						          (XDWORD ID); 
		bool																		Connexion_IsActive										(XDWORD ID, bool& isactive);		
		bool																		Connexion_DeleteByID									(XDWORD ID);
		
		bool																		Connexions_IsAllChecked								();
		bool																		Connexions_IsSomeChecked							();
		bool																		Connexions_WaitToAllChecked						(int timeout);
		bool																		Connexions_WaitToSomeIsChecked				(int timeout);
		bool																		Connexions_SomeIsConnected						();

		bool																		Connexions_DeleteAll									();

		bool																		End	                                  ();
							
		
	private:

		void																		Clean																	()
																						{																									
																							timeconnexionchecks									= DIOCHECKCONNEXIONS_DEFAULTTIMECONNEXIONCHECKS;																							
																							validsomeisconnected								= false;
																							dispersionmode											= false;

																							xtimerfortimeconnexionchecks				= NULL;
																							ischecktimeconnexions								= false;
																							ping																= NULL;
																							xmutexconnexions										= NULL;
																							threadcheckconnexions								= NULL;
																						}

		static void															ThreadCheckConnexions									(void* param);

		int																			timeconnexionchecks;	
		bool																		validsomeisconnected;
		bool																		dispersionmode;

		XTIMER*																	xtimerfortimeconnexionchecks;
		bool																		ischecktimeconnexions;
		DIOPING*																ping;
		XMUTEX*																	xmutexconnexions;
		XVECTOR<DIOCHECKCONNEXION*>							connexions;	
		XTHREADCOLLECTED*												threadcheckconnexions;	
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

