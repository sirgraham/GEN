/*------------------------------------------------------------------------------------------
//	SNDDUMMYRESAMPLER.H
*/	
/**	
// \file 
//   
//  dummy resampler
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 17/03/2017 12:19:01
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDDUMMYRESAMPLER_H_
#define _SNDDUMMYRESAMPLER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "SNDResampler.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class SNDDUMMYRESAMPLER : public SNDRESAMPLER
{
public:

																					SNDDUMMYRESAMPLER							();
	virtual																 ~SNDDUMMYRESAMPLER							();

	virtual		bool													Ini														(XQWORD dstchannels, SND_RESAMPLER_FORMAT dstformat, int dstsamplerate, XQWORD srcchannels, SND_RESAMPLER_FORMAT srcformat, int srcsamplerate);
	virtual		int														Allocate											(XBYTE *** audiodata, int* linesize, void* extradata, SND_RESAMPLER_FORMAT format, int align);
	virtual		void													Free													(XBYTE ** audiodata);
	virtual		int														Convert												(XBYTE** buffer, int buffersize, XBYTE** extendeddata, int samples);

	virtual		SND_RESAMPLER_FORMAT					GetResamplerFormat						(int nativeformat); 

protected:

private:
	void																		Clean													()
																					{

																					}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

