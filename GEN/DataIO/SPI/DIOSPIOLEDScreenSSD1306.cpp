/*------------------------------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306.CPP
//
//  Data IO SPI Screen LCD Philips PCF8833
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 01/06/2014 18:00:35
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"

#include "DIOSPIOLEDScreenSSD1306.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::DIOSPIOLEDSCREENSSD1306
*/
/**
//
//  Class Constructor DIOSPIOLEDSCREENSSD1306
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:16:15
//


//  @param        xpublisher :
//  @param        xsystem :
//  @param        diostreamspi :
*/
/*-----------------------------------------------------------------*/
DIOSPIOLEDSCREENSSD1306::DIOSPIOLEDSCREENSSD1306(XSYSTEM* xsystem) : DIODEVICESPI()
{
  Clean();

  this->xsystem = xsystem;
  databuffer = new XBUFFER();

}



/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::~DIOSPIOLEDSCREENSSD1306
*/
/**
//
//   Class Destructor DIOSPIOLEDSCREENSSD1306
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:17:22
//
*/
/*-----------------------------------------------------------------*/
DIOSPIOLEDSCREENSSD1306::~DIOSPIOLEDSCREENSSD1306()
{
  End();

  if(databuffer)
    {
      delete databuffer;
      databuffer = NULL;
    }

  Clean();
}




/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::IniDevice
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/10/2016 9:33:12
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1306::IniDevice()
{
  if(!diogpio) return false;

  diostream = (DIOSTREAMSPI*)diofactory->CreateStreamIO(diostreamcfg);
  if(!diostream)  return false;

  diostreamcfg->SetMode(DIOSTREAMMODE_MASTER);
  diostream->SetDIOGPIO(diogpio);

  diogpio->SetMode(resetpin, false, resetpinsgroup);
  diogpio->Set(resetpin, true, resetpinsgroup);

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {
          width            = 132;
          height           = 4;//132;
          sizebuffervideo  = ((width*height)*(sizeof(XWORD)));
          sizebuffervideo += ((width*height)*(sizeof(XWORD))/8);


          if(!LCD_Init()) return false;

          return DIODEVICE::Ini();
        }
    }

  return false;
}


/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::Clear
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
bool DIOSPIOLEDSCREENSSD1306::Clear(XWORD color)
{
  if(!IsInitialized()) return false;

  Buffer_Delete();

  databuffer->Resize(sizebuffervideo + 9);    //39213;

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_PASET);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(0);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(width-1);

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_CASET);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(0);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(height-1);

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_RAMWR);

  XBYTE bcolor[2];

  bcolor[0] = (XBYTE)(color >> 8);
  bcolor[1] = (XBYTE)(0x00FF & color);

  for(int c=0; c<(width * height); c++)
    {
      DIOSPIOLEDSCREENSSD1306_SENDDATA(bcolor[0]);
      DIOSPIOLEDSCREENSSD1306_SENDDATA(bcolor[1]);
    }

  return Buffer_Send();
}




/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::Update
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
bool DIOSPIOLEDSCREENSSD1306::Update(XBYTE* buffer)
{
  if(!IsInitialized()) return false;
  if(!buffer)          return false;

  Buffer_Delete();

  databuffer->Resize(sizebuffervideo + 9);

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_PASET);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(0);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(width-1);

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_CASET);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(0);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(height-1);

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_RAMWR);

  int d = 0;

  for(int c=0; c<(width * height); c++)
    {
      DIOSPIOLEDSCREENSSD1306_SENDDATA(buffer[d+1]);
      DIOSPIOLEDSCREENSSD1306_SENDDATA(buffer[d]);
      d+=2;
    }

  return Buffer_Send();
}




/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::End
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
bool DIOSPIOLEDSCREENSSD1306::End()
{
  if(!DIODEVICE::End()) return false;

  if(diostream)
    {
      diostream->Close();
      diofactory->DeleteStreamIO(diostream);
      diostream = NULL;
    }

  Clear();

  return true;
}



/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::LCD_Init
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
bool DIOSPIOLEDSCREENSSD1306::LCD_Init()
{
  LCD_Reset();

  Buffer_Delete();

  if(!databuffer->Resize(18)) return false;

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_SLEEPOUT);
  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_INVOFF);
  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_COLMOD);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(0x05);

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_MADCTL);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(0x00);
  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_SETCON);
  DIOSPIOLEDSCREENSSD1306_SENDDATA(0x3B);
  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_TEOFF);

  if(!Buffer_Send()) return false;

  Buffer_Delete();

  databuffer->Resize(9);

  xsleep->MilliSeconds(100);

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_DISPON);

  if(!Buffer_Send()) return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::LCD_Reset
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
bool DIOSPIOLEDSCREENSSD1306::LCD_Reset()
{
  if(!diostream)                        return false;
  if(resetpin == DIOGPIO_PINNOTUSE) return false;
  if(!diogpio)                      return false;

  diogpio->Set(resetpin, false, resetpinsgroup);
  xsleep->MilliSeconds(100);

  diogpio->Set(resetpin, true, resetpinsgroup);
  xsleep->MilliSeconds(100);

  return true;
}




/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::Buffer_AdjustWithNOP
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:31:51
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1306::Buffer_AdjustWithNOP()
{
  if(!databuffer) return false;
  XBYTE nbitsfree = databuffer->GetNBitsFree();

  DIOSPIOLEDSCREENSSD1306_SENDCOMMAND(DIOSPIOLEDSCREENSSD1306_NOP);

  return databuffer->GetNBitsFree()?false:true;
}



/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::Buffer_Send
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/06/2014 11:56:56
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1306::Buffer_Send()
{
  Buffer_AdjustWithNOP();

  bool status = false;
  if(diostream) status = diostream->Write((*databuffer))?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}



/*-------------------------------------------------------------------
//  DIOSPIOLEDSCREENSSD1306::Buffer_Delete
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:33:35
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1306::Buffer_Delete()
{
  if(!databuffer) return false;

  databuffer->Delete();
  databuffer->AdjustToNBits(9);
  databuffer->SetNBitsFree(0);

  return true;
}

