/*------------------------------------------------------------------------------------------
//	VIDEOWINDOWSCODECH264FFMPEG.H
*/	
/**	
// \file 
//   
//  decodes and shows an mp4 file using ffmpeg
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 27/05/2016 17:47:31
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _VIDEOCODECH264FFMPEG_H_
#define _VIDEOCODECH264FFMPEG_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOCodecH264.h"
#include "GRPObserver.h"


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
//#include <libswresample/swresample.h>
#include <libavutil/channel_layout.h>
}

#include "XList.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class VIDEOPLAYER;


class GRPTEXTUREEXT;
class GRPELEMENT;
class GRPOBJECT;
class GRPVIEWPORT;
class GRPMATERIAL;
class SNDSTREAMELEMENT;

class XMUTEX;

struct MEDIAPACKET;

class VIDEOCODECH264FFMPEG : public VIDEOCODECH264//, public GRPOBSERVER
{
public:
																			VIDEOCODECH264FFMPEG																	(VIDEOPLAYER* videoplayer);
	virtual														 ~VIDEOCODECH264FFMPEG																	();

	virtual bool												Ini																										();
	virtual bool												End																										();

	virtual bool												SendFrame																							(XVECTOR<XBYTE> & frame);

	virtual void												Decode																								();

	virtual void												Update																								();

	//bool																Notify																								(GRPMSGTYPE msg,GRPSUBJECT* notifier);

protected:
	//AVFrame*														picture;

	XQWORD															frame_delay;
	XQWORD															frame_timeout;

	XMUTEX*															xmutex;
	XTIMER*															xtimer;

	XLIST<MEDIAPACKET*>									packets;
	XLIST<MEDIAPACKET*>									audiopackets;

	//SwrContext*													swrcontext;

	XDWORD															bufferedsize;

	bool																updatetexture;

	XBUFFER*														Y;
	XBUFFER*														U;
	XBUFFER*														V;

	GRPTEXTUREEXT*											textureY;
	GRPTEXTUREEXT*											textureU;
	GRPTEXTUREEXT*											textureV;

	GRPMATERIAL*												mat;
	GRPOBJECT*													quad;
	GRPELEMENT*													surface;
	GRPVIEWPORT*												viewport;

	SNDSTREAMELEMENT*										audiostreamer;

private:
	void																Clean																									()
																			{
																				frame_delay			= 0;
																				frame_timeout		= 0;
																				xmutex					= NULL;
																				xtimer					= NULL;
																				bufferedsize		= 0;
																				updatetexture		= false;
																				quad						= NULL;
																				mat							= NULL;
																				textureY				= NULL;
																				textureU				= NULL;
																				textureV				= NULL;
																			}

	bool IniGL();
	bool EndGL();
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

