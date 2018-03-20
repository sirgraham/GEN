//------------------------------------------------------------------------------------------
//	IFABITMAP.CPP
//	
//	Interface Bitmap Class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 31/12/2003 10:32:06
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "GRPCanvas.h"

#include "IFABitmap.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFABITMAP::IFABITMAP
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2004 20:40:15
//	
//	@return				void : 
//	@param				canvas : 
//  @param				screen : 
//  @param				inpmanager : 
 
//  @param				skinmanager : 
//  @param				type : 
*/
//-------------------------------------------------------------------
IFABITMAP::IFABITMAP(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager,XFACTORY* xfactory,IFASKINMANAGER* skinmanager,IFAOBJTYPE type) : IFAOBJ(canvas,screen,inpmanager,xfactory,skinmanager,type)
{
	Clean();
}


//-------------------------------------------------------------------
//  IFABITMAP::~IFABITMAP
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/12/2003 10:33:07
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFABITMAP::~IFABITMAP()
{
	Clean();
}


//-------------------------------------------------------------------
//  IFABITMAP::GetAlpha
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/09/2004 15:43:19
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int IFABITMAP::GetAlpha()
{
	return alpha;
}


//-------------------------------------------------------------------
//  IFABITMAP::SetAlpha
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/09/2004 15:43:50
//	
//	@return				bool : 
//	@param				alpha : 
*/
//-------------------------------------------------------------------
bool IFABITMAP::SetAlpha(int alpha)
{
	this->alpha = alpha;

	return true;
}


//-------------------------------------------------------------------
//  IFABITMAP::GetBitmap
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/12/2003 10:33:57
//	
//	@return				GRPBITMAP* : 
//	*/
//-------------------------------------------------------------------
GRPBITMAP* IFABITMAP::GetBitmap()
{
	return bitmap;
}


//-------------------------------------------------------------------
//  IFABITMAP::SetBitmap
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/12/2003 10:34:26
//	
//	@return				bool : 
//	@param				bitmap : 
*/
//-------------------------------------------------------------------
bool IFABITMAP::SetBitmap(GRPBITMAP* bitmap)
{
	//if(!bitmap) return false;

	this->bitmap = bitmap;

	if(bitmap) 
		 		 SetSize(bitmap->GetWidth(),bitmap->GetHeight());
	  else SetSize(0,0);

	return true;
}
		


//-------------------------------------------------------------------
//  IFABITMAP::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 17:25:03
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFABITMAP::Draw(int x, int y)
{
	if(!bitmap) return false;

	if(!BlinkControl()) return false;

	rectdirect.Set(x,y,bitmap->GetWidth(),bitmap->GetHeight());
	
 	canvas->PutBitmap(x,y,bitmap,(IFAFEATURE_TRANSPARENT&features)?true:false,alpha);

	return true;
}

	

//-------------------------------------------------------------------
//  IFABITMAP::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/12/2003 10:35:55
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFABITMAP::Clean()
{
	bitmap = NULL;
	alpha  = 100;
}


