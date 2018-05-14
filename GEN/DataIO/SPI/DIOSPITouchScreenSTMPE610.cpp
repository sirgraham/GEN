
/*------------------------------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610.CPP
//
//  Data IO SPI STMPE610  (Touch Card) class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/05/2014 16:21:57
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

#include "DIOSPITouchScreenSTMPE610.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::DIOSPITOUCHSCREENSTMPE610
*/
/**
//
//  Class Constructor DIOSPITOUCHSCREENSTMPE610
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:37:54
//


//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOSPITOUCHSCREENSTMPE610::DIOSPITOUCHSCREENSTMPE610() : DIODEVICE()
{
  Clean();


  ;

  xtimer = xfactory->CreateTimer();
  databuffer = new XBUFFER();
}




/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::~DIOSPITOUCHSCREENSTMPE610
*/
/**
//
//   Class Destructor DIOSPITOUCHSCREENSTMPE610
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:25:15
//
*/
/*-----------------------------------------------------------------*/
DIOSPITOUCHSCREENSTMPE610::~DIOSPITOUCHSCREENSTMPE610()
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
//  DIOSPITOUCHSCREENSTMPE610::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2014 17:04:57
//
//  @return       bool :
//
//  @param        chipselect :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::Ini(int chipselect, int timeout)
{
  if(!isdiostreamSPIexternal)
    {
      diostreamSPIcfg = new DIOSTREAMSPICONFIG();
      if(!diostreamSPIcfg)  return false;

      diostreamSPIcfg->SetSPIMode(DIOSTREAMSPI_MODE_0);
      diostreamSPIcfg->SetNBitsWord(8);
      diostreamSPIcfg->SetSpeed(1*1000*1000);
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

  if(!diostreamSPI->Open())  return false;

  if(!diostreamSPI->WaitToConnected(timeout)) return false;

  if(!TouchScreen_Ini()) return false;

  return DIODEVICE::Ini();
}




/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::GetVersion
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2014 13:48:21
//
//  @return       XWORD :
//
*/
/*-----------------------------------------------------------------*/
XWORD DIOSPITOUCHSCREENSTMPE610::GetVersion()
{
  if(!IsInitialized()) return false;

  XWORD version = 0;
  XBYTE data[2] = { 0, 0 };

  if(!ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_CHIP_ID1, data[0])) return 0;
  if(!ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_CHIP_ID2, data[1])) return 0;

  version = data[0];
  version <<= 8;
  version |= data[1];

  return version;
}




/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::IsTouched
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2014 13:41:43
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::IsTouched()
{
  if(!IsInitialized()) return false;

  XBYTE data = 0;

  bool  status = ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_CTRL, data);
  if(!status) return false;

  if(data & 0x80) return true;

  return false;
}



/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::IsBufferEmpty
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2014 13:44:17
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::IsBufferEmpty()
{
  if(!IsInitialized()) return false;

  XBYTE data = 0;

  bool  status = ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_STA, data);
  if(!status) return false;

  if(data & DIOSPITOUCHSCREENSTMPE610_FIFO_STA_EMPTY) return true;

  return false;
}




/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::GetBufferSize
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2014 13:45:39
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOSPITOUCHSCREENSTMPE610::GetBufferSize()
{
  if(!IsInitialized()) return false;

  XBYTE data;
  bool  status = ReadRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_SIZE, data);
  if(!status) return 0;

  return data;
}





/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::ReadPosition
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2014 14:02:35
//
//  @return       bool :
//
//  @param        x :
//  @param        y :
//  @param        z :
*/
/*-----------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::ReadPosition(XWORD& x, XWORD& y, XBYTE& z)
{
  if(!IsInitialized()) return false;

  /*
  XBYTE data[4];

  for(XBYTE c=0; c<4; c++)
    {
      if(!ReadRegister(0xD7, data[c]))  return false;
    }

  XDWORD xyz = (XDWORD) ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3] << 0));

  x = (XWORD)((xyz >> 20) & 0x00000FFF);
  y = (XWORD)((xyz >> 8)  & 0x00000FFF);
  z = (XBYTE)((xyz >> 0)  & 0x0000000F);

  if(IsBufferEmpty()) WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_STA, 0xFF);
  */

  XBYTE data[4];

  for(XBYTE c=0; c<4; c++)
    {
      if(!ReadRegister(0xD7, data[c]))  return false;
    }

  x   = data[0];
  x <<= 4;
  x  |= (data[1] >> 4);

  y   = data[1] & 0x0F;
  y <<= 8;
  y  |= data[2];

  z   = data[3];

  if(IsBufferEmpty()) WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_STA, 0xFF);


  return true;
}





/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::End
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
bool DIOSPITOUCHSCREENSTMPE610::End()
{
  if(!DIODEVICE::End()) return false;

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
//  DIOSPITOUCHSCREENSTMPE610::TouchScreen_Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2014 13:05:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::TouchScreen_Ini()
{
  if(!WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_SYS_CTRL1, DIOSPITOUCHSCREENSTMPE610_SYS_CTRL1_RESET)) return false;

  xsleep->MilliSeconds(250);

  for(XBYTE c=0; c<65; c++)
    {
      XBYTE data;
      ReadRegister(c, data);
    }

  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_SYS_CTRL2       , 0x0);                                                                             // turn on clocks!
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_CTRL        , DIOSPITOUCHSCREENSTMPE610_TSC_CTRL_XYZ | DIOSPITOUCHSCREENSTMPE610_TSC_CTRL_EN);  // XYZ and enable!
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_EN          , DIOSPITOUCHSCREENSTMPE610_INT_EN_TOUCHDET);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_ADC_CTRL1       , DIOSPITOUCHSCREENSTMPE610_ADC_CTRL1_10BIT | (0x6 << 4));                          // 96 clocks per conversion
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_ADC_CTRL2       , DIOSPITOUCHSCREENSTMPE610_ADC_CTRL2_6_5MHZ);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_CFG         , DIOSPITOUCHSCREENSTMPE610_TSC_CFG_4SAMPLE | DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_1MS | DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_5MS);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_FRACT_XYZ   , 0x6);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_TH         , 1);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_STA        , DIOSPITOUCHSCREENSTMPE610_FIFO_STA_RESET);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_FIFO_STA        , 0);                                                                               // unreset
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_TSC_I_DRIVE     , DIOSPITOUCHSCREENSTMPE610_TSC_I_DRIVE_50MA);
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_STA         , 0xFF);                                                                            // reset all ints
  WriteRegister(DIOSPITOUCHSCREENSTMPE610_REG_INT_CTRL        , DIOSPITOUCHSCREENSTMPE610_INT_CTRL_POL_HIGH | DIOSPITOUCHSCREENSTMPE610_INT_CTRL_ENABLE);

  return true;
}




/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610::ReadRegister
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2014 11:27:10
//
//  @return       XBYTE :
//
//  @param        reg :
*/
/*-----------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::ReadRegister(XBYTE reg, XBYTE& data)
{
  //diostreamSPI->GetInXBuffer()->Delete();

  XBYTE regdata[2] = { (XBYTE)(reg | 0x80), 0 };
  XBYTE _data[2]   = { 0, 0 };

  bool status = diostreamSPI->Write(regdata, 2)?true:false;
  //if(status) status = diostreamSPI->WaitToFlushOutXBuffer(timeout);

  if(status) status = diostreamSPI->WaitToFilledReadingBuffer(2, timeout);
  if(status) status = diostreamSPI->Read(_data, 2)?true:false;

  data = _data[1];

  return status;
}




/*-------------------------------------------------------------------
//  DIOSPITOUCHSCREENSTMPE610:WriteRegister
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2014 11:30:49
//
//  @return       bool :
//
//  @param        reg :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOSPITOUCHSCREENSTMPE610::WriteRegister(XBYTE reg, XBYTE data)
{
  if(!databuffer) return false;

  bool status = diostreamSPI->Write(&reg, 1)?true:false;
  if(status) status = diostreamSPI->Write(&data, 1)?true:false;
  if(status) status = diostreamSPI->WaitToFlushOutXBuffer(timeout);
  if(status) status = diostreamSPI->WaitToFilledReadingBuffer(2, timeout);

  diostreamSPI->GetInXBuffer()->Delete();

  return status;
}