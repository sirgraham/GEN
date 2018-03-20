/*------------------------------------------------------------------------------------------
//  DIOCAPEX.CPP
//  
//  Interface PCap Extended Library class 
//   
//  Author            : Abraham J. Velez
//  Date Of Creation  : 22/10/2012 13:30:11
//  Last Mofificacion : 
//  
//  GEN  Copyright (C).  All right reserved. 
//----------------------------------------------------------------------------------------*/
  
#ifdef DIOPCAP_ACTIVE
  
/*---- INCLUDES --------------------------------------------------------------------------*/
  
#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XDebug.h"

#include "DIOPCapEX.h"

#include "XMemory.h"
  
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
  
  
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOPCAPFRAMEEX::DIOPCAPFRAMEEX
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      26/10/2012 13:06:09
//  
//  @return       void : 
//  @param        xfactory : 
*/
/*-----------------------------------------------------------------*/
DIOPCAPFRAMEEX::DIOPCAPFRAMEEX() : DIOPCAPFRAME()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOPCAPFRAMEEX::~DIOPCAPFRAMEEX
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      23/10/2012 16:57:45
//  
//  @return       virtual : 
//  */
/*-----------------------------------------------------------------*/
DIOPCAPFRAMEEX::~DIOPCAPFRAMEEX()
{
  Clean(); 
}




/*-------------------------------------------------------------------
//  DIOPCAPFRAMEEX::GetDNSAsk
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      29/10/2012 10:33:11
//  
//  @return       bool : 
//  @param        header : 
//  @param        url : 
*/
/*-----------------------------------------------------------------*/
bool DIOPCAPFRAMEEX::GetDNSAsk(DIOPCAPEXDNSHEADER& header,DIOPCAPEXDNSASK& ask)
{
  DIOPCAPEXDNSHEADER* _header = (DIOPCAPEXDNSHEADER*)UserData_Get();
  if(!_header) return false;

  memcpy((XBYTE*)&header,(XBYTE*)(_header),sizeof(DIOPCAPEXDNSHEADER));

  if(xfactory->HardwareUseLittleEndian())
    {          
      SWAPWORD(header.ID);   
      SWAPWORD(header.QDcount);    
      SWAPWORD(header.ANcount);    
      SWAPWORD(header.NScount);    
      SWAPWORD(header.ARcount);    
    }
 
  XBYTE* _url = (XBYTE*)_header;
  _url+=sizeof(DIOPCAPEXDNSHEADER);
  
  int size = DNSnameFormatToString(_url,ask.url);

  XBYTE* data = (XBYTE*)(UserData_Get());   
  data+=(sizeof(DIOPCAPEXDNSHEADER)+size);

  XWORD* data2 = (XWORD*)data;
  ask.qclass = (*data2);   SWAPWORD(ask.qclass);   
  data2++;  
  ask.qtype  = (*data2);   SWAPWORD(ask.qtype);
  
  return true;
}



/*-------------------------------------------------------------------
//  DIOPCAPFRAMEEX::GetDNSRequest
*/ 
/**
//  ONLY INTERPRETS ONE ANSWER.
//  
//  @author       Abraham J. Velez
//  @version      29/10/2012 14:52:00
//  
//  @return       bool : 
//  @param        header : 
//  @param        request : 
*/
/*-----------------------------------------------------------------*/
bool DIOPCAPFRAMEEX::GetDNSRequest(DIOPCAPEXDNSHEADER& header,DIOPCAPEXDNSREQUEST& request)
{
  DIOPCAPEXDNSHEADER* _header = (DIOPCAPEXDNSHEADER*)UserData_Get();
  if(!_header) return false;

  memcpy((XBYTE*)&header,(XBYTE*)(_header),sizeof(DIOPCAPEXDNSHEADER));

  if(xfactory->HardwareUseLittleEndian())
    {          
      SWAPWORD(header.ID);   
      SWAPWORD(header.QDcount);    
      SWAPWORD(header.ANcount);    
      SWAPWORD(header.NScount);    
      SWAPWORD(header.ARcount);    
    }
  
  XBYTE* _url = (XBYTE*)_header;
  _url+=sizeof(DIOPCAPEXDNSHEADER);

  for(int c=0;c<header.ANcount;c++)
    {               
      int size = DNSnameFormatToString(_url,request.url);

      XBYTE*  data = (XBYTE*)(UserData_Get());   
      data+=(sizeof(DIOPCAPEXDNSHEADER)+size);

      XWORD*  data2 = (XWORD*)data;
      
      request.qclass      = (*data2);  SWAPWORD(request.qclass);      data2++;  
      request.qtype       = (*data2);  SWAPWORD(request.qtype);       data2++; 
      request.ttl         = (*data2);  SWAPWORD(request.ttl);         data2++;  
      request.rlenght     = (*data2);  SWAPWORD(request.rlenght);     data2++;  
      request.preference  = (*data2);  SWAPWORD(request.preference);  data2++;  
      request.exchange    = (*data2);  SWAPWORD(request.exchange);    data2++;   
      
      data = (XBYTE*)data2;
      request.address.byte1 = (*data); data++;     
      request.address.byte2 = (*data); data++;     
      request.address.byte3 = (*data); data++;     
      request.address.byte4 = (*data); data++;     
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOPCAPFRAMEEX::DNSnameFormatToString
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      29/10/2012 11:47:34
//  
//  @return       int : 
//  @param        urlDNS : 
//  @param        url : 
*/
/*-----------------------------------------------------------------*/
int DIOPCAPFRAMEEX::DNSnameFormatToString(XBYTE* urlDNS,DIOURL& url)
{
  int c = 0;

  while(urlDNS[c])
   {
     int counter = urlDNS[c];     
     
     c++;

     for(int d=0;d<counter;d++)
       {
         url += urlDNS[c];
         c++;
       }

     if(urlDNS[c]) url+=__L(".");
   }

  return c+1;
}



/*-------------------------------------------------------------------
//  DIOPCAPFRAMEEX::StringToDNSNameFormat
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      29/10/2012 12:47:56
//  
//  @return       bool : 
//  @param        name : 
//  @param        nameDNS : 
*/
/*-----------------------------------------------------------------*/
bool DIOPCAPFRAMEEX::StringToDNSNameFormat(XSTRING& name,XBUFFER& nameDNS)
{
  /*
  XCHAR* _name = name.Get();

  nameDNS[0] = 
  */


  return true;
}



/*-------------------------------------------------------------------
//  DIOPCAPFRAMEEX::Clean
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      23/10/2012 16:58:49
//  
//  @return       void : 
//  */
/*-----------------------------------------------------------------*/
void DIOPCAPFRAMEEX::Clean()
{
 
}


#endif



