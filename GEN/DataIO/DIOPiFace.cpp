
/*------------------------------------------------------------------------------------------
//  DIOPIFACE.CPP
//
//  Data IO PiFace Shield (Raspberry Pi)
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

#include "DIOFactory.h"
#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"

#include "DIOPiFace.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOPIFACE::DIOPIFACE
*/
/**
//
//  Class Constructor DIOPIFACE
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:37:54
//


//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOPIFACE::DIOPIFACE() : DIOSPIGPIOMCP23S17()
{

}



/*-------------------------------------------------------------------
//  DIOPIFACE::~DIOPIFACE
*/
/**
//
//   Class Destructor DIOPIFACE
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:25:15
//
*/
/*-----------------------------------------------------------------*/
DIOPIFACE::~DIOPIFACE()
{

}




/*-------------------------------------------------------------------
//  DIOPIFACE::Configure
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 21:41:50
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOPIFACE::Configure()
{
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_IOCON   , 8);     // Enable hardware addressing
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_GPIOA   , 0x00);  // Turn on port A
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_IODIRA  , 0);     // Set port A as an output
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_IODIRB  , 0xFF);  // Set port B as an input
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_GPPUB   , 0xFF);  // Turn on port B pullups

  return true;
}



/*-------------------------------------------------------------------
//  DIOPIFACE::ReadInputs
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 22:50:09
//
//  @return       bool :
//
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOPIFACE::ReadInputs(XBYTE& value)
{
  XBYTE _value[3] = { 0xFF, 0xFF, 0xFF };

  diostreamSPI->GetInXBuffer()->Delete();

  if(!SendCommand(DIOSPIGPIOMCP23S17_READCMD, DIOPIFACE_INPUTPORT, 0xFF)) return false;

  if(!diostreamSPI->WaitToFilledReadingBuffer(3, timeout)) return false;

  bool status = diostreamSPI->Read(_value, 3)?true:false;

  value = _value[2] ^ 0xFF;

  return status;
}



/*-------------------------------------------------------------------
//  DIOPIFACE::WriteOutputs
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 22:49:35
//
//  @return       bool :
//
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOPIFACE::WriteOutputs(XBYTE value)
{
  return SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOPIFACE_OUTPUTPORT, value);
}



/*-------------------------------------------------------------------
//  DIOPIFACE::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 23:00:16
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOPIFACE::End()
{
  WriteOutputs(0x00);

  return DIOSPIGPIOMCP23S17::End();
}