/*------------------------------------------------------------------------------------------
//  DIOMAC.CPP
//
//  Data IO MAC class
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

#include "DIOMAC.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



//-------------------------------------------------------------------
//  DIOMAC:: DIOMAC
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:31
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOMAC:: DIOMAC()
{
  Clean();
}


//-------------------------------------------------------------------
//  DIOMAC::~ DIOMAC
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:20
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOMAC::~ DIOMAC()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOMAC::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/05/2011 12:27:09
//
//  @return       XBYTE* :
//  */
/*-----------------------------------------------------------------*/
XBYTE* DIOMAC::Get()
{
  return MAC;
}




/*-------------------------------------------------------------------
//  DIOMAC::GetManufactured
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/04/2013 22:44:11
//
//  @return       XDWORD :
//  */
/*-----------------------------------------------------------------*/
XDWORD DIOMAC::GetManufactured()
{
  XDWORD manufactured = 0;

  manufactured |= MAC[0];   manufactured <<=8;
  manufactured |= MAC[1];   manufactured <<=8;
  manufactured |= MAC[2];

  return manufactured;
}



/*-------------------------------------------------------------------
//  DIOMAC::GetXString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/01/2014 16:31:37
//
//  @return       bool :
//  @param        MACstring :
*/
/*-----------------------------------------------------------------*/
bool DIOMAC::GetXString(XSTRING& MACstring)
{
  MACstring.Empty();

  MACstring.Format(__L("%02X:%02X:%02X:%02X:%02X:%02X") , this->MAC[0]
                                                        , this->MAC[1]
                                                        , this->MAC[2]
                                                        , this->MAC[3]
                                                        , this->MAC[4]
                                                        , this->MAC[5]);
  return true;
}




/*-------------------------------------------------------------------
//  DIOMAC::GetLongNumber
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/06/2013 22:27:02
//
//  @return       XQWORD :
//  */
/*-----------------------------------------------------------------*/
XQWORD DIOMAC::GetLongNumber()
{
  XQWORD number = 0;

  for(int c=0;c<DIOMAC_MAXSIZE;c++)
    {
      number  |= MAC[c];
      number <<= 8;
    }

  number >>= 8;

  return number;
}




/*-------------------------------------------------------------------
//  DIOMAC::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/05/2011 12:43:19
//
//  @return       bool :
//  @param        MAC :
*/
/*-----------------------------------------------------------------*/
bool DIOMAC::Set(XBYTE* MAC)
{
  for(XDWORD c=0; c<DIOMAC_MAXSIZE; c++)
    {
      this->MAC[c] = MAC[c];
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOMAC::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/04/2013 18:40:14
//
//  @return       bool :
//  @param        MAC :
*/
/*-----------------------------------------------------------------*/
bool DIOMAC::Set(XSTRING& MAC)
{
  MAC.UnFormat(__L("%02X:%02X:%02X:%02X:%02X:%02X") , &this->MAC[0]
                                                    , &this->MAC[1]
                                                    , &this->MAC[2]
                                                    , &this->MAC[3]
                                                    , &this->MAC[4]
                                                    , &this->MAC[5]);
  return true;
}




/*-------------------------------------------------------------------
//  DIOMAC::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2013 15:47:43
//
//  @return       bool :
//  @param        MAC :
*/
/*-----------------------------------------------------------------*/
bool DIOMAC::Set(char* MAC)
{
  XSTRING _MAC;

  _MAC = MAC;

  return Set(_MAC);
}




//-------------------------------------------------------------------
//  DIOMAC::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:12
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOMAC::Clean()
{
  memset(MAC, 0, DIOMAC_MAXSIZE);
}


