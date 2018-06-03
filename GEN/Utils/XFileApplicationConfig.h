/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XFILEAPPLICATIONCONFIG.h
*
* @class       XFILEAPPLICATIONCONFIG
* @brief       POS Server Config class
* @ingroup     
*
* @author      Abraham J. Velez 
* @date        23/05/2018 10:31:15
*
* @note         if use defines to active for the blocks of the config.
*
*               XFILEAPPLICATIONCONFIG_REMOTEFILE_ACTIVE  : Active the remote file CFG
*               
*               Active the block of CFG 
*               XFILEAPPLICATIONCONFIG_GENERAL_ACTIVE     : Default general (debug trace, memory check, etc..
*               XFILEAPPLICATIONCONFIG_LOCATION_ACTIVE    : Location info (street, postal code, etc..)
*               XFILEAPPLICATIONCONFIG_APPUPDATE_ACTIVE   : Application update 
*               XFILEAPPLICATIONCONFIG_ALERTS_ACTIVE      : Alerts
*               XFILEAPPLICATIONCONFIG_LOG_ACTIVE         : Log 
*
* @copyright   Copyright(c) 2018 Versia Desarrollo SW, S.L.  All rights reserved.
*
*---------------------------------------------------------------------------------------------------------------------*/

#ifndef _XFILEAPPLICATIONCONFIG_H_
#define _XFILEAPPLICATIONCONFIG_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XDebugTrace.h"
#include "XFileConfig.h"

#ifdef XFILEAPPLICATIONCONFIG_REMOTEFILE_ACTIVE 
#include "DIORemoteFileConfig.h"
#endif

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#ifdef XFILEAPPLICATIONCONFIG_GENERAL_ACTIVE

#define XFILEAPPLICATIONCONFIG_SECTIONGENERAL													       __L("general")
#define XFILEAPPLICATIONCONFIG_CHECKMEMSTATUSCADENCE									       __L("checkmemstatuscadenceseconds") 
#define XFILEAPPLICATIONCONFIG_CHECKMEMSTATUSLIMITPERCENT							       __L("checkmemstatuslimitpercent") 
#define XFILEAPPLICATIONCONFIG_CHECKINTERNETSTATUSCADENCE							       __L("checkinternetstatuscadenceseconds") 
#define XFILEAPPLICATIONCONFIG_WEBSCRAPERSCRIPTURLDOWNLOAD                   __L("webscraperscripturldownload")
#define XFILEAPPLICATIONCONFIG_CHECKIPPUBLICCHANGECADENCE							       __L("checkippublicchangecadenceminutes") 
#define XFILEAPPLICATIONCONFIG_UPDATETIMEBYNTPCADENCE									       __L("updatetimebyntpcadencehours") 
#define XFILEAPPLICATIONCONFIG_SHOWDETAILINFO                                __L("showdetailinfo")
#define XFILEAPPLICATIONCONFIG_XDEBUGTARGET													       	 __L("xdebugtarget") 

#endif


#ifdef XFILEAPPLICATIONCONFIG_LOCATION_ACTIVE

#define XFILEAPPLICATIONCONFIG_SECTIONLOCATION                               __L("Location")
#define XFILEAPPLICATIONCONFIG_CENTERNAME                                    __L("name")
#define XFILEAPPLICATIONCONFIG_LOCATIONSTREET                                __L("street")
#define XFILEAPPLICATIONCONFIG_LOCATIONTOWN                                  __L("city")
#define XFILEAPPLICATIONCONFIG_LOCATIONSTATE                                 __L("state")
#define XFILEAPPLICATIONCONFIG_LOCATIONCOUNTRY                               __L("country")
#define XFILEAPPLICATIONCONFIG_LOCATIONPOSTALCODE                            __L("postalcode")

#endif


#ifdef XFILEAPPLICATIONCONFIG_APPUPDATE_ACTIVE

#define XFILEAPPLICATIONCONFIG_SECTIONAPPLICATIONUPDATE                      __L("applicationupdate")
#define XFILEAPPLICATIONCONFIG_APPLICATIONUPDATEISACTIVE                     __L("isactive")
#define XFILEAPPLICATIONCONFIG_APPLICATIONUPDATEURL                          __L("url")
#define XFILEAPPLICATIONCONFIG_APPLICATIONUPDATEPORT                         __L("port")
#define XFILEAPPLICATIONCONFIG_APPLICATIONUPDATECHECKCADENCE                 __L("checkcadenceminutes")
#define XFILEAPPLICATIONCONFIG_APPLICATIONUPDATECHECKTIME                    __L("checktime")

#endif


#ifdef XFILEAPPLICATIONCONFIG_DNSMANAGER_ACTIVE

#define XFILEAPPLICATIONCONFIG_SECTIONDNSMANAGER											       __L("dnsmanager")
#define XFILEAPPLICATIONCONFIG_DNSMANAGERURL													       __L("url")
#define XFILEAPPLICATIONCONFIG_DNSMANAGERMAXURL												       8

#endif


#ifdef XFILEAPPLICATIONCONFIG_WEBSERVER_ACTIVE

#define XFILEAPPLICATIONCONFIG_SECTIONWEBSERVER												       __L("webserver")
#define XFILEAPPLICATIONCONFIG_WEBSERVERLOCALADDR											       __L("localaddr")
#define XFILEAPPLICATIONCONFIG_WEBSERVERPORT													       __L("port")
#define XFILEAPPLICATIONCONFIG_WEBSERVERLOGIN   											       __L("login")
#define XFILEAPPLICATIONCONFIG_WEBSERVERPASSWORD											       __L("password")

#endif


#ifdef XFILEAPPLICATIONCONFIG_ALERTS_ACTIVE

#define XFILEAPPLICATIONCONFIG_SECTIONALERTS                                 __L("alerts")
#define XFILEAPPLICATIONCONFIG_ALERTSACTIVE                                  __L("isactive")
#define XFILEAPPLICATIONCONFIG_ALERTSSMTPURL                                 __L("smtpurl")
#define XFILEAPPLICATIONCONFIG_ALERTSSMTPPORT                                __L("smtpport")
#define XFILEAPPLICATIONCONFIG_ALERTSSMTPLOGIN                               __L("smtplogin")
#define XFILEAPPLICATIONCONFIG_ALERTSSMTPPASSWORD                            __L("smtppassword")
#define XFILEAPPLICATIONCONFIG_ALERTSSMTPSENDER                              __L("smtpsender")
#define XFILEAPPLICATIONCONFIG_ALERTSSMTPRECIPIENT                           __L("smtprecipient")
#define XFILEAPPLICATIONCONFIG_ALERTSSMSRECIPIENT                            __L("smsrecipient")
#define XFILEAPPLICATIONCONFIG_ALERTSWEBUISUSEGET                            __L("webisuseget")
#define XFILEAPPLICATIONCONFIG_ALERTSWEBRECIPIENT                            __L("webrecipient")
#define XFILEAPPLICATIONCONFIG_ALERTSUDPPORT                                 __L("udpport")
#define XFILEAPPLICATIONCONFIG_ALERTSUDPRECIPIENT                            __L("udprecipient")
#define XFILEAPPLICATIONCONFIG_ALERTSMAXSMTPRECIPIENTS                       10
#define XFILEAPPLICATIONCONFIG_ALERTSMAXSMSRECIPIENTS                        10
#define XFILEAPPLICATIONCONFIG_ALERTSMAXWEBRECIPIENTS                        10
#define XFILEAPPLICATIONCONFIG_ALERTSMAXUDPRECIPIENTS                        10

#endif


#ifdef XFILEAPPLICATIONCONFIG_LOG_ACTIVE

#define XFILEAPPLICATIONCONFIG_SECTIONLOG                                    __L("log")
#define XFILEAPPLICATIONCONFIG_LOGISACTIVE                                   __L("isactive")
#define XFILEAPPLICATIONCONFIG_LOGBACKUPISACTIVE                             __L("backupisactive")
#define XFILEAPPLICATIONCONFIG_LOGBACKUPMAXFILES                             __L("backupmaxfiles")
#define XFILEAPPLICATIONCONFIG_LOGBACKUPISCOMPRESS                           __L("backupiscompress")
#define XFILEAPPLICATIONCONFIG_LOGACTIVESECTIONSID                           __L("activesectionsID")
#define XFILEAPPLICATIONCONFIG_LOGLEVELMASK                                  __L("levelmask")
#define XFILEAPPLICATIONCONFIG_LOGMAXSIZE                                    __L("maxsize")
#define XFILEAPPLICATIONCONFIG_LOGREDUCTIONPERCENT                           __L("reductionpercent")

#define XFILEAPPLICATIONCONFIG_LOGSECTIONIDINITIATION                        __L("Ini")
#define XFILEAPPLICATIONCONFIG_LOGSECTIONIDGENERIC                           __L("General")
#define XFILEAPPLICATIONCONFIG_LOGSECTIONIDSTATUSAPP                         __L("Status")
#define XFILEAPPLICATIONCONFIG_LOGSECTIONIDENDING                            __L("End")

#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XPATHS;
class XSYSTEM;
class XLOG;


class XFILEAPPLICATIONCONFIG 
#ifdef XFILEAPPLICATIONCONFIG_REMOTEFILE_ACTIVE 
  : public DIOREMOTEFILECONFIG
#else
  : public XFILECONFIG
#endif
{
  public:
                         XFILEAPPLICATIONCONFIG                      (XCHAR* namefile);   
    virtual             ~XFILEAPPLICATIONCONFIG                      ();
      
    virtual bool         Default                                     ();


    #ifdef XFILEAPPLICATIONCONFIG_GENERAL_ACTIVE

    int                  GetCheckMemStatusCadence                    ();
    int                  GetCheckMemStatusLimitPercent               ();                         
    int                  GetCheckInternetStatusCadence               ();
    XSTRING*             GetWebScraperScriptURLDownload              (); 
    int									 GetCheckIPPublicChangeCadence							 ();		
		int									 GetUpdateTimeByNTPCadence									 ();
    XWORD                GetShowDetailInfo                           ();                                
    void                 SetShowDetailInfo                           (XWORD detail);
    XSTRING*             GetXDebugTarget                             (int index);

    #endif  
       

    #ifdef XFILEAPPLICATIONCONFIG_LOCATION_ACTIVE

    XSTRING*             Location_GetStreet                          ();
    XSTRING*             Location_GetCity                            ();
    XSTRING*             Location_GetState                           ();
    XSTRING*             Location_GetCountry                         ();
    int                  Location_GetPostalCode                      ();

    #endif  


    #ifdef XFILEAPPLICATIONCONFIG_APPUPDATE_ACTIVE

    bool                 ApplicationUpdate_IsActive                  ();
    int                  ApplicationUpdate_GetPort                   ();
    XSTRING*             ApplicationUpdate_GetURL                    ();
    int                  ApplicationUpdate_GetCheckCadence           ();
    XSTRING*             ApplicationUpdate_GetCheckTime              (); 

    #endif  


    #ifdef XFILEAPPLICATIONCONFIG_ALERTS_ACTIVE

    bool                 Alerts_IsActive                             ();
    XSTRING*             Alerts_GetSMTPURL                           ();
    int                  Alerts_GetSMTPPort                          ();
    XSTRING*             Alerts_GetSMTPLogin                         ();
    XSTRING*             Alerts_GetSMTPPassword                      ();
    XSTRING*             Alerts_GetSMTPSender                        ();
    XSTRING*             Alerts_GetSMTPRecipient                     (int index);                              
    XSTRING*             Alerts_GetSMSRecipient                      (int index);                    
    bool                 Alerts_GetWEBIsUseGet                       ();
    XSTRING*             Alerts_GetWEBRecipient                      (int index);
    int                  Alerts_GetUDPPort                           ();
    XSTRING*             Alerts_GetUDPRecipient                      (int index);

    #endif 


    #ifdef XFILEAPPLICATIONCONFIG_DNSMANAGER_ACTIVE

		XSTRING*             DNSManager_GetURL												   (int index);

    #endif
		

    #ifdef XFILEAPPLICATIONCONFIG_WEBSERVER_ACTIVE

		XSTRING*						 WebServer_GetLocalAddress									 ();
		int									 WebServer_GetPort													 ();
    XSTRING*						 WebServer_GetLogin     										 ();
		XSTRING*						 WebServer_GetPassword											 ();

    #endif  


    #ifdef XFILEAPPLICATIONCONFIG_LOG_ACTIVE

    bool                 Log_IsActive                                ();  
    bool                 Log_Backup_IsActive                         (); 
    int                  Log_Backup_GetMaxFiles                      ();
    bool                 Log_Backup_IsCompress                       ();
    XSTRING*             Log_ActiveSectionsID                        ();
    XBYTE                Log_LevelMask                               ();
    int                  Log_MaxSize                                 (); 
    int                  Log_ReductionPercent                        ();  

    #endif
    
  protected:


    #ifdef XFILEAPPLICATIONCONFIG_GENERAL_ACTIVE

    int                  checkmemstatuscadence;
    int                  checkmemstatuslimitpercent;
    int                  checkinternetstatuscadence;
    XSTRING              webscraperscripturldownload;
    int								   checkippublicchangecadence;		
		int									 updatetimebyntpcadence;
    XSTRING              showdetailinfo;
    XSTRING              xdebugtarget[XDEBUGTRACE_MAXNTARGETS];

    #endif


    #ifdef XFILEAPPLICATIONCONFIG_LOCATION_ACTIVE

    XSTRING              locationstreet;
    XSTRING              locationcity;
    XSTRING              locationstate;
    XSTRING              locationcountry;
    int                  locationpostalcode;

    #endif


    #ifdef XFILEAPPLICATIONCONFIG_APPUPDATE_ACTIVE

    bool                 applicationupdateisactive;
    XSTRING              applicationupdateURL;
    int                  applicationupdateport;
    int                  applicationupdatecheckcadence;
    XSTRING              applicationupdatechecktime;

    #endif



    #ifdef XFILEAPPLICATIONCONFIG_DNSMANAGER_ACTIVE

    XSTRING              dnsmanagerurl[XFILEAPPLICATIONCONFIG_DNSMANAGERMAXURL];

    #endif

		
    #ifdef XFILEAPPLICATIONCONFIG_WEBSERVER_ACTIVE

		XSTRING							 webserverlocaladdr;
		int									 webserverport;
    XSTRING							 webserverlogin;		
		XSTRING							 webserverpassword;		

    #endif  


    #ifdef XFILEAPPLICATIONCONFIG_ALERTS_ACTIVE

    bool                 alertsisactive;
    XSTRING              alertsSMTPURL;
    int                  alertsSMTPport;
    XSTRING              alertsSMTPlogin;
    XSTRING              alertsSMTPpassword;
    XSTRING              alertsSMTPsender;
    XSTRING              alertsSMTPrecipient[XFILEAPPLICATIONCONFIG_ALERTSMAXSMTPRECIPIENTS];
    XSTRING              alertsSMSrecipient[XFILEAPPLICATIONCONFIG_ALERTSMAXSMSRECIPIENTS];
    bool                 alertsWEBisuseget;
    XSTRING              alertsWEBrecipient[XFILEAPPLICATIONCONFIG_ALERTSMAXWEBRECIPIENTS];
    int                  alertsUDPport;
    XSTRING              alertsUDPrecipient[XFILEAPPLICATIONCONFIG_ALERTSMAXSMSRECIPIENTS];
    
    #endif  


    #ifdef XFILEAPPLICATIONCONFIG_LOG_ACTIVE

    bool                 logisactive;
    bool                 logbackupisactive;
    int                  logbackupmaxfiles;
    bool                 logbackupiscompress;
    XSTRING              logactivesectionsID;
    int                  loglevelmask;
    int                  logmaxsize;
    int                  logreductionpercent;
    
    #endif


  private:

    void                 Clean                                       ();
                            
};


/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

#endif



