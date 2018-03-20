//------------------------------------------------------------------------------------------
//	SCRIPTLIB.CPP
//	
//	Script library class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 19/07/2007 20:10:28
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "Script.h"
#include "ScriptLib.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  SCRIPTLIB::SCRIPTLIB
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/09/2011 19:14:53
//	
//	@return				
//	@param				ID : 
*/
/*-----------------------------------------------------------------*/
SCRIPTLIB::SCRIPTLIB(XCHAR* ID) 
{
	Clean();

	this->ID = ID;
}



//-------------------------------------------------------------------
//  SCRIPTLIB::~SCRIPTLIB
/**
//
//
//	@author				Abraham J. Velez
//	@version			19/07/2007 20:45:40
//
//	@return
//	*/
//-------------------------------------------------------------------
SCRIPTLIB::~SCRIPTLIB()
{
	Clean();
}



/*-------------------------------------------------------------------
//  SCRIPTLIB::GetID
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/09/2011 19:13:59
//	
//	@return				XSTRING* : 
//	*/
/*-----------------------------------------------------------------*/
XSTRING* SCRIPTLIB::GetID()
{
	return &ID;
}



/*-------------------------------------------------------------------
//  SCRIPTLIB::AddLibraryFunctions
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/09/2011 18:59:16
//	
//	@return				bool : 
//	@param				script : 
*/
/*-----------------------------------------------------------------*/
bool SCRIPTLIB::AddLibraryFunctions(SCRIPT* script)
{
	return false;
}




/*-------------------------------------------------------------------
//  SCRIPTLIB::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/11/2009 01:04:19 a.m.
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void SCRIPTLIB::Clean()
{
	script = NULL;
}
