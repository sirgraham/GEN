//------------------------------------------------------------------------------------------
//	IFABUTTON.H
//	
/**	
// \class 
//   
//  Button class for the interface
//   
//	@author	 Abraham J. Velez
//	@version 07/04/2004 11:02:30
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFABUTTON_H_
#define _IFABUTTON_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XSubject.h"

#include "IFAForm.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum IFABUTTON_XFSMEVENTS
{	
	IFABUTTON_XFSMEVENT_NONE				=	0	,
	IFABUTTON_XFSMEVENT_ENABLED					,
	IFABUTTON_XFSMEVENT_PRESELECT				,
	IFABUTTON_XFSMEVENT_SELECT					,
	IFABUTTON_XFSMEVENT_DISABLED				,

	IFABUTTON_LASTEVENT
};


enum IFABUTTON_XFSMSTATES
{	
	IFABUTTON_XFSMSTATE_NONE				=	0	,
	IFABUTTON_XFSMSTATE_ENABLED					,
	IFABUTTON_XFSMSTATE_PRESELECT				,
	IFABUTTON_XFSMSTATE_SELECT					,
	IFABUTTON_XFSMSTATE_DISABLED				,

	IFABUTTON_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XPUBLISHER;
class XPATH;
class INPBUTTON;
class INPCURSOR;
class GRPBITMAPSEQUENCE;
class IFATEXT;
class IFABITMAP;
class IFASKINMANAGER;

	
class IFABUTTON :  public IFAFORM, public XSUBJECT
{
	public:
																	IFABUTTON							(GRPCANVAS* canvas, GRPSCREEN* screen, INPMANAGER* inpmanager, IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_BUTTON);
		virtual											 ~IFABUTTON							();

		virtual bool									Ini										();
		bool													InitFSMachine					();		
		virtual bool									UpdateInputState			();
		virtual	bool									Draw									(int x, int y);	
		virtual	bool									Update								();			
		virtual bool									End										();

		bool													IsEnabled							();
		bool													SetEnabled						(bool on);

		bool													IsPreSelect						();
	//bool													SetSelect							(bool on);

		GRPBITMAPSEQUENCE*						GetBitmapSequence			();
		bool													LoadBitmapSequence		(XPATH& xpath,XCHAR* namefile);
		
		IFABITMAP*										GetBitmap							();	

		bool													SetBitmapStatus				(IFABUTTON_XFSMSTATES status);
		
		INPBUTTON*										GetINPButton					();
		bool													SetINPButton					(INPBUTTON* button);

		INPCURSOR*										GetINPCursor					();
		bool													SetINPCursor					(INPCURSOR* cursor);
		
	private:

		void													Clean									();
	
		IFATEXT*											text;
		IFABITMAP*										bitmap;	
		GRPBITMAPSEQUENCE*						bitmapsequence;
		
		INPBUTTON*										button;
		INPCURSOR*										cursor;
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

