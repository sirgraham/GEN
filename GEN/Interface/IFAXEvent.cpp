//------------------------------------------------------------------------------------------
//	IFAXEVENT.CPP
//	
//	XEvent for IFA
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 18/03/2004 9:53:54
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "IFAXEvent.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------




//-------------------------------------------------------------------
//  IFAXEVENTSELECT::IFAXEVENTSELECT
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2004 10:00:09
//	
//	@return				
//	@param				selectoption : 
//  @param				obj : 
//  @param				subject : 
//  @param				param : 
//  @param				type : 
*/
//-------------------------------------------------------------------
IFAXEVENTSELECT::IFAXEVENTSELECT(int selectoption,IFAOBJ* selectobj,IFAOBJ* obj,XSUBJECT* subject,int param,XDWORD type) : XEVENT(subject,type)
{
	Clean();

  this->selectoption = selectoption;
	this->selectobj		 = selectobj;
	this->obj					 = obj;
	this->param				 = param;
}



/*-------------------------------------------------------------------
//  IFAXEVENTSELECT::~IFAXEVENTSELECT
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/11/2011 17:59:09
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
IFAXEVENTSELECT::~IFAXEVENTSELECT()
{
	Clean();
}



/*-------------------------------------------------------------------
//  IFAXEVENTSELECT::GetSelectOption
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/11/2011 18:04:48
//	
//	@return				int : 
//	*/
/*-----------------------------------------------------------------*/
int IFAXEVENTSELECT::GetSelectOption()
{
	return selectoption;
}



/*-------------------------------------------------------------------
//  IFAXEVENTSELECT::GetSelectObj
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/11/2011 18:04:54
//	
//	@return				IFAOBJ* : 
//	*/
/*-----------------------------------------------------------------*/
IFAOBJ* IFAXEVENTSELECT::GetSelectObj()
{
	return selectobj;
}



/*-------------------------------------------------------------------
//  IFAXEVENTSELECT::GetObj
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/11/2011 18:04:58
//	
//	@return				IFAOBJ* : 
//	*/
/*-----------------------------------------------------------------*/
IFAOBJ* IFAXEVENTSELECT::GetObj()
{
	return obj;
}



/*-------------------------------------------------------------------
//  IFAXEVENTSELECT::GetParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/11/2011 18:05:03
//	
//	@return				int : 
//	*/
/*-----------------------------------------------------------------*/
int IFAXEVENTSELECT::GetParam()
{
	return param;
}



/*-------------------------------------------------------------------
//  IFAXEVENTSELECT::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/11/2011 18:00:39
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void IFAXEVENTSELECT::Clean()
{
	selectoption = 0;
	selectobj		 = NULL;
	obj					 = NULL;
	param				 = 0;
}