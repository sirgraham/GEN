//------------------------------------------------------------------------------------------
//	IFAFORM.CPP
//	
//	Interface Functions Add (Form Class)
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "GRPScreen.h"
#include "GRPCanvas.h"

#include "IFASkinManager.h"
#include "IFAScroll.h"

#include "IFAForm.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  IFAFORM::IFAFORM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2004 20:32:03
//	
//	@return				void : 
//	@param				canvas : 
//  @param				screen : 
//  @param				inpmanager : 
 
//  @param				skinmanager : 
//  @param				type : 
*/
//-------------------------------------------------------------------
IFAFORM::IFAFORM(GRPCANVAS* canvas,GRPSCREEN* screen,INPMANAGER* inpmanager,XFACTORY* xfactory,IFASKINMANAGER* skinmanager,IFAOBJTYPE type) : IFAOBJ(canvas,screen,inpmanager,xfactory,skinmanager,type)
{
	Clean();

	overspace	= 4;
}


//-------------------------------------------------------------------
// IFAFORM::~IFAFORM
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:19:39
//	
//	@return         : 
*/
//-------------------------------------------------------------------
IFAFORM::~IFAFORM()
{
	DeleteAllObj();

	Clean();
}



/*-------------------------------------------------------------------
//  IFAFORM::SetCaption
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/09/2011 9:59:59
//	
//	@return				bool : 
//	@param				caption : 
*/
/*-----------------------------------------------------------------*/
bool IFAFORM::SetCaption(XCHAR* caption)
{
	this->caption = caption;

	return true;
}



/*-------------------------------------------------------------------
//  IFAFORM::SetCaption
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/09/2011 10:00:03
//	
//	@return				bool : 
//	@param				caption : 
*/
/*-----------------------------------------------------------------*/
bool IFAFORM::SetCaption(XSTRING& caption)
{
	return SetCaption(caption.Get());
}




//-------------------------------------------------------------------
//  IFAFORM::SetEdgeIn
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 15:57:42
//	
//	@return				bool : 
//	@param				width : 
//  @param				height : 
*/
//-------------------------------------------------------------------
bool IFAFORM::SetEdgeIn(int width,int height)
{
	widthedgein	 = width;
	heightedgein = height;

	return true;
}




/*-------------------------------------------------------------------
//  IFAFORM::GetSense
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 18:40:18
//	
//	@return				XBYTE : 
//	*/
/*-----------------------------------------------------------------*/
XBYTE IFAFORM::GetSense()
{
	return sense;
}




//-------------------------------------------------------------------
//  IFAFORM::SetSense
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/02/2004 18:11:01
//	
//	@return				bool : 
//	@param				sense : 
*/
//-------------------------------------------------------------------
bool IFAFORM::SetSense(XBYTE sense)
{
	this->sense  = sense;

	return true;
}




//-------------------------------------------------------------------
//  IFAFORM::RestoreAllRegions
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/01/2004 17:02:50
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAFORM::RestoreAllRegions()
{
	if(objs.IsEmpty()) return false;

	for(XDWORD c=0;c<objs.GetSize();c++)
		{
			IFAOBJ* tobj=(IFAOBJ*)objs.Get(c);
			
			if(tobj) tobj->RestoreRegion();					
		}

	region.Restore(canvas);

	return true;
}



//-------------------------------------------------------------------
// IFAFORM::AddObj
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:19:39
//	
//	@return        bool : 
//  @param				 obj  :
*/
//-------------------------------------------------------------------
bool IFAFORM::AddObj(IFAOBJ* obj)
{
	if(!obj) return false;

	objs.Add(obj);

	return true;
}



//-------------------------------------------------------------------
// IFAFORM::DeleteObj
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:19:39
//	
//	@return        bool : 
//  @param				 obj  :
*/
//-------------------------------------------------------------------
bool IFAFORM::DeleteObj(IFAOBJ* obj)
{
	if(!obj)						return false;
	if(objs.IsEmpty()) return false;

	for(XDWORD c=0;c<objs.GetSize();c++)
		{
			IFAOBJ* tobj=(IFAOBJ*)objs.Get(c);

			if(tobj)
				{
					if(tobj==obj)
						{							
							objs.Delete(tobj);	
					
							return true;
						}
				}
		}

	return false;
}


//-------------------------------------------------------------------
// IFAFORM::DeleteAllObj
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:19:39
//	
//	@return        bool : 
//  @param				 obj  :
*/
//-------------------------------------------------------------------
bool IFAFORM::DeleteAllObj()
{
	if(objs.IsEmpty()) return false;

	objs.DeleteAll();

	return true;
}


//-------------------------------------------------------------------
//  IFAFORM::SetCatchRegionsObj
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2004 17:45:50
//	
//	@return				bool : 
//	@param				on : 
*/
//-------------------------------------------------------------------
bool IFAFORM::SetCatchRegionsObj(bool on)
{
	if(objs.IsEmpty()) return false;
	
	for(XDWORD c=0;c<objs.GetSize();c++)	
		{
			IFAOBJ* tobj=(IFAOBJ*)objs.Get(c);
			
			if(tobj) 
				{
					//if(tobj->IsVisible()) 
					tobj->SetCatchRegion(on);
				}
		}

	return true;	
}



//-------------------------------------------------------------------
//  IFAFORM::UpdateObj
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/03/2004 16:23:49
//	
//	@return				bool : 
//	@param				nobj : 
//  @param				obj : 
*/
//-------------------------------------------------------------------
bool IFAFORM::UpdateObj(int nobj,IFAOBJ* obj)
{
	if(!obj) return false;

	obj->Update();

	return true;
}




//-------------------------------------------------------------------
//  IFAFORM::UpdateObjs
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/03/2004 17:09:31
//	
//	@return				bool : 
//	@param				optionrect : 
*/
//-------------------------------------------------------------------
bool IFAFORM::UpdateObjs(GRPRECT* objrect)
{
	if(objs.IsEmpty()) return true;

	canvas->SetViewPort(objrect);

	#ifdef XDEBUG
	//XBYTE RGB[4]= { 0xF0,0xF0,0x00 };
	//canvas->DrawRect(objrect,RGB);
	#endif

	int nobj = 0;
	
	for(XDWORD c=0;c<objs.GetSize();c++)
		{
			IFAOBJ* tobj = (IFAOBJ*)objs.Get(c);

			if(tobj)			
				{
					//if(tobj->IsVisible())
						{
							int		oldx						 = tobj->GetRect()->GetX();
							int		oldy						 = tobj->GetRect()->GetY();
							bool  isscreenrelative = tobj->IsScreenRelative();
							
							int xm = tobj->AjustPosition(oldx,true)  + AjustPosition(rectfull.GetX(),true); 
							int ym = tobj->AjustPosition(oldy,false) + AjustPosition(rectfull.GetY(),false);

							if(tobj->GetRect()->GetX()!=IFAOBJ_CENTER) xm += widthedgein;
							if(tobj->GetRect()->GetY()!=IFAOBJ_CENTER) ym += heightedgein;
						
							if(objrect->IsPartInto(xm,ym,tobj->GetRect()->GetWidth(),tobj->GetRect()->GetHeight()))
								{							
									tobj->SetOrigin(xm,ym,false);							
									UpdateObj(nobj,tobj);														
									tobj->SetOrigin(oldx,oldy,isscreenrelative);												
								}
						}
				} 
		
			nobj++;
		}

	canvas->SetViewPort(NULL);

	return true;
}




//-------------------------------------------------------------------
//  IFAFORM::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2004 16:47:54
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFAFORM::Draw(int x, int y)
{
	if(!canvas) return false;

	if(!BlinkControl()) return false;

	rectdirect.Set(x,y,rect.GetWidth(),rect.GetHeight());

	if(features&IFAFEATURE_BACKGROUND)
		{
			IFASKINOBJ* skinobj = GetSkinObj();
			if(skinobj) return skinobj->Draw(this,x,y);

			/*
			GRPCOLOR* fill = colors[GRPCOLORTYPE_BACKGROUND];
			GRPCOLOR* edge = colors[GRPCOLORTYPE_EDGE];
			
			IFASKINTYPE	type = IFASKINTYPE_DEFAULT;
			if(skinmanager) type = skinmanager->GetType();

			switch(type)
				{
					case IFASKINTYPE_DEFAULT	:	canvas->DrawBox(x,y,rect.width,rect.height,fill->RGB,fill->alpha,1,edge->RGB,edge->alpha,shadow);	
																			break;

					case IFASKINTYPE_COMIC		: canvas->DrawComicBalloon(sense,x,y,rect.width,rect.height,fill->RGB,1, edge->RGB, shadow);
																			break;

					case IFASKINTYPE_BEVEL		:	break;			
				}	
			*/
		}

	return true;
}


//-------------------------------------------------------------------
//  IFAFORM::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2003 16:36:00
//	
//	@return				bool : 
//	@param				enviroment : 
*/
//-------------------------------------------------------------------
bool IFAFORM::Update()
{	
	if(!IsVisible()) return false;

	IFAOBJ::Update();

	GRPRECT trect(rect);

	trect.SetOriginX(AjustPosition(trect.GetX() , true));
	trect.SetOriginY(AjustPosition(trect.GetY() , false));

	if(isscreenrelative)
		{
			trect.SetOriginX(trect.GetX() + canvas->GetScreenZone()->GetX());
			trect.SetOriginY(trect.GetY() + canvas->GetScreenZone()->GetY());
		}

	trect.SetOriginX(trect.GetX() + widthedgein);
	trect.SetOriginY(trect.GetY() + heightedgein);
	trect.SetWidth(rect.GetWidth()	 - (widthedgein*2));
	trect.SetHeight(rect.GetHeight()  - (heightedgein*2));

	UpdateObjs(&trect);

	return true;
}


//-------------------------------------------------------------------
//  IFAFORM::CalculateFullRect
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/03/2004 12:37:01
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAFORM::CalculateFullRect()
{
	IFAOBJ::CalculateFullRect();

	int maxwidth	= 0;
	int maxheight = 0;

	for(XDWORD c=0;c<objs.GetSize();c++)
		{
			IFAOBJ* tobj = (IFAOBJ*)objs.Get(c);

			if(tobj)
				{
					int widthpos	= (widthedgein  *2);
					int heightpos = (heightedgein *2);

					widthpos	+= tobj->AjustPosition(tobj->GetRect()->GetX(), true)  + tobj->GetRect()->GetWidth();
					heightpos += tobj->AjustPosition(tobj->GetRect()->GetY(), false) + tobj->GetRect()->GetHeight();
							
					if(widthpos  > maxwidth)   maxwidth  = widthpos;
					if(heightpos > maxheight)  maxheight = heightpos;
						
				}
		}

	rectfull.SetWidth(maxwidth+1); 
	rectfull.SetHeight(maxheight+1);	

	if(rectfull.GetWidth()  < rect.GetWidth())  rectfull.SetWidth(rect.GetWidth());
	if(rectfull.GetHeight() < rect.GetHeight()) rectfull.SetHeight(rect.GetHeight());

	GetScroll()->SetAvailable();

	return true;
}


	
//-------------------------------------------------------------------
//  IFAFORM::Clean
/**
//	
//	@author        Abraham J. Velez
//	@version       12/3/2003   17:16:16
//	
//	@return        void : 
*/
//-------------------------------------------------------------------
void IFAFORM::Clean()
{
	caption.Empty();

	widthedgein	 = 0;
	heightedgein = 0;
}
