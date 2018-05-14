/*------------------------------------------------------------------------------------------
//  SNDLINUXRESAMPLER.H
*/
/**
// \file
//
//  libav resampler implementation
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 25/11/2016 11:19:53
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDRESAMPLERLIBAV_H_
#define _SNDRESAMPLERLIBAV_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "SNDResampler.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

struct AVAudioResampleContext;

class SNDLINUXRESAMPLER : public SNDRESAMPLER
{
public:

                                          SNDLINUXRESAMPLER             ();
  virtual                                ~SNDLINUXRESAMPLER             ();

  virtual   bool                          Ini                           (XQWORD dstchannels, SND_RESAMPLER_FORMAT dstformat, int dstsamplerate, XQWORD srcchannels, SND_RESAMPLER_FORMAT srcformat, int srcsamplerate);
  virtual   int                           Allocate                      (XBYTE *** audiodata, int* linesize, void* extradata, SND_RESAMPLER_FORMAT format, int align);
  virtual   void                          Free                          (XBYTE ** audiodata);
  virtual   int                           Convert                       (XBYTE** buffer, int buffersize, XBYTE** extendeddata, int samples);

  virtual   SND_RESAMPLER_FORMAT          GetResamplerFormat            (int nativeformat);

protected:
  AVAudioResampleContext*                 avresampler;
private:
  void                                    Clean                         ()
                                          {
                                            avresampler = NULL;
                                          }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

