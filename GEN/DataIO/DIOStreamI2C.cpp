//------------------------------------------------------------------------------------------
//	DIOSTREAMI2C.CPP
//	
//	Data IO Stream I2C class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOGPIO.h"
#include "DIOStreamXEvent.h"

#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	


/*-------------------------------------------------------------------
//  DIOSTREAMI2C::DIOSTREAMI2C
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:51:23
//	
//	@return 			
*/
/*-----------------------------------------------------------------*/
DIOSTREAMI2C::DIOSTREAMI2C() : DIOSTREAM() , XFSMACHINE(0)
{
	Clean();

	AddState(	DIOSTREAMI2C_FSMSTATE_NONE							, 												 
						DIOSTREAMI2C_FSMEVENT_CONNECTED				,	DIOSTREAMI2C_FSMSTATE_CONNECTED			 	 ,						
						DIOSTREAMI2C_FSMEVENT_DISCONNECTING		,	DIOSTREAMI2C_FSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);


	AddState(	DIOSTREAMI2C_FSMSTATE_CONNECTED				, 												 							
						DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD		,	DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD		 ,
						DIOSTREAMI2C_FSMEVENT_SENDINGDATA			,	DIOSTREAMI2C_FSMSTATE_SENDINGDATA			 ,					
						DIOSTREAMI2C_FSMEVENT_DISCONNECTING		,	DIOSTREAMI2C_FSMSTATE_DISCONNECTING		 ,
						EVENTDEFEND);	

	AddState(	DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD		, 												 								
						DIOSTREAMI2C_FSMEVENT_CONNECTED				, DIOSTREAMI2C_FSMSTATE_CONNECTED			 	 ,												
						DIOSTREAMI2C_FSMEVENT_SENDINGDATA			,	DIOSTREAMI2C_FSMSTATE_SENDINGDATA			 ,		
						DIOSTREAMI2C_FSMEVENT_DISCONNECTING		,	DIOSTREAMI2C_FSMSTATE_DISCONNECTING		 ,									
						EVENTDEFEND);	

	AddState(	DIOSTREAMI2C_FSMSTATE_DISCONNECTING		, 												 								
						DIOSTREAMI2C_FSMEVENT_CONNECTED				,	DIOSTREAMI2C_FSMSTATE_CONNECTED			 	 ,						
						DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD		,	DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD		 ,
						DIOSTREAMI2C_FSMEVENT_SENDINGDATA			,	DIOSTREAMI2C_FSMSTATE_SENDINGDATA			 ,											
						EVENTDEFEND);	
}



//-------------------------------------------------------------------
//  DIOSTREAMI2C::~DIOSTREAMI2C
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:19:33
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMI2C::~DIOSTREAMI2C()
{ 
	Close();
  
	Clean();
}



//-------------------------------------------------------------------
//  DIOSTREAMI2C::GetConfig
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/09/2004 14:33:56
//	
//	@return				DIOSTREAMCONFIG* : 
//	*/
//-------------------------------------------------------------------
DIOSTREAMCONFIG* DIOSTREAMI2C::GetConfig()
{
	return (DIOSTREAMCONFIG*)config;
}



//-------------------------------------------------------------------
//  DIOSTREAMI2C::SetConfig
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
//  @param				config :
*/
//-------------------------------------------------------------------
bool DIOSTREAMI2C::SetConfig(DIOSTREAMCONFIG* config)
{	
	if(!config) return false;
	this->config = (DIOSTREAMI2CCONFIG*)config;
	
	return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMI2C::Open
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/10/2012 10:03:42
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMI2C::Open()
{
	if(!config) return false;
	
	if(!threadconnexion)	return false;	
  
	ResetXBuffers();
	ResetConnexionStatistics();
	
	return threadconnexion->Ini();	
}




/*-------------------------------------------------------------------
//  DIOSTREAMI2C::Close
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/10/2012 10:03:47
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMI2C::Close()
{	
	if(!threadconnexion) return false;	

	threadconnexion->End();
  
	return true;
}




//-------------------------------------------------------------------
//  DIOSTREAMI2C::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:19:50
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void DIOSTREAMI2C::Clean()
{
	config					= NULL;

	threadconnexion = NULL;
}



