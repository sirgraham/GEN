/*------------------------------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER.CPP
//	
//	Data I/O Protocol Connexions Manager
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/01/2012 12:04:07
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XDebug.h"
#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamUDPLocalEnumServers.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOProtocolConnexionsInUseDefinitions.h"

#include "DIOProtocolConnexionsManager.h"

#include "XMemory.h"

	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXION::DIOPROTOCOLCONNEXION
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2012 17:30:52
//	
//	@return				 
//  @param				xpublisher : 
//  @param				diostreamcfg : 
//  @param				password : 
//  @param				xcipher : 
//  @param				authenticatekey : 
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOLCONNEXION::DIOPROTOCOLCONNEXION(DIOSTREAMCONFIG* diostreamcfg)
{
	Clean();
	
	this->diostreamcfg	 = diostreamcfg;		

	xmutexinuseID = xfactory->Create_Mutex();

	inuseID.SetIsMulti(true);
}




/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXION::~DIOPROTOCOLCONNEXION
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/01/2012 12:27:54
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOPROTOCOLCONNEXION::~DIOPROTOCOLCONNEXION()
{	
	xfactory->Delete_Mutex(xmutexinuseID);

	Clean();
}



/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXION::Connect
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2012 17:47:22
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXION::Connect()
{
	if(!dioprotocol) return false;

	return dioprotocol->Connect();		
}



/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXION::Disconected
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2012 18:29:20
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXION::Disconected()
{
	if(!dioprotocol)	return false;	
	
	if(dioprotocol->IsConnected())  
		{
			dioprotocol->Disconnect();	
		}
	
	return true;
}







/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER::DIOPROTOCOLCONNEXIONSMANAGER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2012 13:38:18
//	
//	@return				 

 
//  @param				publisher : 
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOLCONNEXIONSMANAGER::DIOPROTOCOLCONNEXIONSMANAGER()
{
	Clean();

	protocolconnexionsnlimit	= DIOPROTOCOLCONNEXIONS_UNLIMITEDNCONNEXIONS;
	
	RegisterEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_CONNECTEDCONNEXION);	
	RegisterEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_INITPROTOCOL);	
	RegisterEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION);
}


/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER::~DIOPROTOCOLCONNEXIONSMANAGER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/01/2012 12:37:22
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOPROTOCOLCONNEXIONSMANAGER::~DIOPROTOCOLCONNEXIONSMANAGER()
{
	DeRegisterEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_CONNECTEDCONNEXION);
	DeRegisterEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_INITPROTOCOL);
	DeRegisterEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION);
	

	Clean();
}



/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER::Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/02/2012 12:33:20
//	
//	@return				bool : 
//	@param				isserver : 
//  @param				diostreamcfg : 
//  @param				password : 
//  @param				cipher : 
//  @param				authenticatekey : 
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::Ini(bool isserver, DIOSTREAMCONFIG* diostreamcfg, DIOSTREAMENUMSERVERS* diostreamenumservers)
{
	if(!diostreamcfg) return false;

	this->isserver							= isserver;
	this->diostreamcfg					= diostreamcfg;	
	this->diostreamenumservers  = diostreamenumservers;

	this->diostreamcfg->SetMode(isserver?DIOSTREAMMODE_SERVER:DIOSTREAMMODE_CLIENT);	
	
	xtimerout = xfactory->CreateTimer();
	if(!xtimerout)  return false;	
	
	xmutexprocotolconnexions = xfactory->Create_Mutex();
	if(!xmutexprocotolconnexions) return false;
	
	xtimerconnexions = xfactory->CreateTimer();
	if(xtimerconnexions)
		{
			xthreadconnexions = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOLCONNEXIONMANAGER, __L("DIOPROTOCOLCONNEXIONSMANAGER::Ini"), ThreadProtocolConnexions, (void*)this);
			if(xthreadconnexions)
				{			
					if(!xthreadconnexions->Ini()) return false;
				}
		}

	xtimerclienttry = xfactory->CreateTimer();
	if(!xtimerclienttry) return false;

	xtimerclienttry->AddSeconds(DIOPROTOCOLCONNEXIONS_DEFAULTIMETRYCONNEXIONS+1);

	return true;
}



/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2012 18:21:35
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::End()
{	  
	WaitToAllConnexionsCanBeDeleted();	

	if(xthreadconnexions)     
		{		
			xthreadconnexions->End();			
			DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOLCONNEXIONMANAGER, xthreadconnexions);
			xthreadconnexions = NULL;
		}

	ProtocolConnexions_DeleteAllWaitConnections();

	ProtocolConnexions_DisconnectAll();

	ProtocolConnexions_DeleteAll();

	if(xtimerclienttry)
		{
			xfactory->DeleteTimer(xtimerclienttry);
			xtimerclienttry = NULL;
		}
		
	if(xtimerconnexions) 
		{
			xfactory->DeleteTimer(xtimerconnexions);
			xtimerconnexions = NULL;
		}
	
	if(xtimerout) 
		{
			xfactory->DeleteTimer(xtimerout);
			xtimerout = NULL;
		}
	
	if(xmutexprocotolconnexions) 
		{
			xfactory->Delete_Mutex(xmutexprocotolconnexions);
			xmutexprocotolconnexions = NULL;
		}
	
	TargetURL_DeleteAll();

	return true;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER::AddURLClientTarget
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:20:09
//	
//	@return				bool : 
//	@param				element : 
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::TargetURL_Add(XCHAR* URL)
{
	if(!URL)		return false;
	
	DIOURL* _URL = new DIOURL();
	if(!_URL) return false;

	(*_URL) = URL;

	targetURLs.Add(_URL);

	return true;
}




/*-------------------------------------------------------------------
//	 DIOPROTOCOLCONNEXIONSMANAGER::TargetURL_Get
*/
/**
//
//	
//
//	@author		Abraham J. Velez 
//	@version		
//
//	@return		XSTRING* : 
//
//	@param		int : 
//
*//*-----------------------------------------------------------------*/
XSTRING* DIOPROTOCOLCONNEXIONSMANAGER::TargetURL_Get(int index)
{
	return (XSTRING*)targetURLs.Get(index);
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::TargetURL_Delete
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 12:30:27
//	
//	@return 			bool : 
//
//  @param				index : 
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::TargetURL_Delete(int index)
{
	DIOURL* URL = (DIOURL*)targetURLs.Get(index);
	if(!URL) return false;

	targetURLs.Delete(URL);

	delete URL;

	return true;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER::DeleteAllURLClientTargets
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:20:52
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::TargetURL_DeleteAll()
{
	if(targetURLs.IsEmpty())  return false;

	targetURLs.DeleteContents();	
  targetURLs.DeleteAll();

	return true;
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetNAvailable
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/09/2015 11:20:29
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetNAvailable()
{	
	return protocolconnexions.GetSize();
}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_Get
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 12:19:23
//	
//	@return 			DIOPROTOCOLCONNEXION* : 
//
//  @param				index : 
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOLCONNEXION* DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_Get(int index)
{	
	if(!xmutexprocotolconnexions)  return NULL;

	xmutexprocotolconnexions->Lock();
	
	DIOPROTOCOLCONNEXION* protocolconnexion = NULL;
	
	if(!protocolconnexions.IsEmpty())	
		{
			if(index<(int)ProtocolConnexions_GetNAvailable())	 
				{
					protocolconnexion = protocolconnexions.Get(index);
				}
		}	
		
	xmutexprocotolconnexions->UnLock();
	
	return protocolconnexion;
}





/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetFirstConnected
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2016 11:43:33
//	
//	@return 			DIOPROTOCOLCONNEXION* : 
//
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOLCONNEXION* DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetFirstConnected()
{
	if(!xmutexprocotolconnexions)  return NULL;

	xmutexprocotolconnexions->Lock();
	
	DIOPROTOCOLCONNEXION* protocolconnexion = NULL;
	if(!protocolconnexions.IsEmpty())	
		{	
			for(int c=0; c<(int)ProtocolConnexions_GetNAvailable(); c++)
				{
					protocolconnexion = protocolconnexions.Get(c);
					if(protocolconnexion)					
						{
							DIOPROTOCOL* protocol = protocolconnexion->GetDIOProtocol();
							if(protocol)
								{
									if(protocol->IsConnected()) 
										{
											protocolconnexion = protocolconnexions.Get(c);
											break;

										} else protocolconnexion = NULL;																				
							
								} else protocolconnexion = NULL;																				
						} 
				}
		}	
		
	xmutexprocotolconnexions->UnLock();
	
	return protocolconnexion;
}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetFirstOperativeProtocol
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2016 11:01:58
//	
//	@return 			DIOPROTOCOL* : 
//
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOLCONNEXION* DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetFirstOperative()
{
	if(!xmutexprocotolconnexions)  return NULL;

	xmutexprocotolconnexions->Lock();
	
	DIOPROTOCOLCONNEXION* protocolconnexion = NULL;
	if(!protocolconnexions.IsEmpty())	
		{	
			for(int c=0; c<(int)ProtocolConnexions_GetNAvailable(); c++)
				{
					protocolconnexion = protocolconnexions.Get(c);
					if(protocolconnexion)					
						{
							DIOPROTOCOL* protocol = protocolconnexion->GetDIOProtocol();
							if(protocol)
								{
									if(protocol->IsOperational()) 
										{
											protocolconnexion = protocolconnexions.Get(c);
											break;

										} else protocolconnexion = NULL;																				
							
								} else protocolconnexion = NULL;																				
						} 
				}
		}	
		
	xmutexprocotolconnexions->UnLock();
	
	return protocolconnexion;
}


/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetByDIOStream
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 12:22:52
//	
//	@return 			DIOPROTOCOLCONNEXION* : 
//
//  @param				diostream : 
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOLCONNEXION* DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetByDIOStream(DIOSTREAM* diostream)
{
	DIOPROTOCOLCONNEXION* protocolconnexion = NULL;

	if(!xmutexprocotolconnexions)  return NULL;

	xmutexprocotolconnexions->Lock();

	if(!protocolconnexions.IsEmpty()) 
		{			
			for(int index=0; index< ProtocolConnexions_GetNAvailable(); index++)
				{
					DIOPROTOCOLCONNEXION* _protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
					if(_protocolconnexion)
						{
							if(_protocolconnexion->GetDIOProtocol())
								{									
									if(_protocolconnexion->GetDIOProtocol()->GetDIOStream() == diostream) 
										{
											protocolconnexion = _protocolconnexion;
											break;										
										}
								}
						}
				}
		}

	xmutexprocotolconnexions->UnLock();

	return protocolconnexion;
}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetProtocol
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 12:31:20
//	
//	@return 			DIOPROTOCOL* : 
//
//  @param				index : 
*/
/*-----------------------------------------------------------------*/
DIOPROTOCOL* DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetProtocol(int index)
{
	DIOPROTOCOL* protocol = NULL;

	if(!xmutexprocotolconnexions)  return NULL;

	xmutexprocotolconnexions->Lock();

	if(index<(int)ProtocolConnexions_GetNAvailable())	
		{
			DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
			if(protocolconnexion)  
				{
					if(protocolconnexion->GetDIOProtocol())
						{
							if(protocolconnexion->GetDIOProtocol()->IsConnected()) 
								{
									protocol = protocolconnexion->GetDIOProtocol();
								}
						}
				}
		}

	xmutexprocotolconnexions->UnLock();

	return protocol;
}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetConnected
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			07/09/2015 10:58:51
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetNConnected()
{
	if(!xmutexprocotolconnexions)  return false;

	xmutexprocotolconnexions->Lock();

	int nconnexions			 = 0; 
	int ntotalconnexions = ProtocolConnexions_GetNAvailable();
	
	for(int index=0; index < ntotalconnexions; index++)
		{
			DIOPROTOCOLCONNEXION* connexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);	
			if(connexion)
				{
					if(connexion->GetDIOProtocol())
						{
							if(connexion->GetDIOProtocol()->IsConnected()) nconnexions++;																	
						} 
				} 
		}

	xmutexprocotolconnexions->UnLock();

	return nconnexions;
}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetNFreeToConnect
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			07/09/2015 10:58:55
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_GetNFreeToConnect()
{
	if(!xmutexprocotolconnexions)  return 0;

	xmutexprocotolconnexions->Lock();

	int nconnexions			 = 0;
	int ntotalconnexions = ProtocolConnexions_GetNAvailable();
	
	for(int index=0; index < ntotalconnexions; index++)
		{
			DIOPROTOCOLCONNEXION* connexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
			if(connexion)
				{
					if(connexion->GetDIOProtocol())
						{
							if(connexion->GetDIOProtocol()->IsConnecting()) nconnexions++;															
						} 
				} 
		}

	xmutexprocotolconnexions->UnLock();

	return nconnexions;
}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_SendEventConnected
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2016 22:29:27
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_SendEventConnected()
{		
	if(!xmutexprocotolconnexions)  return false;

	xmutexprocotolconnexions->Lock();

	int ntotalconnexions = ProtocolConnexions_GetNAvailable();
	
	for(int index = 0; index < ntotalconnexions; index++)
		{
			DIOPROTOCOLCONNEXION* connexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
			if(connexion)
				{
					if(!connexion->IsSendEventConnected())
						{
							if(connexion->GetDIOProtocol())
								{
									if(connexion->GetDIOProtocol()->IsConnected()) 
										{											
											bool sendeventconnected = SendEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_CONNECTEDCONNEXION, connexion);	

											connexion->SetIsSendEventConnected(sendeventconnected);											
										} 
								}
						} 
				}
		}

	xmutexprocotolconnexions->UnLock();

	return true;
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_DeleteAllWaitConnections
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/06/2016 21:17:44
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_DeleteAllWaitConnections()
{
	int index = 0;
	
	if(!xmutexprocotolconnexions)  return false;

	xmutexprocotolconnexions->Lock();

	while(index < ProtocolConnexions_GetNAvailable())
		{			
			DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
			if(protocolconnexion)
				{					
					if(protocolconnexion->GetDIOProtocol())
						{
							if(protocolconnexion->GetDIOProtocol()->GetDIOStream())
								{
									if(protocolconnexion->GetDIOProtocol()->GetDIOStream()->GetConnectStatus() == DIOSTREAMSTATUS_GETTINGCONNEXION)
										{	
											DeleteProtocol(protocolconnexion->GetDIOProtocol());	
											protocolconnexion->SetDIOProtocol(NULL);

											protocolconnexions.Delete(protocolconnexion);					
											delete protocolconnexion;																										
						
										} else index++;	 					

								} else index++;	 					

						} else index++;	 					
				} 
		}
	
	xmutexprocotolconnexions->UnLock();

	return true;
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_DeleteDisconnected
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			07/09/2015 10:59:00
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_DeleteDisconnected()
{
	int index = 0;
	
	if(!xmutexprocotolconnexions)  return false;

	xmutexprocotolconnexions->Lock();

	while(index < ProtocolConnexions_GetNAvailable())
		{			
			DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
			if(protocolconnexion)
				{					
					if(protocolconnexion->GetDIOProtocol())
						{
							if((protocolconnexion->GetDIOProtocol()->IsDisconnected())  && (!protocolconnexion->IsInUse()))
								{	
									SendEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION, protocolconnexion);											
									protocolconnexion->Disconected();	

									DeleteProtocol(protocolconnexion->GetDIOProtocol());	
									protocolconnexion->SetDIOProtocol(NULL);

									protocolconnexions.Delete(protocolconnexion);					
									delete protocolconnexion;																										
						
								} else index++;	 					

						} else index++;	 					
				} 
		}
	
	xmutexprocotolconnexions->UnLock();

	return true;
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_Disconnect
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/02/2016 11:27:13
//	
//	@return 			bool : 
//
//  @param				index : 
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_Disconnect(int index)
{	
	bool status = false;

	if(!xmutexprocotolconnexions)  return false;
	
	xmutexprocotolconnexions->Lock();

	if(index<(int)ProtocolConnexions_GetNAvailable())	
		{
			DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
			if(protocolconnexion)
				{					
					if(protocolconnexion->GetDIOProtocol())
						{		
							if(protocolconnexion->GetDIOProtocol()->IsConnected())						  
								{
									SendEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION, protocolconnexion);							
									protocolconnexion->Disconected();						
														
									status = true;
								}
						} 	
				}		
		}

	xmutexprocotolconnexions->UnLock();

	return status;
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_DisconnectAll
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			17/06/2016 16:05:44
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_DisconnectAll()
{		
	if(!xmutexprocotolconnexions)  return false;

	xmutexprocotolconnexions->Lock();

	if(!protocolconnexions.IsEmpty()) 
		{			
			int ntotalconnexions = ProtocolConnexions_GetNAvailable();

			for(int index=0; index < ntotalconnexions; index++)
				{									
					DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);			
					if(protocolconnexion) 
						{
							if(protocolconnexion->GetDIOProtocol())
								{
									if(protocolconnexion->GetDIOProtocol()->IsConnected())
										{																				
											SendEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_DISCONNECTEDCONNEXION, protocolconnexion);											
											protocolconnexion->Disconected();																
										}
								}
						}
				}
		}

	xmutexprocotolconnexions->UnLock();

  return true;
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_Delete
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/02/2016 12:17:20
//	
//	@return 			bool : 
//
//  @param				index : 
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_Delete(int index)
{
	bool status = false;

	if(!xmutexprocotolconnexions)  return false;
	
	xmutexprocotolconnexions->Lock();

	if(index<(int)ProtocolConnexions_GetNAvailable())	
		{
			DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
			if(protocolconnexion)
				{					
					if(protocolconnexion->GetDIOProtocol()) 
						{
							if(!protocolconnexion->IsInUse())					
								{						
									DIOPROTOCOL* protocol = protocolconnexion->GetDIOProtocol();
									
									protocolconnexions.Delete(protocolconnexion);					
									delete protocolconnexion;			

									DeleteProtocol(protocol);																							

									status = true;
								}			
						}
				}
		}
	
	xmutexprocotolconnexions->UnLock();

	return status;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER::DeleteAllConnexions
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2012 18:58:12
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::ProtocolConnexions_DeleteAll()
{	  			
	if(!xmutexprocotolconnexions)  return false;

	xmutexprocotolconnexions->Lock();
	
	int index = 0;

	while(index < ProtocolConnexions_GetNAvailable())
		{			
			DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);
			if(protocolconnexion)
				{			
					if((!protocolconnexion->IsInUse()))
						{
							DIOPROTOCOL* protocol = protocolconnexion->GetDIOProtocol();
									
							protocolconnexions.Delete(protocolconnexion);					
							delete protocolconnexion;			

							DeleteProtocol(protocol);																																					

						} else index++;	 					

				} else index++;	 								
		}

	xmutexprocotolconnexions->UnLock();
	
  return true;
}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::WaitToAnyConnexionIsConnected
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/09/2015 17:10:46
//	
//	@return 			bool : 
//
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::WaitToAnyConnexionIsConnected(int timeout)
{			
	int  index  = 0;
	bool status = true;

	xtimerout->Reset();

	while(1)
		{
			if(ProtocolConnexions_GetNConnected())
				{
					status = false;
					break;				
				}

			xsleep->MilliSeconds(10);
					
			if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout) 
				{
					status = false;
					break;
				}											
		}
	
	return status;
}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::WaitToAllConnexionsCanBeDeleted
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2016 17:07:09
//	
//	@return 			bool : 
//
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::WaitToAllConnexionsCanBeDeleted(int timeout)
{		
	if(!xmutexprocotolconnexions)  return false;

	xmutexprocotolconnexions->Lock();

	int  index  = 0;
	bool status = true;

	if(timeout) xtimerout->Reset();

	while(1)
		{
			bool candelete = true;

			for(int index=0; index< ProtocolConnexions_GetNAvailable(); index++)
				{												
					DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions.Get(index);	
					if(protocolconnexion) 
						{
							if(protocolconnexion->IsInUse()) candelete = false;
						} 
				}

			if(candelete) break;

			xsleep->MilliSeconds(10);
					
			if(timeout)
				{
					if(xtimerout->GetMeasureSeconds() > 0)
						{
							if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout) 
								{
									status = false;
									break;
								}											
						}
				}
		}

	xmutexprocotolconnexions->UnLock();

	return status;
}




/*-------------------------------------------------------------------
//  DIOPROTOCOLCONNEXIONSMANAGER::SendEvent
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/02/2012 17:31:42
//	
//	@return				bool : 
//	@param				type : 
//  @param				protocolconnexion : 
*/
/*-----------------------------------------------------------------*/
bool DIOPROTOCOLCONNEXIONSMANAGER::SendEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE type, DIOPROTOCOLCONNEXION* protocolconnexion)
{	
	if(!protocolconnexion) return false;

	DIOPROTOCOLCONNEXIONSMANAGERXEVENT xevent(this,type);							
	
	xevent.SetDIOStreamEnumServers(diostreamenumservers);	
	xevent.SetDIOStreamConfig(protocolconnexion->GetDIOStreamConfig());	
	xevent.SetDIOProtocol(protocolconnexion->GetDIOProtocol());
	xevent.SetProtocolConnexion(protocolconnexion);
	xevent.SetProtocolConnexionsManager(this);
	
	return PostEvent(&xevent);
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ManageProtocolConnexionsServer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 13:01:04
//	
*/
/*-----------------------------------------------------------------*/
void DIOPROTOCOLCONNEXIONSMANAGER::ManageProtocolConnexionsServer()
{
	if(!xthreadconnexions)							return;
	if(!xthreadconnexions->IsRunning())	return;

	xtimerconnexions->Reset();

	ProtocolConnexions_DeleteDisconnected();

	if(protocolconnexionsnlimit != DIOPROTOCOLCONNEXIONS_UNLIMITEDNCONNEXIONS)
		{
			if(!protocolconnexionsnlimit) return;
			
			if(ProtocolConnexions_GetNConnected() >= protocolconnexionsnlimit) 
				{
					return;
				}																	
		}	

	ProtocolConnexions_SendEventConnected();

	if(!ProtocolConnexions_GetNFreeToConnect()) 
		{
			DIOPROTOCOLCONNEXION* protocolconnexion = new DIOPROTOCOLCONNEXION(diostreamcfg);
			if(protocolconnexion)
				{
					DIOPROTOCOL* protocol = CreateProtocol();
					if(protocol) 
						{
							protocolconnexion->SetDIOProtocol(protocol);

							if(protocolconnexion->Connect()) 
								{
									if(xmutexprocotolconnexions) xmutexprocotolconnexions->Lock();
							
									protocol->SetConnexion(protocolconnexion);

									protocolconnexions.Add(protocolconnexion);								

									if(xmutexprocotolconnexions) xmutexprocotolconnexions->UnLock();						
								}
						}
					 else
						{
							DeleteProtocol(protocol);
							protocol = NULL;
		
							delete protocolconnexion;
							protocolconnexion = NULL;
						}
				}
			 else
				{
					delete protocolconnexion;
					protocolconnexion = NULL;
	 			}
		}
}



/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ManageProtocolConnexionsClient
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 13:00:43
//	
*/
/*-----------------------------------------------------------------*/
void DIOPROTOCOLCONNEXIONSMANAGER::ManageProtocolConnexionsClient()
{	
	if(!xmutexprocotolconnexions)				return;
	if(!xthreadconnexions)							return;
	if(!xthreadconnexions->IsRunning())	return;
	if(!xtimerconnexions)								return;
	if(!xtimerclienttry)								return;
		
	ProtocolConnexions_DeleteDisconnected();

	if(protocolconnexionsnlimit != DIOPROTOCOLCONNEXIONS_UNLIMITEDNCONNEXIONS)
		{
			if(!protocolconnexionsnlimit) return;
			
			if(ProtocolConnexions_GetNConnected() >= protocolconnexionsnlimit) 
				{
					return;
				}																	
		}


	if(xtimerclienttry->GetMeasureSeconds() <= DIOPROTOCOLCONNEXIONS_DEFAULTIMETRYCONNEXIONS) return;

	//DEBUG_PRINT(__L("---------------------------------------------------------------"));

	for(int c=0;c<(int)targetURLs.GetSize();c++)
		{	
			if(!xthreadconnexions->IsRunning())	break;
						
			DIOURL* URLclient = (DIOURL*)targetURLs.Get(c); 
			if(URLclient)
				{
					bool haveservervalid = false;

					switch(diostreamcfg->GetType())
						{
							case DIOSTREAMTYPE_UNKNOWN		: break;
							case DIOSTREAMTYPE_UART				: break;
							case DIOSTREAMTYPE_USB				: break;
							case DIOSTREAMTYPE_BLUETOOTH	: break;

							case DIOSTREAMTYPE_TCPIP			: { DIOSTREAMTCPIPCONFIG* scfg = (DIOSTREAMTCPIPCONFIG*)diostreamcfg;
																								XSTRING							  address;
																								if(scfg) 
																									{
																										if(URLclient->IsEmpty())																													
																											{																																																				
																												diostreamenumservers->Search();
																			
																												while(diostreamenumservers->IsSearching())
																													{																														
																														xsleep->MilliSeconds(1);
																													}	

																												diostreamenumservers->StopSearch(true);

																												if(diostreamenumservers->AreDevicesAvailable())
																													{
																														DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)diostreamenumservers->GetDevices()->Get(0);
																														if(device) device->GetIP()->GetXString(address);																																																																								
																													} 

																											} else address = URLclient->Get();
																										
																										if(!address.IsEmpty())
																											{
																												DIOURL* url= diofactory->CreateURL();
																												if(url)
																													{
																														(*url) = address.Get();

																														url->ResolveURL();																																																																														
																														scfg->GetRemoteURL()->Set(address);																															
																														diofactory->DeleteURL(url);

																														haveservervalid = true;
																													}
																											}
																									}																								

																								break;
																							}
																		
						}	
							
					if(haveservervalid)
						{
							DIOPROTOCOLCONNEXION* protocolconnexion = new DIOPROTOCOLCONNEXION(diostreamcfg);
							if(protocolconnexion) 
								{
									DIOPROTOCOL* protocol = CreateProtocol();
									if(protocol) 
										{
											protocolconnexion->SetDIOProtocol(protocol);

											if(protocolconnexion->Connect())
												{
													xtimerconnexions->Reset();

													DIOSTREAMTCPIPCONFIG* scfg = (DIOSTREAMTCPIPCONFIG*)diostreamcfg;
																														
													while(1)
														{																																	
															if(protocolconnexion->GetDIOProtocol()->IsConnected())
																{
																	if(xmutexprocotolconnexions) xmutexprocotolconnexions->Lock();															

																	protocol->SetConnexion(protocolconnexion);
																	protocolconnexions.Add(protocolconnexion);
													
																	if(xmutexprocotolconnexions) xmutexprocotolconnexions->UnLock();			
																																													
																	SendEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_CONNECTEDCONNEXION, protocolconnexion);
																
																	break;																										
																} 
															 
															if((xtimerconnexions->GetMeasureSeconds()>=5) || (!xthreadconnexions->IsRunning())) 
																{																																																																					
																	DeleteProtocol(protocol);
																	protocol = NULL;

																	delete protocolconnexion;
																	protocolconnexion = NULL;
																	
																	break;																		
																}	

															xsleep->MilliSeconds(50);
														}
									
													XDEBUG_PRINTCOLOR(1, __L("Trying to connect to %s [%s]: %s (%d) seconds."),  URLclient->Get(), scfg->GetRemoteURL()->Get(), protocolconnexion?__L("Connected!."):__L("Not Connected!."), xtimerconnexions->GetMeasureSeconds());
												}
											 else
												{
													DeleteProtocol(protocol);
													protocol = NULL;
		
													delete protocolconnexion;
													protocolconnexion = NULL;
												}
										}	
									 else
										{
											delete protocolconnexion;
											protocolconnexion = NULL;
										}
								}
						}
				}
		}

	xtimerclienttry->Reset();

}




/*-------------------------------------------------------------------
//	DIOPROTOCOLCONNEXIONSMANAGER::ThreadProtocolConnexions
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2015 13:02:57
//	
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
void DIOPROTOCOLCONNEXIONSMANAGER::ThreadProtocolConnexions(void* param)
{
	DIOPROTOCOLCONNEXIONSMANAGER* protocolconnexions = (DIOPROTOCOLCONNEXIONSMANAGER*)param;
	if(!protocolconnexions) return;	
				
	if(protocolconnexions->isserver)
		{				
			protocolconnexions->ManageProtocolConnexionsServer();		
		}
	 else 
		{
			protocolconnexions->ManageProtocolConnexionsClient();
		}

	int c=0;
	while(c < protocolconnexions->ProtocolConnexions_GetNAvailable())
	  {					
			DIOPROTOCOLCONNEXION* protocolconnexion = (DIOPROTOCOLCONNEXION*)protocolconnexions->protocolconnexions.Get(c);				
			if(protocolconnexion)
				{					
					DIOPROTOCOL* protocol = protocolconnexion->GetDIOProtocol();
					if(protocol)
						{
							if(protocol->IsConnected())
								{
									if(!protocol->IsInitialized())
										{		
											if(protocolconnexions->isserver)
												{													
													protocolconnexion->SetInUse(true, DIOPROTOCOLCONNEXIONSINUSEID_INIPROTOCOL); 

													protocol->GetDIOStream()->GetXTimerNotActivity()->Reset();
													protocolconnexions->SendEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_INITPROTOCOL, protocolconnexion);																																													
													if(protocol->IsInitialized()) protocol->CMD_SendIsInitServerProtocol(true);		

													protocolconnexion->SetInUse(false, DIOPROTOCOLCONNEXIONSINUSEID_INIPROTOCOL); 
												} 
											 else
												{
													XTIMER* timerout = xfactory->CreateTimer();													
													while(1)
														{
															if(protocolconnexion->GetDIOProtocol()->IsDisconnected()) break;
															if(protocol->IsServerInitialized())											  break;																														

															if(timerout) 
																{
																	if(timerout->GetMeasureSeconds() > 5) break;
																}
															
															xsleep->MilliSeconds(10);
														}

													xfactory->DeleteTimer(timerout);

													
													if(protocol->IsServerInitialized())
														{
															protocolconnexion->SetInUse(true, DIOPROTOCOLCONNEXIONSINUSEID_INIPROTOCOL); 

															protocol->GetDIOStream()->GetXTimerNotActivity()->Reset();											
															protocolconnexions->SendEvent(DIOPROTOCOLCONNEXIONSMANAGERXEVENTTYPE_INITPROTOCOL, protocolconnexion);																																													

															protocolconnexion->SetInUse(false, DIOPROTOCOLCONNEXIONSINUSEID_INIPROTOCOL); 
														}
												} 
																						
											XDEBUG_PRINTCOLOR((protocol->IsInitialized()?2:4), __L("Protocol Connexion Manager: %s Init Protocol %s") , protocolconnexions->isserver?__L("Server"):__L("Client"), protocol->IsInitialized()?__L("Ok!"):__L("Error!"));
																																	
											if(!protocol->IsInitialized())
											  {				
													protocolconnexions->ProtocolConnexions_Disconnect(c);																
												}																					
										}
									 else
									  {											
											if(protocol->GetDIOStream()->GetXTimerNotActivity()->GetMeasureSeconds() > DIOPROTOCOLCONNEXIONS_DEFAULTIMECHECKCONNEXIONS)
												{													
													XDWORD milliseconds;

													bool status = protocol->CMD_Ping(milliseconds);
													if(!status) 
														{
															protocolconnexions->ProtocolConnexions_Disconnect(c);																														

															XDEBUG_PRINTCOLOR(status?1:4, __L("Protocol Connexion Manager: Check connexion protocol %s"), status?__L("Ok!"):__L("Error!"));
														}
												}
										}
								}
						}					
				}	

			c++;
		}	
}


