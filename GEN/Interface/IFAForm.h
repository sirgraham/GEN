//------------------------------------------------------------------------------------------
//	IFAFORM.H
//	
/**
// \class IFAFORM
//
//	Interface Functions Add (Form Class)
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
*/
//------------------------------------------------------------------------------------------

#ifndef _IFAFORM_H_
#define _IFAFORM_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"
#include "XString.h"

#include "IFAObj.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum IFAFORM_EVENCONTROL
{
	IFAFORM_EVENCONTROL_NONE		= 0 ,
	IFAFORM_EVENCONTROL_SELECT		
};

//---- CLASS -------------------------------------------------------------------------------

class GRPCANVAS;
class GRPSCREEN;
class INPMANAGER;
class IFASKINMANAGER;

class IFAFORM : public IFAOBJ
{
	public:
 												IFAFORM								(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager,IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_FORM);
		virtual						 ~IFAFORM								();

		bool								SetCaption						(XCHAR* caption);
		bool								SetCaption						(XSTRING& caption);
		bool								SetEdgeIn							(int width,int height);

		XBYTE								GetSense							();	
		bool								SetSense							(XBYTE sense);	

		bool								RestoreAllRegions			();		
		
		bool								AddObj								(IFAOBJ* obj);
		bool								DeleteObj							(IFAOBJ* obj);
		bool								DeleteAllObj					();		
		bool								SetCatchRegionsObj		(bool on);
		
		virtual	bool				UpdateObj							(int nobj,IFAOBJ* obj);
		virtual bool				UpdateObjs						(GRPRECT* objrect);

		virtual	bool				Draw									(int x, int y);
		virtual bool				Update								();		
		virtual bool				CalculateFullRect			();

	protected:

		XSTRING							caption;
		int									widthedgein;
		int									heightedgein;

		XVECTOR<IFAOBJ*>		objs;	
		XBYTE								sense;
				
	private:

		void								Clean									();
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
