/*------------------------------------------------------------------------------------------
// DIOSSHReverse.HPP
*/
/**
// \class
//
//	DIO SSH reverse connexions (only ssh/sshpass avaible mmand)
//
//	@author: No se ha podido abrir el fichero de author
//
//	Date of Creation : 06/12/2017 10:35:30 
//	Last Modification : 
*/
/*	GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _DIOSSHREVERSE_HPP
#define _DIOSSHREVERSE_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/


#include "XFactory.h"
#include "XString.h"
#include "XSystem.h"
#include "XFile.h"
#include "XFileINI.h"
#include "XSleep.h"
#include "XThread.h"
#include "XLog.h"
#include "XDebug.h"



/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOSSHREVERSE_DEFAULTAPPLICATION			__L("sshpass")
#define DIOSSHREVERSE_CFGREMOTENAMEFILE				__L("sshrem.ini")
#define DIOSSHREVERSE_DEFAULTPORTSSH					22

#define DIOSSHREVERSE_LOGSECTIONID						__L("SSHreverse")


/*---- CLASS -----------------------------------------------------------------------------*/


class DIOSSHREVERSE
{
	public:																

		static bool											GetIsInstanced		  () 
													  				{ 
																			return instance!=NULL; 
																		}

		static DIOSSHREVERSE&						GetInstance					()
																		{
																			if(!instance) instance = new DIOSSHREVERSE();
																						
																			return (*instance);	
																		}						

		static bool											DelInstance					()
																		{
																			if(instance)
																				{																			
																					delete instance;
																					instance = NULL;

																					return true;
																				} 
																				
																			return false;
																		}						


		XSTRING*											GetURLTarget					()																								{ return &URLtarget;																						}
		XSTRING*											GetLocalIP						()																								{ return &localIP;																							}
		XSTRING*											GetLogin							()																								{	return &login;																								}
		XSTRING*											GetPassword						()																								{ return &password;																							}
		
		XDWORD												GetPort								()																								{ return port;																									}
		void													SetPort								(XDWORD port)																			{ this->port = port; 																						}

		bool													DownloadCFG           (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP);

		bool													DownloadCFG           (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return DownloadCFG(URL.Get(), publicIP, localIP);										 
																	}

		bool													DownloadCFG           (DIOURL&  URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return DownloadCFG(URL.Get(), publicIP, localIP);										 
																	}

		bool													Activate							();
		bool													DeActivate						();
		
		bool													IsRunning							();
		
		bool													Exec						      (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP)
																	{
																		bool status = false;
																		
																		if(IsRunning())	
																			{
																				status = true;
																				return status;
																			}
																																					
																		if(DownloadCFG(URL, publicIP, localIP))  
																			{
																				Activate();																																							
																			}
																		
																		DelInstance(); 

																		return status;	
																	}
		
		bool													Exec							    (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return Exec(URL.Get(), publicIP, localIP);										 
																	}

		bool													Exec						      (DIOURL&  URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return Exec(URL.Get(), publicIP, localIP);										 
																	}
																
	

	private:
																	DIOSSHREVERSE					()
																	{
																		Clean();
																	}

																	DIOSSHREVERSE					(DIOSSHREVERSE const&);			// Don't implement  

		virtual							     	   ~DIOSSHREVERSE					()
																	{
																		Clean();
																	}

		void													operator =						(DIOSSHREVERSE const&);			// Don't implement	

		void													Clean									()
																	{
																		URLtarget.Empty();
																		localIP.Empty();		
																		login.Empty();
																		password.Empty();														
																		port							= 0;																		
																	}

		
		XSTRING												URLtarget;
		XSTRING												localIP;		
		XSTRING												login;
		XSTRING												password;
		XDWORD												port;	
		
		static DIOSSHREVERSE*					instance;	
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif