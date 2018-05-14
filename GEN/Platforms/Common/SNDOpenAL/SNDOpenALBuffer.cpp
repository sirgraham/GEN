
/*------------------------------------------------------------------------------------------
//  SNDOPENALBUFFER.CPP
//
//  openal data buffer
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 04/11/2015 17:46:14
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDOpenALBuffer.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//  SNDOPENALBUFFER::Generate
*/
/**
//
//  generates an openal buffer
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/11/2015 17:50:21
//
*/
/*-----------------------------------------------------------------*/
void SNDOPENALBUFFER::Generate()
{
  alGenBuffers(1, &buffer);
}





/*-------------------------------------------------------------------
//  SNDOPENALBUFFER::Destroy
*/
/**
//
//  destroy an openal buffer
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/11/2015 17:50:31
//
*/
/*-----------------------------------------------------------------*/
void SNDOPENALBUFFER::Destroy()
{
  alDeleteBuffers(1, &buffer);
}





/*-------------------------------------------------------------------
//  SNDOPENALBUFFER::Upload
*/
/**
//
//  uploadas data to an openal buffer
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/11/2015 17:50:42
//
//  @param        channels :
//  @param        data :
//  @param        size :
//  @param        freq :
*/
/*-----------------------------------------------------------------*/
void SNDOPENALBUFFER::Upload(XWORD channels, void* data, XDWORD size, XWORD freq)
{
  ALenum format = AL_FORMAT_MONO16;

  if(channels == 2)
    {
      format = AL_FORMAT_STEREO16;
    }

  alBufferData(buffer, format, data, size, freq);
}
