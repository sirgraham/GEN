//------------------------------------------------------------------------------------------
//	IFATEXT.CPP
//	
//	Interface Functions Add (Text Class)
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPFont.h"
#include "GRPCanvas.h"

#include "IFAText.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  IFATEXT::IFATEXT
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2004 20:37:04
//	
//	@return				void : 
//	@param				canvas : 
//  @param				screen : 
//  @param				inpmanager : 
 
//  @param				skinmanager : 
//  @param				type : 
*/
//-------------------------------------------------------------------
IFATEXT::IFATEXT(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager,XFACTORY* xfactory,IFASKINMANAGER* skinmanager,IFAOBJTYPE type) : IFAOBJ(canvas,screen,inpmanager,xfactory,skinmanager,type)
{
	Clean();
}


//-------------------------------------------------------------------
// IFATEXT::~IFATEXT
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:19:39
//	
//	@return          
*/
//-------------------------------------------------------------------
IFATEXT::~IFATEXT()
{
	Clean();
}

//-------------------------------------------------------------------
// IFATEXT::GetText
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:19:39
//	
//	@return         : 
*/
//-------------------------------------------------------------------
XSTRING* IFATEXT::GetText()
{
	return &text;
}


//-------------------------------------------------------------------
//  IFATEXT::SetText
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2003 16:02:42
//	
//	@return				bool : 
//	@param				text : 
*/
//-------------------------------------------------------------------
bool IFATEXT::SetText(XCHAR* text)
{
	if(!text) return false;
	
	PushSelectFont();

	this->text = text;
												
	rect.SetWidth(canvas->CalculateWidthText(text));
	rect.SetHeight(canvas->CalculateHeightText());

	rectfull.Clear();

	PopSelectFont();

	return true;
}


/*-------------------------------------------------------------------
//  IFATEXT::SetText
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/09/2011 13:30:32
//	
//	@return				bool : 
//	@param				text : 
*/
/*-----------------------------------------------------------------*/
bool IFATEXT::SetText(XSTRING& text)
{
	return SetText(text.Get());
}


//-------------------------------------------------------------------
//  IFATEXT::GetTransparent
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/03/2004 10:23:59
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFATEXT::GetTransparent()
{
	return transparent;
}



//-------------------------------------------------------------------
//  IFATEXT::SetTransparent
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/03/2004 10:23:24
//	
//	@return				bool : 
//	@param				on : 
*/
//-------------------------------------------------------------------
bool IFATEXT::SetTransparent(bool on)
{
	transparent = on;

	return true;
}


//-------------------------------------------------------------------
//  IFATEXT::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 17:23:57
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFATEXT::Draw(int x, int y)
{
	if(!canvas) return false;

	if(!BlinkControl()) return false;
	
	rectdirect.Set(x,y,rect.GetWidth(),rect.GetHeight());

	PushSelectFont();

	GRPFONT* font = canvas->GetFontManager()?canvas->GetFontManager()->GetSelectFont():NULL;
	if(font)
		{
			font->SetColor(GRPCOLORTYPE_COLOR			 , GetColor(GRPCOLORTYPE_COLOR));
			font->SetColor(GRPCOLORTYPE_EDGE			 , GetColor(GRPCOLORTYPE_EDGE));
			font->SetColor(GRPCOLORTYPE_SHADOW		 , GetColor(GRPCOLORTYPE_SHADOW));
			font->SetColor(GRPCOLORTYPE_BACKGROUND , GetColor(GRPCOLORTYPE_BACKGROUND));
		
			int widthtext = canvas->CalculateWidthText(text.Get());
			if(widthtext>rect.GetWidth())
				{
					XSTRING _text;
					int				 c = 0;

					while(c<(int)text.GetSize())
						{
							_text.Add(text.Get()[c]);
							c++;

							int _widthtext = canvas->CalculateWidthText(_text.Get());
							if(_widthtext>rect.GetWidth()) break;
						}

					canvas->DrawText(x,y,transparent,_text.Get());
				} 
			 else canvas->DrawText(x,y,transparent,text.Get());
		}	

	PopSelectFont();

	return true;
}


	
//-------------------------------------------------------------------
//  IFATEXT::Clean
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:16:16
//	
//	@return        void : 
*/
//-------------------------------------------------------------------
void IFATEXT::Clean()
{
	transparent = true;
}


