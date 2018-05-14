//------------------------------------------------------------------------------------------
//  XSYSTEM.CPP
//
//  System resources Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/03/2004 12:28:40
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "XSystem.h"

#include "DIOFactory.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XSYSTEM::XSYSTEM
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/09/2012 9:17:20
//
//  @return
//  @param        xfactory :
*/
/*-----------------------------------------------------------------*/
XSYSTEM::XSYSTEM()
{

}



//-------------------------------------------------------------------
//  XSYSTEM::~XSYSTEM
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:20
//
//  @return
//  */
//-------------------------------------------------------------------
XSYSTEM::~XSYSTEM()
{

}



/*-------------------------------------------------------------------
//  XSYSTEM::HardwareUseLittleEndian
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/10/2012 13:02:42
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSYSTEM::HardwareUseLittleEndian()
{
  // In ARM can be changed, can be used BIG ENDIAN or LITTLE ENDIAN

  switch(GetTypeHardware())
    {
      case XSYSTEM_HARDWARETYPE_UNKNOWN       : return false;

      case XSYSTEM_HARDWARETYPE_PC            : return true;
      case XSYSTEM_HARDWARETYPE_LEX3V700A     : return true;

      case XSYSTEM_HARDWARETYPE_ARTILA500     : return false;
      case XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX : return false;

      case XSYSTEM_HARDWARETYPE_RASPBERRYPI   : return true;
      case XSYSTEM_HARDWARETYPE_ODROIDC1      : return true;
      case XSYSTEM_HARDWARETYPE_ARMGENERIC    : return true;
    }

  return false;
}




/*-------------------------------------------------------------------
//  XSYSTEM::CheckGetSO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/08/2009 03:44:53 p.m.
//
//  @return       bool :
//  @param        SOtoCheck :
*/
/*-----------------------------------------------------------------*/
bool XSYSTEM::CheckGetSO(XSYSTEM_SO SOtoCheck)
{
  XSYSTEM_SO SO = GetSO();

  if((XWORD)SO&(XWORD)SOtoCheck) return true;

  return false;
}





/*-------------------------------------------------------------------
//  XSYSTEM::GetMemoryFreePercentAvailable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2013 14:18:10
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XSYSTEM::GetMemoryFreePercentAvailable()
{
  XDWORD total;
  XDWORD free;
  int    percent;

  if(!GetMemoryInfo(total,free)) return false;

  percent = ((100*free)/total);

  return percent;
}



