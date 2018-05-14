/*------------------------------------------------------------------------------------------
//	APPUPDATECFG.H
*/	
/**	
// \class 
//   
//  DNS Managerm Config class
//   
//	@author	 Abraham J. Velez
//	@version 13/11/2013 16:39:42
*/	
/*	EndoraSoft (C) Copyright. All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _APPUPDATECFG_H_
#define _APPUPDATECFG_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XFileConfig.h"

#include "APPUpdate.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


#define APPUPDATECFG_SECTIONGENERAL											__L("general")
#define APPUPDATECFG_CHECKINTERNETSTATUS								__L("checkinternetstatus") 
#define APPUPDATECFG_CHECKINTERNETSTATUSCADENCE					__L("checkinternetstatuscadence") 
#define APPUPDATECFG_CHECKMEMSTATUS											__L("checkmemstatus") 
#define APPUPDATECFG_CHECKMEMSTATUSCADENCE							__L("checkmemstatuscadence") 
#define APPUPDATECFG_CHECKMEMSTATUSLIMITPERCENT					__L("checkmemstatuslimitpercent") 

#define APPUPDATECFG_SECTIONEMAILALERT									__L("emailalert")
#define APPUPDATECFG_EMAILALERTSMTPURL									__L("smtpurl")
#define APPUPDATECFG_EMAILALERTSMTPPORT									__L("smtpport")
#define APPUPDATECFG_EMAILALERTSMTPLOGIN								__L("smtplogin")
#define APPUPDATECFG_EMAILALERTSMTPPASSWORD							__L("smtppassword")
#define APPUPDATECFG_EMAILALERTEMAILSENDER							__L("emailsender")
#define APPUPDATECFG_EMAILALERTEMAILRECIPIENT						__L("recipient")
#define APPUPDATECFG_MAXEMAILALERTRECIPIENTS						10

#define APPUPDATECFG_SECTIONLOG													__L("log")
#define APPUPDATECFG_LOGISACTIVE												__L("isactive") 
#define APPUPDATECFG_LOGBACKUPISACTIVE									__L("backupisactive") 
#define APPUPDATECFG_LOGBACKUPMAXFILES									__L("backupmaxfiles") 
#define APPUPDATECFG_LOGBACKUPISCOMPRESS								__L("backupiscompress") 
#define APPUPDATECFG_LOGACTIVESECTIONSID								__L("activesectionsID")
#define APPUPDATECFG_LOGLEVELMASK												__L("levelmask")
#define APPUPDATECFG_LOGMAXSIZE													__L("maxsize")
#define APPUPDATECFG_LOGREDUCTIONPERCENT								__L("reductionpercent")

#define APPUPDATECFG_LOGSECTIONIDINITIATION							__L("Ini")
#define APPUPDATECFG_LOGSECTIONIDGENERIC								__L("General")	
#define APPUPDATECFG_LOGSECTIONIDAPPSTATUS							__L("Status")
#define APPUPDATECFG_LOGSECTIONIDENDING									__L("End")



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPATHS;
class XSYSTEM;




class APPUPDATECFG : public XFILECONFIG
{
	public:																									
																	  APPUPDATECFG														(XCHAR* namefile) : XFILECONFIG(namefile)
																		{
																			XSTRING section;
																			XSTRING key;

																			Clean();

																			//-----------------------------------------------------
																			// GENERAL	
																			AddValue(XFILECONFIGVALUETYPE_BOOLEAN , APPUPDATECFG_SECTIONGENERAL			, APPUPDATECFG_CHECKINTERNETSTATUS								, &checkinternetstatus);
																			AddValue(XFILECONFIGVALUETYPE_INT		  , APPUPDATECFG_SECTIONGENERAL			, APPUPDATECFG_CHECKINTERNETSTATUSCADENCE					, &checkinternetstatuscadence);
																			AddValue(XFILECONFIGVALUETYPE_BOOLEAN , APPUPDATECFG_SECTIONGENERAL			, APPUPDATECFG_CHECKMEMSTATUS											, &checkmemstatus);
																			AddValue(XFILECONFIGVALUETYPE_INT		  , APPUPDATECFG_SECTIONGENERAL			, APPUPDATECFG_CHECKMEMSTATUSCADENCE							, &checkmemstatuscadence);
																			AddValue(XFILECONFIGVALUETYPE_INT		  , APPUPDATECFG_SECTIONGENERAL			, APPUPDATECFG_CHECKMEMSTATUSLIMITPERCENT					, &checkmemstatuslimitpercent);	
																			



																			//-----------------------------------------------------
																			// EMAIL ALERT
																			AddValue(XFILECONFIGVALUETYPE_STRING	, APPUPDATECFG_SECTIONEMAILALERT	, APPUPDATECFG_EMAILALERTSMTPURL									, &emailalertSMTPURL);
																			AddValue(XFILECONFIGVALUETYPE_INT			, APPUPDATECFG_SECTIONEMAILALERT	, APPUPDATECFG_EMAILALERTSMTPPORT									, &emailalertSMTPport);
																			AddValue(XFILECONFIGVALUETYPE_STRING	, APPUPDATECFG_SECTIONEMAILALERT	, APPUPDATECFG_EMAILALERTSMTPLOGIN								, &emailalertSMTPlogin);
																			AddValue(XFILECONFIGVALUETYPE_STRING	, APPUPDATECFG_SECTIONEMAILALERT	, APPUPDATECFG_EMAILALERTSMTPPASSWORD							, &emailalertSMTPpassword);
																			AddValue(XFILECONFIGVALUETYPE_STRING	, APPUPDATECFG_SECTIONEMAILALERT	, APPUPDATECFG_EMAILALERTEMAILSENDER							,	&emailalertemailsender);
																			
																			for(int c=0; c<APPUPDATECFG_MAXEMAILALERTRECIPIENTS; c++)
																				{																					
																					key.Format(__L("%s%02d"), APPUPDATECFG_EMAILALERTEMAILRECIPIENT, c+1);
																					AddValue(XFILECONFIGVALUETYPE_STRING, APPUPDATECFG_SECTIONEMAILALERT, key.Get(),	&emailalertrecipient[c]);
																				}	


																			//-----------------------------------------------------
																			// LOG
																			AddValue(XFILECONFIGVALUETYPE_BOOLEAN , APPUPDATECFG_SECTIONLOG					, APPUPDATECFG_LOGISACTIVE												, &logisactive);	
																			AddValue(XFILECONFIGVALUETYPE_BOOLEAN , APPUPDATECFG_SECTIONLOG					, APPUPDATECFG_LOGBACKUPISACTIVE									, &logbackupisactive);	
																			AddValue(XFILECONFIGVALUETYPE_INT			, APPUPDATECFG_SECTIONLOG					, APPUPDATECFG_LOGBACKUPMAXFILES 									, &logbackupmaxfiles);
																			AddValue(XFILECONFIGVALUETYPE_BOOLEAN , APPUPDATECFG_SECTIONLOG					, APPUPDATECFG_LOGBACKUPISCOMPRESS								, &logbackupiscompress);	
																			AddValue(XFILECONFIGVALUETYPE_STRING  , APPUPDATECFG_SECTIONLOG					, APPUPDATECFG_LOGACTIVESECTIONSID								,	&logactivesectionsID);
																			AddValue(XFILECONFIGVALUETYPE_MASK		, APPUPDATECFG_SECTIONLOG					, APPUPDATECFG_LOGLEVELMASK												, &loglevelmask);	
																			AddValue(XFILECONFIGVALUETYPE_INT			, APPUPDATECFG_SECTIONLOG					, APPUPDATECFG_LOGMAXSIZE													, &logmaxsize);
																			AddValue(XFILECONFIGVALUETYPE_INT		  , APPUPDATECFG_SECTIONLOG					, APPUPDATECFG_LOGREDUCTIONPERCENT								, &logreductionpercent);
	
																			Ini();
																		}

		virtual												 ~APPUPDATECFG														()								
																		{ 
																			End(); 
																			Clean();																	
																		}
	
		
		bool														CheckInternetStatus_IsActive							()								{ return checkinternetstatus;								}
		int															CheckInternetStatus_GetCadence						()								{ return checkinternetstatuscadence;				}
		bool														CheckMemStatus_IsActive										()								{	return checkmemstatus;										}
		int															CheckMemStatus_GetCadence									()								{ return checkmemstatuscadence;							}
		int															CheckMemStatus_GetLimitPercent						()								{ return checkmemstatuslimitpercent;				}


		XSTRING*												EmailAlert_GetSMTPURL											()								{ return &emailalertSMTPURL;								}
		int															EmailAlert_GetSMTPPort										()								{ return emailalertSMTPport;								}
		XSTRING*												EmailAlert_GetSMTPLogin										()								{ return &emailalertSMTPlogin;							}
		XSTRING*												EmailAlert_GetSMTPPassword								()								{ return &emailalertSMTPpassword;						}
		XSTRING*												EmailAlert_GetEmailSender									()								{ return &emailalertemailsender;            }
		XSTRING*												EmailAlert_GetRecipient										(int index)								
																		{ 
																			if(index<0)																					return NULL;
																			if(index>=APPUPDATECFG_MAXEMAILALERTRECIPIENTS)	return NULL;

																			return &emailalertrecipient[index];		
																		}
		
		bool														Log_IsActive															()								{ return logisactive;												}		
		bool														Log_Backup_IsActive												()								{ return logbackupisactive;									}
		int															Log_Backup_GetMaxFiles										()								{ return logbackupmaxfiles;									}
		bool														Log_Backup_IsCompress											()								{ return logbackupiscompress;								}
		XSTRING*												Log_ActiveSectionsID											()								{ return &logactivesectionsID;							}
		XBYTE														Log_LevelMask															()								{ return (XBYTE)loglevelmask;								}			
		int															Log_MaxSize																()								{	return logmaxsize;												}	
		int															Log_ReductionPercent											()								{ return logreductionpercent;								}
				
		virtual bool										Default																		()
																		{
																			checkinternetstatus										= true;
																			checkinternetstatuscadence						= 30;
																			checkmemstatus												= true;
																			checkmemstatuscadence									= 60;
																			checkmemstatuslimitpercent						= 10;
				
																			emailalertSMTPURL											= __L("mail.pictel.es");
																			emailalertSMTPport										= 587;
																			emailalertSMTPlogin										=	__L("appalert");
																			emailalertSMTPpassword								= __L("prometeus1");
																			emailalertemailsender									= __L("appalert@innomatic.es");																			
																			emailalertrecipient[0]								= __L("1,Abraham,ajvelez@endorasoft.com");													

																			logisactive														= true;																			
																			logbackupisactive											= true;
																			logbackupmaxfiles											= 10;
																			logbackupiscompress										= true;																			
																			logactivesectionsID.Empty();														
																			logactivesectionsID += APPUPDATECFG_LOGSECTIONIDINITIATION;
																			logactivesectionsID += __L(",");
																			logactivesectionsID += APPUPDATECFG_LOGSECTIONIDGENERIC; 	
																			logactivesectionsID += __L(",");
																			logactivesectionsID += APPUPDATECFG_LOGSECTIONIDAPPSTATUS;
																			logactivesectionsID += __L(",");
																			logactivesectionsID += APPUPDATECFG_LOGSECTIONIDENDING;
																			loglevelmask													= XLOGLEVEL_ALL;
																			logmaxsize														= 3000;
																			logreductionpercent										= 10;
																				
			
																			return true;
																		}
		
	private:

		void														Clean																			()
																		{		
																			checkinternetstatus										= false;
																			checkinternetstatuscadence						= 0;
																			checkmemstatus												= false;
																			checkmemstatuscadence									= 0;
																			checkmemstatuslimitpercent						= 0;

																			
																			emailalertSMTPport										= 0;
																			for(int c=0; c<APPUPDATECFG_MAXEMAILALERTRECIPIENTS; c++)
																				{	
																					emailalertrecipient[c].Empty();
																				}
	
																			logisactive														= false;
																			logbackupisactive											= false;
																			logbackupmaxfiles											= 0;
																			logbackupiscompress										= false;
																			logactivesectionsID.Empty();
																			loglevelmask													= 0;
																			logmaxsize														= 0;
																			logreductionpercent										= 0;

																		}
		
		bool														checkinternetstatus;
		int															checkinternetstatuscadence;
		bool														checkmemstatus;
		int															checkmemstatuscadence;
		int															checkmemstatuslimitpercent;

		XSTRING													emailalertSMTPURL;
		int															emailalertSMTPport;
	  XSTRING													emailalertSMTPlogin;
		XSTRING													emailalertSMTPpassword;
		XSTRING													emailalertemailsender;
		XSTRING													emailalertrecipient[APPUPDATECFG_MAXEMAILALERTRECIPIENTS];
	
		bool														logisactive;
		bool														logbackupisactive;
		int															logbackupmaxfiles;
		bool														logbackupiscompress;
		XSTRING													logactivesectionsID;
		int															loglevelmask;
		int															logmaxsize;
		int															logreductionpercent;
};
	
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

