/*------------------------------------------------------------------------------------------
//  VIDEOANDROIDJNICODECH264MEDIACODEC.H
*/
/**
// \file
//
//  android h264 player via JNI
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 27/04/2016 17:19:20
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _VIDEOANDROIDJNICODECH264MEDIACODEC_H_
#define _VIDEOANDROIDJNICODECH264MEDIACODEC_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOCodecH264.h"
#include "ANDROIDJNIObject.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class VIDEOANDROIDJNICODECH264MEDIACODEC : public VIDEOCODECH264, public ANDROIDJNIOBJECT
{
public:

                                    VIDEOANDROIDJNICODECH264MEDIACODEC              (VIDEOPLAYER* videoplayer);
  virtual                          ~VIDEOANDROIDJNICODECH264MEDIACODEC              ();

  virtual bool                      Ini                                             ();
  virtual bool                      IniCodec                                        ();
  virtual bool                      SendFrame                                       (XVECTOR<XBYTE>& frame);

  virtual void                      Decode                                          ();

protected:
  XTIMER*                           xtimer;
  XQWORD                            timestamp;
  XDWORD                            flags;

  ANDROIDJNIMETHOD                  jniIni;
  ANDROIDJNIMETHOD                  jniSendFrame;
private:
  void Clean()
  {
    xtimer = NULL;
    timestamp = 0;
    flags = 0;
  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

