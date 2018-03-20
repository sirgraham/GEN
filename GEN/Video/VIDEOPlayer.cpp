
/*------------------------------------------------------------------------------------------
//	VIDEOPLAYER.CPP
//	
//	video player main class
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 11/04/2016 11:50:50
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "VIDEOPlayer.h"
#include "XPath.h"
#include "VIDEOFactory.h"
#include "VIDEOCodec.h"

#include "GRPRenderer.h"
#include "GRPContext.h"

#include "XFactory.h"
#include "XFile.h"

#include "XDebug.h"

#ifdef LINUX
#include "VIDEOLINUXFactory.h"
#endif

#ifdef ANDROID
#include "VIDEOANDROIDFactory.h"
#endif

#ifdef WINDOWS
#include "VIDEOWINDOWSFactory.h"
#endif

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


VIDEOPLAYER* VIDEOPLAYER::instance = NULL;



/*-------------------------------------------------------------------
//	VIDEOPLAYER::GetInstance
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			31/10/2016 10:15:00
//	
//	@return 			VIDEOPLAYER* : 
//
*/
/*-----------------------------------------------------------------*/
VIDEOPLAYER* VIDEOPLAYER::GetInstance()
{
	if(!instance)
		{
			instance = new VIDEOPLAYER();
		}

	return instance;
}



/*-------------------------------------------------------------------
//	VIDEOPLAYER::DeleteInstance
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			31/10/2016 10:16:11
//	
*/
/*-----------------------------------------------------------------*/
void VIDEOPLAYER::DeleteInstance()
{
	delete instance;
	instance = NULL;
}

/*-------------------------------------------------------------------
//	VIDEOPLAYER::VIDEOPLAYER
*/	
/**	
//	
//	Class Constructor VIDEOPLAYER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:54:59
//	
*/
/*-----------------------------------------------------------------*/
VIDEOPLAYER::VIDEOPLAYER()
{
	Clean();
}






/*-------------------------------------------------------------------
//	VIDEOPLAYER::~VIDEOPLAYER
*/	
/**	
//	
//	 Class Destructor VIDEOPLAYER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:55:06
//	
*/
/*-----------------------------------------------------------------*/
VIDEOPLAYER::~VIDEOPLAYER()
{
	
	XDEBUG_PRINTCOLOR(2, __L("videoplayer destructor"));
	Clean();
}






/*-------------------------------------------------------------------
//	VIDEOPLAYER::Ini
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			18/04/2016 14:03:02
//	
//	@return 			bool : 
//
//  @param				screen : 
//  @param				renderer : 
*/
/*-----------------------------------------------------------------*/
bool VIDEOPLAYER::Ini(GRPSCREEN* screen, GRPRENDERER* renderer, SNDFACTORY* soundsystem)
{
	if(!initialized)
		{
			Clean();

			if(!screen || !renderer /*|| !soundsystem*/) return false;

			#ifdef LINUX
			videofactory = new VIDEOLINUXFACTORY(this);
			#endif

			#ifdef ANDROID
			videofactory = new VIDEOANDROIDFACTORY(this);
			#endif

			#ifdef WINDOWS
			videofactory = new VIDEOWINDOWSFACTORY(this);
			#endif

			if(!videofactory) return false;

			this->screen = screen;
			this->renderer = renderer;

			xthread = xfactory->CreateThread(XTHREADGROUPID_VIDEO, __L("VideoPlayer Thread"), VIDEOPLAYER::VideoPlayerThread, this);
			if(!xthread) return false;

			xthread->SetStackSize(32*1024);

			initialized = true;

			this->soundsystem = soundsystem;

			return xthread->Ini();
		}

	return true;
}





/*-------------------------------------------------------------------
//	VIDEOPLAYER::End
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			18/04/2016 14:03:11
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOPLAYER::End()
{
	if(xthread)
		{
			xthread->End();
			xfactory->DeleteThread(XTHREADGROUPID_VIDEO, xthread);
		}

	if(codec)
		{
			delete codec;
			codec = NULL;
		}

	if(videofactory)
		{
			delete videofactory;
		}

	return true;
}





/*-------------------------------------------------------------------
//	VIDEOPLAYER::SetTarget
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:55:13
//	
//  @param				target : 
*/
/*-----------------------------------------------------------------*/
void VIDEOPLAYER::SetTarget(VIDEOPLAYER_TARGET target)
{
	this->target = target;
}




/*-------------------------------------------------------------------
//	VIDEOPLAYER::GetTarget
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:55:53
//	
//	@return 			VIDEOPLAYER_TARGET : 
//
*/
/*-----------------------------------------------------------------*/
VIDEOPLAYER_TARGET VIDEOPLAYER::GetTarget()
{
	return target;
}




/*-------------------------------------------------------------------
//	VIDEOPLAYER::PlayFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:56:02
//	
//	@return 			bool : 
//
//  @param				filename : 
*/
/*-----------------------------------------------------------------*/
bool VIDEOPLAYER::PlayFile(XCHAR* filename)
{
	
	// to avoid seeing the background on the rpi
	renderer->SetClearColor(GRPCOLOR::BLACK);
	renderer->Clear();
	renderer->GetContext()->Update();


	// need to create the correct codec from file extension
	XPATH xpath(filename);

	XFILE* xfile = xfactory->Create_File();
	if(!xfile->Exist(xpath.Get()))
	{
		xfactory->Delete_File(xfile);
		return false;
	}
	xfactory->Delete_File(xfile);

	XSTRING extension;
	xpath.GetExt(extension);

	//if(extension.Compare(__L(".h264")) == 0)
	if(extension.Compare(__L(".mp4")) == 0)
		{
			// H264 codec creation
			VIDEOCODEC* tmpcodec = videofactory->CreateH264Codec();
			tmpcodec->SetTarget(target);
			tmpcodec->SetFile(filename);
			tmpcodec->Ini();
			codec = tmpcodec;
			XDEBUG_PRINTCOLOR(2, __L("VideoPlayer True"));
			isplaying = true;
			isstopped = false;
		}
	else
		{
			return false;
		}

	return true;
}





/*-------------------------------------------------------------------
//	VIDEOPLAYER::PlayFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:56:10
//	
//	@return 			bool : 
//
//  @param				filename : 
*/
/*-----------------------------------------------------------------*/
bool VIDEOPLAYER::PlayFile(XSTRING* filename)
{
	return PlayFile(filename->Get());
}






/*-------------------------------------------------------------------
//	VIDEOPLAYER::Update
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			19/04/2016 17:50:48
//	
*/
/*-----------------------------------------------------------------*/
void VIDEOPLAYER::Update()
{
	if(codec)
		{
			//codec->Decode();
			if(codec->IsPlaying())
				{
					codec->Update();
				}

			if(!codec->IsPlaying())
				{
					VIDEOCODEC* pcodec = codec;
					codec = NULL;
					pcodec->End();
					delete pcodec;
					XDEBUG_PRINTCOLOR(2, __L("VideoPlayer False"));
					isplaying = false;
				}

		}
}



/*-------------------------------------------------------------------
//	VIDEOPLAYER::GetCodecType
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:56:17
//	
//	@return 			VIDEOPLAYER_CODEC : 
//
*/
/*-----------------------------------------------------------------*/
VIDEOPLAYER_CODEC VIDEOPLAYER::GetCodecType()
{
	return codectype;
}




/*-------------------------------------------------------------------
//	VIDEOPLAYER::SetPause
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:56:23
//	
//  @param				ispaused : 
*/
/*-----------------------------------------------------------------*/
void VIDEOPLAYER::SetPause(bool ispaused)
{
	ispaused = true;
}





/*-------------------------------------------------------------------
//	VIDEOPLAYER::Stop
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:56:30
//	
*/
/*-----------------------------------------------------------------*/
void VIDEOPLAYER::Stop()
{
	XDEBUG_PRINTCOLOR(2, __L("VideoPlayer False"));
	isstopped = true;
	isplaying = false;

	if(codec)
		{
			VIDEOCODEC* pcodec = codec;
			codec = NULL;
			pcodec->End();
			delete pcodec;
		}
}





/*-------------------------------------------------------------------
//	VIDEOPLAYER::IsPause
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:56:37
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOPLAYER::IsPause()
{
	return ispaused;
}





/*-------------------------------------------------------------------
//	VIDEOPLAYER::IsStopped
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:56:51
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOPLAYER::IsStopped()
{
	return isstopped;
}



/*-------------------------------------------------------------------
//	VIDEOPLAYER::IsPlaying
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/05/2016 10:25:42
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOPLAYER::IsPlaying()
{
	return isplaying;
}


/*-------------------------------------------------------------------
//	VIDEOPLAYER::SetLoop
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:56:57
//	
//  @param				loop : 
*/
/*-----------------------------------------------------------------*/
void VIDEOPLAYER::SetLoop(bool loop)
{
	this->loop = loop;
}




/*-------------------------------------------------------------------
//	VIDEOPLAYER::GetLoop
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:57:06
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOPLAYER::GetLoop()
{
	return this->loop;
}







/*-------------------------------------------------------------------
//	VIDEOPLAYER::VideoPlayerThread
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 12:57:23
//	
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
void VIDEOPLAYER::VideoPlayerThread(void* param)
{
	// need to handle pause/stop
	VIDEOPLAYER* player = (VIDEOPLAYER*)param;
	if(!player) return;

	if(player->IsPlaying() && !player->IsStopped())
		{
			if(player->codec)
				{
					if(player->codec->IsPlaying())
						{
							player->codec->Decode();
						}
				}
		}
}
