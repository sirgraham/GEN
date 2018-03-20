/*------------------------------------------------------------------------------------------
//	DIOMODBUS.CPP
//	
//	Data IO Mod Bus protocol class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/06/2013 17:56:14
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "DIOStream.h"

#include "DIOModBus.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOMODBUS::DIOMODBUS
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/06/2013 22:09:58
//	
//	@return 			
//  @param				diostream : 
//  @param				xpublisher : 
*/
/*-----------------------------------------------------------------*/
DIOMODBUS::DIOMODBUS(DIOSTREAM* diostream)
{
	Clean();

	this->diostream		= diostream;
	; 

	xtimer = xfactory->CreateTimer();
}




/*-------------------------------------------------------------------
//  DIOMODBUS::~DIOMODBUS
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/06/2013 18:26:57
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOMODBUS::~DIOMODBUS()
{
	Disconnect();

	xfactory->DeleteTimer(xtimer);

	Clean();
}




/*-------------------------------------------------------------------
//  DIOMODBUS::GetDIOStream
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/06/2013 18:33:06
//	
//	@return 			DIOSTREAM* : 
//	*/
/*-----------------------------------------------------------------*/
DIOSTREAM* DIOMODBUS::GetDIOStream()
{
	return diostream;
}




/*-------------------------------------------------------------------
//  DIOMODBUS::GetMode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/06/2013 19:11:50
//	
//	@return 			DIOMODBUSMODE : 
//	*/
/*-----------------------------------------------------------------*/
DIOMODBUSMODE DIOMODBUS::GetMode()
{
	return mode;
}




/*-------------------------------------------------------------------
//  DIOMODBUS::SetMode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/06/2013 19:12:07
//	
//	@return 			bool : 
//	@param				mode : 
*/
/*-----------------------------------------------------------------*/
bool DIOMODBUS::SetMode(DIOMODBUSMODE mode)
{
	this->mode = mode;

	return true;
}





/*-------------------------------------------------------------------
//  DIOMODBUS::Connect
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/06/2013 0:11:10
//	
//	@return 			bool : 
//	@param				unit : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOMODBUS::Connect(XBYTE unit, int timeout)
{
	if(!diostream)																									return false;
	if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED) return false;

	this->unit = unit;

  if(!diostream->Open())  return false;

	return diostream->WaitToConnected(timeout);
}





/*-------------------------------------------------------------------
//  DIOMODBUS::ReadHoldingRegisters
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/06/2013 19:37:07
//	
//	@return 			bool : 
//	@param				address : 
//  @param				nregisters : 
//  @param				answer : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
/*
bool DIOMODBUS::ReadHoldingRegisters(XWORD address, XWORD nregisters, XBUFFER& answer, int timeout)
{
	if(!diostream)																								 return false;
	if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

	answer.Delete();

	switch(mode)
		{
			case DIOMODBUSMODE_UNKNOWN		: return false;

			case DIOMODBUSMODE_RTU			:
			case DIOMODBUSMODE_ASCII		: return false; //Not Implemented yet!!!

			case DIOMODBUSMODE_TCPIP		: {	XBUFFER datawrite();

																			datawrite.Add((XWORD) 0x0000);
																			datawrite.Add((XWORD) 0x0000);
																			datawrite.Add((XWORD) 0x0006);
																			datawrite.Add((XBYTE) unit);
																			datawrite.Add((XBYTE) DIOMODBUSFUNC_READ_HOLDING_REGISTERS);
																			datawrite.Add((XWORD) address);
																			datawrite.Add((XWORD) nregisters);

																			diostream->Write(datawrite);
																			
																			if(!diostream->WaitToFlushOutXBuffer(timeout)) return false;
																
																			xtimer->Reset();

																			int size = (nregisters*sizeof(XWORD)) + 9;

																			while(diostream->GetInXBuffer()->GetSize() != size)
																				{
																					if(xtimer->GetMeasureSec() >= (XDWORD)timeout) 
																						{
																							diostream->GetInXBuffer()->Delete();
																							return false;	
																						}

																					xsleep->MilliSec(10);
																				}	
	
																			answer.Resize(size);
																			answer.FillBuffer(0x00);

																			if(!diostream->Read(answer.Get(), size)) return false;

																			if(answer.GetSize() != size) return false;

																			if(answer.Get()[7] & 0x80)   return false;
																			
																			XBYTE head[10];
																			answer.Extract(head, 0 , 9);
																		}
																		break;
		}

	return true;
}
*/

bool DIOMODBUS::ReadHoldingRegisters(XWORD address, XWORD nregisters, XBUFFER& answer, int timeout)
{
	if(!diostream)																								 return false;
	if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

	answer.Delete();

	switch(mode)
		{
			case DIOMODBUSMODE_UNKNOWN		: return false;

			case DIOMODBUSMODE_RTU			:
			case DIOMODBUSMODE_ASCII		: return false; //Not Implemented yet!!!

			case DIOMODBUSMODE_TCPIP		: {	XBUFFER datawrite;

																			datawrite.Add((XWORD) 0x0000);
																			datawrite.Add((XWORD) 0x0000);
																			datawrite.Add((XWORD) 0x0006);
																			datawrite.Add((XBYTE) unit);
																			datawrite.Add((XBYTE) DIOMODBUSFUNC_READ_HOLDING_REGISTERS);
																			datawrite.Add((XWORD) address);
																			datawrite.Add((XWORD) nregisters);

																			diostream->Write(datawrite);
																			
																			if(!diostream->WaitToFlushOutXBuffer(timeout)) return false;
																
																			xtimer->Reset();

																			int size = nregisters*sizeof(XWORD) + 9;

																			while(diostream->GetInXBuffer()->GetSize() != size)
																				{
																					if(xtimer->GetMeasureSeconds() >= (XDWORD)timeout) 
																						{
																							diostream->GetInXBuffer()->Delete();
																							return false;	
																						}

																					xsleep->MilliSeconds(10);
																				}	
	
																			answer.Resize(size);
																			answer.FillBuffer(0x00);
																			
																			if(!diostream->Read(answer.Get(), size)) return false;

																			if(answer.GetSize() != size) return false;

																			if(answer.Get()[7] & 0x80)   return false;
																			
																			XBYTE head[10];
																			answer.Extract(head, 0 , 9);
																		}
																		break;
		}

	return true;
}







/*-------------------------------------------------------------------
//  DIOMODBUS::Disconnect
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/06/2013 19:49:34
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOMODBUS::Disconnect()
{	
  if(diostream) 
		{			
			diostream->Disconnect();
			diostream->Close();

			diostream = NULL;
		}


	return true;
}



/*-------------------------------------------------------------------
//  DIOMODBUS::CRC16
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/06/2013 22:41:49
//	
//	@return 			XWORD : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
XWORD DIOMODBUS::CRC16(XBUFFER& data)
{
	XWORD poly16 = 0xA001;
	XWORD	LSB;	
	XWORD	crc		 = 0xFFFF;

	for(int d=0; d<(int)data.GetSize(); d++)
		{
			crc = ((crc^data.Get()[d]) | 0xFF00) & (crc | 0x00FF);

			for(int c=0; c<8; c++) 
				{
					LSB=(crc & 0x0001);
					crc=crc/2;

					if(LSB) crc = crc ^ poly16;
				}
		}

	return crc;
}


