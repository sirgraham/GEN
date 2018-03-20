
/*------------------------------------------------------------------------------------------
//	SNDANIMATIONLISTENER.CPP
//	
//	manages sound events on animations
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 13/12/2016 17:02:09
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "SNDManager.h"
#include "sndanimationlistener.h"
#include "XDebug.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SNDANIMATIONLISTENER
*/	
/**	
//	
//	Class Constructor SNDANIMATIONLISTENER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2016 10:14:08
//	
*/
/*-----------------------------------------------------------------*/
SNDANIMATIONLISTENER::SNDANIMATIONLISTENER()
{
	Clean();

	sndfactory=NULL;

	
	sndfactory = SNDFACTORY::Get();
	

}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::~SNDANIMATIONLISTENER
*/	
/**	
//	
//	 Class Destructor SNDANIMATIONLISTENER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/02/2017 17:54:24
//	
*/
/*-----------------------------------------------------------------*/
SNDANIMATIONLISTENER::~SNDANIMATIONLISTENER()
{
	soundgroup.DeleteAll();
	this->Hide();
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::Init
*/	
/**	
//	
//	Initializes sound
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2016 12:12:59
//	
*/
/*-----------------------------------------------------------------*/
bool SNDANIMATIONLISTENER::Init()
{	
	if (this->GetAudioType()!=PLAYLIST)
	{	
		SNDELEMENT* sndelement=sndfactory->GetFile(this->samplename.Get());
		if(!sndelement)
		{
				XPATH soundpath;
				XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SOUNDS, soundpath);
				sndelement=sndfactory->AddFile(soundpath,this->samplename.Get());
				if (!sndelement)
					return false;			
		}
		this->soundgroup.Add(0,sndelement);
		this->SetNextPlaylistItem(0);
		sndelement->SetLoop		(this->loop);
		sndelement->SetVolume	(this->basevolume);	
	}
	
	return true;
}

/*-------------------------------------------------------------------
//	 SNDANIMATIONLISTENER::GetSubjectType
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/11/2017 10:13:11
//	@return		XDWORD : 
//
//	@param		) : 
//
*//*-----------------------------------------------------------------*/
XDWORD SNDANIMATIONLISTENER::GetSubjectType() 
{
		return SND_EVENT; 
}


/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:07:29
//	
//  @param				type : 
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::SetAudioType	(XDWORD type)						
{		
	this->audiotype=(AUDIOTYPE)type;
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::GetType
*/	
/**	
//	
//	Get audio type
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2016 10:31:19
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD	SNDANIMATIONLISTENER::GetAudioType()
{		
	return this->audiotype;
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetLoop
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:07:45
//	
//  @param				loop : 
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::SetLoop	(bool loop)  						
{		
	this->loop = loop;							
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::GetLoop
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:07:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	SNDANIMATIONLISTENER::GetLoop	()											
{		
	return this->loop;							
}	

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetBaseVolume
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:08:02
//	
//  @param				basevolume : 
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::SetBaseVolume	(float basevolume)			
{		
	this->basevolume = basevolume;	
}


/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::GetBaseVolume
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:08:10
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float	SNDANIMATIONLISTENER::GetBaseVolume	()											
{		
	return this->basevolume;				
}


/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetInstance
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:08:17
//	
//  @param				instance : 
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::SetInstance	(SNDINSTANCE* instance)	
{		
	this->sndinstance = instance;			
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::GetInstance
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:08:25
//	
//	@return 			SNDINSTANCE* : 
//
*/
/*-----------------------------------------------------------------*/
SNDINSTANCE*	SNDANIMATIONLISTENER::GetInstance	()											
{		
	return this->sndinstance;					
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetSampleName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2016 12:17:31
//	
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::SetSampleName(XCHAR* name)
{
	this->samplename=name;
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::GetSampleName
*/	
/**	
//	
//	Get Sample Name
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/01/2017 9:50:30
//	
//	@return 			XCHAR* : 
//
*/
/*-----------------------------------------------------------------*/
XCHAR*	SNDANIMATIONLISTENER::GetSampleName()
{
	return this->samplename.Get();
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetPlaylistName
*/	
/**	
//	
//	Set Playlist name
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/01/2017 9:50:40
//	
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::SetPlaylistName	(XCHAR*	name)
{
	this->playlist=name;
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::GetPlaylistName
*/	
/**	
//	
//	Get Playlist name
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/01/2017 9:50:49
//	
//	@return 			XCHAR* : 
//
*/
/*-----------------------------------------------------------------*/
XCHAR*	SNDANIMATIONLISTENER::GetPlaylistName()
{
	return this->playlist.Get();
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetIsRemote
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/01/2017 11:42:07
//	
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::SetIsRemote(bool b)
{
	this->isremote=b;
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetIsLocal
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/01/2017 17:10:17
//	
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::SetIsLocal(bool b)
{
	this->islocal=b;
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::HandleDisable
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:08:33
//	
//	@return 			bool : 
//
//  @param				msg : 
*/
/*-----------------------------------------------------------------*/
bool	SNDANIMATIONLISTENER::HandleDisable	(GRPMSGTYPE msg)				
{
	return true; //for now, we prevent changes to this
	/*
	switch(msg)
	{
	case GRPMSG_ENABLE:		this->disabled = false; break;
	case GRPMSG_DISABLE:	this->disabled = true;  break;
	}
	return false;
	*/
}


/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::Notify
*/	
/**	
//	
//	Manages notifications
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/12/2016 17:34:11
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				subject : 
*/
/*-----------------------------------------------------------------*/
bool	SNDANIMATIONLISTENER::Notify(GRPMSGTYPE		msg,		GRPSUBJECT*		subject)
{
	this->HandleDisable(msg);		
	return true;
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::AddSampleToPlaylist
*/	
/**	
//	
//	Add sample to playlist
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/02/2017 12:03:58
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	SNDANIMATIONLISTENER::AddSampleToPlaylist(XDWORD id, XSTRING* snd)
{
	SNDELEMENT* sndelement=sndfactory->GetFile(snd->Get());
		if(!sndelement)
		{
			XPATH soundpath;
			XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SOUNDS, soundpath);
			sndelement=sndfactory->AddFile(soundpath,snd->Get());
			if (!sndelement)
					return false;			
		}		
	return this->soundgroup.Add(id,sndelement);
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::SetNextPlaylistItem
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/02/2017 12:47:07
//	
//	@return 			bool : 
//
//  @param				next : 
*/
/*-----------------------------------------------------------------*/
bool	SNDANIMATIONLISTENER::SetNextPlaylistItem		(XDWORD next) 
{ 	
		this->playlistItem=next; 

	return true;
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::Play
*/	
/**	
//	
//	PLays the sound
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/01/2017 12:29:02
//	
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::Play()
{
	//XDEBUG_PRINTCOLOR(0,__L("PLAY: %s Item: %d Volume: %f"),this->name.Get(),this->playlistItem,this->basevolume);

		if (Publish(SNDPLAY))				
			if (this->islocal)
			{		
				SNDELEMENT* snd=this->soundgroup.Get(this->playlistItem);
				if (snd)
				{
					snd->SetVolume(this->basevolume);
					this->sndfactory->PlaySound(snd);
				}
			}
		
}

/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::Stop
*/	
/**	
//	
//	Stops the sound
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/01/2017 12:29:44
//	
*/
/*-----------------------------------------------------------------*/
void	SNDANIMATIONLISTENER::Stop()
{		
		if (Publish(SNDSTOP))				
		this->sndfactory->StopSound(this->soundgroup.Get(this->playlistItem));		
}


/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::OnEnter
*/	
/**	
//	
//	Trigger on enter milestone
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2016 10:49:57
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
void SNDANIMATIONLISTENER::OnEnter(XQWORD t)
{
	this->currentpass++;
	//XDEBUG_PRINTCOLOR(2,__L("Pass %s : %f   - %d/%d"),this->name.Get(),this->start_frame,currentpass,passes);
	
	if (currentpass>=this->passes)
	{		
		Publish(GRPMSG_ENTER);
		this->currentpass=0;
		switch(this->audiotype)
		{
		case PLAYLIST:	this->Play();			break;
		case PLAY:			this->Play();			break;
		case ONLYSTOP:	this->Stop();			break;		
		}	
	}
}


/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::OnExit
*/	
/**	
//	
//	Trigger on enter milestone
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2016 10:49:57
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
void SNDANIMATIONLISTENER::OnExit(XQWORD t)
{
		switch(this->audiotype)
		{
		case PLAY:			
			break;

		case ONLYSTOP:			
			break;

		case PLAYANDSTOP:
			this->Stop();
			break;
		}

}


/*-------------------------------------------------------------------
//	SNDANIMATIONLISTENER::OnDisable
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2016 11:13:26
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
void SNDANIMATIONLISTENER::OnDisable(bool		disable)
{
	disabled=false;
}


/*-------------------------------------------------------------------
//	 SNDANIMATIONLISTENER::GetIsRemote
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/11/2017 10:11:13
//	@return		bool : 
//
//	@param		) : 
//
*//*-----------------------------------------------------------------*/
bool SNDANIMATIONLISTENER::GetIsRemote() 
{ 
		return isremote; 
}

/*-------------------------------------------------------------------
//	 SNDANIMATIONLISTENER::GetIsLocal
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/11/2017 10:11:24
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool SNDANIMATIONLISTENER::GetIsLocal()
{
		return islocal; 
}

/*-------------------------------------------------------------------
//	 SNDELEMENT*>* SNDANIMATIONLISTENER::GetSoundGroup
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/11/2017 10:11:28
//	@return		XMAP<XDWORD, : 
//
//	@param		) : 
//
*//*-----------------------------------------------------------------*/
XMAP<XDWORD, SNDELEMENT*>* SNDANIMATIONLISTENER::GetSoundGroup() 
{
		return &this->soundgroup; 
}

/*-------------------------------------------------------------------
//	 SNDANIMATIONLISTENER::GetNextPlaylistItem
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/11/2017 10:11:41
//	@return		XDWORD : 
//
//	@param		) : 
//
*//*-----------------------------------------------------------------*/
XDWORD SNDANIMATIONLISTENER::GetNextPlaylistItem() 
{
		return this->playlistItem; 
}

