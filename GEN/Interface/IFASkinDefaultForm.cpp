//------------------------------------------------------------------------------------------
//	IFASKINDEFAULTFORM.CPP
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

#include "IFASkinDefaultForm.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFASKINDEFAULTFORM::~IFASKINDEFAULTFORM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/11/2004 9:44:58
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFASKINDEFAULTFORM::IFASKINDEFAULTFORM()
{


}
		

//-------------------------------------------------------------------
//  IFASKINDEFAULTFORM::~IFASKINDEFAULTFORM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/11/2004 9:44:45
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFASKINDEFAULTFORM::~IFASKINDEFAULTFORM()
{

}




//-------------------------------------------------------------------
//  IFASKINDEFAULTFORM::Draw
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
bool IFASKINDEFAULTFORM::Draw(IFAOBJ* obj,int x, int y)
{
	if(!obj) return false;

	GRPCOLOR* fillcolor = obj->GetColor(GRPCOLORTYPE_BACKGROUND);
	GRPCOLOR* edgecolor = obj->GetColor(GRPCOLORTYPE_EDGE);
			
	obj->GetCanvas()->DrawBox(x,y , obj->GetRect()->GetWidth(),obj->GetRect()->GetHeight()
																, fillcolor->Get()
																, fillcolor->GetAlpha()
																, 1
																, edgecolor->Get()
																, edgecolor->GetAlpha()
																, obj->GetShadow());	

	return true;
}


