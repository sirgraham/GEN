//------------------------------------------------------------------------------------------
//  XPHONE.CPP
//
//  Phone Function class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 25/10/2004 15:02:12
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XPhone.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XPHONE::XPHONE
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/10/2004 15:02:34
//
//  @return       void :
//  */
//-------------------------------------------------------------------
XPHONE::XPHONE()
{

}


//-------------------------------------------------------------------
//  XPHONE::~XPHONE
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/10/2004 15:02:54
//
//  @return       void :
//  */
//-------------------------------------------------------------------
XPHONE::~XPHONE()
{

}


//-------------------------------------------------------------------
//  XPHONE::Ini
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/05/2004 13:19:19
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XPHONE::Ini()
{
  return true;
}


//-------------------------------------------------------------------
//  XPHONE::ResetScreenSaver
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/10/2004 15:03:17
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XPHONE::ResetScreenSaver()
{
  return true;
}




//-------------------------------------------------------------------
//  XPHONE::StrengthSignal
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/10/2004 15:56:26
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XPHONE::StrengthSignal()
{
  return 0;
}



//-------------------------------------------------------------------
//  XPHONE::StrengthSignal
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/10/2004 15:56:26
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XPHONE::StrengthBattery()
{
  return XPHONEMODEL_NOTBATTERY;
}



//-------------------------------------------------------------------
//  XPHONE::GetModel
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/10/2004 15:56:26
//
//  @return       XPHONEMODEL :
//  */
//-------------------------------------------------------------------
XPHONEMODEL XPHONE::GetModel()
{
  return XPHONEMODEL_UNKNOWN;
}



//-------------------------------------------------------------------
//  XPHONE::GetIMEI
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/10/2004 15:56:26
//
//  @return       bool :
//  @param        XBYTE* :
//  */
//-------------------------------------------------------------------
bool XPHONE::GetIMEI(XBYTE* IMEI)
{
  if(!IMEI) return false;

  return true;
}



//-------------------------------------------------------------------
//  XPHONE::GetGSMLAC
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/10/2004 15:56:26
//
//  @return       bool :
//  @param        XPHONELAC* :
//  */
//-------------------------------------------------------------------
bool XPHONE::GetGSMLAC(XPHONELAC* lac)
{
  if(!lac) return false;


  return true;
}



//-------------------------------------------------------------------
//  XPHONE::SendSMS
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/04/2004 14:44:24
//
//  @return       bool :
//  @param        target :
//  @param        msg :
*/
//-------------------------------------------------------------------
bool XPHONE::SendSMS(char* target,char* msg)
{
  if(!target) return false;
  if(!msg)    return false;


  return true;
}


//-------------------------------------------------------------------
//  XPHONE::GetPhoneBookNContacts
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/04/2005 17:47:56
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XPHONE::GetPhoneBookNContacts()
{
  return 0;
}


//-------------------------------------------------------------------
//  XPHONE::GetPhoneBookContact
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/04/2005 17:48:09
//
//  @return       char* :
//  @param        index :
*/
//-------------------------------------------------------------------
char* XPHONE::GetPhoneBookContact(int index)
{
  if(!index) return NULL;

  return NULL;
}




//-------------------------------------------------------------------
//  XPHONE::End
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/05/2004 13:19:19
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XPHONE::End()
{
  return true;
}
