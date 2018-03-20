//------------------------------------------------------------------------------------------
//	IFATEXT.H
//	
/**
// \class IFATEXT
//
//	Interface Functions Add (Text Class)
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
*/
//------------------------------------------------------------------------------------------

#ifndef _IFATEXT_H_
#define _IFATEXT_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

#include "IFAObj.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------

class GRPCANVAS;
class GRPSCREEN;
class INPMANAGER;
class IFASKINMANAGER;

class IFATEXT : public IFAOBJ
{
	public:
 												 IFATEXT							(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager,IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_TEXT);
		virtual							~IFATEXT							();

		XSTRING*					 GetText							();
		bool								 SetText							(XCHAR* text);
		bool								 SetText							(XSTRING& text);

		bool								 GetTransparent				();
		bool								 SetTransparent				(bool on = true);	

		virtual	bool				 Draw									(int x, int y);	
		
	protected:	

		XSTRING					 text;		
		bool								 transparent;
						
	private:

		void								 Clean								();		
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
