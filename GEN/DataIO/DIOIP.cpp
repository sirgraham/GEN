/*------------------------------------------------------------------------------------------
//  DIOIP.CPP
//
//  Data IO IP class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 30/08/2012 9:14:11
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "DIOURL.h"
#include "DIOIP.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


//-------------------------------------------------------------------
//  DIOIP:: DIOIP
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:31
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOIP:: DIOIP()
{
  Clean();
}




//-------------------------------------------------------------------
//  DIOIP::~ DIOIP
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:20
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOIP::~DIOIP()
{
  Clean();
}





 /*-------------------------------------------------------------------
 //  DIOIP::Get
 */
 /**
 //
 //
 // @author       Abraham J. Velez
 // @version      26/03/2013 18:13:01
 //
 // @return       XBYTE* :
 // */
 /*-----------------------------------------------------------------*/
XBYTE* DIOIP::Get()
{
  return IP;
}



/*-------------------------------------------------------------------
//  DIOIP::GetXString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/05/2013 18:27:39
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
bool DIOIP::GetXString(XSTRING& IPstring)
{
  IPstring.Empty();

  if(IsEmpty()) return false;

  IPstring.Format(__L("%d.%d.%d.%d") , IP[0], IP[1], IP[2], IP[3]);

  return IPstring.IsEmpty()?false:true;
}



/*-------------------------------------------------------------------
//  DIOIP::GetDIOURL
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2014 10:51:10
//
//  @return       bool :
//
//  @param        url :
*/
/*-----------------------------------------------------------------*/
bool DIOIP::GetDIOURL(DIOURL* url)
{
  if(!url) return false;

  url->Empty();

  url->Format(__L("%d.%d.%d.%d") , IP[0], IP[1], IP[2], IP[3]);

  return url->IsEmpty()?false:true;
}





/*-------------------------------------------------------------------
//  DIOIP::IsEmpty
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/05/2013 13:46:22
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOIP::IsEmpty()
{
  if((!IP[0]) && (!IP[1]) && (!IP[2]) && (!IP[3])) return true;

  return false;
}




/*-------------------------------------------------------------------
//  DIOIP::IsLocal
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/03/2013 18:12:51
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOIP::IsLocal()
{
  if(IP[0]==10)  return true;
  if(IP[0]==172)
    {
      if((IP[1]>=16) && (IP[1]<=31)) return true;
    }

  if((IP[0]==192) && (IP[1]==168)) return true;
  if((IP[0]==169) && (IP[1]==254)) return true;

  if((IP[0]==127) && (IP[1]==0) && (IP[2]==0) && (IP[3]==1)) return true;

  return false;
}





/*-------------------------------------------------------------------
//  DIOIP::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/03/2013 18:15:04
//
//  @return       bool :
//  @param        address :
*/
/*-----------------------------------------------------------------*/
bool DIOIP::Set(XBYTE* IP)
{
  for(XDWORD c=0;c< DIOIP_MAXIP;c++)
    {
      this->IP[c] = IP[c];
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOIP::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2014 13:59:24
//
//  @return       bool :
//  @param        byte1 :
//  @param        byte2 :
//  @param        byte3 :
//  @param        byte4 :
*/
/*-----------------------------------------------------------------*/
bool DIOIP::Set(XBYTE byte1, XBYTE byte2, XBYTE byte3, XBYTE byte4)
{
  this->IP[0] = byte1;
  this->IP[1] = byte2;
  this->IP[2] = byte3;
  this->IP[3] = byte4;

  return true;
}





/*-------------------------------------------------------------------
//  DIOIP::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/03/2013 18:17:40
//
//  @return       bool :
//  @param        IPchar :
*/
/*-----------------------------------------------------------------*/
bool DIOIP::Set(XCHAR* IPchar)
{
  XSTRING _IPchar;

  _IPchar = IPchar;

  return Set(_IPchar);
}



/*-------------------------------------------------------------------
//  DIOIP::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/03/2013 18:17:15
//
//  @return       bool :
//  @param        IPstring :
*/
/*-----------------------------------------------------------------*/
bool DIOIP::Set(XSTRING& IPstring)
{
  if(IPstring.IsEmpty()) return false;

  int   iip[DIOIP_MAXIP] = { 0, 0, 0, 0 };
  XBYTE bip[DIOIP_MAXIP] = { 0, 0, 0, 0 };

  if(IPstring.GetNCharacters(__C('.')) != 3) return false;

  for(XDWORD c=0; c<IPstring.GetSize(); c++)
    {
      if((IPstring.Get()[c]<__C('0')) && (IPstring.Get()[c]>__C('9')) && (IPstring.Get()[c]!= __C('.'))) return false;
    }

  IPstring.UnFormat(__L("%d.%d.%d.%d"), &iip[0], &iip[1], &iip[2], &iip[3]);

  for(int c=0; c<DIOIP_MAXIP; c++)
    {
      bip[c] = (XBYTE)iip[c];
    }

  return Set(bip);
}




/*-------------------------------------------------------------------
//  DIOIP::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 10:03:36
//
//  @return       bool :
//  @param        IPchar :
*/
/*-----------------------------------------------------------------*/
bool DIOIP::Set(char* IPchar)
{
  XSTRING _IPchar;

  _IPchar = IPchar;

  return Set(_IPchar);
}



/*-------------------------------------------------------------------
//  DIOIP::Compare
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/05/2014 9:34:38
//
//  @return       bool :
//
//  @param        otherIP :
*/
/*-----------------------------------------------------------------*/
bool DIOIP::Compare(DIOIP& otherIP)
{
  for(int c=0; c<DIOIP_MAXIP; c++)
    {
      if(IP[c] != otherIP.Get()[c]) return false;
    }

  return true;
}



//-------------------------------------------------------------------
//  DIOIP::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:12
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOIP::Clean()
{
  memset(IP,  0, DIOIP_MAXIP);
}

