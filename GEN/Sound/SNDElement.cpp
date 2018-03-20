//------------------------------------------------------------------------------------------
//	SNDELEMENT.CPP
//	
//	Sound Element
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 29/04/2004 15:29:49
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include <string.h>

#include "SNDElement.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  SNDELEMENT::SNDELEMENT
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/04/2004 15:31:15
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
SNDELEMENT::SNDELEMENT()
{
	Clean();

	volume	= 1.0f;
	pitch		= 1.0f;
}


//-------------------------------------------------------------------
//  SNDELEMENT::~SNDELEMENT
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/04/2004 15:31:45
//	
//	@return				
//	*/
//-------------------------------------------------------------------
SNDELEMENT::~SNDELEMENT()
{	
	Clean();
}





/*-------------------------------------------------------------------
//	SNDELEMENT::GetNameFile
*/	
/**	
//	
//	gets the name of the file this element plays
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/11/2015 10:02:54
//	
//	@return 			XSTRING* : 
//
*/
/*-----------------------------------------------------------------*/
XSTRING* SNDELEMENT::GetNameFile()
{
	return &namefile;
}


