
/*------------------------------------------------------------------------------------------
//  VIDEOWINDOWSFACTORY.CPP
//
//  windows video factory
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 19/04/2016 17:58:16
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "VIDEOWINDOWSFactory.h"
//#include "VIDEOWINDOWSCodecH264openh264.h"
#include "VIDEOCodecH264ffmpeg.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
VIDEOWINDOWSFACTORY::VIDEOWINDOWSFACTORY(VIDEOPLAYER* videoplayer) : VIDEOFACTORY(videoplayer)
{
}

VIDEOWINDOWSFACTORY::~VIDEOWINDOWSFACTORY()
{
}

VIDEOCODEC* VIDEOWINDOWSFACTORY::CreateH264Codec()
{
  //return new VIDEOWINDOWSCODECH264OPENH264(videoplayer);
  return new VIDEOCODECH264FFMPEG(videoplayer);
}
