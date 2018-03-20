//------------------------------------------------------------------------------------------
//	IFAPROGRESSBAR.H
//	
/**	
// \class 
//   
//  Interface Progress Bar Class
//   
//	@author	 Abraham J. Velez
//	@version 06/04/2004 10:33:47
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFAPROGRESSBAR_H_
#define _IFAPROGRESSBAR_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "IFAForm.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	

//---- CLASS -------------------------------------------------------------------------------

class GRPSCENE;
class IFATEXT;
class IFASKINMANAGER;

class IFAPROGRESSBAR :  public IFAFORM
{
	public:
																	IFAPROGRESSBAR				(GRPCANVAS* canvas,GRPSCENE* scene,GRPSCREEN* screen,INPMANAGER* inpmanager,IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_PROGRESSBAR);
		virtual											 ~IFAPROGRESSBAR				();

		virtual bool									Ini										();

		virtual	bool									Draw									(int x, int y);	
		virtual	bool									Update								();	
		
		virtual bool									End										();

		int														CalculatePercent			();
		
		IFATEXT*											GetTextPercent				();
		GRPCOLORS*										GetBarColor						();
			
	private:

		void													Clean									();
		bool													DrawProgress					(int x, int y);
				
		
		GRPSCENE*											scene;
		GRPCOLORS											barcolors;
		int														handletextpercent;
		IFATEXT*											textpercent;
		
		int														percent;
		int														minvalue;
		int														maxvalue;
		int														index;
		int														thickness;
		int														separation;

	
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

