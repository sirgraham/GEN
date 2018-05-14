
/*------------------------------------------------------------------------------------------
//  VIDEOANDROIDFACTORY.CPP
//
//  android decoders factory
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 14/04/2016 16:43:33
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "VIDEOANDROIDFactory.h"
#include "VIDEOANDROIDCodecH264MediaCodec.h"
#include "VIDEOANDROIDJNICodecH264MediaCodec.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
VIDEOANDROIDFACTORY::VIDEOANDROIDFACTORY(VIDEOPLAYER* videoplayer) : VIDEOFACTORY(videoplayer)
{
}

VIDEOANDROIDFACTORY::~VIDEOANDROIDFACTORY()
{
}

VIDEOCODEC* VIDEOANDROIDFACTORY::CreateH264Codec()
{
  //return new VIDEOANDROIDCODECH264MEDIACODEC(this->videoplayer);
  return new VIDEOANDROIDJNICODECH264MEDIACODEC(this->videoplayer);
}
