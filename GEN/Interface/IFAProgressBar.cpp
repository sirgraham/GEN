//------------------------------------------------------------------------------------------
//	IFAPROGRESSBAR.CPP
//	
//	Interface Progress Bar class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 06/04/2004 10:36:47
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "GRPCanvas.h"

#include "IFAText.h"

#include "IFAProgressBar.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFAPROGRESSBAR::IFAPROGRESSBAR
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2004 16:35:08
//	
//	@return				
//	@param				canvas : 
//  @param				scene : 
//  @param				screen : 
//  @param				inpmanager : 
 
//  @param				skinmanager :
//  @param				type : 
*/
//-------------------------------------------------------------------
IFAPROGRESSBAR::IFAPROGRESSBAR(GRPCANVAS* canvas,GRPSCENE* scene,GRPSCREEN* screen,INPMANAGER* inpmanager,XFACTORY* xfactory,IFASKINMANAGER* skinmanager,IFAOBJTYPE type) : IFAFORM(canvas,screen,inpmanager,xfactory,skinmanager,type)
{
	Clean();

	this->scene = scene;

	overspace	= 2;	
}



//-------------------------------------------------------------------
//  IFAPROGRESSBAR::~IFAPROGRESSBAR
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2004 10:37:35
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFAPROGRESSBAR::~IFAPROGRESSBAR()
{
	End();

	Clean();
}



//-------------------------------------------------------------------
//  IFAPROGRESSBAR::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2004 10:47:06
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAPROGRESSBAR::Ini()
{
	textpercent = new IFATEXT(canvas,screen,inpmanager,xfactory,skinmanager);
	if(!textpercent) return false;

	textpercent->SetParentRect(&rect);
			
	textpercent->SetOrigin(IFAOBJ_CENTER,IFAOBJ_CENTER);
	
	if(features&IFAFEATURE_TEXTPERCENT) AddObj(textpercent);

	return true;
}



//-------------------------------------------------------------------
//  IFAPROGRESSBAR::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2004 10:52:25
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFAPROGRESSBAR::Draw(int x, int y)
{
	if(!canvas) return false;

	if(!BlinkControl()) return false;

	rectdirect.Set(x,y,rect.GetWidth(),rect.GetHeight());

	if(features&IFAFEATURE_BACKGROUND)
		{
			GRPCOLOR* fill = GetColor(GRPCOLORTYPE_BACKGROUND);
			GRPCOLOR* edge = GetColor(GRPCOLORTYPE_EDGE);

			canvas->DrawBox(x,y,rect.GetWidth(),rect.GetHeight(),fill->Get(),fill->GetAlpha(),1,edge->Get(),edge->GetAlpha(),shadow);	
		}

	DrawProgress(x,y);
	
	return true;
}



//-------------------------------------------------------------------
//  IFAPROGRESSBAR::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2004 10:47:16
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAPROGRESSBAR::Update()
{
	CalculatePercent();
	
	if(textpercent)
		{
			XSTRING string;

			string.Format(__L("%d%%%%"),percent);				
			textpercent->SetText(string);			
		}

	IFAFORM::Update();

	return true;
}



//-------------------------------------------------------------------
//  IFAPROGRESSBAR::End
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2004 10:52:04
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAPROGRESSBAR::End()
{
	DeleteAllObj();

	delete textpercent;
	textpercent = NULL;

	return true;
}



//-------------------------------------------------------------------
//  IFAPROGRESSBAR::CalculatePercent
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2004 10:52:04
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int IFAPROGRESSBAR::CalculatePercent()
{
	if(maxvalue-minvalue)
					percent = ((index*100)/(maxvalue-minvalue));
		else  percent = 100;

	return percent;
}


//-------------------------------------------------------------------
//  IFAPROGRESSBAR::GetTextPercent
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2004 17:17:02
//	
//	@return				IFATEXT* : 
//	*/
//-------------------------------------------------------------------
IFATEXT* IFAPROGRESSBAR::GetTextPercent()
{
	return textpercent;
}


//-------------------------------------------------------------------
//  IFAPROGRESSBAR::GetBarColor
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2004 13:24:13
//	
//	@return				GRPCOLORS* : 
//	*/
//-------------------------------------------------------------------
GRPCOLORS* IFAPROGRESSBAR::GetBarColor()
{
	return &barcolors;
}



//-------------------------------------------------------------------
//  IFAPROGRESSBAR::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2004 10:38:02
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFAPROGRESSBAR::Clean()
{
	percent						= 0;
	maxvalue					= 100;
	minvalue					= 0;
	index							= 0;
	thickness					= 1;
	separation				= 0;
		
	scene							= NULL;

	handletextpercent	= 0;
	textpercent				= NULL;
}



//-------------------------------------------------------------------
//  IFAPROGRESSBAR::DrawProgress
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2004 13:27:55
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFAPROGRESSBAR::DrawProgress(int x, int y)
{
	if(!canvas) return false;

	GRPCOLOR* color		= barcolors.GetColor(GRPCOLORTYPE_COLOR);
	GRPCOLOR* edge		= barcolors.GetColor(GRPCOLORTYPE_EDGE);
	GRPCOLOR  fcolor;
	GRPRECT   vprect;
	int				st			= (features&IFAFEATURE_VERTICAL)?rect.GetHeight()-2:rect.GetWidth()-2;
	int				se			= thickness + separation;
	int				net			= (st/se);
	int       ne			= ((net*index)/(maxvalue-minvalue))+1;
	int				c;

	vprect.Set(x+1,y+1,rect.GetWidth()-2,rect.GetHeight()-2);

	canvas->SetViewPort(&vprect);

	if(!(features&IFAFEATURE_PROGRESSCOLOR)) fcolor.Set(color);

	if(features&IFAFEATURE_VERTICAL)
		{
			int posy = 0;
			for(c=0;c<ne;c++)
				{
					canvas->DrawBox(x+1	,y+posy, rect.GetWidth()-2, thickness,fcolor.Get(),fcolor.GetAlpha(),1,edge->Get(),edge->GetAlpha());											
					posy+=se;
				}
		}
	 else
		{
			int posx = 0;
			for(c=0;c<ne;c++)
				{		
					canvas->DrawBox(x+posx,y+1,thickness,rect.GetHeight()-2,fcolor.Get(),fcolor.GetAlpha(),1,edge->Get(),edge->GetAlpha());							
					posx+=se;
				}
		}

	canvas->SetViewPort(NULL);

	return true;
}
