/*------------------------------------------------------------------------------------------
//  INPLINUXFACTORY.cpp
*/
/**
// \class
//
//  linux input factory
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 05/09/2016 17:55:52
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "INPLINUXFactory.h"

#ifndef HW_RASPBERRYPI
#include "INPLINUXDeviceKeyboardX11.h"
#include "INPLINUXDeviceMouseX11.h"
#endif

#include "XDebugTrace.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  INPLINUXFACTORY::CreateDevice
*/
/**
//
//  Create device
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/09/2016 10:22:02
//
//  @return       INPDEVICE* :
//
//  @param        device        : device name
//  @param        screenpointer : (default screenpointer=NULL)
*/
/*-----------------------------------------------------------------*/
INPDEVICE* INPLINUXFACTORY::CreateDevice(XCHAR* device,void* screenpointer)
{
  XDEBUGTRACE_PRINTCOLOR(0,__L("CreateDevice %s"),device);

#if (defined(GRP_ACTIVE)) && (!defined(HW_RASPBERRYPI))

    if(!XSTRING::Compare(device,__L("keyboard")))
    {
      INPLINUXDEVICEKEYBOARDX11* keyboard=new INPLINUXDEVICEKEYBOARDX11();
      keyboard->SetScreen(screenpointer);
      return keyboard;
    }

  if(!XSTRING::Compare(device,__L("mouse")))
    {
      INPLINUXDEVICEMOUSEX11* mouse=new INPLINUXDEVICEMOUSEX11();
      mouse->SetScreen(screenpointer);
      return mouse;
    }

#else

  if(!XSTRING::Compare(device,__L("keyboard")))
    {
      INPLINUXDEVICEKEYBOARDDIRECT* keyboard=new INPLINUXDEVICEKEYBOARDDIRECT();
      return keyboard;
    }

  if(!XSTRING::Compare(device,__L("mouse")))
    {
      INPLINUXDEVICEMOUSEDIRECT* mouse=new INPLINUXDEVICEMOUSEDIRECT((GRPSCREEN*)screenpointer);
      return mouse;
    }

#endif


  return NULL;
}


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

