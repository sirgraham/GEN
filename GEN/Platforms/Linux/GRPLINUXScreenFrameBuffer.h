//------------------------------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFER.H
//
/**
// \class
//
//  LINUX graphics screen class
//
//  @author  Abraham J. Velez
//  @version 28/06/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPLINUXSCREENFRAMEBUFFER_H_
#define _GRPLINUXSCREENFRAMEBUFFER_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "XDebug.h"

#include "GRPScreen.h"

#include "XMemory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define GRPLINUXSCREENFRAMEBUFFER_DEVICE    "/dev/fb0"

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;

class GRPLINUXSCREENFRAMEBUFFER : public GRPSCREEN
{
  public:
                                          GRPLINUXSCREENFRAMEBUFFER   ();
    virtual                              ~GRPLINUXSCREENFRAMEBUFFER   ();

    virtual bool                          Create                      (void* handle = NULL);
    virtual bool                          Update                      ();
    virtual bool                          Delete                      ();

protected:
    int                                   handlefb;

    int                                   buffersize;
    XBYTE*                                buffer;
  private:

    void                                  Clean                       ();
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif



