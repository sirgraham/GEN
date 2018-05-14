/*------------------------------------------------------------------------------------------
//	APPUPDATE.CPP
//	
//	DNS Manager class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 13/11/2013 12:20:38
//	Last Mofificacion	:	
//	
//	EndoraSoft (C) Copyright. All right reserved.
//----------------------------------------------------------------------------------------*/

	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "XBase.h"
#include "XPath.h"
#include "XFactory.h"
#include "XRand.h"
#include "XDir.h"
#include "XString.h"
#include "XSystem.h"
#include "XLog.h"
#include "XConsole.h"
#include "XFileTXT.h"
#include "XFileINI.h"
#include "XLanguage.h"
#include "XScheduler.h"
#include "XDebug.h"
#include "XThread.h"

#include "Main.h"

#include "DIOFactory.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamEnumDevices.h"
#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIONTP.h"
#include "DIOSMTP.h"
#include "DIOURL.h"
#include "DIOApplicationUpdate.h"
#include "DIOWebClientXEvent.h"
#include "DIOWebClient.h"
#include "DIOCheckInternetConnexion.h"
#include "DIOWebScraper.h"
#include "DIOWebScraperPublicIP.h"
#include "DIOWebScraperGeolocationIP.h"
#include "DIOAlerts.h"

#include "HashCRC32.h"

#include "APPUpdateCFG.h"

#include "APPUpdate.h"

#include "XMemory.h"
	


//---- GENERAL VARIABLE --------------------------------------------------------------------
	
APPUPDATE* appupdate = NULL;
	



//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  APPUPDATE::APPUPDATE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:15:15
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
APPUPDATE::APPUPDATE() :  XFSMACHINE(0)
{		
	Clean();
}


//-------------------------------------------------------------------
//  APPUPDATE::~APPUPDATE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:15:40
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
APPUPDATE::~APPUPDATE()
{
	Clean();
}


//-------------------------------------------------------------------
//  XAPPLICATION::Create
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 11:02:38
//	
//	@return 			XAPPLICATION* : 
//	*/
//-------------------------------------------------------------------
XAPPLICATION* XAPPLICATION::Create()
{
	appupdate = new APPUPDATE();

	return appupdate;
}



//-------------------------------------------------------------------
//  APPUPDATE::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:17:31
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool APPUPDATE::Ini()
{
	if(!xfactory) return false;

	XSTRING string;	 
	XSTRING stringresult;	 
	bool	  status;

	//-------------------------------------------------------------------------------------------------

	GetApplicationName()->Set(APPUPDATE_NAMEAPP);

	//-------------------------------------------------------------------------------------------------
	  
	string = APPUPDATE_NAMEAPP;			XDEBUG_SETAPPLICATIONNAME(string);																				
	XDEBUG_SETAPPLICATIONVERSION(APPUPDATE_VERSION, APPUPDATE_SUBVERSION, APPUPDATE_SUBVERSIONERR);
	string = __L(" ");							XDEBUG_SETAPPLICATIONID(string);

	//--------------------------------------------------------------------------------------------------

	XPATHSMANAGER::GetInstance().AdjustRootPathDefault(APPUPDATE_NAMEAPP, APPUPDATE_DIRECTORYMAIN);

	if(!XPATHSMANAGER::GetInstance().CreateAllPathSectionOnDisk()) return false;

	//--------------------------------------------------------------------------------------------------
	
	InitFSMachine();

	//--------------------------------------------------------------------------------------
	
	xtimer = xfactory->CreateTimer();
	if(!xtimer) return false;

	xtimerupdate = xfactory->CreateTimer();
	if(!xtimerupdate) return false;

	xtimerupdate->AddSeconds(120);

	xdatetime = xfactory->CreateDateTime();
	if(!xdatetime) return false;

	
	//--------------------------------------------------------------------------------------

	xconsole = xfactory->CreateConsole();
	if(!xconsole) return false;

 	xconsole->Clear();																										
	ShowHeader(true);		

	//--------------------------------------------------------------------------------------

	/*
	bool modeserver = false;

	if(GetExecParams())
		{
			XSTRING* param = (XSTRING*)GetExecParams()->Get(0);	
			if(param)
				{
					param->ToUpperCase(); 
					if(!param->Compare(__L("SERVER")))  modeserver = true;
				}
		}
	*/

	//--------------------------------------------------------------------------------------

  string.Format(APPUPDATE_MESSAGEMASK,__L("Cargando configuracion"));    
	xconsole->PrintMessage(string.Get(),1,true,false);

	cfg  = new APPUPDATECFG(APPUPDATE_CFGNAMEFILE);
	string.Format((cfg)?__L("Ok."):__L("ERROR!"));  			
	xconsole->PrintMessage(string.Get(), 0, false, true);
	if(!cfg) return false;

	//--------------------------------------------------------------------------------------

	if(cfg->Log_IsActive())
		{
			string.Format(APPUPDATE_MESSAGEMASK,__L("Activando servicio de LOG"));    
			xconsole->PrintMessage(string.Get(),1,true,false);

			XPATH xpath;
			XPATH xpathgeneric;
			
			XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathgeneric);
			xpath.Create(3 , xpathgeneric.Get(), APPUPDATE_LOGNAMEFILE,__L(".log"));	

			if(!XLOG::GetInstance().Ini(xpath, (*GetApplicationName()), true)) 
				{
					xconsole->PrintMessage(__L("ERROR!"),0,false,true);
					return false;
				}

			xconsole->PrintMessage(__L("Ok."),0,false,true);

			XLOG::GetInstance().SetLimit(XLOGTYPELIMIT_SIZE, cfg->Log_MaxSize()*1000, cfg->Log_ReductionPercent());
			XLOG::GetInstance().SetBackup(true, cfg->Log_Backup_GetMaxFiles(), true);
			XLOG::GetInstance().SetFilters(cfg->Log_ActiveSectionsID()->Get(), cfg->Log_LevelMask());
		}

	//------------------------------------------------------------------------------------
		
	if(cfg->Log_IsActive())	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDINITIATION, false, __L("Inicializando aplicacion (ver %d.%d.%d)%s.")	, APPUPDATE_VERSION			
																																																																												, APPUPDATE_SUBVERSION
																																																																												, APPUPDATE_SUBVERSIONERR
																																																																												#ifdef XDEBUG
																																																																												, __L(" (Modo Debug)")
																																																																												#else
																																																																												, __L("")
																																																																												#endif
																																																																												);								
	//--------------------------------------------------------------------------------------
	
	status = false;

	string.Format(APPUPDATE_MESSAGEMASK, __L("Control de conexiones"));  
	xconsole->PrintMessage(string.Get(),1,true,false);

	checkconnexions =  new DIOCHECKCONNEXIONS();
	status = (checkconnexions)?true:false;

	if(status) status = checkconnexions->Ini(3);

	stringresult.Format((status)?__L("Ok."):__L("ERROR!"));  			
	xconsole->PrintMessage(stringresult.Get(), 0, false, true);

	XLOG::GetInstance().AddEntry((status)?XLOGLEVEL_INFO:XLOGLEVEL_ERROR, APPUPDATECFG_LOGSECTIONIDINITIATION, false, __L("%s: %s")	, string.Get(), stringresult.Get());						

	if(!status) return false;


	//--------------------------------------------------------------------------------------

	string.Format(APPUPDATE_MESSAGEMASK,__L("Chequeando conexion a internet"));    
	xconsole->PrintMessage(string.Get(), 1, true, false);

	checkinternetconnexion	= new DIOCHECKINTERNETCONNEXION( checkconnexions);
	if(!checkinternetconnexion) return false; 

	haveinternet = checkinternetconnexion->Check();		

	stringresult = (haveinternet)?__L("Ok."):__L("Sin conexion!"); 	
			
	xconsole->PrintMessage(stringresult.Get(), 0, false, true);	

	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDINITIATION, false, __L("%s: %s")	, string.Get(), stringresult.Get());	
	
	
	//--------------------------------------------------------------------------------------
	
	publicip = new DIOWEBSCRAPERPUBLICIP();
	if(!publicip) return false;
				
	geolocationip = new DIOWEBSCRAPERGEOLOCATIONIP();
	if(!geolocationip) return false;	


	//--------------------------------------------------------------------------------------

	if(haveinternet) 	
		{
			status = false;

			string.Format(APPUPDATE_MESSAGEMASK,__L("Ajustando fecha/hora desde NTP"));    
			xconsole->PrintMessage(string.Get(), 1, true, false);

			DIONTP* ntp = new DIONTP();
			if(ntp)
				{		
					DIOURL*  url   = diofactory->CreateURL();			
					if(xdatetime && url)  
						{	
							XCHAR* urllist[] =	{ __L("1.es.pool.ntp.org")			,
																		__L("1.europe.pool.ntp.org")	,
																		__L("3.europe.pool.ntp.org") 
																	};

							for(int c=0; c<sizeof(urllist)/sizeof(XCHAR*); c++)
								{
									(*url) = urllist[c];

									xdatetime->Read();

									status = ntp->GetTime((*url), 5, xsystem->HardwareUseLittleEndian(), (*xdatetime));
									if(status)
										{				
											XSTRING dayofweekstring;						
											xdatetime->GetDayOfWeekString(dayofweekstring);

											stringresult.Format(__L("Ok. %02d/%02d/%04d %02d:%02d:%02d %s")	, xdatetime->GetDay()		, xdatetime->GetMonth()		, xdatetime->GetYear()
																																											, xdatetime->GetHours()	, xdatetime->GetMinutes() , xdatetime->GetSeconds()
																																											, dayofweekstring.Get());																										
											xdatetime->Write();	

											break;
										}			
								}
						 } 
		
					if(!status) stringresult = __L("ERROR!");

					xconsole->PrintMessage(stringresult.Get() , 0, false, true);

					XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDINITIATION, false, __L("%s: %s")	, string.Get(), stringresult.Get());		
					
					diofactory->DeleteURL(url);
					 
					delete ntp;
				}			
		}

	//--------------------------------------------------------------------------------------
			
	#ifdef XAPPLICATION_EMAILALERT
	XAPPLICATIONEMAILALERT* emailalert = GetEmailAlert();
	if(emailalert)
		{		
			DIOSMTP* smtp = emailalert->GetSMTP();
			if(smtp)
				{	
					string.Format(APPUPDATE_MESSAGEMASK, __L("Chequeando alertas de email"));    
					xconsole->PrintMessage(string.Get(), 1, true, false);

					smtp->Server_GetURL()->Set(cfg->EmailAlert_GetSMTPURL()->Get());
					smtp->Server_SetPort(cfg->EmailAlert_GetSMTPPort());
					smtp->Server_GetLogin()->Set(cfg->EmailAlert_GetSMTPLogin()->Get());
					smtp->Server_GetPassword()->Set(cfg->EmailAlert_GetSMTPPassword()->Get());
					smtp->GetSenderName()->Set(APPUPDATE_NAMEAPP);
					smtp->GetSenderEmail()->Set(cfg->EmailAlert_GetEmailSender()->Get());									
					smtp->SetContentType(DIOSSMPTCONTENTTYPE_UTF8);
			
					for(int c=0; c<APPUPDATECFG_MAXEMAILALERTRECIPIENTS; c++)
						{
							DIOSMTPRECIPIENTTYPE	type = DIOSMTPRECIPIENTTYPE_UNKNOWN;
							XSTRING								name;
							XSTRING								email;

							name.AjustSize(_MAXSTR);
							email.AjustSize(_MAXSTR);

							cfg->EmailAlert_GetRecipient(c)->UnFormat(__L("%d,%s,%s"), &type, name.Get(), email.Get());
							
							name.AjustSize();
							email.AjustSize();

							if((type == DIOSMTPRECIPIENTTYPE_TO) || (type == DIOSMTPRECIPIENTTYPE_CC) || (type == DIOSMTPRECIPIENTTYPE_BCC))
								{
									smtp->AddRecipient(type, name	, email);
								}
						}

					stringresult  = smtp->Server_IsAvailable()?__L("Ok."):__L("Servidor no disponible!");	

					xconsole->PrintMessage(stringresult.Get(), 0, false, true);
					XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDINITIATION, false, __L("%s: %s")	, string.Get(), stringresult.Get());	

				}
		}
	#endif
	

	#ifdef DIOALERTS_ACTIVE

	status = false;

	string.Format(APPUPDATE_MESSAGEMASK,__L("Soporte para alertas"));    
	xconsole->PrintMessage(string.Get(), 1, true, false);					
				
	XCHAR* recipients[APPUPDATECFG_MAXEMAILALERTRECIPIENTS];
	int    nrecipients = 0;

	memset(recipients, 0, sizeof(XCHAR*)*APPUPDATECFG_MAXEMAILALERTRECIPIENTS);
			
	for(int c=0; c<APPUPDATECFG_MAXEMAILALERTRECIPIENTS; c++)
		{												
			if(cfg->EmailAlert_GetRecipient(c)) 
				{
					if(cfg->EmailAlert_GetRecipient(c)->GetSize())
						{
							recipients[nrecipients] = cfg->EmailAlert_GetRecipient(c)->Get();
							nrecipients++;
						}
				}
		}
			
	status = DIOALERTS::GetInstance().Sender_SMTPConfig(cfg->EmailAlert_GetSMTPURL()->Get()	, cfg->EmailAlert_GetSMTPPort()
																																													, cfg->EmailAlert_GetSMTPLogin()->Get()
																																													, cfg->EmailAlert_GetSMTPPassword()->Get()
																																													, cfg->EmailAlert_GetEmailSender()->Get() 
																																													, nrecipients
																																													, recipients[0], recipients[1], recipients[2], recipients[3], recipients[4]
																																													, recipients[5], recipients[6], recipients[7], recipients[8], recipients[9]);									
		
							
	stringresult = (status)?__L("Ok. "):__L("ERROR!");  						
	xconsole->PrintMessage(stringresult.Get(), 0, false, true);
	XLOG::GetInstance().AddEntry((status)?XLOGLEVEL_INFO:XLOGLEVEL_ERROR, APPUPDATECFG_LOGSECTIONIDINITIATION, false, __L("%s: %s")	, string.Get(), stringresult.Get());	

	#endif

	//--------------------------------------------------------------------------------------
			
	string.Format(APPUPDATE_MESSAGEMASK,__L("Activando gestion temporizador"));  
  xconsole->PrintMessage(string.Get(),1,true,false);

	xscheduler = new XSCHEDULER();
	if(!xscheduler) return false;

	//-------------------------------------

	XDATETIME				xdatetimecadence;
	XTIMER					xtimercadence;
	XSCHEDULERTASK*	xtask;

	if(cfg->CheckInternetStatus_IsActive())
		{
			xtask = new XSCHEDULERTASK( xscheduler);
			if(!xtask) return false;

			xdatetimecadence.SetToZero();
	
			xtimercadence.Reset();
			xtimercadence.AddSeconds(cfg->CheckInternetStatus_GetCadence());
	
			xtimercadence.GetMeasureToDate(&xdatetimecadence);
			xtask->SetNCycles(XSCHEDULER_CYCLEFOREVER, &xdatetimecadence);
			xtask->SetID(APPUPDATETASKID_CHECKINTERNETCONNEXION);
			xtask->SetIsActive(true);
		
			xscheduler->Task_Add(xtask);
		}

	//-------------------------------------
	
	if(cfg->CheckMemStatus_IsActive())
		{
			xtask = new XSCHEDULERTASK( xscheduler);
			if(!xtask) return false;

			xdatetimecadence.SetToZero();
	
			xtimercadence.Reset();
			xtimercadence.AddSeconds(cfg->CheckMemStatus_GetCadence());
	
			xtimercadence.GetMeasureToDate(&xdatetimecadence);
			xtask->SetNCycles(XSCHEDULER_CYCLEFOREVER, &xdatetimecadence);
			xtask->SetID(APPUPDATETASKID_CHECKMEMORYSTATUS);
			xtask->SetIsActive(true);
		
			xscheduler->Task_Add(xtask);			
		}

	//-------------------------------------------------------------------------------------

	SubscribeEvent(XEVENTTYPE_SCHEDULER, xscheduler);	

	status = xscheduler->Ini();

	stringresult = (status)?__L("Ok."):__L("ERROR!");	
	xconsole->PrintMessage(stringresult.Get(), 0, false, true);
	
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDINITIATION, false, __L("%s: %s")	, string.Get(), stringresult.Get());		
	
	if(!status) return false;	
					
	//--------------------------------------------------------------------------------------

	SetEvent(APPUPDATE_XFSMEVENT_INI);

	return true;
}
  




//-------------------------------------------------------------------
//  APPUPDATE::InitFSMachine
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 16:34:54
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool APPUPDATE::InitFSMachine()
{
	if(!AddState(	APPUPDATE_XFSMSTATE_NONE		, 												 												
								APPUPDATE_XFSMEVENT_INI			, APPUPDATE_XFSMSTATE_INI					,								
								APPUPDATE_XFSMEVENT_END			, APPUPDATE_XFSMSTATE_END					,
								EVENTDEFEND)) return false;

	if(!AddState(	APPUPDATE_XFSMSTATE_INI			, 												 												
								APPUPDATE_XFSMEVENT_RUN			, APPUPDATE_XFSMSTATE_RUN					,
								APPUPDATE_XFSMEVENT_END			, APPUPDATE_XFSMSTATE_END					,
								EVENTDEFEND)) return false;

	if(!AddState(	APPUPDATE_XFSMSTATE_RUN			, 												 																				
								APPUPDATE_XFSMEVENT_END			, APPUPDATE_XFSMSTATE_END					,
								EVENTDEFEND)) return false;

	if(!AddState(	APPUPDATE_XFSMSTATE_END			,																
								APPUPDATE_XFSMEVENT_INI			, APPUPDATE_XFSMSTATE_INI					,
								EVENTDEFEND)) return false;

	return true;
}





//-------------------------------------------------------------------
//  APPUPDATE::FirstUpdate
/**
//	
//	@author        Abraham J. Velez
//	@version       20/2/2003   16:26:12
//	
//	@return        bool : 
*/
//-------------------------------------------------------------------
bool APPUPDATE::FirstUpdate()
{
	//--------------------------------------------------------------------------------------

	ShowConfig();

	xconsole->PrintMessage(__L(" "), 0, false, true);		
	//WaitKey(__L("Pulsa una tecla para continuar... (%d)"), 1, true, 5);
	
	return true;
}




//-------------------------------------------------------------------
//  APPUPDATE::PrevUpdate
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:18:04
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool APPUPDATE::PrevUpdate()
{	  
	Update();
	
	return true;
}



//-------------------------------------------------------------------
//  APPUPDATE::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 16:53:03
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool APPUPDATE::Update()
{
  if(GetEvent()==APPUPDATE_XFSMEVENT_NONE) // Not new event
		{			
		  switch(GetCurrentState())
				{
					case APPUPDATE_XFSMSTATE_INI	 			: 
					case APPUPDATE_XFSMSTATE_RUN	 			:
					case APPUPDATE_XFSMSTATE_END	 			:	break;	
					
				}												
		}
	 else //  New event
	  {
			if(GetEvent()<APPUPDATE_LASTEVENT)
				{
					CheckTransition();

					switch(GetCurrentState())
						{							
							case APPUPDATE_XFSMSTATE_INI	 	: { XPATH															xpath;
																									DIOAPPLICATIONUPDATEVERSIONDATA		applicationversiondata;
																									XDWORD														nfiles;
																									XDWORD														sizetotal;
																									bool															status;

																									if(GetApplicationParam(xpath, applicationversiondata))
																										{	
																											xconsole->Printf(__L("   Version        : %d.\n"), applicationversiondata.GetVersion()); 								
																											xconsole->Printf(__L("   Subversion     : %d.\n"), applicationversiondata.GetSubVersion()); 								
																											xconsole->Printf(__L("   Error Control  : %d.\n"), applicationversiondata.GetErrorControl()); 								
																											xconsole->Printf(__L("   Ini Sistema    : %s.\n"), applicationversiondata.SystemMustBeInit()?__L("Si"):__L("No")); 								
																											xconsole->Printf(__L("\n")); 								
																											xconsole->Printf(__L("   Creando %s en [%s] ...\n"), DIOAPPLICATIONUPDATE_INIFILE, xpath.Get()); 						
																											status = GenerateUpdateFile(xpath, applicationversiondata, nfiles, sizetotal);																											
																											xconsole->Printf(__L("   Fichero de actualizacion: %s\n"), status?__L("Ok."):__L("Error!")); 
																											xconsole->Printf(__L("   Numero de ficheros      : %d\n"), nfiles);
																											xconsole->Printf(__L("   Tamano de actualizacion : %dk\n"), sizetotal /1024);
																										}
																								}
								
																								xconsole->Printf(__L("\n"));

																								//WaitKey(__L("Pulsa una tecla para continuar... (%d)"), 1, true, 5);

																								SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER); 
																								SetEvent(APPUPDATE_XFSMEVENT_END);																												

							case APPUPDATE_XFSMSTATE_RUN	 	: break;		

							case APPUPDATE_XFSMSTATE_END		: break;
						}
				}
		}

	return true;
}




/*-------------------------------------------------------------------
//  APPUPDATE::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/05/2012 19:02:07
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::End()
{	
  XSTRING string;
	XSTRING stringresult;

	//--------------------------------------------------------------------------------------

	if(cfg)
		{
			if(cfg->Log_IsActive())
				{
					switch(GetExitStatus())
						{
							case XAPPLICATIONEXITTYPE_NONE		    : XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR		, APPUPDATECFG_LOGSECTIONIDGENERIC, false, __L("No informacion de cierre."));																break;
							case XAPPLICATIONEXITTYPE_APPERROR		: XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR		, APPUPDATECFG_LOGSECTIONIDGENERIC, false, __L("Aplicacion cerrada por un error grave."));									break;	
							case XAPPLICATIONEXITTYPE_APPEND			: XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO			, APPUPDATECFG_LOGSECTIONIDGENERIC, false, __L("Aplicacion terminada."));																		break;
							case XAPPLICATIONEXITTYPE_BYUSER			: XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO			, APPUPDATECFG_LOGSECTIONIDGENERIC, false, __L("Aplicacion cerrada por el usuario."));											break;
							case XAPPLICATIONEXITTYPE_SOSHUTDOWN	: XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING	, APPUPDATECFG_LOGSECTIONIDGENERIC, false, __L("Aplicacion cerrada por apagado del sistema operativo."));		break;	
						}
				}
		}
	

	//--------------------------------------------------------------------------------------

	xconsole->PrintMessage(__L(" "),0,false,true);	
	
	//--------------------------------------------------------------------------------------	

	if(xscheduler)
		{
			string.Format(APPUPDATE_MESSAGEMASK,__L("Eliminando temporizador"));  
			xconsole->PrintMessage(string.Get(),1,true,false);		
			
			UnSubscribeEvent(XEVENTTYPE_SCHEDULER, xscheduler);
					
			xscheduler->End();

			delete xscheduler;
			xscheduler = NULL;

			stringresult = __L("Ok.");			
			xconsole->PrintMessage(stringresult.Get(), 0, false, true);				
			XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDENDING, false, __L("%s: %s")	, string.Get(), stringresult.Get());						

		}

	//--------------------------------------------------------------------------------------
		
  SetCurrentState(APPUPDATE_XFSMSTATE_END);


	//--------------------------------------------------------------------------------------
	
	#ifdef DIOALERTS_ACTIVE

	string.Format(APPUPDATE_MESSAGEMASK,__L("Desactivando alertas"));  
	xconsole->PrintMessage(string.Get(),1,true,false);		
	
	DIOALERTS::GetInstance().End();

	stringresult = __L("Ok.");			
	xconsole->PrintMessage(stringresult.Get(), 0, false, true);				
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDENDING, false, __L("%s: %s")	, string.Get(), stringresult.Get());			
	
	#endif

	//--------------------------------------------------------------------------------------

	string.Format(APPUPDATE_MESSAGEMASK,__L("Desactivando chequeo conexiones"));  
	xconsole->PrintMessage(string.Get(),1,true,false);
	
	if(checkinternetconnexion)
		{
 			delete checkinternetconnexion;
			checkinternetconnexion = NULL;
		}
	
		
	if(checkconnexions)
		{
			checkconnexions->End();

			delete checkconnexions;
			checkconnexions = NULL;
		}


	stringresult = __L("Ok.");						
	xconsole->PrintMessage(stringresult.Get(), 0, false, true);			
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDENDING, false, __L("%s: %s")	, string.Get(), stringresult.Get());	


	//--------------------------------------------------------------------------------------

	if(publicip)
		{
			delete publicip;			
			publicip = NULL;
		}

	if(geolocationip)
		{
			delete geolocationip;
			geolocationip = NULL;
		}		

	//--------------------------------------------------------------------------------------
		
	if(cfg)
		{
			if(cfg->Log_IsActive())
				{
					string.Format(APPUPDATE_MESSAGEMASK,__L("Desactivando servicio de LOG"));  
					xconsole->PrintMessage(string.Get(),1,true,false);			
					
					XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDENDING, false, __L("Desactivando servicio de LOG ..."));				
					XLOG::GetInstance().End();														

					xconsole->PrintMessage(__L("Ok."), 0, false, true);											
				}
		}

			
	//--------------------------------------------------------------------------------------

	if(cfg)
		{			
			string.Format(APPUPDATE_MESSAGEMASK,__L("Descargando configuracion"));  
			xconsole->PrintMessage(string.Get(),1,true,false);							
			
			cfg->End();
			delete cfg;
			cfg = NULL;

			xconsole->PrintMessage(__L("Ok."), 0, false, true);				
		}

	//--------------------------------------------------------------------------------------

	if(xdatetime) 
		{
			xfactory->DeleteDateTime(xdatetime);
			xdatetime = NULL;
		}

	if(xtimerupdate)
		{	
			xfactory->DeleteTimer(xtimerupdate);
			xtimerupdate = NULL;
		}	

	if(xtimer)
		{	
			xfactory->DeleteTimer(xtimer);
			xtimer = NULL;
		}	

	//--------------------------------------------------------------------------------------
	
	xconsole->PrintMessage(__L("Applicacion cerrada."), 1 ,true, true);	
	xconsole->PrintMessage(__L(""),0, false, true);

	if(xconsole) 
		{
			xfactory->DeleteConsole(xconsole);
			xconsole = NULL;
		}

	
	return true;
}



	

/*-------------------------------------------------------------------
//  APPUPDATE::ShowHeader
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/02/2014 13:10:20
//	
//	@return 			bool : 
//	@param				separator : 
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::ShowHeader(bool separator)
{	
	XSTRING header;

  if(!xconsole->TipicalHeader_Create(2013, APPUPDATE_NAMEAPP, APPUPDATE_VERSION, APPUPDATE_SUBVERSION,APPUPDATE_SUBVERSIONERR,APPUPDATE_ENTERPRISE,header)) return false;	

	xconsole->Printf(header.Get());
	xconsole->Printf(__L("\n"));
	if(separator) xconsole->Printf(__L("\n"));

	return true;
}




/*-------------------------------------------------------------------
//  APPUPDATE::ShowLine
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/01/2014 14:11:47
//	
//	@return 			bool : 
//  @param				string : 
//  @param				string2 : 
//  @param				dolog : 
//  @param				tab : 
//  @param				lf : 
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::ShowLine(XSTRING& string, XSTRING& string2, bool dolog, int tab, bool lf)
{
	XSTRING line1;
	XSTRING line2;

	XSTRING linelog;

	xconsole->FormatMessage(string.Get(), tab , false, false, line1);
	if(tab) 
		{
			int _tab = tab;

			if(_tab<37) _tab = 37;						
			line1.AdjustSize(_tab, false, __L(" "));
		}
			
	xconsole->FormatMessage(string2.Get(), 0 , false, lf, line2);					

	xconsole->Print(line1.Get());
	xconsole->Print(line2.Get());  	

	if(dolog)
		{
			linelog  = line1;
			linelog += line2;

			linelog.DeleteCharacter(__C(' '), XSTRINGCONTEXT_ATFIRST);
			linelog.DeleteCharacter(__C('\n'));
			XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDGENERIC, false, linelog.Get());			
		}	
	
	return true;
}





/*-------------------------------------------------------------------
//  APPUPDATE::ShowConfig
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/03/2014 9:48:07
//	
//	@return 			bool : 
//  @param				dolog : 
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::ShowConfig(bool dolog)
{
	if(!xfactory)	return false;
	
  XSTRING string;  
	XSTRING string2;  
	int			typecfgstation = 0;
	
	xconsole->Printf(__L("\n"));
		
	string  = __L("Log de la aplicacion");							
	string2.Format(__L("%s."), cfg->Log_IsActive()?__L("Activado"):__L("Desactivado"));		
	ShowLine(string, string2);	
  		
	return true;
}




/*-------------------------------------------------------------------
//  APPUPDATE::ShowAppStatus
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/02/2014 10:57:53
//	
//	@return 			bool : 
//  @param				dolog : 
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::ShowAppStatus(bool dolog)
{
	if(!xfactory)				return false;
	
  XSTRING string;  
	XSTRING string2;  

	xconsole->Printf(__L("\n"));
			
	XDWORD	total;
	XDWORD	free;
	
	xsystem->GetMemoryInfo(total,free);

	string  = __L("Memoria total");
	string2.Format(__L("%d Kb, libre %d Kb (el %d%%%%)."), total, free, xsystem->GetMemoryFreePercentAvailable());
	ShowLine(string, string2, dolog);

	if(xtimerglobal)
		{
			string  = __L("Tiempo de funcionamiento");
			xtimerglobal->GetMeasureString(string2, true);	
			ShowLine(string, string2, dolog);
		}

	return true;
}




/*-------------------------------------------------------------------
//  APPUPDATE::ShowInternetStatus
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/01/2014 14:14:58
//	
//	@return 			bool : 
//	@param				webHTMLpage : 
//  @param				dolog : 
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::ShowInternetStatus(bool dolog)
{
	if(!xfactory)				return false;
	if(!publicip)				return false;
	
  XSTRING string;  
	XSTRING string2;  
	
	string.Format(__L("Conexion con Internet"),  haveinternet?__L("Si"):__L("No"));
	string2.Format(__L("%s. "),  haveinternet?__L("Si"):__L("No"));
	ShowLine(string, string2, dolog);	
	
	if(haveinternet)
		{					
			DIOIP ip;
						
			if(publicip->Get(ip, 5)) 
				{	
					XSTRING IPstring;

					ip.GetXString(IPstring);

					string.Format(__L("Public IP"));	
					string2.Format(__L("[%s]"), IPstring.Get());	
					ShowLine(string, string2, dolog);							

				} else xconsole->PrintMessage(__L(""), 0, false, true);			
		}
	
	return true;
}



/*-------------------------------------------------------------------
//	APPUPDATE::GetApplicationParam
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/06/2015 11:53:27
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				applicationversiondata : 
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::GetApplicationParam(XPATH& xpath, DIOAPPLICATIONUPDATEVERSIONDATA& applicationversiondata)
{		
	typedef struct
	{
		XCHAR cmd;
		int   code;

	} COMANDINLINE;

	//-p "D:\CasinoServer\resources\web\applicationupdate\JackPotViewer\PC-windows" -v 1 -s 1 -e 0 -b yes

	XSTRING				actualcmd;
	COMANDINLINE	commandinline[] = { { __C('p'), 1 },  
																		{ __C('v'), 2 },   	
																		{ __C('s'), 3 },  
																		{ __C('e'), 4 },  
																		{ __C('b'), 5 }
																	};
	int           codecmd;
	XDWORD				c = 0;							

	xpath.Empty();
	applicationversiondata.Clean();

	if(!GetExecParams()) return false;

	do{ codecmd = -1;
		
			if(!GetExecParams()->Get(c))				return false;
			if(!GetExecParams()->Get(c)->Get()) return false;

			actualcmd = GetExecParams()->Get(c)->Get(); 			
			actualcmd.ToLowerCase();
			actualcmd.DeleteCharacter(__C(' '));

			if(actualcmd.Get()[0]==__C('-'))
				{
					for(int d=0; d<sizeof(commandinline)/sizeof(COMANDINLINE); d++)
						{
							if(commandinline[d].cmd == actualcmd.Get()[1]) 
								{
									codecmd = commandinline[d].code;
									break;
								}
						}
			
					if(codecmd!=-1)
						{
							c++;

							switch(codecmd)
								{
									case 1: xpath = GetExecParams()->Get(c)->Get();																													break;
									case 2: applicationversiondata.SetVersion(GetExecParams()->Get(c)->ConvertToInt());											break;
									case 3: applicationversiondata.SetSubVersion(GetExecParams()->Get(c)->ConvertToInt());									break; 
									case 4: applicationversiondata.SetErrorControl(GetExecParams()->Get(c)->ConvertToInt());								break;			
									case 5: applicationversiondata.SetSystemMustBeInit(GetExecParams()->Get(c)->ConvertToBoolean());		break;			
								}

						} else return false;					

				} else return false;							
			
			c++;

		} while(c < GetExecParams()->GetSize());

	return true;
}





/*-------------------------------------------------------------------
//	APPUPDATE::CreateListOfFiles
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			09/06/2015 14:10:32
//	
//	@return 			bool : 
//
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::CreateListOfFiles(XPATH& xpath, XVECTOR<XPATH*>* updatefiles)
{
	if(!xfactory) return false;

	XDIR* xdir = xfactory->Create_Dir();
  if(!xdir)  return false;
		
	XDIRELEMENT xdirelement;

	if(xdir->FirstSearch(xpath, __L("*") ,&xdirelement))
		{	
			do{ if(xdirelement.GetNameFile()->Compare(DIOAPPLICATIONUPDATE_INIFILE, true))
						{
							XPATH* xpathnext = new XPATH();
							if(xpathnext)
								{												
									xpathnext->Set(xpath);
									xpathnext->Slash_Add();
									xpathnext->Add(xdirelement.GetNameFile()->Get());		
																	
									if(xdirelement.GetType()==XDIRELEMENTTYPE_DIR)
										{														
											CreateListOfFiles((*xpathnext), updatefiles);
											delete xpathnext;
										}
									 else updatefiles->Add(xpathnext);						                																					
								} 
						} 
				} while(xdir->NextSearch(&xdirelement));
		}
  
	xfactory->Delete_Dir(xdir);

  return true;
}





/*-------------------------------------------------------------------
//	APPUPDATE::GenerateUpdateFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/06/2015 12:51:14
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				applicationversiondata : 
//  @param				nfiles : 
//  @param				sizetotal : 
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::GenerateUpdateFile(XPATH& xpath, DIOAPPLICATIONUPDATEVERSIONDATA& applicationversiondata, XDWORD& nfiles, XDWORD& sizetotal)
{
	XVECTOR<XPATH*>	updatefiles;
	XPATH           xpathupdatefile;
	bool						status = false;

	nfiles		= 0;
	sizetotal = 0;

	CreateListOfFiles(xpath, &updatefiles);

	xpathupdatefile = xpath;
	xpathupdatefile.Slash_Add();
	xpathupdatefile += DIOAPPLICATIONUPDATE_INIFILE;

  XFILEINI xfileini;
	if(xfileini.Create(xpathupdatefile))
	  {
			XSTRING key;
			XSTRING value;

			value.ConvertFromInt(applicationversiondata.GetVersion());									xfileini.WriteValue(DIOAPPLICATIONUPDATE_GENERALSECTION, DIOAPPLICATIONUPDATE_GENERALSECTION_VERSION						, value);
			value.ConvertFromInt(applicationversiondata.GetSubVersion());								xfileini.WriteValue(DIOAPPLICATIONUPDATE_GENERALSECTION, DIOAPPLICATIONUPDATE_GENERALSECTION_SUBVERSION					, value);
			value.ConvertFromInt(applicationversiondata.GetErrorControl());							xfileini.WriteValue(DIOAPPLICATIONUPDATE_GENERALSECTION, DIOAPPLICATIONUPDATE_GENERALSECTION_ERRORCONTROL				, value);
			value.ConvertFromBoolean(applicationversiondata.SystemMustBeInit());			  xfileini.WriteValue(DIOAPPLICATIONUPDATE_GENERALSECTION, DIOAPPLICATIONUPDATE_GENERALSECTION_SYSTEMMUSTBEINIT		, value);

			HASHCRC32 hashcrc32;

			for(XDWORD c=0; c<updatefiles.GetSize(); c++)
				{
					XPATH* xpathfile = updatefiles.Get(c);
					if(xpathfile)
						{
							int size = 0;

							hashcrc32.ResetResult();					
							hashcrc32.Do((*xpathfile));

							XFILE* xfile = xfactory->Create_File();
							if(xfile) 
								{
									if(xfile->Open(xpathfile->Get()))
										{
											size = xfile->GetSize();
											xfile->Close();
										}

									xfactory->Delete_File(xfile);
								}

							nfiles++;
							sizetotal += size;

							XPATH* namefile = updatefiles.Get(c);
							if(namefile) 
								{
									namefile->DeleteCharacters(0, xpath.GetSize());
									if((namefile->Get()[0] == __C('/')) || (namefile->Get()[0] == __C('\\'))) namefile->DeleteCharacters(0, 1);
								}
								
							key.Format(__L("%s%d"), DIOAPPLICATIONUPDATE_FILESSECTION_FILES, c);
							value.Format(__L("%s,%d,%08X"), namefile->Get(), size, hashcrc32.GetResultCRC32());

							xfileini.WriteValue(DIOAPPLICATIONUPDATE_FILESSECTION, key, value);
						}
				}

			xfileini.Close();

			status =true;
		} 

	updatefiles.DeleteContents();
	updatefiles.DeleteAll();

	return status;
}






/*-------------------------------------------------------------------
//	APPUPDATE::CheckInternetStatus
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/10/2014 13:17:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::CheckInternetStatus()
{	
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDAPPSTATUS, false, __L("Chequeando estado de conexion a Internet ..."));
					
	haveinternet = checkinternetconnexion->Check();	

	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, APPUPDATECFG_LOGSECTIONIDAPPSTATUS, false, __L("Conexion a Internet: %s."), haveinternet?__L("Ok"):__L("Error!") );

	if(checkinternetconnexion->IsChangeConnexionStatus())
		{
			if(haveinternet)
				{
					DIOCHECKCONNEXIONCUT* connexioncut =  checkinternetconnexion->GetLastConnexionsCut();
					XSTRING					 		  measuretime;

					if(connexioncut)
						{
							connexioncut->GetMeasureTimeString(measuretime, true);																										
							XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING, APPUPDATECFG_LOGSECTIONIDAPPSTATUS, false, __L("Conexion con internet restaurada : %s de desconexion."), measuretime.Get());
						}
					 else
					  {													
							XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING, APPUPDATECFG_LOGSECTIONIDAPPSTATUS, false, __L("Conexion con internet efectuada ..."));
						}
				}
			 else
				{
					XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING, APPUPDATECFG_LOGSECTIONIDAPPSTATUS, false, __L("Conexion con internet perdida ..."));
				}
		}

	return true;

}




/*-------------------------------------------------------------------
//	APPUPDATE::CheckApplicationStatus
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/10/2014 13:19:18
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool APPUPDATE::CheckApplicationStatus()
{
	if(!xsystem)	return false;

	XDWORD total;
	XDWORD free;
					
	xsystem->GetMemoryInfo(total,free);
							
	if(xsystem->GetMemoryFreePercentAvailable() < cfg->CheckMemStatus_GetLimitPercent()) 
		{							
			XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING, APPUPDATECFG_LOGSECTIONIDAPPSTATUS, false, __L("Memoria libre escasa : Total %d Kb, Libre %d Kb (%d%%). "), total, free,  xsystem->GetMemoryFreePercentAvailable());					

			#ifdef LINUX
			GetXSystem()->MakeCommand(__L("sync && sysctl -w vm.drop_caches=3"));
			#endif
		}

	return true;
}




/*-------------------------------------------------------------------
//   APPUPDATE::HandleEvent
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/06/2011 22:28:08
//	
//	@return				void : 
//	@param				xevent : 
*/
/*-----------------------------------------------------------------*/
void APPUPDATE::HandleEvent(XEVENT* xevent)
{
	if(!xevent) return;
	
	switch(xevent->GetEventType())
		{				
			case XEVENTTYPE_SCHEDULER								: { XSCHEDULERXEVENT* event = (XSCHEDULERXEVENT*)xevent;
																									if(!event) return;
																									
																									switch(event->GetTask()->GetID())
																										{
																											case APPUPDATETASKID_CHECKINTERNETCONNEXION	: CheckInternetStatus();
																																																		break;

																											case APPUPDATETASKID_CHECKMEMORYSTATUS			: CheckApplicationStatus();
																																																		break;																											

																										}																																																																																																					
																								}	
																								break;
		}	
}


