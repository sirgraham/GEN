//------------------------------------------------------------------------------------------
//	IFASKINCOMICFORM.CPP
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
#include "IFAForm.h"

#include "IFASkinComicForm.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFASKINCOMICFORM::~IFASKINCOMICFORM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/11/2004 9:44:58
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFASKINCOMICFORM::IFASKINCOMICFORM()
{


}
		

//-------------------------------------------------------------------
//  IFASKINCOMICFORM::~IFASKINCOMICFORM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/11/2004 9:44:45
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFASKINCOMICFORM::~IFASKINCOMICFORM()
{

}




//-------------------------------------------------------------------
//  IFASKINCOMICFORM::Draw
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
bool IFASKINCOMICFORM::Draw(IFAOBJ* obj,int x, int y)
{
	if(!obj) return false;

	IFAFORM* formobj = (IFAFORM*)obj;

	GRPCOLOR* fill = obj->GetColor(GRPCOLORTYPE_BACKGROUND);
	GRPCOLOR* edge = obj->GetColor(GRPCOLORTYPE_EDGE);

	formobj->GetCanvas()->DrawComicBalloon(formobj->GetSense(),x,y,obj->GetRect()->GetWidth(),obj->GetRect()->GetHeight(),fill->Get(),1, edge->Get(), formobj->GetShadow());
			
	return true;
}


