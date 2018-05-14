/*------------------------------------------------------------------------------------------
//  DIOWINDOWSGPIO.CPP
//
//  WINDOWS DIO port class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 25/07/2009 07:14:17 p.m.
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOWINDOWSGPIO.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::DIOWINDOWSGPIO
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
DIOWINDOWSGPIO::DIOWINDOWSGPIO() : DIOGPIO()
{

}


/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::~DIOWINDOWSGPIO
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
DIOWINDOWSGPIO::~DIOWINDOWSGPIO()
{

}


/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      22/10/2015 14:24:25
//
//  @return       bool :
//
//  @param        hardwaretype :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSGPIO::Ini(XPATH* xpath)
{
  #ifdef HW_PC

  XPATH xpathDLL;

  for(int c=0; c<2; c++)
    {
      if(xpath)
        {
          xpathDLL.Add((*xpath));
          xpathDLL.Slash_Add();
        }

      switch(c)
        {
          case 0: xpathDLL.Add("inpout32.dll"); break;
          case 1: xpathDLL.Add("inpout64.dll"); break;
        }

      handleDLL = LoadLibrary(xpathDLL.Get());
      if(handleDLL) break;
    }

  if(!handleDLL) return false;

  OutPort       = (DIOWGPIOOUT)             GetProcAddress(handleDLL, "Out32");
  InpPort       = (DIOWGPIOINP)             GetProcAddress(handleDLL, "Inp32");;
  IsDriverOpen  = (DIOWGPIOISDRIVEROPEN)    GetProcAddress(handleDLL, "IsInpOutDriverOpen");
  IsDriver64Bit = (DIOWGPIOISDRIVER64BITS)  GetProcAddress(handleDLL, "IsXP64Bit");

  if((!OutPort) || (!InpPort) || (!IsDriverOpen) || (!IsDriver64Bit))
    {
      FreeLibrary(handleDLL);
      return false;
    }

  #endif

  return true;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::SetMode
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2016 16:25:08
//
//  @return       bool :
//
//  @param        pin :
//  @param        isinput :
//  @param        group :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSGPIO::SetMode(XBYTE pin,bool isinput, DIOGPIO_PINSGROUP group)
{
  #ifdef HW_PC
  return true;
  #endif

  return false;
}


/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2016 16:24:52
//
//  @return       bool :
//
//  @param        pin :
//  @param        group :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSGPIO::Get(XBYTE pin, DIOGPIO_PINSGROUP group)
{

  #ifdef HW_PC
  PC_Get(PC_PORT,dataport);
  return (dataport&~(0x01<<pin))?true:false;
  #endif

  return false;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::Set
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2016 16:25:44
//
//  @return       bool :
//
//  @param        pin :
//  @param        active :
//  @param        group :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSGPIO::Set(XBYTE pin,bool active, DIOGPIO_PINSGROUP group)
{
  #ifdef HW_PC
  PC_Get(PC_PORT,dataport);
  PC_Set(PC_PORT,active?(dataport|(0x01<<pin)):(dataport&~(0x01<<pin)));
  return true;
  #endif

  return false;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::End
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
bool DIOWINDOWSGPIO::End()
{
  #ifdef HW_PC
  if(!handleDLL) return false;
  FreeLibrary(handleDLL);
  return true;
  #endif

  return false;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2009 07:19:12 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWINDOWSGPIO::Clean()
{
  #ifdef HW_PC

  handleDLL       = NULL;

  OutPort         = NULL;
  InpPort         = NULL;

  IsDriverOpen    = NULL;
  IsDriver64Bit   = NULL;

  #endif
}






#ifdef HW_PC
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------
//
// PC
//
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::PC_Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/08/2009 12:42:40 a.m.
//
//  @return       bool :
//  @param        nport :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSGPIO::PC_Get(XDWORD nport, XBYTE& data)
{
  if(!handleDLL)      return false;
  if(!IsDriverOpen()) return false;

  WORD _data = InpPort(nport);
  data = (XBYTE)(_data & 0x00FF);

  return true;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSGPIO::PC_Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/08/2009 12:42:47 a.m.
//
//  @return       bool :
//  @param        nport :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSGPIO::PC_Set(XDWORD nport, XBYTE data)
{
  if(!handleDLL)      return false;
  if(!IsDriverOpen()) return false;

  WORD _data = data;
  OutPort(nport,_data);

  return true;
}

#endif

