//------------------------------------------------------------------------------------------
//  SCRIPTLIB.H
//
/**
// \class
//
//  Script library class
//
//  @author  Abraham J. Velez
//  @version 19/07/2007 20:06:27
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _SCRIPTLIB_H_
#define _SCRIPTLIB_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class SCRIPT;

class SCRIPTLIB
{
  public:
                          SCRIPTLIB               (XCHAR* ID);
    virtual              ~SCRIPTLIB               ();

    XSTRING*              GetID                   ();

    virtual bool          AddLibraryFunctions     (SCRIPT* script);


  protected:

    SCRIPT*               script;

  private:

    void                  Clean                   ();

    XSTRING               ID;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

