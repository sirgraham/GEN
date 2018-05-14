
/*------------------------------------------------------------------------------------------
//  SNDLINUXRESAMPLER.CPP
//
//  implements libav resampler
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 25/11/2016 11:21:50
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDLINUXResampler.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

extern "C"
{
#include <libavresample/avresample.h>
#include <libavutil/opt.h>
#include <libavutil/mem.h>
#include <libavutil/channel_layout.h>
}


/*-------------------------------------------------------------------
//  SNDLINUXRESAMPLER::SNDLINUXRESAMPLER
*/
/**
//
//  Class Constructor SNDRESAMPLERLIBAV
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 10:45:24
//
*/
/*-----------------------------------------------------------------*/
SNDLINUXRESAMPLER::SNDLINUXRESAMPLER() : SNDRESAMPLER()
{
  Clean();
}



/*-------------------------------------------------------------------
//  SNDLINUXRESAMPLER::~SNDLINUXRESAMPLER
*/
/**
//
//   Class Destructor SNDRESAMPLERLIBAV
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 10:45:18
//
*/
/*-----------------------------------------------------------------*/
SNDLINUXRESAMPLER::~SNDLINUXRESAMPLER()
{
  avresample_free(&avresampler);
  Clean();
}



/*-------------------------------------------------------------------
//  SNDLINUXRESAMPLER::Ini
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 10:45:11
//
//  @return       bool :
//
//  @param        dstchannels :
//  @param        dstformat :
//  @param        dstsamplerate :
//  @param        srcchannels :
//  @param        srcformat :
//  @param        srcsamplerate :
*/
/*-----------------------------------------------------------------*/
bool SNDLINUXRESAMPLER::Ini(XQWORD dstchannels, SND_RESAMPLER_FORMAT dstformat, int dstsamplerate, XQWORD srcchannels, SND_RESAMPLER_FORMAT srcformat, int srcsamplerate)
{
  AVSampleFormat format;

  switch(srcformat)
  {
    case SND_RESAMPLER_FORMAT_U8:
      format = AV_SAMPLE_FMT_U8;
    break;
    case SND_RESAMPLER_FORMAT_S16:
      format = AV_SAMPLE_FMT_S16;
    break;
    case SND_RESAMPLER_FORMAT_S32:
      format = AV_SAMPLE_FMT_S32;
    break;
    case SND_RESAMPLER_FORMAT_FLT:
      format = AV_SAMPLE_FMT_FLT;
    break;
    case SND_RESAMPLER_FORMAT_DBL:
      format = AV_SAMPLE_FMT_DBL;
    break;
    case SND_RESAMPLER_FORMAT_U8P:
      format = AV_SAMPLE_FMT_U8P;
    break;
    case SND_RESAMPLER_FORMAT_S16P:
      format = AV_SAMPLE_FMT_S16P;
    break;
    case SND_RESAMPLER_FORMAT_S32P:
      format = AV_SAMPLE_FMT_S32P;
    break;
    case SND_RESAMPLER_FORMAT_FLTP:
      format = AV_SAMPLE_FMT_FLTP;
    break;
    case SND_RESAMPLER_FORMAT_DBLP:
      format = AV_SAMPLE_FMT_DBLP;
    break;
  }

  avresampler = avresample_alloc_context();

  av_opt_set_int(avresampler, "out_channel_layout", dstchannels, 0);
  av_opt_set_int(avresampler, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
  av_opt_set_int(avresampler, "out_sample_rate", dstsamplerate, 0);
  av_opt_set_int(avresampler, "in_channel_layout", srcchannels, 0);
  av_opt_set_int(avresampler, "in_sample_fmt", format, 0);
  av_opt_set_int(avresampler, "in_sample_rate", srcsamplerate, 0);

  avresample_open(avresampler);

}



/*-------------------------------------------------------------------
//  SNDLINUXRESAMPLER::Allocate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 10:45:03
//
//  @return       int :
//
//  @param        audiodata :
//  @param        linesize :
//  @param        extradata :
//  @param        srcformat :
//  @param        align :
*/
/*-----------------------------------------------------------------*/
int SNDLINUXRESAMPLER::Allocate(XBYTE *** audiodata, int* linesize, void* extradata, SND_RESAMPLER_FORMAT srcformat, int align)
{
    AVSampleFormat format;

  switch(srcformat)
  {
    case SND_RESAMPLER_FORMAT_U8:
      format = AV_SAMPLE_FMT_U8;
    break;
    case SND_RESAMPLER_FORMAT_S16:
      format = AV_SAMPLE_FMT_S16;
    break;
    case SND_RESAMPLER_FORMAT_S32:
      format = AV_SAMPLE_FMT_S32;
    break;
    case SND_RESAMPLER_FORMAT_FLT:
      format = AV_SAMPLE_FMT_FLT;
    break;
    case SND_RESAMPLER_FORMAT_DBL:
      format = AV_SAMPLE_FMT_DBL;
    break;
    case SND_RESAMPLER_FORMAT_U8P:
      format = AV_SAMPLE_FMT_U8P;
    break;
    case SND_RESAMPLER_FORMAT_S16P:
      format = AV_SAMPLE_FMT_S16P;
    break;
    case SND_RESAMPLER_FORMAT_S32P:
      format = AV_SAMPLE_FMT_S32P;
    break;
    case SND_RESAMPLER_FORMAT_FLTP:
      format = AV_SAMPLE_FMT_FLTP;
    break;
    case SND_RESAMPLER_FORMAT_DBLP:
      format = AV_SAMPLE_FMT_DBLP;
    break;
  }

  AVFrame* avframe = (AVFrame*)extradata;

  int nbchannels = av_get_channel_layout_nb_channels(avframe->channel_layout);

  int ret, nb_planes = av_sample_fmt_is_planar(format) ? nbchannels : 1;

   *audiodata = (XBYTE**)av_mallocz(nb_planes * sizeof(**audiodata));

  av_samples_alloc(*audiodata, linesize, nbchannels, avframe->nb_samples, format, 0); // 0 = default aligment, 1 = no aligment
}



/*-------------------------------------------------------------------
//  SNDLINUXRESAMPLER::Free
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 10:44:54
//
//  @param        audiodata :
*/
/*-----------------------------------------------------------------*/
void SNDLINUXRESAMPLER::Free(XBYTE ** audiodata)
{
  if(audiodata)
    {
      av_freep(&audiodata[0]);
      av_freep(&audiodata);
    }
}



/*-------------------------------------------------------------------
//  SNDLINUXRESAMPLER::Convert
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 10:44:48
//
//  @return       int :
//
//  @param        buffer :
//  @param        buffersize :
//  @param        extendeddata :
//  @param        samples :
*/
/*-----------------------------------------------------------------*/
int SNDLINUXRESAMPLER::Convert(XBYTE** buffer, int buffersize, XBYTE** extendeddata, int samples)
{
  return avresample_convert(avresampler, buffer, 0, buffersize, (uint8_t**)extendeddata, 0, samples);
}



/*-------------------------------------------------------------------
//  SNDLINUXRESAMPLER::GetResamplerFormat
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 10:44:40
//
//  @return       SND_RESAMPLER_FORMAT :
//
//  @param        nativeformat :
*/
/*-----------------------------------------------------------------*/
SND_RESAMPLER_FORMAT SNDLINUXRESAMPLER::GetResamplerFormat(int nativeformat)
{
    switch(nativeformat)
  {
    case AV_SAMPLE_FMT_U8:
      return SND_RESAMPLER_FORMAT_U8;
    case AV_SAMPLE_FMT_S16:
      return SND_RESAMPLER_FORMAT_S16;
    case AV_SAMPLE_FMT_S32:
      return SND_RESAMPLER_FORMAT_S32;
    case AV_SAMPLE_FMT_FLT:
      return SND_RESAMPLER_FORMAT_FLT;
    case AV_SAMPLE_FMT_DBL:
      return SND_RESAMPLER_FORMAT_DBL;
    case AV_SAMPLE_FMT_U8P:
      return SND_RESAMPLER_FORMAT_U8P;
    case AV_SAMPLE_FMT_S16P:
      return SND_RESAMPLER_FORMAT_S16P;
    case AV_SAMPLE_FMT_S32P:
      return SND_RESAMPLER_FORMAT_S32P;
    case AV_SAMPLE_FMT_FLTP:
      return SND_RESAMPLER_FORMAT_FLTP;
    case AV_SAMPLE_FMT_DBLP:
      return SND_RESAMPLER_FORMAT_DBLP;
  }

  return SND_RESAMPLER_FORMAT_NONE;
}
