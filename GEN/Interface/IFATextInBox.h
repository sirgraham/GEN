//------------------------------------------------------------------------------------------
//	IFATEXTINBOX.H
//	
/**	
// \class 
//   
//  Text in Box Obj Class
//   
//	@author	 Abraham J. Velez
//	@version 30/12/2003 12:29:13
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFATEXTINBOX_H_
#define _IFATEXTINBOX_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "IFAText.h" 
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
	
//---- CLASS -------------------------------------------------------------------------------

class GRPCANVAS;
class GRPSCREEN;
class INPMANAGER;
class IFASKINMANAGER;

class IFATEXTINBOX : public IFATEXT
{
	public:												
										 IFATEXTINBOX				(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager, IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_TEXTINBOX);
		virtual							~IFATEXTINBOX				();
												
		virtual	bool				 	 Draw						(int x = 0, int y = 0);	

		bool							 CalculateFullRect			();

private:

		void							 Clean						();	
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

