/*------------------------------------------------------------------------------------------
//	SNDEVENT.H
*/	
/**	
// \file 
//   
//  sound system events
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 09/11/2015 17:15:13
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDEVENT_H_
#define _SNDEVENT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XEvent.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum SNDEVENT_TYPE
{
	SNDEVENT_TYPE_STOP			= 1 ,
	SNDEVENT_TYPE_PLAY			= 2 ,
	SNDEVENT_TYPE_UNKNOWN
};
	
/*---- CLASS -----------------------------------------------------------------------------*/

class SNDELEMENT;
class SNDSOURCE;
class SNDINSTANCE;

class SNDEVENT : public XEVENT
{
	public:

															SNDEVENT							(XSUBJECT* subject, XDWORD type, XDWORD family);
		virtual									 ~SNDEVENT							();

		void											SetSNDType						(SNDEVENT_TYPE type)																							{ sndtype = type;										}
		SNDEVENT_TYPE							GetSNDType						()																																{ return sndtype;										}

		void											SetSound							(XCHAR* sound)																										{ soundname.Set(sound);							}
		XSTRING*									GetSound							()																																{ return &soundname;								}

		void											SetSNDElement					(SNDELEMENT* sndelement)																					{ this->sndelement = sndelement;		}
		SNDELEMENT*								GetSNDElement					()																																{ return this->sndelement;					}

		void											SetSNDSource					(SNDSOURCE* sndsource)																						{ this->sndsource = sndsource;			}
		SNDSOURCE*								GetSNDSource					()																																{ return this->sndsource;						}

		void											SetSNDInstance				(SNDINSTANCE* sndinstance)																				{ this->sndinstance = sndinstance;	}
		SNDINSTANCE*							GetSNDInstance				()																																{ return this->sndinstance;					}

	protected:

		SNDEVENT_TYPE							sndtype;
		XSTRING										soundname;
		SNDELEMENT*								sndelement;
		SNDSOURCE*								sndsource;
		SNDINSTANCE*							sndinstance;

	private:
		
		void											Clean									()
															{
																sndtype				= SNDEVENT_TYPE_UNKNOWN;
																sndelement		= NULL;
																sndsource			= NULL;
																sndinstance		= NULL;
															}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

