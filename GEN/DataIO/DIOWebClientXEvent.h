//------------------------------------------------------------------------------------------
//	DIOWEBCLIENTXEVENT.H
//	
/**	
// \class 
//   
//  DIO Web Client Event class
//   
//	@author	 Abraham J. Velez
//	@version 20/01/2007 21:05:48
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOWEBCLIENTXEVENT_H_
#define _DIOWEBCLIENTXEVENT_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XEvent.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOWEBCLIENTXEVENTTYPE
{
	DIOWEBCLIENTXEVENTTYPE_UNKNOWN		   		= XEVENTTYPE_WEBCLIENT	,	
	DIOWEBCLIENTXEVENTTYPE_OPENWEB						  									,
	DIOWEBCLIENTXEVENTTYPE_WRITEHEADER					 									,
	DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA				  									,
	DIOWEBCLIENTXEVENTTYPE_HEADERERROR				  									,
	DIOWEBCLIENTXEVENTTYPE_READHEADER					  									,
	DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK			  									,
	DIOWEBCLIENTXEVENTTYPE_CLOSEWEB						  									
};


//---- CLASS -------------------------------------------------------------------------------


class DIOWEBCLIENTXEVENT : public XEVENT
{
	public:
																	DIOWEBCLIENTXEVENT								(XSUBJECT* subject, XDWORD type = DIOWEBCLIENTXEVENTTYPE_UNKNOWN, XDWORD family =  XEVENTTYPE_WEBCLIENT) : XEVENT(subject, type, family)   
																	{ 
																		Clean();							
																	}

		virtual			 								 ~DIOWEBCLIENTXEVENT								()																																											 { Clean();							}				
	
		void													SetDownloadSize										(XDWORD downloadsize)
																	{
																		this->downloadsize = downloadsize;
																	}

		XDWORD												GetDownloadSize										()
																	{
																		return this->downloadsize;
																	}

	private:

		XDWORD												downloadsize;

		void													Clean															()
																	{
																		downloadsize = 0;
																	}				
};	

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

