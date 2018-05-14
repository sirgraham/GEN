/*------------------------------------------------------------------------------------------
//  MAIN.H
*/
/**
// \class
//
//  Main function and Main Loop
//
//  @author  Abraham J. Velez
//  @version 04/12/2013 11:23:11
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _MAIN_H_
#define _MAIN_H_


  /*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>

#include "XVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum MAIN_TYPESYSTEM
{
  MAIN_SYSTEM_UNKNOWN               = 0 ,
  MAIN_SYSTEM_WINDOWS                   ,
  MAIN_SYSTEM_LINUX                     ,
  MAIN_SYSTEM_MICRO_STM32FXXX
};


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XLOG;
class XAPPLICATION;
class XMUTEX;
class XSTRING;
class DIOFACTORY;
class DIOSTREAMTCPIPCONFIG;
class DIOSTREAM;
class DIOSMTP;

class MAIN
{
  public:
                                MAIN                    ();
    virtual                    ~MAIN                    ();

    virtual bool                Update                  ();
    virtual bool                End                     ();

    XVECTOR<XSTRING*>*          GetExecParams           ();
    bool                        DeleteAllExecParams     ();

  protected:

    MAIN_TYPESYSTEM             typesystem;
    XVECTOR<XSTRING*>           execparams;
    XLOG*                       xlogexception;

  private:

    void                        Clean                   ();
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif

