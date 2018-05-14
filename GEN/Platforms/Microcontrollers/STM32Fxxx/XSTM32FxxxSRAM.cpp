//------------------------------------------------------------------------------------------
//  XSTM32FXXXSRAM.CPP
//
//  STM32Fxxx SRAM class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XSTM32FxxxSRAM.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XSTM32FXXXSRAM::XSTM32FXXXSRAM
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/04/2002 14:54:45
//
//  @return
//  */
//-------------------------------------------------------------------
XSTM32FXXXSRAM::XSTM32FXXXSRAM()
{

}



/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::~XSTM32FXXXSRAM
*/
/**
//
//   Class Destructor XSTM32FXXXSRAM
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 6:56:26
//
*/
/*-----------------------------------------------------------------*/
XSTM32FXXXSRAM::~XSTM32FXXXSRAM()
{

}



/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 15:53:58
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Ini()
{
  __PWR_CLK_ENABLE();

  __BKPSRAM_CLK_ENABLE();

  HAL_PWR_EnableBkUpAccess();

  HAL_PWR_EnableBkUpReg();

  return true;
}


/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 15:54:22
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::End()
{
  //HAL_PWR_DisableBkUpReg();

  __BKPSRAM_CLK_DISABLE();

  __HAL_RCC_PWR_CLK_DISABLE();

  return true;
}




/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:42:56
//
//  @return       bool :
//
//  @param        offset :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Read(XDWORD offset, XBYTE& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XBYTE));
}




/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:43:58
//
//  @return       bool :
//
//  @param        offset :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Read(XDWORD offset, XWORD& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XWORD));
}




/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:44:17
//
//  @return       bool :
//
//  @param        offset :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Read(XDWORD offset, XDWORD& data)
{
  return Read(offset, (XBYTE*)&data, sizeof(XDWORD));
}



/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:44:39
//
//  @return       bool :
//
//  @param        offset :
//  @param        data :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Read(XDWORD offset, XBYTE* data, XDWORD size)
{
  if((offset + size) > XSTM32FXXXSRAM_MAXSIZE) return false;

  for(int c=0; c<size; c++)
    {
      (*(data+c)) = (*(__IO uint8_t *)(XSTM32FXXXSRAM_BASE+offset+c));
    }


  return true;
}





/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:45:09
//
//  @return       bool :
//
//  @param        offset :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Write(XDWORD offset, XBYTE data)
{
  return Write(offset, (XBYTE*)&data, sizeof(XBYTE));
}



/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:45:35
//
//  @return       bool :
//
//  @param        offset :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Write(XDWORD offset, XWORD data)
{
  return Write(offset, (XBYTE*)&data, sizeof(XWORD));
}


/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:47:17
//
//  @return       bool :
//
//  @param        offset :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Write(XDWORD offset, XDWORD data)
{
  return Write(offset,(XBYTE*)&data, sizeof(XDWORD));
}




/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:47:33
//
//  @return       bool :
//
//  @param        offset :
//  @param        data :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::Write(XDWORD offset, XBYTE* data, XDWORD size)
{
  if((offset + size) > XSTM32FXXXSRAM_MAXSIZE) return false;

  for(int c=0; c<size; c++)
    {
      (*(__IO uint8_t *)(XSTM32FXXXSRAM_BASE+offset+c)) = (*(data+c));
    }


  return true;
}




/*-------------------------------------------------------------------
//  XSTM32FXXXSRAM::EraseAll
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2017 12:48:22
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XSTM32FXXXSRAM::EraseAll()
{
  for(int c=0; c<XSTM32FXXXSRAM_MAXSIZE; c++)
    {
      (*(__IO uint8_t *)(XSTM32FXXXSRAM_BASE+c))= 0;
    }
  return true;
}






