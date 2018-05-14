/*------------------------------------------------------------------------------------------
//  GRPANDROIDCONTEXT.H
*/
/**
// \file
//
//  android graphics context
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 04/04/2016 16:04:41
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPANDROIDCONTEXT_H_
#define _GRPANDROIDCONTEXT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPContext.h"
#include "GRPOpenGL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPANDROIDCONTEXT : public GRPCONTEXT
{
public:

                                    GRPANDROIDCONTEXT                         ();
  virtual                          ~GRPANDROIDCONTEXT                         ();

  virtual bool                      Create                                    (void* handle = NULL);
  virtual bool                      Destroy                                   ();
  virtual bool                      IsLost                                    ();

  virtual bool                      Update                                    ();

protected:
  GRPOPENGLCTRL                     openglctrl;

  EGLDisplay                        display;
  EGLConfig                         config;
  EGLSurface                        surface;

  EGLContext                        context;
private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

