//------------------------------------------------------------------------------------------
//	IFASKINBEVELFORM.CPP
//	
//	Interface Skin Default Form CLASS
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 09/11/2004 9:38:12
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPColor.h"
#include "GRPCanvas.h"

#include "IFAObj.h"

#include "IFASkinBevelForm.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFASKINBEVELFORM::~IFASKINBEVELFORM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/11/2004 9:44:58
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFASKINBEVELFORM::IFASKINBEVELFORM()
{


}
		

//-------------------------------------------------------------------
//  IFASKINBEVELFORM::~IFASKINBEVELFORM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/11/2004 9:44:45
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFASKINBEVELFORM::~IFASKINBEVELFORM()
{

}




//-------------------------------------------------------------------
//  IFASKINBEVELFORM::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/11/2004 9:45:41
//	
//	@return				bool : 
//	@param				obj : 
//  @param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFASKINBEVELFORM::Draw(IFAOBJ* obj,int x, int y)
{
	if(!obj) return false;

	GRPCOLOR* fill = obj->GetColor(GRPCOLORTYPE_BACKGROUND);

	GRPCOLOR color;
	color.Set(obj->GetColor(GRPCOLORTYPE_BACKGROUND));	

	GRPCOLOR color2;
		
	XBYTE cbright1[] = { 10, 30, 50 ,30, 20};	
	int  sbright1   = (sizeof(cbright1)/sizeof(XBYTE));
  int  c;

	for(c=0;c<sbright1;c++)
		{			
			color2.Set(obj->GetCanvas()->COLOR(WHITE));
			color2.Alpha(&color,cbright1[c]);					
			color2.ConvertToMode(obj->GetCanvas()->GetMode());

			obj->GetCanvas()->DrawLine(x,(y+obj->GetRect()->GetHeight()-c),(x+obj->GetRect()->GetWidth()) ,(y+obj->GetRect()->GetHeight()-c),color2.Get(),100);
		}
	
	XBYTE cbright2[] = {	20, 15, 10 ,5};	
	int  sbright2   = (sizeof(cbright2)/sizeof(XBYTE));

	for(c=0;c<sbright2;c++)
		{
			color2.Set(obj->GetCanvas()->COLOR(BLACK));
			color2.Alpha(&color,cbright2[c]);
			color2.ConvertToMode(obj->GetCanvas()->GetMode());

			obj->GetCanvas()->DrawLine(x,y+c,(x+obj->GetRect()->GetWidth()), y+c ,color2.Get(),100);
		}

	obj->GetCanvas()->DrawBox(x,y+sbright2,obj->GetRect()->GetWidth(),(obj->GetRect()->GetHeight()-(sbright1+sbright2))+1,fill->Get(),fill->GetAlpha(),1,fill->Get(),fill->GetAlpha(),false);

	for(c=0;c<1;c++)
		{
			obj->GetCanvas()->DrawLine(x+c ,y ,(x+c) , (y+obj->GetRect()->GetHeight()) ,obj->GetCanvas()->COLOR(WHITE)->Get(),100);
			obj->GetCanvas()->DrawLine(x+c ,y ,(x+c) , (y+obj->GetRect()->GetHeight()) ,fill->Get(),60);

			obj->GetCanvas()->DrawLine((x+obj->GetRect()->GetWidth()-1-c) ,y ,(x+obj->GetRect()->GetWidth()-1-c) , (y+obj->GetRect()->GetHeight()) ,obj->GetCanvas()->COLOR(BLACK)->Get(),100);
			obj->GetCanvas()->DrawLine((x+obj->GetRect()->GetWidth()-1-c) ,y ,(x+obj->GetRect()->GetWidth()-1-c) , (y+obj->GetRect()->GetHeight()) ,fill->Get(),60);
		}

	return true;
}


