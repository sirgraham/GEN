/*------------------------------------------------------------------------------------------
//	IFAMANAGER.H
*/	
/**	
// \class 
//   
//  IFA Manager class
//   
//	@author	 Abraham J. Velez
//	@version 18/01/2012 19:55:52
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _IFAMANAGER_H_
#define _IFAMANAGER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <stdio.h>

#include "XFileXML.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define IFAMANAGER_INTERFACE	 __L("INTERFACE")

#define IFAMANAGER_TYPEFORM		 __L("FORM")
#define IFAMANAGER_TYPEBITMAP  __L("BITMAP")
#define IFAMANAGER_TYPEBUTTON  __L("BUTTON")
#define IFAMANAGER_TYPETEXT		 __L("TEXT")
#define IFAMANAGER_TYPEMENU    __L("MENU")


/*---- CLASS -----------------------------------------------------------------------------*/

class GRPCANVAS;
class GRPSCENE;
class GRPSCREEN;
class INPMANAGER;
class IFAOBJ;
class IFASKINMANAGER; 
class XFACTORY;
class XPATH;
class XPUBLISHER;
class XFILEXMLELEMENT;


class IFAMANAGER
{
	public:
 													IFAMANAGER							(GRPCANVAS* canvas, GRPSCENE* scene, GRPSCREEN* screen, INPMANAGER* inpmanager, IFASKINMANAGER* skinmanager = NULL);
		virtual							 ~IFAMANAGER							();

		bool									LoadElements						(XPATH& xpath,XCHAR* namefile);

		bool									DeleteAllElements				(bool deletecontent);

	private:

		void									Clean										();	
		
		bool									AddElement							(XFILEXML& xmlfile,XFILEXMLELEMENT* element,int& index,IFAFORM* form);
		IFAOBJ*								AddElement							(XFILEXMLELEMENT* element);

		GRPCANVAS*						canvas;
		GRPSCENE*							scene;
		GRPSCREEN*						screen;
		INPMANAGER*						inpmanager;		
		IFASKINMANAGER*				skinmanager;

		XVECTOR<IFAOBJ*>			elements;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

