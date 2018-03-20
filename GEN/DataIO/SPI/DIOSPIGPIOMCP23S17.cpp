
/*------------------------------------------------------------------------------------------
//	DIOSPIGPIOMCP23S17.CPP
//	
//	Data IO SPI MCP23S17 (Microchip) (8+8 Digital I/O) class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 03/05/2014 16:21:57
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"

#include "DIOFactory.h"
#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"
	
#include "DIOSPIGPIOMCP23S17.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	DIOSPIGPIOMCP23S17::DIOSPIGPIOMCP23S17
*/	
/**	
//	
//	Class Constructor DIOSPIGPIOMCP23S17
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:37:54
//	
 
 
//  @param				xpublisher : 
*/
/*-----------------------------------------------------------------*/
DIOSPIGPIOMCP23S17::DIOSPIGPIOMCP23S17() : DIODEVICE()
{
	Clean();

	 
	;
}




/*-------------------------------------------------------------------
//	DIOSPIGPIOMCP23S17::~DIOSPIGPIOMCP23S17
*/	
/**	
//	
//	 Class Destructor DIOSPIGPIOMCP23S17
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:25:15
//	
*/
/*-----------------------------------------------------------------*/
DIOSPIGPIOMCP23S17::~DIOSPIGPIOMCP23S17()
{
	End();
	Clean();
}




/*-------------------------------------------------------------------
//	DIOSPIGPIOMCP23S17::Ini
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/06/2014 17:04:57
//	
//	@return 			bool : 
//
//  @param				chipselect : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::Ini(int chipselect, int timeout)
{
	if(!isdiostreamSPIexternal)
		{				
			diostreamSPIcfg = new DIOSTREAMSPICONFIG();
			if(!diostreamSPIcfg)  return false;

			diostreamSPIcfg->SetSPIMode(DIOSTREAMSPI_MODE_0);	
			diostreamSPIcfg->SetSpeed(20*1000*1000);	
			diostreamSPIcfg->SetDelay(0);			
			diostreamSPIcfg->GetLocalDeviceName()->Format(__L("/dev/spidev0.%d"), chipselect);
			
			diostreamSPI = (DIOSTREAMSPI*)diofactory->CreateStreamIO(diostreamSPIcfg);					
			if(!diostreamSPI) return false;
		}
	 else
	  {
			if(!diostreamSPI)	return false;

			diostreamSPIcfg = (DIOSTREAMSPICONFIG *)diostreamSPI->GetConfig();
			if(!diostreamSPIcfg) return false;
		}	

	this->timeout = timeout;

	if(!diostreamSPI->Open())  return false;

	if(!diostreamSPI->WaitToConnected(timeout)) return false;

	if(!DIODEVICE::Ini()) return false;
		
	return Configure();
}



/*-------------------------------------------------------------------
//	DIOSPIGPIOMCP23S17::Configure
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 21:28:11
//	
//	@return 			virtual : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::Configure()
{
	if(!IsInitialized()) return false;

	SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_IOCON		, 8);			// Enable hardware addressing
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_GPIOA		, 0x00);	// Turn on port A
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_IODIRA	, 0x00);	// Set port A as an output
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_IODIRB	, 0xFF);	// Set port B as an input
  SendCommand(DIOSPIGPIOMCP23S17_WRITECMD, DIOSPIGPIOMCP23S17_GPPUB		, 0xFF);  // Turn on port B pullups

	return true;
}





/*-------------------------------------------------------------------
//	DIOSPIGPIOMCP23S17::SendCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 17:45:35
//	
//	@return 			bool : 
//
//  @param				cmd : 
//  @param				port : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::SendCommand(XBYTE cmd, XBYTE port, XBYTE value)
{
	if(!diostreamSPI)			return false;
	if(!IsInitialized())	return false;

	XBUFFER xbuffer;

	xbuffer.Add((XBYTE)cmd);
	xbuffer.Add((XBYTE)port);
	xbuffer.Add((XBYTE)value);

	bool status = diostreamSPI->Write(xbuffer)?true:false;

	if(!diostreamSPI->WaitToFlushOutXBuffer(timeout)) return false;

	return status;
}




/*-------------------------------------------------------------------
//	DIOSPIGPIOMCP23S17::End
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			03/05/2014 16:29:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSPIGPIOMCP23S17::End()
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



