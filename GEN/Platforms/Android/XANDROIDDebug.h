//------------------------------------------------------------------------------------------
//	XANDROIDDEBUG.H
//	
/**	
// \class 
//   
//  LINUX debug class
//   
//	@author	 Abraham J. Velez
//	@version 05/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XANDROIDDEBUG_H_
#define _XANDROIDDEBUG_H_

#ifdef XDEBUG	
	
//---- INCLUDES ----------------------------------------------------------------------------			

#include <pthread.h>

#include "XBase.h"	
#include "XString.h"
#include "DIOURL.h"

#include "XANDROIDDateTime.h"
#include "XANDROIDFactory.h"

#include "XDebug.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XANDROIDDEBUGCTRL : public XDEBUGCTRL 
{
  public:
									        XANDROIDDEBUGCTRL			();												
		virtual			         ~XANDROIDDEBUGCTRL			();												
		
		void					        PrintSpecial					(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
    void					        PrintFile							(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
		void					        PrintNet							(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);

		bool									GetHandleNet          (XDEBUGCTRLTARGET* target);
		bool									CloseHandleNet		    (XDEBUGCTRLTARGET* target);

	private:

		void					        Clean									()
													{
													  mutexhandle	 = 0;  
													}


    bool                  Lock                  ();
    bool                  UnLock                ();
		
		XANDROIDDATETIME			xdatetime;  

    XDWORD  				      mutexhandle;
		pthread_mutexattr_t		attr;
		pthread_mutex_t				mutex;
		int										nsendctrlnetresources;
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif
