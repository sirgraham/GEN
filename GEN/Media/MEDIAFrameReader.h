/*------------------------------------------------------------------------------------------
//	MEDIAFRAMEREADER.H
*/	
/**	
// \file 
//   
//  Supplies individual frames
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 11/04/2016 17:33:39
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _MEDIAFRAMEREADER_H_
#define _MEDIAFRAMEREADER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
#include "XString.h"	

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum MEDIAPACKETTYPE
{
	MEDIAPACKETTYPE_UNKNOWN		= 0	,
	MEDIAPACKETTYPE_VIDEO					,
	MEDIAPACKETTYPE_AUDIO					,
	MEDIAPACKETTYPE_SUBTITLES			,
};
	
/*---- CLASS -----------------------------------------------------------------------------*/
template <class T>
class XVECTOR;

struct MEDIAPACKET
{
	MEDIAPACKET()
	{
		buffer = NULL;
		size = 0;
		pts = 0;
		dts = 0;
		duration = 0.0;
		packettype = MEDIAPACKETTYPE_UNKNOWN;
	}

	XBYTE*						buffer;
	int								size;
	long long					pts;
	long long					dts;
	double						duration;
	MEDIAPACKETTYPE		packettype;
};


class MEDIAFRAMEREADER
{
public:

																				MEDIAFRAMEREADER()
																				{
																					Clean();
																				}

	virtual															 ~MEDIAFRAMEREADER()
																				{
																					Clean();
																				}

	virtual bool													ReadFrame																					(XVECTOR<XBYTE> & xvector, long long* dts = NULL, long long* pts = NULL)															{ return true;	}
	virtual bool													ReadFrame																					(MEDIAPACKET* packet)																																									{ return true;	}
	virtual bool													ReadStream																				(XVECTOR<XBYTE> & xvector, XDWORD size)																																{ return true;	}

	virtual void													ResetStream																				()																																																		{								}

	virtual bool													OpenFile																					(XCHAR* filename)																																											{ return false; }
	virtual bool													OpenFile																					(XSTRING* filename)																																										{ return false;	}

	virtual bool													IsEndOfStream																			()																																																		{ return true;	}

	bool																	IsEOF																							()																																																		{ return eof;		}

	void																	SetVideoEnabled																		(bool videoenabled)																							{ this->videoenabled = videoenabled;					}
	void																	SetSoundEnabled																		(bool soundenabled)																							{ this->soundenabled = soundenabled;					}
	void																	SetSubtitleEnabled																(bool subtitlesenabled)																					{ this->subtitlesenabled = subtitlesenabled;	}
	bool																	GetVideoEnabled																		()																															{ return videoenabled;												}
	bool																	GetSoundEnabled																		()																															{ return soundenabled;												}
	bool																	GetSubtitlesEnabled																()																															{ return subtitlesenabled;										}

protected:
	bool																	eof;

	
	bool																	videoenabled;
	bool																	soundenabled;
	bool																	subtitlesenabled;

private:
	void																	Clean																							()
																				{
																					eof = false;

																					videoenabled							= false;
																					soundenabled							= false;
																					subtitlesenabled					= false;
																				}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

