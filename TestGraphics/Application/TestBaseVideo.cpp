
/*------------------------------------------------------------------------------------------
//	TESTBASEVIDEO.CPP
//	
//	video test file
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 22/06/2016 10:13:30
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBaseVideo.h"

#ifdef VIDEO_ACTIVE
#include "VideoPlayer.h"
#endif

#include "XApplication.h"

#include "GRPScene.h"
#include "GRPViewPort.h"
#include "GRPRenderer.h"

#include "SNDFactory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	TESTBASEVIDEO::TESTBASEVIDEO
*/	
/**	
//	
//	Class Constructor TESTBASEVIDEO
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/06/2016 11:50:03
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASEVIDEO::TESTBASEVIDEO(GRPAPPLICATION* xapplication) : TESTBASE(xapplication)
{
	Clean();
}





/*-------------------------------------------------------------------
//	TESTBASEVIDEO::~TESTBASEVIDEO
*/	
/**	
//	
//	 Class Destructor TESTBASEVIDEO
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/06/2016 11:50:10
//	
*/
/*-----------------------------------------------------------------*/
TESTBASEVIDEO::~TESTBASEVIDEO()
{
	Clean();
}





/*-------------------------------------------------------------------
//	TESTBASEVIDEO::IniResources
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/06/2016 11:50:20
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEVIDEO::IniResources()
{
	PathManagement(__L("Video"));

#ifdef VIDEO_ACTIVE

	GRPSCENE*		 secondaryscene = new GRPSCENE(xapplication->GetMainScreen());
	//GRPVIEWPORT* viewport				= new GRPVIEWPORT();

	secondaryscene->Ini();

	GRPVIEWPORT* viewport = secondaryscene->GetViewport();

	if(!secondaryscene || !viewport) return false;

	// #Imanol : probably not the best way of creating a default second scene //Diego: no it is not, is like the worst option you had
	viewport->SetSize(1.0f, 1.0f);
	viewport->SetPosition(0.0f, 0.0f);
	viewport->SetProjection(GRPPROJECTIONTYPE_ORTHO);
	viewport->SelectProjectionOrtho();
	viewport->Select();
	//secondaryscene->GetViewports()->Add(viewport);
	
	xapplication->GetMainRenderer()->AddScene(secondaryscene);

	videoplayer = new VIDEOPLAYER();

	SNDFACTORY* sndptr = NULL;
#ifdef SND_ACTIVE
	sndptr  = SNDFACTORY::Get();
#endif

	videoplayer->Ini(xapplication->GetMainScreen(), xapplication->GetMainRenderer(), sndptr);
	videoplayer->SetTarget(VIDEOPLAYER_TARGET_FULLSCREEN);
#endif

	return true;
}




/*-------------------------------------------------------------------
//	TESTBASEVIDEO::EndResources
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/06/2016 11:50:30
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEVIDEO::EndResources()
{
#ifdef VIDEO_ACTIVE
	if(videoplayer)
		{
			if(!videoplayer->End())
				{
					return false;
				}

			delete videoplayer;
		}

#endif
	return true;
}





/*-------------------------------------------------------------------
//	TESTBASEVIDEO::Do
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/06/2016 11:50:41
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEVIDEO::Do()
{
#ifdef VIDEO_ACTIVE

	videoplayer->Update();

	if(!videoplayer->IsPlaying())
		{
			// play movie
			XPATH xpath;
			//xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L("bbb.mp4"));
			xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L("bingoHD.mp4"));
			videoplayer->PlayFile(xpath.Get());
		}
#endif

	return true;
}

