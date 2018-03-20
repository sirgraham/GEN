//------------------------------------------------------------------------------------------
//	IFACOMICBALLOON.H
//	
/**	
// \class 
//   
//  Talk Comic Sentence Class
//   
//	@author	 Abraham J. Velez
//	@version 26/02/2004 15:23:30
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFACOMICBALLOON_H_
#define _IFACOMICBALLOON_H_
		
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "IFAForm.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
#define IFACOMICBALLOON_STEP 4

//---- CLASS -------------------------------------------------------------------------------

class	IFATEXTINBOX;

class IFACOMICBALLOON : public IFAFORM
{
	public:
														IFACOMICBALLOON			(GRPCANVAS* canvas,GRPSCENE* scene,GRPSCREEN* screen,INPMANAGER* inpmanager,GRPRECT* sourcerect = NULL);
		virtual								 ~IFACOMICBALLOON			();
		
		bool										SetSourceRect				(GRPRECT* sourcerect);

		bool										SetColor						(GRPCOLORTYPE type,GRPPROPERTYMODE mode,XBYTE* RGB,XBYTE alpha =100);
		bool										SetColor						(GRPCOLORTYPE type,GRPCOLOR* color);
		
		bool										ShowSentence				(XCHAR* text,int xsenseajust = 0, int ysenseajust=0 , bool talk = true);			

		IFATEXTINBOX*						GetTextInBox				();	
	
	private:

		void										Clean								();
		GRPRECT*								GetSourceXY					(int hdlsource);

		GRPSCENE*								scene;				
		int											hdlscene;
		int											hdlsource;
		int											scrollsignal;
		IFATEXTINBOX*						textib;	

		GRPRECT									sourcerect;		
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

