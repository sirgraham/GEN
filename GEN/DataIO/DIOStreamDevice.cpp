//------------------------------------------------------------------------------------------
//  DIOSTREAMDEVICE.CPP
//
//  Data IO Stream Device class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XDebugTrace.h"
#include "XConsole.h"
#include "DIOStreamDevice.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE:: DIOSTREAMDEVICE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 17:17:12
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICE:: DIOSTREAMDEVICE()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::~DIOSTREAMDEVICE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 17:17:23
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICE::~DIOSTREAMDEVICE()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::GetIndex
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 17:17:39
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOSTREAMDEVICE::GetIndex()
{
  return index;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::SetIndex
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 17:17:45
//
//  @return       bool :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICE::SetIndex(int index)
{
  this->index = index;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::GetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 23:00:03
//
//  @return       DIOSTREAMDEVICE_TYPE :
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICE_TYPE DIOSTREAMDEVICE::GetType()
{
  return type;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::SetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 22:59:58
//
//  @return       bool :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICE::SetType(DIOSTREAMDEVICE_TYPE type)
{
  this->type = type;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::GetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2013 2:36:12
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOSTREAMDEVICE::GetName()
{
  return &name;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::GetDescription
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2013 2:36:30
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOSTREAMDEVICE::GetDescription()
{
  return &description;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::GetResource
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2013 2:37:44
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOSTREAMDEVICE::GetResource()
{
  return &resource;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::CopyFrom
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 17:21:35
//
//  @return       bool :
//  @param        device :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICE::CopyFrom(DIOSTREAMDEVICE& device)
{
  index       = device.GetIndex();
  name        = (*device.GetName());
  description = (*device.GetDescription());
  resource    = (*device.GetResource());

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::CopyTo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 17:21:39
//
//  @return       bool :
//  @param        device :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICE::CopyTo(DIOSTREAMDEVICE& device)
{
  device.SetIndex(index);
  device.GetName()->Set(name);
  device.GetDescription()->Set(description);
  device.GetResource()->Set(resource);

  return true;
}


/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::DebugPrintInfo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/05/2013 11:17:06
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMDEVICE::DebugPrintInfo(XCONSOLE* xconsole)
{

  #ifdef XDEBUG_TRACE
  XSTRING line;

  line = __L(" ");                                                DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());
  line.Format(__L("Index         : %02d"), index);                DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());
  line.Format(__L("Name          : %s")  , name.Get());           DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());
  line.Format(__L("Description   : %s")  , description.Get());    DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());
  line.Format(__L("Resource      : %s")  , resource.Get());       DIOSTREAMDEBUGPRINTINFO(xconsole, line.Get());

  return true;
  #else
  return false;
  #endif
}


/*-------------------------------------------------------------------
//  DIOSTREAMDEVICE::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 17:18:09
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOSTREAMDEVICE::Clean()
{
  index = 0;
  type  = DIOSTREAMDEVICE_TYPE_UNKNOWN;
}

