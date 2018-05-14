/*------------------------------------------------------------------------------------------
//  GRPLINUXSCREENX11GL.H
*/
/**
// \file
//
//  opens an OpenGL context on X11
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 02/02/2016 14:11:58
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPLINUXSCREENX11GL_H_
#define _GRPLINUXSCREENX11GL_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPLINUXScreenX11.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;

class GRPLINUXSCREENX11GL : public GRPLINUXSCREENX11
{
public:

                                        GRPLINUXSCREENX11GL         ( );
  virtual                              ~GRPLINUXSCREENX11GL         ();

  bool                                  Create                      (void* handle = NULL);
  bool                                  Update                      ();

protected:

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

