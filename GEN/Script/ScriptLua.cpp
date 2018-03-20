//------------------------------------------------------------------------------------------
//	SCRIPT.CPP
//	
//	Script Interpreter (Mini CPP interpreter)
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 17/07/2007 17:46:23
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


#include "ScriptLib.h"
#include "ScriptLua.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------
	

//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  SCRIPTLUA::SCRIPTLUA
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/09/2011 16:34:18
//	
//	@return				

//  @param				xpublisher : 
*/
/*-----------------------------------------------------------------*/
SCRIPTLUA::SCRIPTLUA()
{
	Clean();	
}



//-------------------------------------------------------------------
//  SCRIPTLUA::~SCRIPT
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			17/07/2007 18:21:48
//	
//	@return				virtual : 
//	*/
//-------------------------------------------------------------------
SCRIPTLUA::~SCRIPTLUA()
{
	Clean();
}
	



//-------------------------------------------------------------------
//  SCRIPTLUA::Load
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			17/07/2007 18:51:20
//	
//	@return				bool : 
//	@param				xpath : 
*/
//-------------------------------------------------------------------
bool SCRIPTLUA::Load(XPATH& xpath)
{
	

	return false;
}




//-------------------------------------------------------------------
//  SCRIPTLUA::Save
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/07/2007 17:25:27
//	
//	@return				bool : 
//	@param				xpath : 
*/
//-------------------------------------------------------------------
bool SCRIPTLUA::Save(XPATH& xpath)
{
	
	return false;
}






/*-------------------------------------------------------------------
//  SCRIPTLUA::Run
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/12/2009 08:46:25 a.m.
//	
//	@return				SCRIPTERRORMSG : 
//	@param				returnval : 
*/
/*-----------------------------------------------------------------*/
SCRIPTERRORMSG SCRIPTLUA::Run(int* returnval)
{
	
	errorscript  = SCRIPTERRORMSG_NONE;
	iscancelexec = false;
	
	return errorscript;
}



/*-------------------------------------------------------------------
//  SCRIPTLUA::HaveError
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/01/2010 03:51:34 p.m.
//	
//	@return				void : 
//	@param				errorscript : 
*/
/*-----------------------------------------------------------------*/
bool SCRIPTLUA::HaveError(SCRIPTERRORMSG errorcode)
{

	return (errorscript==SCRIPTERRORMSG_NONE)?false:true;
}





