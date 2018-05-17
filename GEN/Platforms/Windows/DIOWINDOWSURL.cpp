/*------------------------------------------------------------------------------------------
//  DIOWINDOWSURL.CPP
//
//  Windows Data I/O URL Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/02/2012 10:38:07
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "DIOWINDOWSURL.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOWINDOWSURL::DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:39:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::DIOWINDOWSURL() : DIOURL()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSURL::DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:39:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::DIOWINDOWSURL(XDWORD size) : DIOURL(size)
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOWINDOWSURL::DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:39:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::DIOWINDOWSURL(const char* url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSURL::DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:39:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::DIOWINDOWSURL(const XCHAR* url) : DIOURL(url)
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOWINDOWSURL::DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:39:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::DIOWINDOWSURL(const XCHAR* url,XDWORD size) : DIOURL(url,size)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSURL::DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:39:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::DIOWINDOWSURL(DIOURL& url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSURL::DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:39:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::DIOWINDOWSURL(XSTRING& string) : DIOURL(string)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSURL::DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:39:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::DIOWINDOWSURL(XWORD* url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSURL::~DIOWINDOWSURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:38:59
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSURL::~DIOWINDOWSURL()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOWINDOWSURL::ResolveURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/02/2013 22:44:28
//
//  @return       bool :
//  @param        IP :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSURL::ResolveURL(XSTRING& IPstring)
{
  if(IsAURLResolved())
    {
      IPstring = Get();
      return true;
    }

  if(IsEmpty()) return true;

  #ifdef BUILDER

  SOCKADDR_IN     addr;
  char*           saddr;
  struct hostent* localhost;

  XSTRING_CREATEOEM((*this), charstr)
  localhost = gethostbyname(charstr);
  XSTRING_DELETEOEM(charstr)

  if(!localhost) return false;

  saddr = localhost->h_addr_list[0];
  memcpy(&addr.sin_addr.s_addr, saddr,4);

  IPstring = inet_ntoa(addr.sin_addr);

  #else

  SOCKADDR_IN*            addr;
  struct addrinfo*        res;
  int                     status;

  XSTRING_CREATEOEM((*this), charstr)
  status = getaddrinfo(charstr, NULL, NULL, &res);
  XSTRING_DELETEOEM(charstr)

  if(status!=0) return false;

  addr = (SOCKADDR_IN*) res->ai_addr;

  char IPcharstring[_MAXSTR];
  inet_ntop(AF_INET, &(addr->sin_addr), IPcharstring, _MAXSTR);
  IPstring = IPcharstring;

  #endif

  return IPstring.IsEmpty()?false:true;
}




/*-------------------------------------------------------------------
//  DIOWINDOWSURL::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:38:49
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWINDOWSURL::Clean()
{

}

