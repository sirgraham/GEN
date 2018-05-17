/*------------------------------------------------------------------------------------------
//  DIOLINUXURL.CPP
//
//  Linux Data I/O URL Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/02/2012 10:38:07
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "DIOLINUXURL.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOLINUXURL::DIOLINUXURL
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
DIOLINUXURL::DIOLINUXURL() : DIOURL()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXURL::DIOLINUXURL
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
DIOLINUXURL::DIOLINUXURL(XDWORD size) : DIOURL(size)
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOLINUXURL::DIOLINUXURL
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
DIOLINUXURL::DIOLINUXURL(const char* url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXURL::DIOLINUXURL
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
DIOLINUXURL::DIOLINUXURL(const XCHAR* url) : DIOURL(url)
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOLINUXURL::DIOLINUXURL
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
DIOLINUXURL::DIOLINUXURL(const XCHAR* url,XDWORD size) : DIOURL(url,size)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXURL::DIOLINUXURL
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
DIOLINUXURL::DIOLINUXURL(DIOURL& url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXURL::DIOLINUXURL
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
DIOLINUXURL::DIOLINUXURL(XSTRING& string) : DIOURL(string)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXURL::DIOLINUXURL
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
DIOLINUXURL::DIOLINUXURL(XWORD* url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXURL::~DIOLINUXURL
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
DIOLINUXURL::~DIOLINUXURL()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOLINUXURL::ResolveURL
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
bool DIOLINUXURL::ResolveURL(XSTRING& IPstring)
{
  if(IsAURLResolved())
    {
      IPstring = Get();
      return true;
    }

  struct sockaddr_in  addr = { 0 };
  char*               saddr     = NULL;
  struct hostent*     localhost;

  XSTRING_CREATEOEM((*this), charOEM)
  localhost = gethostbyname(charOEM);
  XSTRING_DELETEOEM(charOEM)

  if(!localhost) return false;

  saddr = localhost->h_addr_list[0];
  memcpy(&addr.sin_addr.s_addr, saddr,4);

  IPstring = inet_ntoa(addr.sin_addr);

  return IPstring.IsEmpty()?false:true;
}




/*-------------------------------------------------------------------
//  DIOLINUXURL::Clean
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
void DIOLINUXURL::Clean()
{

}

