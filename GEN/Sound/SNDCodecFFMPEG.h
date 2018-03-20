/*------------------------------------------------------------------------------------------
//	SNDCODECFFMPEG.H
*/	
/**	
// \file 
//   
//  decodes audio via ffmpeg
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 22/11/2016 13:22:29
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDCODECFFMPEG_H_
#define _SNDCODECFFMPEG_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XList.h"
#include "XString.h"
#include "SNDCodec.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
//#include <libswresample/swresample.h>
#include <libavutil/channel_layout.h>
}
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class MEDIAFRAMEREADER;
struct MEDIAPACKET;
class XBUFFER;
class SNDRESAMPLER;

class SNDCODECFFMPEG : public SNDCODEC
{
public:

																			SNDCODECFFMPEG																				();
	virtual														 ~SNDCODECFFMPEG																				();

	virtual bool												SetFile																								(XCHAR* filename);
	virtual bool												SetFile																								(XSTRING* filename)																{ return this->SetFile(filename->Get());			}

	bool Ini();

	MEDIAFRAMEREADER*										GetFrameReader																				()																								{ return framereader;			}

	virtual void												Decode																								(); // but need to output buffers

	virtual void												Update																								(); // use thread or pass?

	virtual bool												DecodeAll																							(XBUFFER* xbuffer);

	float																GetDuration																						()																								{ return this->duration;	}
	int																	GetSamples																						()																								{ return this->samples;		}
	int																	GetChannels																						()																								{ return this->channels;	}
	int																	GetFrequency																					()																								{ return this->freq;			}

	double															ConvertDuration																				(long long duration);
	bool																DecodePacket																					(XBUFFER* xbuffer, MEDIAPACKET* packet);

protected:
	//MEDIAFRAMEREADER*										framereader;
	SNDRESAMPLER*												resampler;

	XLIST<MEDIAPACKET*>									audiopackets;

	float																duration;
	XDWORD															samples;
	XDWORD															channels;
	XDWORD															freq;

private:
	

	void																Clean																									()
																			{
																				framereader		= NULL;
																				resampler			= NULL;
																				duration			= 0;
																				samples				= 0;
																				channels			= 0;
																			}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

