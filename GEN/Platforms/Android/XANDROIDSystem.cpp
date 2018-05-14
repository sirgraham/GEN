//------------------------------------------------------------------------------------------
//  XANDROIDSYSTEM.CPP
//
//  System resources Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/03/2004 12:28:40
//  Lsdddast Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"
#include "XFileTXT.h"

#include "XANDROIDSystem.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XANDROIDSYSTEM::XANDROIDSYSTEM
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:28:59
//
//  @return
//  */
//-------------------------------------------------------------------
XANDROIDSYSTEM::XANDROIDSYSTEM() : XSYSTEM()
{

}



//-------------------------------------------------------------------
//  XANDROIDSYSTEM::~XANDROIDSYSTEM
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:20
//
//  @return
//  */
//-------------------------------------------------------------------
XANDROIDSYSTEM::~XANDROIDSYSTEM()
{

}



/*-------------------------------------------------------------------
//  XANDROIDSYSTEM::GetTypeHardware
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      21/04/2014 20:36:43
//
//  @return       XSYSTEM_HARDWARE :
//
//  @param        revision :
*/
/*-----------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XANDROIDSYSTEM::GetTypeHardware(int* revision)
{
  return XSYSTEM_HARDWARETYPE_ARMGENERIC;
}



//-------------------------------------------------------------------
//  XANDROIDSYSTEM::GetSO
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:40
//
//  @return       XANDROIDSYSTEM_SO :
//  */
//-------------------------------------------------------------------
XSYSTEM_SO XANDROIDSYSTEM::GetSO()
{
  return XSYSTEM_SO_ANDROID;
}


//-------------------------------------------------------------------
//  XANDROIDSYSTEM::GetLanguage
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:55
//
//  @return       XLANGUAGE_CODE :
//  */
//-------------------------------------------------------------------
XLANGUAGE_CODE XANDROIDSYSTEM::GetLanguage()
{
  return XLANGUAGE_UNKNOWN;
}


//-------------------------------------------------------------------
//  XANDROIDSYSTEM::GetTotalMemory
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:30:12
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XANDROIDSYSTEM::GetTotalMemory()
{

  return 0;
}


//-------------------------------------------------------------------
//  XANDROIDSYSTEM::GetFreeMemory
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:30:41
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XANDROIDSYSTEM::GetFreeMemory()
{

  return 0;
}




