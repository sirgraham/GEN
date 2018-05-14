/*------------------------------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11.H
*/
/**
// \class
//
//  LINUX input device mouse X11 class
//
//  @author  Abraham J. Velez
//  @version 21/12/2011 16:54:40
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _INPLINUXDEVICEMOUSEX11_H_
#define _INPLINUXDEVICEMOUSEX11_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"

#include "INPCursor.h"
#include "INPDevice.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define INPLINUXDEVICEMOUSE11_EVRELEASED    0
#define INPLINUXDEVICEMOUSE11_EVPRESSED     1
#define INPLINUXDEVICEMOUSE11_EVREPEAT      2

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class GRPSCREEN;
class GRPLINUXSCREENX11;

class INPLINUXDEVICEMOUSEX11 : public INPDEVICE
{
  public:
                            INPLINUXDEVICEMOUSEX11    ();
    virtual                ~INPLINUXDEVICEMOUSEX11    ();

    XVECTOR<INPBUTTON*>*    GetButtons                ();
    XVECTOR<INPCURSOR*>*    GetCursors                ();

    bool                    Update                    ();
    bool                    SetScreen                 (void* screenpointer);

  private:

    void                    Clean                     ();

    bool                    CreateAllButtons          ();
    bool                    DeleteAllButtons          ();

    bool                    CreateAllCursors          ();
    bool                    DeleteAllCursors          ();



    GRPLINUXSCREENX11*    grpscreenx11;

    XVECTOR<INPBUTTON*>     buttons;
    XVECTOR<INPCURSOR*>     cursors;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

