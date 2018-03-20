/*------------------------------------------------------------------------------------------
//	VIDEOPLAYER.H
*/	
/**	
// \file 
//   
//  main videoplayer class
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 11/04/2016 11:42:50
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _VIDEOPLAYER_H_
#define _VIDEOPLAYER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XString.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
enum VIDEOPLAYER_CODEC
{
	VIDEOPLAYER_CODEC_NONE					= 0	,
	VIDEOPALYER_CODEC_UNKNOWN						,
	VIDEOPLAYER_CODEC_THEORA						,
	VIDEOPLAYER_CODEC_H264
};
	
enum VIDEOPLAYER_TARGET
{
	VIDEOPLAYER_TARGET_NONE					= 0	,
	VIDEOPLAYER_TARGET_UNKNOWN					,
	VIDEOPLAYER_TARGET_FULLSCREEN				,
	VIDEOPLAYER_TARGET_TEXTURE
};

/*---- CLASS -----------------------------------------------------------------------------*/
class VIDEOCODEC;
class VIDEOFACTORY;

class GRPSCREEN;
class GRPRENDERER;

class SNDFACTORY;

class XTHREAD;

class VIDEOPLAYER
{
public:

	static VIDEOPLAYER*												GetInstance																						();
	static void																DeleteInstance																				();

																						VIDEOPLAYER																						();
	virtual																	 ~VIDEOPLAYER																						();

	bool																			Ini																										(GRPSCREEN* screen, GRPRENDERER* renderer, SNDFACTORY* soundsystem);
	bool																			End																										();

	void																			SetTarget																							(VIDEOPLAYER_TARGET target);
	VIDEOPLAYER_TARGET												GetTarget																							();

	bool																			PlayFile																							(XCHAR* filename);
	bool																			PlayFile																							(XSTRING* filename);

	void																			Update																								();

	VIDEOPLAYER_CODEC													GetCodecType																					();

	void																			SetPause																							(bool ispaused);
	void																			Stop																									();

	bool																			IsPause																								();
	bool																			IsStopped																							();

	bool																			IsPlaying																							();

	void																			SetLoop																								(bool loop);
	bool																			GetLoop																								();

	GRPSCREEN*																GetPlatformScreen																			()																										{ return this->screen;			}
	GRPRENDERER*															GetRenderer																						()																										{ return this->renderer;		}
	SNDFACTORY*																GetSoundSystem																				()																										{ return this->soundsystem; }

	static void																VideoPlayerThread																			(void* param);

	VIDEOCODEC*																codec;

protected:

	static VIDEOPLAYER*												instance;

	VIDEOPLAYER_CODEC													codectype;
	VIDEOPLAYER_TARGET												target;

	VIDEOFACTORY*															videofactory;

	GRPSCREEN*																screen;
	GRPRENDERER*															renderer;
	SNDFACTORY*																soundsystem;

	bool																			initialized;
	bool																			isplaying;
	bool																			ispaused;
	bool																			isstopped;
	bool																			loop;

	XTHREAD*																	xthread;

private:
	void																			Clean																									()
																						{
																							initialized		= false;
																							isplaying			= false;
																							ispaused			= false;
																							isstopped			= false;
																							loop					= false;
																							codectype			= VIDEOPLAYER_CODEC_NONE;
																							target				= VIDEOPLAYER_TARGET_NONE;
																							videofactory	= NULL;
																							codec					= NULL;
																							screen				= NULL;
																							renderer			= NULL;
																							soundsystem		= NULL;
																							xthread				= NULL;
																						}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

