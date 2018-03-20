/*------------------------------------------------------------------------------------------
//	MEDIAFRAMEREADERLIBAV.H
*/	
/**	
// \file 
//   
//  h265 frame reader demuxing an mp4
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 09/05/2016 11:59:14
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _VIDEOH264FRAMEREADERLIBAV_H_
#define _VIDEOH264FRAMEREADERLIBAV_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "MEDIAFrameReader.h"

#include "XVector.h"
#include "XString.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

	
/*---- CLASS -----------------------------------------------------------------------------*/
class XFILE;


class MEDIAFRAMEREADERLIBAV : public MEDIAFRAMEREADER
{
public:

																				MEDIAFRAMEREADERLIBAV															();
	virtual															 ~MEDIAFRAMEREADERLIBAV															();

	int																		init_demuxer																			(XSTRING* filename);
	void																	finish_demuxer																		();

	virtual bool													OpenFile																					(XCHAR* filename);
	virtual bool													OpenFile																					(XSTRING* filename);

	virtual bool													ReadFrame																					(MEDIAPACKET* packet);
	virtual bool													ReadStream																				(XVECTOR<XBYTE> & xvector, XDWORD size);

	virtual bool													IsEndOfStream																			();
	virtual void													ResetStream																				();

	XVECTOR<XBYTE>*												GetSPS																						();
	XVECTOR<XBYTE>*												GetPPS																						();

	AVCodec*															GetVideoCodec																			()																															{ return videocodec;							}
	AVCodec*															GetAudioCodec																			()																															{ return audiocodec;							}
	AVCodecContext*												GetVideoCodecContext															()																															{ return videocodeccontext;				}
	AVCodecContext*												GetAudioCodecContext															()																															{ return this->audiocodeccontext; }
	AVStream*															GetVideoStream																		()																															{ return this->video_stream;			}
	AVStream*															GetAudioStream																		()																															{ return this->audio_stream;			}

	XDWORD																time_base_num;
	XDWORD																time_base_den;

	XDWORD																width;
	XDWORD																height;


protected:
	XSTRING																filename;
	static bool														registered;

	bool																	endofstream;
	XVECTOR<XBYTE>												SPS;
	XVECTOR<XBYTE>												PPS;

	XDWORD																readframes;

	
	XFILE*																xfile;
	int																		video_stream_idx;
	int																		audio_stream_idx;
	int																		subtitles_stream_idx;
	AVStream*															video_stream;
	AVStream*															audio_stream;
	AVStream*															subtitles_stream;
	AVFormatContext*											formatcontext;
	AVIOContext*													aviocontext;
	AVCodec*															videocodec;
	AVCodec*															audiocodec;
	AVCodec*															subtitlescodec;
	AVCodecContext*												videocodeccontext;
	AVCodecContext*												audiocodeccontext;
	AVCodecContext*												subtitlescodeccontext;

	XBYTE																	extradatasize;
	void*																	extradata;

private:
	void																	Clean																							()
																				{
																					endofstream								= false;
																					readframes								= 0;
																					width											= 0;
																					height										= 0;
																					xfile											= NULL;
																					video_stream_idx					= -1;
																					audio_stream_idx					= -1;
																					subtitles_stream_idx			= -1;
																					video_stream							= NULL;
																					audio_stream							= NULL;
																					subtitles_stream					= NULL;
																					formatcontext							= NULL;
																					aviocontext								= NULL;
																					videocodec								= NULL;
																					audiocodec								= NULL;
																					subtitlescodec						= NULL;
																					videocodeccontext					= NULL;
																					audiocodeccontext					= NULL;
																					subtitlescodeccontext			= NULL;
																					extradatasize							= 0;
																					extradata									= NULL;
																					readframes								= 0;
																					time_base_num							= 0;
																					time_base_den							= 0;
																				}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

