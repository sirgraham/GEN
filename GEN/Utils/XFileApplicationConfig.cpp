/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XFileApplicationConfig.cpp
*
* @class       XFILEAPPLICATIONCONFIG
* @brief       POS Server Config class
* @ingroup     
*
* @author      Abraham J. Velez 
* @date        29/05/2018 15:10:10
*
* @copyright   Copyright(c) 2018 Versia Desarrollo SW, S.L.  All rights reserved.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XDebugTrace.h"

#include "XFileApplicationConfig.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XFILEAPPLICATIONCONFIG::XFILEAPPLICATIONCONFIG(XCHAR* namefile)
* @brief      Constructor
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:12:19
*
* @param[in]  namefile: namefile of the CFG file.
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XFILEAPPLICATIONCONFIG::XFILEAPPLICATIONCONFIG(XCHAR* namefile) 
#ifdef XFILEAPPLICATIONCONFIG_REMOTEFILE_ACTIVE 
 : DIOREMOTEFILECONFIG(namefile)
#else
 : XFILECONFIG(namefile)
#endif
{
  XSTRING section;
  XSTRING key;

  Clean();


  #ifdef XFILEAPPLICATIONCONFIG_GENERAL_ACTIVE
  //-----------------------------------------------------
  // GENERAL
  
	AddValue(XFILECONFIGVALUETYPE_INT		  , XFILEAPPLICATIONCONFIG_SECTIONGENERAL		                , XFILEAPPLICATIONCONFIG_CHECKMEMSTATUSCADENCE							              , &checkmemstatuscadence);
	AddValue(XFILECONFIGVALUETYPE_INT		  , XFILEAPPLICATIONCONFIG_SECTIONGENERAL		                , XFILEAPPLICATIONCONFIG_CHECKMEMSTATUSLIMITPERCENT					              , &checkmemstatuslimitpercent);	
  AddValue(XFILECONFIGVALUETYPE_INT		  , XFILEAPPLICATIONCONFIG_SECTIONGENERAL		                , XFILEAPPLICATIONCONFIG_CHECKINTERNETSTATUSCADENCE					              , &checkinternetstatuscadence);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONGENERAL                   , XFILEAPPLICATIONCONFIG_WEBSCRAPERSCRIPTURLDOWNLOAD                      , &webscraperscripturldownload);
	AddValue(XFILECONFIGVALUETYPE_INT		  , XFILEAPPLICATIONCONFIG_SECTIONGENERAL		                , XFILEAPPLICATIONCONFIG_CHECKIPPUBLICCHANGECADENCE					              , &checkippublicchangecadence);	
	AddValue(XFILECONFIGVALUETYPE_INT		  , XFILEAPPLICATIONCONFIG_SECTIONGENERAL		                , XFILEAPPLICATIONCONFIG_UPDATETIMEBYNTPCADENCE							              , &updatetimebyntpcadence);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONGENERAL                   , XFILEAPPLICATIONCONFIG_SHOWDETAILINFO                                   , &showdetailinfo);
																			
  for(int c=0; c<XDEBUGTRACE_MAXNTARGETS; c++)
    {
      key.Format(__L("%s%02d"), XFILEAPPLICATIONCONFIG_XDEBUGTARGET, c+1);
      AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONGENERAL, key.Get(), &xdebugtarget[c]);
    }
  #endif


  #ifdef XFILEAPPLICATIONCONFIG_LOCATION_ACTIVE
  //-----------------------------------------------------
  // LOCATION
 
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONLOCATION                  , XFILEAPPLICATIONCONFIG_LOCATIONSTREET                                   , &locationstreet);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONLOCATION                  , XFILEAPPLICATIONCONFIG_LOCATIONTOWN                                     , &locationcity);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONLOCATION                  , XFILEAPPLICATIONCONFIG_LOCATIONSTATE                                    , &locationstate);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONLOCATION                  , XFILEAPPLICATIONCONFIG_LOCATIONCOUNTRY                                  , &locationcountry);
  AddValue(XFILECONFIGVALUETYPE_INT     , XFILEAPPLICATIONCONFIG_SECTIONLOCATION                  , XFILEAPPLICATIONCONFIG_LOCATIONPOSTALCODE                               , &locationpostalcode);  
  #endif


  #ifdef XFILEAPPLICATIONCONFIG_APPUPDATE_ACTIVE
  //-----------------------------------------------------
  // APPLICATION UPDATE 
  
  AddValue(XFILECONFIGVALUETYPE_BOOLEAN , XFILEAPPLICATIONCONFIG_SECTIONAPPLICATIONUPDATE         , XFILEAPPLICATIONCONFIG_APPLICATIONUPDATEISACTIVE                        , &applicationupdateisactive);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONAPPLICATIONUPDATE         , XFILEAPPLICATIONCONFIG_APPLICATIONUPDATEURL                             , &applicationupdateURL);
  AddValue(XFILECONFIGVALUETYPE_INT     , XFILEAPPLICATIONCONFIG_SECTIONAPPLICATIONUPDATE         , XFILEAPPLICATIONCONFIG_APPLICATIONUPDATEPORT                            , &applicationupdateport);
  AddValue(XFILECONFIGVALUETYPE_INT     , XFILEAPPLICATIONCONFIG_SECTIONAPPLICATIONUPDATE         , XFILEAPPLICATIONCONFIG_APPLICATIONUPDATECHECKCADENCE                    , &applicationupdatecheckcadence);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONAPPLICATIONUPDATE         , XFILEAPPLICATIONCONFIG_APPLICATIONUPDATECHECKTIME                       , &applicationupdatechecktime);
  #endif


  #ifdef XFILEAPPLICATIONCONFIG_DNSMANAGER_ACTIVE
  //-----------------------------------------------------
	// DNSMANAGER

	for(int c=0; c<XFILEAPPLICATIONCONFIG_DNSMANAGERMAXURL; c++)
		{																					
			key.Format(__L("%s%02d"), XFILEAPPLICATIONCONFIG_DNSMANAGERURL, c+1);																						
			AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONDNSMANAGER, key.Get(), &dnsmanagerurl[c]);
		}							  
  #endif


  #ifdef XFILEAPPLICATIONCONFIG_WEBSERVER_ACTIVE
  //-----------------------------------------------------
	// WEBSERVER
	AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONWEBSERVER					        , XFILEAPPLICATIONCONFIG_WEBSERVERLOCALADDR								              	,	&webserverlocaladdr);
	AddValue(XFILECONFIGVALUETYPE_INT		  ,	XFILEAPPLICATIONCONFIG_SECTIONWEBSERVER				        	, XFILEAPPLICATIONCONFIG_WEBSERVERPORT											              , &webserverport);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONWEBSERVER					        , XFILEAPPLICATIONCONFIG_WEBSERVERLOGIN     								              ,	&webserverlogin);
	AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONWEBSERVER					        , XFILEAPPLICATIONCONFIG_WEBSERVERPASSWORD									              ,	&webserverpassword);
  #endif


  #ifdef XFILEAPPLICATIONCONFIG_ALERTS_ACTIVE
  //-----------------------------------------------------
  // ALERTS

  AddValue(XFILECONFIGVALUETYPE_BOOLEAN , XFILEAPPLICATIONCONFIG_SECTIONALERTS                    , XFILEAPPLICATIONCONFIG_ALERTSACTIVE                                     , &alertsisactive);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONALERTS                    , XFILEAPPLICATIONCONFIG_ALERTSSMTPURL                                    , &alertsSMTPURL);
  AddValue(XFILECONFIGVALUETYPE_INT     , XFILEAPPLICATIONCONFIG_SECTIONALERTS                    , XFILEAPPLICATIONCONFIG_ALERTSSMTPPORT                                   , &alertsSMTPport);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONALERTS                    , XFILEAPPLICATIONCONFIG_ALERTSSMTPLOGIN                                  , &alertsSMTPlogin);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONALERTS                    , XFILEAPPLICATIONCONFIG_ALERTSSMTPPASSWORD                               , &alertsSMTPpassword);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONALERTS                    , XFILEAPPLICATIONCONFIG_ALERTSSMTPSENDER                                 , &alertsSMTPsender);

  for(int c=0; c<XFILEAPPLICATIONCONFIG_ALERTSMAXSMTPRECIPIENTS; c++)
    {
      key.Format(__L("%s%02d"), XFILEAPPLICATIONCONFIG_ALERTSSMTPRECIPIENT, c+1);
      AddValue(XFILECONFIGVALUETYPE_STRING, XFILEAPPLICATIONCONFIG_SECTIONALERTS, key.Get(),  &alertsSMTPrecipient[c]);
    }


  for(int c=0; c<XFILEAPPLICATIONCONFIG_ALERTSMAXSMSRECIPIENTS; c++)
    {
      key.Format(__L("%s%02d"), XFILEAPPLICATIONCONFIG_ALERTSSMSRECIPIENT, c+1);
      AddValue(XFILECONFIGVALUETYPE_STRING, XFILEAPPLICATIONCONFIG_SECTIONALERTS, key.Get(),  &alertsSMSrecipient[c]);
    }


  AddValue(XFILECONFIGVALUETYPE_BOOLEAN , XFILEAPPLICATIONCONFIG_SECTIONALERTS                    , XFILEAPPLICATIONCONFIG_ALERTSWEBUISUSEGET                               , &alertsWEBisuseget);
  for(int c=0; c<XFILEAPPLICATIONCONFIG_ALERTSMAXWEBRECIPIENTS; c++)
    {
      key.Format(__L("%s%02d"), XFILEAPPLICATIONCONFIG_ALERTSWEBRECIPIENT, c+1);
      AddValue(XFILECONFIGVALUETYPE_STRING, XFILEAPPLICATIONCONFIG_SECTIONALERTS, key.Get(),  &alertsWEBrecipient[c]);
    }


  AddValue(XFILECONFIGVALUETYPE_INT     , XFILEAPPLICATIONCONFIG_SECTIONALERTS                    , XFILEAPPLICATIONCONFIG_ALERTSUDPPORT                                    , &alertsUDPport);
  for(int c=0; c<XFILEAPPLICATIONCONFIG_ALERTSMAXUDPRECIPIENTS; c++)
    {
      key.Format(__L("%s%02d"), XFILEAPPLICATIONCONFIG_ALERTSUDPRECIPIENT, c+1);
      AddValue(XFILECONFIGVALUETYPE_STRING, XFILEAPPLICATIONCONFIG_SECTIONALERTS, key.Get(),  &alertsUDPrecipient[c]);
    }

  #endif


  #ifdef XFILEAPPLICATIONCONFIG_LOG_ACTIVE
  //-----------------------------------------------------
  // LOG

  AddValue(XFILECONFIGVALUETYPE_BOOLEAN , XFILEAPPLICATIONCONFIG_SECTIONLOG                     , XFILEAPPLICATIONCONFIG_LOGISACTIVE                                        , &logisactive);
  AddValue(XFILECONFIGVALUETYPE_BOOLEAN , XFILEAPPLICATIONCONFIG_SECTIONLOG                     , XFILEAPPLICATIONCONFIG_LOGBACKUPISACTIVE                                  , &logbackupisactive);
  AddValue(XFILECONFIGVALUETYPE_INT     , XFILEAPPLICATIONCONFIG_SECTIONLOG                     , XFILEAPPLICATIONCONFIG_LOGBACKUPMAXFILES                                  , &logbackupmaxfiles);
  AddValue(XFILECONFIGVALUETYPE_BOOLEAN , XFILEAPPLICATIONCONFIG_SECTIONLOG                     , XFILEAPPLICATIONCONFIG_LOGBACKUPISCOMPRESS                                , &logbackupiscompress);
  AddValue(XFILECONFIGVALUETYPE_STRING  , XFILEAPPLICATIONCONFIG_SECTIONLOG                     , XFILEAPPLICATIONCONFIG_LOGACTIVESECTIONSID                                , &logactivesectionsID);
  AddValue(XFILECONFIGVALUETYPE_MASK    , XFILEAPPLICATIONCONFIG_SECTIONLOG                     , XFILEAPPLICATIONCONFIG_LOGLEVELMASK                                       , &loglevelmask);
  AddValue(XFILECONFIGVALUETYPE_INT     , XFILEAPPLICATIONCONFIG_SECTIONLOG                     , XFILEAPPLICATIONCONFIG_LOGMAXSIZE                                         , &logmaxsize);
  AddValue(XFILECONFIGVALUETYPE_INT     , XFILEAPPLICATIONCONFIG_SECTIONLOG                     , XFILEAPPLICATIONCONFIG_LOGREDUCTIONPERCENT                                , &logreductionpercent);
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XFILEAPPLICATIONCONFIG::~XFILEAPPLICATIONCONFIG()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:14:14
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XFILEAPPLICATIONCONFIG::~XFILEAPPLICATIONCONFIG()
{
  End();

  Clean();
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XFILEAPPLICATIONCONFIG::Default()
* @brief      Default CFG
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 13:04:52
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XFILEAPPLICATIONCONFIG::Default()
{
  return true;
}




#ifdef XFILEAPPLICATIONCONFIG_GENERAL_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::GetCheckMemStatusCadence()
* @brief      Get cadence for check mem status (in seconds)
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:39:52
*
* @return     int : check cadence in seconds
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::GetCheckMemStatusCadence()
{ 
  return checkmemstatuscadence;                             
} 



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::GetCheckMemStatusLimitPercent()
* @brief      Get CFG Memory Limit (in Percent)
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:40:45
*
* @return     int : return memory limit (in percent)
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::GetCheckMemStatusLimitPercent()  
{
  return checkmemstatuslimitpercent;                        
} 


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::GetCheckInternetStatusCadence()
* @brief      Get cadence for check Internet Status (in seconds)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       01/06/2018 17:58:03
*
* @return     int : check cadence in seconds
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::GetCheckInternetStatusCadence()
{
  return checkinternetstatuscadence;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         
XSTRING* XFILEAPPLICATIONCONFIG::GetWebScraperScriptURLDownload()
* @brief      Get CFG URL Web Scraper CFG file download
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:41:55
*
* @return     XSTRING* : return URL WEB scrapper CFG file to download
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::GetWebScraperScriptURLDownload() 
{ 
  return &webscraperscripturldownload;                      
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::GetCheckIPPublicChangeCadence()
* @brief      Get cadence for check IP public status (in minutes)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       01/06/2018 18:01:34
*
* @return     int : check cadence in seconds
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::GetCheckIPPublicChangeCadence()
{ 
  return (checkippublicchangecadence*60);								
} 



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::GetUpdateTimeByNTPCadence()
* @brief      Get cadence for update time by NTP (in hours)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       01/06/2018 18:05:29
*
* @return     int : check cadence in seconds
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::GetUpdateTimeByNTPCadence()
{ 
  return (updatetimebyntpcadence*3600);									
} 



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD XFILEAPPLICATIONCONFIG::GetShowDetailInfo()
* @brief      Get CFG of Mask to Show detail Info
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:43:01
*
* @return     XWORD : return mask to show detail info in console
*
*---------------------------------------------------------------------------------------------------------------------*/
XWORD XFILEAPPLICATIONCONFIG::GetShowDetailInfo()
{
  XDWORD detail = 0x0000;

  showdetailinfo.ToUpperCase();
  showdetailinfo.UnFormat(__L("%04X"), &detail);

  return detail;
}


   
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XFILEAPPLICATIONCONFIG::SetShowDetailInfo(XWORD detail)
* @brief      Set CFG of Mask to Show detail Info
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:43:29
*
* @param[in]  detail : mask to show detail info in console
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XFILEAPPLICATIONCONFIG::SetShowDetailInfo(XWORD detail)   
{ 
  showdetailinfo.Format(__L("%04X"), detail);               
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::GetXDebugTarget(int index)
* @brief      Get CFG of XDebug trace target
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:45:03
*
* @param[in]  index : index of URL MAX > XDEBUG_MAXNTARGETS
*
* @return     XSTRING* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::GetXDebugTarget(int index)
{
  if(index <  0)                        return NULL;
  if(index >= XDEBUGTRACE_MAXNTARGETS)  return NULL;

  return &xdebugtarget[index];
}


#endif



#ifdef XFILEAPPLICATIONCONFIG_LOCATION_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Location_GetStreet()
* @brief      Get CFG Location Street
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:46:55
*
* @return     XSTRING* : return string with the CFG location street 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Location_GetStreet() 
{ 
  return &locationstreet;                                   
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Location_GetCity()
* @brief      Get CFG Location City
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:47:42
*
* @return     XSTRING* : return string CFG Location City
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Location_GetCity()
{ 
  return &locationcity;                                     
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Location_GetState()
* @brief      CFG Location City
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:09:52
*
* @return     XSTRING* : return string CFG Location City
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Location_GetState()
{ 
  return &locationstate;                                    
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Location_GetCountry()
* @brief      Get CFG location contry
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:11:28
*
* @return     XSTRING* : return string CFG location contry
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Location_GetCountry() 
{
  return &locationcountry;                                  
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::Location_GetPostalCode()
* @brief      Get CFG location Postal code
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:18:48
*
* @return     int : return CFG location Postal code
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::Location_GetPostalCode()
{ 
  return locationpostalcode;                                
}

#endif


#ifdef XFILEAPPLICATIONCONFIG_APPUPDATE_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XFILEAPPLICATIONCONFIG::ApplicationUpdate_IsActive()
* @brief      Get CFG if the Application has the update Application  control active
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:21:55
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XFILEAPPLICATIONCONFIG::ApplicationUpdate_IsActive()  
{ 
  return applicationupdateisactive;                         
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::ApplicationUpdate_GetPort()
* @brief      Get CFG Port number of the update application control
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:22:39
*
* @return     int : return Port number of the update application control
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::ApplicationUpdate_GetPort()  
{ 
  return applicationupdateport;                             
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::ApplicationUpdate_GetURL()
* @brief      Get CFG URL application update
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:24:03
*
* @return     XSTRING* : return URL application update
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::ApplicationUpdate_GetURL()
{ 
  return &applicationupdateURL;                             
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::ApplicationUpdate_GetCheckCadence()
* @brief      Get CFG Check Cadence of the application update (hours)
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:25:02
*
* @return     int : return Check Cadence of the application update (hours)
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::ApplicationUpdate_GetCheckCadence() 
{ 
  return applicationupdatecheckcadence;                     
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::ApplicationUpdate_GetCheckTime()
* @brief      Get CFG Check time to Update
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 12:22:22
*
* @return     XSTRING* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::ApplicationUpdate_GetCheckTime()
{ 
  return &applicationupdatechecktime;                       
}


#endif



#ifdef XFILEAPPLICATIONCONFIG_ALERTS_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XFILEAPPLICATIONCONFIG::Alerts_IsActive()
* @brief      Get CFG if the Alerts is active
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:26:24
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XFILEAPPLICATIONCONFIG::Alerts_IsActive()  
{ 
  return alertsisactive;                                    
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPURL()
* @brief      Get CFG for URL of STMP for the Alerts
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:26:46
*
* @return     XSTRING* : return string CFG URL of STMP for the Alerts
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPURL()  
{ 
  return &alertsSMTPURL;                                   
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::Alerts_GetSMTPPort()
* @brief      Get CFG for port number of STMP for the Alerts
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:27:29
*
* @return     int : return port number of STMP for the Alerts
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::Alerts_GetSMTPPort()
{
  return alertsSMTPport;                                    
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPLogin()
* @brief      Get CFG for login of STMP for the Alerts
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:29:03
*
* @return     XSTRING* : return string login of STMP for the Alerts
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPLogin()
{ 
  return &alertsSMTPlogin;                                 
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPPassword()
* @brief      Get CFG for password of STMP for the Alerts
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:29:39
*
* @return     XSTRING* : return string password of STMP for the Alerts
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPPassword()
{ 
  return &alertsSMTPpassword;                               
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPSender()
* @brief      Get CFG for sender of STMP for the Alerts
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:30:28
*
* @return     XSTRING* : return string sender of STMP for the Alerts
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPSender()
{ 
  return &alertsSMTPsender;                                
}

    
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPRecipient(int index)
* @brief      Get CFG of Recipients of STMP for the Alerts
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:31:43
*
* @param[in]  index : index of recipiens (XFILEAPPLICATIONCONFIG_ALERTSMAXSMTPRECIPIENTS)
*
* @return     XSTRING* : return string Recipients of STMP for the Alerts
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMTPRecipient(int index)
{
  if(index<0)                                       return NULL;
  if(index>=XFILEAPPLICATIONCONFIG_ALERTSMAXSMTPRECIPIENTS) return NULL;

  return &alertsSMTPrecipient[index];
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMSRecipient(int index)
* @brief      
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:33:33
*
* @param[in]  index : 
*
* @return     XSTRING* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetSMSRecipient(int index)
{
  if(index<0)                                         return NULL;
  if(index>=XFILEAPPLICATIONCONFIG_ALERTSMAXSMSRECIPIENTS)    return NULL;

  return &alertsSMSrecipient[index];
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XFILEAPPLICATIONCONFIG::Alerts_GetWEBIsUseGet()
* @brief      
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:33:41
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XFILEAPPLICATIONCONFIG::Alerts_GetWEBIsUseGet() 
{ 
  return alertsWEBisuseget;                                 
}
 


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetWEBRecipient(int index)
* @brief      
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:33:49
*
* @param[in]  index : 
*
* @return     XSTRING* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetWEBRecipient(int index)
{
  if(index<0)                                         return NULL;
  if(index>=XFILEAPPLICATIONCONFIG_ALERTSMAXWEBRECIPIENTS)    return NULL;

  return &alertsWEBrecipient[index];
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::Alerts_GetUDPPort()
* @brief      
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:33:56
*
* @return     int : 
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::Alerts_GetUDPPort()  
{ 
  return alertsUDPport;                                     
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetUDPRecipient(int index)
* @brief      
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 16:34:12
*
* @param[in]  index : 
*
* @return     XSTRING* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Alerts_GetUDPRecipient(int index)
{
  if(index<0)                                                 return NULL;
  if(index>=XFILEAPPLICATIONCONFIG_ALERTSMAXUDPRECIPIENTS)    return NULL;

  return &alertsUDPrecipient[index];
}


#endif



#ifdef XFILEAPPLICATIONCONFIG_DNSMANAGER_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::DNSManager_GetURL(int index)
* @brief      
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       01/06/2018 20:22:19
*
* @param[in]  index : 
*
* @return     XSTRING* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::DNSManager_GetURL(int index)
{ 
	if(index < 0)        																 return NULL;
	if(index >= XFILEAPPLICATIONCONFIG_DNSMANAGERMAXURL) return NULL;

	return &dnsmanagerurl[index];															
}		

#endif


#ifdef XFILEAPPLICATIONCONFIG_WEBSERVER_ACTIVE
		

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::WebServer_GetLocalAddress()
* @brief      Get Local Address for Web Server
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:49:01
*
* @return     XSTRING* : local address
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::WebServer_GetLocalAddress()
{ 
  return &webserverlocaladdr;														
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::WebServer_GetPort()
* @brief      Get Port for Web Server
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:49:29
*
* @return     int : port number
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::WebServer_GetPort()
{
  return webserverport;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::WebServer_GetLogin()
* @brief      WebServer_GetLogin
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 12:01:33
*
* @return     XSTRING* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::WebServer_GetLogin()
{
  return &webserverlogin;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::WebServer_GetPassword()
* @brief      Get Password for Web Server
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:50:02
*
* @return     XSTRING* : Password web server
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::WebServer_GetPassword()
{
  return &webserverpassword;
}

#endif


#ifdef XFILEAPPLICATIONCONFIG_LOG_ACTIVE



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XFILEAPPLICATIONCONFIG::Log_IsActive()
* @brief      Is Active Log
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:50:36
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XFILEAPPLICATIONCONFIG::Log_IsActive() 
{ 
  return logisactive;                                       
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XFILEAPPLICATIONCONFIG::Log_Backup_IsActive()
* @brief      Is Active Log Backup
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:50:52
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XFILEAPPLICATIONCONFIG::Log_Backup_IsActive()  
{ 
  return logbackupisactive;                                
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::Log_Backup_GetMaxFiles()
* @brief      Get Max Files to Log Backup
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:51:27
*
* @return     int : max files number
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::Log_Backup_GetMaxFiles()
{ 
  return logbackupmaxfiles;                                 
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XFILEAPPLICATIONCONFIG::Log_Backup_IsCompress()
* @brief      Is Backup Compress 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:54:36
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XFILEAPPLICATIONCONFIG::Log_Backup_IsCompress()
{ 
  return logbackupiscompress;                               
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XFILEAPPLICATIONCONFIG::Log_ActiveSectionsID()
* @brief      Active Sections ID of log
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:53:51
*
* @return     XSTRING* : string with the secctions active (separated by comma ',')
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XFILEAPPLICATIONCONFIG::Log_ActiveSectionsID() 
{ 
  return &logactivesectionsID;                              
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE XFILEAPPLICATIONCONFIG::Log_LevelMask()
* @brief      Level Mask for Log
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:53:32
*
* @return     XBYTE : Mask of log
*
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE XFILEAPPLICATIONCONFIG::Log_LevelMask() 
{ 
  return (XBYTE)loglevelmask;                               
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::Log_MaxSize()
* @brief      Max Size of Log
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:53:05
*
* @return     int : size in bytes
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::Log_MaxSize()  
{ 
  return logmaxsize;                                        
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XFILEAPPLICATIONCONFIG::Log_ReductionPercent()
* @brief      Reduction Percent to Log file 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 10:52:07
*
* @return     int : percent reduction (what remains)
*
*---------------------------------------------------------------------------------------------------------------------*/
int XFILEAPPLICATIONCONFIG::Log_ReductionPercent() 
{ 
  return logreductionpercent;                               
}


#endif




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XFILEAPPLICATIONCONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    
*
* @author     Abraham J. Velez 
* @date       29/05/2018 15:26:19
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
 void XFILEAPPLICATIONCONFIG::Clean()
{
  #ifdef XFILEAPPLICATIONCONFIG_GENERAL_ACTIVE

  checkmemstatuscadence                             = 0;
  checkmemstatuslimitpercent                        = 0;
  checkinternetstatuscadence                        = 0;
  webscraperscripturldownload.Empty();
  checkippublicchangecadence                        = 0;		
	updatetimebyntpcadence                            = 0;
  showdetailinfo                                    = __L("0000");

  #endif


  #ifdef XFILEAPPLICATIONCONFIG_LOCATION_ACTIVE

  locationstreet.Empty();
  locationcity.Empty();
  locationstate.Empty();
  locationcountry.Empty();
  locationpostalcode                                = 0;

  #endif


  #ifdef XFILEAPPLICATIONCONFIG_APPUPDATE_ACTIVE

  applicationupdateisactive                         = false;
  applicationupdateport                             = 0;
  applicationupdatecheckcadence                     = 0;
  applicationupdatechecktime.Empty();

  #endif


  #ifdef XFILEAPPLICATIONCONFIG_WEBSERVER_ACTIVE

  webserverlocaladdr.Empty();
  webserverport													= 0;	

  #endif


  #ifdef XFILEAPPLICATIONCONFIG_ALERTS_ACTIVE

  alertsisactive                                    = false;
  alertsSMTPport                                    = 0;
  for(int c=0; c<XFILEAPPLICATIONCONFIG_ALERTSMAXSMTPRECIPIENTS; c++)
    {
      alertsSMTPrecipient[c].Empty();
    }

  for(int c=0; c<XFILEAPPLICATIONCONFIG_ALERTSMAXSMSRECIPIENTS; c++)
    {
      alertsSMSrecipient[c].Empty();
    }

  alertsWEBisuseget                                 = false;
  for(int c=0; c<XFILEAPPLICATIONCONFIG_ALERTSMAXWEBRECIPIENTS; c++)
    {
      alertsWEBrecipient[c].Empty();
    }

  alertsUDPport                                     = 0;
  for(int c=0; c<XFILEAPPLICATIONCONFIG_ALERTSMAXUDPRECIPIENTS; c++)
    {
      alertsUDPrecipient[c].Empty();
    }
  
  #endif



  #ifdef XFILEAPPLICATIONCONFIG_LOG_ACTIVE

  logisactive                                       = false;
  logbackupisactive                                 = false;
  logbackupmaxfiles                                 = 0;
  logbackupiscompress                               = false;
  logactivesectionsID.Empty();
  loglevelmask                                      = 0;
  logmaxsize                                        = 0;
  logreductionpercent                               = 0;

  #endif
}

