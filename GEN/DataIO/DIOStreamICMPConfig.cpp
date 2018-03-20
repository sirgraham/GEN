//------------------------------------------------------------------------------------------
//	DIOSTREAMICMPCONFIG.CPP
//	
//	Data IO Stream ICMP Config class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOFactory.h"
#include "DIOStreamICMPConfig.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//	DIOSTREAMICMPCONFIG::DIOSTREAMICMPCONFIG
*/	
/**	
//	
//	Class Constructor DIOSTREAMICMPCONFIG
//	
//	@author				Abraham J. Velez
//	@version			29/05/2014 17:29:35
//	
 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMICMPCONFIG::DIOSTREAMICMPCONFIG() : DIOSTREAMCONFIG()
{
	Clean();

	type = DIOSTREAMTYPE_ICMP;

	if(diofactory) remoteURL = diofactory->CreateURL();

}


//-------------------------------------------------------------------
//  DIOSTREAMICMPCONFIG::~DIOSTREAMICMPCONFIG
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:15:01
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMICMPCONFIG::~DIOSTREAMICMPCONFIG()
{
	if(diofactory && remoteURL) diofactory->DeleteURL(remoteURL);

	Clean();	
}




/*-------------------------------------------------------------------
//  DIOSTREAMICMPCONFIG ::GetLocalIP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2013 10:16:04
//	
//	@return 			DIOIP* : 
//	*/
/*-----------------------------------------------------------------*/
DIOIP* DIOSTREAMICMPCONFIG ::GetLocalIP()
{
	return &localIP;
}



/*-------------------------------------------------------------------
//  DIOSTREAMICMPCONFIG ::GetRemoteURL
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2013 10:21:54
//	
//	@return 			DIOURL* : 
//	*/
/*-----------------------------------------------------------------*/
DIOURL* DIOSTREAMICMPCONFIG ::GetRemoteURL()
{
	return remoteURL;
}





//-------------------------------------------------------------------
//  DIOSTREAMICMPCONFIG ::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:15:22
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
void DIOSTREAMICMPCONFIG ::Clean()
{
	type 										= DIOSTREAMTYPE_UDP;		
	remoteURL								= NULL;	
}

