//------------------------------------------------------------------------------------------
//	IFAMENU.H
//	
/**	
// \class 
//   
//  Interface Functions Add (Menu Class)
//   
//	@author	 Abraham J. Velez
//	@version 12/03/2004 16:29:18
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFAMENU_H_
#define _IFAMENU_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XMap.h"
#include "XSubject.h"	

#include "IFAForm.h"	
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum IFAMENU_DISTRIBUTEOPTIONS
{
	IFAMENU_DISTRIBUTEOPTIONS_NONE			= 0 ,
	IFAMENU_DISTRIBUTEOPTIONS_HORIZONTAL		,
	IFAMENU_DISTRIBUTEOPTIONS_VERTICAL			,
};

enum IFAMENU_BUTTONS
{
	IFAMENU_BUTTON_UP		  = IFAOBJLOCATION_UP				,
	IFAMENU_BUTTON_DOWN   = IFAOBJLOCATION_DOWN			,	
	IFAMENU_BUTTON_RIGHT  = IFAOBJLOCATION_LEFT			,
	IFAMENU_BUTTON_LEFT	  = IFAOBJLOCATION_RIGHT		, 
	IFAMENU_BUTTON_B			= IFAOBJLOCATION_DOWN + 1	,
	IFAMENU_BUTTON_C			= IFAOBJLOCATION_DOWN + 2	,                      
};

#define IFAMENU_MAXKEYS						6
#define IFAMENU_CANCEL					 -1

#define IFAMENU_OPTIONOVERSIZE		4
	
//---- CLASS -------------------------------------------------------------------------------

class INPBUTTON;
class GRPSCENE;
class IFASKINMANAGER;

class IFAMENU : public IFAFORM, public XSUBJECT
{
	public:
																IFAMENU											(GRPCANVAS* canvas, GRPSCENE* scene,GRPSCREEN* screen,INPMANAGER* inpmanager, IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_MENU);
		virtual										 ~IFAMENU											();
						
		virtual bool								Ini													();
		virtual bool								SetInputControl							();	
		virtual bool								UpdateInputState						();
		virtual bool								UpdateObj										(int nobj,IFAOBJ* obj);
		virtual	bool								Update											();			
		virtual bool								End													();
	
		IFAOBJ*											GetOption										(int option);
		int													GetNumberOptions						();
		int													GetSelectOption							();
		bool												SelectOption								(int option,bool goahead = true);		
		IFAOBJ*											GetSelectObj								();

		bool												AjustScrollSelectOption			();

		bool												DistributeOptions						(IFAMENU_DISTRIBUTEOPTIONS mode,int overspace = 2);

		IFAMENU*										GetParentPullDownMenu       ();
		bool												SetParentPullDownMenu       (IFAMENU* menu);

		bool												AddOptionPullDownMenu				(IFAOBJ* option,IFAMENU* menu,IFAMENU* parent = NULL);
		IFAMENU*										GetOptionPullDownMenu				(IFAOBJ* option);
		bool												DelOptionPullDownMenu				(IFAOBJ* option);

		bool												ShowPullDownMenu						(IFAOBJ* sobj,IFAMENU* pulldownmenu);

		bool												CheckLimitsOptions					(int option);		
			

	protected:

		GRPSCENE*										scene;
	
	private:
		
		void												Clean												();		

		IFAMENU_DISTRIBUTEOPTIONS		distributeoptions;
		int													optionoverspace;
		int													selectoption;		
		
		XMAP<IFAOBJ*, IFAMENU*>			optionpulldown;
		IFAMENU*										parentpulldownmenu;
		
		INPBUTTON*									button[IFAMENU_MAXKEYS];

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

