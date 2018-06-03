
/*------------------------------------------------------------------------------------------
//  SNDOPENALSTREAMELEMENT.CPP
//
//  openal stream implementation
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 20/07/2016 10:47:28
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDOpenALStreamElement.h"
#include "SNDOpenALBuffer.h"
#include "SNDOpenALSource.h"
#include "SNDFile.h"
#include "SNDCodec.h"

#include "XDebugTrace.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//  SNDOPENALSTREAMELEMENT::SNDOPENALSTREAMELEMENT
*/
/**
//
//  Class Constructor SNDOPENALSTREAMELEMENT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/07/2016 11:22:47
//
*/
/*-----------------------------------------------------------------*/
SNDOPENALSTREAMELEMENT::SNDOPENALSTREAMELEMENT() : SNDSTREAMELEMENT()
{
  Clean();

  //for(XDWORD i = 0; i < 10; i++)
  //  {
  //    SNDOPENALBUFFER* buffer = new SNDOPENALBUFFER();
  //    buffer->Generate();
  //
  //    freebuffers.Add(buffer);
  //  }
}





/*-------------------------------------------------------------------
//  SNDOPENALSTREAMELEMENT::~SNDOPENALSTREAMELEMENT
*/
/**
//
//   Class Destructor SNDOPENALSTREAMELEMENT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/07/2016 11:22:39
//
*/
/*-----------------------------------------------------------------*/
SNDOPENALSTREAMELEMENT::~SNDOPENALSTREAMELEMENT()
{
  if(file)
    {
      delete file;
    }

  ClearBuffers();
  Clean();
}




/*-------------------------------------------------------------------
//  SNDOPENALSTREAMELEMENT::Play
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      05/12/2016 16:55:00
//
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSTREAMELEMENT::Play()
{
  source->Play();
}


/*-------------------------------------------------------------------
//  SNDOPENALSTREAMELEMENT::AddData
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/07/2016 11:22:31
//
//  @param        size :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSTREAMELEMENT::AddData(XDWORD size, void* data)
{
  if(size)
    {
      SNDOPENALBUFFER* buffer = NULL;

      buffer = new SNDOPENALBUFFER();
      buffer->Generate();

      buffer->Upload(channels, data, size, freq);

      duration += ((float)size) / (sizeof(short)*freq);

      if(source)
        {
          source->Queue(buffer);
          queuedbuffers.Add(buffer); // need to clean this up later(maybe in SNDOPENAL::Update if we know it's a streamer?)
        }
    }
}

/*-------------------------------------------------------------------
//  SNDOPENALSTREAMELEMENT::SetFile
*/
/**
//
//  sets the file that this element is going to play
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/11/2015 16:29:14
//
//  @return       bool :
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSTREAMELEMENT::SetFile(SNDFILE* file)
{
  this->file = file;

  //buffer->Upload(file->GetChannels(), file->GetData()->Get(), file->GetData()->GetSize(), file->GetSampleRate());

  //name.Set(file->GetName()->Get());
  namefile.Set(file->GetName()->Get());
  this->channels = file->GetChannels();
  this->freq = file->GetSampleRate();
}



/*-------------------------------------------------------------------
//  SNDOPENALSTREAMELEMENT::ClearBuffers
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      19/01/2017 12:00:33
//
*/
/*-----------------------------------------------------------------*/
void SNDOPENALSTREAMELEMENT::ClearBuffers()
{
  XLIST<SNDOPENALBUFFER*>::XITERATOR it;
  for(it = queuedbuffers.Begin(); it != queuedbuffers.End(); it++)
    {
      SNDOPENALBUFFER* buffer = *it;
      source->UnQueue(buffer);
      buffer->Destroy();
      delete buffer;
    }

  queuedbuffers.DeleteAll();
}
