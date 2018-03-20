
/*------------------------------------------------------------------------------------------
//	GRPCONTEXT.CPP
//	
//	Graphics Context
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 04/04/2016 14:24:17
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPContext.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	GRPCONTEXT::GRPCONTEXT
*/	
/**	
//	
//	Class Constructor GRPCONTEXT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:28:14
//	
*/
/*-----------------------------------------------------------------*/
GRPCONTEXT::GRPCONTEXT()
{
	Clean();

	this->mustrebuild = true;
}





/*-------------------------------------------------------------------
//	GRPCONTEXT::~GRPCONTEXT
*/	
/**	
//	
//	 Class Destructor GRPCONTEXT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:28:20
//	
*/
/*-----------------------------------------------------------------*/
GRPCONTEXT::~GRPCONTEXT()
{
	Clean();
}






/*-------------------------------------------------------------------
//	GRPCONTEXT::SetScreen
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:28:28
//	
//  @param				screen : 
*/
/*-----------------------------------------------------------------*/
void GRPCONTEXT::SetScreen(GRPSCREEN* screen)
{
	this->screen = screen;
}





/*-------------------------------------------------------------------
//	GRPCONTEXT::GetScreen
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:28:36
//	
//	@return 			GRPSCREEN* : 
//
*/
/*-----------------------------------------------------------------*/
GRPSCREEN* GRPCONTEXT::GetScreen()
{
	return this->screen;
}





/*-------------------------------------------------------------------
//	GRPCONTEXT::Create
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:28:43
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONTEXT::Create(void* handle)
{
	return true;
}





/*-------------------------------------------------------------------
//	GRPCONTEXT::Destroy
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:28:49
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONTEXT::Destroy()
{
	return true;
}





/*-------------------------------------------------------------------
//	GRPCONTEXT::IsLost
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:28:56
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONTEXT::IsLost()
{
	return false;
}





/*-------------------------------------------------------------------
//	GRPCONTEXT::Update
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/04/2016 13:53:19
//	
*/
/*-----------------------------------------------------------------*/
bool GRPCONTEXT::Update()
{
	return true;
}



/*-------------------------------------------------------------------
//	GRPCONTEXT::SetMustRebuild
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:29:03
//	
//  @param				mustrebuild : 
*/
/*-----------------------------------------------------------------*/
void GRPCONTEXT::SetMustRebuild(bool mustrebuild)
{
	this->mustrebuild = mustrebuild;
}





/*-------------------------------------------------------------------
//	GRPCONTEXT::IsMustRebuild
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 14:29:09
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONTEXT::IsMustRebuild()
{
	return this->mustrebuild;
}





/*-------------------------------------------------------------------
//	GRPCONTEXT::SetMultiSampling
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 13:17:22
//	
//  @param				multisampling : 
*/
/*-----------------------------------------------------------------*/
void GRPCONTEXT::SetMultiSampling(XDWORD multisampling)
{
	this->multisampling = multisampling;
}






/*-------------------------------------------------------------------
//	GRPCONTEXT::GetMultisampling
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 13:17:30
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPCONTEXT::GetMultisampling()
{
	return this->multisampling;
}


