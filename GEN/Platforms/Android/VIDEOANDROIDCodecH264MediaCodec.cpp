
/*------------------------------------------------------------------------------------------
//	VIDEOANDROIDCODECH264MEDIACODECCPP.CPP
//	
//	android h264 decoder using mediacodec
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 14/04/2016 13:28:16
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <android/window.h>
#include <android\native_window_jni.h>
//#include <android/ui/android_native_buffer.h>
//#include <android/ui/egl/android_natives.h>

#include "VIDEOANDROIDCodecH264MediaCodec.h"
#include "VIDEOPlayer.h"	
#include "MEDIAH264FrameReader.h"

#include "GRPRenderer.h"
#include "GRPContext.h"

#include "XTimer.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
VIDEOANDROIDCODECH264MEDIACODEC::VIDEOANDROIDCODECH264MEDIACODEC(VIDEOPLAYER* videoplayer) : VIDEOCODECH264(videoplayer)
{
	xtimer = xfactory->CreateTimer(); 
}

VIDEOANDROIDCODECH264MEDIACODEC::~VIDEOANDROIDCODECH264MEDIACODEC()
{
	if(xtimer) xfactory->DeleteTimer(xtimer);

	AMediaFormat_delete(format);
	AMediaCodec_delete(codec);
}

bool VIDEOANDROIDCODECH264MEDIACODEC::Ini()
{
	videoplayer->GetRenderer()->GetContext()->Destroy();
	//initialized = true;

	return true;
}

void VIDEOANDROIDCODECH264MEDIACODEC::Decode()
{
	if(!initialized)
		{
			if(!videoplayer) return;

			GRPANDROIDSCREEN* androidscreen = (GRPANDROIDSCREEN*)this->videoplayer->GetPlatformScreen();
			// need to destroy the context
			//this->videoplayer->GetRenderer()->GetContext()->Destroy();

			format = AMediaFormat_new();

			// use the width and heigt that we must have already read

			AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_WIDTH, width);
			AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_HEIGHT, height);

			AMediaFormat_setBuffer(format, "csd-0", SPS.GetPointer(0), SPS.GetSize());
			AMediaFormat_setBuffer(format, "csd-1", PPS.GetPointer(0), PPS.GetSize());

			AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, "video/avc");
			AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_MAX_INPUT_SIZE, width*height);
			AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_PUSH_BLANK_BUFFERS_ON_STOP, 1);

			// need the screen ANativeWindow to configure the codec
			
			codec = AMediaCodec_createDecoderByType("video/avc");
			media_status_t status = AMediaCodec_configure(codec, format, androidscreen->GetAndroidHandle(), NULL, 0);
			
			if(status != AMEDIA_OK)
			{
				XDEBUG_PRINTCOLOR(4, __L("Error Configuring MediaCodec: "));
	
				switch(status)
				{
				case AMEDIA_ERROR_UNKNOWN:
					XDEBUG_PRINTCOLOR(4, __L("UNKNOWN"));
					break;
				case AMEDIA_ERROR_MALFORMED:
					XDEBUG_PRINTCOLOR(4, __L("MALFORMED"));
					break;
				case AMEDIA_ERROR_UNSUPPORTED:
					XDEBUG_PRINTCOLOR(4, __L("UNSUPPORTED"));
					break;
				case AMEDIA_ERROR_INVALID_OBJECT:
					XDEBUG_PRINTCOLOR(4, __L("INVALID OBJECT"));
					break;
				case AMEDIA_ERROR_INVALID_PARAMETER:
					XDEBUG_PRINTCOLOR(4, __L("INVALID PARAMETER"));
					break;
				}
			}

			status = AMediaCodec_start(this->codec);

			if(status != AMEDIA_OK)
			{
				XDEBUG_PRINTCOLOR(4, __L("Error Starting Codec"));
			
				switch(status)
				{
				case AMEDIA_ERROR_UNKNOWN:
					XDEBUG_PRINTCOLOR(4, __L("UNKNOWN"));
					break;
				case AMEDIA_ERROR_MALFORMED:
					XDEBUG_PRINTCOLOR(4, __L("MALFORMED"));
					break;
				case AMEDIA_ERROR_UNSUPPORTED:
					XDEBUG_PRINTCOLOR(4, __L("UNSUPPORTED"));
					break;
				case AMEDIA_ERROR_INVALID_OBJECT:
					XDEBUG_PRINTCOLOR(4, __L("INVALID OBJECT"));
					break;
				case AMEDIA_ERROR_INVALID_PARAMETER:
					XDEBUG_PRINTCOLOR(4, __L("INVALID PARAMETER"));
					break;
				}
			}

			// from this point onwards decoding can happen
			initialized = true;
			isplaying = true;
		}


	if(timestamp + xtimer->GetMeasureMicroSeconds() > (((1.0/fps)*1000*1000) + timestamp))
		{
			XVECTOR<XBYTE> frame;
			if(framereader->ReadFrame(frame)) // make sure we can read the frame
				{
					ssize_t idx = AMediaCodec_dequeueInputBuffer(codec, 2000);
					if(idx < 0)
						{
							XDEBUG_PRINTCOLOR(4, __L("Android MediaCodec: No valid input buffer ID found"));
							return;
						}
	
					XDWORD size;
					XBYTE* buf = AMediaCodec_getInputBuffer(codec, idx, &size);
					if(!buf)
						{
							XDEBUG_PRINTCOLOR(4, __L("Android MediaCodec: No valid buffer found"));
							return;
						}
	
			
					if(size < frame.GetSize())
						{
							XDEBUG_PRINTCOLOR(4, __L("Android MediaCodec: Error, input buffer is smaller than read frame"));
						}
					else
						{
							memcpy(buf, frame.GetPointer(0), frame.GetSize());
						}
	
					// Careful here as it asks for the size of the NAL unit, not the size of the buffer it just gave us
					// which makes sense
					// the timestamp seems to need to be different for each queued input buffer
					// in increasing order, and the time it needs to be according to the framerate
					
					XDWORD flags = 0;

					if(framereader->IsEndOfStream())
					{
						//flags |= AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM;
					}

					AMediaCodec_queueInputBuffer(codec, idx, 0, frame.GetSize(), timestamp, flags);
					XDEBUG_PRINTCOLOR(2, __L("InputBuffer Queued Back"));
				}

			// dequeue the output buffer
			AMediaCodecBufferInfo bufferinfo; // correct? fuck java
			ssize_t odx = AMediaCodec_dequeueOutputBuffer(codec, &bufferinfo, 2000); // buffer info?

			// check bufferinfoflag for AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM
			if(bufferinfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM)
				{
					isplaying = false;
				}

			// release the output buffer
			ssize_t releasestatus = AMediaCodec_releaseOutputBuffer(codec, odx, true); // true for rendering the output buffer, maybe false if manual GL rendering?


			timestamp += xtimer->GetMeasureMicroSeconds();
			xtimer->Reset();
		}
}
