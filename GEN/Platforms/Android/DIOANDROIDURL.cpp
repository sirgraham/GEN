/*------------------------------------------------------------------------------------------
//  DIOANDROIDURL.CPP
//
//  Data I/O URL Class
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

#include "DIOANDROIDURL.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOANDROIDURL::DIOANDROIDURL
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
DIOANDROIDURL::DIOANDROIDURL() : DIOURL()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOANDROIDURL::DIOANDROIDURL
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
DIOANDROIDURL::DIOANDROIDURL(int size) : DIOURL(size)
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOANDROIDURL::DIOANDROIDURL
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
DIOANDROIDURL::DIOANDROIDURL(const char* url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOANDROIDURL::DIOANDROIDURL
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
DIOANDROIDURL::DIOANDROIDURL(const XCHAR* url) : DIOURL(url)
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOANDROIDURL::DIOANDROIDURL
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
DIOANDROIDURL::DIOANDROIDURL(const XCHAR* url,int size) : DIOURL(url,size)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOANDROIDURL::DIOANDROIDURL
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
DIOANDROIDURL::DIOANDROIDURL(DIOURL& url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOANDROIDURL::DIOANDROIDURL
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
DIOANDROIDURL::DIOANDROIDURL(XSTRING& string) : DIOURL(string)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOANDROIDURL::DIOANDROIDURL
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
DIOANDROIDURL::DIOANDROIDURL(XWORD* url) : DIOURL(url)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOANDROIDURL::~DIOANDROIDURL
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
DIOANDROIDURL::~DIOANDROIDURL()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOANDROIDURL::ResolveURL
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
bool DIOANDROIDURL::ResolveURL(XSTRING& IPstring)
{
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
//  DIOANDROIDURL::Clean
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
void DIOANDROIDURL::Clean()
{

}

