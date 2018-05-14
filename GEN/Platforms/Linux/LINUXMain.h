/*------------------------------------------------------------------------------------------
//  LINUXMAIN.H
*/
/**
// \class
//
//  Linux MAIN
//
//  @author  Abraham J. Velez
//  @version 16/09/2012 12:43:04
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _LINUXMAIN_H_
#define _LINUXMAIN_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <termios.h>

#include "XBase.h"
#include "Main.h"

#include "XLINUXDebug.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XPATH;
class XLINUXFACTORY;
class XLINUXSYSTEM;
class DIOLINUXFACTORY;
class GRPLINUXFACTORY;
class SNDLINUXBASE;
class XAPPLICATION;
class INPMANAGER;
class INPDEVICE;
class INPLINUXHID;
class SNDLINUXFACTORY;

class LINUXMAIN : public MAIN
{
  public:
                                  LINUXMAIN             ();
    virtual                      ~LINUXMAIN             ();

    bool                          Ini                   (XSTRING* xpath = NULL);

    bool                          Update                ();
    bool                          End                   ();

    XAPPLICATION*                 GetXApplication       ();

  private:

    void                          Clean                 ();

    XLINUXSYSTEM*                 xsystem;

    #ifdef INP_ACTIVE
    //struct termios                termiosinitialsettings;
    #endif

    XAPPLICATION*                 xapplication;
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#ifdef XLIB
extern "C"
{
  void __attribute__ ((constructor)) LIB_Ini  (void);
  void __attribute__ ((destructor))  LIB_End  (void);
}
#endif

#ifdef XDEBUG
extern XLINUXDEBUGCTRL       debug;
#endif

#endif

