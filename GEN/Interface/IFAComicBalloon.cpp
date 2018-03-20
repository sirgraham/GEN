//------------------------------------------------------------------------------------------
//	IFACOMICBALLOON.CPP
//	
//	Talk Comic Class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 26/02/2004 15:33:37
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPRect.h"
#include "GRPCanvas.h"
#include "GRPFont.h"
#include "GRPScene.h"
#include "GRPScreen.h"

#include "IFASkinManager.h"
#include "IFAScroll.h"
#include "IFATextInBox.h"

#include "IFAComicBalloon.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  IFACOMICBALLOON::IFACOMICBALLOON
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2004 21:29:11
//	
//	@return				void : 
//	@param				canvas : 
//  @param				scene : 
//  @param				screen : 
//  @param				inpmanager : 
 
//  @param				sourcerect :
*/
//-------------------------------------------------------------------
IFACOMICBALLOON::IFACOMICBALLOON(GRPCANVAS* canvas,GRPSCENE* scene,GRPSCREEN* screen,INPMANAGER* inpmanager,XFACTORY* xfactory,GRPRECT* sourcerect) : IFAFORM(canvas,screen,inpmanager,xfactory,NULL)
{
	Clean();	

	this->scene			 = scene;	
	this->sourcerect.Set(sourcerect);

	if(skinmanager) skinmanager->SetType(IFASKINTYPE_COMIC);

	overspace	= 10;

	SetFeatures(IFAFEATURE_BACKGROUND);

	textib = new IFATEXTINBOX(canvas, screen,inpmanager,xfactory,skinmanager);

	if((scene)&&(textib))
		{				
			AddObj(textib);
			hdlscene = scene->AddElement(GRPSCENEELEMENT_TYPE_IFA,(void*)this);		

			SetVisible(false);					
		}
}


//-------------------------------------------------------------------
//  IFACOMICBALLOON::~IFACOMICBALLOON
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2004 15:34:22
//	
//	@return				virtual : 
//	*/
//-------------------------------------------------------------------
IFACOMICBALLOON::~IFACOMICBALLOON()
{
	scene->DelElement(hdlscene);		
	
	delete textib;
	
	Clean();
}


//-------------------------------------------------------------------
//  IFACOMICBALLOON::SetSourceRect
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			17/06/2004 17:35:43
//	
//	@return				bool : 
//	@param				sourcerect : 
*/
//-------------------------------------------------------------------
bool IFACOMICBALLOON::SetSourceRect(GRPRECT* sourcerect)
{
	this->sourcerect.Set(sourcerect);

	return true;
}



//-------------------------------------------------------------------
//  IFACOMICBALLOON::SetColor
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 11:05:20
//	
//	@return				bool : 
//	@param				type : 
//	@param				mode :
//  @param				RGB : 
//  @param				alpha : 
*/
//-------------------------------------------------------------------
bool IFACOMICBALLOON::SetColor(GRPCOLORTYPE type,GRPPROPERTYMODE mode,XBYTE* RGB,XBYTE alpha)
{
	if(!textib)	return false;

	SetColor(type , mode, RGB, alpha);		

	textib->SetColor(type , mode, RGB[GRPCOLOR_COMPONENT_RED]
															, RGB[GRPCOLOR_COMPONENT_GREEN]
															, RGB[GRPCOLOR_COMPONENT_BLUE]
															, alpha);	

	return true;
}



//-------------------------------------------------------------------
//  IFACOMICBALLOON::SetColor
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 11:02:48
//	
//	@return				bool : 
//	@param				type : 
//  @param				color : 
*/
//-------------------------------------------------------------------
bool IFACOMICBALLOON::SetColor(GRPCOLORTYPE type,GRPCOLOR* color)
{
	if(!textib)	return false;

	IFAFORM::SetColor(type , color);		
	textib->SetColor(type , color);	

	return true;
}



//-------------------------------------------------------------------
//  IFACOMICBALLOON::ShowSentence
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/07/2004 18:03:31
//	
//	@return				bool : 
//	@param				text : 
//  @param				xsenseajust : 
//  @param				 : 
//  @param				talk : 
*/
//-------------------------------------------------------------------
bool IFACOMICBALLOON::ShowSentence(XCHAR* text,int xsenseajust, int ysenseajust ,bool talk)
{
	if(!scene)				return false;
	if(!canvas)				return false;
	if(!textib)				return false;	
	if(!hdlscene)			return false;

	scene->GotoToFinalQueue(hdlscene);

	if(!text)
		{						
			if(textib->GetScroll()->IsAtEnd(IFASCROLLPOS_YEND))
				{
					textib->GetScroll()->SetStep(0,0,false,0,0);				
					SetVisible(false);
					return false;
				} 
			 else 
				{
					textib->GetScroll()->SetStep(0,textib->GetRect()->GetHeight(),true, 0,IFACOMICBALLOON_STEP);				
					return true;	
				}
				
		} else textib->SetText(text);

	XSTRING* ptext;

	ptext = textib->GetText();
	if(!ptext)					 return false;
	if(ptext->IsEmpty()) return false;


	PushSelectFont();

	int			 xmaxwidth		 = (screen->GetWidth() >= canvas->GetWidth() ) ? canvas->GetWidth() :screen->GetWidth();
	int			 ymaxheight		 = (screen->GetHeight()>= canvas->GetHeight()) ? canvas->GetHeight():screen->GetHeight();
	int			 xsourcemiddle = sourcerect.GetX() + (sourcerect.GetWidth()  / 2);
//int			 ysourcemiddle = sourcerect.GetY() + (sourcerect.GetHeight() / 2);
	int			 wtextmax;
	bool		 toleft;
	int			 sense				 = talk?GRPCOMICBALLONSENSE_TALK:GRPCOMICBALLONSENSE_THINK;
	int			 scrollsize		 = 0;
	int			 letterh			 = canvas->CalculateHeightText();
	GRPRECT	 trect;

	xsourcemiddle -= canvas->GetScreenZone()->GetX();
//ysourcemiddle -= canvas->GetScreenZone()->GetY();

	if(xsourcemiddle>(xmaxwidth-xsourcemiddle))
		{			
			toleft   = true;
			sense |= GRPCOMICBALLONSENSE_RIGHT	 |GRPCOMICBALLONSENSE_TORIGHT;
			wtextmax = (xsourcemiddle - (18+8));	
		}
	 else
		{			
			toleft   = false;
			sense |= GRPCOMICBALLONSENSE_LEFT	 |GRPCOMICBALLONSENSE_TOLEFT;
			wtextmax = (xmaxwidth-xsourcemiddle) - (18+8);	
		}

	if(wtextmax>((int)((xmaxwidth/1.3))-24)) wtextmax = ((int)((xmaxwidth/1.3))-24);
	
	int wmtext = canvas->CalculateWidthText(ptext->Get());
	if(wtextmax>=wmtext) wtextmax = wmtext+canvas->CalculateWidthText(__L("A"));

	canvas->CalculateRectTextInBox(ptext->Get(),wtextmax,2,trect);	

	textib->GetScroll()->SetColor(GRPCOLORTYPE_COLOR,GetColor(GRPCOLORTYPE_COLOR)/*,25*/);	
	textib->GetScroll()->ActiveBlink(true);
	textib->GetScroll()->SetFeatures(IFAFEATURE_VBOTTONINDICATOR|IFAFEATURE_INDICATORSIFNEED);

	if(trect.GetHeight()>=(letterh*5))  trect.SetHeight(letterh*5);			
		
	textib->SetOrigin(5,5);
	textib->SetSize(trect.GetWidth(),trect.GetHeight());
	textib->SetCatchRegion(false);

	textib->CalculateFullRect();
	if(textib->GetRect()->GetHeight()!=textib->GetRectFull()->GetHeight()) scrollsize = textib->GetScroll()->GetSize();			

	textib->GetRectFull()->Set(0,0,0,0);

	int xformsize = (trect.GetWidth()  + scrollsize + 9);
	int yformsize = (trect.GetHeight() + 9);
	int xpos;
	
	if(toleft)
		   xpos = (xsourcemiddle - xformsize + xsenseajust);
	else xpos = (xsourcemiddle             - xsenseajust);

	int ypos = (sourcerect.GetY() + sourcerect.GetHeight() - ysenseajust - canvas->GetScreenZone()->GetY());	
	
	if((ypos+yformsize)>ymaxheight)
		{ 
			sense |= GRPCOMICBALLONSENSE_UP;			
			ypos	-= (yformsize + (10*2) + 5); // 10 (size of sense)
		}
	 else sense |= GRPCOMICBALLONSENSE_DOWN;						
		
	SetOrigin(xpos,ypos);			
	SetSize(xformsize,yformsize);	

	SetSense(sense);
	GetScroll()->SetFeatures(0);
	SetVisible(true);

	PopSelectFont();

	return true;
}


//-------------------------------------------------------------------
//  IFACOMICBALLOON::GetTextInBox
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/08/2004 15:14:26
//	
//	@return				IFATEXTINBOX* : 
//	*/
//-------------------------------------------------------------------
IFATEXTINBOX* IFACOMICBALLOON::GetTextInBox()
{
	return textib;
}


//-------------------------------------------------------------------
//  IFACOMICBALLOON::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2004 15:34:50
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFACOMICBALLOON::Clean()
{
	scene								 = NULL;
	
	hdlscene						 = 0;
	hdlsource						 = 0;
	scrollsignal				 = 0;

	textib							 = NULL;
}


