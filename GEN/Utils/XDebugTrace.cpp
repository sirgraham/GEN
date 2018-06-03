//------------------------------------------------------------------------------------------
//  XDEBUG.CPP
//
//  Debug Funtions
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStreamEnumDevices.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOWebScraperPublicIP.h"

#include "XDebugTrace.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

XDEBUGTRACE* XDEBUGTRACE::instance = NULL;

//---- CLASS MEMBERS -----------------------------------------------------------------------


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDEBUGTRACE_TARGET::XDEBUGTRACE_TARGET()
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:28:31
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XDEBUGTRACE_TARGET::XDEBUGTRACE_TARGET()
{ 
  Clean();                                    
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDEBUGTRACE_TARGET::~XDEBUGTRACE_TARGET()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:26:56
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XDEBUGTRACE_TARGET::~XDEBUGTRACE_TARGET()
{ 
  Clean();                                    
}


    
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDEBUGTRACE_TYPE XDEBUGTRACE_TARGET::GetType()
* @brief      Get Type of debug trace (enum XDEBUGTRACE_TYPE)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:27:16
*
* @return     XDEBUGTRACE_TYPE : type of debug trace (enum XDEBUGTRACE_TYPE)
*
*---------------------------------------------------------------------------------------------------------------------*/
XDEBUGTRACE_TYPE XDEBUGTRACE_TARGET::GetType()
{ 
  return type;                                
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE_TARGET::SetType(XDEBUGTRACE_TYPE type)
* @brief      Set debug trace type (enum XDEBUGTRACE_TYPE)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:28:57
*
* @param[in]  type : type of debug trace (enum XDEBUGTRACE_TYPE)
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE_TARGET::SetType(XDEBUGTRACE_TYPE type)
{ 
  this->type = type;                          
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCHAR* XDEBUGTRACE_TARGET::GetAim()
* @brief      Get aim of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:30:11
*
* @return     XCHAR* : aim of target
*
*---------------------------------------------------------------------------------------------------------------------*/
XCHAR* XDEBUGTRACE_TARGET::GetAim()
{ 
  return aim;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE_TARGET::GetAim(XSTRING& aim)
* @brief      Get aim of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:30:50
*
* @param[out] aim : Get aim of target
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE_TARGET::GetAim(XSTRING& aim)
{
  aim.Empty();
  aim = this->aim;

  return aim.IsEmpty()?false:true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE_TARGET::SetAim(XSTRING& aim)
* @brief      Set aim of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:31:39
*
* @param[in]  aim : new aim of target
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE_TARGET::SetAim(XSTRING& aim)
{
  memset(this->aim, 0, (_MAXSTR * sizeof(XCHAR)));
  memcpy(this->aim, aim.Get(), (aim.GetSize() * sizeof(XCHAR)));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE_TARGET::SetAim(XCHAR* aim)
* @brief      Set aim of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:32:17
*
* @param[in]  aim : new aim of target
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE_TARGET::SetAim(XCHAR* aim)
{
  XDWORD size = XSTRING::GetSize(aim);
  memset(this->aim, 0, (_MAXSTR * sizeof(XCHAR)));
  memcpy(this->aim, aim, size * sizeof(XCHAR));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD XDEBUGTRACE_TARGET::GetPort()
* @brief      Get port of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:33:00
*
* @return     XWORD : port of target
*
*---------------------------------------------------------------------------------------------------------------------*/
XWORD XDEBUGTRACE_TARGET::GetPort()
{ 
  return port;                                
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE_TARGET::SetPort(XWORD port)
* @brief      Set Port of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:33:28
*
* @param[in]  port : port of target
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE_TARGET::SetPort(XWORD port)
{ 
  this->port = port;                          
}

  

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE_TARGET::ResolvedIPTarget()
* @brief      Resolved IP of target aim = URL -> IP 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:34:39
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE_TARGET::ResolvedIPTarget()
{
  if(!diofactory)                return false;
  if(type != XDEBUGTRACE_TYPE_NET) return false;

  DIOURL*  URL;
  XSTRING  remoteIP;

  URL = diofactory->CreateURL();
  if(!URL) return false;

  URL->Set(aim);
  URL->ResolveURL(remoteIP);
  diofactory->DeleteURL(URL);

  XSTRING_CREATEOEM(remoteIP, charstr)
  memcpy(IP, charstr, strlen(charstr)+1);
  XSTRING_DELETEOEM(charstr)

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         char* XDEBUGTRACE_TARGET::GetIP()
* @brief      Get IP of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:43:31
*
* @return     char* : string with the IP 
*
*---------------------------------------------------------------------------------------------------------------------*/
char* XDEBUGTRACE_TARGET::GetIP()
{ 
  return IP;                                  
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XQWORD XDEBUGTRACE_TARGET::GetNETHandle()
* @brief      Get net Handle of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:44:04
*
* @return     XQWORD : Net Handle of target
*
*---------------------------------------------------------------------------------------------------------------------*/
XQWORD XDEBUGTRACE_TARGET::GetNETHandle()
{ 
  return NEThandle;                           
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE_TARGET:: SetNETHandle(XQWORD NEThandle)
* @brief      Set Net Handle of target
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:44:21
*
* @param[in]  NEThandle : new Net Handle of target
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE_TARGET:: SetNETHandle(XQWORD NEThandle)
{ 
  this->NEThandle = NEThandle;                
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XDEBUGTRACE_TARGET::GetNSending()
* @brief      Get number of sendings
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:37:22
*
* @return     XDWORD : number of sendings
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XDEBUGTRACE_TARGET::GetNSendings()
{ 
  return nsendings;                               
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE_TARGET::AddNSendings()
* @brief      Add One to number of sendings
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:41:22
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE_TARGET::AddNSendings()
{ 
  nsendings++;                                    
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE_TARGET::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       30/05/2018 16:41:34
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE_TARGET::Clean()
{
  type          = XDEBUGTRACE_TYPE_NONE;

  memset(aim  , 0, (_MAXSTR * sizeof(XCHAR)));

  port          = 0;

  memset(IP, 0, _MAXSTR * sizeof(char));

  nsendings     = 0;
  NEThandle     = 0;
}



/**-------------------------------------------------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDEBUGTRACE::XDEBUGTRACE()
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 12:51:20
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XDEBUGTRACE::XDEBUGTRACE()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDEBUGTRACE::~XDEBUGTRACE()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 12:52:32
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XDEBUGTRACE::~XDEBUGTRACE()
{
  Clean();                                  
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::SetTarget(int index, XDEBUGTRACE_TYPE type, XCHAR* aim)
* @brief      Set Target by index 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 12:54:43
*
* @param[in]  index : index to set target
* @param[in]  type : type of the target
* @param[in]  aim : aim of the target
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::SetTarget(int index, XDEBUGTRACE_TYPE type, XCHAR* aim)
{
  if(index >= XDEBUGTRACE_MAXNTARGETS) return false;

  if(!aim) return false;

  targets[index].SetType(type);

  if(type == XDEBUGTRACE_TYPE_NET)
    {
      XSTRING string;
      XSTRING url;
      int     port;

      string = aim;
      url.AdjustSize(_MAXSTR);
      string.UnFormat(__L("%s:%d"), url.Get(), &port);
      url.AdjustSize();

      targets[index].SetAim(url.Get());
      targets[index].SetPort(port);
      targets[index].ResolvedIPTarget();

      if(targets[index].GetNETHandle()) CloseHandleNet(&targets[index]);
      GetHandleNet(&targets[index]);

      if(!localIP)    ObtainLocalIP();
      if(!publicIP)   ObtainPublicIP();
    }
   else
    {
      targets[index].SetAim(aim);
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::AddTarget(XDEBUGTRACE_TYPE type, XCHAR* aim)
* @brief      Add target (first index free)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 12:55:46
*
* @param[in]  type : type of the target
* @param[in]  aim : aim of the target
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::AddTarget(XDEBUGTRACE_TYPE type, XCHAR* aim)
{
  for(int c=0; c<XDEBUGTRACE_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XDEBUGTRACE_TYPE_NONE)  return SetTarget(c, type, aim);
    }

  return false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XDEBUGTRACE::GetSizeLimit()
* @brief      Get size limit
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 13:04:44
*
* @return     XDWORD : size limit
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XDEBUGTRACE::GetSizeLimit()
{ 
  return sizelimit;                         
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::SetSizeLimit(XDWORD sizelimit)
* @brief      Set size limit
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 13:05:37
*
* @param[in]  sizelimit : new size limit
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::SetSizeLimit(XDWORD sizelimit)
{
  if(!sizelimit) return false;

  this->sizelimit = sizelimit;
  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCHAR* XDEBUGTRACE::GetApplicationName()
* @brief      Get Application Name
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 13:07:20
*
* @return     XCHAR* : application name
*
*---------------------------------------------------------------------------------------------------------------------*/
XCHAR* XDEBUGTRACE::GetApplicationName()
{ 
  return applicationname;                   
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::SetApplicationName(XCHAR* applicationname)
* @brief      Set Application Name
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 13:08:24
*
* @param[in]  applicationname : new application name
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::SetApplicationName(XCHAR* applicationname)
{
  XDWORD size = 0;
  if(applicationname) size=XSTRING::GetSize(applicationname);

  memset(this->applicationname, 0, _MAXSTR * sizeof(XCHAR));
  if(applicationname) memcpy(this->applicationname, applicationname, (size + 1) * sizeof(XCHAR));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::SetApplicationName(XCHAR* applicationname, XDWORD size)
* @brief      Set Application Name
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 13:09:31
*
* @param[in]  applicationname : new application name
* @param[in]  size : size of application name
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::SetApplicationName(XCHAR* applicationname, XDWORD size)
{
  memset(this->applicationname, 0, _MAXSTR*sizeof(XCHAR));
  if(applicationname) memcpy(this->applicationname, applicationname, (size+1)*sizeof(XCHAR));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::SetApplicationName(XSTRING& applicationname)
* @brief      Set Application name
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:18:22
*
* @param[in]  applicationname : new application name
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::SetApplicationName(XSTRING& applicationname)
{
  SetApplicationName(applicationname.Get(), applicationname.GetSize());
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::GetApplicationVersion(int& applicationversion, int& applicationsubversion, int& applicationsubversionerr)
* @brief      Get Application version (three data)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:18:54
*
* @param[in]  applicationversion : version number
* @param[in]  applicationsubversion : subversion number
* @param[in]  applicationsubversionerr : subversion error number
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::GetApplicationVersion(int& applicationversion, int& applicationsubversion, int& applicationsubversionerr)
{
  applicationversion        = this->applicationversion;
  applicationsubversion     = this->applicationsubversion;
  applicationsubversionerr  = this->applicationsubversionerr;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::SetApplicationVersion(int applicationversion, int applicationsubversion, int applicationsubversionerr)
* @brief      Set Application version
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:20:37
*
* @param[in]  applicationversion : new version number
* @param[in]  applicationsubversion : new subversion number
* @param[in]  applicationsubversionerr : new subversion error number
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::SetApplicationVersion(int applicationversion, int applicationsubversion, int applicationsubversionerr)
{
  this->applicationversion        = applicationversion;
  this->applicationsubversion     = applicationsubversion;
  this->applicationsubversionerr  = applicationsubversionerr;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCHAR* XDEBUGTRACE::GetApplicationID()
* @brief      Get Application ID
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:22:24
*
* @return     XCHAR* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XCHAR* XDEBUGTRACE::GetApplicationID()
{ 
  return applicationID;                     
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::SetApplicationID(XCHAR* applicationID, XDWORD size)
* @brief      Set Application ID
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:22:46
*
* @param[in]  applicationID : new string application ID
* @param[in]  size : size application ID
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::SetApplicationID(XCHAR* applicationID, XDWORD size)
{
  memset(this->applicationID, 0, _MAXSTR*sizeof(XCHAR)),
  memcpy(this->applicationID, applicationID, (size+1)*sizeof(XCHAR));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::SetApplicationID(XSTRING& applicationID)
* @brief      Set Application ID
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:23:50
*
* @param[in]  applicationID : new string application ID
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::SetApplicationID(XSTRING& applicationID)
{
  SetApplicationID(applicationID.Get(), applicationID.GetSize());
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XDEBUGTRACE::GetPublicIPString()
* @brief      Get Public IP (in string format)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:25:17
*
* @return     XSTRING* : string with public IP
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XDEBUGTRACE::GetPublicIPString()
{ 
  return &publicIPstring;                   
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::SetPublicIPString(XSTRING* publicIPstring)
* @brief      Set public IP (in string format)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:27:13
*
* @param[in]  publicIPstring : public ip in string format (xxx.xxx.xxx.xxx)
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::SetPublicIPString(XSTRING* publicIPstring)
{
  int data[4];

  publicIPstring->UnFormat(__L("%d.%d.%d.%d"), &data[0], &data[1], &data[2], &data[3]);

  publicIP  |= (data[0] << 24);
  publicIP  |= (data[1] << 16);
  publicIP  |= (data[2] << 8);
  publicIP  |= (data[3]);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XDEBUGTRACE::GetLocalIPString()
* @brief      Get Local IP (in string format)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:29:12
*
* @return     XSTRING* : local ip in string format (xxx.xxx.xxx.xxx)
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XDEBUGTRACE::GetLocalIPString()
{ 
  return &localIPstring;                    
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::SetLocalIPString(XSTRING* localIPstring)
* @brief      Set Local IP (in string format)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:29:57
*
* @param[in]  localIPstring : new local ip in string format (xxx.xxx.xxx.xxx)
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::SetLocalIPString(XSTRING* localIPstring)
{
  int data[4];

  localIPstring->UnFormat(__L("%d.%d.%d.%d"), &data[0], &data[1], &data[2], &data[3]);

  localIP  |= (data[0] << 24);
  localIP  |= (data[1] << 16);
  localIP  |= (data[2] << 8);
  localIP  |= (data[3]);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XDEBUGTRACE::GetPublicIP()
* @brief      Get Public IP (in DWORD format)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:34:22
*
* @return     XDWORD : public IP in DWORD format
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XDEBUGTRACE::GetPublicIP() 
{ 
  return publicIP;                          
}


    
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XDEBUGTRACE::GetLocalIP()
* @brief      Get Local IP (in DWORD format)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:35:49
*
* @return     XDWORD : local IP in DWORD format
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XDEBUGTRACE::GetLocalIP()
{ 
  return localIP;                           
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCHAR* XDEBUGTRACE::GetWebScraperScriptURLDownload()
* @brief      Get Web Scrapper Script URL download
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:37:23
*
* @return     XCHAR* : URL download
*
*---------------------------------------------------------------------------------------------------------------------*/
XCHAR* XDEBUGTRACE::GetWebScraperScriptURLDownload()
{ 
  return webscraperscripturldownload;       
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::SetWebScraperScriptURLDownload(XCHAR* webscraperscripturldownload)
* @brief      Get Web Scrapper Script URL download
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:38:10
*
* @param[in]  webscraperscripturldownload : URL download
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::SetWebScraperScriptURLDownload(XCHAR* webscraperscripturldownload)
{
  XDWORD size = 0;
  if(webscraperscripturldownload) size=XSTRING::GetSize(webscraperscripturldownload);

  memset(this->webscraperscripturldownload, 0, _MAXSTR * sizeof(XCHAR));
  if(webscraperscripturldownload) memcpy(this->webscraperscripturldownload, webscraperscripturldownload, (size + 1) * sizeof(XCHAR));
}

    

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::PrintHeader(XCHAR* header)
* @brief      Print Header 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:39:20
*
* @param[in]  header : header string
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::PrintHeader(XCHAR* header)
{
  XSTRING line;

  line.Empty();

  line  = __L("---");

  if(header)
    {
      line += __L(" ");
      line += header;
      line += __L(" ");
    }

  while(line.GetSize()<64)
    {
      line += __L("-");
    }

  Print(0,line.Get());

  openheader = header?true:false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::Print(XBYTE level, XCHAR* mask,...)
* @brief      Print format line
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:40:07
*
* @param[in]  level : level of line
* @param[in]  mask : mask of format of print
* @param[in]  ... : variables to print
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::Print(XBYTE level, XCHAR* mask,...)
{
  XSTRING string;
  XSTRING outstring;

  string.Add(applicationname);
  if(string.GetSize())
    {
      string.AdjustSize(XDEBUGTRACE_MAXAPPLICATIONNAMESIZE, false, __L(" "));
      outstring += string;
    }

  string.Format(__L("%d.%d.%d"), applicationversion, applicationsubversion, applicationsubversionerr);
  if(string.GetSize())
    {
      string.AdjustSize(XDEBUGTRACE_MAXAPPLICATIONVERSIONSIZE, false, __L(" "));
      outstring += string;
    }

  string = applicationID;
  if(string.GetSize())
    {
      string.AdjustSize(XDEBUGTRACE_MAXAPPLICATIONIDSIZE, false, __L(" "));
      outstring += string;
    }

  va_list arg;

  va_start(arg, mask);

  string.FormatArg(mask, &arg);

  va_end(arg);

  sequence++;

  outstring += string;

  for(int c=0; c<XDEBUGTRACE_MAXNTARGETS ; c++)
    {
      switch(targets[c].GetType())
        {
          case XDEBUGTRACE_TYPE_SPECIAL   : PrintSpecial(&targets[c], level, outstring.Get());  break;
          case XDEBUGTRACE_TYPE_FILE      : PrintFile(&targets[c], level, outstring.Get());     break;
          case XDEBUGTRACE_TYPE_NET       : PrintNet(&targets[c], level, outstring.Get());      break;
                          default       : break;
        }
    }

  return true;
}





/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::PrintDataBlock(XBYTE level, XBYTE* data, XDWORD size, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
* @brief      Print data block
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:42:49
*
* @param[in]  level : level of line
* @param[in]  data : buffer of data to print
* @param[in]  size : size of buffer
* @param[in]  marginsize : size of margin
* @param[in]  sizeline : size of line
* @param[in]  showoffset : true if had offset
* @param[in]  showtext : true if show assci of buffer
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::PrintDataBlock(XBYTE level, XBYTE* data, XDWORD size, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  XSTRING    margin;
  XSTRING    strdata;
  XDWORD     _size     = 0;
  XDWORD     _sizeline = sizeline;
  int        index     = 0;

  for(XDWORD c=0; c<marginsize; c++)
    {
      margin += __L(" ");
    }

  while(_size<size)
    {
      XSTRING string;

      string   = margin;
      if(_sizeline > (size - _size))
         _sizeline = (size - _size);

      if(showoffset)
        {
          strdata.Format(__L("%04X"),index);
          string += strdata;
          string += __L("   ");
        }

      for(XDWORD c=0; c<_sizeline; c++)
        {
          strdata.Format(__L("%02X "),data[index]);
          string += strdata;
          _size++;
          index++;
        }

      //if(_sizeline != sizeline)
        {
          for(XDWORD c=_sizeline; c<sizeline; c++)
            {
              string += __L("   ");
            }
        }


      if(showtext)
        {
          index -= _sizeline;
          string += __L(" ");

          for(XDWORD c=0; c<_sizeline; c++)
            {
              XCHAR character = (XCHAR)data[index];

              if((character<0x20) || (character>0x80) || (character==__C('%')) || (character==__C('\\'))) character = __C('.');

              strdata.Format(__L("%c"),character);
              string += strdata;
              index++;
            }
        }

      Print(level, string.Get());

      if(_size>XDEBUGTRACE_MAXSIZEDATABLOCK) break;
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::PrintDataBlock(XBYTE level, XBUFFER& data, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
* @brief      Print data block
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:54:10
*
* @param[in]  level : level of line
* @param[in]  data : buffer of data to print
* @param[in]  marginsize : size of margin
* @param[in]  sizeline : size of line 
* @param[in]  showoffset : true if had offset 
* @param[in]  showtext : true if show assci of buffer
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::PrintDataBlock(XBYTE level, XBUFFER& data, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  return PrintDataBlock(level,data.Get(), data.GetSize(), marginsize, sizeline, showoffset, showtext);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::PrintSpecial(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print Special aim
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:47:30
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::PrintSpecial(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
{                                           

}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::PrintFile(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print in file aim
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:49:22
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::PrintFile(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)              
{                                           

}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::PrintNet(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print net aim
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:50:23
*
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::PrintNet(XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string)              
{                                           

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::GetHandleNet(XDEBUGTRACE_TARGET* target)
* @brief      Get Handle NET (depends of plataform)
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:51:32
*
* @param[in]  target : target of debug
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::GetHandleNet(XDEBUGTRACE_TARGET* target)                                          
{ 
  return false;                             
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::CloseHandleNet(XDEBUGTRACE_TARGET* target)
* @brief      Close Handle NET (depends of plataform)
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:52:07
*
* @param[in]  target : target of debug
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::CloseHandleNet(XDEBUGTRACE_TARGET* target)                                          
{ 
  return false;                             
}

    
    
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XDEBUGTRACE::GetDebugFromXBuffer(XBUFFER& xbufferpacket, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string)
* @brief      Get Debug pack From XBuffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:59:25
*
* @param[in]  xbufferpacket : pack of debug
* @param[out] publicIP : Public IP (in DWORD format)
* @param[out] localIP : Local IP (in DWORD format)
* @param[out] level : Level of debug
* @param[out] sequence : sequence of debug
* @param[out] xtime : date/time of line
* @param[out] string : string with of line
*
* @return     XDWORD : 0 if is successful ok, other number error
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XDEBUGTRACE::GetDebugFromXBuffer(XBUFFER& xbufferpacket, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string)
{
  if(!xtime)                   return 1;
  if(!xbufferpacket.GetSize()) return 1;

  XDWORD    IDpacket        = 0;
  XDWORD    sizepacketread  = 0;
  XDWORD    dword;
  XDWORD    sizeinbuffer    = xbufferpacket.GetSize();
  XBUFFER   xbuffer;
  XDWORD    sizeheader      = (sizeof(XDWORD)    +    // ID.
                               sizeof(XDWORD)    +    // SizePacket.
                               sizeof(XDWORD)    +    // public IP address origin.
                               sizeof(XDWORD)    +    // local  IP address origin.
                               sizeof(XBYTE)     +    // Level.
                               sizeof(XDWORD)    +    // Secuence.
                              (sizeof(XDWORD)*7) +    // Date Time.
                               sizeof(XWORD));        // CRC16

  if(!sizeinbuffer)             return 1;
  if(sizeinbuffer < sizeheader) return 1;


  while(sizeinbuffer-4)
    {
      xbufferpacket.Get(IDpacket, 0);
      if(IDpacket == XDEBUGTRACE_IDPACKET) break;

      xbufferpacket.Extract(NULL, 0, 1);
      sizeinbuffer--;
    }

  if(IDpacket != XDEBUGTRACE_IDPACKET) return 1;

  if(!sizeinbuffer)               return 1;
  if(sizeinbuffer < sizeheader)   return 1;

  if(!xbufferpacket.Get(sizepacketread, sizeof(IDpacket)))
    {
      return 1;
    }

  if(!sizepacketread)
    {
      return 2;
    }

  if(sizepacketread > sizeinbuffer)
    {
      return 1;
    }

  //xbuffer.Resize(sizepacketread);

  XDWORD sizeassing    = sizepacketread;
  XDWORD sizeassingmin = 5*1024;

  if(sizeassing < sizeassingmin) sizeassing = sizeassingmin;
  xbuffer.Resize(sizeassing);



  xbufferpacket.Extract(xbuffer.Get(), 0, sizepacketread);

  XWORD crc16;
  XDEBUGTRACE_CRC16(xbuffer.Get(), (xbuffer.GetSize()-sizeof(XWORD)), crc16)

  XWORD crc16read;

  xbuffer.Get(crc16read, xbuffer.GetSize()-sizeof(XWORD));
  if(crc16 != crc16read)
    {
      return 2;
    }

  xbuffer.SetPosition(0);

  xbuffer.Get(IDpacket);
  xbuffer.Get(sizepacketread);
  xbuffer.Get(publicIP);
  xbuffer.Get(localIP);
  xbuffer.Get(level);
  xbuffer.Get(sequence);

  xbuffer.Get(dword); xtime->SetYear(dword);
  xbuffer.Get(dword); xtime->SetMonth(dword);
  xbuffer.Get(dword); xtime->SetDay(dword);
  xbuffer.Get(dword); xtime->SetHours(dword);
  xbuffer.Get(dword); xtime->SetMinutes(dword);
  xbuffer.Get(dword); xtime->SetSeconds(dword);
  xbuffer.Get(dword); xtime->SetMilliSeconds(dword);

  XDWORD sizestr = sizepacketread - sizeheader;        // CRC16
  sizestr /= sizeof(XWORD);

  for(XDWORD c=0; c<(sizestr-1); c++)
    {
      XWORD  word;
      if(!xbuffer.Get(word)) break;

      string.Add((XCHAR)word);
    }

  return 0;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XDEBUGTRACE::GetDebugFromStreamIO(DIOSTREAM* streamIO, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string)
* @brief      Get Debug pack From Stream IO
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:04:44
*
* @param[in]  streamIO : stream IO to read packet of debug
* @param[out] publicIP : Public IP (in DWORD format)
* @param[out] localIP : Local IP (in DWORD format)
* @param[out] level : Level of debug
* @param[out] sequence : sequence of debug
* @param[out] xtime : date/time of line
* @param[out] string : string with of line
*
* @return     XDWORD : 0 if is successful ok, other number error
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XDEBUGTRACE::GetDebugFromStreamIO(DIOSTREAM* streamIO, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string)
{
  if(!streamIO) return false;
  if(!xtime)    return false;

  return GetDebugFromXBuffer((*streamIO->GetInXBuffer()), publicIP, localIP, level, sequence, xtime, string);
}






/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::SetDebugToXBuffer(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XCHAR* string, XBUFFER& xbufferpacket)
* @brief      Set Debug pack to XBuffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:07:05
*
* @param[in]  publicIP : Public IP (in DWORD format)
* @param[in]  localIP : Local IP (in DWORD format)
* @param[in]  level : Level of debug
* @param[in]  sequence : sequence of debug
* @param[in]  xtime : date/time of line
* @param[in]  string : string with of line
* @param[out] xbufferpacket : buffer with debug pack
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::SetDebugToXBuffer(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XCHAR* string, XBUFFER& xbufferpacket)
{
  if(!xtime)  return false;
  if(!string) return false;

  XSTRING  line;
  XDWORD   IDpacket;
  XDWORD   sizepacket;

  xbufferpacket.Empty();

  line        = string;
  IDpacket    = XDEBUGTRACE_IDPACKET;
  sizepacket  = sizeof(XDWORD) +                        // ID.
                sizeof(XDWORD) +                        // SizePacket.
                sizeof(XDWORD) +                        // public IP address origin.
                sizeof(XDWORD) +                        // local  IP address origin.
                sizeof(XBYTE)  +                        // Level.
                sizeof(XDWORD) +                        // Secuence.
                (sizeof(XDWORD)*7) +                    // Date Time.
                ((line.GetSize()+1)*sizeof(XWORD)) +    // Line (WORD) each character.
                sizeof(XWORD);                          // CRC16

  xbufferpacket.Resize(sizepacket);

  xbufferpacket.Set(IDpacket);
  xbufferpacket.Set(sizepacket);
  xbufferpacket.Set(publicIP);
  xbufferpacket.Set(localIP);
  xbufferpacket.Set(level);
  xbufferpacket.Set(sequence);

  xbufferpacket.Set((XDWORD)xtime->GetYear());
  xbufferpacket.Set((XDWORD)xtime->GetMonth());
  xbufferpacket.Set((XDWORD)xtime->GetDay());
  xbufferpacket.Set((XDWORD)xtime->GetHours());
  xbufferpacket.Set((XDWORD)xtime->GetMinutes());
  xbufferpacket.Set((XDWORD)xtime->GetSeconds());
  xbufferpacket.Set((XDWORD)xtime->GetMilliSeconds());

  XSTRING_CREATENORMALIZE(line, charstr, false)

  for(int c=0; c<(int)line.GetSize()+1; c++)
    {
      xbufferpacket.Set((XWORD)charstr[c]);
    }

  XSTRING_DELETENORMALIZE(charstr)

  XWORD crc16;
  XDEBUGTRACE_CRC16(xbufferpacket.Get(), (xbufferpacket.GetSize()-sizeof(XWORD)), crc16)

  xbufferpacket.Set((XWORD)crc16);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::ReConnectedAllNetTargets()
* @brief      Re connect all Net Targets
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:10:54
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::ReConnectedAllNetTargets()
{
  for(int c=0; c<XDEBUGTRACE_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XDEBUGTRACE_TYPE_NET)
        {
          if(targets[c].GetNETHandle()) CloseHandleNet(&targets[c]);

          GetHandleNet(&targets[c]);
        }
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::ResolvedAllNetTargets(bool& changed)
* @brief      Resolved All Net Targets
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:11:23
*
* @param[out] changed : true if the target net changed IP
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::ResolvedAllNetTargets(bool& changed)
{
  changed = false;

  for(int c=0; c<XDEBUGTRACE_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XDEBUGTRACE_TYPE_NET)
        {
          XSTRING IP;

          IP = targets[c].GetIP();

          targets[c].ResolvedIPTarget();

          if(IP.Compare(targets[c].GetIP())) changed = true;
        }
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::ObtainLocalIP()
* @brief      Obtain Local IP
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:14:24
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::ObtainLocalIP()
{
  bool status = false;

  #ifndef MICROCONTROLLER

  DIOSTREAMIPLOCALENUMDEVICES* enumdevices = (DIOSTREAMIPLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_IP_LOCAL);
  if(enumdevices)
    {
      DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetFirstActiveDevice();
      if(device)
        {
          XSTRING string;

          device->GetIP()->GetXString(string);
          SetLocalIPString(&string);

          status = true;
        }

      diofactory->DeleteStreamEnumDevices(enumdevices);
    }

  #endif

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::ObtainPublicIP()
* @brief      Obtain Public IP
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:14:48
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::ObtainPublicIP()
{
  bool  status = false;

  #ifndef MICROCONTROLLER

  DIOIP ip;
  DIOWEBSCRAPERPUBLICIP* publicip = new DIOWEBSCRAPERPUBLICIP();
  if(!publicip) return false;

  publicip->GetURLDownload()->Set(webscraperscripturldownload);

  if(publicip->Get(ip, 5))
    {
      XSTRING string;

      ip.GetXString(string);
      SetPublicIPString(&string);

    }

  delete publicip;

  #endif

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::ObtainResourcesIP()
* @brief      Obtain Resources IP (public + local)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:15:21
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::ObtainResourcesIP()
{
  if(!ObtainLocalIP())  return false;
  if(!ObtainPublicIP()) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::ResolvedAllResources()
* @brief      Resolved All Resources local + public: (if chnaged obtain and reconnect)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:15:53
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::ResolvedAllResources()
{
  bool changed;

  ResolvedAllNetTargets(changed);
  if(changed)
    {
      ObtainResourcesIP();
      ReConnectedAllNetTargets();
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XDEBUGTRACE::GenerateTab(int ntab, XSTRING& tab)
* @brief      Generate Tab for the line
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 19:17:20
*
* @param[in]  ntab : number of tab
* @param[out  tab : string with the tab generated
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XDEBUGTRACE::GenerateTab(int ntab, XSTRING& tab)
{
  tab.Empty();

  for(int c=0;c<ntab;c++)
    {
      tab += __L("  ");
    }

  return true;
}

    


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XDEBUGTRACE::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       31/05/2018 18:58:22
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XDEBUGTRACE::Clean()
{
  instance                  = NULL;

  sizelimit                 = XDEBUGTRACE_SIZELIMITDEFAULT;

  for(int c=0; c<_MAXSTR; c++)
    {
      applicationname[c] = 0;
      applicationID[c]   = 0;
    }

  applicationversion        = 0;
  applicationsubversion     = 0;
  applicationsubversionerr  = 0;

  publicIP                  = 0;
  localIP                   = 0;

  openheader                = false;
  sequence                  = 0;  
}











