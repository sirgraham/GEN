
/*------------------------------------------------------------------------------------------
//	SNDOPENALELEMENT.CPP
//	
//	handles an openal element
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 05/11/2015 10:40:23
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "SNDOpenALElement.h"
#include "SNDOpenALSource.h"
#include "SNDOpenALBuffer.h"
#include "SNDFileOGG.h"
#include "SNDFileFFMPEG.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	SNDOPENALELEMENT::SetFile
*/	
/**	
//	
//	sets the file that this element is going to play
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:29:14
//	
//	@return 			bool : 
//
//  @param				file : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALELEMENT::SetFile(SNDFILE* file)
{
	this->file = file;

	buffer->Upload(file->GetChannels(), file->GetData()->Get(), file->GetData()->GetSize(), file->GetSampleRate());

	name.Set(file->GetName()->Get());
	namefile.Set(file->GetName()->Get());
}



/*-------------------------------------------------------------------
//	SNDOPENALELEMENT::SetLoop
*/	
/**	
//	
//	sets the loop of an element
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/11/2015 10:27:13
//	
//  @param				loop : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALELEMENT::SetLoop(bool loop)
{
	this->loop = loop;
	
	SNDOPENALSOURCE* source = GetSource();

	if(source)
		{
			source->SetLoop(loop);
		}
}



/*-------------------------------------------------------------------
//	SNDOPENALELEMENT::Queue
*/	
/**	
//	
//	queues this element to play
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:30:03
//	
*/
/*-----------------------------------------------------------------*/
void SNDOPENALELEMENT::Queue()
{
	//GetSource()->Play(buffer);
	lastsource->Play(buffer);
}




/*-------------------------------------------------------------------
//	SNDOPENALELEMENT::SetVolume
*/	
/**	
//	
//	sets the volume of the element
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/11/2015 10:23:35
//	
//  @param				volume : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALELEMENT::SetVolume(float volume)
{
	this->volume = volume;
	
	SNDOPENALSOURCE* source = GetSource();

	if(source)
		{
			source->SetVolume(volume);
		}
}






/*-------------------------------------------------------------------
//	SNDOPENALELEMENT::SetPitch
*/	
/**	
//	
//	sets the pitxch of the element
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/11/2015 10:23:49
//	
//  @param				pitch : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENALELEMENT::SetPitch(float pitch)
{
	this->pitch = pitch;

	SNDOPENALSOURCE* source = GetSource();

	if(source)
		{
			source->SetPitch(pitch);
		}
}
