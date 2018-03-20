//------------------------------------------------------------------------------------------
//	IFAOBJ.H
//	
/**
// \class IFAOBJ
//	
//	Interface Functions Add (Basic OBJ)
// 
//	@author	 Abraham J. Velez
//	@version 12/5/2003  
//	
//	GEN  Copyright (C).  All right reserved.
*/
//------------------------------------------------------------------------------------------

#ifndef _IFAOBJ_H_
#define _IFAOBJ_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"
#include "XFSMachine.h"

#include "GRP_Color.h"
#include "GRPRect.h"
#include "GRPRegion.h"
#include "GRPCanvas.h"

#include "IFAXEvent.h"
#include "IFAFeatures.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum IFAOBJTYPE
{
	IFAOBJTYPE_UNKNOW					= 0	,
	IFAOBJTYPE_SCROLL							,	
	IFAOBJTYPE_FORM								,
	IFAOBJTYPE_TEXT								,
	IFAOBJTYPE_TEXTINBOX					,
	IFAOBJTYPE_BITMAP							,		
	IFAOBJTYPE_BUTTON							,		
	IFAOBJTYPE_PROGRESSBAR				,
	IFAOBJTYPE_MENU								,
	IFAOBJTYPE_MENUPULLDOWN				,	
};



enum IFAOBJLOCATION
{	
	IFAOBJLOCATION_UP					= 0	,	
	IFAOBJLOCATION_RIGHT					,
	IFAOBJLOCATION_LEFT						,
	IFAOBJLOCATION_DOWN						,	
};


#define IFAOBJ_CENTER						 65535
#define IFAOBJ_OVERSPACE				 2

#define IFAOBJ_FRAMESSECOND			 12


//---- CLASS -------------------------------------------------------------------------------

class GRPSCREEN;
class GRPFONT;
class INPMANAGER;
class XFACTORY;
class XTIMER;
class IFASKINMANAGER;
class IFASKINOBJ;
class IFASCROLL;


class IFAOBJ : public GRP_COLORS, public XFSMACHINE 
{
	public:
												IFAOBJ								(GRPCANVAS* canvas, GRPSCREEN* screen,INPMANAGER* inpmanager,IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_UNKNOW);
		virtual						 ~IFAOBJ								();

		GRPCANVAS*					GetCanvas							();

		int									GetSceneHandle				();
		bool								SetSceneHandle				(int scenehdl);

		IFAOBJTYPE					GetType								();

		XSTRING*					GetName								();		
		bool								SetName								(XSTRING& name);
		bool								SetName								(XCHAR* name);

		GRPRECT*						GetRect								();	
		GRPRECT*						GetRectFull						();
		GRPRECT*						GetRectDirect					();
		
		bool								IsScreenRelative			();

		bool								SetOrigin							(int x=0,int y=0,bool isscreenrelative = true);
		bool								SetSize								(int width,int height);

		bool								IsVisible							();
		bool								SetVisible						(bool visible = true);

		bool								IsActive							();
		bool								SetActive							(bool active = true);
		
		int									GetEventHandleCode		();
		bool								SetEventHandleCode		(int code);

		bool								IsActiveBlink					();
		bool								ActiveBlink						(bool on,int framebysec = 0);

		bool								AjustTime							(int framebysec);

		int									GetShadow							();
		bool								SetShadow							(int shadow);

		GRPFONT*						GetFont								();
		bool								SetFont								(XSTRING& namefont);
		bool								SetFont								(GRPFONT* font);

		bool								PushSelectFont				();
		bool								PopSelectFont					();

		XDWORD								GetFeatures						();
		bool								SetFeatures						(XDWORD features);

		GRPRECT*	          GetParentRect					();
		bool                SetParentRect					(GRPRECT* parent);

		int									AjustPosition					(int value, bool xory);
		
		bool								SetCatchRegion				(bool on);
					
		bool								GetRegion							(int x,int y,int overspace);		
		bool								RestoreRegion					();

		virtual bool				Ini										();

		virtual	bool				Draw									(int x, int y);	
		virtual	bool				Update								();	

		virtual bool				CalculateFullRect			();
		virtual bool				CalculeAutomaticSize	();

		virtual bool				End										();
		
		IFASCROLL*					GetScroll							();
		IFASKINMANAGER*			GetSkinManager				();
		IFASKINOBJ*					GetSkinObj						();
					
	protected:
		
		bool								BlinkControl					();

		GRPCANVAS*					canvas;		
		GRPSCREEN*					screen;
		INPMANAGER*					inpmanager;
		GRPREGION						region;

		int									scenehdl;
		
		IFAOBJTYPE					type;	
		XSTRING							name;
		GRPRECTINT					rect;		
		GRPRECTINT					rectfull;
		GRPRECTINT					rectdirect;
		bool								isscreenrelative;
		int									exparam;
		
		int									shadow;

		GRPRECT*						parentrect;

		bool								active;
		int									evenhandlecode;
		bool								visible;
		bool								blink;
		bool								blinkstatus;
		
		GRPFONT*						selectfont;

		XDWORD								features;

		int									overspace;
		bool								catchregion;

		int									framebysec;
		float								timebyframe;
		XTIMER*							timer;

		bool								delskinmanager;
		IFASKINMANAGER*			skinmanager;		

		IFASCROLL*					scroll;
						
  private:

		IFASKINOBJ*					skinobj;

		void								Clean									();		
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
