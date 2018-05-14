/*------------------------------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A.CPP
//
//  LINUX DIO port data class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 25/07/2009 07:14:17 p.m.
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "XDebug.h"

#include "DIOLINUXGPIOLEX3V700A.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::DIOLINUXGPIOLEX3V700A
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2009 07:18:20 p.m.
//
//  @return
*/
/*-----------------------------------------------------------------*/
DIOLINUXGPIOLEX3V700A::DIOLINUXGPIOLEX3V700A() : DIOGPIO()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::~DIOLINUXGPIOLEX3V700A
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2009 07:18:44 p.m.
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOLINUXGPIOLEX3V700A::~DIOLINUXGPIOLEX3V700A()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      22/10/2015 14:25:06
//
//  @return       bool :
//
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOLEX3V700A::Ini(XPATH* xpath)
{
  return F75111_Ini();
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::SetMode
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2016 16:22:26
//
//  @return       bool :
//
//  @param        pin :
//  @param        isinput :
//  @param        group :
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOLEX3V700A::SetMode(XBYTE pin,bool isinput, DIOGPIO_PINSGROUP group)
{
  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2016 16:22:47
//
//  @return       bool :
//
//  @param        pin :
//  @param        group :
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOLEX3V700A::Get(XBYTE pin, DIOGPIO_PINSGROUP group)
{
  return false;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::Set
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2016 16:23:17
//
//  @return       bool :
//
//  @param        pin :
//  @param        active :
//  @param        group :
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOLEX3V700A::Set(XBYTE pin, bool active, DIOGPIO_PINSGROUP group)
{
  F75111_outputvalue = (active?F75111_outputvalue&~((XBYTE)(0x01<<GPIOvalue[DIOGPIO_PINSGROUP_A][pin])):F75111_outputvalue|((XBYTE)(0x01<<GPIOvalue[DIOGPIO_PINSGROUP_A][pin])));
  F75111_DIOOutput(F75111_outputvalue);

  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::End
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2009 08:24:53 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOLEX3V700A::End()
{
  return true;
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::InitF75111
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 07:35:35 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOLEX3V700A::F75111_Ini()
{
  F75111_mapioaddress = 0x500;
  F75111_outputvalue  = 0x00;

  // Set GPIO1X to Input function
  F75111_SMBusWriteByte(F75111_INTERNAL_ADDR,F75111_GPIO1X_CONTROL_MODE , 0x00);

  // Set GPIO3X to Input function
  F75111_SMBusWriteByte(F75111_INTERNAL_ADDR,F75111_GPIO3X_CONTROL_MODE , 0x00);

  // Set GPIO2X to Output function
  F75111_SMBusWriteByte(F75111_INTERNAL_ADDR,F75111_GPIO2X_CONTROL_MODE , 0xFF);

  // Enable WDT OUT function
  F75111_SMBusWriteByte(F75111_INTERNAL_ADDR,F75111_CONFIGURATION       , 0x03);

  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_DIOOutput
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 07:40:08 p.m.
//
//  @return       void :
//  @param        byteValue :
*/
/*-----------------------------------------------------------------*/
void DIOLINUXGPIOLEX3V700A::F75111_DIOOutput(XBYTE value)
{
  XBYTE data = 0;

  if(value&0x01) data|=0x01;
  if(value&0x02) data|=0x02;
  if(value&0x04) data|=0x04;
  if(value&0x08) data|=0x80;

  if(value&0x10) data|=0x10;
  if(value&0x20) data|=0x20;
  if(value&0x40) data|=0x40;
  if(value&0x08) data|=0x08;

  // write data value via GPIO2X output pin
  F75111_SMBusWriteByte(F75111_INTERNAL_ADDR,F75111_GPIO2X_OUTPUT_DATA,data);
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_DIOInput
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 07:42:18 p.m.
//
//  @return       XBYTE :
//  */
/*-----------------------------------------------------------------*/
XBYTE DIOLINUXGPIOLEX3V700A::F75111_DIOInput()
{
  XBYTE GPIO1X = 0;
  XBYTE GPIO3X = 0;
  XBYTE data    = 0;

  // Get value from GPIO1X
  F75111_SMBusReadByte(F75111_INTERNAL_ADDR,F75111_GPIO1X_INPUT_DATA,&GPIO1X) ;

  // Get value from GPIO3X
  F75111_SMBusReadByte(F75111_INTERNAL_ADDR,F75111_GPIO3X_INPUT_DATA,&GPIO3X) ;

  // Get value from GPIO3X
  GPIO1X  = GPIO1X & 0xF0; // Mask unuseful value
  GPIO3X  = GPIO3X & 0x0F; // Mask unuseful value

  data = ( GPIO1X & 0x10 )? data + 0x01 : data;
  data = ( GPIO1X & 0x80 )? data + 0x02 : data;
  data = ( GPIO1X & 0x40 )? data + 0x04 : data;
  data = ( GPIO3X & 0x01 )? data + 0x08 : data;
  data = ( GPIO3X & 0x02 )? data + 0x10 : data;
  data = ( GPIO3X & 0x04 )? data + 0x20 : data;
  data = ( GPIO3X & 0x08 )? data + 0x40 : data;
  data = ( GPIO1X & 0x20 )? data + 0x80 : data;

  // Get correct DI value from GPIO1X & GPIO3X

  return data;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SetWDTEnable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 07:44:42 p.m.
//
//  @return       void :
//  @param        timer :
*/
/*-----------------------------------------------------------------*/
void DIOLINUXGPIOLEX3V700A::F75111_SetWDTEnable(XBYTE timer)
{
  // set WatchDog range and timer
  F75111_SMBusWriteByte(F75111_INTERNAL_ADDR,F75111_WDT_TIMER_RANGE ,timer);

  // Enable WatchDog, Setting WatchDog configure
  F75111_SMBusWriteByte(F75111_INTERNAL_ADDR,F75111_WDT_CONFIGURATION, F75111_WDT_TIMEOUT_FLAG | F75111_WDT_ENABLE | F75111_WDT_PULSE | F75111_WDT_PSWIDTH_100MS);
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SetWDTDisable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 07:45:04 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOLINUXGPIOLEX3V700A::F75111_SetWDTDisable ()
{
  F75111_SMBusWriteByte(F75111_INTERNAL_ADDR,F75111_WDT_CONFIGURATION,0x00);
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SMBusWriteByte
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 07:53:59 p.m.
//
//  @return       int :
//  @param        slave :
//  @param        cmd :
//  @param        byte :
*/
/*-----------------------------------------------------------------*/
int DIOLINUXGPIOLEX3V700A::F75111_SMBusWriteByte(XWORD slave, XBYTE cmd, XBYTE byte)
{
  F75111_SMBusClear();

  if(F75111_SMBusBusy()) return F75111_SMBUS_BUSY;

  F75111_SMBusIOWrite(F75111_SMBHSTADD  , slave & ~1 ); // write address in first variable
  F75111_SMBusIOWrite(F75111_SMBHSTCMD  , cmd );        // write command in second variable
  F75111_SMBusIOWrite(F75111_SMBHSTDAT0 , byte );     // write data in third variable
  F75111_SMBusIOWrite(F75111_SMBHSTCNT  , F75111_SMBHSTCNT_START | F75111_SMBHSTCNT_BYTE);

  // Sent start command to SMBus control register

  return (int)F75111_SMBusWait(); // return wait command when SMBus finish the job
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SMBusReadByte
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 08:27:47 p.m.
//
//  @return       int :
//  @param        slave :
//  @param        cmd :
//  @param        byte :
*/
/*-----------------------------------------------------------------*/
int DIOLINUXGPIOLEX3V700A::F75111_SMBusReadByte(XWORD slave, XBYTE cmd, XBYTE* byte)
{
  F75111_SMBusClear();
  if(F75111_SMBusBusy())  return F75111_SMBUS_BUSY;

  F75111_SMBusIOWrite(F75111_SMBHSTADD , slave | 1 ); // write address in first variable
  F75111_SMBusIOWrite(F75111_SMBHSTCMD , cmd );       // write command in second variable
  F75111_SMBusIOWrite(F75111_SMBHSTCNT , F75111_SMBHSTCNT_START | F75111_SMBHSTCNT_BYTE);

  // Sent start command to SMBus control register

  int ret = F75111_SMBusWait(); // Check SMBus Status
  if(ret == F75111_SMBUS_OK)  (*byte) = (XBYTE)F75111_SMBusIORead(F75111_SMBHSTDAT0) &0xFF; // Get SMBus host data value

  return ret; // reture SMBus status
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SMBusCheckDevice
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 08:29:32 p.m.
//
//  @return       bool :
//  @param        deviceaddress :
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOLEX3V700A::F75111_SMBusCheckDevice(XWORD deviceaddress)
{
  int ret;

  F75111_SMBusClear(); // Clear SMBus data first
  if(F75111_SMBusBusy()) return F75111_SMBUS_BUSY;

  F75111_SMBusIOWrite(F75111_SMBHSTADD , deviceaddress & ~1 ); // write address in first variable
  F75111_SMBusIOWrite(F75111_SMBHSTCNT , F75111_SMBHSTCNT_START | F75111_SMBHSTCNT_SENDRECV);

  // Sent start command to SMBus control register
  ret = F75111_SMBusWait(); // Check SMBus Status

  if(ret == F75111_SMBUS_OK) // Check device exist or not, if exist return ture else false
         return true;
    else return false;
}


/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SMBusClear
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 08:30:35 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOLINUXGPIOLEX3V700A::F75111_SMBusClear()
{
  F75111_SMBusIOWrite(F75111_SMBHSTSTS ,0xFF); // Clear SMBus status
  F75111_SMBusIOWrite(F75111_SMBHSTDAT0,0x0 ); // Clear SMBus data
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SMBusWait
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 08:30:55 p.m.
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOLINUXGPIOLEX3V700A::F75111_SMBusWait()
{
  int timeout = F75111_SMBUS_TIMEOUT;
  XDWORD value;

  while(timeout--)
    {
      usleep(1000); // I/O Delay

      value = F75111_SMBusIORead(F75111_SMBHSTSTS) & 0x00FF; // Read Host Status Register

      // if status value equal SMBHSTSTS_INTR, return SMBus_OK
      if(value & F75111_SMBHSTSTS_INTR)    return F75111_SMBUS_OK;

      // if status value equal SMBHSTSTS_FAILED, return SMBHSTSTS_FAILED
      if(value & F75111_SMBHSTSTS_FAILED) return F75111_SMBHSTSTS_FAILED;

      // if status value equal SMBHSTSTS_COLLISION, return SMBHSTSTS_COLLISION
      if(value & F75111_SMBHSTSTS_COLLISION) return F75111_SMBHSTSTS_COLLISION;

      // if status value equal SMBHSTSTS_ERROR, return SMBHSTSTS_ERROR
      if(value & F75111_SMBHSTSTS_ERROR) return F75111_SMBHSTSTS_ERROR;
    }

  return F75111_SMBUS_BUSY;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SMBusBusy
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 08:34:04 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOLEX3V700A::F75111_SMBusBusy()
{
  // Check SMBus status if equal SMBHSTSTS_BUSY
  if((F75111_SMBusIORead(F75111_SMBHSTSTS) & F75111_SMBHSTSTS_BUSY ) == 1)
          return true;
    else  return false;
}


/*-------------------------------------------------------------------
//   DIOLINUXGPIOLEX3V700A::F75111_SMBusIOWrite
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 08:34:32 p.m.
//
//  @return       void :
//  @param        offset :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOLINUXGPIOLEX3V700A::F75111_SMBusIOWrite(XWORD offset,XBYTE data)
{
  PC_Set(F75111_mapioaddress + offset,data);
}


/*-------------------------------------------------------------------
//  DIOLINUXGPIOLEX3V700A::F75111_SMBusIORead
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/07/2009 08:35:22 p.m.
//
//  @return       XBYTE :
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
XBYTE DIOLINUXGPIOLEX3V700A::F75111_SMBusIORead(XWORD offset)
{
  XBYTE data = 0;

  PC_Get(F75111_mapioaddress + offset,data);

  return data;
}

