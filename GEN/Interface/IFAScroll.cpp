//------------------------------------------------------------------------------------------
//	IFASCROLL.CPP
//	
//	Scroll Indicator for the Interface
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 08/03/2004 11:31:26
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>

#include "XTime.h"

#include "GRPRect.h"
#include "GRPCanvas.h"
#include "IFAObj.h"

#include "IFAScroll.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFASCROLL::IFASCROLL
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2004 20:41:20
//	
//	@return				void : 
//	@param				canvas : 
//  @param				screen : 
//  @param				inpmanager : 
 
//  @param				skinmanager : 
//  @param				type : 
*/
//-------------------------------------------------------------------
IFASCROLL::IFASCROLL(GRPCANVAS* canvas, GRPSCREEN* screen,INPMANAGER* inpmanager,XFACTORY* xfactory,IFASKINMANAGER* skinmanager,IFAOBJTYPE type) :  IFAOBJ(canvas,screen,inpmanager,xfactory,skinmanager,type)
{
	Clean();

	SetSize(IFASCROLL_STANDARDSIZE);

	SetFeatures(IFAFEATURE_ALLINDICATORS);

	AjustTime(IFASCROLL_BLINKFRAMESSECOND);
}


//-------------------------------------------------------------------
//  IFASCROLL::~IFASCROLL
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 11:33:27
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFASCROLL::~IFASCROLL()
{
	Clean();
}


//-------------------------------------------------------------------
//  IFASCROLL::SetRects
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 12:20:30
//	
//	@return				bool : 
//	@param				rect : 
//  @param				rectfull : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::SetRects(GRPRECT* rect,GRPRECT* rectfull)
{
	if(!rect)			 return false;
	if(!rectfull)  return false;

	objrect			= rect;
	objrectfull = rectfull;

	return true;
}


//-------------------------------------------------------------------
//  IFASCROLL::GetSize
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 13:21:24
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int IFASCROLL::GetSize()
{
	return size;
}


//-------------------------------------------------------------------
//  IFASCROLL::SetSize
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 13:21:39
//	
//	@return				bool : 
//	@param				size : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::SetSize(int size)
{
	this->size = size;

	if(!(this->size%2)) this->size++;

	return true;
}


//-------------------------------------------------------------------
//  IFASCROLL::IsAvailable
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 11:46:18
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFASCROLL::IsAvailable()
{
	if((horizontalON)||(verticalON)) return true;

	return false;
}


//-------------------------------------------------------------------
//  IFASCROLL::IsHorizontalAvailable
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 13:08:13
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFASCROLL::IsHorizontalAvailable()
{
	return horizontalON;
}


//-------------------------------------------------------------------
//  IFASCROLL::IsVerticalAvailable
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 13:07:40
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFASCROLL::IsVerticalAvailable()
{
	return verticalON;
}


//-------------------------------------------------------------------
//  IFASCROLL::SetOrigin
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2004 15:15:38
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFASCROLL::SetOrigin()
{
  posx = 0;
	posy = (objrectfull->GetHeight() - objrect->GetHeight());

	stepx = 0;
	stepy = 0;

	animationstepx = 0;
	animationstepy = 0;
	
	return true;
}



//-------------------------------------------------------------------
//  IFASCROLL::SetEnd
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2004 15:52:45
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFASCROLL::SetEnd()
{
	posx = 0;
	posy = 0;

	stepx = 0;
	stepy = 0;

	animationstepx = 0;
  animationstepy = 0;

	return true;	
}


//-------------------------------------------------------------------
//  IFASCROLL::IsAtEnd
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2004 16:00:51
//	
//	@return				bool : 
//	@param				pos : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::IsAtEnd(IFASCROLLPOS pos)
{
	switch(pos)
		{
			case IFASCROLLPOS_NONE		:	break;
			case IFASCROLLPOS_XSTART	:	if(posx==-objrectfull->GetWidth())													return true;
			case IFASCROLLPOS_XEND		: if(posx==-(objrectfull->GetWidth() - objrect->GetWidth()))	return true;																	
			case IFASCROLLPOS_YSTART	: if(posy==(objrectfull->GetHeight() - objrect->GetHeight()))	return true;
			case IFASCROLLPOS_YEND		: if(!posy)																										return true;
		}

	return false;	
}


//-------------------------------------------------------------------
//  IFASCROLL::SetStep
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2004 15:22:12
//	
//	@return				bool : 
//	@param				stepx : 
//  @param				stepy : 
//  @param				animation : 
//  @param				incx : 
//  @param				incy : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::SetStep(int stepx,int stepy,bool animation, int incx, int incy)
{	
	this->stepx = stepx;
	this->stepy = stepy;
	
	if(animation)
		{
			this->animation		= true;
			animationincx			= incx;
			animationincy			= incy;
			animationstepx	  = 0;
	 	  animationstepy		= 0;
		}
	 else 
		{
			this->animation		= false;		 
		}

	return true;
}


//-------------------------------------------------------------------
//  IFASCROLL::SetGoStep
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/04/2004 17:39:48
//	
//	@return				bool : 
//	@param				gotox : 
//  @param				gotoy : 
//  @param				animation : 
//  @param				incx : 
//  @param				incy : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::SetGoStep(int gotox,int gotoy,bool animation, int incx, int incy)
{
	int sx = abs(gotox - posx);
	int sy = abs(gotoy - posy);

	return SetStep(sx,sy,animation,incx,incy);
}


//-------------------------------------------------------------------
//  IFASCROLL::MakeStep
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2004 15:26:49
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFASCROLL::MakeStep()
{	
	if(animation)
		{
			if(animationstepx<stepx) 
				{
					posx						-= animationincx;
					animationstepx  += abs(animationincx);
				}
			if(animationstepy<stepy) 
				{
					posy						-= animationincy;
					animationstepy  += abs(animationincy);
				}			
		}
	 else
	  {
			posx -= stepx;
			posy -= stepy;
	  }

		
	if(posx>0)																										posx = 0;	
	if(posx<-(objrectfull->GetWidth() - objrect->GetWidth()))			posx = -(objrectfull->GetWidth()  - objrect->GetWidth());		
	if(posy<0)																										posy = 0;
	if(posy>(objrectfull->GetHeight() - objrect->GetHeight()))		posy = (objrectfull->GetHeight() - objrect->GetHeight());
	
	return true;
}


//-------------------------------------------------------------------
//  IFASCROLL::IsMakeStep
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/04/2004 16:40:35
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFASCROLL::IsMakeStep()
{
	if((animationstepx<stepx)||(animationstepy<stepy)) return true;

	return false;
}


//-------------------------------------------------------------------
//  IFASCROLL::IncrementStep
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 11:49:15
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::IncrementStep(int x,int y)
{
	objrectfull->SetOriginX(x + posx);
	objrectfull->SetOriginY(y - posy);
	
	return true;
}


//-------------------------------------------------------------------
//  IFASCROLL::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 11:34:07
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::Draw(int x, int y)
{
	GRPRECT trect;

	if(!BlinkControl()) return false;

	if(horizontalON) DrawScrollH(x,y-size-1,objrect->GetWidth());									
	if(verticalON)	 DrawScrollV(x+objrect->GetWidth()+1,y,objrect->GetHeight());

	return true;
}


//-------------------------------------------------------------------
//  IFASCROLL::SetAvailable
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/03/2004 11:46:15
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFASCROLL::SetAvailable()
{
	horizontalON = false;
	verticalON	 = false;

	rectdirect.Set(0,0,0,0);

	if(objrectfull->GetWidth()  > objrect->GetWidth())   horizontalON = true; 
	if(objrectfull->GetHeight() > objrect->GetHeight())  verticalON	  = true;

	return true;
}



//-------------------------------------------------------------------
//  IFASCROLL::DrawScrollSignal
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 11:58:55
//	
//	@return				bool : 
//	@param				direction : 
//  @param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::DrawScrollSignal(IFAOBJLOCATION location,int x,int y)
{
	if(!canvas) return false;
	
	int c;

	//XBYTE RGB[4] = { 0x00,0x00,0x00,0x00 };

	//canvas->DrawRect(x,y,size,size,colors[GRPCOLORTYPE_COLOR].RGB,1,colors[GRPCOLORTYPE_COLOR].alpha);	

	switch(location)
		{
			case IFAOBJLOCATION_UP		:
			case IFAOBJLOCATION_RIGHT	: for(c=0;c<size;c++)
																	  {	
																			 if(location == IFAOBJLOCATION_UP)
																	 						canvas->DrawLine(x+c							, y+c+(size/2), x+size-c-1				,y+c+(size/2)	, GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());	
																				 else canvas->DrawLine(x+c-size+(size/2), y+c					, x+c-size+(size/2)	,y+size-c-1		, GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());																			 

																			 if(c>=(size-c-1)) break;
																	  }
																	break;

			case IFAOBJLOCATION_DOWN	:		
			case IFAOBJLOCATION_LEFT  : for(c=0;c<size;c++)
																	  {	
																			 if(location == IFAOBJLOCATION_DOWN)
																							 canvas->DrawLine(x+c									,	y+size-c-1-(size/2) ,x+size-c-1						,y+size-c-1-(size/2)	,GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());	
																	 				else canvas->DrawLine(x-c+size-(size/2)-1 ,	y+c									,x-c+size-(size/2)-1	,y+size-c-1						,GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());						
																			
																			 if(c>=(size-c-1)) break;		
																	  }
																	break;

											default   : break;
		}

	return true;
}



//-------------------------------------------------------------------
//  IFASCROLL::DrawScrollH
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 15:46:15
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
//  @param				sizebar : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::DrawScrollH(int x,int y,int sizebar)
{
	if(!canvas) return false;

	int htf    = objrectfull->GetWidth();	
	int htp		 = objrect->GetWidth();
	int hrf	   = sizebar-(size*2)+((size/2)*2);	
	int rhrp   = ((htp*hrf)%htf);
	int hrp    = ((htp*hrf)/htf)+ (rhrp?1:0);
	int xp     = -((hrf*posx)/htf);

	if(features&IFAFEATURE_HBARINDICATOR)
		{
			canvas->DrawBox(x+xp+(size/2)+1,
											y,
											hrp,
											size,
											GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());

			if(xp) canvas->DrawLine(x+(size/2)+1,
															y+(size/2),
															x+(size/2)+xp,
															y+(size/2),
															GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());

			if(xp+(size/2)+hrp+1<sizebar-size+(size/2)-1)	canvas->DrawLine(x+xp+(size/2)+hrp+1,
																																		y+(size/2),
																																		x+sizebar-size+(size/2)-1,
																																		y+(size/2),
																																		GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());
		}

	


	if(features&IFAFEATURE_HRIGHTINDICATOR) 
		{
			if(features&IFAFEATURE_INDICATORSIFNEED) 
				{
					if(!IsAtEnd(IFASCROLLPOS_XSTART)) DrawScrollSignal(IFAOBJLOCATION_LEFT ,x , y);

				} else DrawScrollSignal(IFAOBJLOCATION_LEFT ,x , y);
		}

	if(features&IFAFEATURE_HLEFTINDICATOR)  
		{
			if(features&IFAFEATURE_INDICATORSIFNEED) 
				{
					if(!IsAtEnd(IFASCROLLPOS_XEND)) DrawScrollSignal(IFAOBJLOCATION_RIGHT ,x+sizebar,y);

				} else DrawScrollSignal(IFAOBJLOCATION_RIGHT ,x+sizebar,y);
		}


	return true;
}



//-------------------------------------------------------------------
//  IFASCROLL::DrawScrollV
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 15:46:15
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
//  @param				sizebar : 
//  @param				height : 
*/
//-------------------------------------------------------------------
bool IFASCROLL::DrawScrollV(int x,int y,int sizebar)
{
	if(!canvas) return false;

	int vtf    = objrectfull->GetHeight();	
	int vtp		 = objrect->GetHeight();
	int vrf	   = sizebar-(size*2)+((size/2)*2);	
	int rvrp   = ((vtp*vrf)%vtf);
	int vrp    = ((vtp*vrf)/vtf)+ (rvrp?1:0);
	int yp     = ((vrf*posy)/vtf);

	if(features&IFAFEATURE_VBARINDICATOR)
		{
			canvas->DrawBox(x,
											y+yp+(size/2)+1,
											size,
											vrp,
											GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());
			
			if(yp) canvas->DrawLine(x+(size/2),
								   						y+(size/2)+1,
															x+(size/2),
															y+yp+(size/2),
															GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());
			
			if(yp+(size/2)+vrp+1<sizebar-size+(size/2)-1)	canvas->DrawLine(x+(size/2),
																																		y+yp+(size/2)+vrp+1,
																																		x+(size/2),
																																		y+sizebar-size+(size/2)-1,
																																		GetColor(GRPCOLORTYPE_COLOR)->Get(),GetColor(GRPCOLORTYPE_COLOR)->GetAlpha());
		}		

	if(features&IFAFEATURE_VTOPINDICATOR)		
		{
			if(features&IFAFEATURE_INDICATORSIFNEED) 
				{
					if(!IsAtEnd(IFASCROLLPOS_YSTART)) DrawScrollSignal(IFAOBJLOCATION_UP ,x,y+sizebar-size);

				} else DrawScrollSignal(IFAOBJLOCATION_UP ,x,y+sizebar-size);
		}

	if(features&IFAFEATURE_VBOTTONINDICATOR) 
		{
			if(features&IFAFEATURE_INDICATORSIFNEED) 
				{		
					if(!IsAtEnd(IFASCROLLPOS_YEND)) DrawScrollSignal(IFAOBJLOCATION_DOWN ,x,y);

				} else DrawScrollSignal(IFAOBJLOCATION_DOWN ,x,y);
		} 

	return true;
}





//-------------------------------------------------------------------
//  IFASCROLL::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2004 11:34:51
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFASCROLL::Clean()
{	
	objrect						= NULL;
	objrectfull				= NULL;

	horizontalON			= false;					
	verticalON				= false;

	features					= 0;

	size							= IFASCROLL_STANDARDSIZE;

	posx							= 0;
	posy							= 0;
	stepx							=	0;
	stepy							= 0;	

	animation					= false;
	animationincx			= 0;
	animationincy			= 0;	
	animationstepx		= 0;
	animationstepy		= 0;
}
