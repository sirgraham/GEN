/*------------------------------------------------------------------------------------------
//  DIODYNDNS.H
*/
/**
// \class
//
//  Data IO DynDNS interface class
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 08/05/2014 16:53:48
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIODYNDNS_H_
#define _DIODYNDNS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/


#include <stdio.h>

#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DIODYNDNS_STATUSCODE
{
  DIODYNDNS_STATUSCODE_UNKNOWN     = -2 ,   // Unknown result from dyndns service.
  DIODYNDNS_STATUSCODE_NOTCONNECT       ,   // Not connected.
  DIODYNDNS_STATUSCODE_GOOD             ,   // "good"         The update was successful, and the hostname is now updated.
  DIODYNDNS_STATUSCODE_NOCHG            ,   // "nochg"        The update changed no settings, and is considered abusive. (Additional nochg updates will cause the hostname to become blocked).
  DIODYNDNS_STATUSCODE_BADSYS           ,   // "badsys"       The system parameter given is not valid. Valid system parameters are dyndns, statdns and custom.
  DIODYNDNS_STATUSCODE_BASAGENT         ,   // "badagent"     The user agent that was sent has been blocked for not following these specifications or no user agent was specified.
  DIODYNDNS_STATUSCODE_BADAUTH          ,   // "badauth"      The username or password specified are incorrect.
  DIODYNDNS_STATUSCODE_DONATOR          ,   // "!donator"     An option available only to credited users (suchas offline URL) was specified, but the user is not a credited user.
  DIODYNDNS_STATUSCODE_NOTFQDN          ,   // "notfqdn"      The hostname specified is not a fully-qualified domain name (not in the form hostname.dyndns.org or domain.com).
  DIODYNDNS_STATUSCODE_NOHOST           ,   // "nohost"       The hostname specified does not exist (or is not in the service specified in the system parameter).
  DIODYNDNS_STATUSCODE_YOURS            ,   // "!yours"       The hostname specified exists, but not under the username specified.
  DIODYNDNS_STATUSCODE_ABUSE            ,   // "abuse"        The hostname specified is blocked for update abuse.
  DIODYNDNS_STATUSCODE_NUMHOST          ,   // "numhost"      Too many or too few hosts found.
  DIODYNDNS_STATUSCODE_DNSERR           ,   // "dnserr"       DNS error encountered.
  DIODYNDNS_STATUSCODE_911              ,   // "911"          There is a serious problem on our side, such as a database or DNS server failure. The client should stop updating until notified via the status page that the service is back up.
};



#define DIODYNDNS_DEFAULTTIMEOUT  10

/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class DIOFACTORY;
class XPUBLISHER;
class DIOURL;
class DIOIP;
class DIOWEBCLIENT;


class DIODYNDNS
{
  public:

                            DIODYNDNS           ();
    virtual                ~DIODYNDNS           ();

    XSTRING*                GetLogin            ()                        { return &login;                }
    XSTRING*                GetPassword         ()                        { return &password;             }

    bool                    GetPublicIP         (DIOIP& publicIP, int timeout = DIODYNDNS_DEFAULTTIMEOUT, XSTRING* IPlocal = NULL) ;
    bool                    IsChangedPublicIP   (DIOURL& url, bool& ischanged, DIOIP* newpublicip = NULL, int timeout = DIODYNDNS_DEFAULTTIMEOUT, XSTRING* IPlocal = NULL);


    DIODYNDNS_STATUSCODE    Update              (DIOURL& url, DIOIP& newpublicip, int timeout = DIODYNDNS_DEFAULTTIMEOUT, XSTRING* IPlocal = NULL);


  private:

    void                    Clean               ()
                            {
                              webclient   = NULL;
                            }

    DIODYNDNS_STATUSCODE    GetError            (XSTRING& result);


    DIOWEBCLIENT*           webclient;

    XSTRING                 login;
    XSTRING                 password;

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif



