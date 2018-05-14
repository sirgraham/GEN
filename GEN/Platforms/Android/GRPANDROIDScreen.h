//------------------------------------------------------------------------------------------
//  GRPANDROIDSCREEN.H
//
/**
// \class
//
//  Virtual Screen
//
//  @author  Abraham J. Velez
//  @version 28/06/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPANDROIDSCREEN_H_
#define _GRPANDROIDSCREEN_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include <android_native_app_glue.h>
#include <android/native_activity.h>

#include "XString.h"
#include "XDebug.h"

#include "GRPOpenGL.h"
#include "GRPScreen.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;


class GRPANDROIDSCREEN : public GRPSCREEN
{
  public:
                                          GRPANDROIDSCREEN            ( );
    virtual                              ~GRPANDROIDSCREEN            ();


    bool                                  Create                      (void* handle = NULL);
    bool                                  Update                      ();
    bool                                  Delete                      ();
    bool                                  UpdateSize                  ();

    ANativeWindow*                        GetAndroidHandle            ()                                  { return this->handle; }
  private:

    void                                  Clean                       ();

    ANativeWindow*                        handle;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

