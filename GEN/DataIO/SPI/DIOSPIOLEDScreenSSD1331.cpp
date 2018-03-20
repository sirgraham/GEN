/*------------------------------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331.CPP
//	
//	Data IO SPI Screen LCD Philips PCF8833
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 01/06/2014 18:00:35
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"

#include "DIOSPIOLEDScreenSSD1331.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::DIOSPIOLEDSCREENSSD1331
*/	
/**	
//	
//	Class Constructor DIOSPIOLEDSCREENSSD1331
//	
//	@author				Abraham J. Velez
//	@version			01/06/2014 18:16:15
//	
 
 
//  @param				xpublisher : 
//  @param				xsystem : 
//  @param				diostreamspi : 
*/
/*-----------------------------------------------------------------*/
DIOSPIOLEDSCREENSSD1331::DIOSPIOLEDSCREENSSD1331() : DIODEVICESPI()
{
	Clean();
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::~DIOSPIOLEDSCREENSSD1331
*/	
/**	
//	
//	 Class Destructor DIOSPIOLEDSCREENSSD1331
//	
//	@author				Abraham J. Velez
//	@version			01/06/2014 18:17:22
//	
*/
/*-----------------------------------------------------------------*/
DIOSPIOLEDSCREENSSD1331::~DIOSPIOLEDSCREENSSD1331()
{	
	End();	

	Clean();
}




/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::IniDevice
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/10/2016 9:33:12
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::IniDevice()
{
	if(!diogpio) return false;
	
	diostream = (DIOSTREAMSPI*)diofactory->CreateStreamIO(diostreamcfg);
	if(!diostream)  return false;

  diostreamcfg->SetMode(DIOSTREAMMODE_MASTER);

	diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_3);	
	diostreamcfg->SetNBitsWord(8);		
	diostreamcfg->SetSpeed(16*1000*1000);	
	diostreamcfg->SetDelay(0);
	diostreamcfg->SetIsOnlyWrite(true);
 
	diostream->SetDIOGPIO(diogpio);

	diogpio->SetMode(resetpin, false, resetpinsgroup);		
	diogpio->Set(resetpin, true, resetpinsgroup);	

	diogpio->SetMode(DCpin, false, DCpinsgroup);		
	diogpio->Set(DCpin, true, DCpinsgroup);	
	
	if(diostream->Open())
    {			
      if(diostream->WaitToConnected(timeout))
        {	
					width						 = 96;
					height					 = 64;
					sizebuffervideo  = ((width*height)*(sizeof(XWORD))); 
					             																	
					if(!LCD_Init()) return false;
                  
          return DIODEVICE::Ini();	
        }
    }
  
	return false;
}




/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::Clear
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2014 19:09:37
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::Clear(XWORD color)
{
	if(!IsInitialized()) return false;

	if(!LCD_EnableFillMode(true)) return false;
	
	if(!LCD_FillRect(0 ,0 ,width-1 ,height-1 ,color ,color)) return false;

	return LCD_EnableFillMode(false);
}




/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::Update
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2014 18:20:09
//	
//	@return 			bool : 
//
//  @param				buffer : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::Update(XBYTE* buffer)
{	
	if(!IsInitialized()) return false;
	if(!buffer)					 return false;

	if(!LCD_MaxWindow()) return false;

	WriteData(buffer, sizebuffervideo);

	return true;
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::PutPixel
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/12/2016 20:42:28
//	
//	@return 			bool : 
//
//  @param				x : 
//  @param				y : 
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::PutPixel(XWORD x, XWORD y, XWORD color)
{
	if(!IsInitialized()) return false;

	XBYTE cmd[]= { DIOSPIOLEDSCREENSSD1331_CMD_SETCOLUMNADDRESS, (XBYTE)x, (XBYTE)x, DIOSPIOLEDSCREENSSD1331_CMD_SETROWADDRESS, (XBYTE)y, (XBYTE)y };

	if(!WriteCommand(cmd, sizeof(cmd))) return false;
  
	XBYTE datacolor[2];

	datacolor[0] = (color >> 8);
	datacolor[1] = (color &  0xFF);
	
	return WriteData(datacolor, sizeof(datacolor));
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::End
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2014 20:52:53
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::End()
{	
	LCD_Active(false);

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
//	DIOSPIOLEDSCREENSSD1331::LCD_Reset
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2014 18:28:16
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_Reset()
{
	if(!diostream)										return false;
	if(resetpin == DIOGPIO_PINNOTUSE) return false;
	if(!diogpio)											return false;
	
	diogpio->Set(DCpin, false, DCpinsgroup);			
	xsleep->MilliSeconds(10);

	diogpio->Set(resetpin, false, resetpinsgroup);  
	xsleep->MilliSeconds(10); 

	diogpio->Set(resetpin, true, resetpinsgroup);			
	xsleep->MilliSeconds(10);

	return true;
}



													
/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_Init
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2014 18:26:17
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_Init()
{  	
	LCD_Reset();
	
	// --------------------------------------------------------------
	// Initialize sequence
	//

	XBYTE cmd[] = {  DIOSPIOLEDSCREENSSD1331_CMD_DISPLAYOFF							,
									 DIOSPIOLEDSCREENSSD1331_CMD_SETREMAP								, 0x72 ,
									 DIOSPIOLEDSCREENSSD1331_CMD_SETDISPLAYSTARTLINE		, 0x00 ,
									 DIOSPIOLEDSCREENSSD1331_CMD_SETDISPLAYOFFSET				, 0x00 ,
									 DIOSPIOLEDSCREENSSD1331_CMD_NORMALDISPLAY					,		
									 DIOSPIOLEDSCREENSSD1331_CMD_SETMULTIPLEXRATIO			, 0x3F ,
									 DIOSPIOLEDSCREENSSD1331_CMD_SETMASTERCONFIGURE			, 0x8E ,
									 DIOSPIOLEDSCREENSSD1331_CMD_POWERSAVEMODE					, 0x0B ,  // Disabled.
									 DIOSPIOLEDSCREENSSD1331_CMD_PHASE12PERIOD					, 0x74 ,  // Default value.
									 DIOSPIOLEDSCREENSSD1331_CMD_CLOCKDIVIDER					  , 0xD0 ,  // Default value.
									 DIOSPIOLEDSCREENSSD1331_CMD_SETPRECHARGESPEEDA			, 0x80 ,
									 DIOSPIOLEDSCREENSSD1331_CMD_SETPRECHARGESPEEDB			, 0x80 , 
									 DIOSPIOLEDSCREENSSD1331_CMD_SETPRECHARGESPEEDC			, 0x80 ,
									 DIOSPIOLEDSCREENSSD1331_CMD_SETPRECHARGEVOLTAGE		, 0x3E ,// Default value.
									 DIOSPIOLEDSCREENSSD1331_CMD_SETVVOLTAGE						, 0x3E ,// Default value.
									 DIOSPIOLEDSCREENSSD1331_CMD_MASTERCURRENTCONTROL		, 0x0F ,
 									 DIOSPIOLEDSCREENSSD1331_CMD_SETCONTRASTA						, 0xFF ,
									 DIOSPIOLEDSCREENSSD1331_CMD_SETCONTRASTB						, 0xFF ,
									 DIOSPIOLEDSCREENSSD1331_CMD_SETCONTRASTC						, 0xFF ,
									 DIOSPIOLEDSCREENSSD1331_CMD_DISPLAYON	
								};
	
	WriteCommand(cmd, sizeof(cmd));

	LCD_Clear();
	
	return true;
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_Active
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/12/2016 21:32:04
//	
//	@return 			bool : 
//
//  @param				on : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_Active(bool on)
{		
	if(!WriteCommand((on?DIOSPIOLEDSCREENSSD1331_CMD_DISPLAYON:DIOSPIOLEDSCREENSSD1331_CMD_DISPLAYOFF))) return false;

	return true;
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_EnableFillMode
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/12/2016 21:23:01
//	
//	@return 			bool : 
//
//  @param				on : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_EnableFillMode(bool on)
{
	XBYTE cmd[]=	{  (XBYTE)DIOSPIOLEDSCREENSSD1331_CMD_FILLRECT																															,  
									 on?(XBYTE)DIOSPIOLEDSCREENSSD1331_CMD_FILLRECTENABLE:(XBYTE)DIOSPIOLEDSCREENSSD1331_CMD_FILLRECTDISABLE
								};
  	
	return WriteCommand(cmd, sizeof(cmd));	
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_EnableLockMode
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/12/2016 21:24:38
//	
//	@return 			bool : 
//
//  @param				on : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_EnableLockMode(bool on)
{		
	XBYTE cmd[]	=	{ (XBYTE)DIOSPIOLEDSCREENSSD1331_CMD_LOCKMODE																																	,  
									on? (XBYTE)DIOSPIOLEDSCREENSSD1331_CMD_LOCKMODEENABLE: (XBYTE)DIOSPIOLEDSCREENSSD1331_CMD_LOCKMODEDISABLE 
								};
  	
	return WriteCommand(cmd, sizeof(cmd));		
}


/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_EnableScrollMode
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/12/2016 21:27:13
//	
//	@return 			bool : 
//
//  @param				on : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_EnableScrollMode(bool on)
{
	if(!WriteCommand((on?DIOSPIOLEDSCREENSSD1331_CMD_ACTIVESCROLLING:DIOSPIOLEDSCREENSSD1331_CMD_DEACTIVESCROLLING))) return false;

	return true;
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_SetScrollMode
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/12/2016 21:30:09
//	
//	@return 			bool : 
//
//  @param				horizontal : 
//  @param				vertical : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_SetScrollMode(XWORD horizontal, XWORD vertical)
{
	LCD_EnableScrollMode(false);

	XBYTE cmd[]= { DIOSPIOLEDSCREENSSD1331_CMD_CONTINUOUSSCROLLINGSETUP, (XBYTE)horizontal, 0x00, 0x3F, (XBYTE)vertical, 0x00 };

	return WriteCommand(cmd, sizeof(cmd));		
}




/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_Clear
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			05/12/2016 22:45:02
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_Clear()
{ 
	XBYTE cmd[]=	{	(XBYTE)DIOSPIOLEDSCREENSSD1331_CMD_CLEARWINDOW	, 
									(XBYTE)0																				,
									(XBYTE)0																				,
									(XBYTE)(width-1)																,
									(XBYTE)(height-1)
								};

	WriteCommand(cmd, sizeof(cmd));
		
	return true;
}





/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_MaxWindow
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			05/12/2016 22:36:48
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_MaxWindow()
{    
	XBYTE cmd[]= {  DIOSPIOLEDSCREENSSD1331_CMD_SETCOLUMNADDRESS, 0x00, (XBYTE)(width-1),  DIOSPIOLEDSCREENSSD1331_CMD_SETROWADDRESS, 0x00, (XBYTE)(height-1) };
  	
	return WriteCommand(cmd, sizeof(cmd));	
}




/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::LCD_FillRect
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/12/2016 20:55:08
//	
//	@return 			bool : 
//
//  @param				x0 : 
//  @param				y0 : 
//  @param				x1 : 
//  @param				y1 : 
//  @param				color : 
//  @param				background : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::LCD_FillRect(XBYTE x0, XBYTE y0, XBYTE x1, XBYTE y1, XWORD color, XWORD background)
{	
	//---------------------------------------------------------------

	XBYTE cmd[]			= { (XBYTE)DIOSPIOLEDSCREENSSD1331_CMD_DRAWRECT	, 
											(XBYTE)(x0 & 0xFF)													, 
											(XBYTE)(y0 & 0xFF)													, 
											(XBYTE)(x1 & 0xFF)													, 
											(XBYTE)(y1 & 0xFF)
										};

	if(!WriteCommand(cmd, sizeof(cmd)))		return false;

	//---------------------------------------------------------------

	XBYTE cmd1[]		= {	(XBYTE)((color >> 11) << 1)									, 
											(XBYTE)((color >> 5) & 0x3F)								, 
											(XBYTE)((color << 1) & 0x3F)
										};

	if(!WriteCommand(cmd1, sizeof(cmd1))) return false;

	//---------------------------------------------------------------

	XBYTE cmd2[]		= { (XBYTE)((background >> 11) << 1)						, 
											(XBYTE)((background >> 5) & 0x3F)						, 
											(XBYTE)((background << 1) & 0x3F)
										};

	if(!WriteCommand(cmd2, sizeof(cmd2))) return false;

	//---------------------------------------------------------------

	return true;
}




/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::WriteCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/12/2016 13:49:28
//	
//	@return 			bool : 
//
//  @param				command : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::WriteCommand(XBYTE command, int timeout)
{
	bool status = false;

	if(diogpio) diogpio->Set(DCpin, false, DCpinsgroup);  

	if(diostream) status = diostream->Write(&command, 1)?true:false;
	if(status) status = diostream->WaitToFlushOutXBuffer(timeout);    

	return status;
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::WriteCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/12/2016 13:49:33
//	
//	@return 			bool : 
//
//  @param				command : 
//  @param				size : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::WriteCommand(XBYTE* command, XDWORD size, int timeout)
{	
	bool status = false;

	if(diogpio) diogpio->Set(DCpin, false, DCpinsgroup);  

	if(diostream) status = diostream->Write(command, size)?true:false;
	if(status) status = diostream->WaitToFlushOutXBuffer();    

	return status;
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::WriteData
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/12/2016 12:46:01
//	
//	@return 			bool : 
//
//  @param				data : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::WriteData(XBYTE data, int timeout)
{
  bool status = false;

	if(diogpio) diogpio->Set(DCpin, true, DCpinsgroup);  

	if(diostream) status = diostream->Write(&data, 1)?true:false;
	if(status) status = diostream->WaitToFlushOutXBuffer(timeout);    

	return status;
}



/*-------------------------------------------------------------------
//	DIOSPIOLEDSCREENSSD1331::WriteData
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/12/2016 21:39:30
//	
//	@return 			bool : 
//
//  @param				data : 
//  @param				size : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIOLEDSCREENSSD1331::WriteData(XBYTE* data, XDWORD size, int timeout)
{	
	bool status = false;

	if(diogpio) diogpio->Set(DCpin, true, DCpinsgroup);  

	if(diostream) status = diostream->Write(data, size)?true:false;
	if(status) status = diostream->WaitToFlushOutXBuffer();    

	return status;
}