/*------------------------------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264.H
*/
/**
// \file
//
//  windows h264 decoder
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 19/04/2016 10:59:17
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _VIDEOWINDOWSCODECH264OPENH264_H_
#define _VIDEOWINDOWSCODECH264OPENH264_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOCodecH264.h"
#include "codec_api.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class GRPELEMENT;
class GRPVIEWPORT;

class XMUTEX;
class GRPTEXTUREEXT;

class VIDEOWINDOWSCODECH264OPENH264 : public VIDEOCODECH264
{
public:

                                      VIDEOWINDOWSCODECH264OPENH264                         (VIDEOPLAYER* videoplayer);
  virtual                            ~VIDEOWINDOWSCODECH264OPENH264                         ();

  virtual bool                        Ini                                                   ();
  virtual bool                        End                                                   ();

  virtual bool                        SendFrame                                             (XVECTOR<XBYTE> & frame);

  virtual void                        Decode                                                ();

  virtual void                        Update                                                ();

protected:
  XMUTEX*                             xmutex;
  ISVCDecoder*                        decoder;
  XBYTE*                              yuvdata[3];
  SBufferInfo                         sbufferinfo;

  XTIMER*                             xtimer;
  XQWORD                              timestamp;

  XBUFFER                             Y;
  XBUFFER                             U;
  XBUFFER                             V;

  bool                                initialized;
  bool                                ready;
  GRPTEXTUREEXT*                      textureY;
  GRPTEXTUREEXT*                      textureU;
  GRPTEXTUREEXT*                      textureV;

  GRPELEMENT*                         surface;
  GRPVIEWPORT*                        viewport;

private:
  void                                Clean                                                 ()
                                      {
                                        xmutex        = NULL;
                                        decoder       = NULL;
                                        yuvdata[0]    = NULL;
                                        yuvdata[1]    = NULL;
                                        yuvdata[2]    = NULL;
                                        xtimer        = NULL;
                                        timestamp     = 0;
                                        initialized   = false;
                                        ready         = false;
                                        textureY      = NULL;
                                        textureU      = NULL;
                                        textureV      = NULL;
                                      }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

