/*------------------------------------------------------------------------------------------
//	TESTBASEUI.H
*/	
/**	
// \class 
//   
//  Test Base UI (User Interface)
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEUI_H_
#define _TESTBASEUI_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class UIMANAGER;
class UIWIDGETCREATORHELPER;
class UIWIDGETIMAGE;
class UIWIDGETCONTAINER;
class UIWIDGETBUTTON;

class TESTBASEUI : public TESTBASE, public XOBSERVER							
{																								
	public:																				
																								TESTBASEUI								(GRPAPPLICATION* xapplication);
																							 ~TESTBASEUI								();

		virtual bool																IniResources							();
		virtual	bool																EndResources							();
		virtual bool																Do												();
		void 																				HandleEvent								(XEVENT* event);

	private:

		void																				Clean											()	
																								{
																									uimanager			=	NULL;
																									widgetcreator = NULL;

																									erase					= false;

																									rotatetest		= NULL;
																								}
		
		GRPSCENE*																		uiscene;

		XPATH																				xpath;
		UIMANAGER*																	uimanager;		
		UIWIDGETCREATORHELPER*											widgetcreator;

		bool																				erase;

		XVECTOR<UIWIDGETCONTAINER*>									fiches;
		UIWIDGETBUTTON*															borrar;
		UIWIDGETIMAGE*															rotatetest;
		float																				rotation;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

