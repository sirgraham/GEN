/*------------------------------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX.CPP
//
//  LINUX DIO port data GUMSTIX CONNEX class
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

#include "XDebugTrace.h"

#include "DIOLINUXGPIOGumstixConnex.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::DIOLINUXGPIOGUMSTIXCONNEX
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
DIOLINUXGPIOGUMSTIXCONNEX::DIOLINUXGPIOGUMSTIXCONNEX() : DIOGPIO()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::~DIOLINUXGPIOGUMSTIXCONNEX
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
DIOLINUXGPIOGUMSTIXCONNEX::~DIOLINUXGPIOGUMSTIXCONNEX()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::Ini
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
bool DIOLINUXGPIOGUMSTIXCONNEX::Ini(XPATH* xpath)
{
  /* 0*/ GPIOvalue[DIOGPIO_PINSGROUP_A][0] = 60;
  /* 1*/ GPIOvalue[DIOGPIO_PINSGROUP_A][1] = 61;
  /* 2*/ GPIOvalue[DIOGPIO_PINSGROUP_A][2] = 62;
  /* 3*/ GPIOvalue[DIOGPIO_PINSGROUP_A][3] = 63;

  return GSTX_Ini();
}




/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::SetMode
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
bool DIOLINUXGPIOGUMSTIXCONNEX::SetMode(XBYTE pin,bool isinput, DIOGPIO_PINSGROUP group)
{
  GSTX_GPIO_Function(GPIOvalue[DIOGPIO_PINSGROUP_A][pin] , GSTX_GPIO);
  GSTX_GPIO_Direction(GPIOvalue[DIOGPIO_PINSGROUP_A][pin], isinput?GSTX_IN:GSTX_OUT);

  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::Get
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
bool DIOLINUXGPIOGUMSTIXCONNEX::Get(XBYTE pin, DIOGPIO_PINSGROUP group)
{
  return GSTX_GPIO_Status(GPIOvalue[DIOGPIO_PINSGROUP_A][pin])?true:false;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::Set
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
bool DIOLINUXGPIOGUMSTIXCONNEX::Set(XBYTE pin, bool active, DIOGPIO_PINSGROUP group)
{
  if(active)
         GSTX_GPIO_Set(GPIOvalue[DIOGPIO_PINSGROUP_A][pin]);
    else GSTX_GPIO_Clear(GPIOvalue[DIOGPIO_PINSGROUP_A][pin]);

  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::End
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
bool DIOLINUXGPIOGUMSTIXCONNEX::End()
{
  return GSTX_End();
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/05/2008 10:51:21
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOGUMSTIXCONNEX::GSTX_Ini()
{
  GSTX_fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (GSTX_fd<0) return false;

  GSTX_map = mmap(0,
                  GSTX_MAP_SIZE,
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED,
                  GSTX_fd,
                  #ifdef __ARM_EABI__
                  0x40E00000 / GSTX_MAP_SIZE
                  #else
                  0x40E00000 & ~GSTX_MAP_MASK
                  #endif
                 );
  if (GSTX_map == (void*)-1 ) return false;

  /*
  GSTX_GPIO_Function(60, GSTX_GPIO);  GSTX_GPIO_Direction(60, GSTX_OUT);
  GSTX_GPIO_Function(61, GSTX_GPIO);  GSTX_GPIO_Direction(61, GSTX_OUT);
  GSTX_GPIO_Function(62, GSTX_GPIO);  GSTX_GPIO_Direction(62, GSTX_OUT);
  GSTX_GPIO_Function(63, GSTX_GPIO);  GSTX_GPIO_Direction(63, GSTX_OUT);
  */

  return true;
}


/*-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_End
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/05/2008 10:51:18
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOGUMSTIXCONNEX::GSTX_End()
{
  munmap(0,GSTX_MAP_SIZE);

  return true;
}


//-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_PutMem
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2007 23:35:55
//
//  @return       void :
//  @param        addr :
//  @param        val :
*/
//-------------------------------------------------------------------
void DIOLINUXGPIOGUMSTIXCONNEX::GSTX_PutMem(XDWORD addr, XDWORD val)
{
  GSTX_regaddr = (XBYTE*)GSTX_map + (addr & GSTX_MAP_MASK);
  *(XDWORD*) GSTX_regaddr = val;
}


//-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GetMem
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2007 23:36:08
//
//  @return       int :
//  @param        addr :
*/
//-------------------------------------------------------------------
int DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GetMem(XDWORD addr)
{
  XDWORD val = 0;

  GSTX_regaddr = (XBYTE*)GSTX_map + (addr & GSTX_MAP_MASK);
  val = *(XDWORD*) GSTX_regaddr;

  return val;
}


//-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Set
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2007 23:36:11
//
//  @return       void :
//  @param        gpio :
*/
//-------------------------------------------------------------------
void DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Set(XDWORD gpio)
{
  XDWORD pos;
  XDWORD bit = 1;

  pos = gpio / 32;
  bit <<= gpio % 32;

  GSTX_PutMem(GSTX_GPSR0 + (pos * 4), bit);
}


//-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Clear
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2007 23:36:20
//
//  @return       void :
//  @param        gpio :
*/
//-------------------------------------------------------------------
void DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Clear(XDWORD gpio)
{
  XDWORD pos;
  XDWORD bit = 1;

  pos = gpio / 32;
  bit <<= gpio % 32;

  GSTX_PutMem(GSTX_GPCR0 + (pos * 4), bit);
}


//-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Status
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2007 23:36:25
//
//  @return       XDWORD :
//  @param        gpio :
*/
//-------------------------------------------------------------------
XDWORD DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Status(XDWORD gpio)
{
  XDWORD pos;
  XDWORD bit = 1;
  XDWORD data;

  pos = gpio / 32;
  bit <<= gpio % 32;

  data = GSTX_GetMem(GSTX_GPLR0 + (pos * 4));

  data &= bit;
  if (data == 0) return 0; else return 1;

  return 0;
}


//-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Direction
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2007 23:36:29
//
//  @return       void :
//  @param        gpio :
//  @param        dir :
*/
//-------------------------------------------------------------------
void DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Direction(XDWORD gpio, XDWORD dir)
{
  XDWORD pos;
  XDWORD bit = 1;
  XDWORD data;

  pos = gpio / 32;
  bit <<= gpio % 32;

  data = GSTX_GetMem(GSTX_GPDR0 + (pos * 4));

  data &= ~bit;
  if (dir == GSTX_OUT) data |= bit;

  GSTX_PutMem(GSTX_GPDR0 + (pos * 4), data);
}


//-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Function
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2007 23:36:34
//
//  @return       void :
//  @param        gpio :
//  @param        fun :
*/
//-------------------------------------------------------------------
void DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO_Function(XDWORD gpio, XDWORD fun)
{
  XDWORD pos;
  XDWORD bit = 3;
  XDWORD data;

  pos = gpio / 16;
  bit <<= (gpio % 16) * 2;
  fun <<= (gpio % 16) * 2;

  data = GSTX_GetMem(GSTX_GAFR0_L + (pos * 4));

  data &= ~bit;
  data |= fun;

  GSTX_PutMem(GSTX_GAFR0_L + (pos * 4), data);
}



//-------------------------------------------------------------------
//  DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GPIO
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2007 23:36:41
//
//  @return       XDWORD :
//  @param        dir :
//  @param        set :
//  @param        reg :
*/
//-------------------------------------------------------------------
XDWORD DIOLINUXGPIOGUMSTIXCONNEX::GSTX_GP_IO(XDWORD dir, XDWORD set, XDWORD reg)
{
  if ((dir != GSTX_IN)  & (dir != GSTX_OUT))   return 1;
  if ((set != GSTX_SET) & (set != GSTX_CLEAR)) return 1;

  if (reg > 84) return 1;

  GSTX_GPIO_Function(reg, GSTX_GPIO);
  GSTX_GPIO_Direction(reg, dir);

  if(dir == GSTX_OUT)
    {
      if (set == GSTX_SET)
              GSTX_GPIO_Set(reg);
        else  GSTX_GPIO_Clear(reg);
    }

  return 0;
}

