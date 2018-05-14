
/*------------------------------------------------------------------------------------------
//  DIOAIOBIMAINBOARD.CPP
//
//  Main Board AIOBI class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 17/02/2016 16:02:34
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/


#include "DIOAIOBIMainBoard.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOAIOBIMAINBOARD::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2016 16:08:54
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::Ini()
{

  for(int c=0; c<3; c++)
    {
      pcf8574[c] = new DIOI2CGPIOPCF8574();
      if(!pcf8574[c]) return false;
    }

  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN]->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME     , DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_IN      , DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C GPIO IN not init!"));
      return false;
    }

  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT]->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME    , DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_OUT     , DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C GPIO OUT not init!"));
      return false;
    }

  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME , DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_RELAYS  , DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C GPIO Relays not init!"));
      return false;
    }

  for(int c=1; c<9; c++)
    {
      SetDigitalOutput(c, false);
    }

  eeprom  = new DIOI2CEEPROM24XXX();
  if(!eeprom) return false;

  eeprom->SetType(DIOI2CEEPROM24XXXTYPE_16);

  if(!eeprom->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME, DIOAIOBIMAINBOARD_I2CEEPROM, DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C Serial EEPROM not init!"));
      return false;
    }


  pca9685 = new DIOI2CPWMCONTROLERPCA9685();
  if(!pca9685) return false;

  if(!pca9685->Ini(DIOAIOBIMAINBOARD_I2CDEVICENAME, DIOAIOBIMAINBOARD_I2CPWM, DIOAIOBIMAINBOARD_DEFAULTTIMEOUT))
    {
      XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! I2C PCA9685 (PWM) not init!"));
      return false;
    }


  gpio = diofactory->CreateGPIO();
  if(!gpio)
    {
      XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR, DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! GPIO not created!"));
      return false;
    }

  if(!gpio->Ini())
    {
      XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR,  DIOAIOBIMAINBOARD_LOGSECTIONID, false, __L("AIOBI Main Board: ERROR! GPIO not init!"));
      return false;
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOAIOBIMAINBOARD::SwitchReleK1
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2016 9:36:10
//
//  @return       bool :
//
//  @param        on :
*/
/*-----------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SwitchReleK1(bool on)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS])                  return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->IsInitialized()) return false;

  XBYTE value = 0;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Read(value)) return false;

  value = on?(value|0x01):(value&~0x01);
  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Write(value);
}




/*-------------------------------------------------------------------
//  DIOAIOBIMAINBOARD::SwitchReleK2
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      22/02/2016 13:27:11
//
//  @return       bool :
//
//  @param        on :
*/
/*-----------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SwitchReleK2(bool on)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS])                  return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->IsInitialized()) return false;

  XBYTE value = 0;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Read(value)) return false;

  value = on?(value|0x02):(value&~0x02);
  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Write(value);
}



/*-------------------------------------------------------------------
//  DIOAIOBIMAINBOARD::SwitchReleK3
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      22/02/2016 13:27:50
//
//  @return       bool :
//
//  @param        on :
*/
/*-----------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SwitchReleK3(bool on)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS])                  return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->IsInitialized()) return false;

  XBYTE value = 0;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Read(value)) return false;

  value = on?(value|0x04):(value&~0x04);
  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS]->Write(value);
}





/*-------------------------------------------------------------------
//  DIOAIOBIMAINBOARD::GetDigitalInput
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/02/2016 16:07:31
//
//  @return       bool :
//
//  @param        XBYTE& data :
*/
/*-----------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::GetDigitalInput(XBYTE& data)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN])                          return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN]->IsInitialized())         return false;

  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN]->Read(data);
}




/*-------------------------------------------------------------------
//  DIOAIOBIMAINBOARD::SetDigitalOutput
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/02/2016 16:12:02
//
//  @return       bool :
//
//  @param        bit :
//  @param        on :
*/
/*-----------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::SetDigitalOutput(XBYTE bit, bool on)
{
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT])                           return false;
  if(!pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT]->IsInitialized())          return false;

  XBYTE mask = 0x01;

  if(bit-1)
    {
      mask <<= (bit-1);
    }

  if(on)
        outputdigitaldata |=  mask;
   else outputdigitaldata &= ~mask;

  return pcf8574[DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT]->Write(outputdigitaldata);
}







/*-------------------------------------------------------------------
//  DIOAIOBIMAINBOARD::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2016 16:08:58
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOAIOBIMAINBOARD::End()
{
  for(int c=0; c<3; c++)
    {
      if(pcf8574[c])
        {
          pcf8574[c]->End();

          delete pcf8574[c];
          pcf8574[c] = NULL;
        }
    }

  if(eeprom)
    {
      eeprom->End();
      delete eeprom;
      eeprom = NULL;
    }


  if(pca9685)
    {
      pca9685->GetDIOStream()->WaitToFlushXBuffers(3);
      pca9685->End();
      delete pca9685;
      pca9685 = NULL;
    }

  if(gpio)
    {
      SetLedA(false);
      SetLedB(false);

      gpio->End();
      diofactory->DeleteGPIO(gpio);
      gpio = NULL;
    }

  return true;
}