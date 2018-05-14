/*------------------------------------------------------------------------------------------
//  GRPWINDOWSCONTEXT.H
*/
/**
// \file
//
//  windows graphics context
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 04/04/2016 17:12:25
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPWINDOWSCONTEXT_H_
#define _GRPWINDOWSCONTEXT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPOpenGL.h"
#include "GRPContext.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPWINDOWSCONTEXT : public GRPCONTEXT
{
public:

                                    GRPWINDOWSCONTEXT                         ();
  virtual                          ~GRPWINDOWSCONTEXT                         ();

  virtual bool                      Create                                    (void* handle = NULL);
  virtual bool                      Destroy                                   ();
  virtual bool                      IsLost                                    ();

protected:
  HGLRC                             hdlopenglrc;
  GRPOPENGLCTRL                     openglctrl;
private:
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

