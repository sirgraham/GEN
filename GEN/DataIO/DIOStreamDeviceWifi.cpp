//------------------------------------------------------------------------------------------
//  DIOSTREAMDEVICEWIFI.CPP
//
//  Data IO Stream Device Wifi class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XConsole.h"
#include "XDebugTrace.h"

#include "DIOStreamDeviceWifi.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  DIOSTREAMDEVICEWIFI:: DIOSTREAMDEVICEWIFI
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:31
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOSTREAMDEVICEWIFI:: DIOSTREAMDEVICEWIFI()
{
  Clean();
}


//-------------------------------------------------------------------
//  DIOSTREAMDEVICEWIFI::~ DIOSTREAMDEVICEWIFI
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:20
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOSTREAMDEVICEWIFI::~ DIOSTREAMDEVICEWIFI()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEWIFI::DebugPrintInfo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/05/2013 17:01:57
//
//  @return       bool :
//  @param        console :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICEWIFI::DebugPrintInfo(XCONSOLE* xconsole)
{
  XSTRING line;
  XSTRING data;

  DIOSTREAMDEVICEIP::DebugPrintInfo(xconsole);

  //GetMAC()->GetXString(data);  line.Format(__L("MAC           : %s")  , data.Get());  DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());
  //GetIP()->GetXString(data);   line.Format(__L("IP            : %s") , data.Get());   DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());

  return true;
}


