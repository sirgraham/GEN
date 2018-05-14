/*------------------------------------------------------------------------------------------
//  GRPLINUXX11GLCONTEXT.H
*/
/**
// \file
//
//  linux X11 OpenGL Context
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 08/04/2016 12:39:36
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPLINUXX11GLCONTEXT_H_
#define _GRPLINUXX11GLCONTEXT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPContext.h"
#include "GRPOpenGL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPLINUXX11GLCONTEXT : public GRPCONTEXT
{
public:

                                    GRPLINUXX11GLCONTEXT                      ();
  virtual                          ~GRPLINUXX11GLCONTEXT                      ();

  virtual bool                      Create                                    (void* handle = NULL);
  virtual bool                      Destroy                                   ();
  virtual bool                      IsLost                                    ();

  virtual bool                      Update                                    ();


protected:

  GLXContext                        glc;
  GRPOPENGLCTRL                     openglctrl;

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

