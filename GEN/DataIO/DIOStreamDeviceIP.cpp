//------------------------------------------------------------------------------------------
//  DIOSTREAMDEVICEIP.CPP
//
//  Data IO Stream Device IP class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XConsole.h"
#include "XDebug.h"

#include "DIOStreamDeviceIP.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  DIOSTREAMDEVICEIP:: DIOSTREAMDEVICEIP
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:31
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOSTREAMDEVICEIP:: DIOSTREAMDEVICEIP()
{
  Clean();
}


//-------------------------------------------------------------------
//  DIOSTREAMDEVICEIP::~ DIOSTREAMDEVICEIP
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:20
//
//  @return
//  */
//-------------------------------------------------------------------
 DIOSTREAMDEVICEIP::~ DIOSTREAMDEVICEIP()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEIP::GetIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/03/2013 18:25:25
//
//  @return       DIOIP* :
//  */
/*-----------------------------------------------------------------*/
DIOIP* DIOSTREAMDEVICEIP::GetIP()
{
  return &IP;
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEIP::GetMAC
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/05/2013 0:32:24
//
//  @return       DIOMAC* :
//  */
/*-----------------------------------------------------------------*/
DIOMAC* DIOSTREAMDEVICEIP::GetMAC()
{
  return &MAC;
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEIP::GetIPType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/05/2013 22:42:46
//
//  @return       DIOSTREAMIPDEVICE_TYPE :
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMIPDEVICE_TYPE DIOSTREAMDEVICEIP::GetIPType()
{
  return iptype;
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEIP::SetIPType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/05/2013 22:42:53
//
//  @return       bool :
//  @param        iptype :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICEIP::SetIPType(DIOSTREAMIPDEVICE_TYPE iptype)
{
  this->iptype = iptype;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICEIP::DebugPrintInfo
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
bool DIOSTREAMDEVICEIP::DebugPrintInfo(XCONSOLE* xconsole)
{
  XSTRING line;
  XSTRING data;

  DIOSTREAMDEVICE::DebugPrintInfo(xconsole);

  GetMAC()->GetXString(data);  line.Format(__L("MAC           : %s")  , data.Get());  DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());
  GetIP()->GetXString(data);   line.Format(__L("IP            : %s") , data.Get());   DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());

  return true;
}



//-------------------------------------------------------------------
//  DIOSTREAMDEVICEIP::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2007 19:14:12
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOSTREAMDEVICEIP::Clean()
{
  type   = DIOSTREAMDEVICE_TYPE_IP;
  iptype = DIOSTREAMIPDEVICE_TYPE_UNKNOWN;

}



