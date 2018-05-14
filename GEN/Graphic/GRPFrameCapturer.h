/*------------------------------------------------------------------------------------------
//  GRPFRAMECAPTURER.H
*/
/**
// \file
//
//  GRP Frame Capturer
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 17/05/2017 13:13:17
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPFRAMECAPTURER_H_
#define _GRPFRAMECAPTURER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPRENDERER;
class GRPBITMAP;

class GRPFRAMECAPTURER
{
public:

                                        GRPFRAMECAPTURER                                    (GRPRENDERER* renderer);
  virtual                              ~GRPFRAMECAPTURER                                    ();

  bool                                  Capture                                             ();
  XBYTE*                                GetFrameData                                        ();
  GRPBITMAP*                            CreateBitmap                                        ();

protected:

  XBYTE*                                framedata;
  XDWORD                                datasize;
  GRPRENDERER*                          renderer;

private:
  void                                  Clean                                               ()
                                        {
                                          framedata   = NULL;
                                          datasize    = 0;
                                          renderer    = NULL;
                                        }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

