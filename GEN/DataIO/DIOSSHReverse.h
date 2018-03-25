/*------------------------------------------------------------------------------------------
// DIOSSHReverse.HPP
*/
/**
// \class
//
//	DIO SSH reverse connexions (Linux)
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

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/wait.h>
	#include <sys/ioctl.h>
	#include <sys/select.h>
	#include <sys/prctl.h>

	#include <unistd.h>
	#include <fcntl.h>
	#include <signal.h>
	#include <termios.h>

	#include <stdio.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <string.h>
#endif

#include "XFactory.h"
#include "XString.h"
#include "XSystem.h"
#include "XFile.h"
#include "XFileINI.h"
#include "XSleep.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOWebClient.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOSSHREVERSE_DEFAULTAPPLICATION			__L("sshpass")
#define DIOSSHREVERSE_CFGREMOTENAMEFILE				__L("sshrem.ini")
#define DIOSSHREVERSE_DEFAULTPORTSSH					22


enum DIOSSHREVERSE_RETURN_CODES 
{
	RETURN_NOERROR,
  RETURN_INVALID_ARGUMENTS,
  RETURN_CONFLICTING_ARGUMENTS,
  RETURN_RUNTIME_ERROR,
  RETURN_PARSE_ERRROR,
  RETURN_INCORRECT_PASSWORD,
  RETURN_HOST_KEY_UNKNOWN,
  RETURN_HOST_KEY_CHANGED,
};


typedef struct 
{
	enum 	{ PWT_STDIN, 
					PWT_FILE, 
					PWT_FD, 
					PWT_PASS 

				} pwtype;

  union	{ const char*  filename;
				  int					fd;
				  const char*	password;

    		} pwsrc;

  const char*  pwprompt;
  int					 verbose;
    
} DIOSSHREVERSE_ARGS;



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

		bool													DownloadCFG           (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP)
																	{
																		if(!URL)   return false;

																		XBUFFER xbuffer;
																		DIOURL  _URL;
																		bool	  status  = false;
																		bool    result  = false;

																		DIOWEBCLIENT* webclient = new DIOWEBCLIENT();
																		if(!webclient)  return false;	
	
																		_URL.Set(URL);	
																		_URL.Slash_Add();			
																		_URL.Add(DIOSSHREVERSE_CFGREMOTENAMEFILE);
					
																		status = webclient->Get(_URL, xbuffer, NULL, 3);
																		if(status)
																			{
																				XFILEINI* fileini = new XFILEINI();
																				if(fileini)
																					{
																						status = fileini->AddBufferLines(XFILETXTFORMATCHAR_ASCII, xbuffer);
																						if(status)
																							{
																								status = fileini->ConvertFromLines();
																								if(status)
																									{	
																										int c = 1;
																										while(1)
																											{
																												XSTRING section;
																												XSTRING value;

																												section.Format(__L("ENTRY%02d"), c);

																												if(!fileini->SelectSection(section)) break;

																												status = fileini->ReadValue(__L("isactive"), value);																											
																												if(status)
																													{
																														if(!value.Compare(__L("yes"), true))
																															{
																																DIOURL* urlorigin = diofactory->CreateURL();
																																if(urlorigin)
																																	{
																																		XSTRING _publicIP;
																																		XSTRING _localremoteIP;

																																		status = fileini->ReadValue(__L("urlorigin"), (*urlorigin));																											
																																		if(status)
																																			{
																																				bool validaccess = false;

																																				urlorigin->ResolveURL(_publicIP);

																																				if(!publicIP.Compare(_publicIP))
																																					{
																																						validaccess = true;

																																						status = fileini->ReadValue(__L("localremoteip"), _localremoteIP);																											
																																						if(status)  
																																							{
																																								if(_localremoteIP.GetSize())
																																									{
																																										if(_localremoteIP.Compare(localIP)) validaccess = false;
																																									}
																																							}
																																					}


																																				if(validaccess)
																																					{
																																						localIP		= __L("localhost");

																																						status = fileini->ReadValue(__L("urltarget"), value);																											
																																						if(status) GetURLTarget()->Set(value);

																																						status = fileini->ReadValue(__L("localip"), value);																											
																																						if(status)  GetLocalIP()->Set(value);

																																						status = fileini->ReadValue(__L("login"), value);																											
																																						if(status) GetLogin()->Set(value);

																																						status = fileini->ReadValue(__L("password"), value);																											
																																						if(status) GetPassword()->Set(value);

																																						status = fileini->ReadValue(__L("port"), value);																											
																																						if(status) SetPort(value.ConvertToInt());		

																																						result = true;
																																					}
																																			}

																																		diofactory->DeleteURL(urlorigin);
																																	}
																												 			}
																													}

																												c++;
																											}
																									}
																							}

																						delete fileini;
																					}
																			}	

																		delete webclient;

																		return result;
																	}

		bool													DownloadCFG           (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return DownloadCFG(URL.Get(), publicIP, localIP);										 
																	}

		bool													DownloadCFG           (DIOURL&  URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return DownloadCFG(URL.Get(), publicIP, localIP);										 
																	}

		bool													Activate        			()
																	{
																		bool status = false;

																		threadsshreverse = CREATEXTHREAD(XTHREADGROUPID_DIOSSHREVERSE, __L("DIOSSHREVERSE::Activate"), ThreadRunFunction ,(void*)this);	
																		if(!threadsshreverse) return false;

																		status = threadsshreverse->Ini();

																		if(status)
																			{
																				XTIMER* xtimeractive = xfactory->CreateTimer();
																				if(xtimeractive)
																					{
																						while(1)
																							{
																								if(xtimeractive->GetMeasureSeconds() > 10) break;

																								if(IsRunning())
																									{
																										status = true;
																										break;
																									}

																								xsleep->Seconds(1);
																							}			

																						xfactory->DeleteTimer(xtimeractive);
																					}									
																			}

																		return status;
																	}

																																	
		bool													DeActivate						()
																	{																			
																		XSTRING command;
																		bool		status 		 = false;
																		int 		returncode = 0;

																	  XSYSTEM* xsystem = xfactory->CreateSystem();
																		if(xsystem) 
																			{
																				command.Format(__L("killall -9 %s > /dev/null"), DIOSSHREVERSE_DEFAULTAPPLICATION);
																				status = xsystem->MakeCommand(command.Get(), &returncode);

																				xfactory->DeleteSystem(xsystem);																		
																			}

																		if(threadsshreverse)
																			{																			
																				DELETEXTHREAD(XTHREADGROUPID_DIOSSHREVERSE, threadsshreverse);																			  
																				threadsshreverse = NULL;
																			}

																		XDEBUG_PRINTCOLOR(2, __L("SSHReverse: Deactivate exec %s, return code: %d"), status?__L("ok"):__L("error"), returncode);

																		status = true;
																		if(returncode) status = false;
																		
																		return status;
																	}

		bool													IsRunning							()	
																	{
																		XSYSTEM* xsystem = xfactory->CreateSystem();
																		if(!xsystem) return false;

																		bool status = xsystem->IsApplicationRunning(DIOSSHREVERSE_DEFAULTAPPLICATION);
																		if(status)
																			{
																				status = false;
																																		
																				XSTRING command;
																				XSTRING publicIPtarget;
																				int     returncode = 0;																				
																																								
																				DIOURL* URLpublic = diofactory->CreateURL();
																				if(URLpublic)
																					{																				
																						URLpublic->Set(URLtarget.Get());
																						URLpublic->ResolveURL(publicIPtarget);

																						diofactory->DeleteURL(URLpublic);																						
																					}

																				if(!publicIPtarget.IsEmpty())
																					{
																						XPATH		xpath;					
																						XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
																						xpath.Slash_Add();
																						xpath.Add(__L("backscreen"));

																						command.Format(__L("netstat -napt > %s"), xpath.Get());
																						if(xsystem->MakeCommand(command.Get(), &returncode))
																							{
																								XFILETXT* xfiletxt = new XFILETXT();
																								if(xfiletxt)
																									{	
																										if(xfiletxt->Open(xpath, true))
																											{
																												if(xfiletxt->ReadAllFile())
																													{
																														for(int c=0; c<xfiletxt->GetNLines(); c++)
																															{
																																if(xfiletxt->GetLine(c))
																																	{				
																																		XSTRING* line = xfiletxt->GetLine(c);
																																		if(line)
																																			{
																																				if((line->Find(publicIPtarget.Get() , false, 0) != XSTRING_NOTFOUND) &&
																																					 (line->Find(__L("ESTABLISHED")   , false, 0) != XSTRING_NOTFOUND) &&
																																					 (line->Find(__L("/ssh ")				  , false, 0) != XSTRING_NOTFOUND)) 																																					
																																					{	
																																						status = true;																																																																										
																																						break;							
																																					}
																																			}
																																	}																														
																															}
																													}

																												xfiletxt->Close();																												
																											}
																										
																										xfiletxt->GetPrimaryFile()->Erase(xpath);	
																										
																										delete xfiletxt;
																									}																							
																							}
																					}
																			}			

																		xfactory->DeleteSystem(xsystem);

																		XDEBUG_PRINTCOLOR(2, __L("SSHReverse: Is active? %s"), status?__L("yes"):__L("no"));
																		
																		return status;
																	}


		bool													Exec						      (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP)
																	{
																		bool status = false;
																		
																		if(IsRunning())	
																			{
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

																		threadsshreverse	= NULL;
																	}

		bool													ActivateInThread				()
																	{
																		XSTRING command;
																		bool		status 		 = false;
																		int 		returncode = 0;

																		XSYSTEM* xsystem = xfactory->CreateSystem();
																		if(xsystem) 
																			{
																				XDEBUG_PRINTCOLOR(2, __L("SSHReverse: Activate exec %s, return code: %d"), status?__L("ok"):__L("error"), returncode);
																				
																				command.Format(__L("%s -p %s ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -N -R %d:%s:%d %s@%s > /dev/null"), DIOSSHREVERSE_DEFAULTAPPLICATION, password.Get(), port, localIP.Get() ,DIOSSHREVERSE_DEFAULTPORTSSH, login.Get(), URLtarget.Get());
																				
																				status = xsystem->MakeCommand(command.Get(), &returncode);

																				xfactory->DeleteSystem(xsystem);																		
																			}		
																		
																		return status;																		
																	}

		static void										ThreadRunFunction			(void* param);	
		
		XSTRING												URLtarget;
		XSTRING												localIP;		
		XSTRING												login;
		XSTRING												password;
		XDWORD												port;	

		XTHREADCOLLECTED*							threadsshreverse;
		
		static DIOSSHREVERSE*					instance;	
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

DIOSSHREVERSE* DIOSSHREVERSE::instance = NULL;




/*-------------------------------------------------------------------
//	 DIOSSHREVERSE::ThreadRunFunction
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	25/03/2018 18:55:39
//
//	@return		void : 
//
//	@param		void* : 
//
*//*-----------------------------------------------------------------*/
void DIOSSHREVERSE::ThreadRunFunction(void* param)
{
	DIOSSHREVERSE* diosshreverse = (DIOSSHREVERSE*)param;
	if(!diosshreverse) return;

	diosshreverse->ActivateInThread();

	diosshreverse->threadsshreverse->Run(false);	
}


#endif