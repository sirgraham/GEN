//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMSPI.CPP
//
//  Data IO Stream SPI class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


#if defined(DIO_ACTIVE) && defined(DIOSPI_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "XBuffer.h"
#include "XDebug.h"
#include "DIOStreamSPIConfig.h"

#include "DIOLINUXStreamSPI.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOLINUXSTREAMSPI::DIOLINUXSTREAMSPI
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2013 23:14:54
//
//  @return




*/
/*-----------------------------------------------------------------*/
DIOLINUXSTREAMSPI::DIOLINUXSTREAMSPI() : DIOSTREAMSPI()
{
  Clean();
}


//-------------------------------------------------------------------
//  DIOLINUXSTREAMSPI::~DIOLINUXSTREAMSPI
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:19:33
//
//  @return
//  */
//-------------------------------------------------------------------
DIOLINUXSTREAMSPI::~DIOLINUXSTREAMSPI()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMSPI::Open
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2012 15:33:40
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXSTREAMSPI::Open()
{
  if(!config) return false;

  //XDEBUG_PRINTCOLOR(1, __L("open: %s"), config->GetLocalDeviceName()->Get());

  XSTRING_CREATEOEM((*config->GetLocalDeviceName()), charOEM)
  handle = open(charOEM, O_RDWR);
  XSTRING_DELETEOEM(charOEM)

  if(handle<0) return false;

  //  Set SPI parameters.
  //  Why are we reading it afterwriting it? I've no idea, but for now I'm blindly
  //  copying example code I've seen online...

  XBYTE   _mode  = config->GetSPIMode();
  XBYTE   _bits  = config->GetNBitsWord();
  XDWORD  _speed = config->GetSpeed();

  if (ioctl(handle, SPI_IOC_WR_MODE           , &_mode)   < 0) return false;
  if (ioctl(handle, SPI_IOC_WR_BITS_PER_WORD  , &_bits)   < 0) return false;
  if (ioctl(handle, SPI_IOC_WR_MAX_SPEED_HZ   , &_speed)  < 0) return false;

  if (ioctl(handle, SPI_IOC_RD_MODE           , &_mode)   < 0) return false;
  if (ioctl(handle, SPI_IOC_RD_BITS_PER_WORD  , &_bits)   < 0) return false;
  if (ioctl(handle, SPI_IOC_RD_MAX_SPEED_HZ   , &_speed)  < 0) return false;

  return DIOSTREAMSPI::Open();
}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMSPI::Close
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2012 15:33:45
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXSTREAMSPI::Close()
{
  bool status = DIOSTREAMSPI::Close();

  if(handle>=0)
    {
      close(handle);
      handle  = -1;
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOLINUXSTREAMSPI::TransferBuffer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/12/2012 0:53:13
//
//  @return       bool :
//  @param        bufferread :
//  @param        bufferwrite :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXSTREAMSPI::TransferBuffer(XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size)
{
  struct spi_ioc_transfer transf;

  int sizeint = size;

  memset((XBYTE*)&transf, 0, sizeof(struct spi_ioc_transfer));

  //XDEBUG_PRINTCOLOR(1, __L("real SPI %d"), size);

  //wprintf((wchar_t *)__L("\n Write %4d bytes ->"),size);
  //fflush(stdout);

  transf.tx_buf         = (unsigned long)bufferwrite;
  transf.rx_buf         = (unsigned long)bufferread;
  transf.len            = sizeint;
  transf.speed_hz       = config->GetSpeed();
  transf.delay_usecs    = config->GetDelay();
  transf.bits_per_word  = config->GetNBitsWord();
  transf.cs_change      = true;

  int err = ioctl(handle, SPI_IOC_MESSAGE(1), &transf);

  //wprintf((wchar_t *)__L("%4d %d\n"), transf.len, err);
  //fflush(stdout);

  if(err<0) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOLINUXSTREAMSPI::Sleep
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/02/2013 19:09:26
//
//  @return       void :
//  @param        count :
*/
/*-----------------------------------------------------------------*/
void DIOLINUXSTREAMSPI::Sleep(int count)
{
  for(int i=0;i<count;i++)
    {
      asm("nop");
    }

}




//-------------------------------------------------------------------
//  DIOLINUXSTREAMSPI::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:19:50
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOLINUXSTREAMSPI::Clean()
{
  handle = -1;
}


#endif