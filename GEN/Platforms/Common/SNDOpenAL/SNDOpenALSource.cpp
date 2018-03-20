
/*------------------------------------------------------------------------------------------
//	SNDOPENALSOURCE.CPP
//	
//	source of a sound
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 05/11/2015 10:17:31
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDInstance.h"
#include "SNDOpenALSource.h"
#include "SNDOpenALBuffer.h"	
#include "SNDOpenALElement.h"
#include "SNDOpenAL.h"

#include "XDebug.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::SNDOPENALSOURCE
*/	
/**	
//	
//	Class Constructor SNDOPENALSOURCE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:30:48
//	
*/
/*-----------------------------------------------------------------*/
SNDOPENALSOURCE::SNDOPENALSOURCE(SNDOPENAL* openalsystem)
{
	Clean();

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	//alSourcef(source, AL_MAX_GAIN, 1000.0f);

	this->openalsystem = openalsystem;
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::~SNDOPENALSOURCE
*/	
/**	
//	
//	 Class Destructor SNDOPENALSOURCE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:30:54
//	
*/
/*-----------------------------------------------------------------*/
SNDOPENALSOURCE::~SNDOPENALSOURCE()
{
	alDeleteSources(1, &source);

	Clean();
}



/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::Play
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/12/2016 16:55:42
//	
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::Play()
{
	if(!IsPLaying())
		{
			alSourcePlay(source);
			isplaying = true;
		}
}


/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::Play
*/	
/**	
//	
//	plays a buffer
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:31:06
//	
//  @param				buffer : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::Play(SNDOPENALBUFFER* buffer)
{
	if(!IsPLaying())
		{
			alSourcei(source, AL_BUFFER, buffer->buffer);
			alSourcePlay(source);
			isplaying = true;
		}
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::Queue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/07/2016 9:48:58
//	
//  @param				buffer : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::Queue(SNDOPENALBUFFER* buffer)
{
	if(buffer)
		{
			alSourceQueueBuffers(source, 1, &(buffer->buffer));
			//bufferlist.Add(buffer);
		}
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::UnQueue
*/	
/**	
//	
//	unqueues an openal buffer
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			28/07/2016 10:11:01
//	
//  @param				buffer : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::UnQueue(SNDOPENALBUFFER* buffer)
{
	//if(buffer)
	//	{
	//		alSourceUnqueueBuffers(source, 1, &(buffer->buffer));
	//	}
	alSourceStop(source);
	ALuint ID = 0;
	alSourceUnqueueBuffers(source, 1, &ID);
}



/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::GetQueueLength
*/	
/**	
//	
//	gets the number of currently queued buffers
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			28/07/2016 10:11:16
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int SNDOPENALSOURCE::GetQueueLength()
{
	int queue = 0;
	alGetSourcei(source, AL_BUFFERS_QUEUED, &queue);
	return queue;
}





/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::GetProcessedBuffers
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			28/07/2016 11:31:04
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int SNDOPENALSOURCE::GetProcessedBuffers()
{
	int processed = 0;
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
	return processed;
}


/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::Stop
*/	
/**	
//	
//	stops the source
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:31:15
//	
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::Stop()
{
	alSourceStop(source);
	isplaying = false;
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::Pause
*/	
/**	
//	
//	pauses the source
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:31:37
//	
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::Pause()
{
	alSourcePause(source);
	isplaying = true;
}



/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::UnPause
*/	
/**	
//	
//	unpauses the source
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:31:48
//	
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::UnPause()
{
	alSourcePlay(source);
}



/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::SetLoop
*/	
/**	
//	
//	sets the source to looping
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:31:59
//	
//  @param				loop : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::SetLoop(bool loop)
{
	alSourcei(source, AL_LOOPING, loop);
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::IsPLaying
*/	
/**	
//	
//	checks if the source is currently playing
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:32:11
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SNDOPENALSOURCE::IsPLaying()
{
	if(isplaying)
		{
			ALint source_state;
			alGetSourcei(source, AL_SOURCE_STATE, &source_state);
			bool oldisplaying = isplaying;
			isplaying = ((source_state == AL_PLAYING) || (source_state == AL_PAUSED));
			if((oldisplaying == true) && (isplaying == false))
			{
				if(sndinstance)
				{
					SNDEVENT	sndevent(SNDFACTORY::Get()->GetXSUBJECT(), XEVENTTYPE_SOUND, XEVENTTYPE_SOUND);
										sndevent.SetSNDType(SNDEVENT_TYPE_STOP);
					sndinstance->HandleEvent(&sndevent);
				}
			}

			return isplaying;
		}

	return false;
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::IsStopped
*/	
/**	
//	
//	checks if the source is currently stopped
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:32:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SNDOPENALSOURCE::IsStopped()
{
	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);

	return (source_state == AL_STOPPED) || (source_state == AL_INITIAL);
}



/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::IsPaused
*/	
/**	
//	
//	checks if the source is currently paused
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:32:46
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SNDOPENALSOURCE::IsPaused()
{
	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);

	return source_state == AL_PAUSED;
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::SetVolume
*/	
/**	
//	
//	sets the volume of the source
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:32:56
//	
//  @param				volume : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::SetVolume(float volume)
{
	alSourcef(source, AL_GAIN, volume*openalsystem->GetMasterVolume());
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::GetVolume
*/	
/**	
//	
//	gets the volume of the source
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:33:06
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float SNDOPENALSOURCE::GetVolume()
{
	float volume;

	alGetSourcef(source, AL_GAIN, &volume);

	return volume/openalsystem->GetMasterVolume();
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::SetPitch
*/	
/**	
//	
//	sets the pitch of the source
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:33:19
//	
//  @param				pitch : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::SetPitch(float pitch)
{
	alSourcef(source, AL_PITCH, pitch);
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::GetPitch
*/	
/**	
//	
//	gets the pitch of the source
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:33:30
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float SNDOPENALSOURCE::GetPitch()
{
	float pitch;

	alGetSourcef(source, AL_PITCH, &pitch);

	return pitch;
}





/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::SetSecondsOffset
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/01/2016 12:14:30
//	
//  @param				seconds : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::SetSecondsOffset(float seconds)
{
	if(element)
		{
			if(seconds <= element->GetDuration())
				{
					alSourcef(source, AL_SEC_OFFSET, seconds);
				}
		}
}




/*-------------------------------------------------------------------
//	SNDOPENALSOURCE::SetSamplesOffset
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/01/2016 10:52:06
//	
//  @param				samples : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSOURCE::SetSamplesOffset(int samples)
{
	if(element)
		{
			if(samples > element->GetSamples())
				{
					samples = element->GetSamples();
				}
			alSourcei(source, AL_SAMPLE_OFFSET, samples);
		}
}
