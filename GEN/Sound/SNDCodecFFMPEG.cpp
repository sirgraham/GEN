
/*------------------------------------------------------------------------------------------
//  SNDCODECFFMPEG.CPP
//
//  decodes audio via ffmpeg
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 22/11/2016 13:27:17
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDFactory.h"
#include "SNDCodecFFMPEG.h"
#include "MEDIAFrameReaderLibAV.h"

#include "SNDResampler.h"
#include "XDebug.h"

#include "XBuffer.h"
#include "XVector.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::SNDCODECFFMPEG
*/
/**
//
//  Class Constructor SNDCODECFFMPEG
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      22/11/2016 13:28:00
//
*/
/*-----------------------------------------------------------------*/
SNDCODECFFMPEG::SNDCODECFFMPEG()
{
  Clean();

  framereader = new MEDIAFRAMEREADERLIBAV();


}



/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::~SNDCODECFFMPEG
*/
/**
//
//   Class Destructor SNDCODECFFMPEG
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      22/11/2016 13:28:04
//
*/
/*-----------------------------------------------------------------*/
SNDCODECFFMPEG::~SNDCODECFFMPEG()
{
  if(framereader)
  {
    delete framereader;
  }

  if(resampler)
  {
    delete resampler;
  }

  Clean();
}



/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::SetFile
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 11:12:34
//
//  @return       bool :
//
//  @param        filename :
*/
/*-----------------------------------------------------------------*/
bool SNDCODECFFMPEG::SetFile(XCHAR* filename)
{
  if(!framereader) return false;

  framereader->SetSoundEnabled(true);

  if(!framereader->OpenFile(filename))
    {
      XDEBUG_PRINTCOLOR(4, __L("Can't Open File %s"), filename);
      delete framereader;
      framereader = NULL;
      return false;
    }

  return true;
}



/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::Ini
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 11:12:43
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SNDCODECFFMPEG::Ini()
{
  MEDIAFRAMEREADERLIBAV* avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;
  if(!avframereader->GetAudioCodec()) return false;
  if(!avframereader->GetAudioCodecContext()) return false;

  if(avcodec_open2(avframereader->GetAudioCodecContext(), avframereader->GetAudioCodec(), NULL) < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("Error Opening Audio Codec"));
    }

  AVCodecContext* audiocontext = avframereader->GetAudioCodecContext();


  if(audiocontext->channel_layout == AV_CH_LAYOUT_STEREO)
    {
      channels = 2;
    }
  else if(audiocontext->channel_layout == AV_CH_LAYOUT_MONO)
    {
      channels = 1;
    }
  else
    {
      // failsafe if channel_layout is not populated
      channels = audiocontext->channels;
      if(channels == 2)
        {
          audiocontext->channel_layout = AV_CH_LAYOUT_STEREO;
        }
      else if(channels == 1)
        {
          audiocontext->channel_layout = AV_CH_LAYOUT_MONO;
        }
      else
        {
          XDEBUG_PRINTCOLOR(4, __L("AUDIO DECODE ERROR: no channel information"));
          return false;
        }
    }

  freq = audiocontext->sample_rate;

  AVStream* audio_stream = avframereader->GetAudioStream();

  double time_base =  (double)audio_stream->time_base.num / (double)audio_stream->time_base.den;
  double tmp_duration = (double)audio_stream->duration * time_base * 1000.0;

  duration = (float)(tmp_duration/1000.0);

  resampler = SNDFACTORY::Get()->GetResampler();

  resampler->Ini(audiocontext->channel_layout, SND_RESAMPLER_FORMAT_S16, audiocontext->sample_rate,
                 audiocontext->channel_layout, resampler->GetResamplerFormat(audiocontext->sample_fmt), audiocontext->sample_rate);

  this->samples = (XDWORD)(duration*audiocontext->sample_rate*channels);

  return true;
}



/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::Decode
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 11:12:51
//
*/
/*-----------------------------------------------------------------*/
void SNDCODECFFMPEG::Decode()
{

}



/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::Update
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 11:12:58
//
*/
/*-----------------------------------------------------------------*/
void SNDCODECFFMPEG::Update()
{

}




/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::DecodeAll
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 11:13:04
//
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool SNDCODECFFMPEG::DecodeAll(XBUFFER* xbuffer)
{
  //MEDIAFRAMEREADERLIBAV* avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;
  MEDIAPACKET packet;

  XDWORD realsize = 0;
  XDWORD position = 0;
  //XDWORD toresize = 10000;
  XDWORD toresize = 10000000;
  xbuffer->Resize(toresize);

  while(!framereader->IsEOF())
    {
      framereader->ReadFrame(&packet);

      XBUFFER newdata(false);

      DecodePacket(&newdata, &packet);

      //xbuffer->Add(newdata);
      if(realsize + newdata.GetSize() > xbuffer->GetSize())
        {
          xbuffer->Resize(xbuffer->GetSize() + toresize, false);
        }

      memcpy(xbuffer->Get() + position, newdata.Get(), newdata.GetSize());
      realsize += newdata.GetSize();
      position += newdata.GetSize();
    }

  xbuffer->Resize(realsize, false);
  return true;
}




/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::ConvertDuration
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      29/11/2016 11:12:56
//
//  @return       double :
//
//  @param        duration :
*/
/*-----------------------------------------------------------------*/
double SNDCODECFFMPEG::ConvertDuration(long long duration)
{
  MEDIAFRAMEREADERLIBAV* avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;

  AVStream* audio_stream = avframereader->GetAudioStream();

  double time_base =  (double)audio_stream->time_base.num / (double)audio_stream->time_base.den;
  return (double)duration * time_base * 1000.0;
}


/*-------------------------------------------------------------------
//  SNDCODECFFMPEG::DecodePacket
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      28/11/2016 11:13:11
//
//  @return       bool :
//
//  @param        xbuffer :
//  @param        packet :
*/
/*-----------------------------------------------------------------*/
bool SNDCODECFFMPEG::DecodePacket(XBUFFER* xbuffer, MEDIAPACKET* packet)
{
  // decode an individual ffmpeg packet
  MEDIAFRAMEREADERLIBAV* avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;

  AVFrame* audioframe = av_frame_alloc();
  int audioframefinished;

  // need to check we actually have data
  if(packet->buffer)
    {
      AVPacket pkt;
      av_init_packet(&pkt);
      pkt.data  = packet->buffer;
      pkt.size  = packet->size;
      pkt.dts   = packet->dts;
      pkt.pts   = packet->pts;

      int len = avcodec_decode_audio4(avframereader->GetAudioCodecContext(), audioframe, &audioframefinished, &pkt);

      XBYTE** buffer = NULL;
      int buffersize = 0;

      av_packet_unref(&pkt);

      int res = resampler->Allocate(&buffer, &buffersize, audioframe, SND_RESAMPLER_FORMAT_S16, 0); // 0 = default aligment, 1 = no aligment

      if(buffersize)
        {
          // maybe check that allocate gave something?
          int conv = resampler->Convert(buffer, buffersize, audioframe->extended_data, audioframe->nb_samples);

          xbuffer->Add(*buffer, buffersize);
        }

      resampler->Free(buffer);
    }

  av_frame_free(&audioframe);

  // need to check working
  //// free the buffer
  delete [] packet->buffer;
  packet->buffer = NULL;

  return true;
}
