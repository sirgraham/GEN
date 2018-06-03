/*------------------------------------------------------------------------------------------
//  DIOI2COLEDSCREENSSD1306.CPP
//
//  Data IO I2C Sensor AOSONG AM2315  class (Temperature and humidity module)
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
#include "XDebugTrace.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"

#include "DIOI2COLEDScreenSSD1306.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOI2COLEDSCREENSSD1306::DIOI2COLEDSCREENSSD1306
*/
/**
//
//  Class Constructor DIOI2COLEDSCREENSSD1306
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:37:54
//
*/
/*-----------------------------------------------------------------*/
DIOI2COLEDSCREENSSD1306::DIOI2COLEDSCREENSSD1306() : DIODEVICEI2C()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOI2COLEDSCREENSSD1306::~DIOI2COLEDSCREENSSD1306
*/
/**
//
//   Class Destructor DIOI2COLEDSCREENSSD1306
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:25:15
//
*/
/*-----------------------------------------------------------------*/
DIOI2COLEDSCREENSSD1306::~DIOI2COLEDSCREENSSD1306()
{
  End();

  Clean();
}





/*-------------------------------------------------------------------
//  DIOI2COLEDSCREENSSD1306::End
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
bool DIOI2COLEDSCREENSSD1306::End()
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
//  DIOI2COLEDSCREENSSD1306::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      25/05/2014 18:55:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2COLEDSCREENSSD1306::IniDevice()
{
  diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO(diostreamcfg);
  if(!diostream)  return false;

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {
          XBUFFER xbuffer;

          xbuffer.Add((XBYTE)(0xAE));   //  Display off
          xbuffer.Add((XBYTE)(0x00));   //  Set Memory Addressing Mode
          xbuffer.Add((XBYTE)(0x10));   //  00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
          xbuffer.Add((XBYTE)(0x40));   //  Set Page Start Address for Page Addressing Mode,0-7
          xbuffer.Add((XBYTE)(0x81));   //  Set COM Output Scan Direction
          xbuffer.Add((XBYTE)(0xCF));   //  ---set low column address
          xbuffer.Add((XBYTE)(0xA1));   //  ---set high column address
          xbuffer.Add((XBYTE)(0xC8));   //  --set start line address
          xbuffer.Add((XBYTE)(0xA6));   //  --set contrast control register
          xbuffer.Add((XBYTE)(0xA8));
          xbuffer.Add((XBYTE)(0x3F));   //  --set segment re-map 0 to 127
          xbuffer.Add((XBYTE)(0xD3));   //  --set normal display
          xbuffer.Add((XBYTE)(0x00));   //  --set multiplex ratio(1 to 64)
          xbuffer.Add((XBYTE)(0xD5));   //
          xbuffer.Add((XBYTE)(0x80));   //  0xa4,Output follows RAM content;0xa5,Output ignores RAM content
          xbuffer.Add((XBYTE)(0xD9));   //  -set display offset
          xbuffer.Add((XBYTE)(0xF1));   //  -not offset
          xbuffer.Add((XBYTE)(0xDA));   //  --set display clock divide ratio/oscillator frequency
          xbuffer.Add((XBYTE)(0x12));   //  --set divide ratio
          xbuffer.Add((XBYTE)(0xDB));   //  --set pre-charge period
          xbuffer.Add((XBYTE)(0x40));   //
          xbuffer.Add((XBYTE)(0x20));   //  --set com pins hardware configuration
          xbuffer.Add((XBYTE)(0x02));
          xbuffer.Add((XBYTE)(0x8D));   //  --set vcomh
          xbuffer.Add((XBYTE)(0x14));   //  0x20,0.77xVcc
          xbuffer.Add((XBYTE)(0xA4));   //  --set DC-DC enable
          xbuffer.Add((XBYTE)(0xA6));   //
          xbuffer.Add((XBYTE)(0xAF));   //  --turn on oled panel

          diostream->Write(xbuffer);
          if(!diostream->WaitToFlushOutXBuffer(5)) return false;

          return DIODEVICE::Ini();
        }
    }

  return false;
}
