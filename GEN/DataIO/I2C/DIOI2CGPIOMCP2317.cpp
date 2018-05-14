/*------------------------------------------------------------------------------------------
//  DIOI2CGPIOMCP2317.CPP
//
//  Data IO I2C MCP23S17 (Microchip) (8+8 Digital I/O)
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

#include "DIOI2CGPIOMCP2317.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOI2CGPIOMCP2317::DIOI2CGPIOMCP2317
*/
/**
//
//  Class Constructor DIOI2CGPIOMCP2317
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:37:54
//

//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOI2CGPIOMCP2317::DIOI2CGPIOMCP2317() :  DIODEVICEI2C()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOI2CGPIOMCP2317::~DIOI2CGPIOMCP2317
*/
/**
//
//   Class Destructor DIOI2CGPIOMCP2317
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:25:15
//
*/
/*-----------------------------------------------------------------*/
DIOI2CGPIOMCP2317::~DIOI2CGPIOMCP2317()
{
  End();

  Clean();
}





/*-------------------------------------------------------------------
//  DIOI2CGPIOMCP2317::Configure
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2014 17:19:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOMCP2317::Configure()
{
  SendCommand(DIOI2CGPIOMCP2317_IODIR + DIOI2CGPIOMCP2317PORT_A, 0xFF);
  SendCommand(DIOI2CGPIOMCP2317_IODIR + DIOI2CGPIOMCP2317PORT_B, 0x00);

  return true;
}




/*-------------------------------------------------------------------
//  DIOI2CGPIOMCP2317::ReadInputs
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2014 23:56:38
//
//  @return       bool :
//
//  @param        port :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOMCP2317::ReadInputs(DIOI2CGPIOMCP2317PORT port, XBYTE& value)
{
  if(!diostream)        return false;
  if(!IsInitialized())  return false;

  XBYTE cmd  = DIOI2CGPIOMCP2317_GPIO + port;

  bool status = diostream->Write(&cmd, 1)?true:false;
  if(!diostream->WaitToFlushOutXBuffer(timeout)) return false;

  status = diostream->WaitToFilledReadingBuffer(1, timeout);
  if(status) status = diostream->Read(&value, 1)?true:false;
  //if(status) value ^= value;

  return status;
}



/*-------------------------------------------------------------------
//  DIOI2CGPIOMCP2317::WriteOutputs
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/06/2014 8:53:00
//
//  @return       bool :
//
//  @param        port :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOMCP2317::WriteOutputs(DIOI2CGPIOMCP2317PORT port, XBYTE value)
{
  XBYTE cmd = (DIOI2CGPIOMCP2317_OLAT + port);

  return SendCommand(cmd, value);
}



/*-------------------------------------------------------------------
//  DIOI2CGPIOMCP2317::SendCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/06/2014 8:46:29
//
//  @return       bool :
//
//  @param        cmd :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOMCP2317::SendCommand(XBYTE cmd, XBYTE value)
{
  if(!diostream)        return false;
  if(!IsInitialized())  return false;

  XBUFFER xbuffer;

  xbuffer.Add((XBYTE)cmd);
  xbuffer.Add((XBYTE)value);

  bool status = diostream->Write(xbuffer)?true:false;

  if(!diostream->WaitToFlushOutXBuffer(timeout)) return false;

  return status;
}



/*-------------------------------------------------------------------
//  DIOI2CGPIOMCP2317::End
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
bool DIOI2CGPIOMCP2317::End()
{
  if(!DIODEVICE::End()) return false;

  if(diostream)
    {
      diostream->Close();
      diofactory->DeleteStreamIO(diostream);
      diostream = NULL;
    }

  if(diostreamcfg)
    {
      delete diostreamcfg;
      diostreamcfg = NULL;
    }

  if(xtimerout)
    {
      xfactory->DeleteTimer(xtimerout);
      xtimerout = NULL;
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOI2CGPIOMCP2317::IniDevice
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 14:05:09
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CGPIOMCP2317::IniDevice()
{
  diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO( diostreamcfg);
  if(!diostream)  return false;

  if(!diostream->Open())  return false;

  if(!diostream->WaitToConnected(timeout)) return false;

  return DIODEVICE::Ini();
}
