/*------------------------------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341.CPP
//
//  Data IO SPI Screen LCD Ilitek ILI9341
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 01/06/2014 18:00:35
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"

#include "DIOSPILCDScreenILI9341.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::DIOSPILCDSCREENILI9341
*/
/**
//
//  Class Constructor DIOSPILCDSCREENILI9341
//
//  @author       Abraham J. Velez
//  @version      09/06/2014 10:59:23
//


//  @param        xpublisher :
//  @param        xsystem :
*/
/*-----------------------------------------------------------------*/
DIOSPILCDSCREENILI9341::DIOSPILCDSCREENILI9341(XSYSTEM* xsystem) : DIODEVICE()
{
  Clean();


  ;
  this->xsystem       = xsystem;

  xtimer = xfactory->CreateTimer();
  databuffer = new XBUFFER();
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::~DIOSPILCDSCREENILI9341
*/
/**
//
//   Class Destructor DIOSPILCDSCREENILI9341
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:17:22
//
*/
/*-----------------------------------------------------------------*/
DIOSPILCDSCREENILI9341::~DIOSPILCDSCREENILI9341()
{
  End();

  if(xtimer)
    {
      xfactory->DeleteTimer(xtimer);
      xtimer = NULL;
    }

  if(databuffer)
    {
      delete databuffer;
      databuffer = NULL;
    }

  Clean();
}


/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/06/2014 11:01:27
//
//  @return       bool :
//
//  @param        chipselect :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::Ini(int chipselect, int timeout)
{
  if(!diogpio) diogpio = diostreamSPI->GetDIOGPIO();
  if(!diogpio) return false;

  if(!isdiostreamSPIexternal)
    {
      diostreamSPIcfg = new DIOSTREAMSPICONFIG();
      if(!diostreamSPIcfg)  return false;

      diostreamSPIcfg->SetSPIMode(DIOSTREAMSPI_MODE_0);
      diostreamSPIcfg->SetNBitsWord(8);
      diostreamSPIcfg->SetSpeed(160*1000*1000);
      diostreamSPIcfg->SetDelay(0);
      diostreamSPIcfg->SetIsOnlyWrite(false);
      diostreamSPIcfg->GetLocalDeviceName()->Format(__L("/dev/spidev0.%d"), chipselect);

      diostreamSPI = (DIOSTREAMSPI*)diofactory->CreateStreamIO(diostreamSPIcfg);
      if(!diostreamSPI) return false;
    }
   else
    {
      if(!diostreamSPI) return false;

      diostreamSPIcfg = (DIOSTREAMSPICONFIG *)diostreamSPI->GetConfig();
      if(!diostreamSPIcfg) return false;
    }

  this->timeout = timeout;

  diogpio->SetMode(pinDC, false);

  if(!diostreamSPI->Open())  return false;

  width      = 320;
  height     = 240;

  if(!LCD_Init()) return false;

  return DIODEVICE::Ini();
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::Clear
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 19:09:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::Clear(XWORD color)
{
  /*
  XWORD* buffer = new XWORD[GetSizeBuffer()/sizeof(XWORD)];
  if(!buffer) return false;

  for(int c=0; c<(int)(GetSizeBuffer()/sizeof(XWORD)); c++)
    {
      buffer[c] = color;
    }

  bool status = Update((XBYTE*)buffer);

  delete [] buffer;

  return status;
  */

  return true;
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::Update
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:20:09
//
//  @return       bool :
//
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::Update(XBYTE* buffer)
{
  if(!IsInitialized()) return false;

  if(!LCD_SetWindow(0, 0, 319, 239)) return false;
  if(!SendCommand(0x2c)) return false;

  databuffer->Delete();
  databuffer->Add(buffer, GetSizeBuffer());

  return SendData();
}





/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 20:52:53
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::End()
{
  if(!DIODEVICE::End()) return false;

  Clear();

  if(diostreamSPI) diostreamSPI->Close();

  if(!isdiostreamSPIexternal)
    {
      if(diostreamSPI)
        {
          diofactory->DeleteStreamIO(diostreamSPI);
          diostreamSPI = NULL;
        }

      if(diostreamSPIcfg)
        {
          delete diostreamSPIcfg;
          diostreamSPIcfg = NULL;
        }
    }

  return true;
}





/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::LCD_Init
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:26:17
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::LCD_Init()
{
  if(!xtimer) return false;

  DIOSPILCDSCREENILI9341_DCHIGH;

  LCD_Reset();


  if(!SendCommand(0x01)) return false;
  xsleep->MilliSeconds(5);
  SendCommand(0x28);

  //-----------------------------------------------------------------------------------------------
  // 1

  WriteRegister(0xCF,  3, 0x00, 0x83, 0x30);
  WriteRegister(0xED,  4, 0x64, 0x03, 0x12, 0x81);
  WriteRegister(0xE8,  3, 0x85, 0x01, 0x79);
  WriteRegister(0xCB,  5, 0x39, 0x2C, 0x00, 0x34, 0x02);
  WriteRegister(0xF7,  1, 0x20);
  WriteRegister(0xEA,  2, 0x00, 0x00);
  WriteRegister(0xC0,  1, 0x26);
  WriteRegister(0xC1,  1, 0x11);
  WriteRegister(0xC5,  2, 0x35, 0x3E);
  WriteRegister(0xC7,  1, 0xBE);
  WriteRegister(0x3A,  1, 0x55);
  WriteRegister(0xB1,  2, 0x00, 0x1B);
  WriteRegister(0x26,  1, 0x01);
  WriteRegister(0xB7,  1, 0x07);
  WriteRegister(0xB6,  4, 0x0A, 0x82, 0x27, 0x00);

  //-----------------------------------------------------------------------------------------------



  //-----------------------------------------------------------------------------------------------
  // 2
  /*
  WriteRegister(0xCB,  5, 0x39, 0x2C, 0x00, 0x34, 0x02);
  WriteRegister(0xCF,  3, 0x00, 0xC1, 0x30);
  WriteRegister(0xE8,  3, 0x85, 0x00, 0x78);
  WriteRegister(0xEA,  2, 0x00, 0x00);
  WriteRegister(0xED,  4, 0x64, 0x03, 0x12, 0x81);
  WriteRegister(0xF7,  1, 0x20);
  WriteRegister(0xC0,  1, 0x23);
  WriteRegister(0xC1,  1, 0x10);
  WriteRegister(0xC5,  2, 0x3e, 0x28);
  WriteRegister(0xC7,  1, 0x86);
  WriteRegister(0x36,  1, 0x48);
  WriteRegister(0x3A,  1, 0x55);
  WriteRegister(0xB1,  2, 0x00, 0x18);
  WriteRegister(0xB6,  3, 0x08, 0x82, 0x27);
  WriteRegister(0xF2,  1, 0x00);
  WriteRegister(0x26,  1, 0x01);
  WriteRegister(0xE0, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00);
  WriteRegister(0XE1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F);
  */
  //-----------------------------------------------------------------------------------------------




  //-----------------------------------------------------------------------------------------------
  // 3
  /*
  WriteRegister(0xCD,  5, 0x39, 0x2C, 0x00, 0x34, 0x02);        // Power Control A
  WriteRegister(0xCF,  3, 0x00, 0xAA, 0xB0);                    // Power Control B
  WriteRegister(0xF7,  1, 0x30);                                // CMD_PUMP_RATIO_CONTROL
  WriteRegister(0xC0,  1, 0x25);                                // POWER_CONTROL_1
  WriteRegister(0xC1,  1, 0x11);                                // POWER_CONTROL_2
  WriteRegister(0xC5,  2, 0x5C, 0x4C);                          // VCOM_CONTROL_1
  WriteRegister(0xC7,  1, 0x94);                                // VCOM_CONTROL_2
  WriteRegister(0xE8,  3, 0x85, 0x01, 0x78);                    // DRIVER_TIMING_CONTROL_A
  WriteRegister(0xEA,  2, 0x00, 0x00);                          // DRIVER_TIMING_CONTROL_B
  WriteRegister(0x3A,  1, 0x05);                                // COLMOD_PIXEL_FORMAT_SET
  WriteRegister(0x36,  1, 0x48);                                // MEMORY_ACCESS_CONTROL
  */
  //-----------------------------------------------------------------------------------------------


  if(!LCD_RotateRGB(90, 2))          return false;

  SendCommand(0x11);                // Sleep out
  xsleep->MilliSeconds(100);

  SendCommand(0x29);                // display on
  xsleep->MilliSeconds(20);

  return true;
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::LCD_Reset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:28:16
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::LCD_Reset()
{
  if(!xtimer)                            return false;
  if(!diostreamSPI)                      return false;
  if(pinreset == DIOGPIO_PINNOTUSE)  return false;
  if(!diogpio)                       return false;

  diogpio->Set(pinreset,false);
  xsleep->MilliSeconds(100);

  diogpio->Set(pinreset,true);
  xsleep->MilliSeconds(100);

  return true;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::LCD_RotateRGB
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/07/2014 14:09:30
//
//  @return       bool :
//
//  @param        rotate :
//  @param        bgr :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::LCD_RotateRGB(int rotate, XBYTE bgr)
{
  #define MEM_Y   (7) /* MY row address order */
  #define MEM_X   (6) /* MX column address order */
  #define MEM_V   (5) /* MV row / column exchange */
  #define MEM_L   (4) /* ML vertical refresh order */
  #define MEM_H   (2) /* MH horizontal refresh order */
  #define MEM_BGR (3) /* RGB-BGR Order */

  bool status = false;

  switch(rotate)
    {
      case   0: status = WriteRegister(0x36, 1, (1 << MEM_X) | (bgr << MEM_BGR));                               break;
      case  90: status = WriteRegister(0x36, 1, (1 << MEM_Y) | (1 << MEM_X) |(1 << MEM_V) | (bgr << MEM_BGR));  break;
      case 180: status = WriteRegister(0x36, 1, (1 << MEM_Y) | (bgr << MEM_BGR));                               break;
      case 270: status = WriteRegister(0x36, 1, (1<<MEM_V) | (1 << MEM_L) | (bgr << MEM_BGR));                  break;
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::LCD_SetWindow
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/07/2014 18:28:36
//
//  @return       bool :
//
//  @param        xs :
//  @param        ys :
//  @param        xe :
//  @param        ye :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::LCD_SetWindow(int xs, int ys, int xe, int ye)
{
  bool status;

  /* Column address set */
  status = WriteRegister(0x2A, 4,  (xs >> 8) & 0xFF, xs & 0xFF, (xe >> 8) & 0xFF, xe & 0xFF);

  /* Row adress set */
  if(status) status = WriteRegister(0x2B, 4,  (ys >> 8) & 0xFF, ys & 0xFF, (ye >> 8) & 0xFF, ye & 0xFF);

  return status;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::SendCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/06/2014 17:03:12
//
//  @return       bool :
//
//  @param        command :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::SendCommand(XBYTE command)
{
  bool status;

  DIOSPILCDSCREENILI9341_DCLOW;
  status = diostreamSPI->Write(&command, 1)?true:false;
  if(status) status = diostreamSPI->WaitToFlushOutXBuffer(timeout);

  return status;
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::SendData
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/06/2014 16:35:55
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::SendData()
{
  bool status;

  DIOSPILCDSCREENILI9341_DCHIGH;
  status = diostreamSPI->Write((*databuffer))?true:false;
  if(status) status = diostreamSPI->WaitToFlushOutXBuffer(timeout);
  DIOSPILCDSCREENILI9341_DCLOW;

  return status;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::WriteRegister
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/07/2014 10:45:40
//
//  @return       bool :
//
//  @param        reg :
//  @param        ndata :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::WriteRegister(XBYTE reg, int ndata, ...)
{
  va_list arg;

  va_start(arg, ndata);

  SendCommand(reg);

  DeleteBuffer();

  for(int c=0; c<ndata; c++)
    {
      int data = (int)va_arg(arg, int);
      databuffer->Add((XBYTE)data);
    }

  SendData();

  va_end(arg);

  return true;
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENILI9341::DeleteBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/06/2014 16:33:21
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENILI9341::DeleteBuffer()
{
  if(!databuffer) return false;

  databuffer->Delete();

  return true;
}

