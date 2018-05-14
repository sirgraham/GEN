
/*------------------------------------------------------------------------------------------
//  DIOEMAILADDRESS.CPP
//
//  Data IO Email Address class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 27/05/2014 15:55:17
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOEmailAddress.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  DIOEMAILADDRESS::GetUser
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      27/05/2014 16:00:18
//
//  @return       bool :
//
//  @param        user :
*/
/*-----------------------------------------------------------------*/
bool DIOEMAILADDRESS::GetUser(XSTRING& user)
{
  user.Empty();

  int end = Find(DIOEMAILADDRESS_SEPARATOR, true);

  if(end == XSTRING_NOTFOUND) return false;
  if(!end)                  return false;

  return Copy(0, end, user)?true:false;
}



/*-------------------------------------------------------------------
//  DIOEMAILADDRESS::GetDomain
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      27/05/2014 16:00:21
//
//  @return       bool :
//
//  @param        domain :
*/
/*-----------------------------------------------------------------*/
bool DIOEMAILADDRESS::GetDomain(XSTRING& domain)
{
  domain.Empty();

  int start = Find(DIOEMAILADDRESS_SEPARATOR, true);

  if(start == XSTRING_NOTFOUND) return false;
  if(!start)                    return false;

  return Copy(start+1, domain)?true:false;
}



/*-------------------------------------------------------------------
//  DIOEMAILADDRESS::IsValid
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      27/05/2014 16:18:27
//
//  @return       bool :
//

*/
/*-----------------------------------------------------------------*/
bool DIOEMAILADDRESS::IsValid()
{
  XSTRING  user;
  XSTRING  domain;
  bool     status = false;

  if(!GetUser(user))      return false;
  if(!GetDomain(domain))  return false;

  DIOIP   IP;
  DIOURL* url = diofactory->CreateURL();
  if(!url) return false;

  url->Set(domain);

  status = url->ResolveURL(IP);

  diofactory->DeleteURL(url);

  return status;
}
