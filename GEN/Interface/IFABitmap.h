//------------------------------------------------------------------------------------------
//	IFABITMAP.H
//	
/**	
// \class 
//   
//  Interface Bitmap Class
//   
//	@author	 Abraham J. Velez
//	@version 31/12/2003 10:28:47
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFABITMAP_H_
#define _IFABITMAP_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "IFAObj.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
	
//---- CLASS -------------------------------------------------------------------------------

class IFASKINMANAGER;

class IFABITMAP : public IFAOBJ
{
	public:
 												 IFABITMAP							(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager,IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_BITMAP);
		virtual							~IFABITMAP							();

		int									 GetAlpha								();	
		bool								 SetAlpha								(int alpha);

		GRPBITMAP*					 GetBitmap							();
		bool								 SetBitmap							(GRPBITMAP* bitmap);

		virtual	bool				 Draw										(int x, int y);				
		
	protected:	

		GRPBITMAP*					 bitmap;		
		int									 alpha;
	
	private:

		void								 Clean									();				
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif



