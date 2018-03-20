//------------------------------------------------------------------------------------------
//	IFAOBJ.CPP
//	
//	Interface Functions Add (Basic OBJ)
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/5/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

//---- INCLUDES ----------------------------------------------------------------------------

#include "XTime.h"
#include "XFactory.h"

#include "GRPObj.h"
#include "GRPFont.h"
#include "GRPCanvas.h"

#include "INPButton.h"

#include "IFASkinManager.h"
#include "IFAScroll.h"

#include "IFAObj.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFAOBJ::IFAOBJ
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 12:32:43
//	
//	@return				
//	@param				canvas : 
//  @param				screen : 
//  @param				inpmanager : 
//  @param				skinmanager :
//  @param				type : 
*/
//-------------------------------------------------------------------
IFAOBJ::IFAOBJ(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager,XFACTORY* xfactory,IFASKINMANAGER* skinmanager,IFAOBJTYPE type) : XFSMACHINE(0)
{		
	Clean();

	this->canvas			= canvas;
	this->screen			= screen;
	this->inpmanager	= inpmanager;
	
	this->skinmanager = skinmanager;
	this->type				= type;

	if(type!=IFAOBJTYPE_SCROLL)  
		{
			scroll = new IFASCROLL(canvas,screen,inpmanager,xfactory,skinmanager);			
			if(scroll) scroll->SetRects(&rect,&rectfull);
		}

	if(!this->skinmanager)  
		{ 
			delskinmanager	  = true;
			this->skinmanager = new IFASKINMANAGER(IFASKINTYPE_DEFAULT);
		}

	if(this->skinmanager) skinobj = this->skinmanager->GetSkinObj(this);
		
	if()
		{
			if(!timer)
				{
					timer = xfactory->CreateTimer();
					AjustTime(IFAOBJ_FRAMESSECOND);		
				}
		}
}



//-------------------------------------------------------------------
// IFAOBJ::~IFAOBJ
/**
//	
//	@author        Abraham J. Velez
//	@version       12/5/2003   14:56:56
//	
//	@return         : 
*/
//-------------------------------------------------------------------
IFAOBJ::~IFAOBJ()
{			
	if(skinmanager && delskinmanager)  
		{ 
			delskinmanager = false;
			delete skinmanager;
		}

	delete skinobj;

	if(type!=IFAOBJTYPE_SCROLL) 
		{
			delete scroll;
			scroll = NULL;
		}

	if()
		{
			if(timer)	xfactory->DeleteTimer(timer);
		}

  Clean();

	SetOrigin();	
	
}


/*-------------------------------------------------------------------
//  IFAOBJ::GetCanvas
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 19:41:26
//	
//	@return				GRPCANVAS* : 
//	*/
/*-----------------------------------------------------------------*/
GRPCANVAS* IFAOBJ::GetCanvas()
{
	return canvas;
} 


/*-------------------------------------------------------------------
//  IFAOBJ::GetSceneHandle
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 23:17:14
//	
//	@return				int : 
//	*/
/*-----------------------------------------------------------------*/
int IFAOBJ::GetSceneHandle()
{
	return scenehdl;
}



/*-------------------------------------------------------------------
//  IFAOBJ::SetSceneHandle
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 23:17:18
//	
//	@return				bool : 
//	@param				scenehdl : 
*/
/*-----------------------------------------------------------------*/
bool IFAOBJ::SetSceneHandle(int scenehdl)
{
	this->scenehdl = scenehdl;

	return true;
}




//-------------------------------------------------------------------
//  IFAOBJ::GetType
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/11/2004 10:36:46
//	
//	@return				IFAOBJTYPE : 
//	*/
//-------------------------------------------------------------------
IFAOBJTYPE IFAOBJ::GetType()
{
	return (IFAOBJTYPE)type;
}



/*-------------------------------------------------------------------
//  IFAOBJ::GetName
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/01/2012 12:46:22
//	
//	@return				XSTRING* : 
//	*/
/*-----------------------------------------------------------------*/
XSTRING* IFAOBJ::GetName()
{
	return &name;
}



/*-------------------------------------------------------------------
//  IFAOBJ::SetName
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/01/2012 12:47:22
//	
//	@return				bool : 
//	@param				name : 
*/
/*-----------------------------------------------------------------*/
bool IFAOBJ::SetName(XSTRING& name)
{
	this->name = name;

	return true;
}



/*-------------------------------------------------------------------
//  IFAOBJ::SetName
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/01/2012 12:48:03
//	
//	@return				bool : 
//	@param				name : 
*/
/*-----------------------------------------------------------------*/
bool IFAOBJ::SetName(XCHAR* name)
{
	this->name = name;

	return true;
}




/*-------------------------------------------------------------------
//  IFAOBJ::GetRect
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 19:45:10
//	
//	@return				GRPRECT* : 
//	*/
/*-----------------------------------------------------------------*/
GRPRECT* IFAOBJ::GetRect()
{
	return &rect;
}




/*-------------------------------------------------------------------
//  IFAOBJ::GetRectFull
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 19:45:10
//	
//	@return				GRPRECT* : 
//	*/
/*-----------------------------------------------------------------*/
GRPRECT* IFAOBJ::GetRectFull()
{
	return &rectfull;
}




/*-------------------------------------------------------------------
//  IFAOBJ::GetRectDirect
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 23:09:25
//	
//	@return				GRPRECT* : 
//	*/
/*-----------------------------------------------------------------*/
GRPRECT* IFAOBJ::GetRectDirect()
{
	return &rectdirect;
}




/*-------------------------------------------------------------------
//  IFAOBJ::IsScreenRelative
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 20:12:47
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool IFAOBJ::IsScreenRelative()
{
	return isscreenrelative;
}



//-------------------------------------------------------------------
//  IFAOBJ::SetOrigin
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/04/2004 14:47:53
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
//  @param				scrrelative : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetOrigin(int x,int y,bool isscreenrelative)
{	
	rect.SetOriginX(x);
	rect.SetOriginY(y);

	this->isscreenrelative = isscreenrelative;
	
	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetSize
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/12/2003 18:01:59
//	
//	@return				bool : 
//	@param				width : 
//  @param				height : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetSize(int width,int height)
{	
	rect.SetWidth(width);
	rect.SetHeight(height);

	return true;	
}



//-------------------------------------------------------------------
//  IFAOBJ::IsVisible
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 17:31:17
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::IsVisible()
{
  return (bool)visible;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetVisible
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 17:32:08
//	
//	@return				bool : 
//	@param				visible : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetVisible(bool visible)
{	
	this->visible = visible;

	return true;	
}


//-------------------------------------------------------------------
//  IFAOBJ::IsActive
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 17:31:17
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::IsActive()
{
  return (bool)active;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetActive
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 17:32:08
//	
//	@return				bool : 
//	@param				active : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetActive(bool active)
{
	this->active = active;

	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::GetEventHandleCode
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2004 9:28:21
//	
//	@return				int	 : 
//	*/
//-------------------------------------------------------------------
int IFAOBJ::GetEventHandleCode()
{
	return (int)evenhandlecode;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetEventHandleCode
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2004 9:28:44
//	
//	@return				bool : 
//	@param				code : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetEventHandleCode(int code)
{
	evenhandlecode = code;

	return true;
}



//-------------------------------------------------------------------
//  IFAOBJ::IsActiveBlink
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 9:46:07
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::IsActiveBlink()
{
	return (bool)blink;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetBlink
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 9:49:34
//	
//	@return				bool : 
//	@param				on : 
//  @param				framebysec : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::ActiveBlink(bool on,int framebysec)
{
	blink = on;
	if(framebysec) AjustTime(framebysec);

	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::::AjustTime
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/02/2004 11:57:27
//	
//	@return				bool : 
//	@param				int	framebysec : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::AjustTime(int framebysec)
{
	
	if(!timer) return false;
	
	this->framebysec  = framebysec; 
	timebyframe				= ((float)1000/(float)framebysec);
	
	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::GetShadow
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/03/2004 10:57:32
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int IFAOBJ::GetShadow()
{
	return (int)shadow;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetShadow
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/03/2004 10:57:52
//	
//	@return				bool : 
//	@param				shadow : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetShadow(int shadow)
{
	this->shadow = shadow;

	return true;
}



//-------------------------------------------------------------------
//  IFAOBJ::GetFont
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/08/2004 14:16:14
//	
//	@return				GRPFONT* : 
//	*/
//-------------------------------------------------------------------
GRPFONT* IFAOBJ::GetFont()
{
	return (GRPFONT*)selectfont;
}



//-------------------------------------------------------------------
//  IFAOBJ::SetFont
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/08/2004 14:17:34
//	
//	@return				bool : 
//	@param				namefont : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetFont(XSTRING& namefont)
{
	if(!canvas)   return false;
	
	GRPFONTMANAGER* fontmanger = canvas->GetFontManager();
	if(!fontmanger) return false;

	GRPFONT* font = fontmanger->GetSetFont(namefont);
	if(!font) return false;

	return SetFont(font);
}
		


//-------------------------------------------------------------------
//  IFAOBJ::SetFont
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/08/2004 14:17:39
//	
//	@return				bool : 
//	@param				font : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetFont(GRPFONT* font)
{	
	if(!canvas) return false;
	if(!font)		return false;
	
	selectfont = font;
	
	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::PushSelectFont
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/08/2004 14:46:43
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::PushSelectFont()
{	
	if(!selectfont) return false;

	GRPFONTMANAGER* fontmanger = canvas->GetFontManager();
	if(!fontmanger) return false;

	return (bool)fontmanger->PushSelectFont(selectfont);	
}



//-------------------------------------------------------------------
//  IFAOBJ::PopSelectFont
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/08/2004 14:23:55
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::PopSelectFont()
{	
	if(!canvas)			return false;
	if(!selectfont)	return false;
	
	GRPFONTMANAGER* fontmanger = canvas->GetFontManager();
	if(!fontmanger) return false;

	return (bool)fontmanger->PopSelectFont();
}



//------------------------------------------------------------------
//  IFAOBJ::GetFeatures
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			17/03/2004 10:19:19
//	
//	@return				XDWORD : 
//	*/
//-------------------------------------------------------------------
XDWORD IFAOBJ::GetFeatures()
{
	return (XDWORD)features;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetFeatures
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			17/03/2004 10:20:02
//	
//	@return				bool : 
//	@param				features : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetFeatures(XDWORD features)
{
	this->features = features;

	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::GetParentRect
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/04/2004 10:20:25
//	
//	@return				GRPRECT* : 
//	*/
//-------------------------------------------------------------------
GRPRECT* IFAOBJ::GetParentRect()
{
	return (GRPRECT*)parentrect;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetParentRect
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/04/2004 10:20:43
//	
//	@return				bool : 
//	@param				parentrect : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetParentRect(GRPRECT* parentrect)
{
	this->parentrect = parentrect;

	return true;
}



//-------------------------------------------------------------------
//  IFAOBJ::AjustPosition
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/04/2004 10:23:39
//	
//	@return				int : 
//	@param				value : 
//  @param				xory : 
*/
//-------------------------------------------------------------------
int IFAOBJ::AjustPosition(int value, bool xory)
{	
	if(value!=IFAOBJ_CENTER) return value;

	int vt = 0;
			
	if(parentrect)
		{
			int msize = (xory)?parentrect->GetWidth():parentrect->GetHeight();
			int rsize = (xory)?rect.GetWidth():rect.GetHeight();

			if((msize - rsize)>0) vt = ((msize - rsize)/2);						
		}

	return vt;
}


//-------------------------------------------------------------------
//  IFAOBJ::SetCatchRegion
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2004 17:41:45
//	
//	@return				bool : 
//	@param				on : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::SetCatchRegion(bool on)
{
	catchregion = on;

	return true;
}



//-------------------------------------------------------------------
//  IFAOBJ::GetRegion
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2003 17:09:32
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::GetRegion(int x,int y,int overspace)
{	
	bool status;

	if(!canvas)	return false;	

	GRPRECT nr;

	nr.SetOriginX(x - overspace);
	nr.SetOriginY(y - overspace);
	nr.SetWidth(rect.GetWidth()  + (overspace*2));
	nr.SetHeight(rect.GetHeight() + (overspace*2)); 

	status = region.Get(canvas,&nr);

	return status;
}



//-------------------------------------------------------------------
//  IFAOBJ::RestoreRegion
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2003 17:10:06
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::RestoreRegion()
{
	return (bool)region.Restore(canvas);
}


//-------------------------------------------------------------------
//  IFAOBJ::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/04/2004 17:20:49
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::Ini()
{

	return false;
}



//-------------------------------------------------------------------
//  IFAOBJ::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 16:47:33
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::Draw(int x, int y)
{
	rectdirect.Set(x,y,0,0);

	return false;
}


//-------------------------------------------------------------------
//  IFAOBJ::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/12/2003 9:26:11
//	
//	@return				bool : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::Update()
{	
	if(!canvas) return false;

	int xm = AjustPosition(rect.GetX(), true);
	int ym = AjustPosition(rect.GetY(), false);

	if(isscreenrelative)
		{
			xm += canvas->GetScreenZone()->GetX();
			ym += canvas->GetScreenZone()->GetY();
		}

	if(catchregion) GetRegion(xm,ym,overspace);

	bool firsttime = false;	
	if((!rectfull.GetWidth())&&(!rectfull.GetHeight()))  firsttime = true;

	CalculateFullRect();	

	if(firsttime) scroll->SetOrigin();
	scroll->SetAvailable();

	scroll->MakeStep();

	scroll->IncrementStep(xm,ym);

	if(visible) Draw(xm,ym);	

	if(scroll->IsAvailable()) scroll->Draw(xm,ym);		
					
  return false;
}


//-------------------------------------------------------------------
//  IFAOBJ::CalculateFullRect
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2004 11:54:14
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::CalculateFullRect()
{	
	rectfull.Set(&rect);

	scroll->SetAvailable();
		
	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::CalculeAutomaticSize
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 13:13:16
//	
//	@return				bool : 
*/
//-------------------------------------------------------------------
bool IFAOBJ::CalculeAutomaticSize()
{		
	rect.Set(rect.GetX(),rect.GetY());
	rectfull.Set(rectfull.GetX(),rectfull.GetY());
	rectdirect.Set(rectdirect.GetX(),rectdirect.GetY());

	for(int c=0;c<8;c++)
		{
			CalculateFullRect();
			rect.Set(&rectfull);
		}
	
	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::End
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/04/2004 17:21:40
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::End()
{
	return true;
}


//-------------------------------------------------------------------
//  IFAOBJ::GetScroll
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 11:09:45
//	
//	@return				IFASCROLL* : 
//	*/
//-------------------------------------------------------------------
IFASCROLL* IFAOBJ::GetScroll()
{
	return (IFASCROLL*)scroll;
}



//-------------------------------------------------------------------
//  IFAOBJ::GetSkinManager
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/11/2004 12:24:29
//	
//	@return				IFASKINMANAGER* : 
//	*/
//-------------------------------------------------------------------
IFASKINMANAGER* IFAOBJ::GetSkinManager()
{
	return (IFASKINMANAGER*)skinmanager;
}


//-------------------------------------------------------------------
//  IFAOBJ::GetSkinObj
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/11/2004 21:09:32
//	
//	@return				IFASKINMANAGER* : 
//	*/
//-------------------------------------------------------------------
IFASKINOBJ* IFAOBJ::GetSkinObj()
{			
	if(skinmanager)
		{
			if((!skinobj)||(skinobj->GetType()!=skinmanager->GetType()))
				{		
					delete skinobj;
					skinobj = skinmanager->GetSkinObj(this);
				}
		}
	
	return skinobj;
}



//-------------------------------------------------------------------
//  IFAOBJ::BlinkControl
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 9:44:20
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAOBJ::BlinkControl()
{	
	if(!IsActiveBlink()) return true;

	if(!(timer->GetMeasure(false)<timebyframe)) 		
		{
			timer->Reset();
			blinkstatus=!blinkstatus; 				
		}	

	return (bool)blinkstatus;
}



//-------------------------------------------------------------------
//  IFAOBJ::Clean
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:16:16
//	
//	@return        void : 
*/
//-------------------------------------------------------------------
void IFAOBJ::Clean()
{		
	type							= IFAOBJTYPE_UNKNOW;	

	canvas						= NULL;
	screen						= NULL;
	inpmanager				= NULL;	
	
	scenehdl					= 0;

	parentrect				= NULL;

	active						= true;
	evenhandlecode		= 0;
	visible						= true;
	blink							= false;
	blinkstatus				= true;

	shadow						= 0;

	selectfont				= NULL;

	features					= IFAFEATURE_NONE;

	overspace					= IFAOBJ_OVERSPACE;
	catchregion				= true;

	framebysec				= 0;
	timebyframe				= 0;
	timer							= NULL;

	delskinmanager		= false;
	skinmanager				= NULL;
	skinobj						= NULL;
	
	scroll						= NULL;

	isscreenrelative	= true;
	exparam						= 0;	
}
