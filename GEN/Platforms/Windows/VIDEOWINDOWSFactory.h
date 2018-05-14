/*------------------------------------------------------------------------------------------
//  VIDEOWINDOWSFACTORY.H
*/
/**
// \file
//
//  windows video factory
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 19/04/2016 17:57:20
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _VIDEOWINDOWSFACTORY_H_
#define _VIDEOWINDOWSFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOFactory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class VIDEOWINDOWSFACTORY : public VIDEOFACTORY
{
public:

                                    VIDEOWINDOWSFACTORY                               (VIDEOPLAYER* videoplayer);
  virtual                          ~VIDEOWINDOWSFACTORY                               ();

  virtual VIDEOCODEC*               CreateH264Codec                                 ();

protected:

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

