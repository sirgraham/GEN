//------------------------------------------------------------------------------------------
//	IFATEXTINBOX.CPP
//	
//	Text in Box Obj class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/12/2003 13:09:58
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPFont.h"
#include "GRPCanvas.h"
#include "IFAScroll.h"

#include "IFATextInBox.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFATEXTINBOX::IFATEXTINBOX
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2004 20:37:58
//	
//	@return				void : 
//	@param				canvas : 
//  @param				screen : 
//  @param				inpmanager : 
 
//  @param				skinmanager : 
//  @param				type : 
*/
//-------------------------------------------------------------------
IFATEXTINBOX::IFATEXTINBOX(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager,XFACTORY* xfactory,IFASKINMANAGER* skinmanager,IFAOBJTYPE type) : IFATEXT(canvas,screen,inpmanager,xfactory,skinmanager,type)
{
	Clean();
}


//-------------------------------------------------------------------
//  IFATEXTINBOX::~IFATEXTINBOX
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2003 13:16:30
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFATEXTINBOX::~IFATEXTINBOX()
{
	Clean();
}



//-------------------------------------------------------------------
//  IFATEXTINBOX::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 17:21:38
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFATEXTINBOX::Draw(int x, int y)
{
	if(!canvas) return false;

	if(!BlinkControl()) return false;

	rectdirect.Set(&rect);

	PushSelectFont();
	
	GRPFONT* font = canvas->GetFontManager()?canvas->GetFontManager()->GetSelectFont():NULL;
	
	if(font) 
		{
			font->SetColor(GRPCOLORTYPE_COLOR	   , GetColor(GRPCOLORTYPE_COLOR));
			font->SetColor(GRPCOLORTYPE_EDGE	   , GetColor(GRPCOLORTYPE_EDGE));
			font->SetColor(GRPCOLORTYPE_SHADOW	   , GetColor(GRPCOLORTYPE_SHADOW));
			font->SetColor(GRPCOLORTYPE_BACKGROUND , GetColor(GRPCOLORTYPE_BACKGROUND));
		}

	canvas->SetViewPort(&rect);

	canvas->DrawTextInBox(&rectfull,transparent,true,2,GRPTEXTFORMATPARAGRAPH_LEFT,text.Get());

	canvas->SetViewPort(NULL);

	PopSelectFont();

	return true;
}


//-------------------------------------------------------------------
//  IFATEXTINBOX::CalculateFullRect
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2004 11:30:52
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFATEXTINBOX::CalculateFullRect()
{
	rectfull.Set(&rect);

	PushSelectFont();

	if(!canvas->CalculateRectTextInBox(text.Get(),rect.GetWidth(),2,rectfull)) return false;

	PopSelectFont();

	if(rectfull.GetWidth()  < rect.GetWidth())   rectfull.SetWidth(rect.GetWidth());
	if(rectfull.GetHeight() < rect.GetHeight())  rectfull.SetHeight(rect.GetHeight());	 

	return true;
}


//-------------------------------------------------------------------
//  IFATEXTINBOX::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2003 13:17:01
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFATEXTINBOX::Clean()
{
	
}

