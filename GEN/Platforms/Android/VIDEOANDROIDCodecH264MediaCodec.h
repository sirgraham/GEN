/*------------------------------------------------------------------------------------------
//  VIDEOANDROIDCODECH264MEDIACODEC.H
*/
/**
// \file
//
//  android h264 decoder using mediacodec
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 14/04/2016 13:29:14
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _VIDEOANDROIDCODECH264MEDIACODEC_H_
#define _VIDEOANDROIDCODECH264MEDIACODEC_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOCodecH264.h"
#include "GRPANDROIDScreen.h"
#include "media/NdkMediaCodec.h"
#include "android/native_window.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class VIDEOANDROIDCODECH264MEDIACODEC : public VIDEOCODECH264
{
public:

                                    VIDEOANDROIDCODECH264MEDIACODEC                 (VIDEOPLAYER* videoplayer);
  virtual                          ~VIDEOANDROIDCODECH264MEDIACODEC                 ();

  virtual bool                      Ini                                             ();

  virtual void                      Decode                                          ();
  virtual void                      Encode                                          ()                                                          {                                         }

protected:

  XTIMER*                           xtimer;
  XQWORD                            timestamp;
  AMediaCodec*                      codec;
  AMediaFormat*                     format;

private:
  void                              Clean                                           ()
                                    {
                                      xtimer      = NULL;
                                      timestamp   = 0;
                                      codec       = NULL;
                                      format      = NULL;
                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

