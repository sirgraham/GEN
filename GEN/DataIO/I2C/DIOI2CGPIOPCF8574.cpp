/*------------------------------------------------------------------------------------------
//  DIOI2CGPIOPCF8574.CPP
//
//  Data IO I2C PCF8574 (GPIO 8 Pins) class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/05/2014 16:21:57
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"

#include "DIOI2CGPIOPCF8574.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOI2CGPIOPCF8574::DIOI2CGPIOPCF8574
*/
/**
//
//  Class Constructor DIOI2CGPIOPCF8574
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:37:54
//
*/
/*-----------------------------------------------------------------*/
DIOI2CGPIOPCF8574::DIOI2CGPIOPCF8574() : DIODEVICEI2C()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOI2CGPIOPCF8574::~DIOI2CGPIOPCF8574
*/
/**
//
//   Class Destructor DIOI2CGPIOPCF8574
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:25:15
//
*/
/*-----------------------------------------------------------------*/
DIOI2CGPIOPCF8574::~DIOI2CGPIOPCF8574()
{
  End();

  Clean();
}




/*-------------------------------------------------------------------
//  DIOI2CGPIOPCF8574::ReadPin
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/06/2014 10:07:13
//
//  @return       bool :
//
//  @param        pin :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOPCF8574::Read(XBYTE& value)
{
  if(!diostream)        return false;
  if(!IsInitialized())  return false;

  bool  status;

  status = diostream->WaitToFilledReadingBuffer(1, timeout);
  if(status) status = diostream->Read(&value, 1)?true:false;

  return status;
}




/*-------------------------------------------------------------------
//  DIOI2CGPIOPCF8574::WritePin
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/06/2014 10:12:23
//
//  @return       bool :
//
//  @param        pin :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOPCF8574::Write(XBYTE value)
{
  bool  status = false;

  if(!diostream)        return false;
  if(!IsInitialized())  return false;

  if(diostream->Write(&value, 1)) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}





/*-------------------------------------------------------------------
//  DIOI2CGPIOPCF8574::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:29:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOPCF8574::End()
{
  if(!DIODEVICE::End()) return false;

  if(diostream)
    {
      diostream->Close();
      diofactory->DeleteStreamIO(diostream);
      diostream = NULL;
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOI2CGPIOPCF8574::IniDevice
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 13:50:14
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOPCF8574::IniDevice()
{
  diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO(diostreamcfg);
  if(!diostream)  return false;

  if(!diostream->Open())  return false;

  if(!diostream->WaitToConnected(timeout)) return false;

  return DIODEVICE::Ini();
}

