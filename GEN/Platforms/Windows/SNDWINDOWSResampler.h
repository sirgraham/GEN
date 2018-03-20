/*------------------------------------------------------------------------------------------
//	SNDWINDOWSRESAMPLER.H
*/	
/**	
// \file 
//   
//  ffmpeg resampler
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 24/11/2016 13:40:30
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDRESAMPLERFFMPEG_H_
#define _SNDRESAMPLERFFMPEG_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "SNDResampler.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
struct SwrContext;

class SNDWINDOWSRESAMPLER : public SNDRESAMPLER
{
public:

																					SNDWINDOWSRESAMPLER						();
	virtual																 ~SNDWINDOWSRESAMPLER						();

	virtual		bool													Ini														(XQWORD dstchannels, SND_RESAMPLER_FORMAT dstformat, int dstsamplerate, XQWORD srcchannels, SND_RESAMPLER_FORMAT srcformat, int srcsamplerate);
	virtual		int														Allocate											(XBYTE *** audiodata, int* linesize, void* extradata, SND_RESAMPLER_FORMAT format, int align);
	virtual		void													Free													(XBYTE ** audiodata);
	virtual		int														Convert												(XBYTE** buffer, int buffersize, XBYTE** extendeddata, int samples);

	virtual		SND_RESAMPLER_FORMAT					GetResamplerFormat						(int nativeformat);

protected:

	SwrContext*															swrcontext;

private:
	void																		Clean													()
																					{
																						swrcontext = NULL;
																					}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

