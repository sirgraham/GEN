/*------------------------------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE.H
*/
/**
// \class
//
//  WINDOWS INPUT Device Mouse class
//
//  @author  Abraham J. Velez
//  @version 21/12/2011 16:54:40
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _INPWINDOWSDEVICEMOUSE_H_
#define _INPWINDOWSDEVICEMOUSE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"

#include "INPCursor.h"
#include "INPDevice.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;



class INPWINDOWSDEVICEMOUSE : public INPDEVICE
{
  public:
                              INPWINDOWSDEVICEMOUSE           ( GRPWINDOWSSCREEN* grpscreen=NULL);
    virtual                  ~INPWINDOWSDEVICEMOUSE           ();

    virtual bool              SetScreen                       (void* screenpointer);
    XVECTOR<INPBUTTON*>*      GetButtons                      ();
    XVECTOR<INPCURSOR*>*      GetCursors                      ();

    bool                      Update                          ();

  private:

    void                      Clean                           ();

    bool                      GetWindowPosition               (int& screenx, int& screeny, int& screenw, int& screenh);

    bool                      CreateAllButtons                ();
    bool                      DeleteAllButtons                ();

    bool                      CreateAllCursors                ();
    bool                      DeleteAllCursors                ();

    GRPWINDOWSSCREEN*         grpscreen;

    XVECTOR<INPBUTTON*>       buttons;
    XVECTOR<INPCURSOR*>       cursors;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

