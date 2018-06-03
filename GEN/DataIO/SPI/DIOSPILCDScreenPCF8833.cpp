/*------------------------------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833.CPP
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
#include "XDebugTrace.h"

#include "DIOFactory.h"
#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"

#include "DIOSPILCDScreenPCF8833.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::DIOSPILCDSCREENPCF8833
*/
/**
//
//  Class Constructor DIOSPILCDSCREENPCF8833
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:16:15
//


//  @param        xpublisher :
//  @param        diostreamspi :
*/
/*-----------------------------------------------------------------*/
DIOSPILCDSCREENPCF8833::DIOSPILCDSCREENPCF8833() : DIODEVICESPI()
{
  Clean();

  databuffer = new XBUFFER();
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::~DIOSPILCDSCREENPCF8833
*/
/**
//
//   Class Destructor DIOSPILCDSCREENPCF8833
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:17:22
//
*/
/*-----------------------------------------------------------------*/
DIOSPILCDSCREENPCF8833::~DIOSPILCDSCREENPCF8833()
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
//  DIOSPILCDSCREENPCF8833::IniDevice
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
bool DIOSPILCDSCREENPCF8833::IniDevice()
{
  if(!diogpio) return false;

  diostream = (DIOSTREAMSPI*)diofactory->CreateStreamIO(diostreamcfg);
  if(!diostream)  return false;

  diostreamcfg->SetMode(DIOSTREAMMODE_MASTER);

  diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_2);
  diostreamcfg->SetNBitsWord(8);
  diostreamcfg->SetSpeed(8*1000*1000);
  diostreamcfg->SetDelay(0);
  diostreamcfg->SetIsOnlyWrite(true);

  diostream->SetDIOGPIO(diogpio);

  diogpio->SetMode(resetpin, false, resetpinsgroup);
  diogpio->Set(resetpin, true, resetpinsgroup);

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {
          width            = 132;
          height           = 132;
          sizebuffervideo  = ((width*height)*(sizeof(XWORD)));
          sizebuffervideo += ((width*height)*(sizeof(XWORD))/8);

          if(!LCD_Init()) return false;

          return DIODEVICE::Ini();
        }
    }

  return false;
}


/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::Clear
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
bool DIOSPILCDSCREENPCF8833::Clear(XWORD color)
{
  if(!IsInitialized()) return false;

  Buffer_Delete();

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_PASET);
  DIOSPILCDSCREENPCF8833_ADDDATA(0);
  DIOSPILCDSCREENPCF8833_ADDDATA(height-1);

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_CASET);
  DIOSPILCDSCREENPCF8833_ADDDATA(0);
  DIOSPILCDSCREENPCF8833_ADDDATA(width-1);

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_RAMWR);

  XBYTE bcolor[2];

  bcolor[0] = (XBYTE)(color >> 8);
  bcolor[1] = (XBYTE)(0x00FF & color);

  for(int c=0; c<(width * height); c++)
    {
      DIOSPILCDSCREENPCF8833_ADDDATA(bcolor[0]);
      DIOSPILCDSCREENPCF8833_ADDDATA(bcolor[1]);
    }

  return Buffer_Send();
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::Update
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
bool DIOSPILCDSCREENPCF8833::Update(XBYTE* buffer)
{
  if(!IsInitialized()) return false;
  if(!buffer)          return false;

  Buffer_Delete();

  SetWindow(0, 0, (width-1), (height-1));

  int d = 0;
  for(int c=0; c<(width * height); c++)
    {
      DIOSPILCDSCREENPCF8833_ADDDATA(buffer[d+1]);
      DIOSPILCDSCREENPCF8833_ADDDATA(buffer[d]);
      d+=2;
    }

  return Buffer_Send();
}





/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::SetPixel
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/11/2016 8:56:20
//
//  @return       bool :
//
//  @param        x :
//  @param        y :
//  @param        color :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENPCF8833::PutPixel(XWORD x, XWORD y, XWORD color)
{
  //if(!IsInitialized()) return false;

  if(x >= width)  return false;
  if(y >= height) return false;

  Buffer_Delete();

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_PASET);
  DIOSPILCDSCREENPCF8833_ADDDATA(y);
  DIOSPILCDSCREENPCF8833_ADDDATA(y+1);

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_CASET);
  DIOSPILCDSCREENPCF8833_ADDDATA(x);
  DIOSPILCDSCREENPCF8833_ADDDATA(x+1);

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_RAMWR);

  XWORD bcolor[2];

  bcolor[0] = (XBYTE)(color >> 8);
  bcolor[1] = (XBYTE)(0x00FF & color);

  DIOSPILCDSCREENPCF8833_ADDDATA(bcolor[0]);
  DIOSPILCDSCREENPCF8833_ADDDATA(bcolor[1]);

  return Buffer_Send();
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::SetInversion
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/11/2016 8:58:43
//
//  @return       bool :
//
//  @param        on :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENPCF8833::SetInversion(bool on)
{
  DIOSPILCDSCREENPCF8833_ADDCOMMAND(on?DIOSPILCDSCREENPCF8833_INVON:DIOSPILCDSCREENPCF8833_INVOFF);
  return true;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::SetWindow
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2016 12:19:53
//
//  @param        x0 :
//  @param        y0 :
//  @param        x1 :
//  @param        y1 :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENPCF8833::SetWindow(XWORD x0, XWORD y0, XWORD x1, XWORD y1)
{

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_PASET);
  DIOSPILCDSCREENPCF8833_ADDDATA(y0);
  DIOSPILCDSCREENPCF8833_ADDDATA(y1);

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_CASET);
  DIOSPILCDSCREENPCF8833_ADDDATA(x0);
  DIOSPILCDSCREENPCF8833_ADDDATA(x1);

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_RAMWR);

  return true;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::End
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
bool DIOSPILCDSCREENPCF8833::End()
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
//  DIOSPILCDSCREENPCF8833::LCD_Reset
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
bool DIOSPILCDSCREENPCF8833::LCD_Reset()
{
  if(!diostream)                        return false;
  if(resetpin == DIOGPIO_PINNOTUSE) return false;
  if(!diogpio)                      return false;

  diogpio->Set(resetpin, false, resetpinsgroup);
  XSLEEP::GetInstance().MilliSeconds(200);

  diogpio->Set(resetpin, true, resetpinsgroup);
  XSLEEP::GetInstance().MilliSeconds(200);

  return true;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::LCD_Init
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
bool DIOSPILCDSCREENPCF8833::LCD_Init()
{
  LCD_Reset();

  Buffer_Delete();

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_SLEEPOUT);
  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_INVOFF);
  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_COLMOD);
  DIOSPILCDSCREENPCF8833_ADDDATA(0x05);
  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_MADCTL);
  DIOSPILCDSCREENPCF8833_ADDDATA(0x00);
  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_SETCON);
  DIOSPILCDSCREENPCF8833_ADDDATA(0x3B);
  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_TEOFF);

  if(!Buffer_Send()) return false;

  XSLEEP::GetInstance().MilliSeconds(50);

  Buffer_Delete();

  DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_DISPON);

  if(!Buffer_Send()) return false;

  return true;
}






/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::Buffer_AdjustWithNOP
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
bool DIOSPILCDSCREENPCF8833::Buffer_AdjustWithNOP()
{
  if(!databuffer) return false;
  XBYTE nbitsfree = databuffer->GetNBitsFree();

  if(nbitsfree) DIOSPILCDSCREENPCF8833_ADDCOMMAND(DIOSPILCDSCREENPCF8833_NOP);

  return databuffer->GetNBitsFree()?false:true;
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::Buffer_Resize
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2016 12:47:00
//
//  @return       bool :
//
//  @param        newsize :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENPCF8833::Buffer_Resize(XDWORD newsize)
{
  if(!databuffer) return false;

  Buffer_Delete();

  return databuffer->Resize(newsize);
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::Buffer_Send
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
bool DIOSPILCDSCREENPCF8833::Buffer_Send()
{
  Buffer_AdjustWithNOP();

  bool status = false;
  if(diostream) status = diostream->Write((*databuffer))?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPCF8833::Buffer_Delete
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
bool DIOSPILCDSCREENPCF8833::Buffer_Delete()
{
  if(!databuffer) return false;

  databuffer->Delete();
  databuffer->AdjustToNBits(9);
  databuffer->SetNBitsFree(0);

  return true;
}

