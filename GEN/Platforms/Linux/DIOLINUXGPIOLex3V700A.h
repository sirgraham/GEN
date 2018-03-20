/*------------------------------------------------------------------------------------------
//	DIOLINUXGPIOLEX3V700A.H
*/	
/**	
// \class 
//   
//  LINUX DIO port data LEX3V700A class
//   
//	@author	 Abraham J. Velez
//	@version 25/07/2009 07:13:30 p.m.
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOLINUXGPIOLEX3V700A_H_
#define _DIOLINUXGPIOLEX3V700A_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOGPIO.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



// HW_LEX3V700A
//--------------------------------------------
// Lex F75111N (I2C)
//
#define F75111_INTERNAL_ADDR 						0x9C // OnBoard F75111 Chipset
#define F75111_CONFIGURATION 						0x03 // Configure GPIO13 to WDT2 Function
#define F75111_GPIO1X_CONTROL_MODE 			0x10 // Select Output Mode or Input Mode
#define F75111_GPIO2X_CONTROL_MODE 			0x20 // Select GPIO2X Output Mode or Input Mode
#define F75111_GPIO3X_CONTROL_MODE 			0x40 // Select GPIO3X Output Mode or Input Mode
#define F75111_GPIO1X_INPUT_DATA 				0x12 // GPIO1X Input
#define F75111_GPIO3X_INPUT_DATA 				0x42 // GPIO3X Input
#define F75111_GPIO2X_OUTPUT_DATA 			0x21 // GPIO2X Output
#define F75111_WDT_TIMER_RANGE 					0x37 // 0-255 (secord or minute program by WDT_UNIT)
#define F75111_WDT_CONFIGURATION 				0x36 // Configure WDT Function
#define F75111_WDT_TIMEOUT_FLAG 				0x40 // When watchdog timeout.this bit will be set to 1.
#define F75111_WDT_ENABLE 							0x20 // Enable watchdog timer
#define F75111_WDT_PULSE 								0x10 // Configure WDT output mode
#define F75111_WDT_PSWIDTH_100MS 				0x02 // When select Pulse mode: 100 ms.

#define F75111_SMBHSTSTS 								0x00 // SMBus Host Status Register Offset
#define F75111_SMBHSTSTS_BUSY 					0x01 // SMBus Host -> 0000-0001 Busy
#define F75111_SMBHSTSTS_INTR 					0x02 // SMBus Host -> 0000-0010 Interrupt / complection
#define F75111_SMBHSTSTS_ERROR 					0x04 // SMBus Host -> 0000-0100 Error
#define F75111_SMBHSTSTS_COLLISION 			0x08 // SMBus Host -> 0000-1000 Collistion
#define F75111_SMBHSTSTS_FAILED 				0x10 // SMBus Host -> 0001-0000 Failed
#define F75111_SMBHSTCNT 								0x02 // SMBus Host Contorl Register Offset
#define F75111_SMBHSTCNT_SENDRECV 			0x04 // SMBus Host Contorl -> 0000 0100 Byte
#define F75111_SMBHSTCNT_BYTE 					0x08 // SMBus Host Contorl -> 0000 1000 Byte Data
#define F75111_SMBHSTCNT_START 					0x40 // SMBus Host Contorl -> 0100 0000 Start
#define F75111_SMBHSTCMD 								0x03 // SMBus Host Command Register Offset
#define F75111_SMBHSTADD 								0x04 // SMBus Host Address Register Offset
#define F75111_SMBHSTDAT0 							0x05 // SMBus Host Data0 Register Offset
#define F75111_SMBUS_OK 								0x0  // SMBUS OK
#define F75111_SMBUS_BUSY 							0x1  // SMBUS BUSY
#define F75111_SMBUS_TIMEOUT 						100




/*---- CLASS -----------------------------------------------------------------------------*/
	
class DIOLINUXGPIOLEX3V700A : public DIOGPIO
{
	public:
													DIOLINUXGPIOLEX3V700A		();
		virtual	  				 	 ~DIOLINUXGPIOLEX3V700A		();

		bool									Ini													(XPATH* xpath = NULL);

		bool									SetMode											(XBYTE pin, bool isinput, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		bool									Get													(XBYTE pin, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		bool									Set													(XBYTE pin, bool active, DIOGPIO_PINSGROUP group = DIOGPIO_PINSGROUP_A);
		
		bool									End													();

	private:

		void									Clean												()
													{														
														F75111_mapioaddress = 0;
														F75111_outputvalue	= 0;														
													}
    
		bool									F75111_Ini									();
		void									F75111_DIOOutput						(XBYTE value);
		XBYTE									F75111_DIOInput							();
		void									F75111_SetWDTEnable					(XBYTE timer);
		void									F75111_SetWDTDisable				();

		int										F75111_SMBusWriteByte				(XWORD slave, XBYTE cmd, XBYTE byte);
		int										F75111_SMBusReadByte				(XWORD slave, XBYTE cmd, XBYTE* byte);
		bool									F75111_SMBusCheckDevice			(XWORD deviceaddress);
		void									F75111_SMBusClear						();
		int										F75111_SMBusWait						();
		bool									F75111_SMBusBusy						();
		void									F75111_SMBusIOWrite					(XWORD offset,XBYTE data);
		XBYTE									F75111_SMBusIORead					(XWORD offset);

		XDWORD								F75111_mapioaddress;
		XBYTE									F75111_outputvalue;
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

