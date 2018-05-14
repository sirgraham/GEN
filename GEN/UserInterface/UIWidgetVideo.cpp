
/*------------------------------------------------------------------------------------------
//  UIWIDGETVIDEO.CPP
//
//  plays a video in the UI
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 27/10/2016 12:58:24
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetVideo.h"
#include "VIDEOPlayer.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//  UIWIDGETVIDEO::UIWIDGETVIDEO
*/
/**
//
//  Class Constructor UIWIDGETVIDEO
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/10/2016 13:20:43
//
//  @param        name :
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UIWIDGETVIDEO::UIWIDGETVIDEO(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETFORM(name, uimanager)
{
  Clean();
  SetType();
}




/*-------------------------------------------------------------------
//  UIWIDGETVIDEO::~UIWIDGETVIDEO
*/
/**
//
//   Class Destructor UIWIDGETVIDEO
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/10/2016 13:20:35
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETVIDEO::~UIWIDGETVIDEO()
{
  Clean();
}



/*-------------------------------------------------------------------
//  UIWIDGETVIDEO::SetVideo
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/10/2016 13:20:30
//
//  @param        video :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETVIDEO::SetVideo(XCHAR* video)
{
  this->video.Set(video);
}


/*-------------------------------------------------------------------
//  UIWIDGETVIDEO::Play
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      31/10/2016 11:13:38
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETVIDEO::Play()
{
  VIDEOPLAYER::GetInstance()->PlayFile(&video);
}



/*-------------------------------------------------------------------
//  UIWIDGETVIDEO::Stop
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      31/10/2016 11:13:45
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETVIDEO::Stop()
{
  VIDEOPLAYER::GetInstance()->Stop();
}
