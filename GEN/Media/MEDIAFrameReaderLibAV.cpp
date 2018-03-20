
/*------------------------------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV.CPP
//	
//	LibAV frame demuxer/reader
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 09/05/2016 12:12:30
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "MEDIAFrameReaderLibAV.h"

#include "XFactory.h"
#include "XFile.h"

#include "XDebug.h"

#include "XMemory.h"

#define FFMPEG_FILE_BUFFER_SIZE   32768

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

// LIBAV specific interface functions

static bool g_abort = false;
bool MEDIAFRAMEREADERLIBAV::registered = false;

/*-------------------------------------------------------------------
//	interrupt_cb
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:22
//	
//	@return 			int : 
//
//  @param				unused : 
*/
/*-----------------------------------------------------------------*/
static int interrupt_cb(void *unused)
{
  if(g_abort)
    return 1;
  return 0;
}


/*-------------------------------------------------------------------
//	file_read
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:22
//	
//	@return 			int : 
//
//  @param				h :
//
//	@param				buf :
//
//	@param				size :
*/
/*-----------------------------------------------------------------*/
static int file_read(void *h, XBYTE* buf, int size)
{
	if(interrupt_cb(NULL))
    return -1;

	XFILE* file = (XFILE*)h;

	int s = size;
	file->Read(buf, &s);

	return s;
}


/*-------------------------------------------------------------------
//	file_seek
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:22
//	
//	@return 			long long : 
//
//  @param				h : 
//
//	@param				pos :
//
//	@param				whence :
*/
/*-----------------------------------------------------------------*/
static long long file_seek(void *h, long long pos, int whence)
{
	if(interrupt_cb(NULL))
    return -1;

	XFILE* file = (XFILE*)h;

	if(whence == AVSEEK_SIZE)
		{
			return file->GetSize();
		}
  else
		{
			if(file->SetPosition((int)pos))
				{
					return pos;
				}

			return file->GetSize();
		}
}




/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::MEDIAFRAMEREADERLIBAV
*/	
/**	
//	
//	Class Constructor VIDEOH264FRAMEREADERLIBAV
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:10
//	
*/
/*-----------------------------------------------------------------*/
MEDIAFRAMEREADERLIBAV::MEDIAFRAMEREADERLIBAV()
{
	Clean();
}





/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::~MEDIAFRAMEREADERLIBAV
*/	
/**	
//	
//	 Class Destructor VIDEOH264FRAMEREADERLIBAV
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:15
//	
*/
/*-----------------------------------------------------------------*/
MEDIAFRAMEREADERLIBAV::~MEDIAFRAMEREADERLIBAV()
{
	if(xfile)
		{
			xfile->Close();
			xfactory->Delete_File(xfile);
		}

	finish_demuxer();

	Clean();
}




/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::init_demuxer
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:22
//	
//	@return 			int : 
//
//  @param				filename : 
*/
/*-----------------------------------------------------------------*/
int MEDIAFRAMEREADERLIBAV::init_demuxer(XSTRING* filename)
{
	//XDEBUG_PRINTCOLOR(2, __L("Initializing demuxer"));
	if(!registered)
		{
			av_register_all(); // shouldn't be needed as ffmpeg is supposedly clever enough not to run this twice internally
			registered = true;
		}
	
	this->filename = *filename;
	xfile->SetPosition(0);

	//const AVIOInterruptCB int_cb = { interrupt_cb, NULL };

	char* cstr = NULL;
	if(!filename->CreateOEM(cstr))
		{
			XDEBUG_PRINTCOLOR(4, __L("Error Creating OEM string"));
		}
	

	XBYTE* avbuffer = NULL;
	avbuffer = (XBYTE*)av_malloc(FFMPEG_FILE_BUFFER_SIZE);
	if(!avbuffer) return 0;
	
	aviocontext = avio_alloc_context(avbuffer, FFMPEG_FILE_BUFFER_SIZE, 0, xfile, file_read, NULL, file_seek);
	
	aviocontext->max_packet_size = 6144;
  if(aviocontext->max_packet_size)
		{
			aviocontext->max_packet_size *= FFMPEG_FILE_BUFFER_SIZE / aviocontext->max_packet_size; // why?
		}

	aviocontext->write_flag = 0;

	AVInputFormat* iformat = NULL;

	av_probe_input_buffer(aviocontext, &iformat, cstr, NULL, 0, 0);
	formatcontext = avformat_alloc_context();
	formatcontext->pb = aviocontext;

	//XDEBUG_PRINTCOLOR(3, __L("START OPEN INPUT"));
	//avformat_open_input(&formatcontext, cstr, iformat, NULL);
	avformat_open_input(&formatcontext, NULL, NULL, NULL);
	//XDEBUG_PRINTCOLOR(3, __L("END OPEN INPUT"));

	// set the interrupt callback, appeared in libavformat 53.15.0
  //formatcontext->interrupt_callback = int_cb;

	//formatcontext->flags |= AVFMT_FLAG_NONBLOCK;

	//if (iformat && (strcmp(iformat->name, "mjpeg") == 0) && aviocontext->seekable == 0)
	//	{
	//		formatcontext->max_analyze_duration = 500000;
	//	}

	//XDEBUG_PRINTCOLOR(3, __L("START STREAM INFO"));
	avformat_find_stream_info(formatcontext, NULL);
	//XDEBUG_PRINTCOLOR(3, __L("END STREAM INFO"));
	
	// find all the enabled streams
	video_stream_idx = av_find_best_stream(formatcontext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);

	if(videoenabled)
		{
			if(video_stream_idx >= 0)
				{		
					video_stream = formatcontext->streams[video_stream_idx];
			
					width = video_stream->codec->width;
					height = video_stream->codec->height;
					extradata = video_stream->codec->extradata;
					extradatasize = video_stream->codec->extradata_size;
			
					videocodeccontext = video_stream->codec;
			
					time_base_num = video_stream->time_base.num;
					time_base_den = video_stream->time_base.den;
			
					SPS.Resize(extradatasize);
					memcpy(SPS.GetPointer(0), extradata, extradatasize);
			
					videocodec = avcodec_find_decoder(video_stream->codec->codec_id);
				}
		}

	audio_stream_idx = av_find_best_stream(formatcontext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

	if(soundenabled)
		{
			if(audio_stream_idx >= 0)
				{
					// open the audio codec context
					audio_stream = formatcontext->streams[audio_stream_idx];
					audiocodeccontext = audio_stream->codec;
					audiocodec = avcodec_find_decoder(audio_stream->codec->codec_id);
				}
		}

	subtitles_stream_idx = av_find_best_stream(formatcontext, AVMEDIA_TYPE_SUBTITLE, -1, -1, NULL, 0);

	if(subtitlesenabled)
		{
			if(subtitles_stream_idx >= 0)
				{
					subtitles_stream = formatcontext->streams[audio_stream_idx];
					subtitlescodeccontext = subtitles_stream->codec;
					subtitlescodec = avcodec_find_decoder(subtitles_stream->codec->codec_id);
				}
		}

	delete [] cstr;

	readframes = 0;

	return 0;
}




/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::finish_demuxer
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/01/2017 10:15:07
//	
*/
/*-----------------------------------------------------------------*/
void MEDIAFRAMEREADERLIBAV::finish_demuxer()
{
	// copied from chrome: https://src.chromium.org/viewvc/chrome/trunk/src/media/filters/ffmpeg_glue.cc?revision=202275
	if(video_stream)
		{
			video_stream->discard = AVDISCARD_ALL;
		}

	if(videocodeccontext)
		{
			avcodec_close(videocodeccontext);
		}
	
	if(audiocodeccontext)
		{
			avcodec_close(audiocodeccontext);
		}

	avformat_close_input(&formatcontext);

	// avformat_free_context(formatcontext); not really needed it seems? (rpi crashes with it)

	// need to clear them separately, specially since the buffer is manually allocated
	av_freep(&(aviocontext->buffer));
	av_freep(&aviocontext);
}


/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::OpenFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:32
//	
//	@return 			bool : 
//
//  @param				filename : 
*/
/*-----------------------------------------------------------------*/
bool MEDIAFRAMEREADERLIBAV::OpenFile(XCHAR* filename)
{
	XSTRING xstr(filename);
	return OpenFile(&xstr);
}





/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::OpenFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:38
//	
//	@return 			bool : 
//
//  @param				filename : 
*/
/*-----------------------------------------------------------------*/
bool MEDIAFRAMEREADERLIBAV::OpenFile(XSTRING* filename)
{
	xfile = xfactory->Create_File();
	if(!xfile) return false;

	XPATH xpath;
	xpath.Set(*filename);

	if(!xfile->Open(xpath))
		{
			xfactory->Delete_File(xfile);
			return false;
		}

	return 0 == init_demuxer(filename);
}





/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::ReadFrame
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:51:45
//	
//	@return 			bool : 
//
//  @param				xvector : 
//  @param				timestamp : 
//  @param				decompression : 
*/
/*-----------------------------------------------------------------*/
bool MEDIAFRAMEREADERLIBAV::ReadFrame(MEDIAPACKET* packet)
{
	AVPacket pkt;
	
	// use av_read_frame to read the input frame
	int readframe = 0;
	readframe = av_read_frame(formatcontext, &pkt);
	
	// pkt side data is not freed
	// call av_packet_free_side_data maybe?

	if(readframe < 0)
		{
			av_packet_free_side_data(&pkt);
			#if LIBAVFORMAT_VERSION_MAJOR < 56
			av_free_packet(&pkt);
			#else
			av_packet_unref(&pkt);
			#endif
	
			endofstream = true;
			eof = true;
			return false;
		}

	bool cleanpacket = false;

	if((pkt.stream_index == video_stream_idx) && !videoenabled)
		{
			cleanpacket = true;
		}

	if((pkt.stream_index == audio_stream_idx) && !soundenabled)
		{
			cleanpacket = true;
		}

	if((pkt.stream_index == subtitles_stream_idx) && !subtitlesenabled)
		{
			cleanpacket = true;
		}

	if(cleanpacket)
		{
			av_packet_free_side_data(&pkt);
			#if LIBAVFORMAT_VERSION_MAJOR < 56
			av_free_packet(&pkt);
			#else
			av_packet_unref(&pkt);
			#endif
			return false;
		}
	
	// we need to pass back the frame we have read
	
	if(packet)
		{
			if(pkt.stream_index == video_stream_idx)
				{
					packet->packettype = MEDIAPACKETTYPE_VIDEO;
				}
			else if(pkt.stream_index == audio_stream_idx)
				{
					packet->packettype = MEDIAPACKETTYPE_AUDIO;
				}
			else if(pkt.stream_index == subtitles_stream_idx)
				{
					packet->packettype = MEDIAPACKETTYPE_SUBTITLES;
				}

			packet->buffer = new XBYTE[pkt.size];
			memcpy(packet->buffer, pkt.data, pkt.size);
			packet->size = pkt.size;
			packet->pts = pkt.pts;
			packet->dts = pkt.dts;

			if(soundenabled)
				{
					double time_base =  (double)audio_stream->time_base.num / (double)audio_stream->time_base.den;
					double tmp_duration = (double)pkt.duration * time_base * 1000.0;

					packet->duration = (float)(tmp_duration/1000.0);
				}
		}
	
	av_packet_free_side_data(&pkt);
	#if LIBAVFORMAT_VERSION_MAJOR < 56
	av_free_packet(&pkt);
	#else
	av_packet_unref(&pkt);
	#endif
	
	if(!packet) return false;

	readframes++;

	return true;
}




/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::ReadStream
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:45:03
//	
//	@return 			bool : 
//
//  @param				xvector : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool MEDIAFRAMEREADERLIBAV::ReadStream(XVECTOR<XBYTE> & xvector, XDWORD size)
{
	return true;
}





/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::IsEndOfStream
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:44:57
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool MEDIAFRAMEREADERLIBAV::IsEndOfStream()
{
	return endofstream;
}





/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::ResetStream
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:44:51
//	
*/
/*-----------------------------------------------------------------*/
void MEDIAFRAMEREADERLIBAV::ResetStream()
{
	av_seek_frame(formatcontext, audio_stream_idx, 0,  AVSEEK_FLAG_FRAME);
	endofstream = false;
}





/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::GetSPS
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:44:45
//	
//	@return 			XVECTOR<XBYTE>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<XBYTE>* MEDIAFRAMEREADERLIBAV::GetSPS()
{
	return &SPS;
}




/*-------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV::GetPPS
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/05/2016 11:44:39
//	
//	@return 			XVECTOR<XBYTE>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<XBYTE>* MEDIAFRAMEREADERLIBAV::GetPPS()
{
	return &PPS;
}


