/*------------------------------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833.CPP
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

#include "DIOSPILCDPhilipsPCF8833.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::DIOSPILCDSCREENPHILIPSPCF8833
*/
/**
//
//  Class Constructor DIOSPILCDSCREENPHILIPSPCF8833
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:16:15
//


//  @param        xpublisher :
//  @param        xsystem :
//  @param        diostreamspi :
*/
/*-----------------------------------------------------------------*/
DIOSPILCDSCREENPHILIPSPCF8833::DIOSPILCDSCREENPHILIPSPCF8833(XSYSTEM* xsystem) : DIODEVICESPI()
{
  Clean();

  this->xsystem = xsystem;
  databuffer = new XBUFFER();

}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::~DIOSPILCDSCREENPHILIPSPCF8833
*/
/**
//
//   Class Destructor DIOSPILCDSCREENPHILIPSPCF8833
//
//  @author       Abraham J. Velez
//  @version      01/06/2014 18:17:22
//
*/
/*-----------------------------------------------------------------*/
DIOSPILCDSCREENPHILIPSPCF8833::~DIOSPILCDSCREENPHILIPSPCF8833()
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
//  DIOSPILCDSCREENPHILIPSPCF8833::IniDevice
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
bool DIOSPILCDSCREENPHILIPSPCF8833::IniDevice()
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
//  DIOSPILCDSCREENPHILIPSPCF8833::Clear
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
bool DIOSPILCDSCREENPHILIPSPCF8833::Clear(XWORD color)
{
  //if(!IsInitialized()) return false;

  Buffer_Delete();

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_PASET);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(0);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(width-1);

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_CASET);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(0);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(height-1);

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_RAMWR);

  XBYTE bcolor[2];

  bcolor[0] = (XBYTE)(color >> 8);
  bcolor[1] = (XBYTE)(0x00FF & color);

  for(int c=0; c<(width * height); c++)
    {
      DIOSPILCDSCREENPHILIPSPCF8833_DATA(bcolor[0]);
      DIOSPILCDSCREENPHILIPSPCF8833_DATA(bcolor[1]);
    }

  return Buffer_Send();
}




/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::Update
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
bool DIOSPILCDSCREENPHILIPSPCF8833::Update(XBYTE* buffer)
{
  if(!IsInitialized()) return false;
  if(!buffer)          return false;

  Buffer_Delete();

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_PASET);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(0);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(width-1);

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_CASET);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(0);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(height-1);

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_RAMWR);

  int d = 0;

  for(int c=0; c<(width * height); c++)
    {
      DIOSPILCDSCREENPHILIPSPCF8833_DATA(buffer[d+1]);
      DIOSPILCDSCREENPHILIPSPCF8833_DATA(buffer[d]);
      d+=2;
    }

  return Buffer_Send();
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::PutPixel
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      26/11/2016 21:30:08
//
//  @return       bool :
//
//  @param        x :
//  @param        y :
//  @param        color :
*/
/*-----------------------------------------------------------------*/
bool DIOSPILCDSCREENPHILIPSPCF8833::PutPixel(XWORD x, XWORD y, XWORD color)
{
  if(!IsInitialized()) return false;

  if(x >= width)  return false;
  if(y >= height) return false;

  Buffer_Delete();

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_PASET);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(y);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(y);

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_CASET);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(x);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(x);

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_RAMWR);

  XWORD bcolor[2];

  bcolor[0] = (XBYTE)(color >> 8);
  bcolor[1] = (XBYTE)(0x00FF & color);

  DIOSPILCDSCREENPHILIPSPCF8833_DATA(bcolor[0]);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(bcolor[1]);

  return Buffer_Send();
}





/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::End
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
bool DIOSPILCDSCREENPHILIPSPCF8833::End()
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
//  DIOSPILCDSCREENPHILIPSPCF8833::LCD_Reset
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
bool DIOSPILCDSCREENPHILIPSPCF8833::LCD_Reset()
{
  if(!diostream)                        return false;
  if(resetpin == DIOGPIO_PINNOTUSE) return false;
  if(!diogpio)                      return false;

  diogpio->Set(resetpin, false, resetpinsgroup);
  xsleep->MilliSeconds(10);

  diogpio->Set(resetpin, true, resetpinsgroup);
  xsleep->MilliSeconds(100);

  return true;
}


/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::LCD_Init
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
bool DIOSPILCDSCREENPHILIPSPCF8833::LCD_Init()
{
  LCD_Reset();


  Buffer_Delete();

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_SLEEPOUT);
  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_INVOFF);
  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_COLMOD);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(0x05);
  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_MADCTL);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(0x00);
  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_SETCON);
  DIOSPILCDSCREENPHILIPSPCF8833_DATA(0x3B);
  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_TEOFF);

  if(!Buffer_Send()) return false;

  xsleep->MilliSeconds(100);

  Buffer_Delete();
  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_DISPON);
  if(!Buffer_Send()) return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::Buffer_AdjustWithNOP
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
bool DIOSPILCDSCREENPHILIPSPCF8833::Buffer_AdjustWithNOP()
{
  if(!databuffer) return false;
  XBYTE nbitsfree = databuffer->GetNBitsFree();

  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_NOP);
  DIOSPILCDSCREENPHILIPSPCF8833_COMMAND(DIOSPILCDSCREENPHILIPSPCF8833_NOP);

  return databuffer->GetNBitsFree()?false:true;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::Buffer_Send
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
bool DIOSPILCDSCREENPHILIPSPCF8833::Buffer_Send()
{
  //Buffer_AdjustWithNOP();

  bool status = false;
  if(diostream) status = diostream->Write(databuffer->Get(), databuffer->GetSize()+1)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}



/*-------------------------------------------------------------------
//  DIOSPILCDSCREENPHILIPSPCF8833::Buffer_Delete
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
bool DIOSPILCDSCREENPHILIPSPCF8833::Buffer_Delete()
{
  if(!databuffer) return false;

  databuffer->Delete();
  databuffer->AdjustToNBits(9);
  databuffer->SetNBitsFree(0);

  return true;
}

