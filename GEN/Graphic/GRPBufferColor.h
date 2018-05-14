/*------------------------------------------------------------------------------------------
//  GRPBUFFERCOLOR.H
*/
/**
// \class
//
//  Graphics Color Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 05/09/2013 19:38:51
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPBUFFERCOLOR_H_
#define _GRPBUFFERCOLOR_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class GRPBUFFERCOLOR
{
  public:
                              GRPBUFFERCOLOR                ()                                {                                     }
    virtual                  ~GRPBUFFERCOLOR                ()                                {                                     }

    static GRPBUFFERCOLOR     GetNoColor                    ()                                { static GRPBUFFERCOLOR  color; return (const GRPBUFFERCOLOR)(color);     }
    static GRPBUFFERCOLOR     FromWaveLength                (double wl, double gamma = 1.0)   { static GRPBUFFERCOLOR  color; return (const GRPBUFFERCOLOR)(color);     }

};


#endif

