/*------------------------------------------------------------------------------------------
//	DIOALERTS.H
*/	
/**	
// \class 
//   
//  Data IO Alerts
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 16/01/2015 8:43:34
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOALERT_H_
#define _DIOALERT_H_
	
	
#ifdef DIOALERTS_ACTIVE


/*---- INCLUDES --------------------------------------------------------------------------*/


#include "XVector.h"
#include "XString.h"
#include "XDateTime.h"
#include "XFactory.h"

#include "DIOFactory.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum DIOALERTLEVEL
{
	DIOALERTLEVEL_UNKNOWN								= 0			,
	DIOALERTLEVEL_INFO													,
	DIOALERTLEVEL_WARNING												,
	DIOALERTLEVEL_SERIOUS												,
	DIOALERTLEVEL_DANGER												,	
};

enum DIOALERTTYPE
{
	DIOALERTTYPE_UNKNOWN								= 0			,
	DIOALERTTYPE_ERRORAPP												,
	DIOALERTTYPE_OWNAPPLICATION					= 100	  ,
};


enum DIOALERTSENDER
{
	DIOALERTSSENDER_SMPT								=  0x01	,
	DIOALERTSSENDER_SMS									=  0x02	,
	DIOALERTSSENDER_WEB                 =  0x04 ,
	DIOALERTSSENDER_UDP									=  0x08	,

	DIOALERTSSENDER_ALL									=  (DIOALERTSSENDER_SMPT	| DIOALERTSSENDER_SMS | DIOALERTSSENDER_WEB | DIOALERTSSENDER_UDP),
};


#define	DIOALERTS_QSPARAM_APPLICATIONID		__L("applicationID")
#define	DIOALERTS_QSPARAM_TYPE						__L("type")	
#define	DIOALERTS_QSPARAM_LEVEL						__L("level")	
#define	DIOALERTS_QSPARAM_TITTLE					__L("tittle")	
#define	DIOALERTS_QSPARAM_ORIGIN					__L("origin")	
#define	DIOALERTS_QSPARAM_MESSAGE					__L("message")

/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XDATETIME;
class XPUBLISHER;
class XTHREADCOLLECTED;
class DIOFACTORY;
class DIOSTREAMTCPIPCONFIG;
class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDPACKNOWLEDGE;
class DIOIP;
class DIOSTREAM;
class DIOSMTP;
class DIOWEBCLIENT;
class DIOATCMDGSM;



class DIOALERT
{
	public:
																DIOALERT								()												
																{
																	Clean();																
																	xdatetime = xfactory->CreateDateTime();																	
																}

		virtual										 ~DIOALERT								()												
																{ 																	
																	if(xdatetime) xfactory->DeleteDateTime(xdatetime);																	
																	Clean();																
																}

		XDATETIME*									GetDateTime							()															{ return xdatetime;                       }

		XSTRING*										GetApplicationID				()															{ return &applicationID;									}	
		bool                        GetApplicationVersion   (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)                                                       
																{
																	version				= applicationversion;
																	subversion		= applicationsubversion;
																	subversionerr	= applicationsubversionerr;
																	return true;
																}
		
		bool                        SetApplicationVersion   (XDWORD version, XDWORD subversion, XDWORD subversionerr)                                                       
																{
																	applicationversion				= version;
																	applicationsubversion			= subversion;
																	applicationsubversionerr	= subversionerr;
																	return true;
																}

		XDWORD											GetID                   ()															{ return ID;                              }
		void												SetID										(XDWORD ID)											{ this->ID = ID;													}
		XDWORD											CalculateID							(bool withdatetime = true);

		XDWORD											GetType									()															{ return type;														}
		void												SetType									(XDWORD type)										{ this->type = type;											}
		
		DIOALERTLEVEL								GetLevel								()															{ return level;														}
		void												SetLevel								(DIOALERTLEVEL level)						{ this->level=level;											}
		
		XSTRING*										GetOrigin								()															{ return &origin;													}	
		XSTRING*										GetTitle								()															{ return &title;                          }
		XSTRING*										Get_Message							()															{ return &message;												}

		bool												CopyFrom                (DIOALERT* alert)
																{
																	if(!alert) return false;

																	xdatetime->CopyFrom(alert->GetDateTime());
																	
																	applicationID	= alert->GetApplicationID()->Get();
																	
																	alert->GetApplicationVersion(applicationversion, applicationsubversion, applicationsubversionerr);
																	
																	ID						= alert->GetID();
																	type					= alert->GetType();
																	level					= alert->GetLevel();		
																	origin				= alert->GetOrigin()->Get();
																	title					= alert->GetTitle()->Get();	
																	message				= alert->Get_Message()->Get();

																	return true;
																}

	private:

		void												Clean										()         
																{
																	applicationversion				= 0;
																	applicationsubversion			= 0;
																	applicationsubversionerr	= 0;

																	xdatetime									= NULL; 																	
																	ID												= 0;
																	type											= 0;
																	level											= DIOALERTLEVEL_UNKNOWN;
																}

		XDATETIME*									xdatetime;
		XSTRING	                    applicationID;
		XDWORD                      applicationversion;
		XDWORD                      applicationsubversion;
		XDWORD                      applicationsubversionerr;
		XDWORD											ID;
		XDWORD											type;
		DIOALERTLEVEL								level;	
		XSTRING											origin;
		XSTRING											title;	
		XSTRING											message;
};





class DIOALERTS
{
	public:
		
		static DIOALERTS&						GetInstance							()
																{
																	if(!instance) instance = new DIOALERTS();
																						
																	return (*instance);	
																}						

		static bool									DelInstance							()
																{
																	if(instance)
																		{
																			delete instance;
																			instance = NULL;

																			return true;
																		} 
																				
																	return false;
																}						

		
		bool												Ini											();	
		
		XSTRING*										GetApplicationID				()																																																								{ return &applicationID;																																									}		
		bool                        GetApplicationVersion   (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)                                                       
																{
																	version				= applicationversion;
																	subversion		= applicationsubversion;
																	subversionerr	= applicationsubversionerr;
																	return true;
																}
		
		bool                        SetApplicationVersion   (XDWORD version, XDWORD subversion, XDWORD subversionerr)                                                       
																{
																	applicationversion				= version;
																	applicationsubversion			= subversion;
																	applicationsubversionerr	= subversionerr;
																	return true;
																}
		
		XSTRING*										GetOrigin								()																																																								{ return &origin;																																													}	

		DIOALERT*										CreateAlert							(XCHAR* applicationID, XDWORD type, DIOALERTLEVEL level, XCHAR* origin, XCHAR* title, XCHAR* message);
		DIOALERT*										CreateAlert							(XSTRING& applicationID, XDWORD type, DIOALERTLEVEL level, XSTRING& origin, XSTRING& title, XSTRING& message)			{ return CreateAlert(applicationID.Get(), type, level, origin.Get(), title.Get(), message.Get());					}
		DIOALERT*										CreateAlert							(XDWORD type, DIOALERTLEVEL level, XCHAR* title, XCHAR* message)																									{ return CreateAlert(applicationID.Get(), type, level, origin.Get(), title, message);											}
		DIOALERT*										CreateAlert							(XDWORD type, DIOALERTLEVEL level, XSTRING& title, XSTRING& message)																							{ return CreateAlert(applicationID.Get(), type, level, origin.Get(), title.Get(), message.Get());         }

		bool												Sender_SMTPConfig				(XCHAR* URL, int port, XCHAR* login, XCHAR* password, XCHAR* senderemail, int nrecipients, ...);
		bool												Sender_SMTPSend					(DIOALERT* alert);

		bool												Sender_SMSConfig				(DIOSTREAM* diostream, int nrecipients, ...);
		bool												Sender_SMSSend					(DIOALERT* alert);

		bool												Sender_WEBConfig				(XCHAR* command, bool withget, int nrecipients, ...);
		bool												Sender_WEBSend					(DIOALERT* alert);

		bool												Sender_UDPConfig				(int port, int nrecipients, ...);
		bool												Sender_UDPSend					(DIOALERT* alert);
	
		int													Send										(DIOALERTSENDER sender, DIOALERT* alert);

		bool												End											();


	private:
																DIOALERTS								()												
																{ 
																	Clean();		
																}	

																DIOALERTS								(DIOALERTS const&);				// Don't implement  

		virtual										 ~DIOALERTS								()												
																{ 
																	Clean();																
																}		
		
		void												operator =							(DIOALERTS const&);				// Don't implement	

		void												Clean										()         
																{
																	applicationversion				= 0;
																	applicationsubversion			= 0;
																	applicationsubversionerr	= 0;

																	SMTPsenderisactive				= false;	
																	SMTPdiostreamcfg					= NULL;
																	SMTPdiostream							= NULL;
																	SMTP											= NULL;				
																	
																	SMSsenderisactive					= false;	
																	SMSdiostream							= NULL;				

																	WEBsenderisactive					= false;	
																	WEBisuseget               = false;
																	WEBdiowebclient						= NULL;	
																	 
																	UDPsenderisactive					= false;	
																	UDPdiostreamcfg						= NULL;
																	UDPdiostream							= NULL;																	

																}

		static DIOALERTS*						instance;		

		XSTRING	                    applicationID;
		XDWORD                      applicationversion;
		XDWORD                      applicationsubversion;
		XDWORD                      applicationsubversionerr;
		XSTRING											origin;

		bool												SMTPsenderisactive;	
		DIOSTREAMTCPIPCONFIG*				SMTPdiostreamcfg;
		DIOSTREAM*									SMTPdiostream;
		DIOSMTP*										SMTP;				
		
		bool												SMSsenderisactive;	
		DIOSTREAM*									SMSdiostream;
		XVECTOR<XSTRING*>						SMSrecipients;

		XSTRING											WEBcommand;		
		bool												WEBsenderisactive;	
		bool												WEBisuseget;
		DIOWEBCLIENT*								WEBdiowebclient;	
		XVECTOR<DIOURL*>						WEBrecipients;

		bool												UDPsenderisactive;	
		DIOSTREAMUDPCONFIG*					UDPdiostreamcfg;
		DIOSTREAMUDPACKNOWLEDGE*		UDPdiostream;
		XVECTOR<DIOIP*>							UDPrecipients;
};
	



class DIOALERTSUDPSERVER
{
	public:
																DIOALERTSUDPSERVER			()												
																{ 
																	Clean();		
																}	

		virtual										 ~DIOALERTSUDPSERVER			()												
																{ 
																	End();

																	Clean();																
																}

		bool												Ini											(XCHAR* URL, int port);				
		bool												End											();		

		int													GetNAlerts			        ();

		DIOALERT*										GetAlertByIndex         (int index);
		DIOALERT*										GetAlertByID	          (XDWORD ID);

		bool												DeleteAlertByIndex      (int index);
		bool												DeleteAlertByID	        (XDWORD ID);

		bool												DeleteAllAlerts         ();

	private:

		void												Clean										()         
																{
																	 																																	
																	UDPdiostreamcfg			= NULL;
																	UDPdiostream				= NULL;																	

																	xmutexalert					= NULL;

																	threadread          = NULL;
																}

		static void									ThreadReadFunction			(void* param);	
		bool												ReceivedEvents					();
		
		DIOSTREAMUDPCONFIG*					UDPdiostreamcfg;
		DIOSTREAMUDPACKNOWLEDGE*		UDPdiostream;

		XMUTEX*											xmutexalert;
		XVECTOR<DIOALERT*>					alerts;

		XTHREADCOLLECTED*						threadread;
};



	

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


#endif

