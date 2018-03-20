//------------------------------------------------------------------------------------------
//	XWINDOWSDEBUG.H
//	
/**	
// \class 
//   
//  WINDOWS Debug class
//   
//	@author	 Abraham J. Velez
//	@version 05/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XWINDOWSDEBUG_H_
#define _XWINDOWSDEBUG_H_

#ifdef XDEBUG	
	
//---- INCLUDES ----------------------------------------------------------------------------			

#include "XBase.h"	
#include "XString.h"
#include "DIOURL.h"

#include "XWINDOWSFactory.h"
#include "XDebug.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XWINDOWSDEBUGCTRL : public XDEBUGCTRL 
{
  public:
												XWINDOWSDEBUGCTRL				();												
		virtual						 ~XWINDOWSDEBUGCTRL				();	
		
		void								PrintSpecial						(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
    void								PrintFile								(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);
		void								PrintNet								(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string);

		bool								GetHandleNet            (XDEBUGCTRLTARGET* target);
		bool								CloseHandleNet		      (XDEBUGCTRLTARGET* target);

  private:

		void								Clean										()
												{
													mutexhandle  = NULL;
												}

    bool								Lock                    ();
    bool								UnLock                  ();
		
    XDWORD  						mutexhandle;
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

#endif
