/*------------------------------------------------------------------------------------------
//  INPWINDOWSFACTORY.H
*/
/**
// \class
//
//  Input Factory for Windows
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 05/09/2016 12:19:54
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _INPWINDOWSFACTORY_H_
#define _INPWINDOWSFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "INPFactory.h"
#include "INPWINDOWSDeviceKeyboard.h"
#include "INPWINDOWSDeviceMouse.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class INPWINDOWSFACTORY : public INPFACTORY
{
public:

      virtual INPDEVICE* CreateDevice(XCHAR* device,void* screenpointer=NULL)
      {

        if (XSTRING::Compare(device,__L("keyboard"))==0)
        {
          INPWINDOWSDEVICEKEYBOARD* keyboard=new INPWINDOWSDEVICEKEYBOARD((GRPWINDOWSSCREEN*)screenpointer);
          return keyboard;
        }

        if (XSTRING::Compare(device,__L("mouse"))==0)
        {
          INPWINDOWSDEVICEMOUSE* keyboard=new INPWINDOWSDEVICEMOUSE((GRPWINDOWSSCREEN*)screenpointer);
          return keyboard;
        }
        return NULL;
      }

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

