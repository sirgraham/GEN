
/*------------------------------------------------------------------------------------------
//	TESTBASEFONTTTF.CPP
//	
//	Test Base Audio
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/05/2016 16:19:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XApplication.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"

#include "SNDFactory.h"
#include "SNDElement.h"
#include "SNDSource.h"
#include "SNDStreamElement.h"
#include "SNDInstance.h"

//#include "SNDCodecFFMPEG.h"
#include "MEDIAFrameReader.h"

#include "TestBaseAudio.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/






/*-------------------------------------------------------------------
//	TESTBASEAUDIO::TESTBASEAUDIO
*/	
/**	
//	
//	Class Constructor TESTBASEAUDIO
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 11:17:15
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASEAUDIO::TESTBASEAUDIO(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)
{
	timer = xfactory->CreateTimer();
}




/*-------------------------------------------------------------------
//	TESTBASEAUDIO::~TESTBASEAUDIO
*/	
/**	
//	
//	 Class Destructor TESTBASEAUDIO
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 11:17:27
//	
*/
/*-----------------------------------------------------------------*/
TESTBASEAUDIO::~TESTBASEAUDIO()
{
	xfactory->DeleteTimer(timer);
}





/*-------------------------------------------------------------------
//	TESTBASEAUDIO::IniResources
*/	
/**	
//	
//	initializes audio test resources
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 11:17:42
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEAUDIO::IniResources()
{
	#ifdef SND_ACTIVE

	if(!xapplication) return NULL;
		
	PathManagement(__L("audio"));

	soundsystem = SNDFACTORY::Get();
	if(!soundsystem)
		{
			return false;
		}
	XPATH soundpath;
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SOUNDS, soundpath);
	if(!soundsystem->AddFile(soundpath,__L("Deeper.ogg")))
		{
			XDEBUG_PRINTCOLOR(3, __L("Deeper Load failed"));
			return false;
		}
	
	if(!soundsystem->AddFile(soundpath, __L("miami.ogg")))
		{
			XDEBUG_PRINTCOLOR(3, __L("miami Load failed"));
			return false;
		}
	

	beep = soundsystem->AddFile(soundpath, __L("GAMMD.ogg"));
	if (!beep)
		{
		XDEBUG_PRINTCOLOR(3, __L("GAMMD Load failed"));
			return false;
		}

	SNDELEMENT* sndelement = soundsystem->GetFile(__L("Deeper.ogg"));
	if(!sndelement)
		{
			return false;
		}
	
	sndelement->SetLoop(true);
	
	soundsystem->SetMasterVolume(1.0f);

	soundsystem->PlaySound(sndelement);
	soundsystem->PlaySound(beep);

	codec = NULL;

	#endif

	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEAUDIO::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:28:48
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEAUDIO::EndResources()
{
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEAUDIO::Do
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 11:17:51
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEAUDIO::Do()
{
#ifdef SND_ACTIVE
		/*
	if (timer->GetMeasureSeconds() >=2)
	{
		soundsystem->PlaySound(beep);
		timer->Reset();
	}
	*/
#endif
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASEAUDIO::HandleEvent
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/01/2017 9:31:38
//	
//  @param				xevent : 
*/
/*-----------------------------------------------------------------*/
void TESTBASEAUDIO::HandleEvent(XEVENT* xevent)
{
}
