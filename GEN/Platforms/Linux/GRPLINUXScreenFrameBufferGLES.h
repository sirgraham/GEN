/*------------------------------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFERGLES.H
*/
/**
// \file
//
//  opens a GLES context using the framebuffer
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 10/02/2016 11:02:18
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPLINUXSCREENFRAMEBUFFERGLES_H_
#define _GRPLINUXSCREENFRAMEBUFFERGLES_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPLINUXScreenFrameBuffer.h"
#include "GRPOpenGL.h"

#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPLINUXSCREENFRAMEBUFFERGLES : GRPLINUXSCREENFRAMEBUFFER
{
public:

                                        GRPLINUXSCREENFRAMEBUFFERGLES               ( );
  virtual                              ~GRPLINUXSCREENFRAMEBUFFERGLES               ();

  virtual bool                          Create                                      (void* handle = NULL);
  virtual bool                          Update                                      ();
  virtual bool                          Delete                                      ();

protected:
  EGLDisplay                            display;
  EGLSurface                            surface;
  EGLContext                            context;
private:
  void                                  Clean                                       ()
                                        {
                                        }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

