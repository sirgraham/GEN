/*------------------------------------------------------------------------------------------
//  DIOI2C6AXISTRACKINGLSM303DLHC.CPP
//
//  Data IO I2C PCA9685 (16-channel, 12-bit PWM controller) class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/05/2014 16:21:57
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebugTrace.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"

#include "DIOI2C6AxisTrackingLSM303DLHC.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOI2CLSM303DLHC::DIOI2CLSM303DLHC
*/
/**
//
//  Class Constructor DIOI2CLSM303DLHC
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 14:23:46
//
*/
/*-----------------------------------------------------------------*/
DIOI2CLSM303DLHC::DIOI2CLSM303DLHC() : DIODEVICEI2C()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOI2CLSM303DLHC::~DIOI2CLSM303DLHC
*/
/**
//
//   Class Destructor DIOI2CLSM303DLHC
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 14:24:14
//
*/
/*-----------------------------------------------------------------*/
DIOI2CLSM303DLHC::~DIOI2CLSM303DLHC()
{
  End();

  Clean();
}



/*-------------------------------------------------------------------
//  DIOI2CLSM303DLHC::End
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
bool DIOI2CLSM303DLHC::End()
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
//  DIOI2CLSM303DLHC::IniDevice
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 15:48:58
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CLSM303DLHC::IniDevice()
{
  diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO(diostreamcfg);
  if(!diostream)  return false;

  diostreamcfg->SetMode(DIOSTREAMMODE_MASTER);
  diostreamcfg->SetAddressSize(DIOSTREAMI2CCONFIG_ADDRESSSIZE_7BITS);

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {
          return DIODEVICE::Ini();
        }
    }

  return false;
}




/*-------------------------------------------------------------------
//  DIOI2CLSM303DLHC::ReadRegister
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      07/07/2014 22:01:54
//
//  @return       bool :
//
//  @param        reg :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CLSM303DLHC::ReadRegister(XBYTE reg, XBYTE& value)
{
  if(!diostream)          return false;
  if(!IsInitialized())    return false;

  bool  status;

  status = diostream->Write(&reg, 1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  if(!status) return false;

  status = diostream->WaitToFilledReadingBuffer(1, timeout);
  if(status) status = diostream->Read(&value, 1)?true:false;

  return status;
}



/*-------------------------------------------------------------------
//  DIOI2CLSM303DLHC::ReadRegister
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      07/07/2014 22:02:18
//
//  @return       bool :
//
//  @param        reg :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CLSM303DLHC::ReadRegister(XBYTE reg, XWORD& value)
{
  if(!diostream)  return false;
  if(!IsInitialized())  return false;

  bool  status;

  status = diostream->Write(&reg, 1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  if(!status) return false;

  status = diostream->WaitToFilledReadingBuffer(2, timeout);
  if(status) status = diostream->Read((XBYTE*)&value, 2)?true:false;

  return status;
}




/*-------------------------------------------------------------------
//  DIOI2CLSM303DLHC::WriteRegister
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      07/07/2014 22:02:26
//
//  @return       bool :
//
//  @param        reg :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CLSM303DLHC::WriteRegister(XBYTE reg, XBYTE value)
{
  if(!diostream)  return false;
  if(!IsInitialized())  return false;

  XBUFFER xbuffer;

  xbuffer.Add((XBYTE)reg);
  xbuffer.Add((XBYTE)value);

  bool status = diostream->Write(xbuffer)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}




/*-------------------------------------------------------------------
//  DIOI2C3DACCELEROMETERLSM303DLHC::Configure
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      26/06/2014 23:19:09
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C3DACCELEROMETERLSM303DLHC::Configure()
{
  if(!IsInitialized()) return false;

  XBYTE value;

  value  = 0x00;
  value |= 0x01 << 5;     // Normal mode
  value |= 0x07;          // X/Y/Z axis enable.
  if(!WriteRegister(DIOI2CLSM303DLHC_CTRL_REG1_A , value))    return false;
  value  = 0x00;
  ReadRegister(DIOI2CLSM303DLHC_CTRL_REG1_A, value);

  value  = 0x00;
  value |= 0x01 << 6;     // 1: data MSB @ lower address
  value |= 0x01 << 4;     // +/- 4g
  if(!WriteRegister(DIOI2CLSM303DLHC_CTRL_REG4_A, value))   return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOI2C3DACCELEROMETERLSM303DLHC::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      26/06/2014 23:43:54
//
//  @return       bool :
//
//  @param        a_x :
//  @param        a_y :
//  @param        a_z :
//  @param        m_x :
//  @param        m_y :
//  @param        m_z :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C3DACCELEROMETERLSM303DLHC::Read(XWORD& a_x, XWORD& a_y, XWORD& a_z)
{
  if(!IsInitialized()) return false;

  a_x = 0;
  a_y = 0;
  a_z = 0;

  if(!ReadRegister(DIOI2CLSM303DLHC_OUT_X_A, a_x)) return false;
  if(!ReadRegister(DIOI2CLSM303DLHC_OUT_Y_A, a_y)) return false;
  if(!ReadRegister(DIOI2CLSM303DLHC_OUT_Z_A, a_z)) return false;

  return true;
}






/*-------------------------------------------------------------------
//  DIOI2C3DCOMPASSLSM303DLHC::Configure
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      26/06/2014 23:19:09
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C3DCOMPASSLSM303DLHC::Configure()
{
  if(!IsInitialized()) return false;

  XBYTE value;

  value  = 0x00;
  value |= 0x04 << 2;     // Minimum data output rate = 15Hz
  if(!WriteRegister(DIOI2CLSM303DLHC_CRA_REG_M, value))           return false;

  value  = 0x00;
//value |= 0x01 << 5;     // +-1.3Gauss
  value |= 0x07 << 5;     // +-8.1Gauss
  if(!WriteRegister(DIOI2CLSM303DLHC_CRB_REG_M, value))           return false;

  value = 0x00;           // Continuous-conversion mode
  if(!WriteRegister(DIOI2CLSM303DLHC_MR_REG_M, value))            return false;

  return true;
}





/*-------------------------------------------------------------------
//  DIOI2C3DCOMPASSLSM303DLHC::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      26/06/2014 23:43:54
//
//  @return       bool :
//
//  @param        m_x :
//  @param        m_y :
//  @param        m_z :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C3DCOMPASSLSM303DLHC::Read(XWORD& c_x, XWORD& c_y, XWORD& c_z)
{
  if(!IsInitialized()) return false;

  c_x = 0;
  c_y = 0;
  c_z = 0;

  if(!ReadRegister(DIOI2CLSM303DLHC_OUT_X_M, c_x)) return false;
  if(!ReadRegister(DIOI2CLSM303DLHC_OUT_Y_M, c_y)) return false;
  if(!ReadRegister(DIOI2CLSM303DLHC_OUT_Z_M, c_z)) return false;

  return true;
}



