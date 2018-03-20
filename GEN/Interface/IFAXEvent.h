//------------------------------------------------------------------------------------------
//	IFAXEVENT.H
//	
/**	
// \class 
//   
//  XEvents for IFA
//   
//	@author	 Abraham J. Velez
//	@version 18/03/2004 9:42:46
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFAXEVENT_H_
#define _IFAXEVENT_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XEvent.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum IFAXEVENTTYPE
{
	IFAXEVENTTYPE_UNKNOW				= XEVENTTYPE_AWORLD  ,	
	IFAXEVENTTYPE_SELECT	
};
	
//---- CLASS -------------------------------------------------------------------------------
	
class IFAOBJ;


class IFAXEVENTSELECT : public XEVENT
{
	public:
										IFAXEVENTSELECT				(int selectoption,IFAOBJ* selectobj,IFAOBJ* obj,XSUBJECT* subject,int param = 0,XDWORD type = IFAXEVENTTYPE_SELECT);				
									 ~IFAXEVENTSELECT				();

		int							GetSelectOption				();
		IFAOBJ*					GetSelectObj					();
		IFAOBJ*					GetObj								();
		int							GetParam							();

	private:

		void						Clean									();	

		int							selectoption;	
		IFAOBJ*					selectobj;
		IFAOBJ*					obj;
		int							param;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif
