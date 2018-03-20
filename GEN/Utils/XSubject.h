//------------------------------------------------------------------------------------------
//	XSUBJECT.H
//	
/**	
// \class 
//   
//  Basic eXtender Subject class
//   
//	@author	 Abraham J. Velez
//	@version 09/01/2004 15:36:14
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XSUBJECT_H_
#define _XSUBJECT_H_
	

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XPublisher.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XEVENT;

class XSUBJECT
{
	public:	
											XSUBJECT										()												{ Clean();																															}
		virtual					 ~XSUBJECT										()												{ Clean();																														  }
		
		bool							RegisterEvent								(XDWORD type)							{ return XPUBLISHER::GetInstance().RegisterEvent(type, this);           }
		bool							DeRegisterEvent							(XDWORD type)             { return XPUBLISHER::GetInstance().DeRegisterEvent(type, this);         }			
		bool							DeRegisterAllEvents					()												{ return XPUBLISHER::GetInstance().DeRegisterAllEvents();								}
		
		bool							PostEvent										(XEVENT* event)						
											{ 
												return XPUBLISHER::GetInstance().PostEvent(event, this);										
											}

	private:

		void							Clean												()
											{

											}

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
