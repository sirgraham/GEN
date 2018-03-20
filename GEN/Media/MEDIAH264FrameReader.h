/*------------------------------------------------------------------------------------------
//	VIDEOH264FRAMEREADER.H
*/	
/**	
// \file 
//   
//  H264 frame reader
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 11/04/2016 10:41:50
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _VIDEOH264FRAMEREADER_H_
#define _VIDEOH264FRAMEREADER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBuffer.h"
#include "XVector.h"
#include "XFile.h"	
#include "XString.h"

#include "MEDIAFrameReader.h"

//#include "libutil/util.h"
//#include "mp4file.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

//mp4v2::impl::MP4File*
namespace mp4v2
{
	namespace impl
	{
		class MP4File;
	};
};

class MEDIAH264FRAMEREADER : public MEDIAFRAMEREADER
{
public:

																				MEDIAH264FRAMEREADER															();
	virtual															 ~MEDIAH264FRAMEREADER															();

	virtual bool													OpenFile																					(XCHAR* filename);
	virtual bool													OpenFile																					(XSTRING* filename);


	virtual bool													ReadFrame																					(XVECTOR<XBYTE> & xvector, long long* timestamp = NULL, long long* decompression = NULL);
	virtual bool													ReadStream																				(XVECTOR<XBYTE> & xvector, XDWORD size);

	virtual bool													IsEndOfStream																			();
	virtual void													ResetStream																				();

	int																		NALToRBSP																					(XVECTOR<XBYTE> & NAL, XVECTOR<XBYTE>& RBSP);

	void																	SetReadBufferSize																	(XDWORD readbuffersize);
	XDWORD																GetReadBufferSize																	();

	XVECTOR<XBYTE>*												GetSPS																						();
	XVECTOR<XBYTE>*												GetPPS																						();

//protected:
	XFILE*																xfile;
	XVECTOR<XBYTE>												readbuffer;
	XDWORD																lefttoread;
	XDWORD																readbuffersize;
	XDWORD																fileposition;

	XDWORD																ReadValue;
	XDWORD																ThreeBytesNAL;
	XDWORD																FourBytesNAL;
	XDWORD																NALUID;
	XDWORD																EraseByte;

	XDWORD																isample;
	mp4v2::impl::MP4File*									mp4file;

	XVECTOR<XBYTE>												SPS;
	XVECTOR<XBYTE>												PPS;

	XDWORD																readframes;

	XDWORD																width;
	XDWORD																height;

private:

	void																	FillReadBuffer																		();

	void																	Clean																							()
																				{
																					xfile						= NULL;
																					lefttoread			= 0;
																					readbuffersize	= 0;
																					fileposition		= 0;
																					ThreeBytesNAL		= 0;
																					FourBytesNAL		= 0;
																					NALUID					= 0;
																					EraseByte				= 0;
																					ReadValue				= 0;
																					mp4file					= NULL;
																					readframes			= 0;
																					width						= 0;
																					height					= 0;
																				}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

