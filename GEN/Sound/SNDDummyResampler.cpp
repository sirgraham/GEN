
/*------------------------------------------------------------------------------------------
//  SNDDUMMYRESAMPLER.CPP
//
//  dummy sound resampler
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 17/03/2017 12:20:51
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDDummyResampler.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  SNDDUMMYRESAMPLER::SNDDUMMYRESAMPLER
*/
/**
//
//  Class Constructor SNDDUMMYRESAMPLER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/03/2017 12:32:56
//
*/
/*-----------------------------------------------------------------*/
SNDDUMMYRESAMPLER::SNDDUMMYRESAMPLER() : SNDRESAMPLER()
{
  Clean();
}



/*-------------------------------------------------------------------
//  SNDDUMMYRESAMPLER::~SNDDUMMYRESAMPLER
*/
/**
//
//   Class Destructor SNDDUMMYRESAMPLER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/03/2017 12:33:02
//
*/
/*-----------------------------------------------------------------*/
SNDDUMMYRESAMPLER::~SNDDUMMYRESAMPLER()
{
  Clean();
}



/*-------------------------------------------------------------------
//  SNDDUMMYRESAMPLER::Ini
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/03/2017 12:33:09
//
//  @return       bool :
//
//  @param        dstchannels :
//  @param        dstformat :
//  @param        dstsamplerate :
//  @param        srcchannels :
//  @param        srcformat :
//  @param        srcsamplerate :
*/
/*-----------------------------------------------------------------*/
bool SNDDUMMYRESAMPLER::Ini(XQWORD dstchannels, SND_RESAMPLER_FORMAT dstformat, int dstsamplerate, XQWORD srcchannels, SND_RESAMPLER_FORMAT srcformat, int srcsamplerate)
{
  return true;
}



/*-------------------------------------------------------------------
//  SNDDUMMYRESAMPLER::Allocate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/03/2017 12:33:18
//
//  @return       int :
//
//  @param        audiodata :
//  @param        linesize :
//  @param        extradata :
//  @param        format :
//  @param        align :
*/
/*-----------------------------------------------------------------*/
int SNDDUMMYRESAMPLER::Allocate(XBYTE *** audiodata, int* linesize, void* extradata, SND_RESAMPLER_FORMAT format, int align)
{
  *audiodata = NULL;
  return 0;
}



/*-------------------------------------------------------------------
//  SNDDUMMYRESAMPLER::Free
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/03/2017 12:33:24
//
//  @param        audiodata :
*/
/*-----------------------------------------------------------------*/
void SNDDUMMYRESAMPLER::Free(XBYTE ** audiodata)
{
}



/*-------------------------------------------------------------------
//  SNDDUMMYRESAMPLER::Convert
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/03/2017 12:33:31
//
//  @return       int :
//
//  @param        buffer :
//  @param        buffersize :
//  @param        extendeddata :
//  @param        samples :
*/
/*-----------------------------------------------------------------*/
int SNDDUMMYRESAMPLER::Convert(XBYTE** buffer, int buffersize, XBYTE** extendeddata, int samples)
{
  // do nothing
  return 0;
}



/*-------------------------------------------------------------------
//  GetResamplerFormat
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/03/2017 12:33:37
//
//  @return       SND_RESAMPLER_FORMAT :
//
//  @param        nativeformat :
*/
/*-----------------------------------------------------------------*/
SND_RESAMPLER_FORMAT SNDDUMMYRESAMPLER::GetResamplerFormat(int nativeformat)
{
  return SND_RESAMPLER_FORMAT_NONE;
}
