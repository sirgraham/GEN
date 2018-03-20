//------------------------------------------------------------------------------------------
//	IFASKINMANAGER.CPP
//	
//	Interface Skin Manager Class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 20/02/2004 10:58:26
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "IFAObj.h"

#include "IFASkinDefaultForm.h"
#include "IFASkinComicForm.h"
#include "IFASkinBevelForm.h"

#include "IFASkinManager.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFASKINOBJ::IFASKINOBJ
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/11/2004 10:22:14
//	
//	@return				
//	@param				*obj : 
*/
//-------------------------------------------------------------------
IFASKINOBJ::IFASKINOBJ()
{	
	Clean();
}



//-------------------------------------------------------------------
//  IFASKINOBJ::~IFASKINOBJ
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/11/2004 10:22:51
//	
//	@return				virtual : 
//	*/
//-------------------------------------------------------------------
IFASKINOBJ::~IFASKINOBJ()
{
	Clean();
}


//-------------------------------------------------------------------
//  IFASKINOBJ::GetType
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2004 11:01:04
//	
//	@return				IFASKINTYPE : 
//	*/
//-------------------------------------------------------------------
IFASKINTYPE IFASKINOBJ::GetType()
{
	return skintype;
}


//-------------------------------------------------------------------
//  IFASKINOBJ::SetType
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2004 11:01:31
//	
//	@return				bool : 
//	@param				type : 
*/
//-------------------------------------------------------------------
bool IFASKINOBJ::SetType(IFASKINTYPE type)
{
	skintype = type;

	return true;
}


//-------------------------------------------------------------------
//  IFASKINOBJ::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/11/2004 10:31:12
//	
//	@return				bool : 
//	@param				obj : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFASKINOBJ::Draw(IFAOBJ* obj,int x, int y)
{
	return true;
}


//-------------------------------------------------------------------
//  IFASKINOBJ::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/11/2004 10:23:30
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFASKINOBJ::Clean()
{
	skintype = IFASKINTYPE_DEFAULT;
}




//-------------------------------------------------------------------
//  IFASKINMANAGER::IFASKINMANAGER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2004 11:21:25
//	
//	@return				
//	@param				canvas : 
//  @param				screen : 
//  @param				inpmanager : 
*/
//-------------------------------------------------------------------
IFASKINMANAGER::IFASKINMANAGER()
{
	Clean();
}




//-------------------------------------------------------------------
//  IFASKINMANAGER::IFASKINMANAGER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2004 18:18:22
//	
//	@return				
//	@param				type : 
*/
//-------------------------------------------------------------------
IFASKINMANAGER::IFASKINMANAGER(IFASKINTYPE type)
{
	Clean();

	SetType(type);
}



//-------------------------------------------------------------------
//  IFASKINMANAGER::~IFASKINMANAGER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2004 10:59:23
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFASKINMANAGER::~IFASKINMANAGER()
{
	Clean();
}


//-------------------------------------------------------------------
//  IFASKINMANAGER::GetType
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2004 11:01:04
//	
//	@return				IFASKINTYPE : 
//	*/
//-------------------------------------------------------------------
IFASKINTYPE IFASKINMANAGER::GetType()
{
	return skintype;
}


//-------------------------------------------------------------------
//  IFASKINMANAGER::SetType
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2004 11:01:31
//	
//	@return				bool : 
//	@param				type : 
*/
//-------------------------------------------------------------------
bool IFASKINMANAGER::SetType(IFASKINTYPE type)
{
	skintype = type;

	return true;
}


//-------------------------------------------------------------------
//  IFASKINMANAGER::GetSkinObj
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/11/2004 10:24:59
//	
//	@return				IFASKINOBJ* : 
//	@param				*obj : 
*/
//-------------------------------------------------------------------
IFASKINOBJ* IFASKINMANAGER::GetSkinObj(IFAOBJ *obj)
{
	IFASKINOBJ* skinobj = NULL;

	switch(skintype)
		{
			case IFASKINTYPE_DEFAULT	: switch(obj->GetType())
																		{
																			case IFAOBJTYPE_UNKNOW			 : break;

																			case IFAOBJTYPE_SCROLL			 : break;

																			case IFAOBJTYPE_FORM				 : skinobj = new IFASKINDEFAULTFORM(); 
																																		 break;

																			case IFAOBJTYPE_TEXT				 : break;

																			case IFAOBJTYPE_TEXTINBOX		 : break;

																			case IFAOBJTYPE_BITMAP			 : break;

																			case IFAOBJTYPE_BUTTON			 : break;

																			case IFAOBJTYPE_PROGRESSBAR	 : break;

																			case IFAOBJTYPE_MENU				 : skinobj = new IFASKINDEFAULTFORM(); 
																																		 break;

																			case IFAOBJTYPE_MENUPULLDOWN : break;

																		}
																	break;

			case IFASKINTYPE_COMIC		: switch(obj->GetType())
																		{
																			case IFAOBJTYPE_UNKNOW			 : break;
																			
																			case IFAOBJTYPE_SCROLL			 : break;

																			case IFAOBJTYPE_FORM				 : skinobj = new IFASKINCOMICFORM();  
																																		 break;

																			case IFAOBJTYPE_TEXT				 : break;

																			case IFAOBJTYPE_TEXTINBOX		 : break;

																			case IFAOBJTYPE_BITMAP			 : break;
																			
																			case IFAOBJTYPE_BUTTON			 : break;

																			case IFAOBJTYPE_PROGRESSBAR	 : break;

																			case IFAOBJTYPE_MENU				 : skinobj = new IFASKINCOMICFORM();  
																																		 break;

																			case IFAOBJTYPE_MENUPULLDOWN : break;
																		}
																	break;

			case IFASKINTYPE_BEVEL		: switch(obj->GetType())
																		{
																			case IFAOBJTYPE_UNKNOW			 : break;

																			case IFAOBJTYPE_SCROLL			 : break;

																			case IFAOBJTYPE_FORM				 : skinobj = new IFASKINBEVELFORM();  
																																		 break;

																			case IFAOBJTYPE_TEXT				 : break;

																			case IFAOBJTYPE_TEXTINBOX		 : break;

																			case IFAOBJTYPE_BITMAP			 : break;

																			case IFAOBJTYPE_BUTTON			 : break;

																			case IFAOBJTYPE_PROGRESSBAR	 : break;

																			case IFAOBJTYPE_MENU				 : skinobj = new IFASKINBEVELFORM();  
																																		 break;
																																		 
																			case IFAOBJTYPE_MENUPULLDOWN : break;
																		}
																	break;
		}

	if(skinobj) skinobj->SetType(skintype);

  return skinobj;
}



//-------------------------------------------------------------------
//  IFASKINMANAGER::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/02/2004 10:59:53
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFASKINMANAGER::Clean()
{
	skintype		= IFASKINTYPE_DEFAULT;
}

