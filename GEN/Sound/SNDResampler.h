/*----------------------------	--------------------------------------------------------------
//	SNDRESAMPLER.H
*/	
/**	
// \file 
//   
//  base audio resampler class
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 24/11/2016 11:29:14
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDRESAMPLER_H_
#define _SNDRESAMPLER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
enum SND_RESAMPLER_FORMAT
{
	SND_RESAMPLER_FORMAT_NONE,
	SND_RESAMPLER_FORMAT_U8,
	SND_RESAMPLER_FORMAT_S16,
	SND_RESAMPLER_FORMAT_S32,
	SND_RESAMPLER_FORMAT_FLT,
	SND_RESAMPLER_FORMAT_DBL,
	SND_RESAMPLER_FORMAT_U8P,
	SND_RESAMPLER_FORMAT_S16P,
	SND_RESAMPLER_FORMAT_S32P,
	SND_RESAMPLER_FORMAT_FLTP,
	SND_RESAMPLER_FORMAT_DBLP,
};

class SNDRESAMPLER
{
public:

																					SNDRESAMPLER()
																					{
																						Clean();
																					}


	virtual																 ~SNDRESAMPLER()
																					{
																						Clean();
																					}

	virtual		bool													Ini														(XQWORD dstchannels, SND_RESAMPLER_FORMAT dstformat, int dstsamplerate, XQWORD srcchannels, SND_RESAMPLER_FORMAT srcformat, int srcsamplerate) = 0;
	virtual		int														Allocate											(XBYTE *** audiodata, int* linesize, void* extradata, SND_RESAMPLER_FORMAT format, int align) = 0;
	virtual		void													Free													(XBYTE ** audiodata) = 0;
	virtual		int														Convert												(XBYTE** buffer, int buffersize, XBYTE** extendeddata, int samples) = 0;

	virtual		SND_RESAMPLER_FORMAT					GetResamplerFormat						(int nativeformat) = 0; 

protected:

private:
	void																		Clean													()
																					{
																					}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

