//------------------------------------------------------------------------------------------
//	XEVENT.H
//	
/**	
// \class 
//   
//  Base Class for events
//   
//	@author	 Abraham J. Velez
//	@version 09/01/2004 13:03:56
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
		
#ifndef _XEVENT_H_
#define _XEVENT_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"

#include "XMemory.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XEVENTTYPE
{
	XEVENTTYPE_NONE											= 0x00000000 ,
	XEVENTTYPE_UI												= 0x00001000 ,
	XEVENTTYPE_GRP											= 0x00002000 ,	
	XEVENTTYPE_DIOSTREAM								= 0x00003000 ,
	XEVENTTYPE_DIOIEC60870_5						= 0x00004000 , 
	XEVENTTYPE_DIOMODBUSELECTRICMETER		= 0x00005000 ,
	XEVENTTYPE_PING											= 0x00006000 ,
	XEVENTTYPE_WEBCLIENT								= 0x00007000 ,
	XEVENTTYPE_WEBSERVER								= 0x00008000 ,
	XEVENTTYPE_WEBPAGEHTMLCREATOR				= 0x00009000 ,
	XEVENTTYPE_INTERNET									=	0x0000A000 ,	
	XEVENTTYPE_PROTOCOL									= 0x0000B000 ,
	XEVENTTYPE_PROTOCOLCONNEXIONS				= 0x0000C000 ,	
	XEVENTTYPE_SCRIPT										=	0x0000D000 ,
	XEVENTTYPE_SOUND										= 0x0000E000 ,
	XEVENTTYPE_SCHEDULER								= 0x0000F000 ,
	XEVENTTYPE_LICENSE									= 0x00010000 ,
	XEVENTTYPE_APPLICATIONUPDATE				= 0x00011000 ,
	XEVENTTYPE_APPLICATION							= 0x00012000 ,
	XEVENTTYPE_LOADER										= 0x00013000 ,
	XEVENTTYPE_OWNAPPLICATION						= 0x80000000 ,	
};



//---- CLASS -------------------------------------------------------------------------------

class  XFACTORY;
class  XEVENT;
class  XSUBJECT;


class XEVENT
{
	public:

											XEVENT						(XSUBJECT* subject, XDWORD type, XDWORD family)
											{
												Clean();

												this->subject = subject;
												this->family  = family;
												this->type		= type;
											}

											XEVENT						(XSUBJECT* subject, XDWORD type)
											{
												Clean();

												this->subject = subject;
												this->type		= type;
											}

		virtual					 ~XEVENT						()																			{ Clean();															} 

		XSUBJECT*					GetSubject				()																			{ return subject;												}
		
		XDWORD						GetEventFamily		()																			{ return family;												}	
		void							SetEventFamily		(XDWORD family)													{ this->family = family;								}

		XDWORD						GetEventType			()																			{ return type;													}
		void							SetEventType			(XDWORD type)														{ this->type = type;										}
		
	protected:

		void							Clean							()
											{
												family      = XEVENTTYPE_NONE;
												type				= XEVENTTYPE_NONE;
												subject			= NULL;											
											}

		XDWORD						family;
		XDWORD						type;
		XSUBJECT*					subject;		
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif





