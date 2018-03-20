/*------------------------------------------------------------------------------------------
//	VIDEOFACTORY.H
*/	
/**	
// \file 
//   
//  factory to create correct platform specific codecs
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 11/04/2016 13:58:58
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _VIDEOFACTORY_H_
#define _VIDEOFACTORY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class VIDEOCODEC;
class VIDEOPLAYER;

class VIDEOFACTORY
{
public:

																		VIDEOFACTORY																		(VIDEOPLAYER* videoplayer);
	virtual													 ~VIDEOFACTORY																		();

	virtual VIDEOCODEC*								CreateH264Codec																	() = 0;

	void															DeleteCodec																			(VIDEOCODEC* codec);

	void*															GetVideoPlayer																	()																		{ return this->videoplayer; }

protected:
	VIDEOPLAYER*											videoplayer;
private:
	void															Clean																						()
																		{
																			videoplayer = NULL;
																		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

