//------------------------------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11.H
//
/**
// \class
//
//  LINUX input device keyboard X11 class
//
//  @author  Abraham J. Velez
//  @version 11/02/2003
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _INPLINUXDEVICEKEYBOARDX11_H_
#define _INPLINUXDEVICEKEYBOARDX11_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "XVector.h"

#include "INPButton.h"
#include "INPDevice.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define INPLINUXDEVICEKEYBOARDX11_EVRELEASED    0
#define INPLINUXDEVICEKEYBOARDX11_EVPRESSED     1
#define INPLINUXDEVICEKEYBOARDX11_EVREPEAT      2

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class GRPSCREEN;
class GRPLINUXSCREENX11;

class INPLINUXDEVICEKEYBOARDX11 : public INPDEVICE
{
  public:
                            INPLINUXDEVICEKEYBOARDX11     ();
    virtual                ~INPLINUXDEVICEKEYBOARDX11     ();

    XVECTOR<INPBUTTON*>*    GetButtons                    ();

    bool                    Release                       ();
    bool                    Update                        ();
    bool                    SetScreen(void* screenpointer);

  private:

    void                    Clean                         ();

    bool                    CreateAllButtons              ();
    bool                    DeleteAllButtons              ();

    bool                    AttachToFocusWindow           ();

    int                     GetKeyboardHandleEvent        ();



    GRPLINUXSCREENX11*    grpscreenx11;
    XVECTOR<INPBUTTON*>     buttons;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

