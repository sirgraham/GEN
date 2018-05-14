
/*------------------------------------------------------------------------------------------
//  VIDEOLINUXFACTORY.CPP
//
//  LINUX Video Factory
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 11/04/2016 14:07:53
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "VIDEOLINUXFactory.h"

#ifdef HW_RASPBERRYPI
#include "VIDEOLINUXCodecH264ILClient.h"
//#include "VIDEOCodecH264ffmpeg.h"
#endif

#ifdef HW_PC
#include "VIDEOCodecH264ffmpeg.h"
#endif

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  VIDEOLINUXFACTORY::VIDEOLINUXFACTORY
*/
/**
//
//  Class Constructor VIDEOLINUXFACTORY
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/04/2016 15:52:50
//
*/
/*-----------------------------------------------------------------*/
VIDEOLINUXFACTORY::VIDEOLINUXFACTORY(VIDEOPLAYER* videoplayer) : VIDEOFACTORY(videoplayer)
{
}





/*-------------------------------------------------------------------
//  VIDEOLINUXFACTORY::~VIDEOLINUXFACTORY
*/
/**
//
//   Class Destructor VIDEOLINUXFACTORY
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/04/2016 15:52:56
//
*/
/*-----------------------------------------------------------------*/
VIDEOLINUXFACTORY::~VIDEOLINUXFACTORY()
{
}




/*-------------------------------------------------------------------
//  VIDEOLINUXFACTORY::CreateH264Codec
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/04/2016 15:53:03
//
//  @return       VIDEOCODEC* :
//
*/
/*-----------------------------------------------------------------*/
VIDEOCODEC* VIDEOLINUXFACTORY::CreateH264Codec()
{
  #ifdef HW_PC
  // OpenH264, maybe try VDPAU or VA API as first option?
  // we are already writing different code for each platform...
  return new VIDEOCODECH264FFMPEG(videoplayer);
  #endif

  #ifdef HW_ODROIDC1
  // will need to probably use amcodec?
  // seems to be installed in the system and be the decoding framework for amlogic
  https://github.com/linux-meson/libamcodec/tree/master/amcodec
  #endif

  #ifdef HW_RASPBERRYPI
  // OpenMAX, need to consider how standard the pi code is here
  return new VIDEOLINUXCODECH264ILCLIENT(videoplayer);
  // try out ffmpeg on the pi
  //return new VIDEOCODECH264FFMPEG(videoplayer);
  #endif
}
