
/*------------------------------------------------------------------------------------------
//  DIODYNDNS.CPP
//
//  Data IO DynDNS interface class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 09/05/2014 12:59:47
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOFactory.h"
#include "DIOWebScraperPublicIP.h"
#include "DIOWebClient.h"

#include "DIODynDNS.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIODYNDNS::DIODYNDNS
*/
/**
//
//  Class Constructor DIODYNDNS
//
//  @author       Abraham J. Velez
//  @version      09/05/2014 13:01:01
//


//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIODYNDNS::DIODYNDNS()
{
  Clean();


  ;

  webclient = new DIOWEBCLIENT;
}




/*-------------------------------------------------------------------
//  DIODYNDNS::~DIODYNDNS
*/
/**
//
//   Class Destructor DIODYNDNS
//
//  @author       Abraham J. Velez
//  @version      09/05/2014 13:01:04
//
*/
/*-----------------------------------------------------------------*/
DIODYNDNS::~DIODYNDNS()
{
  delete webclient;

  Clean();
}



/*-------------------------------------------------------------------
//  DIODYNDNS::GetPublicIP
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/05/2014 8:49:27
//
//  @return       bool :
//
//  @param        publicIP :
//  @param        timeout :
//  @param        IPlocal :
*/
/*-----------------------------------------------------------------*/
bool DIODYNDNS::GetPublicIP(DIOIP& publicIP, int timeout, XSTRING* IPlocal)
{
  XBUFFER   xbuffer;
  bool      status;

  status = webclient->Get(__L("checkip.dyndns.com"), xbuffer, DIOWEBCLIENT_DEFAULTUSERAGENT, timeout, IPlocal);
  if(status)
    {
      XSTRING startstring;
      XSTRING result;

      result.Set(xbuffer.Get(), xbuffer.GetSize());

      startstring = __L("Current IP Address: ");

      int start = result.Find(startstring, true);
      if(start != XSTRING_NOTFOUND)
        {
          start += startstring.GetSize();

          int end  = result.Find(__L("</body>"), true, start);
          if(end != XSTRING_NOTFOUND)
            {
              XSTRING IPstring;

              result.Copy(start, end, IPstring);
              if(!IPstring.IsEmpty())
                {
                  return publicIP.Set(IPstring);
                }
            }
        }
    }
   else
    {
      DIOWEBSCRAPERPUBLICIP* webscraperpublicip = new DIOWEBSCRAPERPUBLICIP();
      if(webscraperpublicip)
        {
          status = webscraperpublicip->Get(publicIP, 5, NULL, false);
          delete webscraperpublicip;
        }
    }

  return status;
}



/*-------------------------------------------------------------------
//  DIODYNDNS::IsChangedPublicIP
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/05/2014 9:19:08
//
//  @return       bool :
//
//  @param        url :
//  @param        ischanged :
//  @param        timeout :
//  @param        IPlocal :
*/
/*-----------------------------------------------------------------*/
bool DIODYNDNS::IsChangedPublicIP(DIOURL& url, bool& ischanged, DIOIP* newpublicip, int timeout, XSTRING* IPlocal)
{
  ischanged = false;

  if(!diofactory) return false;

  DIOIP   publicIP[2];
  bool    status  = false;
  DIOURL* _url    = diofactory->CreateURL();
  if(!_url) return false;

  (*_url) = url;

  if(_url->ResolveURL(publicIP[0]))
    {
      if(GetPublicIP(publicIP[1], timeout, IPlocal))
        {
          ischanged = !publicIP[0].Compare(publicIP[1]);

          if(ischanged)
            {
              if(newpublicip) newpublicip->Set(publicIP[1].Get());
            }

          status = true;
        }
    }

  diofactory->DeleteURL(_url);

  return status;
}




/*-------------------------------------------------------------------
//  DIODYNDNS::Update
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/05/2014 9:03:19
//
//  @return       DIODYNDNS_STATUSCODE :
//
//  @param        url :
//  @param        newpublicip :
//  @param        timeout :
//  @param        IPlocal :
*/
/*-----------------------------------------------------------------*/
DIODYNDNS_STATUSCODE DIODYNDNS::Update(DIOURL& url, DIOIP& newpublicip, int timeout, XSTRING* IPlocal)
{
  if(!webclient) return DIODYNDNS_STATUSCODE_UNKNOWN;

  DIOURL    askurl;
  XSTRING   IPstring;
  XBUFFER   xbuffer;
  XSTRING   result;
  bool      status;

  newpublicip.GetXString(IPstring);

  askurl.Format(__L("%s:%s@members.dyndns.org/nic/update?hostname=%s&myip=%s&wildcard=NOCHG&mx=NOCHG&backmx=NOCHG"), login.Get(), password.Get(), url.Get(), IPstring.Get());

  status = webclient->Get(askurl.Get(), xbuffer, DIOWEBCLIENT_DEFAULTUSERAGENT, timeout, IPlocal);
  if(!status)  return DIODYNDNS_STATUSCODE_NOTCONNECT;

  result.Set(xbuffer.Get(), xbuffer.GetSize());

  return GetError(result);
}





/*-------------------------------------------------------------------
//  DIODYNDNS::GetError
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      13/05/2014 17:26:16
//
//  @return       DIODYNDNS_STATUSCODES :
//
//  @param        result :
*/
/*-----------------------------------------------------------------*/
DIODYNDNS_STATUSCODE DIODYNDNS::GetError(XSTRING& result)
{
  // Other          Unknown result from dyndns service.
  // "good"         The update was successful, and the hostname is now updated.
  // "nochg"        The update changed no settings, and is considered abusive. (Additional nochg updates will cause the hostname to become blocked).
  // "badsys"       The system parameter given is not valid. Valid system parameters are dyndns, statdns and custom.
  // "badagent"     The user agent that was sent has been blocked for not following these specifications or no user agent was specified.
  // "badauth"      The username or password specified are incorrect.
  // "!donator"     An option available only to credited users (suchas offline URL) was specified, but the user is not a credited user.
  // "notfqdn"      The hostname specified is not a fully-qualified domain name (not in the form hostname.dyndns.org or domain.com).
  // "nohost"       The hostname specified does not exist (or is not in the service specified in the system parameter).
  // "!yours"       The hostname specified exists, but not under the username specified.
  // "abuse"        The hostname specified is blocked for update abuse.
  // "numhost"      Too many or too few hosts found.
  // "dnserr"       DNS error encountered.
  // "911"          There is a serious problem on our side, such as a database or DNS server failure. The client should stop updating until notified via the status page that the service is back up.


  DIODYNDNS_STATUSCODE code              =  DIODYNDNS_STATUSCODE_UNKNOWN;
  XCHAR*               stringcodes[]     =  {  __L("good")      ,
                                               __L("nochg")     ,
                                               __L("badsys")    ,
                                               __L("badagent")  ,
                                               __L("badauth")   ,
                                               __L("!donator")  ,
                                               __L("notfqdn")   ,
                                               __L("nohost")    ,
                                               __L("!yours")    ,
                                               __L("abuse")     ,
                                               __L("numhost")   ,
                                               __L("dnserr")    ,
                                               __L("911")
                                            };


  for(int c=0; c<(sizeof(stringcodes)/sizeof(XCHAR*)); c++)
    {
      if(result.Find(stringcodes[c],true) != XSTRING_NOTFOUND)
        {
          code = (DIODYNDNS_STATUSCODE)(c);
          break;
        }
    }

  return code;

}




