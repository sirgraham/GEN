
/*------------------------------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER.CPP
//	
//	implements ffmpeg resampler
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 24/11/2016 13:48:06
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "SNDWINDOWSResampler.h"
	
extern "C"
{
#include <libswresample/swresample.h>
#include <libavutil/channel_layout.h>
}

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER::SNDWINDOWSRESAMPLER
*/	
/**	
//	
//	Class Constructor SNDRESAMPLERFFMPEG
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2016 14:11:41
//	
*/
/*-----------------------------------------------------------------*/
SNDWINDOWSRESAMPLER::SNDWINDOWSRESAMPLER() : SNDRESAMPLER()
{
	Clean();
}




/*-------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER::~SNDWINDOWSRESAMPLER
*/	
/**	
//	
//	 Class Destructor SNDRESAMPLERFFMPEG
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2016 14:11:33
//	
*/
/*-----------------------------------------------------------------*/
SNDWINDOWSRESAMPLER::~SNDWINDOWSRESAMPLER()
{
	swr_free(&swrcontext);
	Clean();
}




/*-------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER::Ini
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2016 14:11:07
//	
//	@return 			bool : 
//
//  @param				dstchannels : 
//  @param				dstformat : 
//  @param				dstsamplerate : 
//  @param				srcchannels : 
//  @param				srcformat : 
//  @param				srcsamplerate : 
*/
/*-----------------------------------------------------------------*/
bool SNDWINDOWSRESAMPLER::Ini(XQWORD dstchannels, SND_RESAMPLER_FORMAT dstformat, int dstsamplerate, XQWORD srcchannels, SND_RESAMPLER_FORMAT srcformat, int srcsamplerate)
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

	swrcontext = swr_alloc_set_opts(NULL, dstchannels, AV_SAMPLE_FMT_S16, dstsamplerate,
																	srcchannels, format, srcsamplerate, 0, NULL);



	int swr_status = swr_init(swrcontext);

	return true;
}



/*-------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER::Allocate
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2016 17:12:26
//	
//	@return 			bool : 
//
//  @param				audiodata : 
//  @param				linesize : 
//  @param				nbchannels : 
//  @param				nbsamples : 
//  @param				srcformat : 
//  @param				align : 
*/
/*-----------------------------------------------------------------*/
int SNDWINDOWSRESAMPLER::Allocate(XBYTE *** audiodata, int* linesize, void* extradata, SND_RESAMPLER_FORMAT srcformat, int align)
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

	return av_samples_alloc_array_and_samples(audiodata, linesize, avframe->channels, avframe->nb_samples, format, 0); // 0 = default aligment, 1 = no aligment
}



/*-------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER::Free
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			28/11/2016 10:46:52
//	
//  @param				audiodata : 
*/
/*-----------------------------------------------------------------*/
void SNDWINDOWSRESAMPLER::Free(XBYTE ** audiodata)
{
	//av_free(audiodata);
	if(audiodata)
		{
			av_freep(&audiodata[0]);
			av_freep(&audiodata);
		}
}


/*-------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER::convert
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2016 14:10:58
//	
//	@return 			int : 
//
//  @param				buffer : 
//  @param				buffersize : 
//  @param				extendeddata : 
//  @param				samples : 
*/
/*-----------------------------------------------------------------*/
int SNDWINDOWSRESAMPLER::Convert(XBYTE** buffer, int buffersize, XBYTE** extendeddata, int samples)
{
	return swr_convert(swrcontext, buffer, buffersize, (const uint8_t**)extendeddata, samples);
}



/*-------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER::GetResamplerFormat
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2016 14:17:10
//	
//	@return 			SND_RESAMPLER_FORMAT : 
//
//  @param				nativeformat : 
*/
/*-----------------------------------------------------------------*/
SND_RESAMPLER_FORMAT SNDWINDOWSRESAMPLER::GetResamplerFormat(int nativeformat)
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
