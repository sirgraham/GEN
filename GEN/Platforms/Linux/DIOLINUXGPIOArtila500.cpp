/*------------------------------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500.CPP
//
//  LINUX DIO port data class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 25/07/2009 07:14:17 p.m.
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOLINUXGPIOARTILA500_H_
#define _DIOLINUXGPIOARTILA500_H_

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

#include "XDebugTrace.h"

#include "DIOLINUXGPIOArtila500.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::DIOLINUXGPIOARTILA500
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
DIOLINUXGPIOARTILA500::DIOLINUXGPIOARTILA500() : DIOGPIO()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::~DIOLINUXGPIOARTILA500
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
DIOLINUXGPIOARTILA500::~DIOLINUXGPIOARTILA500()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::Ini
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
bool DIOLINUXGPIOARTILA500::Ini(XPATH* xpath)
{
  /* 0*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 0] = DIOGPIO_PINNOTUSE;  // Not available
  /* 1*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 1] =  7;                     // DOUT1
  /* 2*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 2] =  8;                     // DOUT2
  /* 3*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 3] =  9;                     // DOUT3
  /* 4*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 4] = 10;                     // DOUT4
  /* 5*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 5] = 11;                     // DOUT5
  /* 6*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 6] =  1;                     // DIN1 / DOUT6
  /* 7*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 7] =  2;                     // DIN2 / DOUT7
  /* 8*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 8] =  3;                     // DIN3 / DOUT8
  /* 9*/ GPIOvalue[DIOGPIO_PINSGROUP_A][ 9] =  4;                     // DIN4
  /*10*/ GPIOvalue[DIOGPIO_PINSGROUP_A][10] =  5;                     // DIN5

  return ARTILA500_Ini();
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::SetMode
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
bool DIOLINUXGPIOARTILA500::SetMode(XBYTE pin,bool isinput, DIOGPIO_PINSGROUP group)
{
  if(ARTILA500_fd<0) return false;

  int data = (int)GPIOvalue[DIOGPIO_PINSGROUP_A][pin];
  ioctl(ARTILA500_fd,isinput?GPIO_IOCTL_INPUT:GPIO_IOCTL_OUTPUT,&data);

  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::Get
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
bool DIOLINUXGPIOARTILA500::Get(XBYTE pin, DIOGPIO_PINSGROUP group)
{
  if(ARTILA500_fd<0) return false;

  int data = (int)GPIOvalue[DIOGPIO_PINSGROUP_A][pin];
  return ioctl(ARTILA500_fd,GPIO_IOCTL_GET,&data)?true:false;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::Set
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
bool DIOLINUXGPIOARTILA500::Set(XBYTE pin, bool active, DIOGPIO_PINSGROUP group)
{
  if(ARTILA500_fd<0) return false;

  int data = (int)GPIOvalue[DIOGPIO_PINSGROUP_A][pin];
  ioctl(ARTILA500_fd,active?GPIO_IOCTL_SET:GPIO_IOCTL_CLEAR,&data);

  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::End
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
bool DIOLINUXGPIOARTILA500::End()
{
  return ARTILA500_End();
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::ARTILA500_Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2012 8:48:27
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOARTILA500::ARTILA500_Ini()
{
  ARTILA500_fd = open("/dev/gpio", O_RDWR);
  if(ARTILA500_fd<0) return false;

  if(ioctl(ARTILA500_fd,GPIO_IOCTL_COUNT,&ARTILA500_countIO)!=0) return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOARTILA500::ARTILA500_End
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2012 8:48:33
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOARTILA500::ARTILA500_End()
{
  if(ARTILA500_fd<0) return false;

  close(ARTILA500_fd);

  return true;
}


#endif

