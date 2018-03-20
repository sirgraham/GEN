/*------------------------------------------------------------------------------------------
//	DIOMODBUS.H
*/	
/**	
// \class 
//   
// 	Data IO Mod Bus protocol class
//   
//	@author	 Abraham J. Velez
//	@version 04/06/2013 17:58:49
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOMODBUS_H_
#define _DIOMODBUS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBuffer.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum DIOMODBUSMODE
{	
	DIOMODBUSMODE_UNKNOWN		= 0	,
	DIOMODBUSMODE_RTU						,
	DIOMODBUSMODE_ASCII					,
	DIOMODBUSMODE_TCPIP					,
	
};


#define DIOMODBUSFUNC_READ_COILS												0x01                       
#define DIOMODBUSFUNC_READ_DISCRETE_INPUTS              0x02                       
#define DIOMODBUSFUNC_READ_HOLDING_REGISTERS            0x03                       
#define DIOMODBUSFUNC_READ_INPUT_REGISTERS							0x04                       
#define DIOMODBUSFUNC_WRITE_SINGLE_COIL                 0x05                       
#define DIOMODBUSFUNC_WRITE_SINGLE_REGISTER             0x06                       
#define DIOMODBUSFUNC_WRITE_MULTIPLE_COILS              0x0F                       
#define DIOMODBUSFUNC_WRITE_MULTIPLE_REGISTERS          0x10                       
#define DIOMODBUSFUNC_READ_FILE_RECORD									0x14                       
#define DIOMODBUSFUNC_WRITE_FILE_RECORD									0x15                       
#define DIOMODBUSFUNC_MASK_WRITE_REGISTER								0x16                       
#define DIOMODBUSFUNC_READWRITE_MULTIPLE_REGISTERS			0x17                       
#define DIOMODBUSFUNC_READ_FIFO_QUEUE										0x18                       
#define DIOMODBUSFUNC_ENCAPSULATED_INTERFACE_TRANSPORT	0x2B                       



#define DIOMODBUSMODE_STANDARDPORT   502
	
/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class DIOSTREAM;
class XPUBLISHER;

class DIOMODBUS 
{
	public:
															DIOMODBUS								(DIOSTREAM* diostream);
		virtual									 ~DIOMODBUS								();	

		DIOSTREAM*								GetDIOStream						();

		DIOMODBUSMODE							GetMode									();
		bool											SetMode									(DIOMODBUSMODE mode);

		bool											Connect									(XBYTE unit, int timeout);

		bool											ReadHoldingRegisters		(XWORD address,XWORD nregisters, XBUFFER& answer, int timeout);

		bool											Disconnect							();

	private:
		
		void											Clean										()
															{
																diostream		= NULL;	
	
																mode				= DIOMODBUSMODE_UNKNOWN;
																unit				= 0;

																xtimer			= NULL;
															}

		XWORD											CRC16										(XBUFFER& data);	

		DIOSTREAM*								diostream;
		
		DIOMODBUSMODE							mode;
		XBYTE                     unit;

		XTIMER*										xtimer;	
		
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

