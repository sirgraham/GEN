
/*------------------------------------------------------------------------------------------
//	SNDOPENAL.CPP
//	
//	OpenAL device backend
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 03/11/2015 14:11:28
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XTimer.h"
#include "XDebug.h"
#include "XThread.h"


#include "SNDOpenALSource.h"
#include "SNDOpenALElement.h"
#include "SNDOpenALStreamElement.h"
#include "SNDFileOGG.h"
#include "SNDFileFFMPEG.h"
#include "SNDEvent.h"
#include "SNDInstance.h"
#include "SNDStreamInstance.h"

#include "MEDIAFrameReader.h"

#include "SNDOpenAL.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::SNDOPENALSYSTEM
*/	
/**	
//	
//	Class Constructor SNDOPENALSYSTEM
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 11:07:24
//	 
*/
/*-----------------------------------------------------------------*/
SNDOPENAL::SNDOPENAL(SNDFACTORY* sndsystem)
{
	Clean();

	streammutex = xfactory->Create_Mutex();

	this->sndsystem = sndsystem;

	sndsystem->SetXSUBJECT(this);

	//const ALCchar *defaultDeviceName;

	//defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	// OpenAL Initialization
	//device = alcOpenDevice(defaultDeviceName);
	device = alcOpenDevice("");

	if(!device)
		{
			return;
		}

	ALCenum error;

	error = alGetError();

	context = alcCreateContext(device, NULL);

	error = alGetError();

	if(!alcMakeContextCurrent(context))
		{
			return;
		}

	maxchannels = 32;

	// Create 16 sources as who could possibly want more
	sources.SetAddInLimit(16);
	sources.Resize(maxchannels);
	for(XDWORD i = 0; i < maxchannels; i++)
		{
			SNDOPENALSOURCE* src = new SNDOPENALSOURCE(this);
			if(!src)
				{
					return;
				}

			sources.Set(i, src);
		}

	mastervolume = 1.0f;

	// create the streaming thread
	streamthread = xfactory->CreateThread(XTHREADGROUPID_UNGROUP, __L("SNDTHREAD"), SNDOPENAL::StreamingThread, this);
	streamthread->Ini();
}




/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::~SNDOPENALSYSTEM
*/	
/**	
//	
//	 Class Destructor SNDOPENALSYSTEM
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 11:07:31
//	
*/
/*-----------------------------------------------------------------*/
SNDOPENAL::~SNDOPENAL()
{
	if(streamthread)
		{
			streamthread->End();
			xfactory->DeleteThread(XTHREADGROUPID_UNGROUP, streamthread);
			streamthread = NULL;
		}

	//DeRegisterEvent(XEVENTTYPE_SOUND);
	//
	//sndsystem->SetXSUBJECT(NULL);
	//
	StopAll();

	// destroy all sources, elements
	XDWORD elements = loadedfiles.GetSize();
	for(XDWORD i = 0; i < elements; i++)
		{
			delete loadedfiles.Get(i);
		}


	XDWORD streamers = streamelements.GetSize();
	for(XDWORD i = 0; i < streamers; i++)
		{
			delete streamelements.Get(i);
		}

	XDWORD sourcesnum = sources.GetSize();
	for(XDWORD i = 0; i < sourcesnum; i++)
		{
			if(sources.Get(i)->GetInstance() != NULL)
				{
					delete sources.Get(i)->GetInstance();
					sources.Get(i)->SetInstance(NULL);
				}
			delete sources.Get(i);
		}

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	
	if(streammutex)
		{
			xfactory->Delete_Mutex(streammutex);
		}

	Clean();
}




/*-------------------------------------------------------------------
//	 SNDOPENAL::AddFile
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	21/02/2018 16:57:58
//
//	@return		SNDELEMENT* : 
//
//	@param		XPATH& : 
//	@param		XSTRING* : 
//	@param		bool : 
//
*//*-----------------------------------------------------------------*/
SNDELEMENT* SNDOPENAL::AddFile(XPATH& xpath, XSTRING* namefile, bool stream)
{
	return AddFile(xpath, namefile->Get(), stream);
}



/*-------------------------------------------------------------------
//	 SNDOPENAL::AddFile
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	21/02/2018 16:54:47
//
//	@return		SNDELEMENT* : 
//
//	@param		XPATH& : 
//	@param		XCHAR* : 
//	@param		bool : 
//
*//*-----------------------------------------------------------------*/
SNDELEMENT* SNDOPENAL::AddFile(XPATH& xpath, XCHAR* namefile, bool stream)
{
	XPATH xpathfull;

	xpathfull = xpath;
	xpathfull.Slash_Add();
	xpathfull.Add(namefile);

	if(!stream)
		{
			// create sndfileogg
			SNDOPENALELEMENT* element = (SNDOPENALELEMENT*)GetFile(namefile, stream);
			if(element != NULL)
				{
					//XDEBUG_PRINTCOLOR(0,__L("File already Loaded:  %s"), file);
					return element;
				}

			SNDFILE* fileogg = sndsystem->CreateSNDFile(); // need xfactory in the system
			if(!fileogg)
				{
					return NULL;
				}

			// Change to use any Path in sounds
			//XPATH xpath;
			//xpath.Create(XPATHSMANAGERSECTIONTYPE_SOUNDS, 1 ,file);
			
			// load and decode file
			// for this the system needs to get xpaths
			if(!fileogg->LoadFile(xpathfull, namefile, stream))
				{
					XDEBUG_PRINTCOLOR(4,__L("Couldn't load %s"), xpath.Get());
					delete fileogg;
					return NULL;
				}

			
			// create the element but do not assign source to it
			// upload the data from the fileogg

			element = new SNDOPENALELEMENT(&xpathfull);
			if(!element)
				{
					XDEBUG_PRINTCOLOR(4,__L("Couldn't load %s"), xpathfull.Get());
					delete fileogg;
					return NULL;
				}

			element->SetFile(fileogg);

		
			element->SetDuration(fileogg->GetDuration());

			if (fileogg->GetChannels()==0) 
				{
					XDEBUG_PRINTCOLOR(4,__L(" SNDOPENAL::AddFile: Exception using 0 channels to set samples"), namefile);
				}
			else
				{
					element->SetSamples(fileogg->samples/fileogg->GetChannels());
				}

			
			if (!loadedfiles.Add(element))
				{
					return NULL;
				}

			delete fileogg;

			//XDEBUG_PRINTCOLOR(1,__L("File %s Loaded"), file);

			//return it
			return element;
		}
	else
		{
			SNDOPENALSTREAMELEMENT* streamelement = (SNDOPENALSTREAMELEMENT*)GetFile(namefile, stream);
			
			if(streamelement != NULL)
				{
					return streamelement;
				}

			streamelement = new SNDOPENALSTREAMELEMENT();

			SNDFILE* fileogg = sndsystem->CreateSNDFile();
			if(!fileogg)
				{
					return NULL;
				}

			//XPATH xpath;
			//xpath.Create(XPATHSMANAGERSECTIONTYPE_SOUNDS, 1, namefile);

			// load and decode file
			// for this the system needs to get xpaths
			if(!fileogg->LoadFile(xpathfull, namefile, stream))
				{
					XDEBUG_PRINTCOLOR(4,__L("Couldn't load %s"), namefile);
					delete fileogg;
					return NULL;
				}

			// need to add this to an appropiate list !!
			streamelement->SetFile(fileogg);

			// careful, as I'm treting this array as files being currently played
			streammutex->Lock();
			if (!streamelements.Add(streamelement))
				{
					streammutex->UnLock();
					return NULL;
				}
			streammutex->UnLock();

			return streamelement;
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::GetFile
*/	
/**	
//	
//	gets an existing file
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 11:08:30
//	
//	@return 			SNDOPENALELEMENT* : 
//
//  @param				namefile : 
*/
/*-----------------------------------------------------------------*/
SNDELEMENT* SNDOPENAL::GetFile(XSTRING* namefile, bool stream)
{
	return GetFile(namefile->Get(), stream);
}




/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::GetFile
*/	
/**	
//	
//	gets an existing file
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 11:08:40
//	
//	@return 			SNDOPENALELEMENT* : 
//
//  @param				namefile : 
*/
/*-----------------------------------------------------------------*/
SNDELEMENT* SNDOPENAL::GetFile(XCHAR* namefile, bool stream)
{
	if(!stream)
		{
			XDWORD numberoffiles = loadedfiles.GetSize();
			for(XDWORD i = 0; i < numberoffiles; i++)
				{
					// compare the element file name with the one being requested
					if(loadedfiles.FastGet(i)->GetNameFile()->Compare(namefile) == 0)
						{
							return loadedfiles.Get(i);
						}
				}
		}
	else
		{
			// look for the file on the stream elements
			streammutex->Lock();
			XDWORD numberoffiles = streamelements.GetSize();
			for(XDWORD i = 0; i < numberoffiles; i++)
				{
					// compare the element file name with the one being requested
					if(streamelements.FastGet(i)->GetNameFile()->Compare(namefile) == 0)
						{
							SNDSTREAMELEMENT* ret = streamelements.Get(i);
							streammutex->UnLock();
							return ret;
						}
				}
			streammutex->UnLock();
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	 SNDOPENAL::RemoveFile
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		bool : 
//
//	@param		SNDELEMENT* : 
//
*//*-----------------------------------------------------------------*/
bool SNDOPENAL::RemoveFile(SNDELEMENT* element)
{
	this->StopSound(element);

	deletequeue.Add(element);

	return true;
}


/*-------------------------------------------------------------------
//	SNDOPENAL::GetStreamer
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			12/09/2016 11:16:57
//	
//	@return 			SNDSTREAMELEMENT* : 
//
*/
/*-----------------------------------------------------------------*/
SNDSTREAMELEMENT* SNDOPENAL::GetStreamer()
{
	// create a streaming element and aquire a free source

	SNDOPENALSTREAMELEMENT* streamer = new SNDOPENALSTREAMELEMENT(); // store the streamer somewhere and delete it when destroying openal?
	if(!streamer)
		{
			return NULL;
		}
	
	streammutex->Lock();
	streamelements.Add(streamer);
	streammutex->UnLock();
	return streamer;

	//for(int i = (maxchannels-1); i >= 0; i--)
	//	{
	//		SNDOPENALSOURCE* source = sources.Get(i);
	//		if(!source->IsPLaying())
	//			{
	//				if(!source->GetElement())
	//					{
	//						source->Aquire();
	//						SNDOPENALSTREAMELEMENT* streamer = new SNDOPENALSTREAMELEMENT(); // store the streamer somewhere and delete it when destroying openal?
	//						if(!streamer)
	//							{
	//								return NULL;
	//							}
	//
	//						//streamer->aquiredsource = source;
	//						source->SetElement(streamer);
	//						streamer->SetSource(source);
	//						streamelements.Add(streamer);
	//						return streamer;
	//					}
	//			}
	//	}
	//
	//return NULL;
}



/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::PlaySound
*/	
/**	
//	
//	plays a sound
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 11:08:48
//	
//  @param				element : 
*/
/*-----------------------------------------------------------------*/
SNDINSTANCE* SNDOPENAL::PlaySound(SNDELEMENT* element)
{
	if(!element->IsStreamer())
		{
			// need to know if it's a stream or a regular element to know what to do !!
			SNDOPENALELEMENT* openalelement = (SNDOPENALELEMENT*)element;

			if(!openalelement)
				{
					return NULL;
				}

			for(XDWORD i = 0; i < maxchannels; i++)
				{
					SNDOPENALSOURCE* source = sources.FastGet(i);

					if (source==NULL)
						{
							XDEBUG_PRINTCOLOR(4,__L("SNDOPENAL::PlaySound : Executing a NULL source"));
						}

					if(source->IsStopped())
						{
							if(source->GetElement() == NULL)
								{
									openalelement->SetSource(source);
									source->SetElement(openalelement);
									source->SetVolume(element->GetVolume());
									openalelement->Queue();
									source->SetLoop(openalelement->GetLoop());
									source->SetPitch(openalelement->GetPitch());

									SNDINSTANCE* ins = new SNDINSTANCE(sndsystem, source, openalelement);
									if(!ins) return NULL;
									source->SetInstance(ins);

									SNDEVENT sndevent(this, XEVENTTYPE_SOUND, XEVENTTYPE_SOUND);	// need to make a specific event type for this
									sndevent.SetSNDType(SNDEVENT_TYPE_PLAY);
									ins->HandleEvent(&sndevent);		// direct call to avoid performance penalty

									return ins;
								}
						}
				}
		}
	else
		{
			SNDOPENALSTREAMELEMENT* streamelement = (SNDOPENALSTREAMELEMENT*)element;

			for(XDWORD i = 0; i < maxchannels; i++)
				{
					SNDOPENALSOURCE* source = sources.FastGet(i);

					if (source==NULL)
						{
							XDEBUG_PRINTCOLOR(4,__L("SNDOPENAL::PlaySound : Executing a NULL source"));
						}

					if(source->IsStopped())
						{
							if(source->GetElement() == NULL)
								{
									streamelement->SetSource(source);
									source->SetElement(streamelement);
									source->SetVolume(element->GetVolume());

									source->SetLoop(streamelement->GetLoop());
									source->SetPitch(streamelement->GetPitch());
									source->SetAquired(true);

									SNDSTREAMINSTANCE* streaminstance = new SNDSTREAMINSTANCE(sndsystem, source, streamelement);

									if(!streaminstance) return NULL;
									source->SetInstance(streaminstance);
									streaminstance->SetManaged(false);
									SNDEVENT sndevent(this, XEVENTTYPE_SOUND, XEVENTTYPE_SOUND);	// need to make a specific event type for this
									sndevent.SetSNDType(SNDEVENT_TYPE_PLAY);
									streaminstance->HandleEvent(&sndevent);		// direct call to avoid performance penalty
									
									return streaminstance;
								}
						}
				}			
		}

	if (element)
		if (element->GetNameFile())
		{
				XDEBUG_PRINTCOLOR(4,__L("No available Source : %s"),element->GetNameFile()->Get());
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::StopSound
*/	
/**	
//	
//	stops a sounds
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 16:23:55
//	
//  @param				element : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENAL::StopSound(SNDELEMENT* element)
{
	SNDOPENALELEMENT* sndelement = (SNDOPENALELEMENT*)element;

	if(!sndelement)
		{
			return;
		}

	if(sndelement->GetSource())
		{
			// make sure the element is actually assigned to a source
			if(sndelement->GetSource()->GetElement() == sndelement)
				{
					sndelement->GetSource()->Stop();

					// not sure if deattach source and element here, or wait for update
					SNDEVENT	sndevent(this, XEVENTTYPE_SOUND, XEVENTTYPE_SOUND);	// need to make a specific event type for this
										sndevent.SetSNDElement(element);
										sndevent.SetSNDType(SNDEVENT_TYPE_STOP);
										if(sndelement->GetSource()->GetInstance())
										{
												sndelement->GetSource()->GetInstance()->HandleEvent(&sndevent);		// direct call to avoid performance penalty
												sndelement->GetSource()->SetInstance(NULL);
										}
				}
		}
}



/*-------------------------------------------------------------------
//	 SNDOPENAL::PauseSound
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/01/2018 12:22:18
//	@return		SNDINSTANCE* : 
//
//	@param		SNDELEMENT* : 
//
*//*-----------------------------------------------------------------*/
SNDINSTANCE* SNDOPENAL::PauseSound(SNDELEMENT* element)
{
	SNDOPENALELEMENT* sndelement = (SNDOPENALELEMENT*)element;

	if(!sndelement)
		{
			return NULL;
		}

	if(sndelement->GetSource())
		{
			if(sndelement->GetSource()->GetElement() == sndelement)
				{
					if(sndelement->GetSource()->IsPLaying())
						{
							sndelement->GetSource()->Pause();
							return sndelement->GetSource()->GetInstance();
						}
				}
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::Update
*/	
/**	
//	
//	updates the play queue
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 11:09:44
//	
*/
/*-----------------------------------------------------------------*/
void SNDOPENAL::Update()
{
	for(int i = (maxchannels-1); i >= 0; i--)
		{
			SNDOPENALSOURCE* source = sources.Get(i);

			// an element has exclusive access(probably because of streaming), ignore it
			// but if it's aquired I can't really check when it finished!!

			// need to unify both cases
			if(source->IsAquired() && !source->IsInstancePlaying())
				{
					if(source->GetElement())
						{
							SNDOPENALSTREAMELEMENT* element = (SNDOPENALSTREAMELEMENT*)source->GetElement();

							streammutex->Lock();
							int proc = source->GetProcessedBuffers();
							element->ClearBuffers();
							((SNDOPENALELEMENT*)source->GetElement())->SetSource(NULL);
							proc = source->GetProcessedBuffers();
							

							source->SetLoop(false);
							source->SetPitch(1.0f);

							// need to have stored the quaued buffers to unqueue them
							//XLIST<SNDOPENALBUFFER*>::XITERATOR it;

							

							//for(it = source->bufferlist.Begin(); it != source->bufferlist.End(); it++)
							//	{
							//		source->UnQueue(*it);
							//		(*it)->Destroy();
							//		delete (*it);
							//	}
							
							//source->bufferlist.DeleteAll();

							source->Release();

							if(source->GetElement())
								{
									SNDEVENT sndevent(this, XEVENTTYPE_SOUND, XEVENTTYPE_SOUND);	// need to make a specific event type for this
									sndevent.SetSNDElement(element);

									sndevent.SetSNDInstance(source->GetInstance());
									sndevent.SetSNDElement(element);
									sndevent.SetSNDSource(NULL); // because it's NULL for element
									sndevent.SetSNDType(SNDEVENT_TYPE_STOP);
									PostEvent(&sndevent);
									if(source->GetInstance())
										{
											source->GetInstance()->HandleEvent(&sndevent);		// direct call to avoid performance penalty
											source->SetInstance(NULL);
										}
								}

								source->SetElement(NULL);
								source->Stop();

								if(element->GetFile())
									{
										element->GetFile()->Reset();
									}

								//// send an event telling which sound stopped playing
								//SNDEVENT* event = new SNDEVENT(this, XEVENTTYPE_SOUND, XEVENTTYPE_SOUND);
								//if(event)
								//	{
								//		event->SetSNDType(SNDEVENT_TYPE_STOP);
								//		event->SetSound(element->GetNameFile()->Get());
								//		event->SetSNDElement(element);
								//
								//		PostEvent(event);
								//		
								//		delete event;
								//	}

								streammutex->UnLock();
						}

					continue;
				}

			if(source->IsAquired())
				{
					continue;// need to detect when a streaming instance finishes !!
				}

			// !! important if element is a streamer we need to clear the stored buffers
			if(!source->IsPLaying() && !source->IsAquired()) // also check if not aquired? can help with the checks, but update needs to know when a sound stopped to clean up
			//if(!source->IsPLaying() && !source->IsInstancePlaying())
				{
					if(source->GetElement())
						{
							// check if element is a streamer

							// we need to stop the source
							source->SetLoop(false);
							source->SetPitch(1.0f);
							SNDELEMENT* element = source->GetElement();
							if(element)
								{
									//// clear up the completed queued buffers
									//if(element->IsStreamer())
									//	{
									//		// need to have stored the quaued buffers to unqueue them
									//		XLIST<SNDOPENALBUFFER*>::XITERATOR it;
									//
									//		for(it = source->bufferlist.Begin(); it != source->bufferlist.End(); it++)
									//			{
									//				source->UnQueue(*it);
									//				delete *it;
									//			}
									//	
									//		source->bufferlist.DeleteAll();
									//
									//		source->Release();
									//	}

									((SNDOPENALELEMENT*)source->GetElement())->SetSource(NULL);
								}
							if(source->GetElement())
								{
									SNDEVENT sndevent(this, XEVENTTYPE_SOUND, XEVENTTYPE_SOUND);	// need to make a specific event type for this
									sndevent.SetSNDElement(element);
									if(source->GetInstance())
										{
											sndevent.SetSNDInstance(source->GetInstance());
											sndevent.SetSNDElement(element);
											sndevent.SetSNDSource(NULL);
											sndevent.SetSNDType(SNDEVENT_TYPE_STOP);
											PostEvent(&sndevent);
											source->GetInstance()->HandleEvent(&sndevent);		// direct call to avoid performance penalty
											source->SetInstance(NULL);
										}
								}

							source->SetElement(NULL);
							source->Stop();

							//// send an event telling which sound stopped playing
							//SNDEVENT* event = new SNDEVENT(this, XEVENTTYPE_SOUND, XEVENTTYPE_SOUND);
							//if(event)
							//	{
							//		event->SetSNDType(SNDEVENT_TYPE_STOP);
							//		event->SetSound(element->GetNameFile()->Get());
							//		event->SetSNDElement(element);
							//
							//		PostEvent(event);
							//		
							//		delete event;
							//	}
						}
				}
		}


	XDWORD size = deletequeue.GetSize();
	for(XDWORD i = 0; i < size; i++)
	{
		SNDOPENALELEMENT* e = (SNDOPENALELEMENT*)deletequeue.FastGet(i);
		
		if(e->IsStreamer())
			{
				streammutex->Lock();
				XDWORD index = streamelements.Find((SNDOPENALSTREAMELEMENT*)e);
				if(index != NOTFOUND)
					{
						streamelements.DeleteIndex(index);
						delete e;
					}
				streammutex->UnLock();
			}
		else
			{
				XDWORD index = loadedfiles.Find(e);
				if(index != NOTFOUND)
					{
						loadedfiles.DeleteIndex(index);
						delete e;
					}
			}
	}

	deletequeue.DeleteAll();

	//XDWORD totalstream = streamelements.GetSize();
	//for(XDWORD i = 0; i < totalstream; i++)
	//	{
	//		SNDOPENALSTREAMELEMENT* element = streamelements.FastGet(i);
	//		//SNDSTREAMINSTANCE* instance = streamelements.FastGet(i); // WANT INSTANCES !!
	//		SNDSTREAMINSTANCE* instance = (SNDSTREAMINSTANCE*)element->GetSource()->GetInstance();
	//		if(instance)
	//			{
	//				instance->Update();
	//			}
	//	}

}



/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::IsAnyPlaying
*/	
/**	
//	
//	check if any source is playing
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 11:09:58
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SNDOPENAL::IsAnyPlaying()
{
	for(XDWORD i = 0; i < maxchannels; i++)
		{
			if(sources.Get(i)->IsPLaying())
				{
					return true;
				}
		}


	return false;
}




/*-------------------------------------------------------------------
//	SNDOPENALSYSTEM::StopAll
*/	
/**	
//	
//	stops and clears the queue of all the sounds
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/11/2015 11:33:47
//	
*/
/*-----------------------------------------------------------------*/
void SNDOPENAL::StopAll()
{
	playqueue.DeleteAll();

	for(XDWORD i = 0; i < maxchannels; i++)
		{
			sources.Get(i)->Stop();
		}
}



/*-------------------------------------------------------------------
//	SNDOPENAL::SetMasterVolume
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			14/11/2015 17:18:47
//	
//  @param				mastervolume : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENAL::SetMasterVolume(float mastervolume)
{
	// reset the volume of all sources
	for(XDWORD i = 0; i < maxchannels; i++)
		{
			SNDOPENALSOURCE* source = sources.Get(i);
			float volume;

			if(source)
				{
					alGetSourcef(source->source, AL_GAIN, &volume);
					volume /= this->mastervolume;

					volume *= mastervolume;
					alSourcef(source->source, AL_GAIN, volume);
				}
		}

	this->mastervolume = mastervolume;
}



/*-------------------------------------------------------------------
//	SNDOPENAL::StreamingThread
*/	
/**	
//	
//	thread to decode streaming data
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/12/2016 12:04:12
//	
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
void SNDOPENAL::StreamingThread(void* param)
{
	SNDOPENAL* sndopenal = (SNDOPENAL*)param;

	sndopenal->streammutex->Lock();

	XDWORD totalstream = sndopenal->streamelements.GetSize();
	for(XDWORD i = 0; i < totalstream; i++)
		{
			SNDOPENALSTREAMELEMENT* element = sndopenal->streamelements.FastGet(i);
			//SNDSTREAMINSTANCE* instance = streamelements.FastGet(i); // WANT INSTANCES !!
			if(element->GetSource())
				{
					if(element->GetSource()->IsInstancePlaying()) // but same crash as below, instance is no longer a valid pointer
						{
							SNDSTREAMINSTANCE* instance = (SNDSTREAMINSTANCE*)element->GetSource()->GetInstance();
							if(instance)
								{
									instance->Update();
								}
						}
				}
		}

	sndopenal->streammutex->UnLock();
}

